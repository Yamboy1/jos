#include "stdlib.h"

#include "page.h"
#include "jassert.h"

void pageInit( struct pageDescriptor * self,
	       void * page,
	       char pageType) {
  
  self->pdPage = page;
  self->pdType = pageType;

}


void pageUnmark(struct pageDescriptor * self) {

  unsigned int i;
  
  J_ASSERT_DEBUG( PAGE_IS_FREELIST(self), 
		  "no point calling clear mark bits");

  for (i = 0; i < MARK_BIT_ARRAY_SIZE; i++) {
    PAGE_pdMarkBits(self)[i] = 0;
  }
}


void * pageMarkSmallObject( struct pageDescriptor * self,
  			 unsigned int offsetInPage ) {

  /*
    Marks the object OFFSETINPAGE bytes into the page
    described by SELF. Checks that such an object exists 
    (it must satisfy our page's alignment rule (multiples
    of pdPageSize)) and that we are actually the descriptor
    for a freelist page. Returns 1 if the object
    passes these checks, 0 if not.
   */

  unsigned short int objectSize;
  unsigned int offsetObject, mbIndex, mbBit;


  objectSize = PAGE_pdObjectSize(self);


  /* 
     I'm not a freelist page so the "ptr" being marked 
     isn't a ptr
  */
  if (self->pdType != PAGE_FREELIST) {
#ifdef GC_DEBUG_MARK    
    printf("not freelist\n");
#endif
    return NULL;
  }


  /*
    The offset isn't aligned correctly, so again, it's
    not a pointer. It wouldn't hurt to skip this, but it
    would make the marking less accurate.
   */

  /*
  if ((offsetInPage % objectSize) != 0) {
#ifdef GC_DEBUG_MARK
    printf("misaligned\n");
#endif
    return NULL;
  }
  */


  /* Calculate the bit to mark .. */ 
  offsetObject = offsetInPage / objectSize;

  mbIndex = offsetObject / (8 * sizeof(MarkBits));
  mbBit   = 1 << (offsetObject % (8 *sizeof(MarkBits)));

#ifdef GC_DEBUG_MARK
  printf( "objsz: %d, offsp: %x, offso: %d, mbIndex: %d, mbBit: %d", 
	  objectSize,
	  offsetInPage,
	  offsetObject,
	  mbIndex,
	  mbBit);
#endif

  J_ASSERT_DEBUG( mbIndex < MARK_BIT_ARRAY_SIZE,
		 "mark index off end");

  /* .. and mark it if not already marked */
  if (PAGE_pdMarkBits(self)[mbIndex] & mbBit) {

#ifdef GC_DEBUG_MARK
    printf("already marked\n");
#endif
    return NULL;
    
  } else {

#ifdef GC_DEBUG_MARK
    printf("marking\n");
#endif

    PAGE_pdMarkBits(self)[mbIndex] |= mbBit;

    return (void *) ((jju32) self->pdPage + (offsetObject * objectSize));

  }

}


void * pageMarkLargeObject( struct pageDescriptor * self, 
			 void * p ) {

  /*
    Mark the object pointed to P. SELF must be the pageDescriptor
    for the page that P is on, and must be a PAGE_OBJECT page.
    Tests:
    - P sits at the start of its page
    - SELF is the start page of a large object
   */

  if (pagePtrOffset(p) != 0) {
#ifdef GC_DEBUG_MARK
    printf("not at start of page\n");
#endif
    return NULL;
  }

  if ( !PAGE_pdOIsHead(self) ) {
#ifdef GC_DEBUG_MARK
    printf("not at start of page\n");
#endif
    return NULL;
  }

  if (PAGE_pdOMarked(self)) {

#ifdef GC_DEBUG_MARK
    printf("already marked\n");
#endif
    return NULL;

  } else {

#ifdef GC_DEBUG_MARK
    printf("marking\n");
#endif
    PAGE_pdOMarked(self) = 1;
    return p;
  }


}



void * pageMark( struct pageDescriptor * self, 
		 void * p ) {

  /*
    Mark the object pointed to by P if it passes various 
    consistency checks. Return 1 if consistency checks passed 
    and the object is not already marked, 0 otherwise. 
    SELF *must* be the pageDescriptor for the
    page P is on.
   */

  J_ASSERT_DEBUG( self->pdPage == pageForPtr(p),
		  "pointer / pageDescriptor mismatch");

  switch (self->pdType) {

  case PAGE_FREELIST:
    return pageMarkSmallObject(self, pagePtrOffset(p));
    break;

  case PAGE_OBJECT:
    return pageMarkLargeObject(self, p);
    break;

  default:
    return NULL;

  }

  /* not reached */
}


unsigned int pageGetObjectSize( struct pageDescriptor * self,
				void * p ) {
  
  /*
    Get the size of the object pointed to by P. SELF *must*
    be P's pageDescriptor.
   */

  J_ASSERT_DEBUG( self->pdPage == pageForPtr(p),
		  "pointer / pageDescriptor mismatch");

  switch (self->pdType) {

  case PAGE_FREELIST:
    return PAGE_pdObjectSize(self);
    break;

  case PAGE_OBJECT:
    return PAGE_pdOSize(self);
    break;

  default:
    J_ASSERT( 0,  /* fail */
	      "page neither Object or Freelist");
    return 0;
    break;

  }  

}


void pageInitLargeObject( struct pageDescriptor * self,
			  unsigned int numpages,
			  unsigned int size ) {

  /*
    Set up NUMPAGES starting at SELF to represent a large object
   */

  unsigned int i;
  struct pageDescriptor * pd = self;

  for (i = 0; i < numpages; i++, pd++) {
    PAGE_pdOIsHead(pd) = (i==0);
    PAGE_pdOMarked(pd) = 0;
    PAGE_pdOSize(pd)   = size;
  }

}


int pageNoSmallObjects(struct pageDescriptor * self) {
    
    /*
      Returns 1 if pd (assumed to be a freelist page)
      has no marked objects, otherwise returns 0.
    */

    unsigned int i;

    J_ASSERT_DEBUG( PAGE_IS_FREELIST(self),
		    "can't test non-freelist page for small objects");

    for (i=0; i<MARK_BIT_ARRAY_SIZE; i++) {
	if (PAGE_pdMarkBits(self)[i])
	    return 0;
    }
    
    return 1;
}


void pagePrint(struct pageDescriptor * self, int full) {

  unsigned int i, j, free;


  /* DEBUG
     This needs to take the different types of page into account
  */
 
  printf( "page P: %p; N: %p\n",
	  self->pdPage,
	  PAGE_pdNext(self));

  if (!full) return;


  for (i = 0; i < MARK_BIT_ARRAY_SIZE; i ++) {
    printf("%d\t%8x\t", i, PAGE_pdMarkBits(self)[i]);

    free = 0;

    for (j = 0; j < 8*sizeof(MarkBits); j++) {

      if (PAGE_pdMarkBits(self)[i] & (1 << j)) {
	printf("1");
      } else {
	printf("0");
	free++;
      }
      if ((j%8) == 7) {
	printf(" ");
      }
    }

    printf("\t%d\n", free);

  }

  pageDumpObjects(self);
}



void pageDumpObjects(struct pageDescriptor * self) {

  unsigned int i, j;
  unsigned int p, cnt;
  char * comma;
  
  printf( "Marked objects on %p (object size %d): ", 
	  self->pdPage,
	  PAGE_pdObjectSize(self));

  comma = "";
  p = 0;
  cnt = 0;

  for (i = 0; i < MARK_BIT_ARRAY_SIZE; i ++) {

    for (j = 0; j < 8*sizeof(MarkBits); j++) {

      if (PAGE_pdMarkBits(self)[i] & (1 << j)) {
	printf( "%s%x", comma, p);
	comma = ", ";
	cnt++;
      } 

      p += PAGE_pdObjectSize(self);

      if (p >= PAGE_SIZE) break;
    }

    if (p >= PAGE_SIZE) break;
  }

  printf(" total: %d\n", cnt);
  
}














#include "stdlib.h"

#include "pager.h"
#include "page.h"
#include "jassert.h"


void pagerInitPageFreelist(struct pager * self) {

  /*
    Set up SELF's free freelist by looping the sentinel block
    back on itself.
   */

  struct pageDescriptor * sentinel = &(self->pgFreePageSentinel);
  PAGE_pdFNext(sentinel) = PAGE_pdFPrev(sentinel) = sentinel;

}


void pagerInit( struct pager * self,
		void * ptr,
		unsigned int size ) {

  /*
    Initialise the pager.
    It will manage SIZE contiguous bytes starting at PTR.
  */
  
  int x;
  struct pageDescriptor * pd;

  jju8 * sdesc, * edesc;
  jju8 * spage, * epage;

  jju8 * pg;

  /* 
     Page alignment not needed at bottom
     self->pgDescStart = (ptr == pageForPtr(ptr)) ?
                        ptr :
                        pageForPtr(ptr) + PAGE_SIZE;

    Force page alignment at the top of the heap,
    and adjust size to take page alignment into account.
  */

  sdesc = (jju8 *) ptr;

  epage = (jju8 *) pageForPtr( sdesc + size );

  size = epage - sdesc;


  /*
    Calculate the dividing line between page descriptors and
    actual usable pages.
  */

  x = size / ((PAGE_SIZE / sizeof(struct pageDescriptor)) + 1);

  spage = (jju8 *) pageForPtr(sdesc + x + PAGE_SIZE);

  // set edesc to be byte above the last valid page descriptor
  edesc = sdesc + 
    sizeof(struct pageDescriptor) * ((epage - spage) / PAGE_SIZE);
 

  J_ASSERT_DEBUG( (edesc - sdesc) / sizeof(struct pageDescriptor) >=
		  (epage - spage) / PAGE_SIZE,
		  "more pages than descriptors");

  /*
    Initialise the page descriptors
  */
  pd = (struct pageDescriptor *) sdesc;
  pg = spage;
  while ( ((jju8 *) pd) < edesc) { 
    
    pageInit(pd, pg, (pg < epage) ? PAGE_FREE : PAGE_INVALID);

    pd++;
    pg += PAGE_SIZE;
  }


  self->pgDescStart = (struct pageDescriptor *) sdesc;
  self->pgDescEnd   = (struct pageDescriptor *) edesc;
  self->pgPageStart = (struct pageDescriptor *) spage;
  self->pgPageEnd   = (struct pageDescriptor *) epage;

  /*
    Set up the freelist of pages.
  */
  pagerInitPageFreelist(self);

  pagerAddContiguousPages( self,
			   (struct pageDescriptor *) sdesc,
			   self->pgDescEnd - self->pgDescStart );

}


static void pagerSetPageType( struct pageDescriptor * pd,
			      unsigned int num,
			      char type) {

  /* 
     Set the type of NUM pages starting at and including PD to TYPE.
  */

  unsigned int i;

  J_ASSERT_DEBUG( PAGE_TYPE_IS_VALID(type),
		  "duff type");

  for (i = 0; i < num; i++, pd++) {

    pd->pdType = type;
  }

}


static void pagerAddPagesAfter( struct pageDescriptor * pdPrev,
				struct pageDescriptor * pd ) {

  /*
    Adds the block of pages PD after PDPREV in the freelist of pages. 
   */

  struct pageDescriptor * pdNext = PAGE_pdFNext(pdPrev);

  PAGE_pdFNext(pdPrev) = PAGE_pdFPrev(pdNext) = pd;

  PAGE_pdFPrev(pd) = pdPrev;
  PAGE_pdFNext(pd) = pdNext;

}


void pagerAddContiguousPages( struct pager * self,
			      struct pageDescriptor * pd,
			      unsigned int numpages ) {

  /*
    Adds NUMPAGES pageDescriptors, starting at PD, to the
    page freelist. Doesn't attempt to perform any coalescing
    with other free pages on either side.
   */

  pagerSetPageType( pd, numpages, PAGE_FREE);

  PAGE_pdFCount(pd) = numpages;

  /* add pages to the end of the freelist */
  pagerAddPagesAfter( PAGE_pdFPrev(&(self->pgFreePageSentinel)),
		      pd );

}


void pagerUnmarkLargeObjects(struct pager * self) {
  
  /*
    Unmark all large objects on the heap.

    DEBUG this has to be woefully inefficient - maintain a linked list
    of large objects for this purpose.
   */

  struct pageDescriptor * pd;

  for( pd = self->pgDescStart; pd < self->pgDescEnd; pd++) {
    if (PAGE_IS_OBJECT(pd) && PAGE_pdOIsHead(pd)) {
      PAGE_pdOMarked(pd) = 0;
    } 
  }
}


void pagerDumpLargeObjects(struct pager * self) {

  /*
    Dump details of all large objects on the heap.
   */

  struct pageDescriptor * pd;

  printf("Large objects:\n");

  for( pd = self->pgDescStart; pd < self->pgDescEnd; pd++) {
    if (PAGE_IS_OBJECT(pd) && PAGE_pdOIsHead(pd)) {
      printf("%p (size %d)\n", pd->pdPage, PAGE_pdOSize(pd));
    } 
  }
}


struct pageDescriptor * pagerGetContiguousPages( struct pager * self,
						 unsigned int num,
						 char type ) {

  /*
    Get NUM contiguous pages from SELF. Sets the type of the pages
    to TYPE. Returns the *page descriptor* of the first page in the 
    block of pages.

    Note that this function, pagerRebuildFreelist and pagerAddContiguousPages
    work together to determine the policy used by this allocator ie which
    blocks to use when. Currently set up as address ordered first fit.
   */

  struct pageDescriptor * sentinel, * pd;

  J_ASSERT_DEBUG( PAGE_TYPE_IS_VALID(type),
		  "duff type");

  J_ASSERT_DEBUG( num > 0, "request for 0 pages");


  sentinel = &(self->pgFreePageSentinel);
  pd = PAGE_pdFNext(sentinel);

  /* use first fit to try to find a block with >= num pages */
  while (1) {
    
    /* page freelist finished -  can't get required number of pages */
    if (pd == sentinel) return NULL;

    J_ASSERT_DEBUG( PAGE_IS_FREE(pd),
		    "page on page freelist isn't of type FREE" );

    if (num <= PAGE_pdFCount(pd)) break;

    pd = PAGE_pdFNext(pd);
  }

  J_ASSERT_DEBUG( (pd != sentinel) && (PAGE_pdFCount(pd) >= num),
		  "paranoia test" );

  if (num < PAGE_pdFCount(pd)) {

    /* cut required number of pages off top 
       - can't use pagerAddPagesAfter cos need to cut pd out */
    struct pageDescriptor * pdNext = PAGE_pdFNext(pd),
                          * pdPrev = PAGE_pdFPrev(pd),
                          * newpd  = pd + num;

    PAGE_pdFNext(pdPrev) = newpd;
    PAGE_pdFPrev(pdNext) = newpd;
    PAGE_pdFNext(newpd)  = pdNext;
    PAGE_pdFPrev(newpd)  = pdPrev;

    PAGE_pdFCount(newpd) = PAGE_pdFCount(pd) - num;

    /* cut the required number of pages off the end 
    unsigned int pagesRemaining = PAGE_pdFCount(pd) - num;

    PAGE_pdFCount(pd) = pagesRemaining;
    pd += pagesRemaining;
    */

  } else {

    /* exact match - remove block from the page freelist */
    struct pageDescriptor * pdNext = PAGE_pdFNext(pd),
                          * pdPrev = PAGE_pdFPrev(pd);

    PAGE_pdFNext(pdPrev) = pdNext;
    PAGE_pdFPrev(pdNext) = pdPrev;

  }

  pagerSetPageType(pd, num, type);

  return pd;

}



struct pageDescriptor * pagerDescriptorForPage( struct pager * self,
						void * p ) {
  
  /*
    Given a pointer P return the pageDescriptor for the 
    page P lies on. Returns NULL if P isn't on a page 
    managed by SELF.
   */

  struct pageDescriptor * pd;
  unsigned int offset;

  if ((p < self->pgPageStart) || (p >= self->pgPageEnd)) {
    return NULL;
  }

  offset =  ((unsigned int) ((jju8 *)p - (jju8 *)self->pgPageStart)) >> LOG_PAGE_SIZE;

  pd  = self->pgDescStart + offset;

  J_ASSERT_DEBUG( (pd >= self->pgDescStart) && (pd < self->pgDescEnd),
		  "page descriptor out of range");

  /*
  J_ASSERT_DEBUG( ((pd - self->pgDescStart) % sizeof(struct pageDescriptor)) == 0,
		  "page descriptor not correctly aligned" );
  */

  J_ASSERT_DEBUG( pd->pdPage == pageForPtr(p),
		  "page descriptor wrong for pointer" );

  return pd;

}




void pagerPrint( struct pager * self ) {

  /*
    Print the pager for debugging.
  */
  struct pageDescriptor * sentinel, * pd;
  
  printf("start pds:\t%p\tend pds:\t%p\t(%d page descriptors)\n", 
	  self->pgDescStart, 
	  self->pgDescEnd,
	  (self->pgDescEnd - self->pgDescStart));

  printf("start heap:\t%p\tend heap:\t%p\t(%d pages)\n", 
	  self->pgPageStart,
	  self->pgPageEnd, 
	  ((jju8 *)self->pgPageEnd - (jju8 *)self->pgPageStart) / PAGE_SIZE);

  printf("Page freelist:\n");
  sentinel = &(self->pgFreePageSentinel);
  pd = PAGE_pdFNext(sentinel);

  while (pd != sentinel) {
    printf("%p: %d pages\n", pd->pdPage, PAGE_pdFCount(pd));
    pd = PAGE_pdFNext(pd);
  }
}


#define FREE_AFTER_MARK(pd) ( PAGE_IS_FREE(pd) || (PAGE_IS_OBJECT(pd) && !PAGE_pdOMarked(pd)) )

struct pageDescriptor * pagerChompUnfree(struct pageDescriptor * pd,
					 struct pageDescriptor * stoppd) {
  /*
    Chomp unfree pages from PD up to, but not including stoppd.
   */
  
  while (pd < stoppd)  {
    switch(pd->pdType) {

    case PAGE_FREELIST:
    case PAGE_MARKSTACK:
      pd++;
      break;

    case PAGE_OBJECT:
      if (PAGE_pdOMarked(pd)) 
	pd += (PAGE_pdOSize(pd) >> LOG_PAGE_SIZE) + 1;
      else
	return pd; 
      break;
      
    default:
      return pd;
    }

  }

  return pd;
}


void pagerRebuildFreelist( struct pager * self ) {

  /*
    Scan the heap to rebuild the page freelist from the PAGE_FREE 
    and unmarked PAGE_OBJECT pages.
  */
 
  struct pageDescriptor * pd, * pdStartFree;

  pagerInitPageFreelist(self);

  pd = pagerChompUnfree(self->pgDescStart, self->pgDescEnd);

  while (pd < self->pgDescEnd) {

    J_ASSERT_DEBUG( FREE_AFTER_MARK(pd),
		    "expected at least one free page");

    pdStartFree = pd;

    while ((pd < self->pgDescEnd) && FREE_AFTER_MARK(pd)) {
      pd++;
    }

    J_ASSERT_DEBUG(pdStartFree != pd, "expected at least one free page");

    pagerAddContiguousPages( self,
			     pdStartFree,
			     pd - pdStartFree);

   J_ASSERT_DEBUG( (pd >= self->pgDescEnd) || !FREE_AFTER_MARK(pd), 
		  "expected at least one unfree page");

    pd = pagerChompUnfree(pd, self->pgDescEnd);
  }
 
  J_ASSERT_DEBUG( pd == self->pgDescEnd,
		  "pd overshot end of page descriptors");
}


void pagerPrintFreePages(struct pager * self) {

  struct pageDescriptor * sentinel = &(self->pgFreePageSentinel);
  struct pageDescriptor * pd = PAGE_pdFNext(sentinel);
  struct pageDescriptor * pd2;

  while (pd != sentinel) {
    printf("%sp%x\t%x\n", PAGE_IS_FREE(pd)?"":"!!!", pd->pdPage, PAGE_pdFCount(pd));
    pd2 = pd + 1;
    while (pd2 < pd + PAGE_pdFCount(pd)) {
      printf("%sp%x\n",   PAGE_IS_FREE(pd2)?"":"!!!", pd2->pdPage);
      pd2++;
    }
    pd = PAGE_pdFNext(pd);
  }
}


#ifdef NOTDEF
void pagerFindInHeap( struct pager * self,
		      jju32 thing) {

  void * p;

  for( p = self->pgPageStart; p<self->pgPageEnd; p++) {
    if (thing == *(jju32 *)p) {
      printf("Found %x at %x", thing, p);
    }
  }
}
#endif


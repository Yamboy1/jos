/*
  heap.c
  
  A garbage collected heap.


  Copyright (C) George Marrows 1999
 */

#include "stdlib.h"

#include "heap.h"
#include "freelist.h"
#include "jassert.h"
#include "page.h"
#include "pager.h"
#include "markstack.h"


/*
  Descriptor for a heap. 
*/

struct heapDescriptor {

  /*
    The pager object that looks after pages and page descriptors
  */

  struct pager hdPager;

  /*
    The freelists managed by this heap.
  */
  
  struct freelistDescriptor hdFreelists[MAX_FREELISTS];

  unsigned int hdFreelistCount;


  /*
    Map from size of block required to freelist to use.
   */

  struct freelistDescriptor * hdFreelistMap[MAX_FREELIST_OBJ_SIZE+1];

  /*
    The markstack
   */
  struct markstack hdMarkstack;

  /*
    Callbacks
   */
  struct heapCallbacks hdCallbacks;

  /*
    Whether in mark phase or not.
  */
  char hdInMarkPhase;
};



/*
  The heap.
  DEBUG It's v. unlikely this should actually be statically 
  allocated like this.
 */

static struct heapDescriptor heap;



void heapInit( void * ptr, 
	       unsigned int size,
	       int freelistSizes[MAX_FREELISTS+1],
	       struct heapCallbacks callbacks) {

  /*
    Initialise the global heap object.
    It will manage SIZE contiguous bytes starting at PTR.
    FREELISTSIZES is an array of sizes for the freelists
    (there might be some memory usage trade-offs to be made).
    There size must be in increasing order, ending with 
    MAX_FREELIST_OBJ_SIZE followed by zero.
   */
  

  int i, mapi;
  int sz, prev_sz;

  struct freelistDescriptor * fl;

  /*
    Initialise the pager
   */

  pagerInit( &heap.hdPager, ptr, size );

  /*
    Create the freelists
  */
  prev_sz = 0;
  mapi = 0;
  i = 0;
  while (i < MAX_FREELISTS + 1) {
    
    if ((sz = freelistSizes[i]) == 0) {
      break;
    }

    J_ASSERT( sz > prev_sz,
	      "freelist sizes must increase");

    J_ASSERT( sz <= MAX_FREELIST_OBJ_SIZE,
	      "freelist obj size greater than max");

    J_ASSERT( sz >= MIN_OBJECT_SIZE,
	      "freelist obj size less than min" );
    
    fl = &heap.hdFreelists[i];
      
    freelistInit( fl, sz );

    #ifdef NOTDEF
    /*
      Give the freelist an initial page. 
      DEBUG should this be fetched on demand when first required?
      15/10/99 - they are now!
    */

    pd = pagerGetContiguousPages( &heap.hdPager, 
				  (unsigned int) 1,
				  PAGE_FREELIST);

    J_ASSERT( pd != NULL, "not enough mem to even set up freelists!")

    freelistAddPage( fl, pd );

    freelistStartScan( fl );
   #endif

    while (mapi <= sz) {
      heap.hdFreelistMap[mapi++] = fl;
    }
    
    prev_sz = sz;
    i++;
  }

  heap.hdFreelistCount = i;

  /* lazy! should write code to add this to the end
    of freelistSizes if not present. */
  J_ASSERT( freelistSizes[--i] == MAX_FREELIST_OBJ_SIZE,
	    "last freelist size must be MAX_FREELIST_OBJ_SIZE");

  /* 
     Record the callbacks.
  */
  heap.hdCallbacks = callbacks;

  /*
    Initialise the markstack.
  */
  markstackInit( &(heap.hdMarkstack), &(heap.hdPager) ); 

  // heapPrint();
}



static int heapOnPageFreelist(struct pageDescriptor * pd) {

  /*
    Returns 1 if pd is on the page freelist, 0 otherwise
  */

  struct pageDescriptor * p, * sentinel, *prev;

  sentinel = &heap.hdPager.pgFreePageSentinel;
  p = PAGE_pdFNext(sentinel);
  while (p != sentinel) {
    if ((p <= pd) && (pd < p + PAGE_pdFCount(p))) return 1;
    prev = p;
    p = PAGE_pdFNext(p);
  }
  
  return 0;
}


static int heapOnFreelist(struct pageDescriptor * pd, int i) {

  /*
    Returns 1 if PD is on the object freelist I, 0 otherwise
  */

  struct pageDescriptor * p;

  p = heap.hdFreelists[i].flHead;
  while (p != NULL) {
    if (p == pd) 
      return 1;
    p = PAGE_pdNext(p);
  }
  
  return 0;
}


static void heapValidate() {

  /*
   *Thorough* cross-check that every page is where it should be.
   */

  struct pageDescriptor * pd = heap.hdPager.pgDescStart;
  unsigned int i;

  //return;

  while (pd < heap.hdPager.pgDescEnd) {

    switch (pd->pdType) {

    case PAGE_FREELIST:
      for (i = 0; i < heap.hdFreelistCount; i++ ) {
	if (PAGE_pdObjectSize(pd) == heap.hdFreelists[i].flObjectSize) {
	  J_ASSERT_DEBUG( heapOnFreelist(pd, i),
			  "Validate: object page NOT on object freelist\n");
	} else {
	  J_ASSERT_DEBUG( !heapOnFreelist(pd, i),
			  "Validate: object page on wrong size object freelist\n");
	}
      }      
      
      J_ASSERT_DEBUG( !heapOnPageFreelist(pd), 
		      "Validate: object page on page freelist\n");
      

      break;

    case PAGE_FREE:
      for (i = 0; i < heap.hdFreelistCount; i++ ) {
	J_ASSERT_DEBUG( !heapOnFreelist(pd, i),
			"Validate: free page on object freelist\n");
      }

      J_ASSERT_DEBUG( heapOnPageFreelist(pd), 
		      "Validate: free page NOT on page freelist\n");
      break;
    }

    pd++;
  }
  

}




void heapPrint() {

  /*
    Prints various bits of info about the heap for debugging.
   */


  int fli, i, starti;
  struct freelistDescriptor * fl;

  pagerPrint( &heap.hdPager );

  printf("heap freelists:\n");

  for (fli = 0; fli < MAX_FREELISTS; fli++){
    fl = &heap.hdFreelists[fli];
    
    freelistPrint(fl, 0);

    printf("sizes mapping to this freelist: ");

    i = 0;
    while (i <= MAX_FREELIST_OBJ_SIZE) {
      while ((i <= MAX_FREELIST_OBJ_SIZE) && (heap.hdFreelistMap[i] != fl))
	i++;
      starti = i;
      while ((i <= MAX_FREELIST_OBJ_SIZE) && (heap.hdFreelistMap[i] == fl))
	i++;
      if (starti <= MAX_FREELIST_OBJ_SIZE)
	printf("%d-%d ", starti, i-1);
    }
    printf("\n");

    if (fl->flObjectSize == MAX_FREELIST_OBJ_SIZE)
      break;
  }


}


static void * heapAllocFromFreelist( struct freelistDescriptor * fl,
				     unsigned int size ) {

  /*
    Get a free block from freelist FL, adding a new page if necessary
    (and one can be got). SIZE is only used for debug checks.
   */

  struct pageDescriptor * pd;
  void * res;


  /* try to get from existing freelist pages */
  res = freelistAlloc(fl, size);

  if (res) return res;

  /* failing that, add a new page, returning NULL if one can't be found */
  pd = pagerGetContiguousPages( &heap.hdPager, 1, PAGE_FREELIST );
  //heapValidate();

  if (!pd) return NULL;
  
  freelistAddPageForScan(fl, pd);

  if (((int) pd) == 0x4016e928)
    heapValidate();

  /* .. and try again */  
  return freelistAlloc(fl, size);

}


static void * heapAllocLargeObject( unsigned int size ) {

  /*
    Get enough pages to hold SIZE bytes.
   */

  struct pageDescriptor * pd;
  unsigned int numpages = (size >> LOG_PAGE_SIZE) + 1;

  pd = pagerGetContiguousPages( &heap.hdPager, 
				numpages,
				PAGE_OBJECT );
  //heapValidate();

  if (pd) {
    pageInitLargeObject( pd, numpages, size );
    // printf("allocating large object %p\n", pd->pdPage);
    return pd->pdPage;
  } else {
    return NULL;
  }
}


void * heapAlloc(unsigned int size) {

  /*
    Get a free block of SIZE bytes from the heap.
    Performs a GC if necessary to obtain more memory.
    Returns NULL if no memory available.
   */

  void * res;

  /* decaf seems keen on getting 0 sized blocks of memory -
     removed this assertion to keep it happy */
  //  J_ASSERT( size > 0, "size");

  if (size <= MAX_FREELIST_OBJ_SIZE) {
    
    /* Get off a freelist .. */
    struct freelistDescriptor * fl = heap.hdFreelistMap[size];

    res = heapAllocFromFreelist(fl, size);
    
    if (res) return res;

    /* .. no luck - GC and try again */
    kprintf( "heap.c: collecting garbage...\n" );
    heapGC();

    /* .. a NULL here means out of memory */
    return heapAllocFromFreelist(fl, size);

  } else {
    
    /* Large object - give it pages of its own .. */
    res = heapAllocLargeObject( size );

    if (res) return res;

    /* .. no luck - GC and try again */
    kprintf( "heap.c: collecting garbage...\n" );
    heapGC();

    /* .. a NULL here means out of memory */
    return heapAllocLargeObject( size );

  }

}



void heapMark(void * p) {
  
  /* 
     Tries to mark the pointer P. If succeeds, and P is not already
     marked, pushes P onto the markstack.

     Reasons for not marking P:
     - P not in heap
     - P not on a freelist page
     - P misaligned on a freelist page
  */

  struct pageDescriptor * pd;

  J_ASSERT_DEBUG( heap.hdInMarkPhase, 
		  "not in mark phase");

#ifdef GC_DEBUG_MARK
  printf("%p .. ", p);
#endif

  /* get page descriptor */
  pd = pagerDescriptorForPage( &(heap.hdPager), p );

  /* mark relevant bit in bitmap */
  if (pd) {

    if ((p =  pageMark(pd, p)) ) {
      markstackPush( &(heap.hdMarkstack), p );
    }

  } else {

#ifdef GC_DEBUG_MARK
    printf("not in heap\n");
#endif

  } 
    
}


void heapMarkBlock(void * start, void * end) {

  /*
    Scan the memory between START (inclusive) and
    END (exclusive) for heap pointers.

    !! NB NB NB !!
    This function now assumes that the pointers are sizeof(void **) aligned 
    relative to START. If this causes problems because it misses pointers,
    use the old NOTDEF'd code, which check out every possible
    byte-aligned pointer between START and END.
   */

  void ** p;

  for( p = (void **) start; p < (void **) end; p++) {
    heapMark( *p);
  }

#ifdef NOTDEF
  for (i = 0; i <= ((jju32) end) - ((jju32) start) - sizeof(void *); i++) {
   
#ifdef GC_DEBUG_MARK 
    printf("\toffset %8x contains ", i);
#endif    

    p = * ((void **) ( ((jju8 *) start) + i));
    heapMark(p);
    
  }
#endif


}


void heapDoMarkPhase() {

  /*
    Mark the reachable objects.
    GETROOTS is called to get the roots for this search.
    It should call heapMark for each root in turn.
   */

  struct markstack * ms = &(heap.hdMarkstack);
  struct pageDescriptor * pd;
  void * found;


  heap.hdInMarkPhase = 1;

  markstackReset( ms, &(heap.hdPager) );

  heap.hdCallbacks.getRoots();


  while ((found = markstackPop(ms)) != NULL) {

      // printf("p %x\n", found);

#ifdef GC_DEBUG_MARK
    printf("\nLooking at %p\n", found);
#endif

    /* DEBUG
       Having to refetch the pd here is naff - we
       had this info when we pushed the object
    */

    pd = pagerDescriptorForPage( &(heap.hdPager), found);

    J_ASSERT_DEBUG( pd != NULL,
		    "got invalid page desc for object on markstack");

    heapMarkBlock( found,
		   ((jju8 *) found) + pageGetObjectSize(pd, found) );

  }

  J_ASSERT_DEBUG( !(ms->msOverflow),
		  "heap marking can't cope with markstack overflow (yet)");

  heap.hdInMarkPhase = 0;
  
}


static void heapUnmark() {
  
    /*
      Ensure all mark bits clear prior to GC.
    */
    
    struct freelistDescriptor * fl;
    unsigned int i;
    
    for (i = 0; i < heap.hdFreelistCount; i++ ){
	fl = &heap.hdFreelists[i];
	freelistUnmark(fl);
    }
    
    pagerUnmarkLargeObjects(&heap.hdPager);
}


void heapDumpObjects() {
    
    /*
      Dump details of marked objects for debugging.
     */
    struct freelistDescriptor * fl;
    unsigned int i;
    
    for (i = 0; i < heap.hdFreelistCount; i++ ){
	fl = &heap.hdFreelists[i];
	freelistDumpObjects(fl);
    }
    
    pagerDumpLargeObjects(&heap.hdPager);
}


void heapRemoveEmptyFreelistPages() {
    
  /*
    Ask each small object freelist to drop any pages it has
    which don't contain any marked objects.
   */

  unsigned int i;
  
  for (i = 0; i < heap.hdFreelistCount; i++ ) {
    freelistRemoveEmptyPages(&heap.hdFreelists[i]);
  }
}


void heapRestartFreelists() {
    
    struct freelistDescriptor * fl;
    unsigned int i;
    
    for (i = 0; i < heap.hdFreelistCount; i++ ){
	fl = &heap.hdFreelists[i];
	freelistStartScan(fl);
    }
    
}


void heapPrintFreelistPages() {
    
    /*
      Dump details of small object pages for debugging.
     */

    unsigned int i;

    for (i = 0; i < heap.hdFreelistCount; i++ ) {
	struct pageDescriptor * pd = heap.hdFreelists[i].flHead;

	while (pd != NULL) {
	  printf("f%x\n", pd->pdPage);
	  pd = PAGE_pdNext(pd);
	}
    }
}





void heapGC() {
    
    /*
      Perform a garbage collection.
    */
    
    static int gccount;
    
    kprintf("*** Starting gc #%d ***\n", gccount);

    heapUnmark();
    heapDoMarkPhase();
    heapRemoveEmptyFreelistPages();
    heapRestartFreelists();
    pagerRebuildFreelist(&heap.hdPager);

    kprintf("*** Finished gc #%d ***\n", gccount++);
    
}










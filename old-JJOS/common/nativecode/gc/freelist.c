
#include "stdlib.h"


#include "jassert.h"
#include "page.h"
#include "freelist.h"




void freelistInit(struct freelistDescriptor * self,  
			 unsigned int objectSize) {

  self->flObjectSize = objectSize;

  self->flHead = self->flTail = NULL;
  
  freelistStartScan(self);

}


void freelistScanNewPage( struct freelistDescriptor * self,
			  struct pageDescriptor * page ) {

  /*
    Move on to scan new page PAGE.
    Resets object slot cache, scan index etc.
   */

#ifdef DEBUG
  int i;

  /*
    Only the flCache[0] setting below is strictly necessary.
    Not too clear that this helps debugging anyway.    
  */
  for ( i = 0; i < FREELIST_CACHE_SIZE; i++) {
    self->flCache[i] = NULL;
  }
#endif

  self->flScanPd      = page;
  self->flScanPdIndex = 0;
  
  self->flCache[0]   = NULL;
  self->flCacheIndex = 0;

}


void freelistStartScan(struct freelistDescriptor * self) {

  /*
    Called to start scanning this freelist from the beginning.
    (ie after the mark phase has completed).
   */

  freelistScanNewPage(self, self->flHead);
}



void freelistAddPage( struct freelistDescriptor * self,
		      struct pageDescriptor * pd ) {
  
  /*
    Add a new page descriptor to self.
    Page descriptor must be initialised.
   */

  J_ASSERT_DEBUG( PAGE_IS_FREELIST(pd),
		  "not freelist page");

  pageUnmark(pd);

  if (self->flHead == NULL) {
    J_ASSERT_DEBUG( self->flTail == NULL,
		    "no head but has a tail");

    /* first page on the list */
    self->flHead = self->flTail = pd;
    
  } else {

    /* add to end of list */
    PAGE_pdNext(self->flTail) = self->flTail = pd;

  }

  PAGE_pdNext(pd) = NULL;

  PAGE_pdObjectSize(pd) = self->flObjectSize;
}


void freelistAddPageForScan( struct freelistDescriptor * self,
			     struct pageDescriptor * pd ) {
  freelistAddPage(self, pd);
  freelistScanNewPage(self, pd);

}


void freelistPrint(struct freelistDescriptor * self, int full) {

  /*
    General debuggery-type freelist printer.
   */

  printf("freelist Size: %d; SPD: %p; SPDI: %d\n", 
	 self->flObjectSize,
	 self->flScanPd,
	 self->flScanPdIndex);

  if (full) {
    unsigned int i;

    printf("cache: (curr index %d)\n", self->flCacheIndex);
    for (i = 0; (i < FREELIST_CACHE_SIZE); i++) {
      printf("%s%p\t", (i == self->flCacheIndex)?"->":"",self->flCache[i]);
      if (((i+1) % 4) == 0) {
	printf("\n");
      }
    }

    printf("\n");



  }
}



#define FL_SUCCESS 0
#define FL_PAGE_FINISHED -1

static int freelistReplenishCache(struct freelistDescriptor * self) {

  /*
    Replenish my cache of free object slots.
    Works only within the current page.
   */


  MarkBits markbits;
  int scani;
  unsigned int b;
  int cachei;
  jju8 * freeslot, * maxfreeslot;


  // printf("frc size: %d\n", self->flObjectSize);



  J_ASSERT_DEBUG( self->flCache[self->flCacheIndex - 1] == NULL, 
		  "cache not empty");

  J_ASSERT_DEBUG( self->flScanPd != NULL, 
		  "no scan page");
    
  J_ASSERT_DEBUG( PAGE_IS_FREELIST(self->flScanPd),
		  "not a freelist page");


  // DEBUG this should go on freelistDescriptor as a constant
  maxfreeslot = 
    ((jju8 *)  self->flScanPd->pdPage) + PAGE_SIZE - self->flObjectSize;

  scani = self->flScanPdIndex;

  // DEBUG this could be cached on freelistDescriptor from last time
  freeslot = 
    ((jju8 *) self->flScanPd->pdPage) +
    (scani * 8 * sizeof(MarkBits) * self->flObjectSize);


  while (freeslot <= maxfreeslot) {
    
    markbits = PAGE_pdMarkBits(self->flScanPd)[scani];

    cachei = 0;

    b = 1;
    do {
      
      // if object slot corresponding to b is free ..
      if ( !(markbits & b) ) {
	
	// .. cache the free slot
	self->flCache[cachei++] = freeslot;
	
      }
      
      b = b << 1;
      freeslot += self->flObjectSize;

    } while ((b) && (freeslot <= maxfreeslot));

    scani++;

    if (cachei != 0) {

      /*
	found some free spaces
       */

      J_ASSERT_DEBUG( cachei < FREELIST_CACHE_SIZE,
		      "cachei overran")

      // start scanning at the next index next time	
      self->flScanPdIndex = scani;

      // set sentinel and restart the cache
      self->flCache[cachei] = NULL;
      self->flCacheIndex = 0;
      
      return FL_SUCCESS;
    }

  }


  /*
    no more free spaces on this page
   */

  return FL_PAGE_FINISHED;

}


static void * freelistGetFromCache(struct freelistDescriptor * self) {

  /*
    Get a free object slot from the cache.
    Returns NULL if cache emptied.
   */

  return self->flCache[(self->flCacheIndex)++];
}


void * freelistAlloc( struct freelistDescriptor * self,
		      unsigned int size) {

  /*
    Get a free object slot from this freelist, SELF.
    It is up to the caller to ensure that this freelist offers
    sufficiently large object slots.
    (The SIZE debugging parameter lets the freelist check
    that the it is the right one for the size of object required).
   */

  void * res;


  J_ASSERT_DEBUG( size <= self->flObjectSize,
		  "too big for this freelist");

  do {

    /*
      try to get from flCache
    */

    res = freelistGetFromCache(self);
    if (res)  return res;

    /* get a page added if this freelist doesn't have any yet */
    if (self->flHead == NULL) 
      return NULL;

    /*
      try replenishing cache from the current page
     */

    if (freelistReplenishCache(self) == FL_SUCCESS) {
      
      res = freelistGetFromCache(self);
  
      J_ASSERT_DEBUG( res != NULL, 
		      "successful replenishCache but got NULL from cache")
      
      return res;
      
    }
 
    /*
      current page used up - move on to next page
     */

    freelistScanNewPage(self, PAGE_pdNext(self->flScanPd));

  } while (self->flScanPd != NULL);


  /*
    this freelist used up
   */
  return NULL;
}



void freelistUnmark( struct freelistDescriptor * self ) {
  
  /*
    Unmark all objects on the pages belonging to SELF.
    DEBUG if ReplenishCache unmarked bits as it went along,
    could start from current flScanPd / flScanPdIndex.
   */

  struct pageDescriptor * pd = self->flHead;

  while (pd != NULL) {
    pageUnmark(pd);
    pd = PAGE_pdNext(pd);
  }

}



void freelistRemoveEmptyPages(struct freelistDescriptor * self) {
    
  /*
    Run across SELF's pages looking for those with no marked objects. 
    Return those to the pager.
  */

  struct pageDescriptor * curr = self->flHead,
                        * prev = NULL;
  
  while (curr != NULL) {
    if (pageNoSmallObjects(curr)) {

      /* Unlink the page .. */
      
      struct pageDescriptor * tofree = curr;
      
      if (prev == NULL) {
	curr = self->flHead = PAGE_pdNext(curr);
      } else {
	curr = PAGE_pdNext(prev) = PAGE_pdNext(curr);
      }
      
      /* 
	 .. and mark it free
	 The free page will be picked up when the page freelist
	 is rebuilt.
      */
      
      PAGE_SET_FREE(tofree);
      
    } else {

      /* move on to next page */      
      prev = curr;
      curr = PAGE_pdNext(curr);
      
    }
    
  }

  self->flTail = prev;
  
}





void freelistDumpObjects(struct freelistDescriptor * self ) {

  struct pageDescriptor * pd = self->flHead;

  while (pd != NULL) {
    pageDumpObjects(pd);
    pd = PAGE_pdNext(pd);
  }

}





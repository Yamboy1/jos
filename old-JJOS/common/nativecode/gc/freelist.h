
/*
  This needs to be 1 more than 8 * sizeof(MarkBits), but I'll
  be damned if I can express that in C in a way that's usable
  in an array definition.
 */

#define FREELIST_CACHE_SIZE 32 + 1

/*
  Descriptor for a freelist 
  (There are multiple freelists by size)
*/

struct freelistDescriptor {
  
/*
  the first and last pageDescriptors I own
*/
  struct pageDescriptor * flHead, * flTail;


/*
  the size of objects on my pages
*/
  unsigned int flObjectSize;


/*
  the page currently being scanned
*/  
  struct pageDescriptor * flScanPd;

/*
  index to the element in flScanPd->pdMarkBits that needs scanning next
*/
  unsigned int flScanPdIndex;


/*
  free object cache

  cache of object slots known to be free
*/
  void * flCache [FREELIST_CACHE_SIZE];

/*
   the next unused object in flCache
*/
  unsigned int flCacheIndex;

};


void freelistInit( struct freelistDescriptor * self,  
		   unsigned int objectSize);

void freelistStartScan( struct freelistDescriptor * self);

void freelistAddPage( struct freelistDescriptor * self,
		      struct pageDescriptor * pd );

void freelistAddPageForScan( struct freelistDescriptor * self,
			     struct pageDescriptor * pd );


void freelistPrint( struct freelistDescriptor * self, 
		    int full);

void * freelistAlloc( struct freelistDescriptor * self,
		      unsigned int size);

void freelistRemoveEmptyPages(struct freelistDescriptor * self);

void freelistUnmark( struct freelistDescriptor * self );

void freelistDumpObjects(struct freelistDescriptor * self );




#define MAX_FREELIST_OBJ_SIZE 2048

/* freelistsizes should be a multiple of 8 (for doubleword alignment), */
/* and shouldn't be more 2048 (ie > 1/2 page). So the maximum number */
/* of freelists it makes sense to have is the following */
#define MAX_FREELISTS (MAX_FREELIST_OBJ_SIZE/8)

struct heapCallbacks {

  void (*getRoots)();

};

void heapInit( void * ptr, 
	       unsigned int size,
	       int freelistSizes[MAX_FREELISTS+1],
	       struct heapCallbacks callbacks);

void heapPrint();

void * heapAlloc(unsigned int size);

void heapMark(void * p);

void heapMarkBlock(void * start, void * end);

void heapGC();

void heapDumpObjects();

#ifndef _PAGE_H
#define _PAGE_H

/*
  These PAGE related constants must be kept in step.
 */

#define PAGE_SIZE             4096U
#define LOG_PAGE_SIZE           12U
#define PAGE_START_MASK 0xfffff000U  

#define pageForPtr(p)     ((void *) (((int) (p)) & PAGE_START_MASK))
#define pagePtrOffset(p)            (((int) (p)) & ~PAGE_START_MASK)


/*
  Macros for manipulating the type of a page.
 */

#define PAGE_FREE      0
#define PAGE_FREELIST  1
#define PAGE_OBJECT    2
#define PAGE_MARKSTACK 3
#define PAGE_INVALID   4

#define PAGE_IS_FREE(ptr)      ((ptr)->pdType == PAGE_FREE)
#define PAGE_IS_FREELIST(ptr)  ((ptr)->pdType == PAGE_FREELIST)
#define PAGE_IS_OBJECT(ptr)    ((ptr)->pdType == PAGE_OBJECT)
#define PAGE_IS_MARKSTACK(ptr) ((ptr)->pdType == PAGE_MARKSTACK)
#define PAGE_IS_INVALID(ptr)   ((ptr)->pdType == PAGE_INVALID)

#define PAGE_SET_FREE(ptr)      (ptr)->pdType = PAGE_FREE
#define PAGE_SET_FREELIST(ptr)  (ptr)->pdType = PAGE_FREELIST
#define PAGE_SET_OBJECT(ptr)    (ptr)->pdType = PAGE_OBJECT
#define PAGE_SET_MARKSTACK(ptr) (ptr)->pdType = PAGE_MARKSTACK
#define PAGE_SET_INVALID(ptr)   (ptr)->pdType = PAGE_INVALID

#define PAGE_TYPE_IS_VALID(type) (((type) == PAGE_FREE) ||      \
                                  ((type) == PAGE_FREELIST) ||  \
		                  ((type) == PAGE_OBJECT) ||    \
		                  ((type) == PAGE_MARKSTACK) || \
				  ((type) == PAGE_INVALID))

/*
  Macros for pulling the union apart.
*/

#define PAGE_pdNext(ptr)       (ptr)->u.pdFreelist.pdNext
#define PAGE_pdMarkBits(ptr)   (ptr)->u.pdFreelist.pdMarkBits
#define PAGE_pdObjectSize(ptr) (ptr)->u.pdFreelist.pdObjectSize

#define PAGE_pdOIsHead(ptr)     (ptr)->u.pdObject.pdIsHead
#define PAGE_pdOMarked(ptr)     (ptr)->u.pdObject.pdMarked
#define PAGE_pdOSize(ptr)       (ptr)->u.pdObject.pdSize

#define PAGE_pdFNext(ptr)      (ptr)->u.pdFree.pdNext
#define PAGE_pdFPrev(ptr)      (ptr)->u.pdFree.pdPrev
#define PAGE_pdFCount(ptr)     (ptr)->u.pdFree.pdCount


/*
  Constants relating to a page's markbits
*/

#define MIN_OBJECT_SIZE         8
#define MAX_OBJECTS_PER_PAGE    (PAGE_SIZE/MIN_OBJECT_SIZE)
#define MARK_BIT_ARRAY_SIZE     (MAX_OBJECTS_PER_PAGE / (sizeof(MarkBits) * 8))

typedef unsigned int MarkBits; 


/*
  Descriptor for a page managed by this heap.
 */

struct pageDescriptor {
  
  /*
    The page I describe
    (Redundant -- can be calculated from my address by the pager).
  */

  void * pdPage;

  
  /*
    Whether my page is free, in use by the freelists, 
    or in use by a large object.
   */

  char pdType;
  
  union {

    /* PAGE_OBJECT, page belongs to a large object */
    struct {
      
      // Whether I am the page on which this large object starts
      char pdIsHead;

      // Whether I am marked or not 
      char pdMarked;
      
      // Size of this large object 
      unsigned int pdSize;

      // struct pageDescriptor * pdHead;

    } pdObject;
    
    /* PAGE_FREELIST, page is on a freelist of small objects  */
    struct {
      // the next page in the freelist I belong to
      struct pageDescriptor * pdNext;
      
      // mark bits for the objects on my page
      MarkBits pdMarkBits[MARK_BIT_ARRAY_SIZE];

      // the size of objects on my page
      unsigned short int pdObjectSize;

    } pdFreelist;

    /* PAGE_FREE, page is free */
    struct {
      // for testing: unsigned int dummy;

      // doubly linked list of blocks of free pages
      struct pageDescriptor * pdNext, * pdPrev;

      // number of pages in this block
      unsigned int pdCount;

    } pdFree;

  } u;
};


void pageInit( struct pageDescriptor * self,
	       void * page,
	       char pageType);

void pageUnmark(struct pageDescriptor * self);

void * pageMark( struct pageDescriptor * self,
	      void * p);

unsigned int pageGetObjectSize( struct pageDescriptor * self,
				void * p );

void pageInitLargeObject( struct pageDescriptor * self,
			  unsigned int numpages,
			  unsigned int size );

int pageNoSmallObjects(struct pageDescriptor * self);

void pagePrint( struct pageDescriptor * self, 
		int full);

void pageDumpObjects(struct pageDescriptor * self);

#endif /* PAGE_H */

#ifndef _PAGER_H
#define _PAGER_H

#include "page.h"

struct pager {

  /*
    The heap is broken into two sections:
    a) pgDescStart -> pgDescEnd   page descriptors
    b) pgPageStart -> pgPageEnd   actual usable pages
  */
  
  struct pageDescriptor * pgDescStart;
  struct pageDescriptor * pgDescEnd;
  void * pgPageStart;
  void * pgPageEnd;

  struct pageDescriptor pgFreePageSentinel;

};

void pagerInit( struct pager * self,
		void * ptr,
		unsigned int size );


struct pageDescriptor * pagerGetContiguousPages( struct pager * self,
						 unsigned int num,
						 char type );

void pagerPrint( struct pager * self );


struct pageDescriptor * pagerDescriptorForPage( struct pager * self,
						void * p );

void pagerAddContiguousPages( struct pager * self,
			      struct pageDescriptor * pd,
			      unsigned int numpages );

void pagerUnmarkLargeObjects(struct pager * self);

void pagerDumpLargeObjects(struct pager * self);

void pagerRebuildFreelist( struct pager * self );

void pagerPrintFreePages(struct pager * self);

#endif  /* _PAGER_H */


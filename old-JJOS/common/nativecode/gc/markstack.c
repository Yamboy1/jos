#include "stdlib.h"

#include "markstack.h"

#include "jassert.h"
#include "pager.h"
#include "page.h"


void markstackInit( struct markstack * self, 
		    struct pager * pgr) {
  
  /*
    Initialises SELF.
    Looks after getting its own memory from the pager PGR.
   */

  unsigned int pages = 20;
  struct pageDescriptor * pdBottom;
  jju32 btm, top;

  pdBottom = pagerGetContiguousPages( pgr, pages, PAGE_MARKSTACK );

  J_ASSERT(pdBottom, "Not enough memory to set up mark stack.");
  
  btm = (jju32) pdBottom->pdPage;
  top = btm + PAGE_SIZE * pages;

  self->msBottomOfStack = (void **) btm;
  self->msTopOfStack = (void **) top;

#ifdef NOTDEF
  self->msBottomOfStack = 
    (void **) pagerGetContiguousPages( pgr, pages, PAGE_MARKSTACK )->pdPage;
  
  self->msTopOfStack = (void **)
    (((unsigned int) self->msBottomOfStack) + PAGE_SIZE * pages);
#endif

}


void markstackReset( struct markstack * self,
		     struct pager * pgr ) {

  /*
    Resets SELF ready for a new heap scan.
    DEBUG could maybe go to the pager PGR to try and get
    any free pages remaining.
   */

  self->msCurr = self->msBottomOfStack;
  self->msOverflow = 0;

}


void markstackPrint( struct markstack * self) {

}


void markstackPush( struct markstack * self, 
		    void * ptr) {

  /* 
     Push PTR onto the stack
     Ignores it if the stack is full
  */
   
  if (self->msCurr < self->msTopOfStack) {

#ifdef GC_DEBUG_MARK
    printf("\t\t.. pushing %p\n", ptr); 
#endif

    *((self->msCurr)++) = ptr;

  } else {

    self->msOverflow = 1;
 
  }


}


void * markstackPop( struct markstack * self) {

  /*
    Pop the stack.
    Returns NULL if the stack is empty.
   */


  /* NB greater than because we decrement first */

  if (self->msCurr > self->msBottomOfStack) {

    return *(--(self->msCurr));

  } else {

    return NULL;

  }

}


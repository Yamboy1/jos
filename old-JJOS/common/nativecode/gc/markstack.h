#include "pager.h"

struct markstack {

  void ** msTopOfStack;
  void ** msBottomOfStack;

  void ** msCurr;

  char msOverflow;

};

void markstackInit( struct markstack * self, 
		    struct pager * pgr);

void markstackReset( struct markstack * self,
		     struct pager * pgr );

void markstackPrint(struct markstack * self);

void markstackPush(struct markstack * self, 
		   void * ptr);

void * markstackPop(struct markstack * self);

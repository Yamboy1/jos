#include "stdlib.h"
#include "jassert.h"

void j_assert_fail( const char * msg,
		    const char * func,
		    const char * file,
		    int line ) {

  int i;

  printf( "FAILED assertion: %s in %s, line %d\n",
	   msg,
	   file,
	   line);

  /* force segv - perhaps */
  i = *(int *) 0;

  // exit(1);
}

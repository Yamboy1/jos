/* assert.h
 *
 * declares assertion macros
 */

#include "d_types.h"

#ifndef decaf_assert
#define decaf_assert

/* if ptr is NULL, a memory allocation has failed. */
#define ASSERT_ALLOCATED(ptr,loc,thing) \
	if(ptr==NULL) {kprintf("%s -- unable to allocate %s, aborting.\n",loc,thing); abort();}

/* if ptr is NULL, something terrible has happened to the JVM. */
#define ASSERT_NOT_NULL(ptr,loc,error) \
	if(ptr==NULL) {kprintf("%s -- %s, aborting.\n",loc,error); abort();}

#ifndef DONT_USE_DC
/* is 'dst' of type 'type'?  if so, cast it into dst; otherwise,
   loc is where in the code we are, and thing is what the cast
   is checking for. */
#define ASSERT_CAST(dst,src,type,loc,thing) \
	if((dst=dynamic_cast<type>(src))==0) { kprintf("%s -- %x is not a %s, aborting.\n",loc,src,thing); abort(); }
#else
/* type1 is a type() return, and type2 the constant against
   which to check it.  pos is where in the code we are, thing is
   what the cast is checking for, and dst the pointer. */
#define ASSERT_CAST(dst,type1,type2,pos,thing) \
	if(type1!=type2) { kprintf("%s -- %x is not a %s, aborting.\n", pos, dst, thing ); abort(); }

#endif /* DONT_USE_DC */

#endif

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

/* if src is not of type dst, something terrible has happened to the JVM. */
#ifndef DONT_USE_DC
#define ASSERT_CAST(dst,src,type,loc,thing) \
	if((dst=dynamic_cast<type>(src))==0) { kprintf("%s -- %x is not a %s, aborting.\n",loc,src,thing); abort(); }
#else
#define ASSERT_CAST(dst,type1,type2,pos,thing) \
	if(type1!=type2) { kprintf("%s -- %x is not a %s, aborting.\n", pos, dst, thing ); abort(); }

#endif /* DONT_USE_DC */

#endif

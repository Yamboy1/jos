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
/* Test, by attempting to cast from src to dst by way of type, if dst is an instance of type;
 * if not, report the error at loc, saying that src is not a thing. */
#define ASSERT_CAST(dst,src,type,loc,thing) \
	if((dst=dynamic_cast<type>(src))==0) { kprintf("%s -- %x is not a %s, aborting.\n",loc,src,thing); abort(); }
#else
/* Test, by attempting to cast from src to dst by way of type, if dst is an instance of type;
 * if not, report the error at loc, saying that src is not a thing.
 * Note that the routine with temporaries is necessary if ASSERT_CAST is passed a function
 * as either dst or src; if they're both variables, the extra assignments will be optmized
 * away, so we're okay. */
#define ASSERT_CAST(dst,src,dptr,sptr,pos,thing) \
	{ /* opens scope */ \
	sptr tsrc = (src); \
	dptr tdst = (dst); \
	tdst =  (dptr)(tsrc); \
	if((tdst)->type()!=(tsrc)->type()) \
		{ kprintf("%s -- %x is not a %s, aborting.\n", pos, dst, thing ); abort(); } \
	} /* closes scope */

#endif /* DONT_USE_DC */

#endif

//#ifndef _J_ASSERT_H
//#define _J_ASSERT_H
/*
 * jassert.h
 *
 */

extern void j_assert_fail(const char *, const char *, const char *, int);

#define J_ASSERT(condition, msg) if (!(condition)) { j_assert_fail((msg), "", __FILE__, __LINE__); }


#ifdef GC_DEBUG
#define J_ASSERT_DEBUG(c, m) J_ASSERT(c, m)
#else
#define J_ASSERT_DEBUG(c, m)
#endif

/* the typedefs are a horrible hack to avoid overly tight integration
   with jjos (specifically jjtypes.h) */

typedef unsigned int jju32;

typedef unsigned char jju8;

/* this is an even more horrible hack to avoid changing all the 
   printfs to kprintfs */

#define printf kprintf

//#endif /* _J_ASSERT_H */

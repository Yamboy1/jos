/*
 * d_types.h
 *
 * defines types used by decaf
 */

#ifndef decaf_types
#define decaf_types

#include "jjtypes.h"

#ifndef NULL
#define NULL 0
#endif

/* probably i386-specific */
typedef bool jbool;
typedef jji8 jbyte;
typedef jji16 jshort;
typedef jji32 jint;
typedef jji64 jlong;
typedef float jfloat;
typedef double jdouble;

/* run-time type information; defined values are from the spec for bytecode 0x188 (newarray) */
enum PrimitiveType { PT_OBJECT, PT_ARRAY, PT_CLASS, PT_CLASS_INSTANCE,
                     PT_LONG = 11, PT_INT = 10, PT_SHORT = 9,
                     PT_BYTE = 8, PT_DOUBLE = 7, PT_FLOAT = 6, PT_CHAR = 5, PT_BOOLEAN = 4 };

#endif

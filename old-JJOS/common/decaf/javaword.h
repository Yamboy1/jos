/*
 * javaword.h
 * 
 * declares the C++ which 
 * represents the Java 
 * VM 'word' abstraction.
 */

#ifndef decaf_javaword
#define decaf_javaword

class JavaWord;
class JavaClassInstance;
#include "d_types.h"

/* a JavaWord which is supposed to be an object in Java
 * but whose jcip pointer is NULL is the Java 'null' object. */

class JavaWord {

  public:
    enum WORD { FirstWord, SecondWord };

    /* the NULL constructor and the check */
    JavaWord();
    bool isNull();

    /* assignment and equality */
    JavaWord& operator = ( const JavaWord& jw );
	bool operator == ( const JavaWord& jw );

    /* constructors */
	JavaWord( jju8 val );
	JavaWord( jju16 val );
	JavaWord( jju32 val );
    JavaWord( jju64 val, WORD which );

	JavaWord( jbool val );
	JavaWord( jbyte val );
	JavaWord( jshort val );
	JavaWord( jint val );
    JavaWord( jfloat val );
	/* When jji64 is written, this will no longer be ambigous. */
	/* JavaWord( jlong val, WORD which ); */
	JavaWord( jdouble val, WORD which );

	JavaWord( JavaClassInstance * jci );

    /* conversions */
    operator jju8() const;
    operator jju16() const;
    operator jju32() const;

    operator jbool() const;
    operator jbyte() const;
    operator jshort() const;
    operator jint() const;
    operator jfloat() const;

    operator JavaClassInstance * () const;
    
    /* double-word conversions */
    static jju64 toJJU64( JavaWord & jw1, JavaWord & jw2 );
    static jlong toJLong( JavaWord & jw1, JavaWord & jw2 );
    static jdouble toJDouble( JavaWord & jw1, JavaWord & jw2 );

  protected:
    union { jju32 u32; jfloat jf; jji32 i32; JavaClassInstance * jcip; } value;

}; /* end JavaWord */


/* Useful in several places. */

typedef union {
    struct {
      jju32 w1;
      jju32 w2;
      } words;
    jdouble jd;
} bit_hackery;

#endif

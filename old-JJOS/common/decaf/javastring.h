/*
 * javastring.h
 * 
 * declares the C++ class which
 * represents java strings for
 * the decaf JVM.
 */

#ifndef decaf_javastring
#define decaf_javastring

class JavaString;

#include "d_types.h"

class JavaString {

  public:
    JavaString();
    JavaString( jju32 length );
    JavaString( char * str );

    jju16& operator [] ( jju32 pos ) const;
    bool operator == ( const JavaString& js ) const;
    JavaString& operator + ( const JavaString& js ) const;
    char * c_str();

    void setMyLength( jju32 length );
    jju32 getMyLength() const;

    friend jju32 hash ( JavaString * js );
    friend bool equal ( JavaString * lhs, JavaString * rhs );

  protected:
    jju16 * myWideString;
    char * myNarrowString;
    jju32 myLength;

}; /* end class JavaString */

#endif

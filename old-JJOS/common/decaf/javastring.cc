/*
 * javastring.cc
 * 
 * defines the C++ class which
 * represents java strings for
 * the decaf JVM.
 */

#include "javastring.h"
#include "stdlib.h"

JavaString::JavaString() {
	myWideString = NULL;
	myNarrowString = NULL;
	}

JavaString::JavaString( jju32 length ) {
	myWideString = new jju16[ length ];
	if ( myWideString == NULL ) {
		kprintf( "JavaString::JavaString() -- unable to allocate wide string, aborting.\n" );
		abort();
		}
	myNarrowString = NULL;
	myLength = length;
	}

JavaString::JavaString( char * str ) {
	/* determine length */
	jju32 x = 0;
	while ( str[x] != 0 ) { x++; }
	myLength = x;

	/* allocate wide string */
	myWideString = new jju16[ myLength ];
	if ( myWideString == NULL ) {
		kprintf( "JavaString::JavaString() -- unable to allocate wide string, aborting.\n" );
		abort();
		}

	for ( x = 0; x < myLength; x++ ) {
		myWideString[x] = str[x];
		}
	myNarrowString = NULL;		
	}

char * JavaString::c_str() {
	if ( myNarrowString != NULL ) { return myNarrowString; }

	/* allocate narrow */
	myNarrowString = new char[ myLength ];
	if ( myNarrowString == NULL ) {
		kprintf( "JavaString::c_str() -- unable to allocate narrow string, aborting.\n" );
		abort();
		}

	/* convert wide to narrow */
	for ( jju32 x = 0; x < myLength; x++ ) {
		myNarrowString[x] = (char)myWideString[x];
		}

	return myNarrowString;
	}

JavaString& JavaString::operator + ( const JavaString& js ) const {
    JavaString * rv = new JavaString( js.myLength + this->myLength );
    for ( jju32 x = 0; x < this->myLength; x++ ) {
        rv->myWideString[x] = this->myWideString[x];
        }
    for ( jju32 x = this->myLength; x < rv->myLength; x++ ) {
        rv->myWideString[x] = js.myWideString[x - this->myLength];
        }
    return *rv;
    } /* end operator +() */

jju16& JavaString::operator [] ( jju32 pos ) const {
    if ( pos <= myLength ) {
        /* pos is unsigned, so it must be >= 0 */
        return myWideString[pos];
        } else {
        kprintf( "JavaString::operator [] () -- invalid string index %d, aborting.\n", pos );
        abort();
        }
    return myWideString[0];
    } /* end operator []() */

bool JavaString::operator == ( const JavaString & js ) const {
    if ( this->getMyLength() != js.getMyLength() ) { return JJFALSE; }

    jju32 l = this->getMyLength();
    for ( jju32 x = 0; x < l; x++ ) {
        if ( (*this)[x] != js[x] ) { return JJFALSE; }
        } /* end comparison loop */
    return JJTRUE;
    } /* end operator == */

jju32 hash( JavaString * js ) {
	jju32 hash_value = 0;
	for ( jju32 x = 0; x < js->myLength; x ++ ) {	
		hash_value += (*js)[x];
		}
	return hash_value;
	} /* end hash() */

bool equal( JavaString * lhs, JavaString * rhs ) {
    if ( lhs->getMyLength() != rhs->getMyLength() ) { return JJFALSE; }

    jju32 l = lhs->getMyLength();
    for( jju32 x = 0; x < l; x++ ) {
        if ( (*lhs)[x] != (*rhs)[x] ) { return JJFALSE; }
        } /* end comparison loop */
    return JJTRUE;
    } /* end equal() */

jju32 JavaString::getMyLength() const { return myLength; }
void JavaString::setMyLength( jju32 length ) { myLength = length; }


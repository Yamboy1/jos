/*
 * javaword.cc
 * 
 * defines the C++ which 
 * represents the Java 
 * VM 'word' abstraction.
 */

#include "javaword.h"
#include "stdlib.h"

JavaWord::JavaWord() {
    value.jcip = NULL;
    }

bool JavaWord::isNull() {
    return (value.jcip == NULL);
	}

JavaWord& JavaWord::operator = ( const JavaWord& jw ) {
    this->value.u32 = jw.value.u32;
    return *this;
	}

bool JavaWord::operator == ( const JavaWord& jw ) {
    return this->value.u32 == jw.value.u32;
	}

JavaWord::JavaWord( jbool val ) {
	this->value.u32 = val;
	}

JavaWord::JavaWord( jju8 val ) {
    this->value.u32 = val;
	}

JavaWord::JavaWord( jju16 val ) {
    this->value.u32 = val;   
	}

JavaWord::JavaWord( jju32 val ) {
    this->value.u32 = val;
	}

JavaWord::JavaWord( jju64 val, WORD which ) {
    if ( which == FirstWord ) { this->value.u32 = val.myHigh32; }
    else if ( which == SecondWord ) { this->value.u32 = val.myLow32; }
    else {
        kprintf( "JavaWord::JavaWord( jju64, WORD ) -- illegal value in enumeration, aborting.\n" );
        abort();
        }
	}

JavaWord::JavaWord( jshort val ) {
    this->value.i32 = val;
	}

JavaWord::JavaWord( jint val ) {
    this->value.i32 = val;
	}

JavaWord::JavaWord( jfloat val ) {
    this->value.jf = val;
	}

JavaWord::JavaWord( jdouble val, WORD which ) {
    bit_hackery bh;
    bh.jd = val;
    if ( which == FirstWord ) { this->value.u32 = bh.words.w1; }
    else if ( which == SecondWord ) { this->value.u32 = bh.words.w2; }
    else {
        kprintf( "JavaWord::JavaWord( jju64, WORD ) -- illegal value in enumeration, aborting.\n" );
        abort();
        }
	}

JavaWord::operator jbool() const {
    return (jbool)this->value.u32;
	}

JavaWord::operator jju8() const {
    return (jju8)this->value.u32;
	}

JavaWord::operator jju16() const {
    return (jju16)this->value.u32;
    }

JavaWord::operator jju32() const {
    return this->value.u32;
	}

JavaWord::operator jbyte() const {
   return (jji8)this->value.i32;
	}

JavaWord::operator jshort() const {
    return (jshort)this->value.i32;
	}

JavaWord::operator jint() const {
    return (jint)this->value.i32;
	}

JavaWord::operator jfloat() const {
    return (jfloat)this->value.jf;
	}

jju64 JavaWord::toJJU64( JavaWord & jw1, JavaWord & jw2 ) {
    return jju64( jw1.value.u32, jw2.value.u32 );
	}

jlong JavaWord::toJLong( JavaWord & jw1, JavaWord & jw2 ) {
    return jlong( jw1.value.u32, jw2.value.u32 );
	}

jdouble JavaWord::toJDouble( JavaWord & jw1, JavaWord & jw2 ) {
    bit_hackery bh;
    bh.words.w1 = jw1.value.u32;
    bh.words.w2 = jw2.value.u32;
    return bh.jd;
	}

JavaWord::JavaWord( JavaClassInstance * jci ) {
    this->value.jcip = jci;
    }

JavaWord::operator JavaClassInstance * () const {
    return this->value.jcip;
    }

/*
 * cpentry.cc
 * 
 * defines the classes which represent
 * a Java class's constant pool entry.
 * 
 * subclasses of this class differ
 * by type and degree of resolution.
 */

#include "cpentry.h"
#include "assert.h"

/* CPEntry */

CPEntry::CPEntry() { ; }

CPEntry * CPEntry::generateCPEntry( istream & is ) {
    /* Fetch the tag byte. */
    jju8 byte;
    is >> byte;

    CPEntry * cpe = NULL;

    /* Switch on the tag byte to hand off
     * to the proper *::generate*() function. */
    switch( byte ) {
        case TAG_UTF8:
            cpe = ConstantUTF8::generateConstantUTF8( is );
            break;

        case TAG_CLASSINFO:
            cpe = ClassInfo::generateClassInfo( is );
            break;

        case TAG_FIELDREF:
            cpe = FieldRef::generateFieldRef( is );
            break;

        case TAG_METHODREF:
            cpe = MethodRef::generateMethodRef( is );
            break;

        case TAG_INTERFACEMETHODREF:
            cpe = InterfaceMethodRef::generateInterfaceMethodRef( is );
            break;

        case TAG_STRING:
            cpe = CPString::generateCPString( is );
            break;

        case TAG_INTEGER:
            cpe = CPInteger::generateCPInteger( is );
            break;

        case TAG_FLOAT:
            cpe = CPFloat::generateCPFloat( is );
            break;

        case TAG_LONG:
            cpe = CPLong::generateCPLong( is );        
            break;

        case TAG_DOUBLE:
            cpe = CPDouble::generateCPDouble( is );            
            break;

        case TAG_NAMEANDTYPE:
            cpe = NameAndType::generateNameAndType( is );
            break;
            
        default:
            kprintf( "CPEntry::generateCPEntry() -- invalid tag byte %d, aborting.\n", byte );
            abort();
            } /* end tag byte switch */

    return cpe;
    } /* end generateCPEntry() */


/* ConstantUTF8 */

ConstantUTF8::ConstantUTF8() { myJavaString = NULL; }

ConstantUTF8 * ConstantUTF8::generateConstantUTF8( istream & is ) {
    /* The ConstantUTF8 to return. */
    ConstantUTF8 * cu8 = new ConstantUTF8();

    if ( cu8 == NULL ) {
        kprintf( "ConstantUTF8::generateConstantUTF8() -- unable to generate new entry, aborting.\n" );
        abort();
        }

    /* The length of the UTF8 encoding, not the string it represents. */
    jju16 length;
    is >> length;

    if ( length == 0 ) {
        /* zero-length strings are OK and distinct from null ones. */
        cu8->myJavaString = new JavaString( "" );

        if ( cu8->myJavaString == NULL ) {
            kprintf( "ConstantUTF8::generateConstantUTF8() -- unable to generate empty JavaString, aborting.\n" );
            abort();
            }
            
        return cu8;
        } /* end if zero-length string */

    /* Generate the JavaString to fill. */
    JavaString * js = new JavaString( length );


    /* Decode the UTF8 into wide characters. */
    int char_count = 0;
    jju8 aByte = 0;
    
    for ( int x = 0; x < length; x++ ) {
        /* fetch the initial byte */
        is >> aByte;

        if ( ! (aByte & 128) ) {
            /* 0xxx xxxx, a one byte character */
            (*js)[char_count] = aByte;
            char_count++;
            }
        else if ( aByte & 192 && ! (aByte & 32) ) {
            /* 110x xxxx, a two byte character */
            jju8 bByte;
            is >> bByte;
            
            (*js)[char_count] = ((aByte & 0x1F) << 6) + (bByte & 0x3F);
            char_count++; x++;
            }
        else if ( aByte & 224 && ! (aByte & 16) ) {
            /* 1110 xxxx, a three byte character */
            jju8 bByte, cByte;
            is >> bByte >> cByte;

            (*js)[char_count] = ((aByte & 0xF) << 12) + ((bByte & 0x3f) << 6) + (cByte & 0x3F);
            char_count++; x++; x++;
            }
        else {
            kprintf( "ConstantUTF8::generateConstantUTF8() -- invalid byte in UTF8 sequence, aborting.\n" );
            abort();
            }
        } /* end decoding loop */

    /* null - terminate */
    (*js)[char_count] = 0;
    js->setMyLength( char_count );
    cu8->setMyJavaString( js );

    return cu8;    
    } /* end generateConstantUTF8() */

JavaString * ConstantUTF8::getMyJavaString() { return myJavaString; }

void ConstantUTF8::setMyJavaString( JavaString * js ) {
    if ( myJavaString == NULL ) { myJavaString = js; }
    else {
        kprintf( "ConstantUTF8::setMyString() -- illegal attempt to reset constant string, aborting.\n" );
        abort();
        }
    } /* end setMyJavaString() */

/* CPString */

CPString::CPString() { myJavaString = NULL; }

CPString * CPString::generateCPString( istream & is ) {
    /* The CPString to return. */
    CPString * cps = new CPString();
    if ( cps == NULL ) {
        kprintf( "CPString::generateCPString() -- unable to allocate new CPString, aborting.\n" );
        abort();
        }

    /* Fetch the index to my CU8 */
    is >> cps->myCU8Index;

	/* return it */
    return cps;
    } /* end generateCPString() */

JavaString * CPString::getMyJavaString( ConstantPool * cp ) {
	if ( myJavaString != NULL ) { return myJavaString; }
	if ( cp == NULL ) { return myJavaString; }

	/* fetch the JavaString from the cu8 index */
	ConstantUTF8 * cu8 = NULL;
	ASSERT_CAST( cu8, (*cp)[myCU8Index], ConstantUTF8 *, CPEntry *,
		"CPString::getMyJavaString()", "constant UTF-8 string" );
	myJavaString = cu8->getMyJavaString();

	return myJavaString;
	}

void CPString::setMyJavaString( JavaString * js ) {
    if ( myJavaString == NULL ) { myJavaString = js; }
    else {
        kprintf( "CPString::setMyString() -- illegal attempt to reset constant string, aborting.\n" );
        abort();
        }
    } /* end setMyJavaString() */

/* Ref */

JavaString * Ref::getMyName( ConstantPool * cp ) {
	if ( myName != NULL ) { return myName; }

	/* fetch & verify NameAndTypeIndex */
	NameAndType * nat = NULL;
	ASSERT_CAST( nat, (*cp)[myNameAndTypeIndex], NameAndType *, CPEntry *,
		"Ref::getMyName()", "valid name and type index" );

	myName = nat->getMyName( cp );
	return myName;
	} /* end getMyName() */

JavaString * Ref::getMyType( ConstantPool * cp ) {
	if ( myType != NULL ) { return myType; }

	/* fetch & verify NameAndTypeIndex */
	NameAndType * nat = NULL;
	ASSERT_CAST( nat, (*cp)[myNameAndTypeIndex], NameAndType *, CPEntry *,
		"Ref::getMyName()", "valid name and type index" );

	myType = nat->getMyType( cp );
	return myType;
	} /* end getMyName() */

JavaClassInstance * Ref::getMyClass( ConstantPool * cp = NULL, ClassLoader * cl = NULL ) {
	if ( myJavaClass != NULL ) { return myJavaClass; }

	/* fetch & verify myClassIndex */
	ClassInfo * ci = NULL;
	ASSERT_CAST( ci, (*cp)[myClassIndex], ClassInfo *, CPEntry *,
		"Ref::getMyClass()", "invalid class index" );

	myJavaClass = ci->getMyClass(cp, cl);
	return myJavaClass;
	}

/* NameAndType */

JavaString * NameAndType::getMyName( ConstantPool * cp ) {
	if ( myName != NULL ) { return myName; }

	ConstantUTF8 * cu8 = NULL;
	ASSERT_CAST( cu8, (*cp)[myNameIndex], ConstantUTF8 *, CPEntry *,
		"NameAndType::getMyName()", "valid name index" );

	myName = cu8->getMyJavaString();
	return myName;
	}
	
JavaString * NameAndType::getMyType( ConstantPool * cp ) {
	if ( myType != NULL ) { return myName; }

	ConstantUTF8 * cu8 = NULL;
	ASSERT_CAST( cu8, (*cp)[myTypeIndex], ConstantUTF8 *, CPEntry *,
		"NameAndType::getMyName()", "valid type index" );

	myType = cu8->getMyJavaString();
	return myType;
	}

/* ClassInfo */

JavaClassInstance * ClassInfo::getMyClass( ConstantPool * cp, ClassLoader * cl ) {
	if ( myJavaClass != NULL ) { return myJavaClass; }

	ConstantUTF8 * cu8 = NULL;
	ASSERT_CAST( cu8, (*cp)[myClassIndex], ConstantUTF8 *, CPEntry *,
		"NameAndType::getMyName()", "invalid name index" );

	myJavaClass = cl->getClass( cu8->getMyJavaString() );
	return myJavaClass;
	}


/*
 * attributelist.cc
 * 
 * defines the C++ which
 * represents an attribute
 * list for a Java class.
 */

#include "attributelist.h"

AttributeList::AttributeList() { myAttributeCount = 0; }

AttributeList * AttributeList::generateAttributeList( istream & is, JavaClass * jc ) {
    return RawAttributeList::generateRawAttributeList( is, jc );
    }

RawAttributeList::RawAttributeList() {
    myAttributeList = NULL;
    }


AttributeInfo * RawAttributeList::operator []( int index ) {
    if ( index >= 0 && index < myAttributeCount ) {
        return myAttributeList[index];
        } else {
        kprintf( "RawAttributeList::operator[] (%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never happen */
    return NULL;
    }    

RawAttributeList * RawAttributeList::generateRawAttributeList( istream & is, JavaClass * jc ) {
    /* Generate the RAL to return. */
    RawAttributeList * ral = new RawAttributeList();
    if ( ral == NULL ) {
        kprintf( "generateRawAttributeList() -- unable to allocate ral, aborting.\n" );
        abort();
        }

    /* How many attributes do I have? */
    is >> ral->myAttributeCount;

    /* Allocate memory for that much, if any */
    if ( ral->myAttributeCount == 0 ) { return ral; }
    ral->myAttributeList = new AttributeInfo * [ral->myAttributeCount];

    /* Generate the AttributeInfos */
    for ( int x = 0; x < ral->myAttributeCount; x++ ) {
        ral->myAttributeList[x] = AttributeInfo::generateAttributeInfo( is, jc );
        } /* end generation loop */
    return ral;
    } /* end generateRawAttributeList() */


AttributeInfo::AttributeInfo() { ; }

AttributeInfo * AttributeInfo::generateAttributeInfo( istream & is, JavaClass * jc ) {
    return RawAttributeInfo::generateRawAttributeInfo( is, jc );
    }

RawAttributeInfo::RawAttributeInfo() : AttributeInfo() { myNameIndex = 0; myAttributeLength = 0; }

JavaString * RawAttributeInfo::getMyName( ConstantPool * cp ) {
    if ( myName != NULL ) { return myName; }
    
#ifdef DONT_USE_DC
    CPEntry * cpe = (*cp)[myNameIndex];
    if ( cpe->type() != TAG_UTF8 ) {
        kprintf( "RawAttributeInfo::getMyName() -- erroneous constant pool entry type (%d), aborting.\n", cpe->type() );
        abort();
        }
    myName = ((ConstantUTF8*)cpe)->getMyJavaString();
#else
    if ( ConstantUTF8 * cu8 = dynamic_cast<ConstantUTF8*>( (*cp)[myNameIndex] ) ) {
        myName = cu8->getMyJavaString();
        } else {
        kprintf( "RawAttributeInfo::getMyName() -- erroneous constant pool entry type (%d), aborting.\n", ((*cp)[myNameIndex])->type() );
        abort();
        }        
#endif        

    return myName;
    }

RawAttributeInfo * RawAttributeInfo::generateRawAttributeInfo( istream & is, JavaClass * jc ) {
    /* Generate the RAI we return */
    RawAttributeInfo * rai = new RawAttributeInfo();
    if ( rai == NULL ) {
        kprintf( "generateRawAttributeInfo() -- unable to allocate rai, aborting.\n" );
        abort();
        }

    /* Fetch the name index and attribute length. */
    is >> rai->myNameIndex >> rai->myAttributeLength;

    /* Switch to attribute-specific generation based on the name. */
    JavaString * js = jc->getMyConstantUTF8( rai->myNameIndex );
    if ( js == NULL ) {
        kprintf( "generateRawAttributeInfo() -- unable to fetch attribute name, aborting.\n" );
        abort();
        }

    /* should probably intern the JS constructions */
    RawAttributeInfo * r = NULL;
    if ( (*js) == JavaString("ConstantValue") ) {
        r = RawConstantValueAttribute::generateRawConstantValueAttribute( is, rai );
        }
    else if ( (*js) == JavaString("Code") ) {
        r = RawCodeAttribute::generateRawCodeAttribute( is, rai, jc );
        }
    else if ( (*js) == JavaString("Exceptions") ) {
        r = RawExceptionsAttribute::generateRawExceptionsAttribute( is, rai );
        }
    else {
        /* We can safely ignore every other attribute. */
        /* FIXME: need to add inner class, etc, attributes. */
        r = UnknownRawAttribute::generateUnknownRawAttribute( is, rai );
        }

    delete rai;
    return r;
    } /* end generateRawAttributeInfo */

UnknownRawAttribute::UnknownRawAttribute() : RawAttributeInfo () { myUnknownAttribute = NULL; }

UnknownRawAttribute * UnknownRawAttribute::generateUnknownRawAttribute( istream & is, RawAttributeInfo * rai ) {
    /* Generate a clean URA */
    UnknownRawAttribute * ura = new UnknownRawAttribute ();
    if ( ura == NULL ) {
        kprintf( "generateUknownRawAttribute() -- unable to allocate ura, aborting.\n" );
        abort();
        }

    /* Populate it with information from rai */
    ura->myNameIndex = rai->myNameIndex;
    ura->myAttributeLength = rai->myAttributeLength;

    /* We don't actually have to store the raw attribute data
     * if we don't understand it, so I'll just skip that allocation. */
    if( ura->myAttributeLength == 0 ) {
        return ura;
        }

    jju8 temp;
    for( jju32 x = 0; x < ura->myAttributeLength; x++ ) {
        is >> temp;
        } /* end read (eat) loop */
    return ura;
    } /* end generateUnknownRawAttribute() */

RawConstantValueAttribute::RawConstantValueAttribute() { myCVIndex = 0; }

RawConstantValueAttribute * RawConstantValueAttribute::generateRawConstantValueAttribute( istream & is, RawAttributeInfo * rai ) {
    /* Generate a clean RCVA */
    RawConstantValueAttribute * rcva = new RawConstantValueAttribute();
    if ( rcva == NULL ) {
        kprintf( "generateRawConstantValueAttribute() -- unable to allocate rcva, aborting.\n" );
        abort();
        }

    /* Populate it with information from rai */
    rcva->myNameIndex = rai->myNameIndex;
    rcva->myAttributeLength = rai->myAttributeLength;

    /* Fetch myConstantValueIndex */
    is >> rcva->myCVIndex;

    return rcva;
    } /* end generateRawConstantValueAttribute() */

RawCodeAttribute::RawCodeAttribute() {
    myMaxStack = 0;
    myMaxLocals = 0;
    myCodeLength = 0;
    myCode = NULL;
    myExceptionTable = NULL;
    myAttributeList = NULL;
    } /* end RawCodeAttribute() */

ExceptionTable * RawCodeAttribute::getMyExceptionTable() { return myExceptionTable; }
AttributeList * RawCodeAttribute::getMyAttributeList() { return myAttributeList; }

CodeAttribute * CodeAttribute::generateCodeAttribute( istream & is, RawAttributeInfo * rai, JavaClass * jc ) {
    return RawCodeAttribute::generateRawCodeAttribute( is, rai, jc );
    }

RawCodeAttribute * RawCodeAttribute::generateRawCodeAttribute( istream & is, RawAttributeInfo * rai, JavaClass * jc ) {
    /* Generate a clean RCA */
    RawCodeAttribute * rca = new RawCodeAttribute();
    if ( rca == NULL ) {
        kprintf( "generateRawCodeAttribute() -- unable to allocate rca, aborting.\n" );
        abort();
        }

    /* Populate rca with values from rai. */
    rca->myNameIndex = rai->myNameIndex;
    rca->myAttributeLength = rai->myAttributeLength;
    rca->myName = rai->myName;

    /* Fetch CodeAttribute fixed-size values. */
    is >> rca->myMaxStack >> rca->myMaxLocals >> rca->myCodeLength;

    /* Allocate the Code array. */
    if ( rca->myCodeLength == 0 ) {
        kprintf( "generateRawCodeAttribute() -- panic, no code in code attribute.\n" );
        abort();
        }

    rca->myCode = new jju8[ rca->myCodeLength ];
    if ( rca->myCode == NULL ) {
        kprintf( "generateRawCodeAttribute() -- unable to allocate code, aborting.\n" );
        abort();
        }

    /* Fill the Code array. */
    for ( jju32 x = 0; x < rca->myCodeLength; x++ ) {
        is >> rca->myCode[x];
        }

    /* Generate the exception table */
    rca->myExceptionTable = ExceptionTable::generateExceptionTable( is );
    if ( rca->myExceptionTable == NULL ) {
        kprintf( "generateRawCodeAttribute() -- void exception table, aborting.\n" );
        abort();
        }

    /* Generate the attribute list */
    rca->myAttributeList = AttributeList::generateAttributeList( is, jc );
    if ( rca->myAttributeList == NULL ) {
        kprintf( "generateRawCodeAttribute() -- void attribute list, aborting.\n" );
        abort();
        }

    return rca;
    } /* end generateRawCodeAttribute() */


ExceptionTable::ExceptionTable() { myExceptionCount = 0; myExceptionTable = NULL; }

ExceptionTable * ExceptionTable::generateExceptionTable( istream & is ) {
    /* Generate a clean ET */
    ExceptionTable * et = new ExceptionTable();

    /* Fetch the exception count. */
    is >> et->myExceptionCount;

    /* Allocate the exception table. */
    if ( et->myExceptionCount == 0 ) {
        return et;
        }
    et->myExceptionTable = new ExceptionTableEntry * [et->myExceptionCount];

    /* Populate the table. */
    for ( jju32 x = 0; x < et->myExceptionCount; x++ ) {
        is >> et->myExceptionTable[x]->myStartPC;
        is >> et->myExceptionTable[x]->myEndPC;
        is >> et->myExceptionTable[x]->myHandlerPC;
        is >> et->myExceptionTable[x]->myCatchTypeIndex;
        }
    return et;
    } /* end generateExceptionTable() */


RawExceptionsAttribute::RawExceptionsAttribute() {
    myExceptionCount = 0;
    myExceptionList = NULL;
    } /* end RawExceptionsAttribute() */

RawExceptionsAttribute * RawExceptionsAttribute::generateRawExceptionsAttribute( istream & is, RawAttributeInfo * rai ) {
    /* Generate a clean REA */
    RawExceptionsAttribute * rea = new RawExceptionsAttribute();
    if ( rea == NULL ) {
        kprintf( "generateRawExceptionsAttribute() -- unable to allocate rea, aborting.\n" );
        abort();
        }

    /* Populate rca with values from rai. */
    rea->myNameIndex = rai->myNameIndex;
    rea->myAttributeLength = rai->myAttributeLength;

    /* Fetch the exception count. */
    is >> rea->myExceptionCount;

    /* Allocate the exception list. */
    if ( rea->myExceptionCount == 0 ) {
        return rea;
        }

    rea->myExceptionList = new jju16[ rea->myExceptionCount ];
    if ( rea->myExceptionList == NULL ) {
        kprintf( "generateRawExceptionsAttribute() -- unable to allocate exception list, aborting.\n" );
        abort();
        }

    /* Populate the exception list. */
    for ( jju32 x = 0; x < rea->myExceptionCount; x++ ) {
        is >> rea->myExceptionList[x];
        }
    return rea;
    } /* end generateRawExceptionsAttribute() */

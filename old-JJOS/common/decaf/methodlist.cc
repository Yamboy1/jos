/*
 * methodlist.cc
 * 
 * defines the C++ class which
 * represents a Java class's
 * method list.
 */

#include "methodlist.h"
#include "assert.h"

MethodList::MethodList() {
    myMethodCount = 0;
    } /* end MethodList() */

MethodList * MethodList::generateMethodList( istream & is, JavaClass * jc, MethodList * parent ) {
    return RawMethodList::generateRawMethodList( is, jc, parent );
    } /* end generateMethodList() */

MethodInfo * RawMethodList::operator [] ( int index ) {
    if ( index >= 0 && index < myMethodCount ) {
        if ( myMethodList != NULL ) {
            return myMethodList[index];
            } else {
            kprintf( "RawMethodList::operator []() -- myMethodList is NULL, aborting.\n" );
            abort();
            }
        } else {
        kprintf( "RawMethodList::operator []() -- index out of bounds, aborting.\n" );
        abort();
        }
    /* should never happen */
    return NULL;
    } /* end operator []() */

RawMethodList::RawMethodList() {
    myMethodList = NULL;
    } /* end RawMethodList() */

MethodList * RawMethodList::generateRawMethodList( istream & is, JavaClass * jc, MethodList * parent ) {
    /* allocate a clean MethodList */
    RawMethodList * rml = new RawMethodList();
    if ( rml == NULL ) {
        kprintf( "RawMethodList::generateRawMethodList() -- unable to allocate rml, aborting.\n" );
        abort();
        }

    /* allocate myMethodList */
    is >> rml->myMethodCount;

	/* if I have a parent, allocate space in my table for its methods */
	jju32 parentCount = 0;
	if ( parent != NULL ) { parentCount = parent->getMyMethodCount(); }
    
    rml->myMethodList = new MethodInfo * [rml->myMethodCount + parentCount];
    if( rml->myMethodList == 0 ) {
        kprintf( "RawMethodList::generateRawMethodList() -- unable to allocate myMethodList, aborting.\n" );
        abort();
        }

	/* if I have a parent, copy its methods in */
	for ( jju16 x = 0; x < parentCount; x++ ) {
		rml->myMethodList[x] = (*parent)[x];
		}

    /* fill in myMethodList */
    for ( jju16 x = parentCount; x < rml->myMethodCount + parentCount; x++ ) {
       rml->myMethodList[x] = MethodInfo::generateMethodInfo( is, jc );
       }
       
    rml->myMethodCount += parentCount;
    return rml;
    } /* end generateRawMethodList() */

MethodInfo::MethodInfo() {
    myAccessFlags = 0;
    } /* end MethodInfo() */

MethodInfo * MethodInfo::generateMethodInfo( istream & is, JavaClass * jc) {
    return RawMethodInfo::generateRawMethodInfo( is, jc );
    }

RawMethodInfo::RawMethodInfo () {
    myNameIndex = 0;
    myDescriptorIndex = 0;
    myAttributeList = NULL;
    myName = NULL;
    mySig = NULL;
    } /* end RawMethodInfo() */

RawMethodInfo * RawMethodInfo::generateRawMethodInfo( istream & is, JavaClass * jc ) {
    /* generate a clean MethodInfo */
    RawMethodInfo * rmi = new RawMethodInfo();
    if( rmi == NULL ) {
        kprintf( "RawMethodInfo::generateRawMethodInfo() -- unable to allcoate rmi, aborting.\n" );
        abort();
        }

    is >> rmi->myAccessFlags;
    is >> rmi->myNameIndex >> rmi->myDescriptorIndex;

    rmi->myAttributeList = AttributeList::generateAttributeList( is, jc );

	/* we'll try resolving with a constant pool we know to be right */
	rmi->getMyName( jc->getMyConstantPool() );
	rmi->getMySig( jc->getMyConstantPool() );
    
    return rmi;
    } /* end generateRawMethodInfo() */

JavaString * RawMethodInfo::getMyName( ConstantPool * cp ) {
    /* Have we already resolved it? */
    if ( myName != NULL ) { return myName; }

    /* If not, fetch it from the constant pool. */
    ConstantUTF8 * cu8 = NULL;
    ASSERT_CAST( cu8, (*cp)[myNameIndex], ConstantUTF8 *, CPEntry *, "RawMethodInfo::getMyName()", "constant string" ); 

    myName = cu8->getMyJavaString();
    return myName;
    } /* end getMyName() */

JavaString * RawMethodInfo::getMySig( ConstantPool * cp ) {
    if ( mySig != NULL ) { return mySig; }

    /* If not, fetch it from the constant pool. */
    ConstantUTF8 * cu8 = NULL;
    ASSERT_CAST( cu8, (*cp)[myDescriptorIndex], ConstantUTF8 *, CPEntry *, "RawMethodInfo::getMySig()", "constant string" ); 

    mySig = cu8->getMyJavaString();
    return mySig;    
    } /* end getMySig() */

CodeAttribute * RawMethodInfo::getMyCodeAttribute( ConstantPool * cp ) {
    /* any point in caching this? */
    for ( int x = 0; x < myAttributeList->getMyAttributeCount(); x++ ) {
        if ( * ((*myAttributeList)[x]->getMyName( cp )) == JavaString("Code") ) { /* intern! */
#ifdef DONT_USE_DC            
            return (CodeAttribute*)(*myAttributeList)[x];
#else
            return dynamic_cast<CodeAttribute*>((*myAttributeList)[x]);
#endif                        
            }
        } /* end attribute list iteration */
    kprintf( "RawMethodInfo::getMyCodeAttribute() -- unable to find code in attribute list, aborting.\n" );
    abort();
    return NULL;
    } /* end getMyCodeAttribute() */

/*
 * javaclass.cc
 * 
 * defines the C++ class which
 * represents a given Java class
 * to the decaf JVM.
 */

#include "javaclass.h"
#include "cpentry.h"

/* JavaClass */

JavaClass::JavaClass() {
    myClassLoader = NULL;
    myName = NULL;

    myConstantPool = NULL;
    myInterfaceList = NULL;
    myFieldList = NULL;
    myMethodList = NULL;
    myAttributeList = NULL;
    }

JavaClass::JavaClass( JavaClass * jc ) {
    myClassLoader = jc->myClassLoader;
    myName = jc->myName;

    myConstantPool = jc->myConstantPool;
    myInterfaceList = jc->myInterfaceList;
    myFieldList = jc->myFieldList;
    myMethodList = jc->myMethodList;
    myAttributeList = jc->myAttributeList;

    myThisClassIndex = jc->myThisClassIndex;
    mySuperClassIndex = jc->mySuperClassIndex;
    }

CPEntry * JavaClass::getMyConstant( jju16 index ) {
    return (*myConstantPool)[index];
    } /* end getMyConstantUTF8 */

JavaString * JavaClass::getMyConstantUTF8( jju16 index ) {
    /* fetch the constant */
    CPEntry * cpe = getMyConstant( index );

    if ( cpe == NULL ) {
        kprintf( "JavaClass::getMyConstantUTF8(%d) -- unable to fetch constant, aborting.\n", index );
        abort();
        }
        
#ifdef DONT_USE_DC
    if ( cpe->type() != TAG_UTF8 ) {
        kprintf( "JavaClass::getMyConstantUTF8(%d) -- fetched non-UTF8 constant, continuing...\n", index );
        return NULL;
        }

    /* return the JavaString */
    return ((ConstantUTF8*)cpe)->getMyJavaString();
#else
#ifndef DONT_USE_DC
    if( ConstantUTF8 * cu8 = dynamic_cast<ConstantUTF8*>( cpe ) ) {
        return cu8->getMyJavaString();
#else
   if( cpe->type() == TAG_CU8 ) {
	return ((ConstantUTF8*)cpe)->getMyJavaString();
#endif
        } else {
        kprintf( "JavaClass::getMyConstantUTF8(%d) -- fetched non-UTF8 constant, continuing...\n", index );
        return NULL;
        }
#endif
    } /* end getMyConstantUTF8 */

JavaClassInstance * JavaClass::generateClassInstance( istream & is, ClassLoader * cl ) {
    /* Generating a class consists only of parsing the
       given stream into a urJavaClass, which is a
       direct representation of the classfile. */
    return new JavaClassInstance( new urJavaClass( is, cl ) );
    }

CodeAttribute * JavaClass::getMethodCode( JavaString * method, JavaString * methodSig ) {
	MethodInfo * mi = getMethodInfo( method, methodSig );
	if ( mi != NULL ) { return mi->getMyCodeAttribute( myConstantPool ); }
	else { return NULL; }
	} /* end getMethodCode() */

MethodInfo * JavaClass::getMethodInfo( JavaString * method, JavaString * methodSig ) {
    /* Iterate over the method list until we find one that matches. */
    JavaString * potMeth = NULL;
    JavaString * potSig = NULL;;
    for ( int x = 0; x < myMethodList->getMyMethodCount(); x++ ) {
        potMeth = (*myMethodList)[x]->getMyName( myConstantPool );
        if ( potMeth == NULL ) {
        	kprintf( "JavaClass::getMethod() -- unable to retrieve method name, aborting.\n" );
        	abort();
        	}
        if ( *potMeth == *method ) {
            potSig = (*myMethodList)[x]->getMySig( myConstantPool );
	        if ( potSig == NULL ) {
	        	kprintf( "JavaClass::getMethod() -- unable to retrieve method signature, aborting.\n" );
	        	abort();
	        	}
            if ( *potSig == *methodSig ) {
                /* this is the method, so return its info */
                return (*myMethodList)[x];
                } /* end if signatures match */
            } /* end if names match */
        } /* end method list iteration */
    return NULL;
    } /* end getMethod() */


/* urJavaClass */

urJavaClass::urJavaClass( istream & is, ClassLoader * cl ) {
    /* Store the classloader which generated this class. */
    myClassLoader = cl;
    
    /* Verify that the ramdiskstream is a classfile. */
    if ( ! verifyStream( is ) ) {
        kprintf( "urJavaClass::urJavaClass() -- invalid ramdiskstream, aborting.\n" );
        abort();
        }

    /* Generate the constant pool. */
    myConstantPool = ConstantPool::generateConstantPool( is );

    /* Fetch class indices and flags */
    is >> myAccessFlags >> myThisClassIndex >> mySuperClassIndex;

    /* Generate the interfaces list. */
    myInterfaceList = InterfaceList::generateInterfaceList( is, this );

	FieldList * myParentsFList = NULL;

	/* unless I'm java/lang/Object (have a superclass index of 0),
	 * fetch my superclass's field and method lists to generate my own. */

	JavaClassInstance * myParent = NULL;
	if ( mySuperClassIndex != 0 ) {
		/* fetch my parent class */
#ifndef DONT_USE_DC
		if ( ClassInfo * ci = dynamic_cast<ClassInfo *>((*myConstantPool)[mySuperClassIndex]) ) {
			if ( ConstantUTF8 * cu8 = dynamic_cast<ConstantUTF8 *>((*myConstantPool)[ci->getMyClassIndex()]) ) {
				myParent = myClassLoader->getClass( cu8->getMyJavaString() );
				}
			else {
				kprintf( "urJavaClass::urJavaClass() -- illegal constant pool entry while fetching parent, aborting.\n" );
				abort();
				}
			}
		else {
			kprintf( "urJavaClass::urJavaClass() -- illegal constant pool entry while fetching parent, aborting.\n" );
			abort();
			}

#else
		CPEntry * cpe = (*myConstantPool)[mySuperClassIndex];
		if ( cpe->type() == TAG_CLASSINFO ) {
			CPEntry * cpeb = (*myConstantPool)[((ClassInfo*)cpe)->getMyClassIndex()];
			if ( cpeb->type () == TAG_UTF8 ) {
				myParent = myClassLoader->getClass(((ConstantUTF8*)cpeb)->getMyJavaString() );
				}
			else {
				kprintf( "urJavaClass::urJavaClass() -- illegal constant pool entry (%d) while fetching parent, aborting.\n", cpeb->type() );
				abort();	
				}
			}
		else {
			kprintf( "urJavaClass::urJavaClass() -- illegal constant pool entry (%d) while fetching parent, aborting.\n", cpe->type() );
			abort();
			}			
#endif

		myParentsFList = myParent->myFieldList;
		}

	myFieldList = FieldList::generateFieldList( is, this, myParentsFList );

    /* Generate the method list. */
	MethodList * myParentsMList = NULL;
	if ( myParent != NULL ) {
		myParentsMList = myParent->myMethodList;
		}
    myMethodList = MethodList::generateMethodList( is, this, myParentsMList );

    /* Generate the attribute list. */
    myAttributeList = AttributeList::generateAttributeList( is, this );

    /* Perform verification? */
    } /* end urJavaClass() */

bool urJavaClass::verifyStream( istream & is ) {
    jju32 magic;
    jju16 major, minor;

    is >> magic >> major >> minor;

    if ( magic != 0xCAFEBABE ) { return JJFALSE; }
    if ( major != 3 ) { return JJFALSE; }
    if ( minor != 45 ) { return JJFALSE; }

    return JJTRUE;
    } /* end verifyStream() */

JavaString * JavaClass::getMyName() {
    if ( myName != NULL ) { return myName; }

    CPEntry * cpe = (*myConstantPool)[myThisClassIndex];
    if( cpe->type() != TAG_CLASSINFO ) {
        kprintf( "urJavaClass::getMyName() -- erroneous constant pool tag (%d), aborting.\n", cpe->type() );
        abort();
        }

#ifdef DONT_USE_DC
    CPEntry * cu8 = (*myConstantPool)[((ClassInfo*)cpe)->getMyClassIndex()];
    if ( cu8->type() != TAG_UTF8 ) {
        kprintf( "urJavaClass::getMyName() -- erroneous secondary constant pool tag (%d), aborting.\n", cpe->type() );
        abort();
        }
        
    myName = ((ConstantUTF8*)cu8)->getMyJavaString();
#else
    if ( ClassInfo * cip = dynamic_cast<ClassInfo*>( (*myConstantPool)[myThisClassIndex] ) ) {
	    if ( ConstantUTF8 * cu8 = dynamic_cast<ConstantUTF8*>( (*myConstantPool)[cip->getMyClassIndex()] ) ) {
	        myName = cu8->getMyJavaString();
	        } else {
	        kprintf( "urJavaClass::getMyName() -- erroneous secondary constant pool tag (%d), aborting.\n", cpe->type() );
	        abort();
	        }
        } else {
        kprintf( "urJavaClass::getMyName() -- erroneous name index, aborting.\n" );
        abort();
        }
#endif    
    return myName;
    } /* end getMyName() */


/* JavaClassInstance */

JavaClassInstance::JavaClassInstance( JavaClass * myClass ) : JavaClass( myClass ) {
	myClassFields = new ClassFields( this->myFieldList, this->myConstantPool );
	}

bool JavaClassInstance::isInstanceOf( JavaClassInstance * jci ) {
	#warning JavaClassInstance:isInstanceOf( JavaClassInstance * jci ) is unimplemented.
	return true;
	} /* end isInstanceOf() */

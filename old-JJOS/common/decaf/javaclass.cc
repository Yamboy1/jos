/*
 * javaclass.cc
 * 
 * defines the C++ class which
 * represents a given Java class
 * to the decaf JVM.
 */

#include "javaclass.h"
#include "cpentry.h"
#include "assert.h"

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
    /* fetch the constant string */
	ConstantUTF8 * cu8 = NULL;
	ASSERT_CAST( cu8, getMyConstant( index ), ConstantUTF8 *, CPEntry *,
				 "JavaClass::getMyConstantUTF8()", "constant string" );
				 
	return cu8->getMyJavaString();
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
		/* fetch my parent class constant (reference) */
		ClassInfo * ci = NULL;
		ASSERT_CAST( ci, (*myConstantPool)[mySuperClassIndex], ClassInfo *, CPEntry *,
					 "urJavaClass::urJavaClass()", "parent class symbolic reference" );
					 
		/* fetch my parent class's name */
		ConstantUTF8 * cu8 = NULL;
		ASSERT_CAST( cu8, (*myConstantPool)[ci->getMyClassIndex()], ConstantUTF8 *, CPEntry *,
					 "urJavaClass::urJavaClass()", "parent class name" );

		/* fetch my parent class */
		myParent = myClassLoader->getClass( cu8->getMyJavaString() );
		 
		/* update the field list */
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

	ClassInfo * ci = NULL;
	ASSERT_CAST( ci, (*myConstantPool)[myThisClassIndex], ClassInfo *, CPEntry *, "JavaClass::getMyName()", "class info" );

	ConstantUTF8 * cu8 = NULL;
	ASSERT_CAST( cu8, (*myConstantPool)[ci->getMyClassIndex()], ConstantUTF8 *, CPEntry *, "JavaClass::getMyName()", "class name" );

	myName = cu8->getMyJavaString();

    /* myName is now a class specification.
     * $$$$ $$$$: As long as ClassLoader parses specifications,
     * we're OK. */
    return myName;
    } /* end getMyName() */


/* JavaClassInstance */

JavaClassInstance::JavaClassInstance( JavaClass * myClass ) : JavaClass( myClass ) {
	myClassFields = new ClassFields( this->myFieldList, this->myConstantPool );
	}

bool JavaClassInstance::isInstanceOf( JavaClassInstance * jci ) {
	#warning JavaClassInstance:isInstanceOf( JavaClassInstance * jci ) is unimplemented.

#ifdef NOT_READY_YET

	/* definition:
	 * 
	 * (1) if this is a non-array class, it is an instance of jci iff either
	 *   (A) it is, or is a subclass of, jci OR
	 *   (B) it implements the interface jci.
	 * (2) if this an interface, it is an instance of jci iff either
	 *   (A) jci is java.lang.Object OR
	 *   (B) it is, or is a subinterface of, jci.
	 * (3) if this an array, it is an instance of jci iff either
	 *   (A) jci is java.lang.Object OR
	 *   (B) jci is an array of the same type (reference by these rules, or primitive exactly) OR
	 *   (B) jci is an interface supported by arrays.  (java.lang.Cloneable, java.io.Serializable)
	 *
	 *
	 * If we make this a virtual function, (3) will be taken care of by the JCI subclass implementing
	 * arrays.  One and two are fairly simple conditions, and will be handled here. */

	/* definition:
	 * equivalance and inheritance MUST be checked by name /and/ ClassLoader. */

	/* $$$$ $$$$: do we need to check for illegal casts? */

	/* are we the same class? */
	if( (this->getMyClassLoader() == jci->getMyClassLoader) && (* this->getMyName() == jci->getMyName()) )
		return true;
		}

	/* ... more ... */	

#endif
	
	return true;
	} /* end isInstanceOf() */

/*
 * classloader.cc
 * 
 * defines the default primordial
 * classloader for the decaf JVM.
 */

#include "classloader.h"

ClassLoader::ClassLoader() {
    myRamDisk = NULL;
    /* $$$: dynamic hashtable. */
    myClassHashTable = new ClassHashTable( 1024 );
    if ( myClassHashTable == NULL ) {
        kprintf( "ClassLoader::ClassLoader() -- unable to allocate new hashtable, aborting.\n" );
        abort();
        }
    }

void ClassLoader::setMyRamDisk( jbRamDisk * jbr ) {
    if ( jbr == NULL ) {
        kprintf( "ClassLoader::setMyRamdisk() -- NULL ramdisk, aborting.\n" );
        abort();
        }
    myRamDisk = jbr;
    }

jbRamDisk * ClassLoader::getMyRamDisk() {
    return myRamDisk;
    }

JavaClassInstance * ClassLoader::getClass( char * name ) {
    JavaString js( name );
    return getClass( &js );
    }

JavaClassInstance * ClassLoader::getClass( JavaString * js ) {
    /* Have we already done this class? */
    JavaClassInstance * jci = myClassHashTable->lookup( js );
    if ( jci != NULL ) { return jci; }

	/* Is it an array? */
	if ( (*js)[0] == '[' ) {
		/* The only place an array class is truly necessary is to handle
		 * invoke* operations called on an array instance.
		 * 
		 * The other consideration is for instanceof/checkcast operations.
		 * Both involve a single object reference and an index into the
		 * current class's constant pool defining the type to check against.
		 * 
		 * It would be elegant, then, to use JCI to handle instanceof/checkcast
		 * operations by overloading a method; the normal method would check
		 * interface/class casting (determined by the ACC_INTERFACE flag in
		 * myAccessFlags), and the method overloaded by the array class would
		 * handle itself that way.
		 * 
		 * However, adding a(nother) virtual method to JCI may nix things,
		 * and subclassing it could get very obnoxious.  The solution is to
		 * fall back to what I did before and make checkcast/instanceof a
		 * method on JavaObject, rather than JCI.
		 * 
		 * In both cases, we need to return a JCI of j.l.o., possibly adding
		 * the 'length' field.  (Which could be interned?)
		 */
	
		kprintf( "ClassLoader::getClass() -- handle arrays.\n" );
		abort();
		}

    /* We haven't done this class yet, so
       fetch its definition from the ramdisk. */
    JavaString className = *js + ".class";

    jbRamDiskStream * jbrds = myRamDisk->openStream( className.c_str() );
    if ( jbrds == NULL ) {
        kprintf( "ClassLoader::getClass() -- unable to locate %s, aborting.\n", className.c_str() );
        abort();
        }
    istream is( jbrds );        

    /* generate a JavaClassInstance from the stream. */
    jci = JavaClass::generateClassInstance( is, this );

    if ( jci == NULL ) {
        kprintf( "ClassLoader::getClass() -- JavaClass failed to generate %s, aborting.\n", js->c_str() );
        abort();
        }
        
    return jci;
    } /* end getClass() */

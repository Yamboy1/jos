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

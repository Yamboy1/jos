/*
 * classloader.h
 * 
 * declares the default primordial
 * classloader for the decaf JVM.
 */

#ifndef decaf_classloader
#define decaf_classloader

class ClassLoader;

#include "jbhash.h"
#include "javastring.h"
#include "javaclass.h"
#include "istream.h"

typedef jbHashtable< JavaString *, JavaClassInstance * > ClassHashTable;

class ClassLoader {

  public:
    ClassLoader();

    void setMyRamDisk ( jbRamDisk * jbr );
    jbRamDisk * getMyRamDisk();

    JavaClassInstance * getClass ( JavaString * js );
    JavaClassInstance * getClass ( char * name );

  protected:
    jbRamDisk * myRamDisk;
    ClassHashTable * myClassHashTable;

}; /* end class ClassLoader */

#endif

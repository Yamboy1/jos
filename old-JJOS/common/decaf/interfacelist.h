/*
 * interfacelist.h
 * 
 * declares the class which
 * represents the list of
 * interfaces that a Java class
 * implements.
 */

#ifndef decaf_interfacelist
#define decaf_interfacelist

class InterfaceList;

#include "istream.h"
#include "d_types.h"
#include "javaclass.h"

class InterfaceList {
  public:
    InterfaceList();

    jju16 getMyInterfaceCount() { return myInterfaceCount; }

    static InterfaceList * generateInterfaceList( istream & is, JavaClass * jc );

    virtual jju16 operator [] (int index) = 0;

  protected:
    jju16 myInterfaceCount;

}; /* end class InterfaceList */

class RawInterfaceList;

class RawInterfaceList : public InterfaceList {
  friend InterfaceList;
  
  public:
    RawInterfaceList();

    static RawInterfaceList * generateRawInterfaceList( istream & is, JavaClass * jc );

    jju16 operator [] (int index);

  protected:
    jju16 * myInterfaceIndexList;

}; /* end class RawInterfaceList */

#endif

/*
 * methodlist.h
 * 
 * declares the C++ class which
 * represents a Java class's
 * method list.
 */

#ifndef decaf_methodlist
#define decaf_methodlist

class MethodList;
class MethodInfo;
class RawMethodList;
class RawMethodInfo;

#include "d_types.h"
#include "istream.h"
#include "attributelist.h"
#include "constantpool.h"

class MethodList {

  public:
    MethodList();

    virtual MethodInfo * operator [] ( int index ) = 0;

    jju16 getMyMethodCount() { return myMethodCount; }

    static MethodList * generateMethodList( istream & is, JavaClass * jc, MethodList * parent );

  protected:
    jju16 myMethodCount;

}; /* end class MethodList */

class RawMethodList : public MethodList {
  friend MethodList;

  public:
    RawMethodList();

    MethodInfo * operator [] ( int index );

    static MethodList * generateRawMethodList( istream & is, JavaClass * jc, MethodList * parent );

  protected:
    MethodInfo ** myMethodList;
    
}; /* end class RawMethodList */

class MethodInfo {

  public:
    MethodInfo();

    static MethodInfo * generateMethodInfo( istream & is, JavaClass * jc );

    virtual JavaString * getMyName( ConstantPool * cp ) = 0;
    virtual JavaString * getMySig( ConstantPool * cp ) = 0;
    virtual CodeAttribute * getMyCodeAttribute( ConstantPool * cp ) = 0;
    virtual bool isSynchronized() = 0;
    virtual bool isNative() = 0;
    virtual jint getMyArgumentCount() = 0;

  protected:
    jju16 myAccessFlags;
    
}; /* end class MethodInfo */

class RawMethodInfo : public MethodInfo {

  public:
    RawMethodInfo();

    static RawMethodInfo * generateRawMethodInfo( istream & is, JavaClass * jc );

    JavaString * getMyName( ConstantPool * cp );
    JavaString * getMySig( ConstantPool * cp );
    CodeAttribute * getMyCodeAttribute( ConstantPool * cp );
    bool isSynchronized() { return myAccessFlags & ACC_SYNCHRONIZED; }
    bool isNative() { return myAccessFlags & ACC_NATIVE; }
    jint getMyArgumentCount() {
		#error RawMethodInfo::getMyArgumentCount unimplemented!  (see old parseMethodSig())
		return 0;
    	}

  protected:
    jju16   myNameIndex;
    jju16   myDescriptorIndex;
    AttributeList * myAttributeList;

    JavaString * myName;
    JavaString * mySig;

}; /* end class RawMethodInfo */

#endif

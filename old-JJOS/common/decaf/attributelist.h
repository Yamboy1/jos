/*
 * attributelist.h
 * 
 * declares the C++ which
 * represents an attribute
 * list for a Java class.
 */

#ifndef decaf_attributelist
#define decaf_attributelist

class AttributeList;
class RawAttributeList;
class AttributeInfo;
class RawAttributeInfo;
class UnknownRawAttribute;
class RawConstantValueAttribute;
class ExceptionTable;
class ExceptionTableEntry;
class RawExceptionsAttribute;

class ConstantValueAttribute;
class CodeAttribute;
class ExceptionsAttribute;

#define ACC_PUBLIC        0x0001
#define ACC_PRIVATE       0x0002
#define ACC_PROTECTED     0x0004
#define ACC_STATIC        0x0008
#define ACC_FINAL         0x0010
#define ACC_SYNCHRONIZED  0x0020
#define ACC_SUPER         0x0020
#define ACC_VOLATILE      0x0040
#define ACC_TRANSIENT     0x0080
#define ACC_NATIVE        0x0100
#define ACC_INTERFACE     0x0200
#define ACC_ABSTRACT      0x0400

#include "d_types.h"
#include "istream.h"
#include "javaclass.h"
#include "constantpool.h"

class AttributeList {

  public:
    AttributeList();

    virtual AttributeInfo * operator []( int index ) = 0;
    jju16 getMyAttributeCount() { return myAttributeCount; }

    static AttributeList * generateAttributeList( istream & is, JavaClass * jc );

  protected:
    jju16 myAttributeCount;

}; /* end class AttributeList */

class RawAttributeList : public AttributeList {
  friend AttributeList;

  public:
    RawAttributeList();

    AttributeInfo * operator []( int index );

    static RawAttributeList * generateRawAttributeList( istream & is, JavaClass * jc );

  protected:
    AttributeInfo ** myAttributeList;

}; /* end class RawAttributeList */


class AttributeInfo {

  public:
    AttributeInfo();

    static AttributeInfo * generateAttributeInfo( istream & is, JavaClass * jc );

    virtual JavaString * getMyName( ConstantPool * cp ) = 0;

  protected:

}; /* end class AttributeInfo */ 

class RawAttributeInfo : public AttributeInfo {
  friend AttributeInfo;

  friend UnknownRawAttribute;
  friend RawConstantValueAttribute;
  friend CodeAttribute; /* would be RawCodeAttribute; see note at CodeAttribute */
  friend RawExceptionsAttribute;

  public:
    RawAttributeInfo();

    jju16 getMyNameIndex() { return myNameIndex; }
    jju32 getMyAttributeLength() { return myAttributeLength; }
    JavaString * getMyName( ConstantPool * cp );
    
    static RawAttributeInfo * generateRawAttributeInfo( istream & is, JavaClass * jc );

  protected:
    jju16 myNameIndex;
    jju32 myAttributeLength;

    JavaString * myName;
}; /* end class RawAttributeInfo */


class UnknownRawAttribute : public RawAttributeInfo {

  friend RawAttributeInfo;

  public:
    UnknownRawAttribute();

    jju8 * getMyUnknownAttribute() { return myUnknownAttribute; }

    static UnknownRawAttribute * generateUnknownRawAttribute( istream & is, RawAttributeInfo * rai );

  protected:
    jju8 * myUnknownAttribute;

}; /* end class UnknownRawAttribute */

class RawConstantValueAttribute : public RawAttributeInfo {
  friend RawAttributeInfo;

  public:
    RawConstantValueAttribute();

    jju16 getMyConstantValueIndex() { return myCVIndex; }

    static RawConstantValueAttribute * generateRawConstantValueAttribute( istream & is, RawAttributeInfo * rai );

  protected:
    jju16 myCVIndex;

}; /* end class RawConstantValueAttribute */

/* would have preferred this to have been an interface class,
   but the RawAttributeInfo stuff takes precedence, and we
   can't use multiple virtual inheritance on the i386 build. */
class CodeAttribute : public RawAttributeInfo {

  public:
    CodeAttribute();

    static CodeAttribute * generateCodeAttribute( istream & is, RawAttributeInfo * rai, JavaClass * jc );

    jju16 getMyMaxStack() { return myMaxStack; }
    jju16 getMyMaxLocals() { return myMaxLocals; }
    jju32 getMyCodeLength() { return myCodeLength; }
    jju8 * getMyCode() { return myCode; }

    ExceptionTable * getMyExceptionTable();
    AttributeList * getMyAttributeList();

  protected:
    jju16 myMaxStack;
    jju16 myMaxLocals;
    jju32 myCodeLength;
    jju8 * myCode;
  
    ExceptionTable * myExceptionTable;
    AttributeList * myAttributeList;

}; /* end class CodeAttribute */

class ExceptionTable {

  public:
    ExceptionTable();

    jju16 getMyExceptionCount() { return myExceptionCount; }

    static ExceptionTable * generateExceptionTable( istream & is );  

  protected:
    jju16 myExceptionCount;
    ExceptionTableEntry ** myExceptionTable;
    
}; /* end class ExceptionTable */

class ExceptionTableEntry {

  public:
    ExceptionTableEntry() { myStartPC = 0; myEndPC = 0; myHandlerPC = 0; myCatchTypeIndex = 0; }
    ExceptionTableEntry( jju16 startPC, jju16 endPC, jju16 handlerPC, jju16 catchTypeIndex ) {
        myStartPC = startPC; myEndPC = endPC; myHandlerPC = handlerPC; myCatchTypeIndex = catchTypeIndex; }

    jju16 myStartPC;
    jju16 myEndPC;
    jju16 myHandlerPC;
    jju16 myCatchTypeIndex;

}; /* end class ExceptionTableEntry */

class RawExceptionsAttribute : public RawAttributeInfo {
  friend RawAttributeInfo;

  public:
    RawExceptionsAttribute();

    static RawExceptionsAttribute * generateRawExceptionsAttribute( istream & is, RawAttributeInfo * rai );

  protected:
    jju16 myExceptionCount;
    jju16 * myExceptionList;

}; /* end class RawExceptionsAttribute */

#endif

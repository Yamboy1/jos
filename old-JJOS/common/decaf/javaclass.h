/*
 * javaclass.h
 * 
 * declares the C++ class which
 * represents a given Java class
 * to the decaf JVM.
 */

#ifndef decaf_javaclass
#define decaf_javaclass

class JavaClass;
class urJavaClass;
class JavaClassInstance;
class JavaObject;

#include "typed.h"

class istream;
class ClassLoader;
#include "constantpool.h"
class JavaString;
class CodeAttribute;
class MethodList;
class MethodInfo;
class InterfaceList;
class FieldList;
class AttributeList;
class ClassFields;

#include "javathread.h"

/*
 * A few words of explanation: every class
 * loaded into the VM passes through
 * urJavaClass::urJavaClass( ramdiskstream, classloader ),
 * including classes loaded by calling defineClass(),
 * which generates a ramdiskstream from the byte array
 * passed to it, and hands that off to the constructor.
 * 
 * JavaClass::generateClass() is used by the primoridal
 * classloader when getClass() is called and the class
 * is not already defined.
 * 
 * urJavaClass is the subclass of JavaClass which
 * represents the raw class data as parsed out of
 * the ramdiskstream according to the ClassFile
 * specification in the JVM specification.
 * 
 * In general, classes with static generate methods
 * are superclasses for a set of implementations
 * that vary in stages of resolution.
 *
 * Furthermore: a JavaClass is a class definition, i.e.
 * it does NOT have class-static data (except constant
 * initial values) associated with it.
 *
 * A JavaClassInstance is the decaf JVM's native counterpart
 * to a java.lang.Class instance, e.g. the return value of
 * `object.getClass();'; it is a JavaClass and a structure
 * (ClassFields) for storing its class's fields/constants.
 */

class JavaClass : Typed {

  public:
    JavaClass();
    JavaClass( JavaClass * jc );

    static JavaClassInstance * generateClassInstance( istream & is, ClassLoader * cl );

    CPEntry * getMyConstant( jju16 index );
    ConstantPool * getMyConstantPool() { return myConstantPool; }
    FieldList * getMyFieldList() { return myFieldList; }
    JavaString * getMyConstantUTF8( jju16 index );
    ClassLoader * getMyClassLoader() { return myClassLoader; }
    MethodInfo * getMethodInfo( JavaString * method, JavaString * methodSig );
    CodeAttribute * getMethodCode( JavaString * method, JavaString * methodSig );

    virtual JavaString * getMyName(); // does this need to be virtual?

    PrimitiveType type() { return PT_CLASS; }

  protected:
    ClassLoader * myClassLoader;

    ConstantPool * myConstantPool;
    InterfaceList * myInterfaceList;
    FieldList * myFieldList;
    MethodList * myMethodList;
    AttributeList * myAttributeList;

    jju16 myAccessFlags;

    jju16 myThisClassIndex;
    jju16 mySuperClassIndex;

    JavaString * myName;
}; /* end class JavaClass */

class JavaClassInstance : public JavaClass, public Threadable {
  friend JavaClass;
  friend urJavaClass;

  public:
    JavaClassInstance( JavaClass * myClass );

    ClassFields * getMyClassFields() { return myClassFields; }
    PrimitiveType type() { return PT_CLASS_INSTANCE; }

  protected:
    ClassFields * myClassFields;

}; /* end JavaClassInstance */

class urJavaClass : public JavaClass {
  friend JavaClass;
  friend JavaClassInstance;

  public:
    urJavaClass( istream & is, ClassLoader * cl );

  protected:
    static bool verifyStream( istream & is );
    
}; /* end urJavaClass */

#include "classloader.h"
#include "istream.h"
#include "constantpool.h"
#include "interfacelist.h"
#include "fieldlist.h"
#include "methodlist.h"
#include "attributelist.h"
#include "javathread.h"
#include "javaobject.h"

#endif

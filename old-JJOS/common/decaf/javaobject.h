/*
 * javaobject.h
 *
 * declares the C++
 * class which represents
 * an instance of a Java
 * class -- a Java object.
 */

#ifndef decaf_javaobject
#define decaf_javaobject

class JavaObject;
class JavaArrayObject;
class JavaObjectArray;

#include "jbarray.h"
#include "javathread.h"
#include "javaclass.h"
#include "fieldlist.h"

class JavaObject : JavaClassInstance {
  public:
    JavaObject( JavaClassInstance * jci );

    InstanceFields * getMyInstanceFields() { return myInstanceFields; }
    PrimitiveType type() { return PT_OBJECT; }

    bool isInstanceOf( ClassInfo * cpe ); /* "symbolic class reference" from VM spec / bytecode op */

  protected:
    InstanceFields * myInstanceFields;

}; /* end JavaObject */

class JavaArrayObject : public JavaObject {
  public:
    JavaArrayObject( JavaClassInstance * jLObject, jju32 size );
    
    static JavaArrayObject * generateJavaArrayObject( JavaClassInstance * jLObject, jint size, PrimitiveType pt );
    static JavaArrayObject * generateJavaArrayObject( JavaClassInstance * jLObject, jint size, JavaClassInstance * jci );

    virtual jju32 getMySize() { return mySize; };
    virtual PrimitiveType arrayType() = 0;

  protected:
    jju32 mySize;

}; /* end JavaArrayObject */

class JavaObjectArray : public JavaArrayObject, jbArray<JavaClassInstance *> {
  public:
    JavaObjectArray( JavaClassInstance * jLObject, jint size, JavaClassInstance * jci ) : JavaArrayObject( jLObject, size), jbArray<JavaClassInstance *>( size ) { myArrayType = jci; }
   
    JavaClassInstance * getElement( jju32 index ) { return load( index ); }
	void setElement( jju32 index, JavaClassInstance * jci );
    
    jju32 getMySize() { return size(); }
    PrimitiveType type() { return PT_OBJECT_ARRAY; }
    PrimitiveType arrayType() { return PT_CLASS_INSTANCE; }

  protected:
    JavaClassInstance * myArrayType;

}; /* end JavaObjectArray */

template <class T>
class JavaPrimitiveArray : public JavaArrayObject, jbArray<T *> {
  public:
  	JavaPrimitiveArray( JavaClassInstance * jLObject, jint size, PrimitiveType pt ) : JavaArrayObject( jLObject, size ), jbArray< T* >( size ) { this->myPT = pt;};

	T * getElement( jju32 index ) { return load( index ); }
	void setElement( jju32 index, T * t ) { store( index, t ); }

	jju32 getMySize() { return size(); }
	PrimitiveType type() { return PT_ARRAY; }
	PrimitiveType arrayType() { return myPT; }

  protected:
  	PrimitiveType myPT;
  	
}; /* end JavaPrimitiveArray */

#endif

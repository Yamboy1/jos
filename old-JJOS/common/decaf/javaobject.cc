/*
 * javaobject.h
 *
 * defines the C++
 * class which represents
 * an instance of a Java
 * class -- a Java object.
 */

#include "javaobject.h"


/* JavaObject */

JavaObject::JavaObject( JavaClassInstance * jci ) : JavaClassInstance( jci ) {
	this->myInstanceFields = new InstanceFields( this->myFieldList, this->myConstantPool );
	}


/* JavaArrayObject */

JavaArrayObject::JavaArrayObject( JavaClassInstance * jLObject, jju32 size ) : JavaObject( jLObject ) {
	/* add the array-specific fields */
	this->mySize = size;
	this->myInstanceFields = new ArrayInstanceFields( this->myInstanceFields, size );
   	}

JavaArrayObject * JavaArrayObject::generateJavaArrayObject( JavaClassInstance * jcip, jint size, PrimitiveType pt ) {

	switch ( pt ) {
		case PT_LONG:
			return new JavaPrimitiveArray< jlong >( jcip, size, pt );
		case PT_INT:
			return new JavaPrimitiveArray< jint >( jcip, size, pt );
		case PT_SHORT:
			return new JavaPrimitiveArray< jshort >( jcip, size, pt );
		case PT_BYTE:
			return new JavaPrimitiveArray< jbyte >( jcip, size, pt );
		case PT_DOUBLE:
			return new JavaPrimitiveArray< jdouble >( jcip, size, pt );
		case PT_FLOAT:
			return new JavaPrimitiveArray< jfloat >( jcip, size, pt );
		case PT_CHAR:
			return new JavaPrimitiveArray< /*jchar*/ jju16 >( jcip, size, pt );
		case PT_BOOLEAN:
			return new JavaPrimitiveArray< jbool >( jcip, size, pt );
		
		default:
			kprintf( "JavaArrayObject::generateJavaArrayObject() -- illegal primitive type, aborting.\n" );
			abort();
			/* should never happen */
			return NULL;
			break;
		} /* end switch */
	} /* end generateJavaArrayObject() */
	
JavaArrayObject * JavaArrayObject::generateJavaArrayObject( JavaClassInstance * jcip, jint size, JavaClassInstance * jci ) {
	return new JavaObjectArray( jcip, size, jci );
	}


/* JavaObjectArray */

void JavaObjectArray::setElement( jju32 index, JavaClassInstance * jci ) {
		if ( jci != NULL && jci->isInstanceOf( myArrayType ) ) {
    		store( index, jci );
    		} else {
    		kprintf( "JavaObjectArray::setElement() -- attempt to set element of an illegal type, aborting.\n" );
    		abort();
    		}
    	}

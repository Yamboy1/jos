/*
 * fieldlist.h
 * 
 * declares the C++ class
 * representing a list of
 * fields in a Java class.
 */

#ifndef decaf_fieldlist
#define decaf_fieldlist

class FieldList;
class RawFieldList;
class FieldInfo;
class RawFieldInfo;

class ClassFields;
class ArrayClassFields;
class InstanceFields;
class ArrayInstanceFields;

#include "d_types.h"
#include "istream.h"
#include "attributelist.h"

class FieldList {

  public:
    FieldList() { myFieldCount = 0; }

    static FieldList * generateFieldList( istream & is, JavaClass * jc, FieldList * parent );

    jju16 getMyFieldCount() { return myFieldCount; }

	virtual FieldList * append( FieldInfo * fi ) = 0;
    virtual FieldInfo * operator [] ( int index ) = 0;
    
  protected:
    jju16 myFieldCount;
    
}; /* end class FieldList */

class RawFieldList : public FieldList {
  friend FieldList;

  public:
    RawFieldList() { myFieldCount = 0; myFieldInfoList = NULL; }

    static FieldList * generateRawFieldList( istream & is, JavaClass * jc, FieldList * parent );

	FieldList * append( FieldInfo * fi );
    FieldInfo * operator [] ( int index );

  protected:
    FieldInfo ** myFieldInfoList;
    
}; /* end class RawFieldList */

class FieldInfo {

  public:
    FieldInfo() {}

    static FieldInfo * generateFieldInfo( istream & is, JavaClass * jc );

    virtual jju16 getMyAccessFlags() = 0;
    virtual JavaString * getMyName( ConstantPool * cp ) = 0;
    virtual JavaString * getMyDescriptor( ConstantPool * cp ) = 0;
    virtual bool hasInitialValue( ConstantPool * cp ) = 0;  /* ConstantValueAttribute */
    virtual jju16 getMyValueIndex( ConstantPool * cp ) = 0; /* from the ConstantValueAttribute */

    virtual bool isClassVariable() = 0;  /* as determined by myAccessFlags */
    virtual bool isConstantValue() = 0;  /* as determined by myAccessFlags */
    
}; /* end class FieldInfo */

class ArrayFieldInfo : public FieldInfo {
  public:
    ArrayFieldInfo() {}

    JavaString * getMyName( ConstantPool * cp ) {
    	/* intern! */
    	return new JavaString( "length" );
    	}
    JavaString * getMyDescriptor( ConstantPool * cp ) {
    	/* intern! */
    	return new JavaString( "I" );
    	}
   	/* while it /does/ have an initial value, it doesn't have an entry
   	 * in the constant pool... */
   	bool hasInitialValue( ConstantPool * cp ) { return false; }
   	jju16 getMyValueIndex(  ConstantPool * cp ) { return 0; }

   	bool isClassVariable() { return false; }
   	bool isConstantValue() { return true; }
   	jju16 getMyAccessFlags() { return ACC_PUBLIC & ACC_FINAL; }
}; /* end ArrayFieldInfo */

class RawFieldInfo : public FieldInfo {

  public:
    RawFieldInfo() { myAccessFlags = 0; myNameIndex = 0; myDescriptorIndex = 0; myAttributeList = NULL; }
  
    static RawFieldInfo * generateRawFieldInfo( istream & is, JavaClass * jc );

    JavaString * getMyName( ConstantPool * cp );
    JavaString * getMyDescriptor( ConstantPool * cp );
    bool hasInitialValue( ConstantPool * cp );
    jju16 getMyValueIndex( ConstantPool * cp );
    
    bool isClassVariable() { return myAccessFlags & ACC_STATIC; }
    bool isConstantValue() { return myAccessFlags & ACC_FINAL; }
    jju16 getMyAccessFlags() { return myAccessFlags; }
    
  protected:
    jju16 myAccessFlags;
    jju16 myNameIndex;
    jju16 myDescriptorIndex;
    AttributeList * myAttributeList;

	RawConstantValueAttribute * findMyCV( ConstantPool * cp );

}; /* end class RawFieldInfo */


#define CONSTANT_FIELD 0x01
#define CLASS_FIELD    0x02
#define INSTANCE_FIELD 0x04

/* Because fields hold double-word values w/o using two entries,
 * we'll coerce a pointer to a double into the jju32 field of value;
 * same for the constant strings */
struct FieldData { JavaWord value; jju8 flags; };
bool matchDescriptorAndPrimitiveType( JavaString * desc, PrimitiveType pt );

class ClassFields {
  public:
  	ClassFields() { fl = NULL; cp = NULL; }
    ClassFields( FieldList * fl, ConstantPool * cp );

    jju32 getFieldIndex( JavaString * name, JavaString * type );

    JavaWord getClassField( jju32 index );
    void setClassField( jju32 index, JavaWord value );

    JavaClassInstance * getStaticObject( jju32 index );
    jbool getStaticBoolean( jju32 index );
    jbyte getStaticByte( jju32 index );
    jshort getStaticShort( jju32 index );
    jint getStaticInt( jju32 index );
    jlong getStaticLong( jju32 index );
    jfloat getStaticFloat( jju32 index );
    jdouble getStaticDouble( jju32 index );    
    
  protected:
    FieldList * fl;
    FieldData * fd;

    /* Strictly for convenience.  In truth, though, a ClassFields
     * shouldn't ever have its constant pool changed anyway... */
    ConstantPool * cp;
	
}; /* end class ClassFields */

class InstanceFields : public ClassFields {
  friend ArrayInstanceFields;

  public:
  	InstanceFields() : ClassFields() {}
    InstanceFields( FieldList * fl, ConstantPool * cp ) : ClassFields( fl, cp ) {}

    JavaWord getInstanceField( jju32 index );
    void setInstanceField( jju32 index, JavaWord value );

    JavaClassInstance * getInstanceObject( jju32 index );
    jbool getInstanceBoolean( jju32 index );
    jbyte getInstanceByte( jju32 index );
    jshort getInstanceShort( jju32 index );
    jint getInstanceInt( jju32 index );
    jlong getInstanceLong( jju32 index );
    jfloat getInstanceFloat( jju32 index );
    jdouble getInstanceDouble( jju32 index );    
   
}; /* end class InstanceFields */

class ArrayInstanceFields : public InstanceFields {
  public:
  	ArrayInstanceFields( InstanceFields * parent, jju32 length ) : InstanceFields() {
		/* keep the constant pool reference */
		this->cp = parent->cp;
		
		/* append the length field */
		FieldInfo * fi = new ArrayFieldInfo();
		if ( fi == NULL ) {
			kprintf( "ArrayInstanceFields::ArrayInstanceFields() -- unable to allocate length field, aborting.\n" );
			abort();
			}
		this->fl = parent->fl->append( fi );

		/* generate the field table */
		this->fd = new FieldData[ parent->fl->getMyFieldCount() + 1 ];
		if ( this->fd == NULL ) {
			kprintf( "ArrayInstanceFields::ArrayInstanceFields() -- unable to allocate field data, aborting.\n" );
			abort();
			}
		jju16 x = 0;
		for ( x = 0; x < parent->fl->getMyFieldCount(); x++ ) {
			this->fd[x] = parent->fd[x];
			}

		/* fill out the length field data */
		this->fd[x].value = length;
		this->fd[x].flags = ACC_PUBLIC & ACC_FINAL;
  		}
}; /* end class ArrayInstanceFields */

#endif

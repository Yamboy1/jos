/*
 * cpentry.h
 * 
 * declares the classes which represent
 * a Java class's constant pool entry.
 * 
 * subclasses of this class differ
 * by type and degree of resolution.
 */

#ifndef decaf_cpentry
#define decaf_cpentry

class CPEntry;

#include "javastring.h"
#include "istream.h"
#include "javaword.h"
#include "javaclass.h"

/* From the class file specification. */
#define TAG_UTF8 1
#define TAG_ILLEGAL 2
#define TAG_INTEGER 3
#define TAG_FLOAT 4
#define TAG_LONG 5
#define TAG_DOUBLE 6
#define TAG_CLASSINFO 7
#define TAG_STRING 8
#define TAG_FIELDREF 9
#define TAG_METHODREF 10
#define TAG_INTERFACEMETHODREF 11
#define TAG_NAMEANDTYPE 12

#define TAG_REFERENCE -1

class CPEntry {

  public:
    CPEntry();

    static CPEntry * generateCPEntry( istream & is );

    virtual bool usesTwoEntries() = 0;
    virtual int type() = 0;

  protected:
    
}; /* end class CPEntry */

/* Strictly -- this is a constant value,
 * which MAY be used to generate a constant java.lang.String in
 * a CPString entry.  It is, however, much simpler to skip
 * the intermediate step. */
class ConstantUTF8 : public CPEntry {

  public:
    ConstantUTF8();

    static ConstantUTF8 * generateConstantUTF8( istream & s );

    JavaString * getMyJavaString();
    void setMyJavaString( JavaString * js );
    bool usesTwoEntries() { return false; }
    int type () { return TAG_UTF8; }

  protected:
    JavaString * myJavaString;
       
}; /* end class ConstantUTF8 */

class ClassInfo : public CPEntry {

  public:
    ClassInfo() { myJavaClass = NULL; }

    static ClassInfo * generateClassInfo( istream & is ) {
        /* The ClassInfo to return. */
        ClassInfo * ci = new ClassInfo();
        if ( ci == NULL ) {
            kprintf( "ClassInfo::generateClassInfo() -- unable to allocate new class info, aborting.\n" );
            abort();
            }

        /* fetch the index */
        is >> ci->myClassIndex;

        /* return the class info;
         * resolution will set the JavaClass from the index. */
        return ci;
        } /* end generateClassInfo() */

    jju16 getMyClassIndex() { return myClassIndex; }

    JavaClass * getMyClass( ConstantPool * cp = NULL );
    void setMyClass( JavaClass * jc ) {
        if ( myJavaClass != NULL ) {
            kprintf( "ClassInfo::setMyJavaClass() -- attempt to reset the constant, aborting.\n" );
            abort();
            }
        myJavaClass = jc;
        }
    bool usesTwoEntries() { return false; }
    int type() { return TAG_CLASSINFO; }

  protected:
    JavaClass * myJavaClass;
    jju16 myClassIndex;

}; /* end class ClassInfo */    

/* forward declaration of classes and their static methods. */


class Ref : public CPEntry {

  public:
    Ref() { myJavaClass = NULL; myName = NULL; myType = NULL; }

    static Ref * generateRef( istream & is ) {
        /* The field reference to return. */
        Ref * r = new Ref();
        if ( r == NULL ) {
            kprintf( "Ref::generateRef() -- unable to allocate new reference, aborting.\n" );
            abort();
            }

        /* Fetch the name and type, class indices. */
        is >> r->myClassIndex >> r->myNameAndTypeIndex;

        /* return the field reference;
         * resolution will set its class, name, and type. */
        return r;
        } /* end generateRef */

    Ref& operator = ( Ref& rhs ) {
        this->myJavaClass = rhs.myJavaClass;
        this->myName = rhs.myName;
        this->myType = rhs.myType;

        this->myClassIndex = rhs.myClassIndex;
        this->myNameAndTypeIndex = rhs.myNameAndTypeIndex;
        return *this;
        }

    JavaClassInstance * getMyClass( ConstantPool * cp = NULL) ;
    void setMyClass( JavaClassInstance * jci ) {
        if ( myJavaClass != NULL ) {
            kprintf( "Ref::setMyClass() -- attempt to reset the constant, aborting.\n" );
            abort();
            }
        myJavaClass = jci;
        }

    JavaString * getMyName( ConstantPool * cp = NULL );
    void setMyName( JavaString * js ) {
        if ( myName != NULL ) {
            kprintf( "Ref::setMyName() -- attempt to reset the constant, aborting.\n" );
            abort();
            }
        myName = js;
        }

    JavaString * getMyType( ConstantPool * cp = NULL );
    void setMyType( JavaString * js ) {
        if ( myType != NULL ) {
            kprintf( "Ref::setMyType() -- attempt to reset the constant, aborting.\n" );
            abort();
            }
        myType = js;
        }
    
    bool usesTwoEntries() { return false; }
    virtual int type() { return TAG_REFERENCE; }

  protected:
    JavaClassInstance * myJavaClass;
    JavaString * myName;
    JavaString * myType;

    jju16 myClassIndex;
    jju16 myNameAndTypeIndex;
  
}; /* end class Ref */

class FieldRef : public Ref {
  public:
    int type() { return TAG_FIELDREF; }

    static FieldRef * generateFieldRef( istream & is ) {
        FieldRef * fr = new FieldRef();
        if ( fr == NULL ) {
            kprintf( "FieldRef::generateFieldRef() -- unable to allocate new field reference, aborting.\n" );
            abort();
            }
    
        Ref * r = Ref::generateRef( is );
        (Ref)(*fr) = *r;
        delete r;
        return fr;
        }
}; /* end class FieldRef */

class MethodRef : public Ref {
  public:
    int type() { return TAG_METHODREF; }

    static MethodRef * generateMethodRef( istream & is ) {
        MethodRef * mr = new MethodRef();
        if ( mr == NULL ) {
            kprintf( "MethodRef::generateMethodRef() -- unable to allocate new method reference, aborting.\n" );
            abort();
            }
    
        Ref * r = Ref::generateRef( is );
        (Ref)(*mr) = *r;
        delete r;
        return mr;
        }
}; /* end class MethodRef */

class InterfaceMethodRef : public Ref {
  public:
    int type() { return TAG_INTERFACEMETHODREF; }

    static InterfaceMethodRef * generateInterfaceMethodRef( istream & is ) {
        InterfaceMethodRef * imr = new InterfaceMethodRef();
        if ( imr == NULL ) {
            kprintf( "InterfaceMethodRef::generateInterfaceMethodRef() -- unable to allocate new interface method reference, aborting.\n" );
            abort();
            }
    
        Ref * r = Ref::generateRef( is );
        (Ref)(*imr) = *r;
        delete r;
        return imr;
        }
}; /* end class InterfaceMethodRef */

class NameAndType : public CPEntry {

  public:
    NameAndType() { myName = NULL; myType = NULL; }

    static NameAndType * generateNameAndType( istream & is ) {
        NameAndType * nat = new NameAndType();
        if ( nat == NULL ) {
            kprintf( "NameAndType::generateNameAndType() -- unable to allocate new name and type, aborting.\n" );
            abort();
            }

        is >> nat->myNameIndex >> nat->myTypeIndex;
        return nat;
        }

    JavaString * getMyName( ConstantPool * cp = NULL );
    void setMyName( JavaString * js ) {
        if ( myName != NULL ) {
            kprintf( "NameAndType::setMyName() -- attempt to reset the constant, aborting.\n" );
            abort();
            }
        myName = js;
        }
    
    JavaString * getMyType( ConstantPool * cp = NULL );
    void setMyType( JavaString * js ) {
        if ( myType != NULL ) {
            kprintf( "NameAndType::setMyType() -- attempt to reset the constant, aborting.\n" );
            abort();
            }
        myType = js;
        }

    bool usesTwoEntries() { return false; }
    int type() { return TAG_NAMEANDTYPE; }

  protected:
    JavaString * myName;
    JavaString * myType;

    jju16 myNameIndex;
    jju16 myTypeIndex;
}; /* end class NameAndType */

class CPString : public CPEntry {

  public:
    CPString();

    static CPString * generateCPString( istream & is );

    JavaString * getMyJavaString( ConstantPool * cp = NULL );
    void setMyJavaString( JavaString * js );
    bool usesTwoEntries() { return false; }
    int type () { return TAG_STRING; }

  protected:
    JavaString * myJavaString;
    jju16 myCU8Index;

}; /* end class CPString */

class CPInteger : public CPEntry {

  public:
    CPInteger() { myInteger = 0; }

    static CPInteger * generateCPInteger( istream & is ) {
        /* The CPInteger to return. */
        CPInteger * cpi = new CPInteger();
        if( cpi == NULL ) {
            kprintf( "CPInteger::generateCPInteger() -- unable to allocate new CPInteger, aborting.\n" );
            abort();
            }

        /* Fetch the value. */
        jju32 u32;
        is >> u32;
        cpi->myInteger = (jint)JavaWord(u32);
        
        return cpi;
        } /* end generateCPInteger() */
    
    jint getMyInteger() { return myInteger; }
    void setMyInteger( jint ji ) {
        /* since we can't do a real check on constancy, skip it. */
        myInteger = ji;
        }
    bool usesTwoEntries() { return false; }
    int type () { return TAG_INTEGER; }

  protected:
    jint myInteger;

}; /* end class CPInteger */

class CPFloat : public CPEntry {

  public:
    CPFloat() { myFloat = 0; }

    static CPFloat * generateCPFloat( istream & is ) {
        /* The CPFloat to return. */
        CPFloat * cpf = new CPFloat();
        if( cpf == NULL ) {
            kprintf( "CPFloat::generateCPFloat() -- unable to allocate new CPFloat, aborting.\n" );
            abort();
            }

        /* Fetch the value. */
        jju32 u32;
        is >> u32;
        cpf->myFloat = (jfloat)JavaWord(u32);
        
        return cpf;
        } /* end generateCPFloat() */
    
    jfloat getMyFloat() { return myFloat; }
    void setMyFloat( jfloat jf) {
        /* since we can't do a real check on constancy, skip it. */
        myFloat = jf;
        }
    bool usesTwoEntries() { return false; }
    int type () { return TAG_FLOAT; }

  protected:
    jfloat myFloat;

}; /* end class CPFloat */

class CPLong : public CPEntry {

  public:
    CPLong() { myLong = 0; }
    virtual ~CPLong() {
        /* because jlong is a class,
         * we need to explicitly virtualize the destructor. */
        }

    static CPLong * generateCPLong( istream & is ) {
        /* The CPLong to return. */
        CPLong * cpl = new CPLong();
        if ( cpl == NULL ) {
            kprintf( "CPLong::generateCPLong() -- unable to allocate new CPLong, aborting.\n" );
            abort();
            }

        /* Fetch the value. */
        jju32 w1, w2;
        is >> w1 >> w2;
        JavaWord jw1(w1);
        JavaWord jw2(w2);
        cpl->myLong = (jlong)JavaWord::toJLong( jw1, jw2 );

        return cpl;
        } /* end generateCPLong() */

    jlong getMyLong() { return myLong; }
    void setMyLong( jlong jl ) {
        /* since we can't do a real check on constancy, skip it. */
        myLong = jl;
        }

    bool usesTwoEntries() { return true; }
    int type () { return TAG_UTF8; }
        
  protected:
    jlong myLong;

}; /* end class CPLong */
    
class CPDouble : public CPEntry {

  public:
    CPDouble() { myDouble = 0; }

    static CPDouble * generateCPDouble( istream & is ) {
        /* The CPDouble to return. */
        CPDouble * cpd = new CPDouble();
        if ( cpd == NULL ) {
            kprintf( "CPDouble::generateCPDouble() -- unable to allocate new CPDouble, aborting.\n" );
            abort();
            }

        /* Fetch the value. */
        jju32 w1, w2;
        is >> w1 >> w2;
        JavaWord jw1(w1);
        JavaWord jw2(w2);
        cpd->myDouble = (jdouble)JavaWord::toJDouble( jw1, jw2 );

        return cpd;
        } /* end generateCPDouble() */

    jlong getMyDouble() { return myDouble; }
    void setMyDouble( jdouble jl ) {
        /* since we can't do a real check on constancy, skip it. */
        myDouble = jl;
        }
    bool usesTwoEntries() { return true; }
    int type () { return TAG_LONG; }

  protected:
    jdouble myDouble;

}; /* end class CPDouble */

#endif

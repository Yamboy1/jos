/*
 * frame.h
 * 
 * declares the C++ class
 * representing a single frame
 * on the stack of a Java thread.
 * 
 * It's worth noting that the 2nd Edition JVM Spec
 * requires that doubles and longs be represented on
 * the operand stack as single values, while still taking
 * up two entries in local variables and the like.
 */

#ifndef decaf_frame
#define decaf_frame

class Frame;
class LocalVariables;

#include "javaword.h"
#include "staticstack.h"
#include "javastring.h"

typedef StaticStack< JavaWord >      OperandStack;

#include "attributelist.h"
#include "javathread.h"
#include "javaword.h"
#include "jvm.h"

// #include "exception.h"
typedef int Exception;

class Frame {

  public:
    Frame();

    static Frame * generateFrame( JavaClassInstance * jc, JavaString * methodName, JavaString * methodSignature );
    static Frame * generateFrame( JavaClassInstance * jc, char * methodName, char * methodSignature );

    void setMyThread( JavaThread * jt );
    JavaThread * getMyThread();
    CodeAttribute * getMyCode() { return myCode; }

	/* The interpreter is defined by a set of methods operating on stack frames. */
	
	/* UTILITY */
	jju8  getImmediate_jju8 ( jju32 index );
	jji8  getImmediate_jji8 ( jju32 index );
	jju16 getImmediate_jju16( jju32 index );
	jji16 getImmediate_jji16( jju32 index );
	jji32 getImmediate_jji32( jju32 index );

	jint pop_jint() { return (jint)*myOpStack->pop(); }
	JavaClassInstance * pop_jref() { 
		/* I want a static cast here so that JavaWord's cast operator is called. */
		return (JavaClassInstance*)*myOpStack->pop();
		}
	jfloat pop_jfloat() { return (jfloat)*myOpStack->pop(); }
	jdouble pop_jdouble() {
		return * (jdouble*) myOpStack->pop();
		}
	jlong pop_jlong() {
		return * (jlong*) myOpStack->pop();
		}

	void push_jint( jint value ) {
		/* $: lots of potential for garbage */
		myOpStack->push( new JavaWord( value ) );
		}
	void push_jref( JavaClassInstance * jci ) {
		/* $: lots of potential for garbage */
		myOpStack->push( new JavaWord( jci ) );
		}
	void push_jfloat( jfloat value ) {
		/* $: lots of potential for garbage */
		myOpStack->push( new JavaWord( value ) );
		}
	void push_jdouble( jdouble value ) {
		/* $: lots of potential for garbage */
		jdouble * jd = new jdouble( value );
		myOpStack->push( (JavaWord*)jd );
		}
	void push_jlong( jlong value ) {
		/* $: lots of potential for garbage */
		jlong * jd = new jlong( value );
		myOpStack->push( (JavaWord*)jd );
		}		
		

	/* INVOCATION */
	bool pushInvocationFrame( JavaClassInstance * jci, MethodInfo * mi, JavaWord * arguments );
	
	bool invokestatic(Exception &e);

	/* BYTECODE: A */
	bool aaload(Exception &e);
	bool aastore(Exception &e);
	bool aconst_null(Exception &e);

	bool aload(Exception &e);
	bool aload_0(Exception &e);
	bool aload_1(Exception &e);
	bool aload_2(Exception &e);
	bool aload_3(Exception &e);

	/*
	bool anewarray(Exception &e);
	bool areturn(Exception %e);
	*/
	bool arraysize(Exception &e);

	bool astore(Exception &e);
	bool astore_0(Exception &e);
	bool astore_1(Exception &e);	
	bool astore_2(Exception &e);	
	bool astore_3(Exception &e);

	/*
	bool athrow(Exception &e);
	*/

	/* BYTECODE: B */
	bool baload(Exception &e);
	bool bastore(Exception &e);
	bool bipush(Exception &e);

	/* BYTECODE: C */
	bool caload(Exception &e);
	bool castore(Exception &e);

	/*
	bool checkcast(Exception &e);
	*/
	
	/* BYTECODE: D */
	bool d2f(Exception &e);
	bool d2i(Exception &e);
	bool d2l(Exception &e);

	bool dadd(Exception &e);
	bool daload(Exception &e);
	bool dastore(Exception &e);
	bool dcmpg(Exception &e);
	bool dcmpl(Exception &e);
	bool dconst_0(Exception &e);
	bool dconst_1(Exception &e);
	bool ddiv(Exception &e);
	
  protected:
    jju32 myPC;
    jju16 myLocalVarCount;
    LocalVariables * myLocalVariables;
    OperandStack * myOpStack;

    JavaClass * myClass;
    CodeAttribute * myCode;

    JavaThread * myThread;
    JVM * myJVM;

}; /* end class Frame */

class LocalVariables {
  public:
    LocalVariables( jju32 variableCount );

    JavaWord * getJavaWord( jju32 index );
    void      setJavaWord( jju32 index, JavaWord * jw );

    jdouble   getJDouble( jju32 index );
    void      setJDouble( jju32 index, jdouble jd );

    jfloat    getJFloat( jju32 index );
    void      setJFloat( jju32 index, jfloat jf );

    jlong     getJLong( jju32 index );
    void      setJLong( jju32 index, jlong jl );

    jint      getJInt( jju32 index );
    void      setJInt( jju32 index, jint ji );

  protected:
    jju32 myLocalVariableCount;
    JavaWord * myLocalVariables;
    bool myErr;
}; /* end class LocalVariables */

#endif

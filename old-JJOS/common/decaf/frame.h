/*
 * frame.h
 * 
 * declares the C++ class
 * representing a single frame
 * on the stack of a Java thread.
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
	jju16 getImmediate_jju16( jju32 index );
	jji16 getImmediate_jji16( jju32 index );
	jji32 getImmediate_jji32( jju32 index );

	jint pop_jint() { return (jint)*myOpStack->pop(); }
	JavaClassInstance * pop_jref() { 
		/* I want a static cast here so that JavaWord's cast operator is called. */
		return (JavaClassInstance*)*myOpStack->pop();
		}

	/* $: return bool? */
	void push_jint( jint value ) {
		/* $: lots of potential for garbage */
		myOpStack->push( new JavaWord( value ) );
		}
	void push_jref( JavaClassInstance * jci ) {
		/* $: lots of potential for garbage */
		myOpStack->push( new JavaWord( jci ) );
		}
		

	/* INVOCATION */
	bool pushInvocationFrame( JavaClassInstance * jci, MethodInfo * mi, JavaWord * arguments );
	
	bool invokestatic(Exception &e);

	/* BYTECODE: A */
	bool aaload(Exception &e);
	bool aastore(Exception &e);

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

    JavaWord  getJavaWord( jju32 index );
    void      setJavaWord( jju32 index, JavaWord jw );

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

/*
 * decaf.cc
 * 
 * defines the decaf interpreter.
 */

#include "frame.h"
#include "assert.h"

/* UTILITY CODE */

static int trace = 1;
#define TRACE(args) if(trace){ kprintf args; }

jju8  Frame::getImmediate_jju8 ( jju32 index ) { return (myCode->getMyCode())[index]; }
jji8  Frame::getImmediate_jji8 ( jju32 index ) { return (myCode->getMyCode())[index]; }
jju16 Frame::getImmediate_jju16( jju32 index ) { return ((myCode->getMyCode())[index] << 8) | (myCode->getMyCode())[index+1]; }
jji16 Frame::getImmediate_jji16( jju32 index ) { return ((myCode->getMyCode())[index] << 8) | (myCode->getMyCode())[index+1]; }

jji32 Frame::getImmediate_jji32( jju32 index ) {
  return ((myCode->getMyCode())[index] << 24) | 
  		 ((myCode->getMyCode())[index+1] << 16) |
  		 ((myCode->getMyCode())[index+1] << 8) |
  		 ((myCode->getMyCode())[index+1]);
  }

/* ***** INVOCATION *****
 * 
 * The goal of an invocation to call
 * myStack->pushFrame() with the frame
 * of the method being invoked.
 */

/* invokeNativeMethod is used by the other invocation functions
 * to handle calling native methods, which may, for JOS, be
 * bytecode! See GCHII's BCNI. */
bool invokeNativeMethod( MethodInfo * mi, JavaWord * arguments ) {
	#warning Frame::invokeNativeMethod() unimplemented -- fixme!
	return true;
	}

/* pushInvocationFrame is a utility method for the invocation
 * methods; note that arguments must have been allocated with
 * new so can the pIF() can garbage-collect it. */

/* pushInvocationFrame() checks if the method is native
 * or synchronized, and sets up the arguments before pushing
 * the new frame.  (If necessary, static initialization and late
 * resolution could happen here too.) */

bool Frame::pushInvocationFrame( JavaClassInstance * jci, MethodInfo * mi, JavaWord * arguments ) {
  /* If the method is synchronized, acquire locks on its behalf. */
  if ( mi->isSynchronized() ) {
    /* We don't need to for frame to store the monitor, because
     * when it returns, it still must have the lock. */
    return jci->lock( myThread );
	} /* end if invocation synchronized */

  /* If the method is native, push the frame on the stack (tracking),
   * invoke the method, pop the frame, and carry on. */
  if ( mi->isNative() ) {
	myThread->pushFrame( this );
	invokeNativeMethod( mi, arguments );
	myThread->popFrame();
	delete arguments;
	return true;
  	}

  /* Othewise, pass the method its arguments and push its frame onto the stack. */
  for ( jint i = 0; i < mi->getMyArgumentCount(); i++ ) {
	myLocalVariables->setJavaWord( i, & arguments[i] );
    } delete arguments;
  myThread->pushFrame( this );

  /* The invocation was successful. */
  return true;
  } /* end pushInvocationFrame() */



/* for invokestatic(),
 * the class under discussion never changes,
 * so the native-method check can occur
 * at any time. */

bool Frame::invokestatic(Exception &e) {
  /* ought static method references be cached? */

  /* Fetch the method reference. */
  jju16 methodIndex = getImmediate_jju16( myPC + 1 );
  myPC += 2;

  /* Fetch the referenced method & the class in which the method resides. */
  MethodRef * mr = NULL;

  ASSERT_CAST(mr, myClass->getMyConstant(methodIndex), MethodRef *, CPEntry *,
  				"Frame::invokestatic()", "method reference");

  JavaClassInstance * jci = mr->getMyClass();
  ASSERT_NOT_NULL(jci, "Frame::invokestatic()", "unable to get method's class");

  /* Fetch the method's name and signature. */
  JavaString * methodName = mr->getMyName();
  ASSERT_NOT_NULL(methodName, "Frame::invokestatic()", "unable to get method's name" );

  /* $: assumes that handing off a bad constant pool will be handled gracefully */
  JavaString * methodSignature = mr->getMyType( jci->getMyConstantPool() );
  ASSERT_NOT_NULL(methodSignature, "Frame::invokestatic()", "unable to get method's signature" );

  /* Generate the method's frame.  If the method is native,
   * its Code attribute will be NULL. */
  Frame * f = Frame::generateFrame( jci, methodName, methodSignature );
  ASSERT_NOT_NULL(f, "Frame::invokestatic()", "failed to generate frame" );

  /* Fetch the method information. */
  MethodInfo * mi = jci->getMethodInfo( methodName, methodSignature );
  ASSERT_NOT_NULL(mi, "Frame::invokestatic()", "unable to get method's information" );

  /* Generate the arguments. */
  JavaWord * argument = new JavaWord[mi->getMyArgumentCount()];
  ASSERT_ALLOCATED(argument, "Frame::invokestatic()", "argument array" );

  /* Pop the arguments off the operand stack. */ 
  for ( jint i = (mi->getMyArgumentCount() - 1); i >= 0; i-- ) {
    argument[i] = myOpStack->pop();
    }

  /* Push the frame onto the stack. */
  return f->pushInvocationFrame( jci, mi, argument );
  } /* end invokestatic() */


/* BYTECODE: A */

bool Frame::aaload(Exception & e)
{
	jint index = pop_jint();
	JavaObjectArray * joa = NULL;

	ASSERT_CAST(joa, pop_jref(), JavaObjectArray *, JavaClassInstance *,
		"Frame::aaload()", "JavaObjectArray");	

	/* fetch the reference */
	JavaClassInstance * jci = joa->getElement( index );
	if ( jci == NULL ) {
			/* $: make sure getElement returns NULL rather than barfing. */
			// e = Exception::generateException( "IndexOutOfBoundsException" );
			return false;
			}
			
	TRACE(("aaload (%x = %x[%d])\n", jci, joa, index));
	push_jref( jci );
	return true;
	} /* end aaload() */

bool Frame::aastore(Exception & e) {
	/* fetch the object */
	JavaClassInstance * jci = pop_jref();
	
	/* fetch the index */
	jint index = pop_jint();

	/* fetch the array */
	JavaObjectArray * joa = NULL;

	ASSERT_CAST(joa, pop_jref(), JavaObjectArray *, JavaClassInstance *, 
		"frame::aastore()", "JavaObjectArray" );

	/* store the object in the array at the index */
	TRACE(("aastore (%x[%d] = %x)\n", joa, index, jci));
	joa->setElement( index, jci );
	
	return true;
	} /* end aastore() */

bool Frame::aconst_null(Exception & e) {
	/* push the constant null */
	TRACE(("aconst_null\n"));
	push_jref( NULL );

	return true;
	} /* end acons_null() */

bool Frame::aload(Exception & e) {
	/* which local variable are we loading from? */
	jju8 idx = getImmediate_jju8( myPC ); myPC++;
	JavaWord * jw = myLocalVariables->getJavaWord( idx );

	TRACE(("aload (%x from lv #%d)\n", jw, idx ));
	myOpStack->push( jw );
	
	return true;
	} /* end aload() */

bool Frame::aload_0(Exception & e) {
	JavaWord * jw = myLocalVariables->getJavaWord( 0 );

	TRACE(("aload_0 (%x from lv #d)\n", jw, 0 ));
	myOpStack->push( jw );

	return true;
	}
bool Frame::aload_1(Exception & e) {
	JavaWord * jw = myLocalVariables->getJavaWord( 1 );

	TRACE(("aload_1 (%x from lv #d)\n", jw, 1 ));
	myOpStack->push( jw );

	return true;
	}
bool Frame::aload_2(Exception & e) {
	JavaWord * jw = myLocalVariables->getJavaWord( 2 );

	TRACE(("aload_2 (%x from lv #d)\n", jw, 2 ));
	myOpStack->push( jw );

	return true;
	}
bool Frame::aload_3(Exception & e) {
	JavaWord * jw = myLocalVariables->getJavaWord( 3 );

	TRACE(("aload_3 (%x from lv #d)\n", jw, 3 ));
	myOpStack->push( jw );

	return true;
	}

#warning anewarray
#warning areturn

bool Frame::arraysize(Exception & e) {
	JavaClassInstance * jci = pop_jref();
	JavaArrayObject * jao = NULL;
	
	ASSERT_CAST( jao, jci, JavaArrayObject *, JavaClassInstance *,
				 "Frame::arraysize()", "array" );

	TRACE(( "arraysize = %d\n", jao->getMySize() ));
	push_jint( jao->getMySize() );	
	return true;
	}

bool Frame::astore(Exception &e) {
	jju8 idx = getImmediate_jju8( myPC ); myPC++;
	JavaWord * jw = myOpStack->pop();

	TRACE(( "astore %x at %d\n", jw, idx ));
	myLocalVariables->setJavaWord( idx, jw );
	return true;
	}

bool Frame::astore_0(Exception &e) {
	JavaWord * jw = myOpStack->pop();

	TRACE(( "astore %x at 0\n", jw ));
	myLocalVariables->setJavaWord( 0, jw );
	return true;
	}
bool Frame::astore_1(Exception &e) {
	JavaWord * jw = myOpStack->pop();

	TRACE(( "astore %x at 1\n", jw ));	
	myLocalVariables->setJavaWord( 1, jw );
	return true;
	}
bool Frame::astore_2(Exception &e) {
	JavaWord * jw = myOpStack->pop();

	TRACE(( "astore %x at 2\n", jw ));	
	myLocalVariables->setJavaWord( 2, jw );
	return true;
	}
bool Frame::astore_3(Exception &e) {
	JavaWord * jw = myOpStack->pop();

	TRACE(( "astore %x at 3", jw ));	
	myLocalVariables->setJavaWord( 3, jw );
	return true;
	}

#warning athrow


/* BYTECODE: B */

bool Frame::baload(Exception &e) {
	jint idx = pop_jint();
	JavaPrimitiveArray<jbyte> * jao = NULL;

	ASSERT_CAST(jao, pop_jref(), JavaPrimitiveArray<jbyte> *, JavaClassInstance *, 
		"frame::baload()", "JavaArrayObject" );

	TRACE(( "baload %d from %x\n", idx, jao ));
	push_jint( jao->getElement( idx ) );

	return true;
	}

bool Frame::bastore(Exception &e) {
	jint val = pop_jint();
	jint idx = pop_jint();
	JavaPrimitiveArray<jbyte> * jao = NULL;

	ASSERT_CAST(jao, pop_jref(), JavaPrimitiveArray<jbyte> *, JavaClassInstance *, 
		"frame::bastore()", "JavaArrayObject" );

	TRACE(( "bastore %d in %d to %x\n", val, idx, jao ));	 
	jao->setElement( idx, val );

	return true;
	}

bool Frame::bipush(Exception &e) {
	/* the sign-extension bit might not work right here... */
	TRACE(( "bipush %d", (myCode->getMyCode())[(myPC+1)] ));
	push_jint( (myCode->getMyCode())[(myPC++)] );
	return true;
	}

/* BYTECODE: C */
 
bool Frame::caload(Exception &e) {
	jint idx = pop_jint();
	JavaPrimitiveArray<jchar> * jao = NULL;

	ASSERT_CAST(jao, pop_jref(), JavaPrimitiveArray<jchar> *, JavaClassInstance *, 
		"frame::caload()", "JavaArrayObject" );

	TRACE(( "caload %d from %x\n", idx, jao ));
	push_jint( jao->getElement( idx ) );

	return true;
	}  

bool Frame::castore(Exception &e) {
	jint val = pop_jint();
	jint idx = pop_jint();
	JavaPrimitiveArray<jchar> * jao = NULL;

	ASSERT_CAST(jao, pop_jref(), JavaPrimitiveArray<jchar> *, JavaClassInstance *, 
		"frame::castore()", "JavaArrayObject" );

	TRACE(( "castore %d in %d to %x\n", val, idx, jao ));	 
	jao->setElement( idx, val );

	return true;
	}

#warning checkcast

/* BYTECODE: D */

#warning obey fp_strict in double ops

bool Frame::d2f(Exception &e) {
	jdouble val = pop_jdouble();
	jfloat conv = (jfloat)val;

	TRACE(( "d2f: %f to %f\n", val, conv ));
	push_jfloat( conv );

	return true;
	}
	
bool Frame::d2i(Exception &e) {
	jdouble val = pop_jdouble();
	jint conv = (jint)val;

	TRACE(( "d2i: %f to %d\n", val, conv ));
	push_jint( conv );

	return true;
	}

bool Frame::d2l(Exception &e) {
	jdouble val = pop_jdouble();
	jlong conv = (jlong)val;

	TRACE(( "d2l: %f\n", val ));
	push_jlong( conv );

	return true;
	}

bool Frame::dadd(Exception &e) {
	jdouble jd2 = pop_jdouble();
	jdouble jd1 = pop_jdouble();

	TRACE(( "dadd: %f + %f = %f\n", jd1, jd2, jd1 + jd2 ));
	push_jdouble( jd1 + jd2 );

	return true;
	}

bool Frame::daload(Exception &e) {
	jint idx = pop_jint();
	JavaPrimitiveArray<jdouble> * jao = NULL;

	ASSERT_CAST(jao, pop_jref(), JavaPrimitiveArray<jdouble> *, JavaClassInstance *, 
		"frame::daload()", "JavaArrayObject" );

	TRACE(( "daload %d from %x\n", idx, jao ));
	push_jint( jao->getElement( idx ) );

	return true;
	}

bool Frame::dastore(Exception &e) {
	jdouble val = pop_jdouble();
	jint idx = pop_jint();
	JavaPrimitiveArray<jdouble> * jao = NULL;

	ASSERT_CAST(jao, pop_jref(), JavaPrimitiveArray<jdouble> *, JavaClassInstance *, 
		"frame::dastore()", "JavaArrayObject" );

	TRACE(( "dastore %f in %d to %x\n", val, idx, jao ));	 
	jao->setElement( idx, val );

	return true;
	}

bool Frame::dcmpg(Exception &e) {
	jdouble jd2 = pop_jdouble();
	jdouble jd1 = pop_jdouble();

	TRACE(( "dcmpg %f vs %f\n", jd1, jd2 ));

	if( jd1 > jd2 ) { push_jint( 1 ); }
	else if ( jd1 == jd2 ) { push_jint( 0 ); }
	else if ( jd1 > jd2 ) { push_jint( -1 ); }
	else { /* jd1 or jd2 is NaN */ push_jint( 1 ); }

	return true;
	}

bool Frame::dcmpl(Exception &e) {
	jdouble jd2 = pop_jdouble();
	jdouble jd1 = pop_jdouble();

	TRACE(( "dcmpl %f vs %f\n", jd1, jd2 ));

	if( jd1 > jd2 ) { push_jint( 1 ); }
	else if ( jd1 == jd2 ) { push_jint( 0 ); }
	else if ( jd1 > jd2 ) { push_jint( -1 ); }
	else { /* jd1 or jd2 is NaN */ push_jint( -1 ); }

	return true;
	}

bool Frame::dconst_0(Exception &e) {
	TRACE(( "dconst_0\n" ));
	push_jdouble( 0 );
	return true;
	}

bool Frame::dconst_1(Exception &e) {
	TRACE(( "dconst_1\n" ));
	push_jdouble( 1 );
	return true;
	}

bool Frame::ddiv(Exception &e) {
	jdouble jd2 = pop_jdouble();
	jdouble jd1 = pop_jdouble();

	TRACE(( "ddiv %f / %f\n", jd1, jd2 ));
	push_jdouble( jd1 / jd2 );
	
	return true;
	}
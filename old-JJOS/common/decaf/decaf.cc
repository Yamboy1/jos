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
	myLocalVariables->setJavaWord( i, arguments[i] );
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
	JavaObjectArray * joa =  NULL;

	ASSERT_CAST(joa, pop_jref(), JavaObjectArray *, JavaClassInstance *,
		"Frame::aaload()", "JavaObjectArray");	

	/* fetch the reference */
	JavaClassInstance * jci = joa->getElement( index );
	if ( jci == NULL ) {
			/* $: make sure getElement returns NULL rather than barfing. */
			// e = Exception::generateException( "IndexOutOfBoundsException" );
			return false;
			}
			
	TRACE(("aaload (%x = %x[%d])", jci, joa, index));
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
	TRACE(("aastore (%x[%d] = %x)", joa, index, jci));
	joa->setElement( index, jci );
	
	return true;
	} /* end aastore() */


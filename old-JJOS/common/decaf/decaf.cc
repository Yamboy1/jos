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

/* invokeNativeMethod( MethodInfo * mi, JavaWord * arguments ) */
#warning Frame::invokeNativeMethod() unimplemented -- fixme!


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

#ifdef DONT_USE_DC
  MethodRef * mr = myClass->getMyConstant( methodIndex );

#ifdef NO_ASSERTIONS
  if ( mr->type() != TAG_METHODINFO ) {
    kprintf( "Frame::invokestatic() -- method references non-method constant, aborting.\n" );
    abort();
    }
#else
  ASSERT_CAST(mr, mr->type(), TAG_METHODINFO, "Frame::invokestatic()", "method reference");
#endif
  
  JavaClassInstance * jci = mr->getMyClass();

#else

  JavaClassInstance * jci = NULL;
  MethodRef * mr = NULL;

#ifdef NO_ASSERTIONS
  if ( (mr = dynamic_cast<MethodRef *>(myClass->getMyConstant(methodIndex))) ) {
  		jci = mr->getMyClass();
  		} else {
  		kprintf( "Frame::invokestatic() -- method reference non-method constant, aborting.\n" );
  		abort();
  		}
#endif

  ASSERT_CAST(mr, myClass->getMyConstant(methodIndex), MethodRef *,
  				"Frame::invokestatic()", "method reference");
  jci = mr->getMyClass();
  		
#endif

#ifdef NO_ASSERTIONS
  if ( jci == NULL ) {
	kprintf( "Frame::invokestatic() -- unable to get method's class, aboring.\n" );
	abort(); 
	}
#else
	ASSERT_NOT_NULL(jci, "Frame::invokestatic()", "unable to get method's class");
#endif

  /* Fetch the method's name and signature. */
  JavaString * methodName = mr->getMyName();
#ifdef NO_ASSERTIONS
  if ( methodName == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's name, aborting.\n" );
    abort();
    }
#else
	ASSERT_NOT_NULL(methodName, "Frame::invokestatic()", "unable to get method's name" );
#endif	

  /* $: assumes that handing off a bad constant pool will be handled gracefully */
  JavaString * methodSignature = mr->getMyType( jci->getMyConstantPool() );
#ifdef NO_ASSERTIONS
  if ( methodSignature == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's signature, aborting.\n" );
    abort();
    }
#else
	ASSERT_NOT_NULL(methodSignature, "Frame::invokestatic()", "unable to get method's signature" );
#endif 

  /* Generate the method's frame.  If the method is native,
   * its Code attribute will be NULL. */
  Frame * f = Frame::generateFrame( jci, methodName, methodSignature );
#ifdef NO_ASSERTIONS  
  if ( f == NULL ) {
    kprintf( "Frame::invokestatic() -- failed to generate frame, aborting.\n" );
    abort();
    }
#else
	ASSERT_NOT_NULL(f, "Frame::invokestatic()", "failed to generate frame" );
#endif		

  /* Fetch the method information. */
  MethodInfo * mi = jci->getMethodInfo( methodName, methodSignature );
#ifdef NO_ASSERTIONS  
  if ( mi == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's information, aborting.\n" );
    abort();
    }
#else
	ASSERT_NOT_NULL(mi, "Frame::invokestatic()", "unable to get method's information" );
#endif	

  /* Generate the arguments. */
  JavaWord * argument = new JavaWord[mi->getMyArgumentCount()];
#ifdef NO_ASSERTIONS
  if ( argument == NULL ) {
  	kprintf( "Frame::invokestatic() -- unable to allocate argument array, aborting\n" );
  	abort();
  	}
#else
	ASSERT_ALLOCATED(argument, "Frame::invokestatic()", "argument array" );
#endif	

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

#ifdef DONT_USE_DC
#error frame::aaload() -- DONT_USE_DC unavailable, fixme.
#else

#ifdef NO_ASSERTIONS
	JavaObjectArray * joa = NULL;
	if ( joa = dynamic_cast<JavaObjectArray *>(pop_jref()) ) {
		JavaClassInstance * jci = joa->getElement( index );
		if ( jci == NULL ) {
			/* $: make sure getElement returns NULL rather than barfing. */
			e = Exception::generateException( "IndexOutOfBoundsException" );
			return false;
			}
		TRACE(("aaload (%x = %x[%d]", jci, joa, index );
		myOpStack->push( jci );
		return true;
		} else {
		/* if joa is not an instance of JavaObjectArray,
		 * the VM is hosed. */
		kprintf( "frame::aaload() -- attempt to load from non-array, aborting.\n" );
		abort();
		}
#else

	JavaObjectArray * joa = NULL;
	ASSERT_CAST(joa, pop_jref(), JavaObjectArray *, "frame::aaload()", "attempt to load from non-array" );

	/* fetch the reference */
	JavaClassInstance * jci = joa->getElement( index );
	if ( jci == NULL ) {
			/* $: make sure getElement returns NULL rather than barfing. */
			// e = Exception::generateException( "IndexOutOfBoundsException" );
			e = 1;
			return false;
			}
			
	TRACE(("aaload (%x = %x[%d])", jci, joa, index ));
	push_jref( jci );
	return true;

#endif /* NO_ASSERTIONS */
		
#endif /* DONT_USE_DC */

	} /* end aaload() */
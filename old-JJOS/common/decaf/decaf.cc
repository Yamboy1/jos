/*
 * decaf.cc
 * 
 * defines the decaf interpreter.
 */

#include "frame.h"

/* ***** INVOCATION *****
 * 
 * The goal of an invocation to call
 * myStack->pushFrame() with the frame
 * of the method being invoked.
 */
 

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
  jju16 methodIndex = read_jju16( myPC + 1 );
  myPC += 2;

  /* Fetch the referenced method & the class in which the method resides. */

#ifdef DONT_USE_DC
  MethodRef * mr = myClass->getMyConstant( methodIndex );

  if ( mr->type() != TAG_METHODINFO ) {
    kprintf( "Frame::invokestatic() -- method references non-method constant, aborting.\n" );
    abort();
    }

  JavaClassInstance * jci = mr->getMyClass();

#else

  JavaClassInstance * jci = NULL;
  MethodRef * mr = NULL;

  if ( (mr = dynamic_cast<MethodRef *>(myClass->getMyConstant(methodIndex))) ) {
  		jci = mr->getMyClass();
  		} else {
  		kprintf( "Frame::invokestatic() -- method reference non-method constant, aborting.\n" );
  		abort();
  		}
#endif
    
  if ( jci == NULL ) {
	/* mr may not have cached it yet, so try to resolve it */
	#error Frame::invokestatic() -- resolve uncached JCI.
  
    // kprintf( "Frame::invokestatic() -- unable to get target class, aborting.\n" );
    // abort();
    }

  /* Fetch the method's name and signature. */
  JavaString * methodName = mr->getMyName();
  if ( methodName == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's name, aborting.\n" );
    abort();
    }
  
  JavaString * methodSignature = mr->getMyType();
  if ( methodSignature == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's signature, aborting.\n" );
    abort();
    }

  /* Generate the method's frame.  If the method is native,
   * its Code attribute will be NULL. */
  Frame * f = Frame::generateFrame( jci, methodName, methodSignature );
  if ( f == NULL ) {
    kprintf( "Frame::invokestatic() -- failed to generate frame, aborting.\n" );
    abort();
    }
 
  /* Fetch the method information. */
  MethodInfo * mi = jci->getMethodInfo( methodName, methodSignature );
  if ( mi == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's information, aborting.\n" );
    abort();
    }

  /* Generate the arguments. */
  JavaWord * argument = new JavaWord[mi->getMyArgumentCount()];
  if ( argument == NULL ) {
  	kprintf( "Frame::invokestatic() -- unable to allocate argument temporary, aborting\n" );
  	abort();
  	}

  /* Pop the arguments off the operand stack. */ 
  for ( jint i = (mi->getMyArgumentCount() - 1); i >= 0; i-- ) {
    argument[i] = myOpStack->pop();
    }

  /* Push the frame onto the stack. */
  return f->pushInvocationFrame( jci, mi, argument );
  } /* end invokestatic() */


/* BYTECODE: A */

#ifdef NOT_READY_YET

/* is there, BTW, anything special we need to do to the operand stack
 * w.r.t. throwing exceptions? */
bool frame::aaload(Exception & e)
{
	jint index = myOpStack->pop();
	JavaObjectArray * joa = myOpStack->pop();

#ifdef DONT_USE_DC
#error frame::aaload() -- DONT_USE_DC unavailable, fixme.
#else

	if ( joa = dynamic_cast<JavaObjectArray *>(joa) ) {
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
}

#endif
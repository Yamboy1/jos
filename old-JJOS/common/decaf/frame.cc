/*
 * frame.cc
 * 
 * defines the C++ class
 * representing a single
 * frame on a java thread's
 * stack.
 */

#include "frame.h"

/* Frame */

Frame::Frame() {
    myPC = 0;
    myLocalVarCount = 0;
    myOpStack = NULL;

    myClass = NULL;
    myCode = NULL;

    myThread = NULL;
    myJVM = NULL;
    } /* end Frame() */

void Frame::setMyThread( JavaThread * jt ) {
    if ( myThread != NULL ) {
        kprintf( "Frame::setMyThread() -- attempt to reset thread, aborting.\n" );
        abort();
        } else if ( jt != NULL ){
        myThread = jt;
        myJVM = jt->getMyJVM();
        } else {
        kprintf( "Frame::setMyThread() -- attempt to set thread to NULL, aborting.\n" );
        abort();
        }
    } /* end setMyThread() */

JavaThread * Frame::getMyThread() {
    return myThread;
    } /* end getMyThread() */

Frame * Frame::generateFrame( JavaClassInstance * jc, char * method, char * methodSig ) {
    JavaString * js1 = new JavaString( method );
    JavaString * js2 = new JavaString( methodSig );
    if ( js1 == NULL || js2 == NULL ) {
        kprintf( "Frame::generateFrame() -- unable to allocate method[Sig] string, aborting.\n" );
        abort();
        }
    Frame * f = generateFrame( jc, js1, js2 );
    delete js1;
    delete js2;
    return f;
    }

Frame * Frame::generateFrame( JavaClassInstance * jc, JavaString * method, JavaString * methodSig ) {
    /* Generate a clean frame. */
    Frame * f = new Frame();
    if ( f == NULL ) {
        kprintf( "Frame::generateFrame() -- unable to allocate new frame, aborting.\n" );
        abort();
        }

    /* Initialize variables. */
    f->myClass = jc;

	/* If the method is native, we don't have any more work to do here. */
	MethodInfo * mi = f->myClass->getMethodInfo( method, methodSig );
	if ( mi->isNative() ) {
		/* $: It may be worthwhile at some point to store more information
		 * about native frames... */
		return f;
		}

    /* Fetch the method. */
    f->myCode = f->myClass->getMethodCode( method, methodSig );
    if ( f->myCode == NULL ) {
        kprintf( "Frame::generateFrame() -- \"%s\" [%s] not found in %s, aborting.\n", method->c_str(), methodSig->c_str(), jc->getMyName()->c_str() );
        abort();
        }

    /* Initializ the frame's local variables. */
    f->myLocalVariables = new LocalVariables( f->myCode->getMyMaxLocals() );
    if ( f->myLocalVariables == NULL ) {
        kprintf( "Frame::generateFrame() -- unable to allocate local variables, aborting.\n" );
        abort();
        }

    /* Initialize the frame's stack. */
    f->myOpStack = new OperandStack( f->myCode->getMyMaxStack() );
    if ( f->myOpStack == NULL ) {
        kprintf( "Frame::generateFrame() -- unable to allocate operand stack, aborting.\n" );
        abort();
        }

    /* Return the generated frame. */  
    return f;
    } /* end generateFrame() */

/* LocalVariables */

LocalVariables::LocalVariables( jju32 variableCount ) {
    if ( variableCount != 0 ) {
        myLocalVariableCount = variableCount;
        myLocalVariables = new JavaWord [ variableCount ];
        if ( myLocalVariables == NULL ) {
           kprintf( "LocalVariables::LocalVariables() -- unable to allocate local variable array, aborting.\n" );
           abort();
           }
        } else {
        myLocalVariableCount = 0;
        myLocalVariables = NULL;
        }
     } /* end LocalVariable() */

JavaWord LocalVariables::getJavaWord( jju32 index ) {
    if ( index < myLocalVariableCount ) {
        return myLocalVariables[index];
        } else {
        kprintf( "LocalVariables::getJavaWord(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never do this. */
    return JavaWord();
	}
	
void LocalVariables::setJavaWord( jju32 index, JavaWord jw ) {
    if ( index < myLocalVariableCount ) {
        myLocalVariables[index] = jw;
        } else {
        kprintf( "LocalVariables::setJavaWord(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
	}
	
jdouble LocalVariables::getJDouble( jju32 index ) {
    if ( index < myLocalVariableCount - 1 ) {
        JavaWord jw1 = myLocalVariables[index];
        JavaWord jw2 = myLocalVariables[index + 1];
        return JavaWord::toJDouble( jw1, jw2 );
        } else {
        kprintf( "LocalVariables::getJDouble(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never do this */
    return 0;
	}
	
void LocalVariables::setJDouble( jju32 index, jdouble jd ) {
    if ( index < myLocalVariableCount ) {
        JavaWord jw1 = JavaWord( jd, JavaWord::FirstWord );
        JavaWord jw2 = JavaWord( jd, JavaWord::SecondWord );
        myLocalVariables[index] = jw1;
        myLocalVariables[index + 1] = jw2;
        } else {
        kprintf( "LocalVariables::setJDouble(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
	}
	
jfloat LocalVariables::getJFloat( jju32 index ) {
    if ( index < myLocalVariableCount ) {
        return myLocalVariables[index];
        } else {
        kprintf( "LocalVariables::getJFloat(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never do this */
    return 0;
	}
	
void LocalVariables::setJFloat( jju32 index, jfloat jf ) {
    if ( index < myLocalVariableCount ) {
        myLocalVariables[index] = jf;
        } else {
        kprintf( "LocalVariables::setJFloat(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
	}
	
jlong LocalVariables::getJLong( jju32 index ) {
    if ( index < myLocalVariableCount - 1 ) {
        JavaWord jw1 = myLocalVariables[index];
        JavaWord jw2 = myLocalVariables[index + 1];
        return JavaWord::toJLong( jw1, jw2 );
        } else {
        kprintf( "LocalVariables::getJLong(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never do this */
    return 0;
	}
	
void LocalVariables::setJLong( jju32 index, jlong jl ) {
    if ( index < myLocalVariableCount ) {
        JavaWord jw1 = JavaWord( jl, JavaWord::FirstWord );
        JavaWord jw2 = JavaWord( jl, JavaWord::SecondWord );
        myLocalVariables[index] = jw1;
        myLocalVariables[index + 1] = jw2;
        } else {
        kprintf( "LocalVariables::setJLong(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
	}
	
jint LocalVariables::getJInt( jju32 index ) {
    if ( index < myLocalVariableCount ) {
        return myLocalVariables[index];
        } else {
        kprintf( "LocalVariables::getJInt(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never do this */
    return 0;
	}
	
void LocalVariables::setJInt( jju32 index, jint ji ) {
    if ( index < myLocalVariableCount ) {
        myLocalVariables[index] = ji;
        } else {
        kprintf( "LocalVariables::setJInt(%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
	}

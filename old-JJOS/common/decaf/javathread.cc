/*
 * javathread.h
 *
 * defines the C++ class
 * which represents a Java
 * thread.
 */

#include "javathread.h"

#include "frame.h"   
#include "jvm.h"

JVM * JavaThread::getMyJVM() { return myJVM; }
jint JavaThread::getMyPriority() { return myPriority; }


/* Threadable */

/* Threadable::* returns true if the thread can continue,
 * false if it can't (and is in the scheduler). */

bool Threadable::lock( JavaThread * jt ) {
	if ( myLockCount == 0 ) {
		/* then the thread becomes the owner. */
		myOwner = jt;
		myLockCount = 1;
		return true;
		}
	else if ( myOwner == jt ) {
		/* note that we need thread pointers to be unique... */
		myLockCount++;
		return true;
		}
	else {
		/* the thread is not the owner and blocks */
		myMonitoringThreadsList->addThread( jt, 0 );
		return false;
		}
	} /* end Threadable::lock() */

bool Threadable::unlock( JavaThread * jt ) {
	/* is it okay to update my lock count? */
	if ( jt != myOwner ) {
		/* illegal monitor state */
		kprintf( "Threadable::unlock() -- illegal monitor state, halting thread.\n" );
		kprintf( "Threadable::unlock() -- non-owner %x tried to unlock %x's lock.\n", jt, myOwner );
		return false;
		}
	else if ( myLockCount == 0 ) {
		kprintf( "Threadable::unlock() -- illegal monitor state, halting thread.\n" );
		kprintf( "Threadable::unlock() -- unlocking an object with no lock count.\n", myOwner, jt );
		return false;		
		}
		
	/* update my lock count */
	myLockCount--;

	/* can I start a monitoring thread? */
	tryToStartMonitoringThread();

	/* the thread can continue */
	return true;
	} /* end Threadable::unlock() */

void Threadable::tryToStartMonitoringThread() {
	/* can I start a monitoring thread? */
	if( ! myMonitoringThreadsList->isEmpty() &&	this->lock( myMonitoringThreadsList->myFirstThread() ) ) {
		/* acquire the extra locks */
		myLockCount += myMonitoringThreadsList->myFirstLockCount();

		/* schedule the new thread for execution,
		 * after removing it from the monitoring list */
		myMonitoringThreadsList->removeMyFirstThread();
		myOwner->getMyJVM()->getMyScheduler()->addThread( myOwner );
		} /* end if started a monitoring thread. */
	} /* end tryToStartMonitortingThread() */

bool Threadable::wait( JavaThread * jt ) {
	/* can it wait on me? */
	if ( jt != myOwner ) {
		kprintf( "Threadable::wait() -- illegal monitor state, halting thread.\n" );
		kprintf( "Threadable::wait() -- attempting to wait on a thread without ownership.\n" );
		return false;
		}

	/* add it to my wait list */
	myInterestedThreadsList->addThread( jt, myLockCount );

	/* we can safely skip the attempt to unlock() because
	 * we've already assured that jt is my owner */
	myLockCount = 0;

	/* try to start a monitoring thread */
	tryToStartMonitoringThread();

	/* stop running the thread */
	return false; 	
	} /* end Threadable::wait() */

bool Threadable::notify( JavaThread * jt ) {
	/* can jt notify me? */
	if ( jt != myOwner ) {
		kprintf( "Threadable::wait() -- illegal monitor state, halting thread.\n" );
		kprintf( "Threadable::wait() -- attempting to wait on a thread without ownership.\n" );
		return false;
		}

	/* do we have any interested threads to notify? */
	if ( myInterestedThreadsList->isEmpty() ) { return true; }

	/* inform the scheduler that we a have a notify pending. */
	myOwner->getMyJVM()->getMyScheduler()->addPendingNotify( new pn_item( myInterestedThreadsList->myFirstThread(), this,
			myInterestedThreadsList->myFirstLockCount()) );

	/* remove the notified thread from the interested list */
	myInterestedThreadsList->removeMyFirstThread();

	return true;
	} /* end Threadable::notify() */

bool Threadable::notifyAll( JavaThread * jt ) {
	while ( ! myInterestedThreadsList->isEmpty() ) {
		if ( ! notify( jt ) ) { return false; }
		} /* end all-threads loop */
	return true;
	} /* end of Threadable::notifyAll() */


/* JavaThread */

Frame * JavaThread::popFrame(){
	return myFrameStack->pop();
    } /* end popFrame() */
	
void JavaThread::pushFrame( Frame * f ) {
    if ( f != NULL ) { 
        myFrameStack->push( f );
        } else {
        kprintf( "JavaThread::pushFrame() -- attempt to push NULL frame, aborting.\n" );
        abort();
        }
    } /* end pushFrame() */

JavaThread * JavaThread::generateJavaThread( JVM * jvm, Frame * f, jint priority ) {
    JavaThread * jt = new JavaThread();
    if ( jt == NULL ) {
        kprintf( "JavaThread::generateJavaThread() -- unable to allocate new java thread, aborting.\n" );
        abort();
        }
    jt->myPriority = priority;
    jt->myJVM = jvm;
    jt->myFrameStack = new FrameStack();
    if ( jt->myFrameStack != NULL ) { jt->pushFrame( f ); }
    else {
        kprintf( "JavaThread::generateJavaThread() -- unable to allocate frame stack, aborting.\n" );
        abort();
        }
    return jt;
    } /* end generateJavaThread() */

bool JavaThread::runOpcode() {
    #warning JavaThread::runOpcode() -- implemented in interp.cc (do merge)
    kprintf ( "JavaThread::runOpcode() -- not executing code:\n" );
    jju8 * myCodeCode = myFrameStack->peek()->getMyCode()->getMyCode();
    for ( jju32 x = 0; x < myFrameStack->peek()->getMyCode()->getMyCodeLength(); x++ ) {
        kprintf( "%x ", myCodeCode[x] );
        }
    return false;
    }

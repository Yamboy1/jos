/*
 * scheduler.cc
 *
 * The default sheduler uses
 * a round-robin in priority
 * chunks alogrithm to schedule
 * the Java threads.  Assumes
 * that bytecode operations
 * are uninterruptible / atomic.
 */

#include "scheduler.h"
#include "assert.h"

Scheduler::Scheduler() {
    myThreadList = new JavaThreadSLL();
    if ( myThreadList == NULL ) {
        kprintf( "Scheduler::Scheduler() -- unable to allocate new thread list, aborting.\n" );
        abort();
        }
    myPendingNotifyList = new PendingNotifyList();
    if ( myPendingNotifyList == NULL ) {
        kprintf( "Scheduler::Scheduler() -- unable to allocate new pending notify list, aborting.\n" );
        abort();
        }
    myJVM = NULL;
    myCurrentThread = NULL;
    myTimeSlice = 5; /* # of bytecode per thread-run */
    myTimeCount = 0; /* # of bytecodes executed this thread-run */
    } /* end Scheduler() */

/* For now, we have a single scheduler.  This changes
 * with multiple native processes.  For instance -- it
 * would not be unreasonable to suggest that
 *      #ifdef TARGET_UNIX
 *      int ppid = getpid();
 *      int pid = fork();
 *      if ( ppid = pid ) { return; }
 *      else { s = new Scheduler(); // ...
 *      #else
 *      // ...
 *      #endif
 * would work for the host build to have multiple native
 * processes.  (This would work much better with threads,
 * actually, because then you don't have to mess with jbHeap
 * and declare the heap shared memory, etc.)
 */

Scheduler * Scheduler::generateScheduler() {
    static Scheduler * s = NULL;
    if ( s == NULL ) { s = new Scheduler(); return generateScheduler(); }
    else { return s; }
    } /* end generateScheduler() */

void Scheduler::removeThread( JavaThread * jt ) {
    if ( myCurrentThread == jt ) {
        /* narking the current thread */
        switchToNextThread();
        }
    myThreadList->remove( jt );
    } /* end removeThread() */

void Scheduler::addThread( JavaThread * jt ) {
    if ( jt != NULL ) {
        if ( jt->getMyJVM() != myJVM ) {
            kprintf( "Scheduler::addThread() -- thread from different JVM, aborting.\n" );
            abort();
            }
        myThreadList->append( jt );
        } else {
        kprintf( "Scheduler::addThread() -- attempt to add NULL thread, aborting.\n" );
        abort();
        }
    } /* end addThread() */

/* The process is alive so long as there are any active threads,
 * or any threads waiting to be notified. */
bool Scheduler::noThreadsLeftToRun() {
    return myThreadList->isEmpty() && myPendingNotifyList->isEmpty();
    }

void Scheduler::addPendingNotify( pn_item * pnli ) {
    if ( pnli == NULL ) {
        kprintf( "Scheduler::addPendingNotify() -- attempt to add NULL pending item, aborting.\n" );
        abort();
        }
    if ( pnli->jt == NULL || pnli->t == NULL ) {
        kprintf( "Scheduler::addPendingNotify() -- attempt to add invalid pending item, aborting.\n" );
        abort();
        }
    /* check myPNL? */
    myPendingNotifyList->append( pnli );
    } /* end addPendingNotify() */

void Scheduler::doInterruptHandler( JavaClassInstance * handler, jint interruptNumber ) {
    if ( handler == NULL ) {
        kprintf( "Scheduler::doInterruptHandler() -- NULL handler, aborting.\n" );
        abort();
        }

    /* The method to call. */
    JavaString * methodName = new JavaString( "handleInterrupt" ); /* $ intern! */
    JavaString * methodSig = new JavaString( "(I)V" ); /* $ intern! */
    if ( methodName == NULL || methodSig == NULL ) {
        kprintf( "Scheduler::doInterruptHandler() -- unable to allocate method strings, aborting.\n" );
        abort();
        }

    /* Generate its frame. */
    Frame * f = Frame::generateFrame( handler, methodName, methodSig );
    if ( f == NULL ) {
        kprintf( "Scheduler::doInterruptHandler() -- unable to generate frame, aborting.\n" );
        abort();
        }
    
    delete methodSig;
    delete methodName;

    /* Generate its thread. */
    JavaThread * jt = JavaThread::generateJavaThread( myCurrentThread->getMyJVM(), f, 10 );
    if ( jt == NULL ) {
        kprintf( "Scheduler::doInterruptHandler() -- unable to generate thread, aborting.\n" );
        abort();
        }
    f->setMyThread( jt );

    /* Execute it until it terminates. */
    while ( jt->runOpcode() ) {}
    delete jt;
    /* Shouldn't cause problems; needing a frame means that the thread terminated
     * waiting for something, which drivers shouldn't do, IIRC. */
    delete f;
    } /* end doInterruptHandler() */

bool Scheduler::doInterruptNotification() {
    jjMachine * jjm = myJVM->getMyMachine();

    /* If there aren't any interrupts, don't do anything. */
    if ( jjm->interruptFIFOEmpty() ) {
        return false;
        }

    /* Fetch the interrupt number. */
    jju16 interruptNumber = jjm->popOldestInterruptNumber();

    #ifdef NATIVE_CLOCK
    if ( interruptNumber == 8 ) { return false; }
    #endif 

#ifndef NEW_INTERRUPT_HANDLING
#error Old interrupt handling unavailable.
#endif

    /* Fetch the interrupt class. */
    JavaClassInstance * jci = myCurrentThread->getMyJVM()->getInterruptClass();
    if ( jci == NULL ) {
        kprintf( "Scheduler::doInterruptNotification() -- VM corrupted: interrupt class unavailable, aborting.\n" );
        abort();
        }

    /* Extract the array of arrays from the interrupt class. */
	JavaObjectArray * setArray = NULL;
	ASSERT_CAST( setArray, jci->getMyClassFields()->getStaticObject( 0 ),
				 JavaObjectArray *, JavaClassInstance *,
				 "Scheduler::doInterruptNotification()", "java array of objects" );

    /* Extract the array of interrupt handlers for this particular interrupt. */
	JavaObjectArray * handlerArray = NULL;
	ASSERT_CAST( handlerArray, setArray->getElement( interruptNumber ),
				 JavaObjectArray *, JavaClassInstance *,
				 "Scheduler::doInterruptNotification()", "java array of objects" );
				 
    /* Iterate over the interrupt handler array. */
    for ( jju32 x = 0; x < handlerArray->getMySize(); x++ ) {
        doInterruptHandler( handlerArray->getElement( x ), interruptNumber );
        } /* end handler iteration */

    return true;
    } /* end doInterruptNotification() */

bool Scheduler::currentTimeSliceExpired() {
    return (myTimeCount == (myTimeSlice * myCurrentThread->getMyPriority()));
    }

void Scheduler::notifyPending() {
    if ( myPendingNotifyList->isEmpty() ) { return; }

    /* otherwise, walk the list of pending notifies,
     * trying to wake them up as we go. */
    PendingNotifyListItem * pnli = new PendingNotifyListItem( new pn_item() );
    pnli->myNext = myPendingNotifyList->myFront;

    do {
        /* walk the list */
        pnli = pnli->myNext;
    
        /* If the threadable in question can acquire its locks,
         * it acquires all of them and is scheduled to run. */
        if ( pnli->myItem->t->lock( pnli->myItem->jt ) ) {
            /* $: is there ever a case where this differs than its current
             * lock count plus its old lock count minus one? */
            pnli->myItem->t->setMyLockCount( pnli->myItem->count );
            addThread( pnli->myItem->jt );
            }

        /* If it can't acquire it locks, it will sleep,
         * so in both cases, remove it from the pending notify queue. */
        myPendingNotifyList->remove( pnli->myItem );
        } while ( pnli->myNext != NULL );
        
    } /* end notifyPending() */

void Scheduler::switchToNextThread() {
    /* reset the elapses timeslice */
    myTimeCount = 0;

    /* Fetch the current thread from the list. 
     * It may well be worthwhile to cache this. */
    JavaThreadListItem * jlit = myThreadList->lookup( myCurrentThread );
    if ( jlit == NULL ) {
        kprintf( "Scheduler::switchToNextThread() -- unable to locate current thread, aborting.\n" );
        abort();
        }
    else if ( jlit->myNext == NULL ) {
        /* We're at the end of the list, so wrap around. */
        myCurrentThread = myThreadList->myFront->myItem; /* should be accessors. */
        }
    else {
        myCurrentThread = jlit->myNext->myItem;
        }
    } /* end switchToNextThread() */
   

/* startThread should only be called once,
 * since it only returns on program (process) completion. */
void Scheduler::start( JVM * jvm, JavaThread * jt ) {
    /* Starts execution in the given JVM. */
    myJVM = jvm;

    /* Add the thread to our scheduled-to-run list. */
    addThread( jt );

    /* Make it the initial thread to run. */
    myCurrentThread = jt;

    /* Start scheduling threads. */
    while ( ! noThreadsLeftToRun() ) {
        /* Handle all the interrupts that happened
         * during the last bytecode run. */
        doInterruptNotification();

        /* If the current timeslice has expired,
         * schedule those threads which have pending notifications,
         * and then pick a new thread to run. */
        if ( currentTimeSliceExpired() ) {
            notifyPending();
            switchToNextThread();
            }

        /* Run the current thread, removing it from scheduling if it terminates. */
        if ( ! myCurrentThread->runOpcode() ) {
            removeThread( myCurrentThread );
            }

        /* update the timeslice count. */
        myTimeCount++;
        } /* end scheduling loop */
    } /* end startThread() */
    

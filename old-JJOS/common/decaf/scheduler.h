/*
 * scheduler.h
 *
 * This class implements the scheduler
 * in an opaque fashion, so that the
 * particular details of how, exactly,
 * the concurrency of the threads will
 * be exploited are hidden.
 * 
 * In general, the scheduler should
 * be called for any action involving
 * a thread.
 * 
 * The scheduler is handed pairs of
 * threads and JVM's via .startThread()
 * and manages them appropriately.
 * 
 * This method returns only on program
 * completion, so internal scheduling
 * uses addThread and removeThread().
 * 
 * In general, a thread (group) is
 * considered a 'process' iff its classloader
 * is unique (to its group), which is the
 * rationale behind providing a JVM parameter
 * in the addThread() method.
 * 
 * There may be more than one scheduler
 * in the system.
 */

#ifndef decaf_scheduler
#define decaf_scheduler

class Scheduler;

#include "d_types.h"
#include "jvm.h"
#include "javathread.h"
#include "javaobject.h"
#include "jbsll.h"

struct pn_item { JavaThread * jt; Threadable * t; jju32 count;
				 pn_item( JavaThread * jthr, Threadable * thr, jju32 cnt ) { jt = jthr; t = thr; count = cnt; };
				 pn_item() { jt = NULL; t = NULL; count = 0; }; };
typedef jbSLL<pn_item *>                PendingNotifyList;
typedef jbSLLItem<pn_item *>            PendingNotifyListItem;

typedef jbSLL<JavaThread *>				JavaThreadSLL;
typedef jbSLLItem<JavaThread *>			JavaThreadListItem;

class Scheduler {

  public:
    Scheduler();

    Scheduler * generateScheduler();

    void start( JVM * jvm, JavaThread * jt );
    
    void addThread( JavaThread * jt );
    void removeThread( JavaThread * jt );

    void addPendingNotify( pn_item * pnli );

  protected:
    bool noThreadsLeftToRun();
    bool currentTimeSliceExpired();
    bool doInterruptNotification();
    void notifyPending();
    void switchToNextThread();
    void doInterruptHandler( JavaClassInstance * handler, jint interruptNumber );
  
    JavaThreadSLL * myThreadList;
    PendingNotifyList * myPendingNotifyList;

    JavaThread * myCurrentThread;
    JVM * myJVM;

    jju32 myTimeCount;
    jju32 myTimeSlice;
}; /* end class Scheduler */

#endif

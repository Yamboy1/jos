/*
 * javathread.h
 *
 * declares the C++ class
 * which represents a Java
 * thread.
 */

#ifndef decaf_javathread
#define decaf_javathread

class Frame;
class JVM;

#include "dynamicstack.h"
typedef DynamicStack< Frame > FrameStack;

class Threadable;
class JavaThread;
class JavaThreadList;

#include "d_types.h"

/*
 * a Threadable object is any
 * object which may be the
 * subject of a monitor.
 */
 
class Threadable {

  public:
    Threadable() { myOwner = NULL; myLockCount = 0; myInterestedThreadsList = NULL; myMonitoringThreadsList = NULL; }

    bool wait( JavaThread * jt );
    bool notify( JavaThread * jt );
    bool notifyAll( JavaThread * jt );
    bool lock( JavaThread * jt );
    bool unlock( JavaThread * jt );

    JavaThread * getMyOwner() { return myOwner; }
    jju32 getMyLockCount() { return myLockCount; }
    void setMyLockCount( jju32 lockCount ) { myLockCount = lockCount; }
    
  protected:
    JavaThread * myOwner;
    jju32 myLockCount;
  
    JavaThreadList * myInterestedThreadsList;
    JavaThreadList * myMonitoringThreadsList;

	void tryToStartMonitoringThread();

}; /* end class Threadable */

class JavaThread {

  public:
    JavaThread() { myFrameStack = NULL; myJVM = NULL; }

    static JavaThread * generateJavaThread( JVM * jvm, Frame * f, jint priority );

    void pushFrame( Frame * f );
    Frame * popFrame();

    bool runOpcode();
    
    JVM * getMyJVM();
    jint getMyPriority();

  protected:
    FrameStack * myFrameStack;
    JVM * myJVM;
    jint myPriority;

}; /* end class JavaThread */

struct jtListem;
struct jtListItem { JavaThread * myJavaThread; jju32 myLockCount; };

class JavaThreadList {

  public:
  	JavaThreadList() { myHead = NULL; myRest = NULL; }

  	JavaThreadList( JavaThread * jt, jju32 count = 0, JavaThreadList * rest = NULL ) {
  		myHead->myJavaThread = jt;
  		myHead->myLockCount = count;
  		myRest = rest;
  		}

	void addThread( JavaThread * jt, jju32 count = 0 ) {
		JavaThreadList * cur = this;
		while ( cur->myRest != NULL ) { cur = cur->myRest; }
		if ( cur->myHead->myJavaThread == NULL ) {
			cur->myHead->myJavaThread = jt;
			cur->myHead->myLockCount = 0;
			} else {
			cur->myRest = new JavaThreadList( jt, count );
			}
		} /* end addThread() */
			
	void removeThread( JavaThread * jt ) {
		if ( myHead->myJavaThread == jt ) { removeMyFirstThread(); }

		/* otherwise, iterate over the loop to find it. */
		JavaThreadList * cur = this;
		JavaThreadList * prev = NULL;
		while ( cur != NULL ) {
			if ( cur->myHead->myJavaThread == jt ) {
				prev->myRest = cur->myRest;
				/* delete cur? */
				} /* end if removing cur */

			/* iterate the loop */
			prev = cur;
			cur = cur->myRest;				
			} /* end loop iteration. */
		} /* end removeThread() */

	bool isEmpty() { return (myHead == NULL); }
		
	void removeMyFirstThread() {
		if ( myRest != NULL ) {
			myHead->myJavaThread = myRest->myHead->myJavaThread;
			myHead->myLockCount = myRest->myHead->myLockCount;
			myRest = myRest->myRest;
			/* delete myRest? */
			} else { myHead = NULL; }
		}
	
	JavaThread * myFirstThread() { return myHead->myJavaThread; }
	jju32 myFirstLockCount() { return myHead->myLockCount; }

  protected:
  	JavaThreadList * myRest;
  	jtListItem * myHead;

}; /* end class JavaThreadList */

#endif

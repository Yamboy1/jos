/**
 * jvm.h
 * 
 * declares the decaf JVM class
 */

#ifndef decaf_jvm
#define decaf_jvm

class JVM;

#include "jjmachin.h" /* [e] */
#include "classloader.h"
#include "scheduler.h"
#include "frame.h"

class JVM {

  public:
    JVM();

    jjMachine * getMyMachine();
    void setMyMachine( jjMachine * jjm );

    ClassLoader * getMyPrimordialClassLoader();
    void setMyPrimordialClassLoader( ClassLoader * cl );

    JavaClassInstance * getInterruptClass();
    void setInterruptClass( JavaClassInstance * jc );

    void main( int argc, char** argv, Scheduler * sch );
	Scheduler * getMyScheduler() { return myScheduler; }

    /* For GC */
    static JVM * nextJVM;
    JVM * myNextJVM;

  protected:
    jjMachine * myMachine;
    ClassLoader * myClassLoader;
    JavaClassInstance * interruptClass;
    Scheduler * myScheduler;

}; /* end class jvm */

#endif

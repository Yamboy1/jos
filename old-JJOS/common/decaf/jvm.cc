/**
 * jvm.cc
 * 
 * defines the decaf JVM class
 */

#include "jvm.h"

/*
 * JVM::JVM()
 * 
 * initializes a JVM
 */

/* static initialization */
JVM * JVM::nextJVM = NULL;

/* for GC */
JVM * rootJVM = NULL;

JVM::JVM() {
   myClassLoader = NULL;
   myMachine = NULL;
   /* Maintain a statically-linked list of JVM's as the GC's roots. */
   if ( rootJVM == NULL ) {
        rootJVM = this;
        JVM::nextJVM = this;
        } else {
        JVM::nextJVM->myNextJVM = this;
        JVM::nextJVM = this;
        }
   }


/*
 * JVM::getMyMachine()
 * 
 * returns the jjMachine this JVM is using
 */

jjMachine * JVM::getMyMachine() {
  return myMachine;
  }


/*
 * JVM::setMyMachine()
 * 
 * sets the jjMachine for this JVM to use
 * assumes that anyone calling it has the right to
 */

void JVM::setMyMachine( jjMachine * jjm ) {
    myMachine = jjm;
    }


/*
 * JVM::getMyMachine()
 * 
 * returns the ClassLoader this JVM is using
 */
 
ClassLoader * JVM::getMyPrimordialClassLoader() {
    return myClassLoader;
    }


/*
 * JVM::[get|set]InterruptClass
 * 
 * returns/sets the Java class responsible for
 * handling the native->java interrupt bridging;
 * this class is VM-specific.
 */
 
JavaClassInstance * JVM::getInterruptClass() { 
    if ( interruptClass != NULL ) { return interruptClass; } 
    return (interruptClass = myClassLoader->getClass( "jos/system/interrupts.java" ));
    }
    
void JVM::setInterruptClass( JavaClassInstance * jc ) {
    interruptClass = jc;
    }

/*
 * JVM::setMyClassLoader()
 * 
 * sets the ClassLoader for this JVM to use
 * assumes that anyone calling it has the right to
 */

void JVM::setMyPrimordialClassLoader( ClassLoader * cl ) {
   myClassLoader = cl;
   }


/*
 * JVM::main()
 * 
 * starts the VM.
 * requires a non-NULL class loader and machine.
 */

void JVM::main( int argc, char ** argv, Scheduler * sch ) {
    /* syntax: <init class> */
	myScheduler = sch;

    char * init_class;
    if ( argc == 0 ) { init_class = "init"; }
    else { init_class = argv[1]; }

    if ( myMachine == NULL ) {
        kprintf( "jvm::main() -- NULL machine, aborting.\n" );
        abort();
        }
    if ( myClassLoader == NULL ) {
        kprintf( "jvm::main() -- NULL class loader, aborting.\n" );
        abort();
        }

    /* fetch the initial class */
    JavaClassInstance * jCI = myClassLoader->getClass( init_class );

    if ( jCI == NULL ) {
        kprintf( "jvm::main() -- NULL initial class, aborting.\n" );
        abort();
        }
	
    /* generate the initial frame */
    /* XXX: is the method descriptor correct? */
    Frame * f = Frame::generateFrame( jCI, "main", "([Ljava/lang/String;)V" );

    if ( f == NULL ) {
        kprintf( "jvm::main() -- NULL initial frame, aborting.\n" );
        abort();
        }

    /* generate the initial thread */
    JavaThread * jT = JavaThread::generateJavaThread( this, f, 8 );

    if ( jT == NULL ) {
        kprintf( "jvm::main() -- NULL initial thread, aborting.\n" );
        abort();
        }

    /* Start the scheduler with the generated thread running on this JVM. */
    sch->start( this, jT );
    } /* end main() */

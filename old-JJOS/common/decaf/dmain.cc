/*
 * main.cc
 * 
 * decaf JVM wrapper.
 */

#include "jvm.h"
#include "classloader.h"
#include "jjmachin.h" /* [e] */
#include "jjramdisk.h"
#include "scheduler.h"

void decaf_main( int argc, char ** argv, jjMachine * jjm, jjRamDisk * jjrd ) {
    /* the (single) VM instance */
    JVM * jvm = new JVM();

    /* generate the class loader */
    ClassLoader * cl = new ClassLoader();
    cl->setMyRamDisk( jjrd );

    /* generate the scheduler */
    Scheduler * sch = new Scheduler();

    /* prepare the JVM */
    jvm->setMyPrimordialClassLoader( cl );
    jvm->setMyMachine( jjm );

    /* execute */
    jvm->main( argc, argv, sch );
    } /* end main() */

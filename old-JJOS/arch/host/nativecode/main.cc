/*
 * The contents of this file are subject to the JJOS Public License
 * Version 0.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License
 * at http://www.jos.org/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is JJOS code, released October, 1998.
 *
 * The Initial Developer of the Original Code is John Morrison.
 * Portions created by John Morrison are Copyright (C) 1998.
 * All rights reserved.
 *
 * Contributor(s):
 *
 */

#include <stdlib.h>
#include <sys/types.h>

#include "jbheap.h"
#include "jjconsole.h"
#include "jjmachin.h"
#include "jjramdisk.h"
#include "jbrefcnt.h"

#ifdef OLD_DECAF
#include "d_main.h"
#else
#include "dmain.h"
#endif

#ifdef USE_CURSES
#include <curses.h>
#include <unistd.h>
#endif

#undef TEST_HEAP
#undef TEST_RAM_DISK
#undef TEST_RCO
#undef TEST_JJU64

#include "jvm.h"
#include "jbfifo.h"

/*
 *
 */

static void print_usage(char *program_name)
{
  kprintf("Usage: %s \"java_class java_class_arguments\"\n", program_name);
  kprintf("aborting...\n");
}

/*
 *
 */

#include "jbdebug.h"
static int debug = 0;

/*
 * 
 */

int main(int argc, char *argv[])
{
  jju32 top_of_stack = 0;	// $ Need to find for conservative GC's stack marking.
  jjMachine jjm;
  jjConsole console;
  jbHeap heap;

#ifdef USE_CURSES
  initscr();
  cbreak();
  noecho();
  nonl();
  nodelay( stdscr, TRUE );
  intrflush( stdscr, FALSE );
  keypad( stdscr, FALSE );
  scrollok( stdscr, TRUE );
#endif

  heap.theTopOfTheSingleStack = &top_of_stack; // $ Tell heap where stack top is.
  jjm.setMyConsole(&console); // we need the console to print any error messages that might come up.

  if (argc != 2)
    {
      print_usage(argv[0]);
      abort();
    }
  else
    {
      kprintf("jm's JOS Kernel Host!\n");
      jjm.setMyHeap(&heap);

      jjm.probePhysicalMemory();	// Finds out how much we've got.

      heap.setPhysicalLowAndHigh(jjm.myPhysicalMemoryLowerBound,
				 jjm.myPhysicalMemoryUpperBound);

      /*
       * Set up the ramdisk.
       */

      jjRamDisk ramdisk("../bytecode/jjos.zip", 0, 0);

      /*
       * Use default interrupt handlers.
       */

      jjm.assignInterrupts();

      /*
       * Prepare for interrupts.
       * (Might want to do this later under bytecode control.)
       */

      DEBUG(( "initializing interrupts...\n" ));
      jjm.enableInterrupts();

      /*
       * All the tests we might want to run.
       */

#ifdef TEST_HEAP
      jbmp->myHeap->testHeap();
#endif
    
#ifdef TEST_RAM_DISK
      ramdisk.test();
#endif

#ifdef TEST_RCO
      test_RCO();
#endif

#ifdef TEST_JJU64
      // would test jju64, but it seems to be working now.
      jju64 j1 = 15000;
      jju64 j2 = 2999;
      kprintf( "15000 / 2999 = %d; 15000 modulo 2999 = %d\n", j1 / j2, j1 % j2 );
#endif

      /*
       * Start running Java!
       */

      decaf_main( argc, argv, &jjm, &ramdisk );

      kprintf("memory used: %dK\n", heap.usedMemory() / 1024 );
      kprintf("main exiting -- OK!\n");
#ifdef USE_CURSES
      /* stall here so we can see the output */
      kprintf( "stalling so you can see the output...\n" );
      while ( getch() == ERR ) { sleep(1); }
      endwin();
#endif
    }
}

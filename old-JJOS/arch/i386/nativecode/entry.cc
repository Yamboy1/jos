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
 *   George Marrows (multiboot support)
 */

#include "jjconsole.h"
#include "jjmachin.h"
#include "stdlib.h"
#include "jbdebug.h"

#ifdef MULTIBOOT
  #include "jjmultiboot.h"
#elif ETHERBOOT
  #include "jjetherboot.h"
#else
  #error Unkown boot method.
#endif

#include "jjramdisk.h"
#include "jvm.h"

static int debug = 1;

/*
 * These next routines should be made methods on the jjEtherbootImageHeader.
 * GAM - glad you didn't - they're handy for multiboot too.
 */

#define MAX_NUMBER_ARGUMENTS 32

int process_args(char *s, int &argc, char ***argv)
{
  DEBUG(("process_args(\"%s\")\n", s));

  char **argvlocal = new (char *)[MAX_NUMBER_ARGUMENTS];
  *argv = argvlocal;
  argc = 0;

#ifdef NOTDEF
  /* Stick argv[0] as program name to simulate UNIX behavior. 
     -- now done in kernel_entry_point, cos the GRUB command
     line includes the program name.
   */

  argvlocal[argc] = strdup("arch/i386/nativecode/jjos-binary");
  argc++;
#endif

  /* Grab first argument -- handle 1st differently because of strtok call. */

  char *whitespace = " \t\n";
  argvlocal[argc] = strtok(s, whitespace);

  if (!argvlocal[argc])
    {
      kprintf("Problem getting first argument from string \"%s\"\n", s);
      return(argc);
    }
  else
    {
      DEBUG(("argvlocal[%d] = %s\n", argc, argvlocal[argc]));
      argc++;
    }

  /* Get the rest (up to MAX_NUMBER_ARGUMENTS). */

  for (; argc<MAX_NUMBER_ARGUMENTS; argc++)
    {
      argvlocal[argc] = strtok(NULL, whitespace);

      if (!argvlocal[argc])
	{
	  DEBUG(("bailing out on #%d\n", argc));
	  break;
	}
      else
	{
	  DEBUG(("argvlocal[%d] = %s\n", argc, argvlocal[argc]));
	}
    }
  return(argc);
}

int print_args(int argc, char *argv[])
{
  kprintf("argc = %d\n", argc);
  for (int i=0; i<argc; i++)
    {
      kprintf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }
  return(argc);
}

void test_args(int argc, char *argv[])
{
  char *argstring = strdup(" init arg0 arg1 ");

  int largc; char **largv;
  process_args(argstring, largc, &largv);
#ifdef NOTDEF
  print_args(largc, largv);
#endif
}

/*
 *
 */

void kernel_entry_point(void)
{
  /*
   * Stack allocation is safe before heap is initialized.
   */

  jju32 top_of_stack = 0;	// $ Need to find for conservative GC's stack marking.
  jjMachine jjm;
  jbHeap heap;

#ifdef MULTIBOOT
  jjConsole console(0xb8000);
  console.clear_screen();
#elif ETHERBOOT
  jjConsole console(0xb0000);
  /* screen cleared in ASM, which also ensures text mode --
     this needs to be fixed in head_grub.s86 */
#endif

  jjm.setMyConsole(&console);	 // Set up the console for kprintf.

  kprintf("jJOS / decaf\n");	 // Show some manners.

  jjm.setMyHeap(&heap);		 // Set up a pointer to the heap we'll use.

  heap.theTopOfTheSingleStack = &top_of_stack; // $ Tell heap where stack top is.

#ifdef MULTIBOOT

  /* Get holder of the multiboot_info structure */
  
  jjMultibootInfo * mbi = jjMultibootInfo::getMultibootInfo();

  if (debug) mbi->dump();

  /* A couple of integrity checks */

  if (!mbi->knowsMemory()) {
    kprintf("!!! Multiboot: unknown memory size - aborting !!!");
    jjm.halt();
  }

  if (mbi->getModuleCount() != 1) {
    kprintf("!!! Multiboot: needs exactly one module (jjos.zip) - aborting !!!");
    jjm.halt();
  }


  /* 
   *  Set up memory and ramdisk.
   *  GRUB loads modules immediately after the kernel image at the
   *  *bottom*  of memory. There doesn't seem to be anything in 
   *  the multiboot spec says it has to though :-(
   */

  jjm.probePhysicalMemory(mbi->getMemUpper());
  
  heap.setPhysicalLowAndHigh( mbi->getModule(0)->getEnd(),
			      jjm.myPhysicalMemoryUpperBound );

  jjRamDisk jjramdisk( mbi->getModule(0)->getStart(),
		       mbi->getModule(0)->getEnd());

#elif ETHERBOOT


  /* !!! This bit needs checking !!! */

  jjm.probePhysicalMemory();

  /*
   * This next bit is Etherboot-specific.
   * Don't use all physmem -- stop before ramdisk.
   */

  jjEtherbootImageHeader::dealWithEtherboot(); // Find the ramdisk, etc.
  heap.setPhysicalLowAndHigh(jjm.myPhysicalMemoryLowerBound,
			     jjEtherbootRamDiskLocation);


  /*
   * Set up the ramdisk.
   */
  jjRamDisk jjramdisk( jjEtherbootRamDiskLocation, 
		       jbmp->myPhysicalMemoryUpperBound);

#else
  #error Unknown boot method.
#endif


  /*
   * Use default interrupt handlers.
   */

  jjm.assignInterrupts();

  /*
   * Prepare for interrupts.
   * (Might want to do this later under bytecode control,
   * except that initializing the FP may generate exceptions,
   * and we need the FP initialized before we start running
   * the JVM).
   */

   jjm.enableInterrupts();
 
  /*
   * Enable the floating-point unit (which might generate fp exceptions,
   * so we do this AFTER we've initialized the interrupt subsystem).
   */

   jjm.initializeFloatingPoint();

#ifdef TEST_A20
        int* three = (int*)0x300000;
        int* four = (int*)0x400000;
        *three = 3;
        *four = 4;
        if ( *three == 3 && *four == 4 ) { kprintf( "A20 OK, loading decaf...\n"); }
        else { kprintf( "A20 FAILURE!\n" ); return; }
#endif

  /*
   * Jump off to the decaf Java VM!
   */

  {
    int argc;
    char **argv;

#ifdef MULTIBOOT

    process_args( mbi->getCmdline(), argc, &argv);

#elif ETHERBOOT
    
    /*
      Patch in a UNIX-like program name as argv[0].
      Unfortunately this sits on the stack forever.
     */

    char cmdline[255];
    char *s = cmdline;
    
    s = strcpy(s, "arch/i386/nativecode/jjos-binary ");
    strcpy(s, jjEtherbootCommandLineString);

    process_args(cmdline, argc, &argv);

#else
    #error Unknown boot method.
#endif

#ifdef OLD_DECAF
    decaf_main(argc, argv, &jjm, &jjramdisk);
#else
#warning entry.cc -- needs to properly call decaf; see host/nativecode
#endif
  }

  if ( debug ) { kprintf("memory used: %dK\n", heap.usedMemory() / 1024 ); }
  kprintf("jjos / decaf exiting... OK.\n");
}

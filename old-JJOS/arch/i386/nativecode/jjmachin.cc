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
 *  George Marrows
 */

#include "stdlib.h"
#include "jjmachin.h"

#include "jjetherboot.h"
#include "jjeflags.h"
#include "jjvinfo.h"
#include "jjfinfo.h"

#include "jbdebug.h"
static int debug = 0;

#ifdef NOTDEF
#include "queue.cc"
extern queue<int> interrupt_queue;
int interrupt = -1;
#endif

/*
 *
 */

jjMachine::jjMachine()
  : jj386Interrupts()
{
  ;
}

void jjMachine::halt(void)
{
  /*
   * Seems like we were rolling through this halt(). -jm
   * I think it's because interrupts were enabled,
   * so the next interrupt that came along restored the PC
   * AFTER the halt.
   *
   * I suppose I could've used a goto->halt infinite loop, too,
   * and let the interrupts continue.
   */

  disableInterrupts();
  
  asm("hlt");
}

void jjMachine::getCPUID(jju32 function_code,
			 jju32 &eax, jju32 &ebx, jju32 &ecx, jju32 &edx)
{
  /*
   * Gotta store to these temp vars instead of directly.
   * Otherwise, can't generate enough indirection operands.
   * Also don't want to use one of the registers we want to
   * save as scratch space.
   */

  jju32 eaxtemp = 0;
  jju32 ebxtemp = 0;
  jju32 ecxtemp = 0;
  jju32 edxtemp = 0;

  asm("mov %0, %%eax" : : "m" (function_code));
  asm("cpuid");

  asm("mov %%eax, %0" : "=m" (eaxtemp));
  asm("mov %%ebx, %0" : "=m" (ebxtemp));
  asm("mov %%ecx, %0" : "=m" (ecxtemp));
  asm("mov %%edx, %0" : "=m" (edxtemp));

  eax = eaxtemp;
  ebx = ebxtemp;
  ecx = ecxtemp;
  edx = edxtemp;

  DEBUG(("jjMachine::getCPUID -- \n"));
  DEBUG(("eax input = %X\n", function_code));
  DEBUG(("eax = %X\n", eax));
  DEBUG(("ebx = %X\n", ebx));
  DEBUG(("ecx = %X\n", ecx));
  DEBUG(("edx = %X\n", edx));
}


jjBoolean jjMachine::identificationIsSupported(void)
{
  /*
   * Must try and set bit 21 and see if it "takes."
   * From Undocumented PC, page 65.
   */

  jjEFLAGS eflags;

  eflags.getEFLAGS();		// Get the current value.
  eflags.setCPUIDSupported();
  eflags.setEFLAGS();		// Set the new value.
  eflags.getEFLAGS();		// Read it again, see if it "took."

  if (eflags.identificationIsSupported())
    {
      DEBUG(("jjMachine::identificationIsSupported -- TRUE\n"));
      return(JJTRUE);
    }
  else
    {
      DEBUG(("jjMachine::identificationIsSupported -- FALSE\n"));
      return(JJFALSE);
    }
}

void jjMachine::initializeFloatingPoint(void)
{
  /*
   * We want to know if we've got an FPU.
   * The CPUID instruction will tell us.
   * Unfortunately, it's not universally supported on all x86 parts.
   */

  if (!identificationIsSupported())
    {
      /*
       * We'll end up here for i386, early i486, and probably Nx586.
       *
       * It's probably OK to use fninit/finit on some of these parts,
       * so I'm probably being overly restrictive.
       *
       * To try this with your part, just blow off this whole eflags test,
       * and just charge ahead with fninit/finit (below).
       *
       * Hey, if it works, let us know!
       *
       * Someday, maybe somebody'll do a software floating-point emulation.
       */

      kprintf("jjMachine::initializeFloatingPoint -- CPUID is not supported.\n");
      kprintf("No way to find out whether we have FPU\n");
      kprintf("Software FP not supported.\n");
      halt();
    }
  else
    {
      /*
       * Step 1.  Find out what kind of CPU we're running on,
       * and what's max EAX "function" value for subsequent CPUID queries.
       */

      jju32 eax_contents = 0;
      jju32 ebx_contents = 0;
      jju32 ecx_contents = 0;
      jju32 edx_contents = 0;

      DEBUG(("get CPUID information with EAX=0.\n"));

      getCPUID(0, eax_contents, ebx_contents, ecx_contents, edx_contents);

      kprintf("Processor type: \"%c%c%c%c",
	      ((ebx_contents) & 0xff),
	      ((ebx_contents>>8) & 0xff),
	      ((ebx_contents>>16) & 0xff),
	      ((ebx_contents>>24) & 0xff));

      kprintf("%c%c%c%c",
	      ((edx_contents) & 0xff),
	      ((edx_contents>>8) & 0xff),
	      ((edx_contents>>16) & 0xff),
	      ((edx_contents>>24) & 0xff));

      kprintf("%c%c%c%c\"\n",
	      ((ecx_contents) & 0xff),
	      ((ecx_contents>>8) & 0xff),
	      ((ecx_contents>>16) & 0xff),
	      ((ecx_contents>>24) & 0xff));


      if (eax_contents < 1)
	{
	  /* Bummer.  Can't use function 1 to find out if we've got FPU. */
	  kprintf("Processor doesn't support CPUID instruction with function 1\n");
	  kprintf("Now way to find out whether we have FPU\n");
	  kprintf("Software FP not supported.\n");
	  halt();
	}
      else
	{
	  jju32 eax_version_information = 0;
	  jju32 ebx_reserved = 0;
	  jju32 ecx_reserved = 0;
	  jju32 edx_feature_information = 0;

	  DEBUG(("get CPUID information with EAX=1.\n"));

	  getCPUID(1,
		   eax_version_information,
		   ebx_reserved,
		   ecx_reserved,
		   edx_feature_information);
/*
	  jjVersionInformation ignored_version_info(eax_version_information);
*/
	  jjFeatureInformation feature_info(edx_feature_information);

	  if (!feature_info.hasFPUOnChip())
	    {
	      /*
	       * Bummer.  No FPU on-chip, maybe none at all.
	       * Not sure what i386/i387 combos look like.
	       */

	      kprintf("Processor apparently has no FPU on-chip, and we need one.\n");
	      kprintf("Software FP not supported.\n");
	      halt();
	    }
	  else
	    {
	      DEBUG(("Initializing Floating-Point Unit..."));

#ifdef IGNORE_PENDING_FP_EXCEPTIONS
	      /*
	       * According to the Intel docs, this instruction DOES NOT check
	       * for and handle any pending unmasked floating-point exceptions
	       * BEFORE performing the initialization.
	       */

	      asm("fninit");

#else

	      /* This one DOES check. */

	      asm("finit");

#endif
	      DEBUG(("Done!\n"));
	    }
	}
    }
}


void jjMachine::probePhysicalMemory(void)
{
  unsigned long base = 0x100000;

  /* Find lower bound -- skip the video memory, etc. */
  void *one_megabyte = (void *)base;

  myPhysicalMemoryLowerBound = one_megabyte;

  /* Find upper bound -- */

  /* This sucker is set in head.s86 after a BIOS call. */

  extern unsigned short xmsize_in_kbytes;

  /* Convert Extended Memory size in kbytes to bytes. */

  unsigned long xmsize_in_bytes = xmsize_in_kbytes * 1024;

  /* Add Extended Memory to base memory. */

  unsigned long upper_bound = xmsize_in_bytes + base;

  DEBUG(("physicalMemoryUpperBound=%X", upper_bound));

  myPhysicalMemoryUpperBound = ((void *)upper_bound);
}

/* static */
void *jjMachine::convertFarPointerToLinearAddress(jju32 far_pointer)
{
  jju32 linear_address_u32 = (far_pointer >> 12) & 0xfffff; // Segment
  linear_address_u32 += far_pointer & 0xffff; // Offset

  /* Coerce to pointer value. */

  void *linear_address = (void *)linear_address_u32;

  DEBUG(("jjMachine::convertFarPointerToLinearAddress -- linear_address = %X\n",
	 linear_address));

  return(linear_address);
}


/*
 * CPU-specific, CPU initialization stuff.
 */


/*
 * 
 */

jju8 jjMachine::in8(jju16 port)
{
  jju8 retval = 0;

  asm("movw %0, %%dx" : : "m" (port)); // Move portnum into dx.
  asm("inb %dx, %al");		// Read port into al.
  asm("movb %%al, %0" : "=m" (retval)); // Move al's value into retval

  return(retval);
}

jju16 jjMachine::in16(jju16 port)
{
  jju16 retval = 0;

  asm("movw %0, %%dx" : : "m" (port)); // Move portnum into dx.
  asm("inw %dx, %ax");		// Read port into ax.
  asm("movw %%ax, %0" : "=m" (retval)); // Move ax's value into retval

  return(retval);
}

jju32 jjMachine::in32(jju16 port)
{
  jju32 retval = 0;

  asm("movl %0, %%dx" : : "m" (port)); // Move portnum into dx.
  asm("inl %dx, %eax");		// Read port into ax.
  asm("movl %%eax, %0" : "=m" (retval)); // Move eax's value into retval

  return(retval);
}
 
void jjMachine::out8(jju16 port, jju8 value)
{
  asm("movw %0, %%dx" : : "m" (port));
  asm("movb %0, %%al" : : "m" (value));
  asm("outb %al, %dx");
}

void jjMachine::out16(jju16 port, jju16 value)
{
  asm("movw %0, %%dx" : : "m" (port));
  asm("movw %0, %%ax" : : "m" (value));
  asm("outw %ax, %dx");
}

void jjMachine::out32(jju16 port, jju32 value)
{
  asm("movw %0, %%dx" : : "m" (port));
  asm("movl %0, %%eax" : : "m" (value));
  asm("outl %eax, %dx");
}


/* added by George Marrows for multiboot */

void jjMachine::probePhysicalMemory(jju32 xmsize_in_kbytes) {

  /*
    Variant in which jjMachine is *told* how much memory
    there is. Used by GRUB multiboot.
   */

  unsigned long base = 0x100000;

  /* Find lower bound -- skip the video memory, etc. */
  void *one_megabyte = (void *)base;

  myPhysicalMemoryLowerBound = one_megabyte;

  /* Find upper bound -- */

  /* Convert Extended Memory size in kbytes to bytes. */

  unsigned long xmsize_in_bytes = xmsize_in_kbytes * 1024;

  /* Add Extended Memory to base memory. */

  unsigned long upper_bound = xmsize_in_bytes + base;

  DEBUG(("physicalMemoryUpperBound=%X", upper_bound));

  myPhysicalMemoryUpperBound = ((void *)upper_bound);
  
}


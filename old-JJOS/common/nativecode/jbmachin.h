#ifndef jbMachine_h
#define jbMachine_h

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

#include "jjtypes.h"
#include "jbheap.h"
#include "jbconsol.h"

/*
 * Base machine abstraction.
 */

class jbMachine
{
public:
  jbMachine();

  virtual void halt(void) = 0;
  void delay(void);		/* Busy wait a little while... */

  void setMyConsole(jbConsole *);
  void setMyHeap(jbHeap *);

  /* For handling or simulating interrupts. */
  virtual jjBoolean	interruptFIFOEmpty(void) = 0;
  virtual jju16		popOldestInterruptNumber(void) = 0;

  virtual void printBacktrace(void); /* Default will work for vanilla C++ stack discipline. */

  jbConsole *myConsole;
  jbHeap    *myHeap;

#ifdef NOTDEF
 protected:
#endif

  /* Interrupt handling. */

  virtual void assignInterrupts(void) = 0; /* Would've like to do this via assembler. */
  virtual void enableInterrupts(void) = 0;
  virtual void disableInterrupts(void) = 0;

  virtual void probePhysicalMemory(void) = 0;
  virtual void _printBacktrace(jju32 *linkptr); /* The thing that actually walks the C++ stack. */

  void zeroBss(void);

  jjpaddr myPhysicalMemoryLowerBound; /* Inclusive, above kernel, stack, video mem, etc. */
  jjpaddr myPhysicalMemoryUpperBound; /* Non-inclusive. */
};

extern jbMachine *jbmp;

#endif

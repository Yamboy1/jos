#ifndef JJMACHIN_H
#define JJMACHIN_H

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

#include "jbmachin.h"
#include "jbfifo.h"
#include "jjtypes.h"

#include <sys/types.h>
#define __need_timeval
//#include <timebits.h>		// For timeval
#include <time.h>

/*
 *
 */

class jjMachine: public jbMachine
{
public:
  jjMachine();

  void halt(void);
  void assignInterrupts(void);
  void enableInterrupts(void);
  void disableInterrupts(void);

  /* For simulating interrupts on the host. */
  jjBoolean	interruptFIFOEmpty(void);
  jju16		popOldestInterruptNumber(void);

  void processCommandLineArguments(void);

  void probePhysicalMemory(void);

  int putChar(int c);

 protected:

  fd_set	readfds;	/* File descriptor mask for select(). */
  struct timeval timeout;	/* Timeout value for select(). */
};

#endif

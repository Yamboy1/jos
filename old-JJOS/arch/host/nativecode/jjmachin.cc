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
 * Ryan Heise <ryan@whitewolf.com.au>
 *
 */

#include "stdlib.h"
#include "jbfifo.cc"
#include "jjmachin.h"
#include "jbdebug.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef USE_CURSES
#include <curses.h>
#endif

static int debug = 0;

#define HEAP_SIZE_KBYTES 16000

jjMachine::jjMachine()
  : jbMachine()
{
}

void jjMachine::probePhysicalMemory(void)
{
  /*
   * Set up the heap.
   */

  myPhysicalMemoryLowerBound = (char *)malloc(HEAP_SIZE_KBYTES*1024);
  myPhysicalMemoryUpperBound = myPhysicalMemoryLowerBound + (HEAP_SIZE_KBYTES*1024) + 1;
}

void jjMachine::halt(void)
{
	abort();
}

void jjMachine::assignInterrupts(void)
{
  ; // Should probably set signal handlers.
}

void jjMachine::enableInterrupts(void)
{
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
}

void jjMachine::disableInterrupts(void)
{
  ;
}

void jjMachine::processCommandLineArguments(void)
{
  ;
}

jjBoolean jjMachine::interruptFIFOEmpty(void)
{
#ifdef USE_CURSES
 int c = getch();
 if ( c == ERR ) {
	return (JJTRUE);
	} else {
	ungetch(c);
	return (JJFALSE);
	}
#else

  /* Set up descriptor mask to check stdin. */
  FD_ZERO(&readfds);
//  FD_SET(0, &readfds);
  FD_SET(STDIN_FILENO, &readfds);

  int number_descriptors = select(3, &readfds, NULL, NULL, &timeout);

  if (number_descriptors == -1)
    {
#ifdef NOTDEF
      kprintf("jjMachine::interruptFIFOEmpty -- select failed\n");
      abort();
#else
      DEBUG(("jjMachine::interruptFIFOEmpty -- select failed -- NOT aborting\n"));
#endif
      return(JJTRUE);
    }
  else if (number_descriptors == 0)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
#endif
}

jju16 jjMachine::popOldestInterruptNumber(void)
{
  DEBUG(("readfds = %X\n", readfds));

#ifdef USE_CURSES
  return 9;
#else
  if (FD_ISSET(0, &readfds))
    {
      DEBUG(("BIT 0\n"));
      char c;
      read(0, &c, sizeof(c));
      DEBUG(("<<%c>>", c));
      /* might as well direct it to the keyboard handler... */
      return(9);
    }
  else
    {
      kprintf("jjMachine::popOldestInterruptNumber -- AGH!\n");
      return(0);
    }
#endif
}

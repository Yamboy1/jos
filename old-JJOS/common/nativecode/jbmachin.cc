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

#include "stdlib.h"
#include "jbmachin.h"
#include "jbdebug.h"

static int debug = 0;


jbMachine *jbmp = 0;

jbMachine::jbMachine()
{
  jbmp = this;
  zeroBss();

  myConsole = NULL;

  myPhysicalMemoryLowerBound = NULL;
  myPhysicalMemoryUpperBound = NULL;
}

void jbMachine::zeroBss(void)
{
#ifdef TARGET_UNIX
  ;
#else
  extern unsigned char *__bss_start;
  extern unsigned char *_end;

  /* Use a longer word and it will be faster. */

  register unsigned char *bssp = (unsigned char *)&__bss_start;

  /* while (bssp < &_end ) */
  while (bssp < _end) 
    {
      *bssp++ = 0x0;
    }
#endif
}


void jbMachine::delay(void)
{
  for (volatile int i=0; i<10000000; i++)
    {
      ;
    }
}

void jbMachine::setMyConsole(jbConsole *console_to_use)
{
  myConsole = console_to_use;
}

/*
 * Heap-related stuff.
 */

void jbMachine::setMyHeap(jbHeap *heap)
{
  DEBUG(("jbMachine::setMyHeap -- "));
  myHeap = heap;
}


/*
 *
 */
void jbMachine::_printBacktrace(jju32 *linkptr)
{
  if (!linkptr)
    {
      ;
    }
  else
    {
      jju32 pc = linkptr[1];

      /*
       * These may not actually have been pushed, so this may be misleading.
       */

      jju32 param0 = linkptr[2];
      jju32 param1 = linkptr[3];
      jju32 param2 = linkptr[4];
      jju32 param3 = linkptr[5];
      
      /*
       * Print out in reverse order.
       */

      jju32 *next_link_value = (jju32 *)*linkptr;
      _printBacktrace((jju32 *)next_link_value);

      kprintf("%X ( %X, %X, %X, %X, ...)\n", pc, param0, param1, param2, param3);
    }
}

void jbMachine::printBacktrace(void)
{
#ifdef NOTDEF
  jju32 *stack[0];

  for (int i=0; i<10; i++)
    {
      kprintf("stack[%d] = %X\n", i, stack[i]);
    }
#else
  jju32 *link[0];
  _printBacktrace(link[0]);
#endif
}


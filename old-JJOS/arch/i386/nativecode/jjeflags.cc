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

#include "jjeflags.h"

#include "jbdebug.h"

static int debug = 0;

void jjEFLAGS::getEFLAGS(void)
{
  /* From Undocumented PC, page 64. */

  asm("pushfl");		// Push flags onto stack.
  asm("popl %eax");		// Pop into eax.
  asm("movl %%eax, %0" : "=m" (myRawBits)); // Move from eax to member variable.

  if (debug)
    {
      kprintf("jjEFLAGS::getEFLAGS -- ");
      dump();
    }
}

void jjEFLAGS::setEFLAGS(void)
{
  if (debug)
    {
      kprintf("jjEFLAGS::setEFLAGS(0x%X)\n", myRawBits);
    }

  asm("pushl %0": "=m" (myRawBits)); // Push onto stack.
  asm("popfl");			// Pop into eflags register.
}

  
void jjEFLAGS::setCPUIDSupported(void)
{
  myRawBits |= 0x200000;
}


void jjEFLAGS::dump(void)
{
  kprintf("EFLAGS = 0x%X = ", myRawBits);

  if (identificationIsSupported())
    {
      kprintf(" ID ");
    }

  kprintf("\n");
}

jjBoolean jjEFLAGS::identificationIsSupported(void)
{
  if (myRawBits & 0x200000)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}


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

void jbConsole::printString(char *pp)
{
  int c;

  while ((c = *pp++))
    {
      putChar(c);
    }
}

void jbConsole::printULong(unsigned long l)
{
  char buf[2048];
  register int i;

  for (i=0; i<2048; i++)
    {
      buf[i] = '\0';
    }

  itoa(buf, l);
  printString(buf);
}

void jbConsole::printVoid(void *v)
{
  printULong((unsigned long)v);
}

void jbConsole::printXs(void)
{
  for (int i=0; i<10; i++)
    {
      putChar('X');
    }
}


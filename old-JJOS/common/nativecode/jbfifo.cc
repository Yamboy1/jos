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

#include "jbfifo.h"

template<class qelem_t, jju32 size>
jbFIFO<qelem_t, size>::jbFIFO()
{
  myHead = 0;
  myTail = 0;
  myBitmask = size - 1;
}

template<class qelem_t, jju32 size>
void jbFIFO<qelem_t, size>::dump(void)
{
  kprintf("jbFIFO::dump() -- myHead=%d, myTail=%d, quantity=%d, myBitmask=0x%X\n",
	  myHead, myTail, quantity(), myBitmask);
}

template <class qelem_t, jju32 size>
jju32 jbFIFO<qelem_t, size>::quantity(void)
{
  return(myHead - myTail);
}

template<class qelem_t, jju32 size>
jjBoolean jbFIFO<qelem_t, size>::isEmpty(void)
{
  if (myHead == myTail)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

template <class qelem_t, jju32 size>
jjBoolean jbFIFO<qelem_t, size>::isFull(void)
{
  if (quantity() == myBitmask)	// Really, size - 1 
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

template <class qelem_t, jju32 size>
qelem_t jbFIFO<qelem_t, size>::pop(void)
{
#ifdef PARANOID
  if (isEmpty())
    {
      kprintf("jbFIFO::pop -- attempt to pop empty FIFO!\n");

      /*
       * Not sure what the Right Thing To Do is.
       * Gotta give'em *something", so just give'em
       * what the constructor (if any) leaves.
       */

      qelem_t whatever_constructor_if_any_leaves;

#ifdef NOTDEF
      jbmp->printBacktrace();
      jbmp->halt();
#endif

      return(whatever_constructor_if_any_leaves);
    }
  else
#endif
    {
      myTail++;
      jju32 index = myTail & myBitmask;
      qelem_t retval = myElements[index];
      return(retval);
    }
}

template <class qelem_t, jju32 size>
jjBoolean jbFIFO<qelem_t, size>::push(qelem_t new_element)
{
  if (isFull())
    {
      return(JJFALSE);
    }
  else
    {
      myHead++;
      jju32 index = myHead & myBitmask;
      myElements[index] = new_element;
      return(JJTRUE);
    }
}

#ifdef NOTDEF
class foo
{
public:
  foo();

  int baz;
  int quux;
  int *frob;
};
  
foo::foo()
{
  baz = 1;
  quux = 2;
  frob = NULL;
}

typedef jbFIFO<int, 4> ififot;
typedef jbFIFO<foo, 4> foofifot;

void test_fifo(void)
{
  jju32 tail = 0xfffffffc;
  jju32 head = 0x1;
  jju32 difference = head - tail;
  kprintf("difference = %U = %d\n", difference, difference);

  ififot interrupt_fifo;
  interrupt_fifo.dump();
  kprintf("\n\n");

#ifndef NOTDEF
  foofifot foo_fifo;
  foo_fifo.dump();
  foo f;
  f = foo_fifo.pop();
  kprintf("\n\n");
#endif

  for (int i=1; i<=4; i++)
    {
      kprintf("i=%d: ", i);

      if (interrupt_fifo.isEmpty())
	  kprintf("empty: ");
      if (interrupt_fifo.isFull())
	  kprintf("full: ");
      interrupt_fifo.dump();

      interrupt_fifo.push(i);

      if (interrupt_fifo.isEmpty())
	  kprintf("empty: ");
      if (interrupt_fifo.isFull())
	  kprintf("full: ");
      interrupt_fifo.dump();

      kprintf("\n");
    }

  for (int j=1;j<=4;j++)
    {
      if (!interrupt_fifo.isEmpty())
	{
	  int value = interrupt_fifo.pop();
	  kprintf("Dequeue %d = %d\n", j, value);
	}
      else
	{
	  kprintf("EMPTY! %d\n", j);
	}
    }
}


#endif

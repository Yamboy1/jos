#ifndef JBLIFO_H
#define JBLIFO_H

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

template <class Type>
class jbLIFO
{
 public:
  jbLIFO();
  ~jbLIFO();

  void		init(void);
  jjBoolean	isEmpty(void);
  jjBoolean	isFull(void);
  jju32		length(void);
  Type		pop(void);
  jjBoolean	push(const Type&);
  
 protected:

  static jju32	theMaxNumElementsKludge = 256;
  jju32		myFirstEmpty;
  Type		myElements[theMaxNumElementsKludge];
};

template <class Type>
jbLIFO<Type>::jbLIFO()
{
  init();
}

template <class Type>
void jbLIFO<Type>::init(void)
{
  myFirstEmpty = 0;
};

template <class Type>
jjBoolean jbLIFO<Type>::isEmpty(void)
{
  if (myFirstEmpty == 0)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

template <class Type>
jjBoolean jbLIFO<Type>::isFull(void)
{
  if (myFirstEmpty == theMaxNumElementsKludge)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

template <class Type>
jju32 jbLIFO<Type>::length(void)
{
  return(myFirstEmpty);
}

template <class Type>
Type jbLIFO<Type>::pop(void)
{
  if (isEmpty())		/* Very Bad Karma.  Nothing to pop. */
    {
      kprintf("jbLIFO<Type>::pop -- LIFO is empty\n");

      /* There is no sensible return value at this point. */
      Type whatever_constructor_leaves;
      return(whatever_constructor_leaves);
    }
  else
    {
      return(myElements[--myFirstEmpty]);
    }
}

template <class Type>
jjBoolean jbLIFO<Type>::push(const Type &value)
{
  if (isFull())
    {
      kprintf("jbLIFO<Type>::push -- LIFO is full\n");
      return(JJFALSE);
    }
  else
    {
      myElements[myFirstEmpty++] = value;
      return(JJTRUE);
    }
}

#endif

#ifndef JBFIFO_H
#define JBFIFO_H

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

/*
 * First-In, First-Out queue.
 * "size" MUST be a power of two or this won't work.
 * Really holds size-1 maximum.
 * Could probably use jj16 instead of jju32 for counts and mask.
 */

#include "jjtypes.h"

template <class qelem_t, jju32 size> class jbFIFO
{
public:

  jbFIFO();
  jjBoolean	isEmpty(void);
  jjBoolean	isFull(void);
  jju32		quantity(void);
  qelem_t	pop(void);
  jjBoolean	push(qelem_t);
  void		dump(void);

protected:

  /* It is important we use unsigned counts so rollover works. */

  jju32		myHead;		// Index of youngest full slot.
  jju32		myTail;		// Index of youngest empty slot.
  jju32		myBitmask;
  qelem_t	myElements[size];
};

extern void test_fifo(void);

#endif

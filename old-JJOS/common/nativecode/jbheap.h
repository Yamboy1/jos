#ifndef JBHEAP_H
#define JBHEAP_H

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

class jbHeap
{
public:

  jju32 setPhysicalLowAndHigh(void *low_mem_address_inclusive,
			      void *high_mem_address_exclusive);


  jju32 maxSize(void);
  void *allocate(jju32 size);
  void deallocate(void *thing_to_deallocate);

  jju32 usedMemory(void);

  void testHeap(void);

  jjBoolean locationWithinHeap(void *);
  void zeroPhysicalMemory(); 	       /* -_Quinn to get GC to compile. */
  static void *theTopOfTheSingleStack; /* $ Needed only for conservative GC. */

protected:

  void scrubMemory(void);

  jju8 *myLowMemAddressInclusive;
  jju8 *myHighMemAddressExclusive;

  jju8 *myNextToAllocate;

};

#endif

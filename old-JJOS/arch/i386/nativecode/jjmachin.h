#ifndef jjMachine_h
#define jjMachine_h

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
 *   George Marrows
 */

#include "jj386int.h"

class jjMachine: public jj386Interrupts
{
public:

  jjMachine();

  void halt(void);
  void initializeFloatingPoint(void);
  void probePhysicalMemory(void);
  void probePhysicalMemory(jju32 xmsize_in_kbytes);

  static void *convertFarPointerToLinearAddress(jju32 far_pointer);

  /* These access I/O space. */

  jju8  in8(jju16 port);
  jju16 in16(jju16 port);
  jju32 in32(jju16 port);
  void  out8(jju16 port, jju8);
  void  out16(jju16 port, jju16);
  void  out32(jju16 port, jju32);

 protected:

  void getCPUID(jju32 function_code, jju32 &eax, jju32 &ebx, jju32 &ecx, jju32 &edx);
  jjBoolean identificationIsSupported(void); /* If CPUID instruction is supported. */

};



#endif

#ifndef JBRAMDISK_H
#define JBRAMDISK_H

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
#include "stdlib.h"

class jbRamDiskStream
{
 public:

  jbRamDiskStream(jju8 *first_byte, jju8 *last_byte);

  /* Return JJEOF on end-of-file. */
  int get(void);

  /* Return JJTRUE on success, JJFALSE otherwise. */
  jjBoolean readjju8(jju8 &);
  jjBoolean readjju16(jju16 &);
  jjBoolean readjju32(jju32 &);

  jju8 *myFirstByte;
  jju8 *myLastByte;
  jju8 *myCurrentByte;
};

class jbRamDisk
{
public:

  jbRamDisk();

  jbRamDiskStream *openStream(char *filename);
  void dump(void);
  void test(void);

protected:

  jju8 *myFirstByte;
  jju8 *myLastByte;
};


#endif

#ifndef JJFINFO_H
#define JJFINFO_H

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

class jjFeatureInformation
{
 public:

  jjFeatureInformation(jju32);

  /* Obviously, I was too lazy to do all 15 (or so) of these. */

  jjBoolean hasFPUOnChip(void) const;

 private:
  jju32 myRawBits;
};

#endif

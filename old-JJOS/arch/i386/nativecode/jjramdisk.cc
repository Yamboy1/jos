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

#include "jjramdisk.h"
#include "jbdebug.h"

static int debug = 0;

jjRamDisk::jjRamDisk(void *start_phys_addr, void *end_phys_addr)
  : jbRamDisk()
{
  DEBUG(("jjRamDisk::jjRamDisk(%X, %X)\n",
	 start_phys_addr,
	 end_phys_addr));
  myFirstByte = (jju8*)start_phys_addr;
  myLastByte = (jju8*)end_phys_addr;
}
    


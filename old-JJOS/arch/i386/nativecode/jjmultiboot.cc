/*
 *    The contents of this file are subject to the JJOS Public License
 *    Version 0.0 (the "License"); you may not use this file except in
 *    compliance with the License. You may obtain a copy of the License at
 *    http://www.jos.org/
 *
 *    Software distributed under the License is distributed on an "AS IS"
 *    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 *    License for the specific language governing rights and limitations
 *    under the License.
 *
 *    The Original Code is jjos code, released October, 1998.
 *
 *    The Initial Developer of the Original Code is George Marrows
 *    Portions created by George Marrows  are Copyright (C) 1999.
 *    All Rights Reserved.
 *
 */



#include "jjmultiboot.h"

#include "stdlib.h"

/**********************************************************
 * jjMultibootInfo
 **********************************************************/

jjMultibootInfo * jjMultibootInfo::getMultibootInfo(void) {
  
  /*
    Returns the jjMultibootInfo object - it was deposited
    in the address given by multiboot_info_ptr at boot time.
   */

  extern jjMultibootInfo * multiboot_info_ptr;
  return multiboot_info_ptr;

}


#define MEMORY_FLAG                  (1 << 0)
#define BOOTDEVICE_FLAG              (1 << 1)
#define CMDLINE_FLAG                 (1 << 2)
#define MODULES_FLAG                 (1 << 3)
#define ELF_SECTION_HEADER_FLAG      (1 << 5)
#define MEMORY_MAP_FLAG              (1 << 6)


jjBoolean jjMultibootInfo::knowsMemory(void) {
  return (flags & MEMORY_FLAG) ? JJTRUE : JJFALSE;
}

jjBoolean jjMultibootInfo::knowsBootDevice(void) {
  return (flags & BOOTDEVICE_FLAG) ? JJTRUE : JJFALSE;
}

jjBoolean jjMultibootInfo::knowsCmdline(void) {
  return (flags & CMDLINE_FLAG) ? JJTRUE : JJFALSE;
}

jjBoolean jjMultibootInfo::knowsModules(void) {
  return (flags & MODULES_FLAG) ? JJTRUE : JJFALSE;
}

jjBoolean jjMultibootInfo::knowsELFSectionHeader(void) {
  return (flags & ELF_SECTION_HEADER_FLAG) ? JJTRUE : JJFALSE;
}

jjBoolean jjMultibootInfo::knowsMemoryMap(void) {
  return (flags & MEMORY_MAP_FLAG) ? JJTRUE : JJFALSE;
}


jjMultibootModule * jjMultibootInfo::getModule(jju32 index) {

  /* Return the INDEXth module (first module = 0) */

  if (index < getModuleCount())
    return &mods_addr[index];
  else
    return NULL;

}


void jjMultibootInfo::dump(void) {

  /*
    Dump to screen for debugging purposes
  */

  kprintf("knowsMemory: %s\n",           
	  knowsMemory() ? "YES" : "NO");

  kprintf("knowsBootDevice: %s\n",       
	  knowsBootDevice() ? "YES" : "NO");

  kprintf("knowsCmdline: %s\n",          
	  knowsCmdline() ? "YES" : "NO");

  kprintf("knowsModules: %s\n",          
	  knowsModules() ? "YES" : "NO");

  kprintf("knowsELFSectionHeader: %s\n", 
	  knowsELFSectionHeader() ? "YES" : "NO");

  kprintf("knowsMemoryMap: %s\n", 
	  knowsMemoryMap() ? "YES" : "NO");

  if (knowsMemory()) {
    kprintf("Memory: %x lower, %x upper\n", getMemLower(), getMemUpper());
  }

  if (knowsCmdline()) {
    kprintf("Command line: %s\n", getCmdline());
  }

  if (knowsModules()) {
    for (jju32 i = 0; i < getModuleCount(); i++) {
      getModule(i)->dump();
    }
  }

}


/**********************************************************
 * jjMultibootModule
 **********************************************************/

void jjMultibootModule::dump(void) {

  kprintf( "Module: start %x, end %x: %s\n",
	   getStart(),
	   getEnd(),
	   getString());

}

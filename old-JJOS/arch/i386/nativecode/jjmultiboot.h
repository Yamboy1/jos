#ifndef JJMULTIBOOT_H
#define JJMULTIBOOT_H

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


#include "jjtypes.h"

/*
  Details for a multiboot module.
  (as loaded by GRUB's module= syntax)
 */

class jjMultibootModule {

 public:
  
    void * getStart(void) { return mod_start; };
    void * getEnd(void) { return mod_end; };
    char * getString(void) { return string; };

    void dump(void);
  
 private:
    void * mod_start;
    void * mod_end;
    
    char * string;
  
    jju32 reserved;

};


/*
  A multiboot info block, deposited by the bootloader at boot
  time.

  Access to all multiboot info options is not provided.
  Currently missing functionality to get:
  - detailed memory map
  - details on ELF headers
  However, jjos doesn't need them to boot.
*/

class jjMultibootInfo {
 public:
    
    static jjMultibootInfo * getMultibootInfo(void);
    
    jjBoolean knowsMemory(void);
    jjBoolean knowsBootDevice(void);
    jjBoolean knowsCmdline(void);
    jjBoolean knowsModules(void);
    jjBoolean knowsELFSectionHeader(void);
    jjBoolean knowsMemoryMap(void);

    jju32 getMemLower(void) { return mem_lower; };
    jju32 getMemUpper(void) { return mem_upper; };
    char * getCmdline(void) { return cmdline; };

    jju32 getModuleCount(void) { return mods_count; };
    jjMultibootModule * getModule(jju32 index);

    void dump(void);

 protected:
  
    /*
      DO NOT change or add to this list - the structure
      is determined by the multiboot spec.
    */
    
    jju32 flags;

    jju32 mem_lower;
    jju32 mem_upper;

    jju32 boot_device;

    char * cmdline;

    jju32 mods_count;
    jjMultibootModule * mods_addr;

};



#endif

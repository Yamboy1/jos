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

#ifdef TARGET_UNIX
#include <stdio.h>
#endif

#include "jbmachin.h"
#include "jbdebug.h"

static int debug = 0;

/* $$$$$$$$$$ Why does my copy of egcs-1.1.2 barf on these? */
/* $$$$$$$$$$ Gotta find out what the hell these are. */

#ifndef TARGET_UNIX
extern "C"
{
  void __pure_virtual(void)
  {
    kprintf("__pure_virtual called\n");
    jbmp->printBacktrace();
    jbmp->halt();
  }

  void __rtti_si(void)
  {
    kprintf("__rtti_si called\n");
    jbmp->printBacktrace();
    jbmp->halt();
  }

  void __rtti_user(void)
  {
    kprintf("__rtti_user called\n");
    jbmp->printBacktrace();
    jbmp->halt();
  }

  void __rtti_class(void)
  {
    kprintf("__rtti_class called\n");
    jbmp->printBacktrace();
    jbmp->halt();
  }
};
#endif

/* Should probably call static methods on jbMachine for efficiency. */

void *__builtin_new(int new_size)
{
  DEBUG(("__builtin_new(%d)\n", new_size));
  return(jbmp->myHeap->allocate(new_size));
}

/* This seems to just get called same as __builtin_new. */

void *__builtin_vec_new(int new_size)
{
  DEBUG(("__builtin_vec_new(%d)\n", new_size));
  return(__builtin_new(new_size));
}

void __builtin_delete(void *thing_to_delete)
{
  DEBUG(("__builtin_delete(0x%X)\n", thing_to_delete));
  jbmp->myHeap->deallocate(thing_to_delete);
}

/* This seems to just get called same as __builtin_delete. */

void __builtin_vec_delete(void *thing_to_delete)
{
  DEBUG(("__builtin_vec_delete(0x%X)\n", thing_to_delete));
  __builtin_delete(thing_to_delete);
}

#ifndef JVMBUILTINS_H
#define JVMBUILTINS_H

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
 * The Initial Developers of the Original Code are Todd Miller & John Morrison.
 * Portions created by John Morrison are Copyright (C) 1998.
 * All rights reserved.
 *
 * Contributor(s):
 *
 */

#include "d_main.h"

jjBoolean java_lang_VMClassLoader_defineClass(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean java_lang_VMClassLoader_resolveClass(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean java_lang_VMClassLoader_getPrimitiveClass(java_word argument[], frame *f, resolved_method *rm, int n_args);

#ifndef TARGET_UNIX
jjBoolean jos_system_machine_in16(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_in32(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_in8(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_out16(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_out32(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_out8(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_read16(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_read32(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_write8(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_write16(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_write32(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_system_machine_read8(java_word argument[], frame *f, resolved_method *rm, int n_args);

jjBoolean jos_platform_driver_consoleNative_setVisible(java_word argument[],  frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_consoleNative_putChar(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_consoleNative_setChar(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_consoleNative_getChar(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_consoleNative_setAttr(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_consoleNative_getAttr(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_consoleNative_setCursor(java_word argument[], frame *f, resolved_method *rm, int n_args);

#else

jjBoolean host_machine_in8(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean host_machine_out8(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean host_machine_read8(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean host_machine_write8(java_word argument[], frame *f, resolved_method *rm, int n_args);

jjBoolean jos_platform_driver_console_setCursor(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_console_putChar(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_console_setNativeAttr(java_word argument[], frame *f, resolved_method *rm, int n_args);
jjBoolean jos_platform_driver_console_setNativeChar(java_word argument[], frame *f, resolved_method *rm, int n_args);
#endif

#endif

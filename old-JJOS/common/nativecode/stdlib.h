#ifndef STDLIB_H
#define STDLIB_H

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

/*
 * I/O Stream stuff.
 */

#define JJEOF -1
#ifndef EOF
#define EOF JJEOF
#endif

/*
 * POSIX-ish stuff
 */

int strcmp(char *a, char *b);
size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strdup(const char *s);
const char *index(const char *s, int c);
const char *strpbrk(const char *s, const char *accept);
char *strtok(char *s, const char *delim);
void itoa(char buf[], unsigned long l);

#ifndef TARGET_UNIX
void abort(void);
int isnan(jjfloat32 f);
int isnan(jjfloat64 d);
jjfloat64 pow(jjfloat64 d, jjfloat64 f);
#else
extern "C" void abort(void);
#include <math.h>   /* for pow(), isnan() */
#endif

void jb_test_strings(void);	/* Obviously non-standard test routine. */

/*
 * Random utility functions
 */

void dumpmem(void *from, int nbytes);
int kprintf(const char *format, ...);

#endif

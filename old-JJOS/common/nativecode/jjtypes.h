#ifndef JJTYPES_H
#define JJTYPES_H

class java_word;

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

#ifndef NULL
#define NULL 0
#endif

/*
 * With any luck, this will work for x86 parts.
 */

typedef char jji8;
typedef unsigned char jju8;
typedef short jji16;
typedef unsigned short jju16;
typedef int jji32;
typedef unsigned int jju32;
typedef jju32 size_t;
typedef float jjfloat32;
typedef double jjfloat64;
struct jjlong { jju32 high; jju32 low; };

/* Might not want to use builtins as they call libg++ stuff. */
#undef HAS_LONG_LONGS
#ifdef HAS_LONG_LONGS
typedef unsigned long long jju64;
typedef signed long long jji65;
#else
class jju64
{
 public:

  jju64() {;};
  jju64(const jju64 &);
  jju64(const java_word &);
  jju64(const jju32 high32, jju32 low32);
  jju64(const jju32 value);
  jju64(const jjlong value );
  ~jju64() {;};

  jju64& operator = (const jju32 new_value);
  jju64& operator = (const jju64 &new_value);
  int operator == (const jju64 &other) const;
  jju64& operator += (const jju64 &);
  jju64& operator += (const jju32);
  jju64& operator -= (const jju64 &);
  jju64& operator &= (const jju64 &);
  jju64& operator |= (const jju64 &);
  jju64 operator % (const jju64 &);
  jju64 operator / (const jju64 &);
  jju64 operator * (const jju64 &);
  
  operator jju32 ();

  void shiftleft(void);
  void shiftright(void);
  jju64& operator <<= (const int n);
  jju64& operator >>= (const int n);

  friend jju64 operator ~(const jju64 &a);
  friend jju64 operator -(const jju64 &a);

  // private:

  jju32 myHigh32;
  jju32 myLow32;
};

/* $$$ $$$ $$$: get a /signed/ long long class */
typedef jju64 jji64;
#endif

typedef void *jjpaddr;
typedef void *jjvaddr;

typedef enum { JJFALSE=0, JJTRUE=1 } jjBoolean;

void print_out_sizeof_types(void);

#endif

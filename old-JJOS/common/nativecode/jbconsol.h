#ifndef jbConsole_h
#define jbConsole_h

#ifndef TARGET_UNIX
#include "jjtypes.h"
#endif

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

/*
 * Console abstraction
 */

class jbConsole
{
 public:

  virtual int  putChar(int c) = 0;
#ifndef TARGET_UNIX
  virtual void setVisible(jjBoolean v) = 0;
  virtual void setChar(int row, int col, jju8 c, jju8 a) = 0;
  virtual jju8 getChar(int row, int col) = 0;
#endif

  void printXs(void);
  void printString(char *);
  void printULong(unsigned long);
  void printVoid(void *);
};

#endif

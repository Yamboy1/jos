#ifndef JJCONSOLE_H
#define JJCONSOLE_H

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
#include "jbconsol.h"

#define JJSCREENWIDTH 80
#define JJSCREENHEIGHT 24

class jjConsole: public jbConsole
{
 public:

  jjConsole(jju32 VGAScreenBufferPointer);

  int putChar(int c);
  void clear_screen();

 private:
  jju8 *myVGAScreenBufferPointer;
  
  void scrollUpOneLine(void);
  jju8 getAttribute(int row, int col);
  void setVisible(jjBoolean v);
  jju8 getChar(int row, int col);
  void setChar(int row, int col, jju8 c, jju8 a);

  void setCursor(int row, int col);
  void updateCursor();

  jju8 * buffer;
  jjBoolean visible;
  int myRow;
  int myColumn;
};

/*
 * Programmer's Guide
 * Table 6.3, pg 178
 */

typedef enum
{
  VGA_MODE_7_NORMAL             = 0x07,
  VGA_MODE_7_INTENSE            = 0x0f,
  VGA_MODE_7_UNDERLINE          = 0x01,
  VGA_MODE_7_INTENSE_UNDERLINE  = 0x09,
  VGA_MODE_7_REVERSED           = 0x70,
  VGA_MODE_7_BLINKING_REVERSED  = 0xf0
} VgaMode7CharacterAttributes;

#endif

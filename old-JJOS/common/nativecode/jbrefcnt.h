#ifndef JBREFCNT_H
#define JBREFCNT_H

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

class jjRCOP;

/*
 * This is a Reference-Counted Object (RCO).
 * Inherit from this to get automagic storage reclamation.
 * Don't ever make one of these object except via operator new().
 */

class jjRCO
{
friend jjRCOP;
public:

  jjRCO();
  ~jjRCO();

  void dump(void);
  jju32 incrementReferenceCount();
  jju32 decrementReferenceCount();

protected:

  jju32 myReferenceCount;

private:
};

/*
 * This is a RCO Pointer (RCOP).
 * Use it just like a pointer (via operator->()).
 */

class jjRCOP
{
public:

  jjRCOP();
  jjRCOP(jjRCOP&);
  jjRCOP(jjRCO *);
  ~jjRCOP();

  void dump(void);
  jjRCOP& operator = (jjRCOP&);
  jjRCOP& operator = (jjRCO *);
  jjRCO *operator->();

  jjRCO *myObjectP;
};


void test_RCO(void);		/* Test routine. */

#endif

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

#include "jbdebug.h"
#include "jbmachin.h"

#include "jbrefcnt.h"

static int debug = 1;

/*
 * Referenced-Counted Object (RCO).
 */

jjRCO::jjRCO()
{
  myReferenceCount = 0;
}
  
jju32 jjRCO::incrementReferenceCount(void)
{
  return(++myReferenceCount);
}

jju32 jjRCO::decrementReferenceCount(void)
{
  if (myReferenceCount == 0)
    {
      kprintf("jjRCO::decrementReferenceCount -- ZERO reference count!\n");
      return(0);
    }
  else if (myReferenceCount == 1)
    {
      DEBUG(("jjRCO::decrementReferenceCount -- deleting\n"));
      myReferenceCount = 0;
      delete(this);
      return 0; // ? is this right?
    }
  else
    {
      DEBUG(("jjRCO::decrementReferenceCount -- not deleting (ref cnt = %d)\n",
	     (myReferenceCount-1)));
      return(--myReferenceCount);
    }
}


jjRCO::~jjRCO()
{
  if (myReferenceCount != 0)
    {
      kprintf("jjRCO::~jjRCO -- attempt to delete non-zero ref count %d\n", myReferenceCount);
      jbmp->halt();
    }
  else
    {
      ;
    }
}

void jjRCO::dump(void)
{
  kprintf("jjRCO::dump -- this = %X, myReferenceCount = %d\n", this, myReferenceCount);
}

/*
 * RCO Pointer type (RCOP).
 */

jjRCOP::jjRCOP()
{
  myObjectP = NULL;
}

jjRCOP::jjRCOP(jjRCOP &new_value)
{
  myObjectP = NULL;
  *this = new_value;
}

jjRCOP::jjRCOP(jjRCO *new_object)
{
  myObjectP = NULL;
  *this = new_object;
}

jjRCOP::~jjRCOP()
{
  if (myObjectP)
    {
      myObjectP->decrementReferenceCount();
    }
}

jjRCOP& jjRCOP::operator = (jjRCOP &new_value)
{
  /* Might need to put critical sections in here. */

  new_value.myObjectP->incrementReferenceCount();

  if (myObjectP)
    {
      myObjectP->decrementReferenceCount();
    }

  myObjectP = new_value.myObjectP;
  return *this; // TM -- hopefully right. (JM?)
}

jjRCOP& jjRCOP::operator = (jjRCO *new_object)
{
  /* Might need to put critical sections in here. */

  new_object->incrementReferenceCount();

  if (myObjectP)
    {
      myObjectP->decrementReferenceCount();
    }

  myObjectP = new_object;
  return(*this);
}

jjRCO *jjRCOP::operator -> ()
{
  return(myObjectP);
}

void jjRCOP::dump(void)
{
  kprintf("jjRCOP::dump -- this = %X, myObjectP = %X\n", this, myObjectP);
}

/*
 * Test scaffolding.
 */

void test_RCO(void)
{
  jjRCOP p0;
#ifdef NOTDEF
  p0.dump();
#endif

  jjRCO o0;
#ifdef NOTDEF
  o0.dump();
#endif

#ifdef NOTDEF
  kprintf("AFTER 1st ASSIGNMENT\n");
  
  p0 = &o0;
  p0.dump();
  o0.dump();

  kprintf("AFTER 2nd ASSIGNMENT\n");
  
  jjRCOP p1;
  p1 = p0;
  o0.dump();

  kprintf("AFTER 3rd ASSIGNMENT\n");
  
  jjRCOP p2;

  p2 = &o0;
  o0.dump();


  kprintf("AFTER 4th ASSIGNMENT\n");
  {
    jjRCOP p3;
    p3 = p2;
    o0.dump();
  }

  o0.dump();

  kprintf("UNWINDING\n");

  jjRCO o1;
  p2 = &o1;
  p1 = &o1;
  //  p0 = &o1;
#endif

  kprintf("dynamic test\n");
  
  jjRCOP p10(new jjRCO());

  p10.myObjectP->dump();


  jjRCOP p11;

  p11 = p10;

  p10.myObjectP->dump();

  jjRCOP p12(p11);

  p10.myObjectP->dump();

  {
    jjRCOP p13(p12);
    p10.myObjectP->dump();
  }

  p10.myObjectP->dump();

  kprintf("operator-> test\n");
  p10->dump();
}


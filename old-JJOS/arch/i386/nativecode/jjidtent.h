#ifndef JJIDTENT_H
#define JJIDTENT_H

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

class jjIDTEntry
{
public:
  void jumpToHere(void *);
  void dump(void);

  jju16 myOffset1500;
  jju16 myCodeSegmentSelector;
  jju8 myReserved;
  jju8 myPDPLSX110;
  jju16 myOffset3116;
};

class jjIDT
{
public:
  jjIDTEntry myEntry[256];
};

#endif

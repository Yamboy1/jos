#ifndef JJETHERBOOT_H
#define JJETHERBOOT_H

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

class jjEtherbootImageHeader
{
public:
  jjBoolean isValidHeader(void);
  char *commandLineArgs(void);
  void *startOfRamdisk(void);

  static void dealWithEtherboot(void);
  void processAllRecords(void);
  void dump(void);		/* Just dumps the header. */

 protected:

  jju32 lengthIn32BitWords(void);


  /*
   * Member variable memory layout.
   * DO NOT ADD NEW MEMBER VARIABLES OR CHANGE THEIR ORDER.
   */

  jju32 myInitialMagicNumber;
  jju32 myFlagsAndLength;

  /* Brain-dead Little Endian order. */
  jju16 myLocationAddressOffset;
  jju16 myLocationAddressSegment;

  /* Brain-dead Little Endian order. */
  jju16 myExecuteAddressOffset;
  jju16 myExecuteAddressSegment;

  char *myExtraVendorInformation[0];
};

class jjEtherbootImageRecord
{
public:
  jjEtherbootImageRecord *next(void);

  void dump(void);
  jjBoolean isLastRecord(void);
  jju8 bits24And25(void);
  jju8 vendorTags(void);
  void *loadAddress(void);
  
  jju32 myFlagsTagsAndLengths;
  jju32 myLoadAddress;
  jju32 myImageLength;
  jju32 myMemoryLength;

};

extern void *jjEtherbootCommandLineArgsLocation;
extern void *jjEtherbootRamDiskLocation;
extern char *jjEtherbootCommandLineString;

/*
 * Extremely Primitive IP Address abstraction.
 */

class jjIPAddress
{
public:

  void set(jju8 newb3, jju8 newb2, jju8 newb1, jju8 newb0);
  void dump(void);

  /* Stored in network, big-endian (i.e., correct) byte order. */
  jju8 b3;
  jju8 b2;
  jju8 b1;
  jju8 b0;
};

/*
 * Bootstrap Protocol (BOOTP) reply, from RFC951.
 */

class jjBootPReply
{
public:

  void dump(void);
  void process(void);
  
#define JJBOOTPEQUEST 1
#define JJBOOTREPLY 2
  jju8 myPacketOpcode;
#define JJ10MBETHERNET 1
  jju8 myHardwareAddressType;
  jju8 myHardwareAddressLength;
  jju8 myHops;			// Client sets to zero, optionally used by gways.
  jju32 myTransactionID;	// NETWORK ORDER.
  jju16 mySecondsElapsed;	// since trying to boot.
  jju16 myUnused;
  jjIPAddress myClientIPAddress; // Filled in if known.
  jjIPAddress myYourClientIPAddress; // Filled in by server.
  jjIPAddress myServerIPAddress;
  jjIPAddress myGatewayIPAddress; // Used in optional cross-gateway booting.
  jju8 myClientHardwareAddress[16];
  jju8 myServerHostName[64];	// Null-terminated string.
  jju8 myBootFileName[128];	// Null-terminated string.
  jju8 myVendorSpecificArea[64]; // Optional.
};




#endif

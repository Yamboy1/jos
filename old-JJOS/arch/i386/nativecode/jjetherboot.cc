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

#include "stdlib.h"

#include "jjmachin.h"
#include "jjetherboot.h"

#include "jbdebug.h"

static int debug = 0;

void *jjEtherbootCommandLineArgsLocation = NULL;
void *jjEtherbootRamDiskLocation = NULL;
char *jjEtherbootCommandLineString = NULL;

/*
 * Try and locate the Etherboot-specific bootp reply
 * and boot image header (so we can find the command
 * line arguments and the ramdisk).
 */

void jjEtherbootImageHeader::dealWithEtherboot(void)
{
  extern unsigned long boot_image_header_far_pointer; // Filled by head.s86.

  DEBUG(("jjEtherbootImageHeader::dealWithEtherboot -- boot_image_header_far_pointer = %X\n",
	 boot_image_header_far_pointer));

  void *imagep = jjMachine::convertFarPointerToLinearAddress(boot_image_header_far_pointer);

  jjEtherbootImageHeader *image_headerp = (jjEtherbootImageHeader *)imagep;

  image_headerp->processAllRecords();

  /* Deal with bootp reply (who am I)? */

  {
    extern unsigned long bootp_reply_far_pointer;	// Filled by head.s86.
  
    DEBUG(("jjEtherbootImageHeader::dealWithEtherboot -- bootp_reply_far_pointer = %X\n",
	   bootp_reply_far_pointer));

    void *replyp = jjMachine::convertFarPointerToLinearAddress(bootp_reply_far_pointer);
    jju8 *charreplyp = (jju8 *)replyp;
    jjBootPReply *bootpreplyp = (jjBootPReply *)(charreplyp);

    if (debug) { bootpreplyp->dump(); }

    bootpreplyp->process();
  }
}
  

/*
 * Etherboot image header (the 1st record).
 */

void jjEtherbootImageHeader::dump(void)
{
  kprintf("jjEtherbootImageHeader::dump --\n");
  kprintf("  magic number = %X\n", myInitialMagicNumber);
  kprintf("  flags and length = %X\n", myFlagsAndLength);
  kprintf("  location address = %x::%x\n",
	  myLocationAddressSegment, myLocationAddressOffset);
  kprintf("  execute address = %x::%x\n",
	  myExecuteAddressSegment, myExecuteAddressOffset);

  int num_extra_longwords = (myFlagsAndLength >> 4) & 0xf;

  kprintf("  vendor \"");
  for (int i=0; i<(num_extra_longwords*4); i++)
    {
      int c = (int)myExtraVendorInformation[i];
      jbmp->myConsole->putChar(c);
    }
  kprintf("\"\n");
}  
  
void jjEtherbootImageHeader::processAllRecords(void)
{
  if (!isValidHeader())
    {
      kprintf("jjEtherbootImageHeader::processAllRecords -- Failed to find Etherboot magic number!\n");
      jbmp->halt();
    }
  else
    {
      DEBUG(("jjEtherbootImageHeader::processAllRecords -- Found magic number!\n"));
    }

  if (debug) { dump(); }

  jju32 *p = (jju32 *)this;

  p += lengthIn32BitWords();

  jjEtherbootImageRecord *recordp = (jjEtherbootImageRecord *)p;

  while (recordp)
    {
      if (debug)
	{
	  kprintf("recordp = %X\n", recordp);
	  recordp->dump();
	}

      switch (recordp->vendorTags())
	{
	case 0x10:
	  DEBUG(("KERNEL IMAGE\n"));
	  break;
	case 0x11:
	  {
	    jjEtherbootCommandLineArgsLocation = recordp->loadAddress();
	    jjEtherbootCommandLineString = (char *)jjEtherbootCommandLineArgsLocation;

	    DEBUG(("COMMAND LINE @ 0x%X\n", jjEtherbootCommandLineArgsLocation));
	    DEBUG(("STRING IS \"%s\"\n", jjEtherbootCommandLineString));

#ifdef NOTDEF
	    jju8 *foo = (jju8 *)jjEtherbootCommandLineArgsLocation;
	    for (int i=0; i<512; i++)
	      {
		jju8 c = foo[i];
		kprintf("[%c]", (jju32)c);
	      }
#endif
	  }
	  break;
	case 0x12:
	  jjEtherbootRamDiskLocation = recordp->loadAddress();
	  DEBUG(("RAMDISK @ 0x%X\n", jjEtherbootRamDiskLocation));
	  break;
	default:
	  kprintf("jjEtherbootImageRecord::processAllRecords -- unknown vendorTags %d\n",
		  recordp->vendorTags());
	  break;	
	}

      if (recordp->isLastRecord())
	{
	  return;
	}
      else
	{
	  recordp = recordp->next();
	}
    }
}

jjBoolean jjEtherbootImageHeader::isValidHeader(void)
{
  if (myInitialMagicNumber == 0x1B031336)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

jju32 jjEtherbootImageHeader::lengthIn32BitWords(void)
{
  int num_extra_longwords = (myFlagsAndLength >> 4) & 0xf;
  return(4+num_extra_longwords);
}

char *jjEtherbootImageHeader::commandLineArgs(void)
{
  return("THIS IS A TEST OF jjEtherbootImageHeader::commandLineArgs\n");
}

void *jjEtherbootImageHeader::startOfRamdisk(void)
{
  return(NULL);
}

/*
 * Subsequent records.
 */

void jjEtherbootImageRecord::dump(void)
{
  kprintf("  myFlagsTagsAndLengths = %X\n", myFlagsTagsAndLengths);
  kprintf(" bits 24 & 25 = %d\n", bits24And25());
  kprintf(" vendor tags %X\n", vendorTags());

  kprintf("  myLoadAddress = %X\n", myLoadAddress);
  kprintf("  myImageLength = %X", myImageLength);
  kprintf("  myMemoryLength = %X\n", myMemoryLength);

}

jjBoolean jjEtherbootImageRecord::isLastRecord(void)
{
  if (myFlagsTagsAndLengths & 0x04000000) // Bit 26 indicates last record.
    {
      DEBUG(("jjEtherbootImageRecord::isLastRecord -- TRUE\n"));
      return(JJTRUE);
    }
  else
    {
      DEBUG(("jjEtherbootImageRecord::isLastRecord -- FALSE\n"));
      return(JJFALSE);
    }
}
  
jju8 jjEtherbootImageRecord::vendorTags(void)
{
  jju8 vendor_tags = (myFlagsTagsAndLengths >> 8) & 0xff; // Bits 8 to 15. 
  return(vendor_tags);
}

jju8 jjEtherbootImageRecord::bits24And25(void)
{
  jju8 bits_24_and_25 = (myFlagsTagsAndLengths >> 24) & 0x3;
  DEBUG(("jjEtherbootImageRecord::bits24and25 -- "));
  DEBUG(("myFlagsTagsAndLengths %X, bits24And25 = %x\n",
	 myFlagsTagsAndLengths, bits_24_and_25));
	 
  return(bits_24_and_25);
}

void *jjEtherbootImageRecord::loadAddress(void)
{
  /* $$$ Don't handle some cases properly. */

  switch(bits24And25())
    {
    case 0:
      return((void *)myLoadAddress);
    case 2:
      {
	DEBUG(("jjEtherbootImageRecord::loadAddress -- "));
	DEBUG(("myPhysicalMemoryUpperBound = %X, myLoadAddress = %X\n",
	       jbmp->myPhysicalMemoryUpperBound,
	       myLoadAddress));
	jju8 *retval = (jju8 *)jbmp->myPhysicalMemoryUpperBound;
	retval -= myLoadAddress;
	DEBUG(("  returning %X\n", retval));
	return(retval);
      }
    default:
      kprintf("jjEtherbootImageRecord::loadAddress -- case %d\n", bits24And25());
      jbmp->halt();
      return(NULL);
    }
}

jjEtherbootImageRecord *jjEtherbootImageRecord::next(void)
{
  if (isLastRecord())
    {
      kprintf("jjEtherbootImageRecord::next() -- NO NEXT\n");
      return(NULL);
    }
  else
    {
      jju8 *foo = (jju8 *)this;
      foo += sizeof(*this);
      return((jjEtherbootImageRecord *)foo);
    }
}

/*
 * Extremely Primitive IP Address abstraction.
 */

void jjIPAddress::set(jju8 newb3, jju8 newb2, jju8 newb1, jju8 newb0)
{
  b3 = newb3;
  b2 = newb2;
  b1 = newb1;
  b0 = newb0;
}

void jjIPAddress::dump(void)
{
  kprintf("[%d.%d.%d.%d]", b3, b2, b1, b0);
}

/*
 * Bootstrap Protocol (BOOTP) reply, from RFC951.
 */

void jjBootPReply::dump(void)
{
  kprintf("jjBootPReply::dump -- ");
  kprintf("  myPacketOpcode=%d\n", myPacketOpcode);
  kprintf("  myHardwareAddressType=%d\n", myHardwareAddressType);
  kprintf("  myHardwareAddressLength=%d\n", myHardwareAddressLength);
  kprintf("  myHops=%d\n", myHops);
  kprintf("  myTransactionID=0x%X\n", myTransactionID);
  kprintf("  mySecondsElapsed=%d\n", mySecondsElapsed);
  kprintf("  myUnused=0x%x\n", myUnused);

  kprintf("  myClientIPAddress=");
  myClientIPAddress.dump();
  kprintf("\n");

  kprintf("  myYourClientIPAddress=");
  myYourClientIPAddress.dump();
  kprintf("\n");

  kprintf("  myServerIPAddress=");
  myServerIPAddress.dump();
  kprintf("\n");

  kprintf("  myGatewayIPAddress=");
  myGatewayIPAddress.dump();
  kprintf("\n");

  kprintf("  myClientHardwareAddress= ");
  for (int i=0; i<myHardwareAddressLength; i++)
    {
      kprintf("%x.", myClientHardwareAddress[i]);
    }
  kprintf("\n");

  kprintf("  myServerHostName=\"%s\"\n", myServerHostName);
  kprintf("  myBootFileName=\"%s\"\n", myBootFileName);

  kprintf("  myVendorSpecificArea=");
  for (unsigned int i=0; i<sizeof(myVendorSpecificArea); i++)
    {
#ifdef NOTDEF
      kprintf("%x.", myVendorSpecificArea[i]);
#else
      kprintf("%c", myVendorSpecificArea[i]);
#endif
    }
  kprintf("\n");
}

void jjBootPReply::process(void)
{
  if ( debug ) {
  	kprintf("jjBootPReply::process NOP -- ");
  	kprintf(" booting %s:%s\n", myServerHostName, myBootFileName);
  	kprintf("  my IP Address = ");
  	myYourClientIPAddress.dump();
 	kprintf("\n");
 	}
}

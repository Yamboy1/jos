#include "stdlib.h"
#include "jjidtent.h"

/*
 * Protected mode interrupt descriptor table entry.
 * These are the simplest-possible interrupt gate entries.
 */

void jjIDTEntry::dump(void)
{
  kprintf("myOffset3116=%x\n", myOffset3116);
  kprintf("myPDPLSX110=%x", myPDPLSX110);
  kprintf("myReserved=%x", myReserved);
  kprintf("myCodeSegmentSelector=%x", myCodeSegmentSelector);
  kprintf("myOffset1500=%x", myOffset1500);
}

void jjIDTEntry::jumpToHere(void *interrupt_handler)
{
  jju32 ihaddress = (jju32)interrupt_handler;
  myOffset3116 = (ihaddress >> 16) & 0xffff;
  myOffset1500 = (ihaddress & 0xffff);

  /* P=1, DPL=0, S=0 (int gate desc), X=1 (32-bit) */
  myPDPLSX110 = 0x8e;
  myReserved = 0x00;

  myCodeSegmentSelector = 0x8;	// $$$$$$$$$$ This segment selector should be gotten from asm

#ifdef NOTDEF
  dump();
#endif
}



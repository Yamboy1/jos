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
#include "jbheap.h"
#include "jjmachin.h"

#include "jbdebug.h"
static int debug = 0;

/*
 * Uncomment in order to use conservative GC/heap implementation.
 */

#define USE_CONSERVATIVE_GC 1

/*
 * Uncomment these to use simple malloc-free-style allocator instead.
 * This thing is incredibly S-L-O-W (but it can help spot memory bugs)!
 * -jm
 */

/* #define USE_SIMPLE_SCHEME  1 */
#define TRY_EXACT_FIT_FIRST
#undef CHECK_SENTINEL_BITS

/*
 * Uncomment to use NULL (throwaway) allocator.
 */

/* #define USE_NULL_ALLOCATOR 1 */



#if USE_SIMPLE_SCHEME

class memory_block
{
friend jbHeap;
public:

  void                *startOfData(void);
  static memory_block *toMemoryBlock(void *);
  void                setIsFree(void);
  
  static  jju32       initBlockZero(void *start_inclusive, void *end_exclusive);
  static memory_block *allocate(jju32 size);
  static jjBoolean    setMemoryFree(void *);
  static void         dumpBlocks(void);
  void                dump(void);

protected:

  jju32               setDataSize(jju32 size);
  jju32              *sentinelLocation(void);
  jjBoolean           sentinelIsOk(void);
  static jjBoolean    sentinelsAreAllOK(void);
  void                setIsNotFree(void);
  jjBoolean           breakBlock(jju32 size);
  void                zeroBlock(void);
  jjBoolean           blockIsSane(void);

  static memory_block *findFreeExactFit(jju32 size);
  static memory_block *findFirstBigEnough(jju32 size);

  /* Member variables. (Don't forget sentinel jju32 at end.) */
  class memory_block *myNext;
  jjBoolean           myIsFree;
  jju32               myDataSize;

  /* Static global list of ALL blocks -- both in-use and free. */
  static memory_block *theList;
};

memory_block *memory_block::theList = NULL;

void *memory_block::startOfData(void)
{
  jju8 *foo = (jju8 *)this;
  foo += sizeof(memory_block);
  return((void *)foo);
}

memory_block *memory_block::toMemoryBlock(void *p)
{
  jju8 *foo = (jju8 *)p;
  foo -= sizeof(memory_block);
  return((memory_block *)foo);
}

void memory_block::dump(void)
{
  kprintf("block %X: next %X, isFree %d, size %d, sentinel %X\n",
	  this, myNext, myIsFree, myDataSize, *sentinelLocation());
}

void memory_block::dumpBlocks(void)
{
  memory_block *tmp = theList;

  while (tmp)
    {
      tmp->dump();
      tmp = tmp->myNext;
    }
}

void memory_block::zeroBlock(void)
{
  jju8 *data = (jju8 *)startOfData();

  for (jju32 i=0; i<myDataSize; i++)
    {
      data[i] = 0;
    }
}

void memory_block::setIsFree(void)
{
  if (myIsFree)
    {
      kprintf("memory_block::setIsFree -- already free!\n");
      abort();
    }

  myIsFree = JJTRUE;
}

void memory_block::setIsNotFree(void)
{
  myIsFree = JJFALSE;
}

jju32 memory_block::setDataSize(jju32 size)
{
  myDataSize = size;
  return(myDataSize);
}

jjBoolean memory_block::sentinelIsOk(void)
{
  jju32 *sentinelp = sentinelLocation();

  if (*sentinelp == 0xDEADC0DE)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

jju32 *memory_block::sentinelLocation(void)
{
  jju8 *tmp = (jju8 *)this	// My start
    + sizeof(memory_block)	// skipping header
    + myDataSize;		// and data.

  jju32 *sentinelp = (jju32 *)tmp;
  return(sentinelp);
}
    
jjBoolean memory_block::sentinelsAreAllOK(void)
{
  register memory_block *tmp = theList;

  while (tmp)
    {
      if (!(tmp->sentinelIsOk()))
	{
	  kprintf("memory_block::sentinelsAreAllOK() -- block 0x%X corrupted\n", tmp);
	  return(JJFALSE);
	}
      else
	{
	  tmp = tmp->myNext;
	}
    }

  return(JJTRUE);
}

memory_block *memory_block::findFreeExactFit(register jju32 size)
{
  register memory_block *tmp = theList;

  while (tmp)
    {
      if (tmp->myIsFree && (tmp->myDataSize == size))
	{
	  return(tmp);
	}
      else if (tmp->myNext == NULL)
	{
	  DEBUG(("memory_block::findFreeExactFit -- out of blocks\n"));
	  return(NULL);
	}
      else
	{
	  tmp = tmp->myNext;
	}
    }

  return(NULL);
}
      
memory_block *memory_block::findFirstBigEnough(register jju32 size)
{
  register memory_block *tmp = theList;

  while (tmp)
    {
      if (tmp->myIsFree && (tmp->myDataSize >= size))
	{
	  return(tmp);
	}
      else if (tmp->myNext == NULL)
	{
	  kprintf("memory_block::findFirstBigEnough -- out of blocks\n");
	  return(NULL);
	}
      else
	{
	  tmp = tmp->myNext;
	}
    }

  return(NULL);
}


jju32 memory_block::initBlockZero(void *start_inclusive, void *end_exclusive)
{
  kprintf("Using USE_SIMPLE_SCHEME\n");

  /* Note we don't even try to get alignments right. */

  theList = (memory_block *)start_inclusive;

  theList->myNext = NULL;
  theList->myIsFree = JJTRUE;

  jju32 total_size = (jju8 *)end_exclusive - (jju8 *)start_inclusive;
  jju32 data_size = total_size
    - sizeof(memory_block)	// Less header size and
    - sizeof(jju32);		// less sentinel word size.
  theList->setDataSize(data_size);

  jju32 *block0sentinelp = theList->sentinelLocation();
  *block0sentinelp = 0xDEADC0DE;

  if (debug) theList->dump();

  return(theList->myDataSize);
}

jjBoolean memory_block::breakBlock(jju32 size)
{
  if (myDataSize < size)
    {
      kprintf("memory_block::breakBlock(%d) -- I'm too small (%d)\n", size, myDataSize);
      return(JJFALSE);
    }
  else if (myDataSize == size)
    {
      DEBUG(("memory_block::breakBlock(%d) -- didn't need to break block\n", size));
      return(JJFALSE);
    }
  else if (myDataSize < (size + sizeof(memory_block) + sizeof(jju32)))
    {
      DEBUG(("memory_block::breakBlock(%d) -- too small to include header\n", size));
      return(JJFALSE);
    }
  else
    {
      if (debug)
	{
	  kprintf("Breaking - "); dump(); if (!sentinelIsOk()) abort();
	}

      /* Figure out start of next block using byte arithmetic. */

      jju8 *start_next_block = (jju8 *)this;
      start_next_block +=
	sizeof(memory_block)	// Header
	+ sizeof(jju32)		// Sentinel
	+ size;

      /* Init the next block. */

      memory_block *next = (memory_block *)start_next_block;
      next->myNext = myNext;
      next->myIsFree = JJTRUE;
      next->myDataSize = myDataSize
	- size			// Less data for block 1
	- sizeof(jju32)		// Less new sentinel for block 1
	- sizeof(memory_block);	// Less new block 2 header

      /* Point to next block, change my size, and update sentinel. */

      myNext = next;
      setDataSize(size);
      jju32 *sentinelp = sentinelLocation();
      *sentinelp = 0xDEADC0DE;

      if (debug)
	{
	  kprintf("Blk #1 - "); dump(); if (!sentinelIsOk()) abort();
	  kprintf("Blk #2 - "); next->dump(); if (!next->sentinelIsOk()) abort();
	}

      return(JJTRUE);
    }
}

jjBoolean memory_block::setMemoryFree(void *p)
{
  memory_block *mb = toMemoryBlock(p);

  if (!mb->sentinelIsOk())
    {
      kprintf("memory_block:;setMemoryFree(%X) -- bad sentinel\n", p);
      return(JJFALSE);
    }
  else
    {
      mb->setIsFree();
      return(JJTRUE);
    }
}

jjBoolean memory_block::blockIsSane(void)
{
  if (!sentinelIsOk())
    {
      kprintf("sentinel corruption!\n");
      abort();
      return(JJFALSE);
    }
  else if (!jbmp->myHeap->locationWithinHeap(this))
    {
      kprintf("memory block start out of heap!\n");
      abort();
      return(JJFALSE);
    }
  else if (!jbmp->myHeap->locationWithinHeap(sentinelLocation()))
    {
      kprintf("memory block sentinel out of heap!\n");
      abort();
      return(JJFALSE);
    }
  else
    {
      return(JJTRUE);
    }
}
  

memory_block *memory_block::allocate(jju32 size)
{
  DEBUG(("allocate(%d)...", size));

  memory_block *retval = NULL;

  if (size == 0)
    {
      kprintf("SIZE HACK\n");
      size += 8;
    }

  /* Allocate the block, either an exact or big-enough fit. */

#ifdef TRY_EXACT_FIT_FIRST
  if (retval = findFreeExactFit(size))
    {
      DEBUG(("memory_block::allocate(%d) -- got exact fit\n"));
    }
  else
#endif
    if (retval = findFirstBigEnough(size))
      {
	DEBUG(("memory_block::allocate(%d) -- got big enough fit\n"));
	retval->breakBlock(size);
      }
  else
    {
      kprintf("memory_block::allocate(%d) -- out of blocks!\n", size);
      return(NULL);
    }

  /* Sanity-check the block. */

  if (!retval->blockIsSane())
    {
      return(NULL);
    }
  else
    {
      retval->setIsNotFree();
      retval->zeroBlock();
      if (debug) retval->dump();
      return(retval);
    }
}  

#endif

  


/*
 * This is the simplest possible allocator.
 * It doesn't even deallocate.
 */

jju32 jbHeap::usedMemory(void)
{
#if USE_NULL_ALLOCATOR == 1

  return ((jju32)myNextToAllocate - (jju32)myLowMemAddressInclusive);

#elif USE_SIMPLE_SCHEME == 1

  jju32 used_bytes = 0;
  jju32 free_bytes = 0;
  memory_block *tmp = memory_block::theList;

  while (tmp)
    {
      if (tmp->myIsFree)
	{
	  free_bytes += tmp->myDataSize;
	}
      else
	{
	  used_bytes += tmp->myDataSize;
	}

      tmp = tmp->myNext;
    }
  
  DEBUG(("jbHeap::usedMemory() -- %d used, %d free\n", used_bytes, free_bytes));
  return(used_bytes);

#elif USE_CONSERVATIVE_GC == 1

  kprintf("jbHeap::usedMemory -- don't know the answer\n");
  return(0);

#endif
}

#ifdef USE_CONSERVATIVE_GC

#include "gc/heap.h"

/* _Quinn: for the conservative garbage collector. */

void *jbHeap::theTopOfTheSingleStack = NULL;

static void panic(char *s)
{
#ifndef TARGET_UNIX
  /*
  ((jjMachine *)jbmp)->out8(0x3ce, 0); // Select Set/Reset Register (idx 0) via Graphics Address Reg (pg 375)
  ((jjMachine *)jbmp)->out8(0x3cf, 0); // Reset via Set/Reset Register (pg 376)
  */

  ((jjMachine *)jbmp)->out8(0x3ce, 0x6); // Select Misc Register (idx 6) via Graphics Address Reg (pg 374)
  ((jjMachine *)jbmp)->out8(0x3cf, 0x0); // Request text mode from Misc Register (pg 389)
#endif
  kprintf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  kprintf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  kprintf("%s\n");
  kprintf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  kprintf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
  jbmp->halt();
}
  

static void getRoots()
{
  /* First, mark the stack. */

#ifdef NOTDEF
  panic("GC start!");
#endif

  int this_stack_frame;
  void *approx_bottom_of_stack = &this_stack_frame;
    
  kprintf("bottom %X, top %X, diff %d\n",
	  approx_bottom_of_stack,
	  jbHeap::theTopOfTheSingleStack,
	  (int)jbHeap::theTopOfTheSingleStack - (int)approx_bottom_of_stack);
  heapMarkBlock(approx_bottom_of_stack, jbHeap::theTopOfTheSingleStack);

#ifdef OLD_DECAF
  /* Next, mark the single global variable for the JVM. */

  extern class decafJVM *jvm;
  heapMark(jvm);
#else
  extern class JVM * rootJVM;
  heapMark( rootJVM );
#endif

  return;
}


static struct heapCallbacks cb = {&getRoots};
#endif


jju32 jbHeap::setPhysicalLowAndHigh(void *low_mem_address_inclusive,
				    void *high_mem_address_exclusive)
{
  DEBUG(("jbHeap::setPhysicalLowAndHigh(%X, %X)\n",
	 low_mem_address_inclusive,
	 high_mem_address_exclusive));

  myLowMemAddressInclusive = (jju8 *)low_mem_address_inclusive;
  myHighMemAddressExclusive = (jju8 *)high_mem_address_exclusive;

  myNextToAllocate = myLowMemAddressInclusive;
  
  DEBUG(("jbHeap::setPhysicalLowAndHigh -- scrubbing memory..."));
  scrubMemory();
  DEBUG(("Done!\n"));


#if USE_NULL_ALLOCATOR == 1

  ;

#elif USE_SIMPLE_SCHEME == 1

  memory_block::initBlockZero(myLowMemAddressInclusive, myHighMemAddressExclusive);

#elif USE_CONSERVATIVE_GC == 1

  int freelistsizes[] = {8, 12, 16, 20, 24, 28, 32, 36, 40, 100, MAX_FREELIST_OBJ_SIZE, 0};

  heapInit( myLowMemAddressInclusive, 
	    myHighMemAddressExclusive - myLowMemAddressInclusive,
	    freelistsizes,
	    cb);

#else

  syntax_error;

#endif

  return(maxSize());
}




jju32 jbHeap::maxSize(void)
{
  jju32 size_of_heap = myHighMemAddressExclusive - myLowMemAddressInclusive;

  return(size_of_heap);
}
  

void jbHeap::scrubMemory(void)
{
  jju8 *p = myLowMemAddressInclusive;

  while (p < myHighMemAddressExclusive)
    {
      *p++ = 0;
    }
}


#include "jbmachin.h"		// To get printBacktrace and halt
void *jbHeap::allocate(jju32 size)
{
#if USE_NULL_ALLOCATOR == 1

   if ((myNextToAllocate + size) >= myHighMemAddressExclusive)
     {
       kprintf("jbHeap::allocate -- out of memory, %X >= %X\n",
	       (myNextToAllocate + size),
	       myHighMemAddressExclusive);
       abort();
       return(NULL);
     }
   else
     {
       void *retval = (void *)myNextToAllocate;
       myNextToAllocate += size;
       return(retval);
     }

#elif USE_SIMPLE_SCHEME == 1

  memory_block *mb = memory_block::allocate(size);

  if (!mb)
    {
      kprintf("jbHeap::allocate -- out of memory allocating %d bytes\n", size);
      abort();
      return(NULL);
    }
  else
    {
      void *retval = mb->startOfData();
      DEBUG(("jbHeap::allocate %X for size %d\n", retval, size));  
      return(retval);
    }

#elif USE_CONSERVATIVE_GC == 1

  void * res = heapAlloc(size);

  if (!res)
    {
      kprintf("jbHeap::allocate -- out of memory allocating %d bytes\n", size);
      abort();
      return(NULL);
    }
  else
    {
      DEBUG(("jbHeap::allocate %X for size %d\n", res, size));  
      return res;
    }

#endif

}


void jbHeap::deallocate(void *thing_to_deallocate)
{
#ifdef PARANOID
  if (thing_to_deallocate < (void *)myLowMemAddressInclusive)
    {
      kprintf("jbHeap::deallocate -- address too low (%X)\n", thing_to_deallocate);
    }
  else if (thing_to_deallocate >= (void *)myHighMemAddressExclusive)
    {
      kprintf("jbHeap::deallocate -- address too high (%X)\n", thing_to_deallocate);
    }
  else
#endif
    {

#if USE_NULL_ALLOCATOR == 1

      ;

#elif USE_SIMPLE_SCHEME == 1

      memory_block::setMemoryFree(thing_to_deallocate);

#elif USE_CONSERVATIVE_GC == 1

      ;

#else

      syntax_error;

#endif
    }
}

jjBoolean jbHeap::locationWithinHeap(void *p)
{
  if (p < myLowMemAddressInclusive)
    {
      return(JJFALSE);
    }
  else if (p >= myHighMemAddressExclusive)
    {
      return(JJFALSE);
    }
  else
    {
      return(JJTRUE);
    }
}

/*
 * Testing stub.
 */

static jju8 *will_not_be_scanned = NULL;

void jbHeap::testHeap(void)
{
#ifdef NOTDEF
  for (unsigned int i=0; i<=(maxSize()/1024); i++)
    {
      void *foo = allocate(1024);
      kprintf("%d = %X\n", i, foo);

      /* Should probably deallocate the thing. */
    }
#else

  will_not_be_scanned = (jju8 *)allocate(20);
  will_not_be_scanned[0] = 0xff;

  while (1)
    {
      for (int i=0; i<4096; i++)
	{
	  jju8 *bar = new jju8[i];
	  jju8 **foo = new jju8*[1];
	  foo[0] = bar;

	  if (!(*will_not_be_scanned))
	    {
	      abort();
	    }

	  if (!foo)
	    {
	      abort();
	    }
	  else
	    {
	      for (int j=0; j<i; j++)
		{
		  foo[j] = (jju8 *)0xff;
		}
	    }
	}
    }

#endif
}

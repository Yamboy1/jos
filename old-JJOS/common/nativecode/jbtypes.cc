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
#include "stdlib.h"

jju64::jju64(const jju64 &x)
{
  myHigh32 = x.myHigh32;
  myLow32 = x.myLow32;
}

/*
jju64::jju64(const java_word &x)
{
 *this = (jlong) x.operator jint();
}
*/
jju64::jju64(const jju32 high32, const jju32 low32)
{
  myHigh32 = high32;
  myLow32 = low32;
}

jju64::jju64(const jju32 value)
{
  myHigh32 = 0;
  myLow32 = value;
}

jju64::jju64(const jjlong value)
{
  myHigh32 = value.high;
  myLow32 = value.low;
}

jju64 & jju64::operator = (const jju32 new_value)
{
  myHigh32 = 0;
  myLow32 = new_value;
  return(*this);
}

int jju64::operator == (const jju64 &other) const
{
  return((myHigh32 == other.myHigh32) && (myLow32 == other.myLow32));
}


/*
 * from Embedded Systems Magazine, Jack Crenshaw.
 * From ftp://ftp.mfi.com/pub/espmag/1997/crenshaw1.txt
 */

jju64 & jju64::operator += (const jju64 &x)
{
   jju32 temp, accum;
   jju16 carry;

   // save high half of low word
   temp = myLow32 >> 16;

   // add low halves of low words
   accum = (myLow32 & 0xffff) + (x.myLow32 & 0xffff);

   // put back the result
   myLow32 = accum & 0xffff;

   // create carry bit
   carry = ((accum & 0x10000L)!=0);

   //  repeat for high half of low word
   accum = temp + (x.myLow32 >> 16) + carry;
   myLow32 |= accum << 16;
   carry = ((accum & 0x10000L)!=0);

   // and low half of high word
   temp = myHigh32 >> 16;
   accum = (myHigh32 & 0xffff) + (x.myHigh32 & 0xffff)
    + carry;
   myHigh32 = accum & 0xffff;
   carry = ((accum & 0x10000L)!=0);

   // high half of high word
   accum = temp + (x.myHigh32 >> 16) + carry;
   myHigh32 |= accum << 16;
   return *this;
}

jju64 & jju64::operator -= (const jju64 &x)
{
   jju32 temp, accum;
   short borrow;

   // save high half of low word
   temp = myLow32 >> 16;

   // subtract low halves of low words
   accum = (myLow32 & 0xffff) - (x.myLow32 & 0xffff);

   // put back the result
   myLow32 = accum & 0xffff;

   // create ÒborrowÓ flag
   borrow = ((accum & 0x10000L)!=0);

   // repeat for high half of low word
   accum = temp - borrow - (x.myLow32 >> 16);
   myLow32 |= accum << 16;
   borrow = ((accum & 0x10000L)!=0);

   // low half of high word
   temp = myHigh32 >> 16;
   accum = (myHigh32 & 0xffff) - (x.myHigh32 & 0xffff)
    - borrow;
   myHigh32 = accum & 0xffff;
   borrow = ((accum & 0x10000L)!=0);

   // high half of high word
   accum = temp - (x.myHigh32 >> 16) - borrow;
   myHigh32 |= accum << 16;
   return *this;
}


jju64 & jju64::operator &= (const jju64 &x)
{
   myLow32 &= x.myLow32;
   myHigh32 &= x.myHigh32;
   return *this;
}

jju64 & jju64::operator |= (const jju64 &x)
{
   myLow32 |= x.myLow32;
   myHigh32 |= x.myHigh32;
   return *this;
}

jju64 & jju64::operator += (const jju32 x)
{
   jju32 temp, accum;
   jju16 carry;

   // save high half of low word
   temp = myLow32 >> 16;

   // add low halves of low words
   accum = (myLow32 & 0xffff) + (x & 0xffff);

   // put back the result
   myLow32 = accum & 0xffff;

   // create carry bit
   carry = ((accum & 0x10000L)!=0);

   // repeat for high half of low word
   accum = temp + (x >> 16) + carry;
   myLow32 |= accum << 16;
   carry = ((accum & 0x10000L)!=0);

   // now just ripple carry into hi
   myHigh32 += carry;
   return *this;
}

// complement operator
jju64 operator ~(const jju64 &a)
{
   jju64 retval(a);
   retval.myHigh32 = ~retval.myHigh32;
   retval.myLow32 = ~retval.myLow32;
   return retval;
}

// unary minus
jju64 operator -(const jju64 &a)
{
   jju64 retval(~a);
   retval += 1;
   return retval;
}

void jju64::shiftleft(void)
{
   short carry = ((myLow32 & 0x80000000L) != 0);
   myLow32 <<= 1;
   myHigh32 <<= 1;
   myHigh32 += carry;
}

void jju64::shiftright(void)
{
   jju32 carry = (myHigh32 & 1);
   myLow32 >>= 1;
   myHigh32 >>= 1;
   myLow32 |= (carry << 31);
}

jju64 & jju64::operator <<= (const int n)
{
  for(int i=0; i<n; i++)
    shiftleft();
  return *this;
}

jju64 & jju64::operator >>= (const int n)
{
   for(int i=0; i<n; i++)
      shiftright();
   return *this;
}

jju64 jju64::operator % (const jju64 & a)
{
  jju32 high_result;
  if ( a.myHigh32 != 0 )
    { high_result = this->myHigh32 / a.myHigh32; }
  else { high_result = 0; } // proper behavior?
  jju32 high_rem = this->myHigh32 - (high_result * a.myHigh32);
  jju32 low_result;
  if ( a.myLow32 != 0 )
    { low_result = (this->myLow32 + high_rem) % a.myLow32; }
  else { low_result = 0; } // proper behavior?

  /*
   Why?  There can't be any remainder from the top bits;
   that's handled in adding in the high-bytes' remainder to the
   lower bytes.
  */

  jju64 returnValue ( low_result );
  return returnValue;
}

jju64 jju64::operator / (const jju64 & a)
{
  jju32 high_result;
  if ( a.myHigh32 != 0 )
    { high_result = this->myHigh32 / a.myHigh32; }
  else { high_result = 0; } // proper behavior?
  jju32 high_rem = this->myHigh32 - high_result;	
  jju32 low_result;
  if ( a.myLow32 != 0 )
    { low_result = (this->myLow32 + high_rem) / a.myLow32; }
  else { low_result = 0; } // proper behavior?
  jju64 ret_val = jju64( high_result, low_result );
  return ret_val;
}

jju64 jju64::operator * (const jju64 & a)
{
  jju64 low_result = this->myLow32 * a.myLow32;
  // the overflow behavoir of this will be interesting...
  jju64 high_result = this->myHigh32 * a.myHigh32;
  high_result <<= 32;
  low_result += high_result;

  return low_result;
}

jju64::operator jju32 ()
{
  return this->myLow32;
}

jju64& jju64::operator = (const jju64 &new_value)
{
	this->myHigh32 = new_value.myHigh32;
	this->myLow32 = new_value.myLow32;
	return (*this);
}

void print_out_sizeof_types(void)
{
#ifdef NOTDEF
  jju64 u64;
  jju32 foo = 0xff;
  u64 = foo;
  u64 = 22L;
  u64 = 22;
	
  u64 = 0;
  u64 += 1000;
  u64 += 1000;

  jju64 twoK(0, 2000);
		   
  if (u64 == twoK)
    {
      kprintf("TRUE\n");
    }
  else
    {
      kprintf("FALSE\n");
    }
#endif

  kprintf("sizeof(jju8) = %d\n", sizeof(jju8));
  kprintf("sizeof(jju16) = %d\n", sizeof(jju16));
  kprintf("sizeof(jju32) = %d\n", sizeof(jju32));
  kprintf("sizeof(jju64) = %d\n", sizeof(jju64));
  kprintf("sizeof(unsigned char) = %d\n", sizeof(unsigned char));
  kprintf("sizeof(char) = %d\n", sizeof(char));
  kprintf("sizeof(short) = %d\n", sizeof(short));
  kprintf("sizeof(unsigned short) = %d\n", sizeof(unsigned short));
  kprintf("sizeof(int) = %d\n", sizeof(int));
  kprintf("sizeof(unsigned int) = %d\n", sizeof(unsigned int));
  kprintf("sizeof(long int) = %d\n", sizeof(long int));
  kprintf("sizeof(long) = %d\n", sizeof(long));
  kprintf("sizeof(unsigned long) = %d\n", sizeof(unsigned long));
  kprintf("sizeof(long long) = %d\n", sizeof(long long));
}

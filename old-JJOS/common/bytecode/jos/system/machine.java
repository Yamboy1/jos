/*
 *    The contents of this file are subject to the JJOS Public License
 *    Version 0.0 (the "License"); you may not use this file except in
 *    compliance with the License. You may obtain a copy of the License at
 *    http://www.jos.org/
 *
 *    Software distributed under the License is distributed on an "AS IS"
 *    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 *    License for the specific language governing rights and limitations
 *    under the License.
 *
 *    The Original Code is decaf's code, released October, 1998.
 *
 *    The Initial Developer of the Original Code is Todd L. Miller.
 *    Portions created by Todd L. Miller  are Copyright (C) 1998 & 1999.
 *    All Rights Reserved.
 *
 */

/**
 * machine.java
 *
 * interface to the kernel.
 */

package jos.system;

public class machine extends java.lang.Object {

    public native static byte  in8( short port );
    public native static char in16( short port );
    public native static int  in32( short port );

    public native static void  out8( short port, byte val );
    public native static void out16( short port, char val );
    public native static void out32( short port, int  val );

    public native static short read8(long address);
    public native static short write8(long address, short value); // Returns 8 bits written.
    public native static int read16(long address);
    public native static int write16(long address, int value); // Returns 16 bits written.
    public native static long read32(long address);
    public native static long write32(long address, long value); // Returns 32 bits written.

    public native static byte[] mapByteArray(long address, long length);
} /* end jos.system.machine */


/*
 * constantpool.h
 * 
 * declares the classes representing
 * the constant pool for a Java class.
 */

#ifndef decaf_constantpool
#define decaf_constantpool

class ConstantPool;

#include "cpentry.h"
#include "d_types.h"
#include "istream.h"

class ConstantPool {

  public:
    ConstantPool();

    static ConstantPool * generateConstantPool( istream & is );
    virtual CPEntry * operator [] ( jju16 index ) = 0;

  protected:

}; /* end class ConstantPool */


class RawConstantPool : public ConstantPool {
  friend ConstantPool;

  public:
    RawConstantPool();

    CPEntry * operator [] ( jju16 index );

  protected:
    jju16 myCount;
    CPEntry ** myConstantPool;

}; /* end class RawConstantPool */

#endif

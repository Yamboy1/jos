/*
 * typed.h
 * 
 * Declares a virtual base class
 * for run-time typing.
 */

#ifndef decaf_typed
#define decaf_typed

#include "d_types.h"

class Typed {
  public:
    virtual PrimitiveType type() = 0;
    };

#endif

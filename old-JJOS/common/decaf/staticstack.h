/*
 * staticstack.h
 *
 * Defines the C++ class which
 * implements a static stack,
 * e.g. the Java frame's operand
 * stack.
 */

#ifndef decaf_staticstack
#define decaf_staticstack

template<class Type> class StaticStack;
#include "stdlib.h" /* for kprintf() */

template <class Type>
class StaticStack {

  public:
    StaticStack( jint depth );

    bool push( Type * t );
    Type * pop();
    bool isEmpty();

  protected:
	Type ** myStack;
	jint myMaxDepth;
	jint myCurDepth;

}; /* end class StaticStack */


template <class Type> StaticStack<Type>::StaticStack( jint depth ) {
	myMaxDepth = depth;
	if ( myMaxDepth == 0 ) {
		myStack = NULL;
		return;
		}
	myStack = new Type * [ depth ];
	if (myStack == NULL ) {
		kprintf( "StaticStack::StaticStack(%d) -- unable to allocate stack, aborting.\n" );
		abort();
		}
	myCurDepth = -1;
	} /* end StaticStack() */

template <class Type> bool StaticStack<Type>::push( Type * t ) {
    myCurDepth++;
    if( myCurDepth > myMaxDepth ) {
	kprintf( "StaticStack::push() -- stack overflow, aborting.\n" );
	abort();
	}
    myStack[myCurDepth] = t;
    return true;
    } /* end push() */

template <class Type> Type * StaticStack<Type>::pop() {
    if ( myCurDepth < 0 ) {
	kprintf( "StaticStack::pop() -- stack underflow, aborting.\n" );
	abort();
	}
    Type * tmp = myStack[myCurDepth];
    myStack[myCurDepth] = NULL;
    myCurDepth--;
    return tmp;
    } /* end pop() */

template <class Type> bool StaticStack<Type>::isEmpty() {
    return myCurDepth == -1;
    } /* end isEmpty() */


#endif

/*
 * dynamicstack.h
 * 
 * declares and defines
 * a templated dynamic stack.
 * (e.g. no known max depth)
 */

#ifndef decaf_dynamicstack
#define decaf_dynamicstack

/* BTW, I should probably use the STL,
 * but it's probably overkill in this case. */

/* Also, this could be horribly inefficient,
 * but we'll see.  A hard limit on stack frames
 * may be good architecture, anyway. */

template<class Type0> class DSElement;
template<class Type1> class DynamicStack;

#include "stdlib.h" /* for kprintf() */

template <class Type0>
class DSElement {

  public:
    DSElement();
  
    Type0 * myVal;
    DSElement< Type0 > * myBottom;

}; /* end class DSElement */

template <class Type1>
class DynamicStack {

  public:
    DynamicStack();

    void push( Type1 * t );
    Type1 * pop();
    Type1 * peek();
    bool isEmpty();

  protected:
    DSElement< Type1 > * top;

}; /* end class DyamicStack */


template <class Type1> DynamicStack<Type1>::DynamicStack() { top = NULL; }

template <class Type1> void DynamicStack<Type1>::push( Type1 * t ) {
    /* top's new bottom element */
    DSElement< Type1 > * newBottom = top;
    /* dynamic alloc the new top */
    top = new DSElement< Type1 >();
    if ( top == NULL ) {
        kprintf( "DynamicStack::push() -- unable to allocate another element, aborting.\n" );
        abort();
        }
    /* fill in top */
    top->myVal = t;
    top->myBottom = newBottom;
    } /* end push() */

template <class Type1> Type1 * DynamicStack<Type1>::peek() {
    return top->myVal;
    }

template <class Type1> Type1 * DynamicStack<Type1>::pop() {
    /* the element to dealloc */
    DSElement< Type1 > * oldTop = top;
    /* the new top is top's bottom */
    top = oldTop->myBottom;
    /* save the return value */
    Type1 * tp = oldTop->myVal;
    /* dynamically free the old top */
    delete oldTop;
    /* return the old top */
    return tp;
    } /* end pop() */

template <class Type1> bool DynamicStack<Type1>::isEmpty() { return (top == NULL); }

template <class Type0> DSElement<Type0>::DSElement() { myVal = NULL; myBottom = NULL; }
     
#endif

/*
 * interp.cc
 * 
 * defines the decaf interpreter.
 */


/* ***** INVOCATION *****
 * 
 * The goal of an invocation to call
 * myStack->pushFrame() with the frame
 * of the method being invoked.
 */
 

/* pushInvocationFrame is a utility method for the invocation
 * methods; note that arguments must have been allocated with
 * new so can the pIF() can garbage-collect it. */

/* pushInvocationFrame() checks if the method is native
 * or synchronized, and sets up the arguments before pushing
 * the new frame.  (If necessary, static initialization and late
 * resolution could happen here too.) */

bool Frame::pushInvocationFrame( MethodInfo * mi, JavaWord * arguments ) {

  /* If the method is synchronized, acquire locks on its behalf. */
  if ( mi->isSynchronized() ) {
    DEBUG(( "%X monitorenters %X\n", myThread, jCI ));
    /* We don't need to for frame to store the monitor, because
     * when it returns, it still must have the lock. */
    return jCI->lock( myThread );
	} /* end if invocation synchronized */

  /* If the method is native, push the frame on the stack (tracking),
   * invoke the method, pop the frame, and carry on. */
  if ( mi->isNative() ) {
	myThread->pushFrame( this );
	invokeNativeMethod( methodName, methodSignature, arguments );
	myThread->popFrame();
	delete arguments;
	return true;
  	}

  /* Othewise, pass the method its arguments and push its frame onto the stack. */
  for ( jint i = 0; i < mi->getMyArgumentCount(); i++ ) {
	myLocalVariables->setJavaWord( i, arguments[i] );
    } delete arguments;
  myThread->pushFrame( this );

  /* The invocation was successful. */
  return true;
  } /* end pushInvocationFrame() */



/* for invokestatic(),
 * the class under discussion never changes,
 * so the native-method check can occur
 * at any time. */

bool Frame::invokestatic(exception_to_throw &e) {
  /* ought static method references be cached? */

  int debug = FN_TRACE;

  /* Fetch the method reference. */
  jju16 methodIndex = read_jju16( myPC + 1 );
  myPC += 2;

  /* Fetch the referenced method. */
#ifdef DONT_USE_DC
  ConstantMethodReference * cmr = myClass->getMyConstant( methodIndex );

  if ( cmr->type() != TAG_METHODINFO ) {
    kprintf( "Frame::invokestatic() -- method references non-method constant, aborting.\n" );
    abort();
    }

  /* Fetch the class in which the method resides. */
  JavaString * className = cmr->getMyClassName();
#else
  JavaString * className = NULL;
  ConstantMethodReference * cmr = NULL;
  if ( cmr = dynamic_cast<ConstantMethodReference *>(myClass->getMyConstant( methodIndex ) {
  		className = cmr->getMyClassName();
  		} else {
  		kprintf( "Frame::invokestatic() -- method reference non-method constant, aborting.\n" );
  		abort();
  		}
#endif

  if ( className == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get target class's name, aborting.\n" );
    abort();
    }

#ifdef TRULY_PARANOID
  /* a valid JVM should be insured by generateFrame() */
  if ( myJVM == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get the current JVM, aborting.\n" );
    abort();
    }
#endif 
    
  ClassLoader * cL = myJVM->getMyCurrentClassLoader();
  if ( cL == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get the current classloader, aborting.\n" );
    abort();
    }
  
  JavaClassInstance * jCI = cL->getClass( className );
  if ( jCI == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get target class, aborting.\n" );
    abort();
    }

  /* Fetch the method's name and signature. */
  JavaString * methodName = cmr->getMyName();
  if ( methodName == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's name, aborting.\n" );
    abort();
    }
  
  JavaString * methodSignature = cmr->getMySignature();
  if ( methodSignature == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's signature, aborting.\n" );
    abort();
    }

  /* Generate the method's frame.  If the method is native,
   * its Code attribute will be NULL. */
  Frame * f = Frame::generateFrame( jCI, methodName, methodSignature );
  if ( f == NULL ) {
    kprintf( "Frame::invokestatic() -- failed to generate frame, aborting.\n" );
    abort();
    }
 
  /* Fetch the method information. */
  MethodInfo * mi = jCI->getMethodInfo( methodName, methodSignature );
  if ( mi == NULL ) {
    kprintf( "Frame::invokestatic() -- unable to get method's information, aborting.\n" );
    abort();
    }

  /* Generate the arguments. */
  JavaWord * argument = new JavaWord[mi->getMyArgumentCount()];
  if ( argument == NULL ) {
  	kprintf( "Frame::invokestatic() -- unable to allocate argument temporary, aborting\n" );
  	abort();
  	}

  /* Pop the arguments off the operand stack. */ 
  for ( jint i = (mi->getMyArgumentCount() - 1); i >= 0; i-- ) {
    argument[i] = myOpStack->pop();
    }

  /* Push the frame onto the stack. */
  return f->pushInvocationFrame( mi, argument );
  } /* end invokestatic() */


#ifdef INVOCATION

bool frame::invokespecial(exception_to_throw &e)
{
    int debug = FN_TRACE;

    /* fetch the method reference. */
    jju16 methodref = read_java_order_jju16(myPC+1);
    /* advance the program counter appropriately. */
    myPC += 2;

    /* fetch the resolved method */
    resolved_method *rm = lookupMethod(methodref);

    /* verify that it's OK */
    if ( !rm ) {
        kprintf("frame::invokespecial -- unresolved method.\n" );
        return false;
        }

    /* execution trace */
    TRACE(("invokespecial %d (%s.%s)\n", methodref, 
	  rm->classPtr()->class_name->c_str(), rm->namePtr()->c_str()));

    /* fetch the argument array */
    java_word argument[rm->nArgs()+1];
    popArgs(argument, rm->nArgs()+1);

    /* special methods can't be static, so the first argument,
     * `this', needs to be non-null */
    if (argument[0].isNull())
		{
   		/* This should return false. */
		DEBUG(("frame::invokespecial -- calling method on NULL object\n"));
		}


    /* static binding except when */
	java_class *jc = rm->classPtr();
	
	/* (1) ACC_SUPER is set in cur_class;
	   (2) the rm is NOT private and
	       NOT <init>;
	   (3) the rm->classPtr() IS
	       a superclass of cur_class */
	
	if ( cur_class->access_flags & ACC_SUPER &&
	     ! ( rm->accessFlags() & ACC_PRIVATE ) &&
	     ! ( * rm->namePtr() == constant_utf8( "<init>" ) ) && 
	     cur_class->isParentOf(rm->classPtr()))
	   {
	   jc = ((java_object *)argument[0])->my_class;
       /* dynLink returns the method's class */
       if ( (jc = dynLink(rm, jc)) == NULL)
	       {
		   kprintf("frame::invokespecial -- dynLink failed\n");
		   return(false);
		   } /* end if dynLink failed. */
	   } /* end if non-static binding */	  


    /* we've found it, so check if it's native.
     * this has to be done AFTER the possible dynamic
     * linkage, otherwise we might not find the native
     * call. */
    
    bool retval;
    
#ifdef FAST_NATIVE_CODE

    if ( rm->accessFlags() & ACC_NATIVE ) {
         if ( appliedKludgeForBuiltInMethods( retval, rm, jc, argument, rm->nArgs()+1) ) { 
		return retval;
		} else {
		kprintf( "frame::invokespecial -- can't find \"%s.%s\"\n",
			    jc->class_name->c_str(), rm->namePtr()->c_str() );
		return false;
		}
	}
#else

    DEBUG(( "is: looking at %s.%s\n", jc->class_name->c_str(), rm->namePtr()->c_str() ));
    if ( appliedKludgeForBuiltInMethods( retval, rm, jc, argument, rm->nArgs()+1) ) {
        return retval;
        } 
    else if ( rm->accessFlags() & ACC_NATIVE ) {
        kprintf( "frame::invokespecial -- can't find \"%s.%s\"\n",
	          jc->class_name->c_str(), rm->namePtr()->c_str() );
	}

#endif

    /* It's not native, try pushing its frame onto the stack. */		
    if ( ! pushInvocationFrame( jc, rm, argument, rm->nArgs()+1 ) ) {
            kprintf( "frame::invokespecial -- pIF failed, exiting thread.\n" );
            return false;
            }

    /* if the invocation is synchronized, acquire a lock on the 
	 * caller on behalf of this thread. */	
	if ( rm->accessFlags() & ACC_SYNCHRONIZED ) {
        DEBUG(( "%X monitorenters %X\n", myThread, jc ));
        threadable* t = (threadable*)argument[0];
        myThread->stack()->peek()->myMonitor = t;
        return t->lock( myThread );	  
	  	} /* end if invocation synchronized */

	/* if we've made it here, we're good to go. */
	return true;
} /* end invokespecial */	
    

bool frame::invokevirtual(exception_to_throw &e)
{
    int debug = FN_TRACE;

    /* fetch the method reference. */
    jju16 methodref = read_java_order_jju16(myPC+1);
    /* advance the program counter appropriately. */
    myPC += 2;

    /* fetch the resolved method */
    resolved_method *rm = lookupMethod(methodref);

    /* verify that it's OK */
    if ( !rm ) {
        kprintf("frame::invokevirtual -- unresolved method.\n" );
        return false;
        }

    /* execution trace */
    TRACE(("invokevirtual %d (%s.%s)\n", methodref, 
	  rm->classPtr()->class_name->c_str(), rm->namePtr()->c_str()));

    /* fetch the argument array */
    java_word argument[rm->nArgs()+1];
    popArgs(argument, rm->nArgs()+1);

    /* should be with dynLink, but ugly hack prevents that. */
    java_class * jc = NULL;

    /* special methods can't be static, so the first argument,
     * `this', needs to be non-null */
    if (argument[0].isNull())
		{
#ifndef FAST_NATIVE_CODE
		DEBUG(("frame::invokevirtual -- calling method on NULL object\n"));
		goto ugly_hack;
#else
		kprintf( "frame::invokevirtual -- method called on NULL object, aborting.\n" );
		return false;
#endif
		}


    /* find the method's class */
    jc = ((java_object *)argument[0])->my_class;
    if ( rm->classPtr() != jc ) {
        if ( (jc = dynLink( rm, jc )) == NULL ) {
            kprintf( "invokevirtual == dynLink failed, aborting.\n" );
            abort();
            }
        }    

    /* we've found it, so check if it's native.
     * this has to be done AFTER the possible dynamic
     * linkage, otherwise we might not find the native
     * call. */
    
    bool retval;
    
#ifdef FAST_NATIVE_CODE

    if ( rm->accessFlags() & ACC_NATIVE ) {
         if ( appliedKludgeForBuiltInMethods( retval, rm, jc, argument, rm->nArgs()+1) ) { 
		return retval;
		} else {
		kprintf( "frame::invokevirtual -- can't find \"%s.%s\"\n",
			    jc->class_name->c_str(), rm->namePtr()->c_str() );
		return false;
		}
	}

#else

    DEBUG(( "iv: looking at %s.%s\n", jc->class_name->c_str(), rm->namePtr()->c_str() ));
    ugly_hack:;
    if ( appliedKludgeForBuiltInMethods( retval, rm, jc, argument, rm->nArgs()+1) ) {
        return retval;
        } else if ( rm->accessFlags() & ACC_NATIVE ) {
        kprintf( "frame::invokevirtual -- can't find \"%s.%s\"\n",
	          jc->class_name->c_str(), rm->namePtr()->c_str() );
	}

#endif

    /* It's not native, try pushing its frame onto the stack. */		
    if ( ! pushInvocationFrame( jc, rm, argument, rm->nArgs()+1 ) ) {
            kprintf( "frame::invokevirtual -- pIF failed, exiting thread.\n" );
            return false;
            }

    /* if the invocation is synchronized, acquire a lock on the 
	 * caller on behalf of this thread. */	
	if ( rm->accessFlags() & ACC_SYNCHRONIZED ) {
        DEBUG(( "%X monitorenters %X\n", myThread, jc ));
        threadable* t = (threadable*)argument[0];
        myThread->stack()->peek()->myMonitor = t;
        return t->lock( myThread );	  
	  	} /* end if invocation synchronized */

	/* if we've made it here, we're good to go. */
	return true;
} /* end invokevirtual */	


/* CHANGED to check native AFTER dynLink. */
bool frame::invokeinterface(exception_to_throw &e)
{
    int debug = FN_TRACE;

    /* fetch the method reference. */
    jju16 methodref = read_java_order_jju16(myPC+1);
    /* advance the program counter appropriately.
     * this includes this jju16 reference, and a
     * jju8 argument count plus trailing zero. */
     myPC += 4;

    /* fetch the resolved method */
    resolved_method *rm = lookupMethod(methodref);

    /* verify that it's OK */
    if ( !rm ) {
        kprintf("frame::invokeinterface -- unresolved method.\n" );
        return false;
        }

    /* execution trace */
    TRACE(("invokeinterface %d (%s.%s)\n", methodref, 
	  rm->classPtr()->class_name->c_str(), rm->namePtr()->c_str()));

    /* fetch the argument array */
    java_word argument[rm->nArgs()+1];
    popArgs(argument, rm->nArgs()+1);

    /* interface methods can't be static, so the first argument,
     * `this', needs to be non-null */
    if (argument[0].isNull())
		{
	    /* This should return false. */
		DEBUG(("frame::invokeinterface -- calling method on NULL object\n"));
		}

    /* fetch the class to start searching for the interface method in. */
    java_class *jc = ((java_object *)argument[0])->my_class;

    /* search for it. */
    if ( (jc = dynLink(rm, jc)) == NULL)
       {
	   kprintf("frame::invokeinterface -- dynamic link failed\n");
	   return(false);
       }

    /* we've found it, so check if it's native.
     * this has to be done AFTER the class implementing the
     * interface method is located, otherwise the native-code
     * lookup will munge with the wrong class. */

    bool retval;
    
#ifdef FAST_NATIVE_CODE

    if ( rm->accessFlags() & ACC_NATIVE ) {
         if ( appliedKludgeForBuiltInMethods( retval, rm, jc, argument, rm->nArgs()+1) ) { 
		return retval;
		} else {
		kprintf( "frame::invokeinterface -- can't find \"%s.%s\"\n",
		    jc->class_name->c_str(), rm->namePtr()->c_str() );
		return false;
		}
	}

#else

    DEBUG(( "ii: looking at %s.%s\n", jc->class_name->c_str(), rm->namePtr()->c_str() ));
    if ( appliedKludgeForBuiltInMethods( retval, rm, jc, argument, rm->nArgs()+1) ) {
        return retval;
        } else if ( rm->accessFlags() & ACC_NATIVE ) {
        kprintf( "frame::invokeinterface -- can't find \"%s.%s\"\n",
		  jc->class_name->c_str(), rm->namePtr()->c_str() );
	}

#endif

    /* It's not native, try pushing its frame onto the stack. */		
    if ( ! pushInvocationFrame( jc, rm, argument, rm->nArgs()+1 ) ) {
            kprintf( "frame::invokeinterface -- pIF failed, exiting thread.\n" );
            return false;
            }

    /* if the invocation is synchronized, acquire a lock on the 
	 * caller on behalf of this thread. */	
	if ( rm->accessFlags() & ACC_SYNCHRONIZED ) {
        DEBUG(( "%X monitorenters %X\n", myThread, jc ));
        threadable* t = (threadable*)argument[0];
        myThread->stack()->peek()->myMonitor = t;
        return t->lock( myThread );	  
	  	} /* end if invocation synchronized */

	/* if we've made it here, we're good to go. */
	return true;
} /* end invokeinterface */

#endif /* INVOCATION */

/* is there, BTW, anything special we need to do to the operand stack
 * w.r.t. throwing exceptions? */
bool frame::aaload(Exception & e)
{
	jint index = myOpStack->pop();
	JavaObjectArray * joa = myOpStack->pop();

#ifdef DONT_USE_DC
#error frame::aaload() -- DONT_USE_DC unavailable, fixme.
#else

	if ( joa = dynamic_cast<JavaObjectArray *>(joa) ) {
		JavaClassInstance * jci = joa->getElement( index );
		if ( jci == NULL ) {
			/* $: make sure getElement returns NULL rather than barfing. */
			e = Exception::generateException( "IndexOutOfBoundsException" );
			return false;
			}
		TRACE(("aaload (%x = %x[%d]", jci, joa, index );
		myOpStack->push( jci );
		return true;
		} else {
		/* if joa is not an instance of JavaObjectArray,
		 * the VM is hosed. */
		kprintf( "frame::aaload() -- attempt to load from non-array, aborting.\n" );
		abort();
		}
}

#ifdef INTERPRETATION

bool frame::aastore(exception_to_throw &e)
{
  java_object *value = op_stack.pop_java_object();
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_object_array *joa = (java_object_array *)array_ref;
  TRACE(("aastore (%X[%d]=%X)", joa, indexval, value));
  joa->store(indexval, value);
  return(true);
}

bool frame::acons_null(exception_to_throw &e)
{
  TRACE(("aconst_null"));
  java_word better_be_null;
  op_stack.push_java_word(better_be_null);
  return(true);
}


bool frame::aload(exception_to_throw &e)
{
  myPC++;
    jju32 local_var_num = cur_code->code[myPC];
    java_word to_be_pushed = local_vars->load_java_word(local_var_num);
    TRACE(("aload %d (%X)", local_var_num, to_be_pushed));
    op_stack.push_java_word(to_be_pushed);
  return(true);
}

bool frame::aload_0(exception_to_throw &e)
{
  java_word to_be_loaded = local_vars->load_java_word(0);
  TRACE(("aload_0 (%X)", to_be_loaded));
  op_stack.push_java_word(to_be_loaded);
  return(true);
}

bool frame::aload_1(exception_to_throw &e)
{
  java_word to_be_loaded = local_vars->load_java_word(1);
  TRACE(("aload_1 (%X)", to_be_loaded));
  op_stack.push_java_word(to_be_loaded);
  return(true);
}

bool frame::aload_2(exception_to_throw &e)
{
  java_word to_be_loaded = local_vars->load_java_word(2);
  TRACE(("aload_2 (%X)", to_be_loaded));
  op_stack.push_java_word(to_be_loaded);
  return(true);
}

bool frame::aload_3(exception_to_throw &e)
{
  java_word to_be_loaded = local_vars->load_java_word(3);
  TRACE(("aload_3 (%X)", to_be_loaded));
  op_stack.push_java_word(to_be_loaded);
  return(true);
}

bool frame::anewarray(exception_to_throw &e)
{
  u2 class_x = u2( cur_code->code[myPC+1], cur_code->code[myPC+2] );
  myPC += 2;
  cp_info *cpi = cur_class->constant_pool->operator[] ( class_x.toNative() );
  jint size = op_stack.pop_jint();
  jju32 unsigned_size = size; // $$ ?huh?
  java_object_array *joa = new java_object_array(((resolved_class*)cpi)->classPtr(), unsigned_size);
	    
  /* $$$ DO NOT MANUALLY CONVERT ARRAY POINTERS! */
	    
  TRACE(("anewarray \"%s\" (size=%d, array=%X)",
	 ((resolved_class*)cpi)->classPtr()->class_name->c_str(),
	 size,
	 joa));
  op_stack.push_java_object( joa );
  return(true);
}

bool frame::areturn(exception_to_throw &e)
{
  TRACE(("areturn $$"));
  java_word java_retval = op_stack.pop_java_word();
  frame * f = myThread->stack()->pop();
  if (myThread->stack()->isEmpty())
    {
      kprintf( "run(): empty frame stack!\n" );
      return(false);
    }
  else
    {
      myThread->stack()->peek()->op_stack.push_java_word(java_retval);
	  if ( f->myMonitor != NULL ) {
        DEBUG(( "%d monitorexits %d\n", myThread, f->myMonitor ));
	  	f->myMonitor->unlock( myThread );
	  	} /* end if method was synchronized */
      return(true);
    }
}

bool frame::arraylength(exception_to_throw &e)
{
  TRACE(("arraylength $$"));
  op_stack.push_jint( ((java_jint_array*)op_stack.pop_java_object())->size() );
  return(true);
}

bool frame::astore(exception_to_throw &e)
{
  jju32 local_var_num = cur_code->code[(myPC+1)];
  java_word to_be_stored = op_stack.pop_java_word();
  TRACE(("astore %d (%X)", local_var_num, to_be_stored));
  local_vars->store_java_word(local_var_num, to_be_stored);
  myPC++;
  return(true);
}

bool frame::astore_0(exception_to_throw &e)
{
  TRACE(("astore_0 $$"));
  local_vars->store_java_word(0, op_stack.pop_java_word());
  return(true);
}

bool frame::astore_1(exception_to_throw &e)
{
  TRACE(("astore_1 $$"));
  local_vars->store_java_word(1, op_stack.pop_java_word());
  return(true);
}

bool frame::astore_2(exception_to_throw &e)
{
  TRACE(("astore_2 $$" ));
  local_vars->store_java_word(2, op_stack.pop_java_word());
  return(true);
}

bool frame::astore_3(exception_to_throw &e)
{
  TRACE(("astore_3 $$"));
  local_vars->store_java_word(3, op_stack.pop_java_word());
  return(true);
}

bool frame::athrow(exception_to_throw &e)
{
  /* because e starts out 'new' in java_thread, we're ok here. */
  return handleException( * op_stack.pop_java_object() );
}

bool frame::baload(exception_to_throw &e)
{
  java_word index = op_stack.pop_java_word();
  jint indexval = (jint)index;
	    
  java_object *array_ref = op_stack.pop_java_object();
  /* $$ Wastefully use jints instead of jju8s for booleans / bytes. */
  java_jint_array *jia = (java_jint_array *)array_ref;

  /* It's very important that memory-mapped arrays
     actually point to their real types. */
  jint b_value;
  if ( jia->array_type() == java_jint_array::T_MAPPED ) {
	java_mapped_array * jmap = (java_mapped_array*)array_ref;
        b_value = jmap->load(indexval);
	} else {
	b_value = jia->load(indexval);
	}
	    
  TRACE(("baload (idx=%d, val=%d)", indexval, b_value));
	    
  op_stack.push_jint(b_value);
  return(true);
}

bool frame::bastore(exception_to_throw &e)
{
  /* $$ Wastefully use jints instead of jju8s for booleans / bytes. */
  jint value = op_stack.pop_jint();
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jint_array *jia = (java_jint_array *)array_ref;

  /* It's very important that memory-mapped arrays
     actually point to their real types. */
  if ( jia->array_type() == java_jint_array::T_MAPPED ) {
	java_mapped_array * jmap = (java_mapped_array*)array_ref;
        jmap->store(indexval, (jbyte)value);
	} else {
	jia->store(indexval, value);
	}

  TRACE(("bastore (%X[%d]=%D)", jia, indexval, value));
  return(true);
}

bool frame::bipush(exception_to_throw &e)
{
  /* sign-extension? */
  jjc8 signed_byte_to_be_pushed = cur_code->code[myPC+1];
  jji32 signed_int_to_be_pushed = signed_byte_to_be_pushed;
  jju32 bits_to_be_pushed = signed_int_to_be_pushed;
  java_word to_be_pushed(bits_to_be_pushed);

  TRACE(("bipush %d", signed_int_to_be_pushed));

  op_stack.push_java_word(to_be_pushed);
  myPC++;
  return(true);
}

bool frame::caload			  (exception_to_throw &e)
{
  java_word index = op_stack.pop_java_word();
  jint indexval = (jint)index;
	    
  java_object *array_ref = op_stack.pop_java_object();
  /* $$ Wastefully use jints instead of jju8s for booleans. */
  java_jint_array *jia = (java_jint_array *)array_ref;
	    
  jint char_value = jia->load(indexval); // $$$$$ Should zero-extend, etc.
	    
  TRACE(("caload %X %d (loaded %d)", array_ref, indexval, char_value));
	    
  op_stack.push_jint(char_value);
  return(true);
}

bool frame::castore(exception_to_throw &e)
{
  java_word array_value = op_stack.pop_java_word();
	    
  java_word index = op_stack.pop_java_word();
  int indexval = (jint)index;
	    
  java_object *array_ref = op_stack.pop_java_object();
  /* $$ Wastefully use jints instead of jju8s for booleans. */
  java_jint_array *jia = (java_jint_array *)array_ref;
	    
  TRACE(("castore %X %d %d", array_ref, indexval, array_value));
	    
  jia->store(indexval, array_value); // $$$$$ Should truncate to 16 bits?
  return(true);
}

bool frame::checkcast(exception_to_throw &e)
{
  jju16 idx = u2( cur_code->code[myPC+1], cur_code->code[myPC+2] ).toNative();
  myPC += 2;
	    
  /* fetch the class / array type */
  cp_info* cpi = cur_class->constant_pool->operator[]( idx );
	    
  TRACE(("checkcast"));
  java_object *cast_from = op_stack.pop_java_object();

  if ( cast_from == NULL )
    {
      op_stack.push_java_object(cast_from);
      return(true);
    }		

  if (cast_from->checkcast(cpi, myThread))
    {
      op_stack.push_java_object(cast_from);
    }
  else
    {
      /* Throw a class cast exception. */
      kprintf("fixme: class cast exception should be thrown.\n" );
      /* avoid empty stack errors */
      op_stack.push_java_object(cast_from);
    }
  return(true);
}

bool frame::d2f(exception_to_throw &e)
{
  TRACE(("d2f"));
  jdouble jd = op_stack.pop_jdouble();
  jfloat jf = jd;
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::d2i(exception_to_throw &e)
{
  TRACE(("d2i"));
  jdouble jd = op_stack.pop_jdouble();
  jint ji = (jint)jd;
  java_word jw = ji;
  op_stack.push_java_word(jw);
  return(true);
}

bool frame::d2l(exception_to_throw &e)
{
  TRACE(("d2l"));
	    
  jdouble jd = op_stack.pop_jdouble();
  /* From JVM, by Meyer & Downing. d2l opcode details. */
  jju32 l_low = (jju32)jd;
  /* $$$$ do we *have* pow on the i386 build? */
  jju32 l_high = (jju32)(jd / pow(2, 32));
  jju64 jl(l_high, l_low);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::dadd(exception_to_throw &e)
{
  TRACE(("dadd $$"));
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
  jdouble jd3 = jd1+jd2;
  op_stack.push_jdouble(jd3);
  return(true);
}

bool frame::daload(exception_to_throw &e)
{
  {
    jint indexval = op_stack.pop_jint();
    java_object *array_ref = op_stack.pop_java_object();
    java_jdouble_array *jda = (java_jdouble_array *)array_ref;
    jdouble value = jda->load(indexval);
    TRACE(("daload (%F = %X[%d])", value, jda, indexval));
    op_stack.push_jdouble(value);
  }
  return(true);
}

bool frame::dastore	(exception_to_throw &e)
{
  jdouble value = op_stack.pop_jdouble();
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jdouble_array *jda = (java_jdouble_array *)array_ref;
  jda->store(indexval, value);
  TRACE(("dastore (%X[%d]=%F)", jda, indexval, value));
  return(true);
}

bool frame::dcmpg(exception_to_throw &e)
{
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
	    
  TRACE(("dcmp %F %F", jd1, jd2));
	    
  if (isnan(jd1) || isnan(jd2)) // Check NaNs before doing math on them.
    {
      TRACE((" (NaN pushed 1)"));
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jd2 > jd1)
    {
      TRACE((" (pushed 1)"));
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jd2 == jd1)
    {
      TRACE((" (pushed 0)"));
      op_stack.push_java_word(java_word((jint)0));
    }
  else if (jd2 < jd1)
    {
      TRACE((" (pushed -1)"));
      op_stack.push_java_word(java_word((jint)-1));
    }
  else
    {
      TRACE((" (??????) how did I get here?"));
      abort();
    }
  return(true);
}

bool frame::dcmpl(exception_to_throw &e)
{
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
	    
  TRACE(("dcmpl %F %F", jd1, jd2));
	    
  if (isnan(jd1) || isnan(jd2)) // Check NaNs doing math on them.
    {
      TRACE((" (NaN pushed -1)"));
      op_stack.push_java_word(java_word((jint)-1));
    }
  else if (jd2 > jd1)
    {
      TRACE((" (pushed 1)"));
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jd2 == jd1)
    {
      TRACE((" (pushed 0)"));
      op_stack.push_java_word(java_word((jint)0));
    }
  else if (jd2 < jd1)
    {
      TRACE((" (pushed -1)"));
      op_stack.push_java_word(java_word((jint)-1));
    }
  else
    {
      TRACE((" (how did I get here?)"));
      abort();
    }
  return(true);
}

bool frame::dconst_0(exception_to_throw &e)
{
  TRACE(("dconst_0"));
  op_stack.push_jdouble(0.0);
  return(true);
}

bool frame::dconst_1(exception_to_throw &e)
{
  TRACE(("dconst_1"));
  op_stack.push_jdouble(1.0);
  return(true);
}

bool frame::ddiv(exception_to_throw &e)
{		// for gcc, to limit variable scope.
  TRACE(("ddiv"));
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
  jdouble jd3 = jd2/jd1;
  op_stack.push_jdouble(jd3);
  return(true);
}

bool frame::dload(exception_to_throw &e)
{
  myPC++;
  jju32 local_var_num = cur_code->code[myPC];
  jdouble jd = local_vars->load_jdouble(local_var_num);
  TRACE(("dload %d (pushed %F)$$", local_var_num, jd));
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::dload_0(exception_to_throw &e)
{
  TRACE(("dload_0 $$"));
  jdouble jd = local_vars->load_jdouble(0);
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::dload_1(exception_to_throw &e)
{
  TRACE(("dload_1"));
  jdouble jd = local_vars->load_jdouble(1);
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::dload_2(exception_to_throw &e)
{
  TRACE(("dload_2 $$"));
  jdouble jd = local_vars->load_jdouble(2);
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::dload_3(exception_to_throw &e)
{
  TRACE(("dload_3"));
  jdouble jd = local_vars->load_jdouble(3);
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::dmul(exception_to_throw &e)
{
  TRACE(("dmul $$"));
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
  jdouble jd3 = jd1 * jd2;
  op_stack.push_jdouble(jd3);
  return(true);
}

bool frame::dneg(exception_to_throw &e)
{
  TRACE(("dneg $$"));
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd1neg = -1.0 * jd1;
  op_stack.push_jdouble(jd1neg);
  return(true);
}

bool frame::drem(exception_to_throw &e)
{		// for gcc, to limit variable scope.
  TRACE(("drem"));
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
  jdouble jd3 = jd2 / jd1;
  jdouble jd4 = (jd3 - (jint)jd3) * jd1;
  op_stack.push_jdouble(jd4);
  return(true);
}

bool frame::dreturn(exception_to_throw &e)
{
  TRACE(("dreturn $$"));
  jdouble java_retval = op_stack.pop_jdouble();
	    
  frame * f = myThread->stack()->pop();
  if (myThread->stack()->isEmpty())
    {
      return(false);
    }
  myThread->stack()->peek()->op_stack.push_jdouble(java_retval);
  if ( f->myMonitor != NULL ) {
    DEBUG(( "%d monitorexits %d\n", myThread, f->myMonitor ));
  	f->myMonitor->unlock( myThread );
  	} /* end if method was synchronized */  
  return(true);
}

bool frame::dstore(exception_to_throw &e)
{
  myPC++;
  jju32 indexval = cur_code->code[myPC];
  jdouble jd = op_stack.pop_jdouble();
  TRACE(("dstore %d (stored %F)", cur_code->code[myPC], jd));
  local_vars->store_jdouble(indexval, jd);
  return(true);
}

bool frame::dstore_0(exception_to_throw &e)
{
  TRACE(("dstore_0"));
  local_vars->store_jdouble(0, op_stack.pop_jdouble());
  return(true);
}

bool frame::dstore_1(exception_to_throw &e)
{
  TRACE(("dstore_1"));
  local_vars->store_jdouble(1, op_stack.pop_jdouble());
  return(true);
}

bool frame::dstore_2(exception_to_throw &e)
{
  TRACE(("dstore_2 $$"));
  local_vars->store_jdouble(2, op_stack.pop_jdouble());
  return(true);
}

bool frame::dstore_3(exception_to_throw &e)
{
  TRACE(("dstore_3"));
  local_vars->store_jdouble(3, op_stack.pop_jdouble());
  return(true);
}

bool frame::dsub(exception_to_throw &e)
{
  TRACE(("dsub $$"));
  jdouble jd1 = op_stack.pop_jdouble();
  jdouble jd2 = op_stack.pop_jdouble();
  jdouble jd3 = jd2 - jd1;
  op_stack.push_jdouble(jd3);
  return(true);
}

bool frame::f2d(exception_to_throw &e)
{
  TRACE(("f2d"));
  jfloat jf = op_stack.pop_jfloat();
  jdouble jd = jf;
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::f2i(exception_to_throw &e)
{
  TRACE(("f2i"));
  jfloat jf = op_stack.pop_jfloat();
  jint ji = (jint)jf;
  op_stack.push_jint(ji);
  return(true);
}

bool frame::f2l(exception_to_throw &e)
{
  TRACE(("f2l"));
  jfloat jf = op_stack.pop_jfloat();
  jlong jl = jf;
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::fadd(exception_to_throw &e)
{
  TRACE(("fadd $$"));
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf2 = op_stack.pop_jfloat();
  jfloat res = jf1 + jf2;
  op_stack.push_jfloat(res);
  return(true);
}

bool frame::faload(exception_to_throw &e)
{
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jfloat_array *jfa = (java_jfloat_array *)array_ref;
  jfloat value = jfa->load(indexval);
  TRACE(("faload (%f=%X[%d])\n", value, jfa, indexval));
  op_stack.push_jfloat(value);
  return(true);
}

bool frame::fastore(exception_to_throw &e)
{
  jfloat value = op_stack.pop_jfloat();
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jfloat_array *jfa = (java_jfloat_array *)array_ref;
  jfa->store(indexval, value);
  TRACE(("fastore (%X[%d] = %F)",
	 jfa, indexval, value));
  return(true);
}

bool frame::fcmpl(exception_to_throw &e)
{
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf2 = op_stack.pop_jfloat();
	    
  TRACE(("fcmpl %f %f", jf1, jf2));
	    
  if (isnan(jf1) || isnan(jf2)) // Check NaNs doing math on them.
    {
      TRACE((" (NaN pushed 1)"));
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jf2 > jf1)
    {
      TRACE((" (pushed 1)"));
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jf2 == jf1)
    {
      TRACE((" (pushed 0)"));
      op_stack.push_java_word(java_word((jint)0));
    }
  else if (jf2 < jf1)
    {
      TRACE((" (pushed -1)"));
      op_stack.push_java_word(java_word((jint)-1));
    }
  else
    {
      TRACE((" (how did I get here?)"));
      abort();
    }
  return(true);
}

bool frame::fcmpg(exception_to_throw &e)
{
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf2 = op_stack.pop_jfloat();
	    
  if (isnan(jf1) || isnan(jf2)) // Check NaNs before doing math on them.
    {
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jf2 > jf1)
    {
      op_stack.push_java_word(java_word((jint)1));
    }
  else if (jf2 == jf1)
    {
      op_stack.push_java_word(java_word((jint)0));
    }
  else if (jf2 < jf1)
    {
      op_stack.push_java_word(java_word((jint)-1));
    }
  else
    {
      abort();
    }
  return(true);
}

bool frame::fconst_0(exception_to_throw &e)
{
  TRACE(("fconst_0"));
  op_stack.push_jfloat(0.0);
  return(true);
}

bool frame::fconst_1(exception_to_throw &e)
{
  TRACE(("fconst_1"));
  op_stack.push_jfloat(1.0);
  return(true);
}

bool frame::fconst_2(exception_to_throw &e)
{
  TRACE(("fconst_2"));
  op_stack.push_jfloat(2.0);
  return(true);
}

bool frame::fdiv(exception_to_throw &e)
{
  TRACE(("fdiv $$"));
  jfloat jf2 = op_stack.pop_jfloat();
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf3 = jf2 / jf1;
  op_stack.push_jfloat(jf3);
  return(true);
}

bool frame::fload(exception_to_throw &e)
{
  myPC++;
  jint indexval = cur_code->code[myPC];
  jfloat jf = local_vars->load_jfloat(indexval);
  TRACE(("fload %d (loaded %f)", indexval, jf));
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::fload_0(exception_to_throw &e)
{
  TRACE(("fload_0"));
  jfloat jf = local_vars->load_jfloat(0);
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::fload_1(exception_to_throw &e)
{
  TRACE(("fload_1"));
  jfloat jf = local_vars->load_jfloat(1);
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::fload_2(exception_to_throw &e)
{
  TRACE(("fload_2"));
  jfloat jf = local_vars->load_jfloat(2);
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::fload_3(exception_to_throw &e)
{
  TRACE(("fload_3"));
  jfloat jf = local_vars->load_jfloat(3);
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::fmul(exception_to_throw &e)
{
  TRACE(("fmul $$"));
  jfloat jf2 = op_stack.pop_jfloat();
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf3 = jf1 * jf2;
  op_stack.push_jfloat(jf3);
  return(true);
}

bool frame::fneg(exception_to_throw &e)
{
  TRACE(("fneg $$"));
  jfloat jf = op_stack.pop_jfloat();
  jfloat jfneg = -1.0 * jf;
  op_stack.push_jfloat(jfneg);
  return(true);
}

bool frame::frem(exception_to_throw &e)
{
  TRACE(("frem $$"));
  jfloat jf2 = op_stack.pop_jfloat();
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf3 = jf2 / jf1;
  jfloat jf4 = (jf3 - (jint)jf3) * jf1;
  op_stack.push_jfloat(jf4);
  return(true);
}

bool frame::freturn(exception_to_throw &e)
{
  TRACE(("freturn"));
  jfloat java_retval = op_stack.pop_jfloat();
	    
  frame * f = myThread->stack()->pop();
  if (myThread->stack()->isEmpty())
    {
      return(false);
    }
  myThread->stack()->peek()->op_stack.push_jfloat(java_retval);
  if ( f->myMonitor != NULL ) {
    DEBUG(( "%d monitorexits %d\n", myThread, f->myMonitor ));
  	f->myMonitor->unlock( myThread );
  	} /* end if method was synchronized */  
  return(true);
}

bool frame::fstore(exception_to_throw &e)
{
  myPC++;
  jint indexval = cur_code->code[myPC];
  jfloat jf = op_stack.pop_jfloat();
  TRACE(("fstore %d (stored %f)", indexval, jf));
  local_vars->store_jfloat(indexval, jf);
  return(true);
}

bool frame::fstore_0(exception_to_throw &e)
{
  TRACE(("fstore_0"));
  jfloat jf = op_stack.pop_jfloat();
  local_vars->store_jfloat(0, jf);
  return(true);
}

bool frame::fstore_1(exception_to_throw &e)
{
  TRACE(("fstore_1"));
  jfloat jf = op_stack.pop_jfloat();
  local_vars->store_jfloat(1, jf);
  return(true);
}

bool frame::fstore_2(exception_to_throw &e)
{
  TRACE(("fstore_2"));
  jfloat jf = op_stack.pop_jfloat();
  local_vars->store_jfloat(2, jf);
  return(true);
}

bool frame::fstore_3(exception_to_throw &e)
{
  TRACE(("fstore_3"));
  jfloat jf = op_stack.pop_jfloat();
  local_vars->store_jfloat(3, jf);
  return(true);
}

bool frame::fsub(exception_to_throw &e)
{
  TRACE(("fsub $$"));
  jfloat jf2 = op_stack.pop_jfloat();
  jfloat jf1 = op_stack.pop_jfloat();
  jfloat jf3 = jf1 - jf2;
  op_stack.push_jfloat(jf3);
  return(true);
}

bool frame::getfield			  (exception_to_throw &e)
{
  /* fetch the field index */
  jju16 field = read_java_order_jju16(myPC+1);
  myPC += 2;
	    
  /* fetch the field */
  resolved_field* rf = (resolved_field*)cur_class->constant_pool->operator[]( field );
	    
  TRACE(("getfield %d (%s)", field, rf->namePtr()->c_str()));

  java_object* jop = op_stack.pop_java_object();

  if (!jop)
    {
      /* $$$ Throw NullPointerException. */
      kprintf("frame::getfield -- NullPointerException\n");
      return(false);
    }
  else
    {
      if ( rf->isADouble() )
	{
	  jdouble jd;
	  jop->fields->getOffsetDouble( rf->fieldOffset(), jd );
	  TRACE((" (obj=%X, double=%lf)", jop, jd));
	  op_stack.push_jdouble( jd );
	}			/* end if jdouble */
      else if ( rf->isALong() )
	{
	  jlong jl;
	  jop->fields->getOffsetLong( rf->fieldOffset(), jl );
	  TRACE((" (obj=%X, long=%X:%X)", jop, jl.myHigh32, jl.myLow32));
	  op_stack.push_jlong( jl );
	}			/* end if jlong */
      else
	{
	  java_word word;
	  jop->fields->getOffsetWord( rf->fieldOffset(), word );
	  TRACE((" (obj=%X, word=%X)", jop, word));
	  op_stack.push_java_word( word );
	}			/* end if single word */
      return(true);
    }
}

bool frame::getstatic(exception_to_throw &e)
{
  jju16 fieldref = read_java_order_jju16(myPC+1);
  myPC += 2;
	    
  resolved_field* rf = (resolved_field*)cur_class->constant_pool->operator[](fieldref);
  java_class* static_class = rf->classPtr();
	    
  static_class->doLateResolutionIfNecessary(myThread->classLoader(), myThread);
  static_class->doStaticInitializationIfNecessary();
	    
  /* Gotta put this AFTER the *IfNecessary otherwise looks like happens in wrong order. */
  TRACE(("getstatic %d (%s)", fieldref, rf->namePtr()->c_str()));
	    
  if (rf->isADouble())
    {
      /* $$$$ This should be right. */
      jdouble jd; 
      static_class->fields->getOffsetDouble( rf->fieldOffset(), jd );
      TRACE((" [double %lf]", jd));
      op_stack.push_jdouble( jd );
    }
  else if ( rf->isALong() )
    {
      /* $$$$ This should be right. */
      jlong jl;
      static_class->fields->getOffsetLong( rf->fieldOffset(), jl );
      TRACE((" [long %X:%X]", jl.myHigh32, jl.myLow32));
      op_stack.push_jlong( jl );
    }
  else
    {
      /* $$$$ This should be right. */
      java_word one;
      static_class->fields->getOffsetWord( rf->fieldOffset(), one );
      TRACE((" [single %X]", one));
      op_stack.push_java_word( one );
    }
  return(true);
}

bool frame::bc_goto(exception_to_throw &e)
{
  signed short offset = (( cur_code->code[myPC+1] << 8 ) | cur_code->code[myPC+2]);
  myPC += offset;
  myPC--;
  TRACE(("bc_goto %d", offset));
  return(true);
}

bool frame::goto_w(exception_to_throw &e)
{
  signed int offset = (cur_code->code[myPC+1] << 24) | (cur_code->code[myPC+2] << 16) | (cur_code->code[myPC+3] << 8) | cur_code->code[myPC+4];
  myPC += offset;
  myPC--;
  return(true);
}

bool frame::i2b(exception_to_throw &e)
{
  TRACE(("i2b"));
  jint ji = op_stack.pop_jint();
  jbyte jb = ji;
  java_word jw = jb;
  op_stack.push_java_word(jw);
  return(true);
}

bool frame::i2c(exception_to_throw &e)
{
  TRACE(("i2c"));
  jint ji = op_stack.pop_jint();
  jchar jc = ji;
  java_word jw = jc;
  op_stack.push_java_word(jw);
  return(true);
}

bool frame::i2d(exception_to_throw &e)
{
  TRACE(("i2d"));
  jint ji = op_stack.pop_jint();
  jdouble jd = ji;
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::i2f(exception_to_throw &e)
{
  TRACE(("i2f"));
  jint ji = op_stack.pop_jint();
  jfloat jf = ji;
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::i2l(exception_to_throw &e)
{
  TRACE(("i2l (sign-extend problem?)"));
  jint ji = op_stack.pop_jint();
  /* $$$ May fail to sign-extend (i32->u64)?? */
  jlong jl(ji);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::i2s(exception_to_throw &e)
{
  TRACE(("i2s"));
  jint ji = op_stack.pop_jint();
  jshort js = ji;
  op_stack.push_jshort(js);
  return(true);
}

bool frame::iadd(exception_to_throw &e)
{
  TRACE(("iadd $$"));
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j1 + j2;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::iaload(exception_to_throw &e)
{
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jint_array *jia = (java_jint_array *)array_ref;
  jint value = jia->load(indexval);
  TRACE(("iaload (%D = %X[%d])", value, jia, indexval));
  op_stack.push_jint(value);	
  return(true);
}

bool frame::iastore(exception_to_throw &e)
{
  jint value = op_stack.pop_jint();
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jint_array *jia = (java_jint_array *)array_ref;
  jia->store(indexval, value);
  TRACE(("iastore (%X[%d]=%D)", jia, indexval, value));
  return(true);
}


bool frame::iconst_0(exception_to_throw &e)
{
  TRACE(("iconst_0"));
  op_stack.push_jint(0);
  return(true);
}

bool frame::iconst_1(exception_to_throw &e)
{
  TRACE(("iconst_1"));
  op_stack.push_jint(1);
  return(true);
}

bool frame::iconst_2(exception_to_throw &e)
{
  TRACE(("iconst_2"));
  op_stack.push_jint(2);
  return(true);
}

bool frame::iconst_3(exception_to_throw &e)
{
  TRACE(("iconst_3"));
  op_stack.push_jint(3);
  return(true);
}

bool frame::iconst_4(exception_to_throw &e)
{
  TRACE(("iconst_4"));
  op_stack.push_jint(4);
  return(true);
}

bool frame::iconst_5(exception_to_throw &e)
{
  TRACE(("iconst_5"));
  op_stack.push_jint(5);
  return(true);
}

bool frame::iconst_m1(exception_to_throw &e)
{
  TRACE(("iconst_m1"));
  op_stack.push_jint(-1);
  return(true);
}

bool frame::idiv(exception_to_throw &e)
{
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j2 / j1;
  TRACE(("idiv %d %d (pushed %d)", j1, j2, j3));
  op_stack.push_jint(j3);
  return(true);
}

bool frame::if_acmpeq(exception_to_throw &e)
{
  TRACE(("if_acmpeq"));
  java_object *j1 = op_stack.pop_java_object();
  java_object *j2 = op_stack.pop_java_object();
	    
  if (j1 == j2)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::if_acmpne(exception_to_throw &e)
{
  java_object *j1 = op_stack.pop_java_object();
  java_object *j2 = op_stack.pop_java_object();
	    
  if (j1 != j2)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::iload(exception_to_throw &e)
{
  myPC++;
  TRACE(("iload $$"));
  jju32 index = cur_code->code[myPC];
  op_stack.push_java_word(local_vars->load_jint(index));
  return(true);
}

bool frame::iload_0(exception_to_throw &e)
{
  java_word to_be_pushed = local_vars->load_jint(0);
  TRACE(("iload_0 (%X)", to_be_pushed));
  op_stack.push_java_word(to_be_pushed);
  return(true);
}

bool frame::iload_1(exception_to_throw &e)
{
  java_word to_be_pushed = local_vars->load_jint(1); 
  TRACE(("iload_1 (%X)", to_be_pushed));
  op_stack.push_java_word(to_be_pushed);
  return(true);
}

bool frame::iload_2(exception_to_throw &e)
{
  java_word to_be_pushed = local_vars->load_jint(2); 
  TRACE(("iload_2 (%X)", to_be_pushed));
  op_stack.push_java_word(to_be_pushed);
  return(true);
}

bool frame::iload_3(exception_to_throw &e)
{
  TRACE(("iload_3 $$"));
  op_stack.push_java_word(local_vars->load_jint(3));
  return(true);
}


bool frame::ifgt(exception_to_throw &e)
{
  TRACE(("ifgt $$"));
	    
  jint ji = op_stack.pop_jint();
	    
  if (ji > 0)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }			
  return(true);
}

bool frame::iflt(exception_to_throw &e)
{
  TRACE(("iflt $$"));
	    
  jint ji = op_stack.pop_jint();
	    
  if (ji < 0)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }			
  return(true);
}

bool frame::ifle(exception_to_throw &e)
{
  TRACE(("ifle %d", read_java_order_jji16(myPC+1)));
	    
  jint ji = op_stack.pop_jint();
	    
  TRACE((" %d", ji));
	    
  if (ji <= 0)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
      TRACE((" (took branch)"));
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }			
  return(true);
}

bool frame::ifge(exception_to_throw &e)
{
  TRACE(("ifge $$"));
	    
  jint ji = op_stack.pop_jint();
	    
  if (ji >= 0)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }			
  return(true);
}

bool frame::ifeq(exception_to_throw &e)
{
  TRACE(("ifeq $$"));
	    
  jint ji = op_stack.pop_jint();
	    
  if (ji == 0)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }			
  return(true);
}

bool frame::ifne(exception_to_throw &e)
{
  TRACE(("ifne $$"));
	    
  jint ji = op_stack.pop_jint();
	    
  if (ji != 0)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::if_icmpgt(exception_to_throw &e)
{
  TRACE(("if_icmpgt $$"));
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();
	    
  if (j1 > j2)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::if_icmple(exception_to_throw &e)
{
  TRACE(("if_icmple $$"));
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();
	    
  if (j1 <= j2)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::if_icmpge(exception_to_throw &e)
{
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();
	    
  TRACE(("if_icmpge (%d %d)", j1, j2));

  if (j1 >= j2)
    {
      TRACE((" (T branch)"));
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      TRACE((" (F)"));
      myPC += 2;
    }
  return(true);
}

bool frame::if_icmpeq(exception_to_throw &e)
{
  TRACE(("if_icmpeq $$"));
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();
	    
  if (j1 == j2)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::if_icmpne(exception_to_throw &e)
{
  TRACE(("if_icmpne $$"));
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();
	    
  if (j1 != j2)
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::if_icmplt(exception_to_throw &e)
{
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();

  TRACE(("if_icmplt (%d %d)", j1, j2));

  // DEBUG((" %d < %d?", j1, j2 ));
	    
  if (j1 < j2)
    {
      TRACE((" (T branch)"));
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
      // DEBUG(( " yes\n" ));
    }
  else		// Skip the offset and get to next opcode.
    {
      TRACE((" (F)"));
      myPC += 2;
      // DEBUG(( " no\n" ));
    }
  return(true);
}

bool frame::ifnull(exception_to_throw &e)
{
  TRACE(("ifnull"));
	    
  java_word jw = op_stack.pop_java_word();
	    
  if (jw.isNull())
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::ifnonnull(exception_to_throw &e)
{
  TRACE(("ifnonnull"));
	    
  java_word jw = op_stack.pop_java_word();
	    
  if (!jw.isNull())
    {
      jji16 offset = read_java_order_jji16(myPC+1);
      myPC += offset;
      myPC--;		// $$$ Loop advance offset ?!?! Not sure I believe this.
    }
  else		// Skip the offset and get to next opcode.
    {
      myPC += 2;
    }
  return(true);
}

bool frame::istore(exception_to_throw &e)
{
  TRACE(("istore"));
	    
  jint value = op_stack.pop_jint();
  jju32 local_var_num = cur_code->code[++myPC];
  local_vars->store_jint(local_var_num, value);
  return(true);
}

bool frame::istore_0(exception_to_throw &e)
{
  TRACE(("istore_0 $$"));
  jint value = op_stack.pop_jint();
  local_vars->store_jint(0, value);
  return(true);
}

bool frame::istore_1(exception_to_throw &e)
{
  TRACE(("istore_1 $$"));
  jint value = op_stack.pop_jint();
  local_vars->store_jint(1, value);
  return(true);
}

bool frame::istore_2(exception_to_throw &e)
{
  TRACE(("istore_2 $$"));
  jint value = op_stack.pop_jint();
  local_vars->store_jint(2, value);
  return(true);
}

bool frame::istore_3(exception_to_throw &e)
{
  TRACE(("istore_3 $$"));
  jint value = op_stack.pop_jint();
  local_vars->store_jint(3, value);
  return(true);
}

bool frame::iinc(exception_to_throw &e)
{
  jju32 local_var_num = cur_code->code[myPC+1];
  jji32 increment = cur_code->code[myPC+2]; // $ Sign-extended correctly?
  jint tmp = local_vars->load_jint(local_var_num);

  /* contributed by Hilary Cheng */
  if ((increment & 0x00000080) == 0x00000080) {
  	// If Number is Negative, Change It To Negative
        increment = increment | 0xffffff00;
        tmp += increment;
        } else {
        tmp += increment;
        }

  TRACE(("iinc %d %d (%d)", local_var_num, increment, tmp));
  local_vars->store_jint(local_var_num, tmp);
  myPC += 2;
  return(true);
}

bool frame::imul(exception_to_throw &e)
{
  TRACE(("imul"));
	    
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j1 * j2;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::ineg(exception_to_throw &e)
{
  TRACE(("ineg"));
	    
  jint j = op_stack.pop_jint();
  j *= -1;
  op_stack.push_jint(j);
  return(true);
}

bool frame::ior(exception_to_throw &e)
{
  TRACE(("ior"));
	    
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j1 | j2;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::iand(exception_to_throw &e)
{
  TRACE(("iand"));
	    
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j1 & j2;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::irem(exception_to_throw &e)
{
  TRACE(("iand"));
	    
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j2 % j1;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::ireturn(exception_to_throw &e)
{
  TRACE(("ireturn $$"));
	    
  java_word jw = op_stack.pop_java_word();
  frame * f = myThread->stack()->pop();
	    
  if (myThread->stack()->isEmpty())
    {
      return(false);
    }
  else
    {
      myThread->stack()->peek()->op_stack.push_java_word(jw);
      if ( f->myMonitor != NULL ) {
	   DEBUG(( "%d monitorexits %d\n", myThread, f->myMonitor ));
  	   f->myMonitor->unlock( myThread );
  	   } /* end if method was synchronized */  

      return(true);
    }
}

bool frame::ishr(exception_to_throw &e)
{
  TRACE(("ishr"));
	    
  jint shift = op_stack.pop_jint();
  jint ji = op_stack.pop_jint();
  jint ji_shifted = ji >> shift;
  op_stack.push_jint(ji_shifted);
  return(true);
}

bool frame::iushr(exception_to_throw &e)
{
  TRACE(("iushr"));
	    
  jint shift = op_stack.pop_jint();
  jju32 ju = op_stack.pop_jju32();
  jju32 retval = ju >> shift; // Should be unsigned shift.
  op_stack.push_jju32(retval);
  return(true);
}

bool frame::ishl(exception_to_throw &e)
{
  TRACE(("ishl"));
	    
  jint shift = op_stack.pop_jint();
  jint ji = op_stack.pop_jint();
  jint ji_shifted = ji << shift;
  op_stack.push_jint(ji_shifted);
  return(true);
}

bool frame::isub(exception_to_throw &e)
{
  TRACE(("isub"));
  jint j2 = op_stack.pop_jint();
  jint j1 = op_stack.pop_jint();
  jint j3 = j1 - j2;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::ixor(exception_to_throw &e)
{
  TRACE(("ixor"));
	    
  jint j1 = op_stack.pop_jint();
  jint j2 = op_stack.pop_jint();
  jint j3 = j1 ^ j2;
  op_stack.push_jint(j3);
  return(true);
}

bool frame::instanceof(exception_to_throw &e)
{
  jju16 idx = read_java_order_jju16(myPC+1);
  myPC += 2;
	    
  resolved_class *instance_of = (resolved_class*)cur_class->constant_pool->operator[](idx);
  java_object *cast_from = op_stack.pop_java_object();
	    
  /* catches null java objects... */
  if ( cast_from == NULL )
    {
      op_stack.push_jint( 0 );
      return true;
    }

  if ( cast_from->checkcast(instance_of, myThread ) )
    {
      op_stack.push_jint(1);
    }
  else
    {
      op_stack.push_jint(0);
    }
  return(true);
}


bool frame::jsr(exception_to_throw &e)
{
  TRACE(("jsr"));
  op_stack.push_jju32(myPC+1);
  jji16 offset = read_java_order_jji16(myPC+1);
  myPC += offset;
  myPC--;		// $$$ Not sure I believe this, but it must be working.
  return(true);
}

bool frame::jsr_w(exception_to_throw &e)
{
  TRACE(("jsr_w"));
  op_stack.push_jju32(myPC+1);
  jji32 offset = read_java_order_jji32(myPC+1);
  myPC += offset;
  myPC--;		// $$$ Not sure I believe this, but it must be working.
  return(true);
}

bool frame::l2d(exception_to_throw &e)
{
  TRACE(("l2d"));
  jlong jl = op_stack.pop_jlong();
	    
  /* Loss of precision. */
  jdouble jd_high = jl.myHigh32 * pow(2, 32);
  jdouble jd_low = jl.myLow32;
  jdouble jd = jd_high + jd_low;
  op_stack.push_jdouble(jd);
  return(true);
}

bool frame::l2f(exception_to_throw &e)
{
  TRACE(("l2f"));
  jlong jl = op_stack.pop_jlong();
	    
  /* Do as much as possible in doubles for precision's sake. */
  /* Loss of precision anyways. */
  jdouble jd_high = jl.myHigh32 * pow(2, 32);
  jdouble jd_low = jl.myLow32;
  jdouble jd = jd_high + jd_low;
  jfloat jf = jd; // Lose lots of precision.
  op_stack.push_jfloat(jf);
  return(true);
}

bool frame::l2i(exception_to_throw &e)
{
  TRACE(("l2i"));
  jlong jl = op_stack.pop_jlong();
	    
  jint ji = jl.myHigh32; // Throw away low 32 bits.
  op_stack.push_jint(ji);
  return(true);
}

bool frame::ladd(exception_to_throw &e)
{
  TRACE(("ladd $$"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl1 + jl2;
  op_stack.push_jlong(jl3);
  return(true);
}

bool frame::laload(exception_to_throw &e)
{
  {
    jint indexval = op_stack.pop_jint();
    java_object *array_ref = op_stack.pop_java_object();
    java_jlong_array *jla = (java_jlong_array *)array_ref;
    jlong value = jla->load(indexval);
    TRACE(("laload (%X:%X=%X[%d])",
	   value.myHigh32, value.myLow32, jla, indexval));
    op_stack.push_jlong(value);
  }
  return(true);
}

bool frame::lastore	(exception_to_throw &e)
{
  {
    jlong value = op_stack.pop_jlong();
    jint indexval = op_stack.pop_jint();
    java_object *array_ref = op_stack.pop_java_object();
    java_jlong_array *jia = (java_jlong_array *)array_ref;
    jia->store(indexval, value);
    TRACE(("lastore (%X[%d]=%X:%X)",
	   jia, indexval, value.myHigh32, value.myLow32));
  }
  return(true);
}

bool frame::land(exception_to_throw &e)
{
  TRACE(("land"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl1 & jl3;
  op_stack.push_jlong(jl3);
  return(true);
}

bool frame::lcmp(exception_to_throw &e)
{
#ifdef UNDEF
  TRACE(("lcmp"));
	    
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
	    
  TRACE((" %X:%X %X:%X",
	 jl1.myHigh32, jl1.myLow32,
	 jl2.myHigh32, jl2.myLow32));
	    
  if (jl1 == jl2)
    {
      TRACE((" (pushed 0)"));
      op_stack.push_jint(0);
    }
  else if (jl1 > jl2)
    {
      TRACE((" (pushed -1)"));
      op_stack.push_jint(-1);
    }
  else if (jl1 < jl2)
    {
      TRACE((" (pushed 1)"));
      op_stack.push_jint(1);
    }
  else
    {
      kprintf("unexpected situation in lcmp!\n");
      return(false);
    }
  return(true);
#endif

/* Hilary's fix */
TRACE(("lcmp"));
 
jlong jl1 = op_stack.pop_jlong();
jlong jl2 = op_stack.pop_jlong();
jji32 sign1 = 0, sign2 = 0, sign_result = 0;
 
TRACE((" %X:%X %X:%X",
jl1.myHigh32, jl1.myLow32,
jl2.myHigh32, jl2.myLow32));
 
sign1 = jl1.myHigh32 & 0x80000000;
sign2 = jl2.myHigh32 & 0x80000000;
 
if (sign1 > sign2) {
	sign_result = 1;
	} else {
	sign_result = 0;
	}
 
if (jl1.myHigh32 > jl2.myHigh32) {
	op_stack.push_jint(-1);
	return true;
	} else {
	if (jl1.myHigh32 == jl2.myHigh32) {
		if (jl1.myLow32 > jl2.myLow32) {
			op_stack.push_jint(-1);
			return true;
			} else {
			if (jl1.myLow32 == jl2.myLow32) {
				op_stack.push_jint(0);
				return true;
				} else {
				op_stack.push_jint(1);
				return true;
				} /* end if/else low == */
			} /* end if/else low > */
		} else {
			if (sign_result)
				op_stack.push_jint(-1);
			else
				op_stack.push_jint(1);
			return true;
		} /* end if/else high == */
	} /* end if/else high > */
} /* end lcmp */

bool frame::lconst_0(exception_to_throw &e)
{
  TRACE(("lconst_0"));
  jlong zero(0);
  op_stack.push_jlong(zero);
  return(true);
}

bool frame::lconst_1(exception_to_throw &e)
{
  TRACE(("lconst_1\n"));
  jlong one(1);
  op_stack.push_jlong(one);
  return(true);
}

bool frame::ldiv(exception_to_throw &e)
{
  TRACE(("ldiv"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl2 / jl1;
  op_stack.push_jlong(jl3);
  return(true);
}

bool frame::lload(exception_to_throw &e)
{
  myPC++;
  TRACE(("lload"));
  jju32 local_var_num = cur_code->code[myPC];
  jlong jl = local_vars->load_jlong(local_var_num);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::lload_0(exception_to_throw &e)
{
  TRACE(("lload_0"));
  jlong jl = local_vars->load_jlong(0);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::lload_1(exception_to_throw &e)
{
  TRACE(("lload_1"));
  jlong jl = local_vars->load_jlong(1);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::lload_2(exception_to_throw &e)
{
  TRACE(("lload_2"));
  jlong jl = local_vars->load_jlong(2);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::lload_3(exception_to_throw &e)
{
  TRACE(("lload_3"));
  jlong jl = local_vars->load_jlong(3);
  op_stack.push_jlong(jl);
  return(true);
}

bool frame::lmul(exception_to_throw &e)
{
  TRACE(("lmul"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl1 * jl2;
  op_stack.push_jlong(jl3);
  return(true);
}

bool frame::lneg(exception_to_throw &e)
{
  TRACE(("lneg"));
  jlong jl = op_stack.pop_jlong();
  jlong jlneg = jl * ((jlong)-1);
  op_stack.push_jlong(jlneg);
  return(true);
}

bool frame::lor(exception_to_throw &e)
{
  TRACE(("lor"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl1 | jl2;
  op_stack.push_jlong(jl3);
  return(true);
}

bool frame::lrem(exception_to_throw &e)
{
  TRACE(("lrem"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl2 / jl1;
  jlong jl4 = jl2 - (jl2 * jl3);
  op_stack.push_jlong(jl4);
  return(true);
}

bool frame::lreturn(exception_to_throw &e)
{
  TRACE(("lreturn"));
  jlong java_retval = op_stack.pop_jlong();
  frame * f = myThread->stack()->pop();
  if (myThread->stack()->isEmpty())
    {
      return(false);
    }
  else
    {
      myThread->stack()->peek()->op_stack.push_jlong(java_retval);
      if ( f->myMonitor != NULL ) {
        DEBUG(( "%d monitorexits %d\n", myThread, f->myMonitor ));
    	f->myMonitor->unlock( myThread );
  	    } /* end if method was synchronized */  
      return(true);
    }
}

bool frame::lshl(exception_to_throw &e)
{
  TRACE(("lshl"));
  jint shift = op_stack.pop_jint();
  jlong jl = op_stack.pop_jlong();
  jlong jl_shifted = jl << shift;
  op_stack.push_jlong(jl_shifted);
  return(true);
}

bool frame::lstore(exception_to_throw &e)
{
  myPC++;
  TRACE(("lstore"));
  jju32 var_num = cur_code->code[myPC];
  jlong jl = op_stack.pop_jlong();
  local_vars->store_jlong(var_num, jl);
  return(true);
}

bool frame::lstore_0(exception_to_throw &e)
{
  TRACE(("lstore_0"));
  jlong jl = op_stack.pop_jlong();
  local_vars->store_jlong(0, jl);
  return(true);
}

bool frame::lstore_1(exception_to_throw &e)
{
  TRACE(("lstore_1"));
  jlong jl = op_stack.pop_jlong();
  local_vars->store_jlong(1, jl);
  return(true);
}

bool frame::lstore_2(exception_to_throw &e)
{
  TRACE(("lstore_2"));
  jlong jl = op_stack.pop_jlong();
  local_vars->store_jlong(2, jl);
  return(true);
}

bool frame::lstore_3(exception_to_throw &e)
{
  TRACE(("lstore_0"));
  jlong jl = op_stack.pop_jlong();
  local_vars->store_jlong(3, jl);
  return(true);
}

bool frame::lsub(exception_to_throw &e)
{
  TRACE(("lsub"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl2 - jl1;
  op_stack.push_jlong(jl3);
  return(true);
}

/* contributed by Hilary Cheng */
bool frame::lushr(exception_to_throw &e)
{
  TRACE(("lushr"));
                                
  jint shift = op_stack.pop_jint();
  jlong ju = op_stack.pop_jlong();
  unsigned long jju = ju;
  jju32 retval = jju >> shift; // Should be unsigned shift.
  op_stack.push_jlong(retval);
  return(true);
}

bool frame::lshr(exception_to_throw &e)
{
  TRACE(("lshl"));
  jint shift = op_stack.pop_jint();
  jlong jl = op_stack.pop_jlong();
  jlong jl_shifted = jl >> shift;
  op_stack.push_jlong(jl_shifted);
  return(true);
}

bool frame::lxor(exception_to_throw &e)
{
  TRACE(("lxor"));
  jlong jl1 = op_stack.pop_jlong();
  jlong jl2 = op_stack.pop_jlong();
  jlong jl3 = jl1 ^ jl2;
  op_stack.push_jlong(jl3);
  return(true);
}

bool frame::ldc(exception_to_throw &e)
{
  myPC++;
  cp_info *cpi = cur_class->constant_pool->operator[]( cur_code->code[myPC] );
	
  TRACE(("ldc %d", cur_code->code[myPC]));
	
  if (cpi->tag() == cp_info::tag_integer)
    {
      constant_integer *cpii = to_constant_integer(cpi);
      jint val = cpii->value();
      TRACE((" (pushed integer %d)", val));
      op_stack.push_jint(val);
    }
  else if (cpi->tag() == cp_info::tag_float)
    {
      constant_float *cpif = to_constant_float(cpi);
      jfloat val = cpif->value();
      TRACE((" (pushed float %f)", val));
      op_stack.push_jfloat(val);
    }
  else if ((cpi->tag() == cp_info::tag_string) || (cpi->tag() == cp_info::tag_utf8))
    {
      constant_utf8 *cpiu = to_constant_utf8(cpi);
      java_string *foo = new java_string(cpiu,
					 myThread->classLoader()->getClass(constant_utf8("java/lang/String").string(),
									   myThread));
      TRACE((" (pushed string \"%s\")", foo->myUTF8->c_str()));
      op_stack.push_java_object(foo);
    }
  else
    {
      kprintf("Bad constant type (%d) to push in bc::ldc\n", cpi->tag());
      op_stack.push_jint(0);
    }
  return(true);
}

bool frame::ldc_w(exception_to_throw &e)
{
#ifdef NOTDEF
  jju16 idx = u2( cur_code->code[myPC+1], cur_code->code[myPC+2] ).toNative();
#else
  jju16 idx = read_java_order_jju16(myPC+1);
#endif
  myPC += 2;
	    
  cp_info *cpi = cur_class->constant_pool->operator[]( idx );		        
  /* $ This code is mostly duplicated from above -- should be subr. */
  if (cpi->tag() == cp_info::tag_integer)
    {
      constant_integer *cpii = (constant_integer *)cpi;
      op_stack.push_jint(cpii->value());
    }
  else if (cpi->tag() == cp_info::tag_float)
    {
      constant_float *cpif = (constant_float *)cpi;
      op_stack.push_jfloat(cpif->value());
    }
  else if ((cpi->tag() == cp_info::tag_string) || (cpi->tag() == cp_info::tag_utf8))
    {
      constant_utf8 *cpiu = (constant_utf8 *)cpi;
      java_string *foo = new java_string(cpiu, myThread->classLoader()->getClass(constant_utf8("java/lang/String").string(),
										 myThread));
      op_stack.push_java_object(foo);
    }
  else
    {
      kprintf("Bad constant type (%d) to push in bc::ldc\n", cpi->tag());
      return(false);
    }
  return(true);
}


bool frame::ldc2_w(exception_to_throw &e)
{
  jju16 idx = u2( cur_code->code[myPC+1], cur_code->code[myPC+2] ).toNative();
  myPC += 2;
	    
  cp_info *cpi = cur_class->constant_pool->operator[]( idx );
  TRACE(("ldc2_w"));
	    
  if (cpi->tag() == cp_info::tag_double)
    {
      constant_double *cpid = to_constant_double(cpi);
      jdouble val = cpid->value();
      TRACE((" %F (jdouble)", val));
      op_stack.push_jdouble(val);
    }
  else if (cpi->tag() == cp_info::tag_long)
    {
      constant_long *cpil = to_constant_long(cpi);
      jlong val = cpil->value();
      TRACE((" %X:%X (jlong) ", val.myHigh32, val.myLow32));
      op_stack.push_jlong(val);
    }
  else
    {
      kprintf("Bad constant type (%d) to push in bc::ldc2_w\n", cpi->tag());
      return(false);
    }
  return(true);
}

bool frame::lookupswitch(exception_to_throw &e)
{
  DEBUG(( "testme: lookupswitching...\n" ));
  int index = myPC + 4 - (myPC % 4);
	    
  /* fetch 32-bit signed default offset */
  jji32 def = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
  index += 4;
	    
  /* fetch 32-bit signed count of pairs */
  jji32 pairs = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
  index += 4;
	    
  /* the key to switch on */
  jint key = op_stack.pop_jint();
	    
  /* switch on key */
  jji32 match = 0;
  for ( int x = 0; x < pairs; x++ )
    {
      /* should be able to do binary search? */
      match = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
      index += 4;
		
      if ( match == key )
	{
	  int jump = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
	  myPC += jump - 1; /* offset loop advance */
	  break;
	}		/* end jump */
		
      /* advance past offset */
      index += 4;
    }			/* end lookup loop */
	    
  if ( match != key )
    {
      myPC += def -1;	/* offset loop advance */
    }			/* end default case */
  return(true);
}

bool frame::monitorenter(exception_to_throw &e)
{
  threadable* t = (threadable*)op_stack.pop_java_object();
  /* true/false means continue/stop thread */
  DEBUG(( "%X monitorenters %X\n", myThread, t ));
  bool r = t->lock( myThread );
  return r;
}

bool frame::monitorexit(exception_to_throw &e)
{
  threadable* t = (threadable*)op_stack.pop_java_object();
  /* false means exception was thrown */
  DEBUG(( "%X monitorexits on %X\n", myThread, t ));
  bool r = t->unlock( myThread );
  return r;
}

bool frame::nop(exception_to_throw &e)
{
  TRACE(("nop\n"));
  return(true);
}

bool frame::bc_new(exception_to_throw &e)
{
  jju16 index = u2( cur_code->code[myPC+1], cur_code->code[myPC+2] ).toNative();
  myPC += 2;
	    
  java_class* the_class = ((resolved_class*)cur_class->constant_pool->operator[]( index ))->classPtr();
	    
  the_class->doLateResolutionIfNecessary(myThread->classLoader(), myThread);
  /* $ Why no static init? */

  java_object* jop = new java_object( the_class );
  op_stack.push_java_object(jop);

  TRACE(("bc_new idx=%d (class=%s, object=%X)\n",
	 index,
	 the_class->class_name->c_str(),
	 jop));

  return(true);
}

bool frame::putfield(exception_to_throw &e)
{
  /* fetch the field index */
  jju16 field = u2( cur_code->code[myPC+1], cur_code->code[myPC+2]).toNative();
  myPC += 2;
	    
  /* fetch the field */
  resolved_field* rf = (resolved_field*)cur_class->constant_pool->operator[]( field );
	    
  TRACE(("putfield %d (%s)", field, rf->namePtr()->c_str()));

  if ( rf->isADouble() )
    {
      jdouble jd = op_stack.pop_jdouble();
      java_object* jop = op_stack.pop_java_object();
      TRACE((" (obj=%X, double=%lf)", jop, jd));
      jop->fields->setOffsetDouble( rf->fieldOffset(), jd );
    }			/* end if jdouble */
  else if ( rf->isALong() )
    {
      jlong jl = op_stack.pop_jlong();
      java_object* jop = op_stack.pop_java_object();
      TRACE((" (obj=%X, long=%X:%X)", jop, jl.myHigh32, jl.myLow32));
      jop->fields->setOffsetLong( rf->fieldOffset(), jl );
    }			/* end if jlong */
  else
    {
      java_word word = op_stack.pop_java_word();
      java_object* jop = op_stack.pop_java_object();
      TRACE((" (obj=%X, word=%X)", jop, word));
      jop->fields->setOffsetWord( rf->fieldOffset(), word );
    }			/* end if single word */
  return(true);
}

bool frame::putstatic(exception_to_throw &e)
{
  // fetch the field.
  u2 field = u2( cur_code->code[myPC+1], cur_code->code[myPC+2]).toNative();
  myPC += 2;
	    
  resolved_field* rf = (resolved_field*)cur_class->constant_pool->operator[]( field.toNative() );
  java_class* static_class = rf->classPtr();
	    
  static_class->doLateResolutionIfNecessary(myThread->classLoader(), myThread);
  static_class->doStaticInitializationIfNecessary();
	    
  TRACE(("putstatic %d \"%s\"",
	 field.toNative(),
	 rf->namePtr()->c_str()));
	    
  if (rf->isALong())
    {
      jlong jl = op_stack.pop_jlong();
      static_class->fields->setOffsetLong( rf->fieldOffset(), jl );
    }
  else if ( rf->isADouble() )
    {
      jdouble jd = op_stack.pop_jdouble();
      static_class->fields->setOffsetDouble( rf->fieldOffset(), jd );
    }
  else
    {
      java_word word = op_stack.pop_java_word();
      static_class->fields->setOffsetWord( rf->fieldOffset(), word );
    }
  return(true);
}

bool frame::ret(exception_to_throw &e)
{
  jju32 var_num = cur_code->code[myPC+1];
  myPC = local_vars->load_jint(var_num); // $$$ Should be unsigned if you ask me.
  myPC--; // offset loop advance
  return(true);
}

bool frame::bc_return(exception_to_throw &e)
{
  TRACE(("bc_return"));

  frame * f = myThread->stack()->pop();

  if ( myThread->stack()->isEmpty() )
    {
      return false;
    }
    
  if ( f->myMonitor != NULL ) {
    DEBUG(( "%d monitorexits %d\n", myThread, f->myMonitor ));
  	f->myMonitor->unlock( myThread );
  	} /* end if method was synchronized */      
  return(true);
}

bool frame::sipush(exception_to_throw &e)
{
  jji16 short_value = read_java_order_jji16(myPC+1);
  myPC += 2;
  jji32 to_be_pushed = short_value;

  TRACE(("sipush %d (%X)", short_value, to_be_pushed));

  op_stack.push_jint(to_be_pushed);
  return(true);
}

bool frame::sastore(exception_to_throw &e)
{
  /* $$$ Wasteful/wrong use of jint. */
  jint value = op_stack.pop_jint();
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jint_array *jia = (java_jint_array *)array_ref;
  jia->store(indexval, value);
  TRACE(("sastore (%X[%d]=%D)", jia, indexval, value));
  return(true);
}

bool frame::saload(exception_to_throw &e)
{
  /* $$$ Wasteful/wrong use of jint. */
  jint indexval = op_stack.pop_jint();
  java_object *array_ref = op_stack.pop_java_object();
  java_jint_array *jia = (java_jint_array *)array_ref;
  jint value = jia->load(indexval);
  TRACE(("saload (%D = %X[%d])", value, jia, indexval));
  op_stack.push_jint(value);
  return(true);
}

bool frame::tableswitch(exception_to_throw &e)
{
  DEBUG(( "testme: tableswitching...\n" ));
  int index = myPC + 4 - (myPC % 4);
	    
  /* fetch 32-bit signed default offset */
  int def = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
  index += 4;
	    
  /* fetch 32-bit signed low value */
  int low = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
  index += 4;
	    
  /* fetch 32-bit signed high value */
  int high = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
  index += 4;			
	    
  /* check index against high/low values */
  jint idx = op_stack.pop_jint();
	    
  if ( idx < low || idx > high )
    {
      myPC += def - 1;	/* offset loop advance */
#ifdef NOTDEF
      break;
#else
      return(true);
#endif
    }
  else
    {
      /* offset into table */
      index += 4 * idx;
		
      /* calculate jump point */
      int jump = (((((cur_code->code[index] << 8) | cur_code->code[index+1]) << 8 ) | cur_code->code[index+2] ) << 8 ) | cur_code->code[index+3];
		
      myPC += jump - 1;			
#ifdef NOTDEF
      break;
#else
      return(true);
#endif
    }
}			/* end tableswitch */


bool frame::wide(exception_to_throw &e)
{
  jju8 opcode = cur_code->code[myPC+1];
	    
  /* form one */
  if ( opcode == bc::iinc )
    {
      /* $ should be function */
      jju16 idx = (cur_code->code[myPC+2] << 8) | cur_code->code[myPC+3];
      jint inc = (cur_code->code[myPC+4] << 8) | cur_code->code[myPC+5];
		
      inc += local_vars->load_jint( idx );
      local_vars->store_jint( idx, inc ); 
		
      myPC += 5;
      return(true);
    }			/* end wide iinc */
	    
  /* form two */
  jju16 idx = (cur_code->code[myPC+2] << 8) | cur_code->code[myPC+3];
#ifdef NOTDEF
  code += 3;
#else
  abort();
#endif
  switch (opcode)
    {
    case bc::iload:
    case bc::fload:
    case bc::aload:
      /* single-word loads */
      op_stack.push_java_word( local_vars->load_java_word(idx) );
      break;
    case bc::lload:
      op_stack.push_jlong( local_vars->load_jlong(idx) );
      break;
    case bc::dload:
      op_stack.push_jdouble( local_vars->load_jdouble(idx) );
      break;
    case bc::istore:
    case bc::fstore:
    case bc::astore:
      /* single-word stores */
      local_vars->store_java_word( idx, op_stack.pop_java_word() );
      break;
    case bc::lstore:
      local_vars->store_jlong( idx, op_stack.pop_jlong() );
      break;
    case bc::dstore:
      local_vars->store_jdouble( idx, op_stack.pop_jdouble() );
      break;				
    default:
      kprintf( "invalide wide opcode, halting thread...\n" );
      return false;
    }			/* end opcode switch */
  return(true);
}

bool frame::multianewarray(exception_to_throw &e)
{
  jju16 index = read_java_order_jji16(myPC+1);
  jju8 number_of_dimensions = cur_code->code[myPC+3];
	    
  TRACE(("multianewarray %u %u", index, number_of_dimensions));
	    
  cp_info *cpi = cur_class->constant_pool->operator[](index);
  if (debug) { cpi->dump(); }
  resolved_array *ra = (resolved_array *)cpi;
	    
  java_object *new_array = NULL;
  new_array = makeJavaMultiArray(myThread->classLoader(),
				 op_stack,
				 number_of_dimensions,
				 ra);
  if (new_array == NULL) // Bad karma.
    {
      kprintf("bc::multianewarray failed\n");
      return(false);
    }
  else
    {
      op_stack.push_java_object(new_array);
    }
	    
  myPC += 3;		// $ Only advance the args.
  return(true);
}


bool frame::newarray(exception_to_throw &e)
{
  /* type the array */
  myPC++;
  constant_utf8* cu8;
  switch ( cur_code->code[myPC] )
    {
    case 4:
      cu8 = new constant_utf8( "[Z" );
      break;		
    case 5:
      cu8 = new constant_utf8( "[C" );
      break;
    case 6:
      cu8 = new constant_utf8( "[F" );
      break;
    case 7:
      cu8 = new constant_utf8( "[D" );
      break;		
    case 8:
      cu8 = new constant_utf8( "[B" );
      break;		
    case 9:
      cu8 = new constant_utf8( "[S" );
      break;
    case 10:
      cu8 = new constant_utf8( "[I" );
      break;
    case 11:
      cu8 = new constant_utf8( "[J" );
      break;
    default:
      kprintf( "unknown primitive array type %d, halting thread\n", cur_code->code[myPC] );
      return false;
    }			/* end switch */
	    
  TRACE(( "newarray" ));
  op_stack.push_java_object(makeJavaArray(op_stack.pop_jint(),
					  myThread->classLoader(),
					  new resolved_array(cu8)));
  return(true);
}

bool frame::pop(exception_to_throw &e)
{
  TRACE(("pop"));
  op_stack.pop_java_word();
  return(true);
}

bool frame::pop2(exception_to_throw &e)
{
  TRACE(("pop2"));
  op_stack.pop_jlong(); // As good as jdouble.
  return(true);
}

bool frame::dup(exception_to_throw &e)
{
  TRACE(("dup"));
  java_word to_be_duped = op_stack.pop_java_word();
  op_stack.push_java_word(to_be_duped);
  op_stack.push_java_word(to_be_duped);
  return(true);
}

bool frame::dup_x1(exception_to_throw &e)
{
  TRACE(("dup_x1\n"));
  java_word w1 = op_stack.pop_java_word();
  java_word w2 = op_stack.pop_java_word();
  op_stack.push_java_word(w1);
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  return(true);
}

bool frame::dup_x2(exception_to_throw &e)
{
  TRACE(("dup_x2"));
  java_word w1 = op_stack.pop_java_word();
  java_word w2 = op_stack.pop_java_word();
  java_word w3 = op_stack.pop_java_word();
  op_stack.push_java_word(w1);
  op_stack.push_java_word(w3);
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  return(true);
}

bool frame::dup2(exception_to_throw &e)
{
  TRACE(("dup2"));
  java_word w1 = op_stack.pop_java_word();
  java_word w2 = op_stack.pop_java_word();
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  return(true);
}

bool frame::dup2_x1(exception_to_throw &e)
{
  TRACE(("dup2_x1"));
  java_word w1 = op_stack.pop_java_word();
  java_word w2 = op_stack.pop_java_word();
  java_word w3 = op_stack.pop_java_word();
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  op_stack.push_java_word(w3);
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  return(true);
}

bool frame::dup2_x2(exception_to_throw &e)
{
  TRACE(("dup2_x2"));
  java_word w1 = op_stack.pop_java_word();
  java_word w2 = op_stack.pop_java_word();
  java_word w3 = op_stack.pop_java_word();
  java_word w4 = op_stack.pop_java_word();
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  op_stack.push_java_word(w4);
  op_stack.push_java_word(w3);
  op_stack.push_java_word(w2);
  op_stack.push_java_word(w1);
  return(true);
}

bool frame::swap(exception_to_throw &e)
{
  TRACE(("swap"));
  java_word w1 = op_stack.pop_java_word();
  java_word w2 = op_stack.pop_java_word();
  op_stack.push_java_word(w1);
  op_stack.push_java_word(w2);
  return(true);
}








/*
 * runOpcode()
 * arguments: none
 * returns: if the thread has terminated.
 */

bool frame::runOpcode(exception_to_throw &e)
{
  bool retval = true;

#ifndef NOTDEF
  TRACE(("\n%X %X %X ", myThread, this, myPC));
#else
  if ( code[i] != bc::bc_goto ) { TRACE(("\n")); }
#endif
    
  //  TRACE(("-<<<<%d>>>>-", cur_code->code[myPC]));

  switch ( cur_code->code[myPC] )
    {
	
      // A* BYTECODES	
	
    case bc::aaload:
      retval = aaload(e);
      break;
    case bc::aastore:
      retval = aastore(e);
      break; 
    case bc::aconst_null:
      retval = acons_null(e);
      break;
    case bc::aload:
      retval = aload(e);
      break;
    case bc::aload_0:
      retval = aload_0(e);
      break;
    case bc::aload_1:
      retval = aload_1(e);
      break;
    case bc::aload_2:
      retval = aload_2(e);
      break;
    case bc::aload_3:
      retval = aload_3(e);
      break;
    case bc::anewarray:
      retval = anewarray(e);
      break;
    case bc::areturn:
      retval = areturn(e);
      break;
    case bc::arraylength:
      retval = arraylength(e);
      break;
    case bc::astore:
      retval = astore(e);
      break;
    case bc::astore_0:
      retval = astore_0(e);
      break;
    case bc::astore_1:
      retval = astore_1(e);
      break;
    case bc::astore_2:
      retval = astore_2(e);
      break;
    case bc::astore_3:
      retval = astore_3(e);
      break;
    case bc::athrow:
      retval = athrow(e);
      break;
	
      // B* BYTECODES
	
    case bc::baload:
      retval = baload(e);
      break;
    case bc::bastore:
      retval = bastore(e);
      break;
    case bc::bipush:
      retval = bipush(e);
      break;	
	
      // C* BYTECODES
	
    case bc::caload:
      retval = caload(e);
      break;
    case bc::castore:
      retval = castore(e);
      break;
    case bc::checkcast:
      retval = checkcast(e);
      break;
	
      // D* BYTECODES
	
    case bc::d2f:
      retval = d2f(e);
      break;
    case bc::d2i:
      retval = d2i(e);
      break;
    case bc::d2l:
      retval = d2l(e);
      break;
    case bc::dadd:
      retval = dadd(e);
      break;
    case bc::daload:
      retval = daload(e);
      break;
    case bc::dastore:
      retval = dastore(e);
      break;
    case bc::dcmpg:
      retval = dcmpg(e);
      break;
    case bc::dcmpl:
      retval = dcmpl(e);
      break;
    case bc::dconst_0:
      retval = dconst_0(e);
      break;
    case bc::dconst_1:
      retval = dconst_1(e);
      break;
    case bc::ddiv:
      retval = ddiv(e);
      break;
    case bc::dload:
      retval = dload(e);
      break;
    case bc::dload_0:
      retval = dload_0(e);
      break;
    case bc::dload_1:
      retval = dload_1(e);
      break;
    case bc::dload_2:
      retval = dload_2(e);
      break;
    case bc::dload_3:
      retval = dload_3(e);
      break;
    case bc::dmul:
      retval = dmul(e);
      break;
    case bc::dneg:
      retval = dneg(e);
      break;
    case bc::drem:
      retval = drem(e);
      break;
    case bc::dreturn:
      retval = dreturn(e);
      break;
    case bc::dstore:
      retval = dstore(e);
      break;
    case bc::dstore_0:
      retval = dstore_0(e);
      break;
    case bc::dstore_1:
      retval = dstore_1(e);
      break;
    case bc::dstore_2:
      retval = dstore_2(e);
      break;
    case bc::dstore_3:
      retval = dstore_3(e);
      break;
    case bc::dsub:
      retval = dsub(e);
      break;
	
      // E* BYTECODES
	
      // F* BYTECODES
	
    case bc::f2d:
      retval = f2d(e);
      break;
    case bc::f2i:
      retval = f2i(e);
      break;
    case bc::f2l:
      retval = f2l(e);
      break;
    case bc::fadd:
      retval = fadd(e);
      break;
    case bc::faload:
      retval = faload(e);
      break;		
    case bc::fastore:
      retval = fastore(e);
      break;
    case bc::fcmpl:
      retval = fcmpl(e);
      break;
    case bc::fcmpg:
      retval = fcmpg(e);
      break;
    case bc::fconst_0:
      retval = fconst_0(e);
      break;
    case bc::fconst_1:
      retval = fconst_1(e);
      break;
    case bc::fconst_2:
      retval = fconst_2(e);
      break;
    case bc::fdiv:
      retval = fdiv(e);
      break;
    case bc::fload:
      retval = fload(e);
      break;
    case bc::fload_0:
      retval = fload_0(e);
      break;
    case bc::fload_1:
      retval = fload_1(e);
      break;
    case bc::fload_2:
      retval = fload_2(e);
      break;
    case bc::fload_3:
      retval = fload_3(e);
      break;
    case bc::fmul:
      retval = fmul(e);
      break;
    case bc::fneg:
      retval = fneg(e);
      break;
    case bc::frem:
      retval = frem(e);
      break;
    case bc::freturn:
      retval = freturn(e);
      break;
    case bc::fstore:
      retval = fstore(e);
      break;
    case bc::fstore_0:
      retval = fstore_0(e);
      break;
    case bc::fstore_1:
      retval = fstore_1(e);
      break;
    case bc::fstore_2:
      retval = fstore_2(e);
      break;
    case bc::fstore_3:
      retval = fstore_3(e);
      break;
    case bc::fsub:
      retval = fsub(e);
      break;		
	
      // G* BYTECODES
	
    case bc::getfield:
      retval = getfield(e);
      break;
    case bc::getstatic:
      retval = getstatic(e);
      break;
    case bc::bc_goto:
      retval = bc_goto(e);
      break;
    case bc::goto_w:
      retval = goto_w(e);
      break;
	
      // H* BYTECODES
	
      // I* BYTECODES
	
    case bc::i2b:
      retval = i2b(e);
      break;
    case bc::i2c:
      retval = i2c(e);
      break;
    case bc::i2d:
      retval = i2d(e);
      break;
    case bc::i2f:
      retval = i2f(e);
      break;
    case bc::i2l:
      retval = i2l(e);
      break;
    case bc::i2s:
      retval = i2s(e);
      break;
    case bc::iadd:
      retval = iadd(e);
      break;		
    case bc::iaload:
      retval = iaload(e);
      break;
    case bc::iastore:
      retval = iastore(e);
      break;
    case bc::iconst_0:
      retval = iconst_0(e);
      break;
    case bc::iconst_1:
      retval = iconst_1(e);
      break;
    case bc::iconst_2:
      retval = iconst_2(e);
      break;
    case bc::iconst_3:
      retval = iconst_3(e);
      break;
    case bc::iconst_4:
      retval = iconst_4(e);
      break;
    case bc::iconst_5:
      retval = iconst_5(e);
      break;
    case bc::iconst_m1:
      retval = iconst_m1(e);
      break;
    case bc::idiv:
      retval = idiv(e);
      break;
    case bc::if_acmpeq:
      retval = if_acmpeq(e);
      break;
    case bc::if_acmpne:
      retval = if_acmpne(e);
      break;
    case bc::iload:
      retval = iload(e);
      break;
    case bc::iload_0:
      retval = iload_0(e);
      break;
    case bc::iload_1:
      retval = iload_1(e);
      break;
    case bc::iload_2:
      retval = iload_2(e);
      break;
    case bc::iload_3:
      retval = iload_3(e);
      break;
    case bc::ifgt:
      retval = ifgt(e);
      break;
    case bc::iflt:
      retval = iflt(e);
      break;
    case bc::ifle:
      retval = ifle(e);
      break;
    case bc::ifge:
      retval = ifge(e);
      break;
    case bc::ifeq:
      retval = ifeq(e);
      break;
    case bc::ifne:
      retval = ifne(e);
      break;
    case bc::if_icmpgt:
      retval = if_icmpgt(e);
      break;
    case bc::if_icmple:
      retval = if_icmple(e);
      break;
    case bc::if_icmpge:
      retval = if_icmpge(e);
      break;
    case bc::if_icmpeq:
      retval = if_icmpeq(e);
      break;
    case bc::if_icmpne:
      retval = if_icmpne(e);
      break;
    case bc::if_icmplt:
      retval = if_icmplt(e);
      break;
    case bc::ifnull:
      retval = ifnull(e);
      break;
    case bc::ifnonnull:
      retval = ifnonnull(e);
      break;
    case bc::istore:
      retval = istore(e);
      break;
    case bc::istore_0:
      retval = istore_0(e);
      break;
    case bc::istore_1:
      retval = istore_1(e);
      break;
    case bc::istore_2:
      retval = istore_2(e);
      break;
    case bc::istore_3:
      retval = istore_3(e);
      break;
    case bc::iinc:
      retval = iinc(e);
      break;
    case bc::imul:
      retval = imul(e);
      break;
    case bc::ineg:
      retval = ineg(e);
      break;
    case bc::ior:
      retval = ior(e);
      break;
    case bc::iand:
      retval = iand(e);
      break;
    case bc::irem:
      retval = irem(e);
      break;
    case bc::ireturn:
      retval = ireturn(e);
      break;
    case bc::ishr:
      retval = ishr(e);
      break;
    case bc::iushr:
      retval = iushr(e);
      break;
    case bc::ishl:
      retval = ishl(e);
      break;
    case bc::isub:
      retval = isub(e);
      break;
    case bc::ixor:
      retval = ixor(e);
      break;
    case bc::instanceof:
      retval = instanceof(e);
      break;
	
	
      // J* BYTECODES
	
    case bc::jsr:
      retval = jsr(e);
      break;
    case bc::jsr_w:
      retval = jsr_w(e);
      break;
	
      // K* BYTECODES
	
      // L* BYTECODES
	
    case bc::l2d:
      retval = l2d(e);
      break;
    case bc::l2f:
      retval = l2f(e);
      break;
    case bc::l2i:
      retval = l2i(e);
      break;
    case bc::ladd:
      retval = ladd(e);
      break;
    case bc::laload:
      retval = laload(e);
      break;
    case bc::lastore:
      retval = lastore	(e);
      break;
    case bc::land:
      retval = land(e);
      break;
    case bc::lcmp:
      retval = lcmp(e);
      break;
    case bc::lconst_0:
      retval = lconst_0(e);
      break;
    case bc::lconst_1:
      retval = lconst_1(e);
      break;
    case bc::ldiv:
      retval = ldiv(e);
      break;
    case bc::lload:
      retval = lload(e);
      break;
    case bc::lload_0:
      retval = lload_0(e);
      break;
    case bc::lload_1:
      retval = lload_1(e);
      break;
    case bc::lload_2:
      retval = lload_2(e);
      break;
    case bc::lload_3:
      retval = lload_3(e);
      break;
    case bc::lmul:
      retval = lmul(e);
      break;
    case bc::lneg:
      retval = lneg(e);
      break;
    case bc::lor:
      retval = lor(e);
      break;
    case bc::lrem:
      retval = lrem(e);
      break;
    case bc::lreturn:
      retval = lreturn(e);
      break;
    case bc::lshl:
      retval = lshl(e);
      break;
    case bc::lstore:
      retval = lstore(e);
      break;
    case bc::lstore_0:
      retval = lstore_0(e);
      break;
    case bc::lstore_1:
      retval = lstore_1(e);
      break;
    case bc::lstore_2:
      retval = lstore_2(e);
      break;
    case bc::lstore_3:
      retval = lstore_3(e);
      break;
    case bc::lsub:
      retval = lsub(e);
      break;
    case bc::lshr:
      retval = lshr(e);
      break;
    case bc::lushr:
      retval = lushr(e);
      break;
    case bc::lxor:
      retval = lxor(e);
      break;
    case bc::ldc:
      retval = ldc(e);
      break;
    case bc::ldc_w:
      retval = ldc_w(e);
      break;
    case bc::ldc2_w:
      retval = ldc2_w(e);
      break;
    case bc::lookupswitch:
      retval = lookupswitch(e);
      break;
	
      // M* BYTECODES
	
    case bc::monitorenter:
      retval = monitorenter(e);
      break;
    case bc::monitorexit:
      retval = monitorexit(e);
      break;
	
      // N* BYTECODES
	
    case bc::nop:
      retval = nop(e);
      break;
    case bc::bc_new:
      retval = bc_new(e);
      break;
	
	
      // O* BYTECODES
	
      // P* BYTECODES
	
    case bc::putfield:
      retval = putfield(e);
      break;
    case bc::putstatic:
      retval = putstatic(e);
      break;
	
      // Q* BYTECODES
	
      // R* BYTECODES
	
    case bc::ret:
      retval = ret(e);
      break;
	
    case bc::bc_return:
      retval = bc_return(e);
      break;
	
      // S* BYTECODES
	
    case bc::sipush:
      retval = sipush(e);
      break;
    case bc::sastore:
      retval = sastore(e);
      break;
    case bc::saload:
      retval = saload(e);
      break;
	
	
      // T* BYTECODES
	
    case bc::tableswitch:
      retval = tableswitch(e);
      break;
	
      // U* BYTECODES
	
      // V* BYTECODES
	
      // W* BYTECODES
	
    case bc::wide:
      retval = wide(e);
      break;
	
      // X* BYTECODES
	
      // Y* BYTECODES
	
      // Z* BYTECODES
	
      // ARRAY OPS		
	
    case bc::multianewarray:
      retval = multianewarray(e);
      break;
    case bc::newarray:
      retval = newarray(e);
      break;
	
      // INVOCATION OPS
	
#ifdef NOTDEF
    case bc::invokestatic:
    case bc::invokespecial:
    case bc::invokevirtual:
    case bc::invokeinterface:
      if (!runInvoke(e)) // Bad Karma happened.
	{
	  return(false);
	};
      break;
#else
    case bc::invokestatic:
      retval = invokestatic(e);
      break;
    case bc::invokespecial:
      retval = invokespecial(e);
      break;
    case bc::invokevirtual:
      retval = invokevirtual(e);
      break;
    case bc::invokeinterface:
      retval = invokeinterface(e);
      break;
#endif
	
      // STACK OPS
	
    case bc::pop:
      retval = pop(e);
      break;
    case bc::pop2:
      retval = pop2(e);
      break;	
    case bc::dup:
      retval = dup(e);
      break;
    case bc::dup_x1:
      retval = dup_x1(e);
      break;		
    case bc::dup_x2:
      retval = dup_x2(e);
      break;		
    case bc::dup2:
      retval = dup2(e);
      break;		
    case bc::dup2_x1:
      retval = dup2_x1(e);
      break;		
    case bc::dup2_x2:
      retval = dup2_x2(e);
      break;		
    case bc::swap:
      retval = swap(e);
      break;
      // END OPS
	
    default:
      kprintf( "unknown bytecode %d, halting thread.\n", (int)cur_code->code[myPC]);
      return false;
      break;
    }
    
  /* advance the program counter */
  myPC++;
    
  return(retval);
}
  
#endif /* INTERPRETATION */

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
 * The Initial Developers of the Original Code are Todd Miller & John Morrison.
 * Portions created by John Morrison are Copyright (C) 1998.
 * All rights reserved.
 *
 * Contributor(s):
 *
 */

#include "java_object.h"
#include "java_thread.h"
#include "d_main.h"

#ifndef TARGET_UNIX
#include "jbmachin.h"
#include "jjconsole.h"
#else
#include <stdio.h>
#ifdef USE_CURSES
#include <curses.h>
#endif
#endif

#include "jbdebug.h"
static int debug = 0;

/* java.lang.VMClass */

jjBoolean java_lang_VMClassLoader_defineClass(java_word argument[], frame *f, resolved_method *rm, int n_args) {
    /* static defineClass( ClassLoader cl, String name, byte[] data,
     *                     int offset, int len ) */

    return JJFALSE;
    } /* end defineClass() */
    
jjBoolean java_lang_VMClassLoader_resolveClass(java_word argument[], frame *f, resolved_method *rm, int n_args) {
    /* static resolveClass(Class c) */

    return JJFALSE;
    } /* end resolveClass() */
    
jjBoolean java_lang_VMClassLoader_getPrimitiveClass(java_word argument[], frame *f, resolved_method *rm, int n_args) {
    /* static getPrimitiveClass(String class) */

#ifdef PARANOID
    if ( (java_object *)argument[0] == NULL ) {
        kprintf( "VMClassLoader.getPrimitiveClass: null argument string, aborting.\n" );
        return JJFALSE;
        }
#endif        

    /* fetch the type string. */
    java_string * js = (java_string *)(java_object *)argument[0];
    char * type_string = js->myUTF8->c_str();

    /* return a bogus class representing that primitive type */
    

    if ( strcmp( type_string, "float" ) == 0 ) {
        // f->op_stack.push_java_object( new java_object() );
        }
    else if ( strcmp( type_string, "int" ) == 0 ) {
	// f->op_stack.push_java_object( new java_object() );
        }
    else {
        kprintf( "VMClassLoader.getPrimitiveClass(): unknown primitive type %s.\n", type_string );            
        delete type_string;
        return JJFALSE;
        }

    delete type_string;
    return JJTRUE;
    } /* end getPrimitiveClass() */



/****************************************************************/


jjBoolean java_lang_Class_getPrimitiveClass(java_word argument[], frame* f, resolved_method *rm, int n_args) {
	kprintf( "FIXME: getPrimitiveClass()\n" );	
	f->op_stack.push_java_object( NULL );
	return JJTRUE;
	} /* end java_lang_Class_getPrimitiveClass() */

/*
 * print_string()
 *
 * arguments: java_word* parameters, constant_utf8* parameter string
 */

void print_string( java_word* arguments, constant_utf8* type )
{

#ifdef PARANOID
	if ( arguments == NULL || type == NULL ) {
		kprintf( "print_string(): [1] VM corrupted, exiting.\n" );
		abort();
		}
#endif

    /* determine type to convert from */
    char * str = type->c_str();

	if ( str == NULL ) {
		kprintf( "print_string(): [2] VM corrupted, exiting.\n" );
		abort();
		} 

    switch ( str[1] ) {		/* we only want the first argument's type */
      case 'D':
      case 'F':
		/* floating-point */
		kprintf( "!fixme: unable to print floating-point values!" );
		break;
		
      case 'J':
		/* longs are broken for now */
		kprintf( "!fixme: printing longs not implemented" );
		break;

      case 'B':
      case 'I':
      case 'S':
		kprintf( "%d", (jint)arguments[1] );
		break;
	
      case 'Z':
		if ( (jint)arguments[1] != 0 )
		{
	    	kprintf( "true" );
		}
		else
		{
		    kprintf( "false" );
		}
		break;

      case 'C':
		kprintf( "%c", (unsigned char)arguments[1] );
		break;

      case '[':
		/* should be a character array, only */
		kprintf( "!fixme: char[] printing!" );
		break;
	
      default:
		{
	    	java_object *foo = (java_object *)arguments[1];
	  		if ( foo == NULL ) {
	  			// kprintf( "print_string: [3] VM corrupted, exiting.\n" );
				kprintf( "[null java object]" );
	  			// abort();
				return;
	  			}
		    java_class *bar = foo->my_class;
	  		if ( bar == NULL ) {
	  			kprintf( "print_string: [4] VM corrupted, exiting.\n" );
	  			abort();
	  			}
		    constant_utf8 *quux = bar->class_name;
	  		if ( quux== NULL ) {
	  			kprintf( "print_string: [5] VM corrupted, exiting.\n" );
	  			abort();
	  			}
	    	if (*quux == constant_utf8("java/lang/String"))
		    {
				/* java_object *jo = (java_object *)(arguments[1]); */
				java_string *js = (java_string *)foo;
				if( js->myUTF8 != 0 ) {
					kprintf("%s", js->myUTF8->c_str()); // $$ Leaks.
					} else {
					kprintf( "null" );
					} /* end NULL check */
				break;
	    	}			/* end if a string */
		    else
		    { 
			/* if it's not a String */
			// kprintf( "!fixme: Object.toString() unimplemented!" );
			kprintf( "[java object at %x]", foo );
		    }
		}
		break;
    } /* end switch */
} /* end print_string() */

/*
 *
 */

jjBoolean java_lang_System_arraycopy(java_word argument[], frame *f, resolved_method *rm, int n_args) {
	/* arguments: ( Object src, int offset, Object dest, int pos, int length ) */
	java_object * src = (java_object *)argument[0];
	jju32 offset = (jint)argument[1];
	java_object * dest = (java_object *)argument[2];
	jju32 pos = (jint)argument[3];
	jju32 length = (jint)argument[4];

	if ( src->type() != typed_data::array ) {
		kprintf( "ArrayStoreException should be thrown:" );
		kprintf( "source object is not an array.\n" );
		return JJFALSE;
		}

	if ( dest->type() != typed_data::array ) {
		kprintf( "ArrayStoreException should be thrown:\n" );
		kprintf( "destination object is not an array.\n" );
		return JJFALSE;
		}

	/* load & store are virtual, so this will do the Right Thing. */
	java_jint_array * src_array = (java_jint_array*)src;	
	java_jint_array * dest_array = (java_jint_array*)dest;

#ifdef UNDEF
	/* special case for mapped or byte arrays because integer
	   conversion blows them away */
	if ( src_array->array_type() == java_jint_array::T_MAPPED ||
	     src_array->array_type() == java_jint_array::T_BYTE ||
	     dest_array->array_type() == java_jint_array::T_MAPPED ||
	     dest_array->array_type() == java_jint_array::T_BYTE ) {
		java_mapped_array * src_a = (java_mapped_array*)src;
		java_mapped_array * dst_a = (java_mapped_array*)dest;
	
		for( jju32 x = offset; x < offset + length; x++ ) {
			dst_a->store( x, src_a->load( pos + x ) );
			} /* end copy loop */
		} /* end if */
#endif


	jju32 count = 0;
	int x = pos;
	int y = offset;
	while ( count < length ) {
		dest_array->store( x, src_array->load( y ) );
		x++; y++; count++;
		} /* end transfer loop */

#ifdef UNDEF
	// This can't be right, because offset is never used.
	for( jju32 x = pos; x < pos + length; x++ ) {
		dest_array->store( x, src_array->load( pos + x ) );
		} /* end copy loop */
#endif

	return JJTRUE;	
	} /* end arraycopy() */

jjBoolean java_lang_System_currentTimeMillis(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(("java_lang_System_currentTimeMillis\n"));
  f->op_stack.push_jlong((jlong)100);
  return(JJTRUE);
}

jjBoolean java_io_PrintStream_print(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(("java_io_PrintStream_print\n"));
  print_string(argument, rm->descPtr());
  return(JJTRUE);
}

jjBoolean java_io_PrintStream_println(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(("java_io_PrintStream_println\n"));
  print_string(argument, rm->descPtr());
  kprintf("\n");
  return(JJTRUE);
}

jjBoolean java_lang_Throwable_fillInStackTrace(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(( "java_lang_Throwable_fillInStackTrace -- not filling in stack trace...\n" ));
  /* need to return a dummy Throwable... */
  f->op_stack.push_java_object( new java_object(NULL) );
  return(JJTRUE);
}

jjBoolean java_lang_Object_notify(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(( "notify()ing thread...\n" ));
  /*
   * notify() returns false if it threw an exception,
   * and true otherwise
   */
  if (((java_object *)argument[0])->notify( f->myThread ))
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

jjBoolean java_lang_Object_notifyAll(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(( "notifyAll() thread...\n" ));
  /*
   * notifyAll() returns false if it threw an exception,
   * and true otherwise
   */
  if (((java_object *)argument[0])->notifyAll( f->myThread ))
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

jjBoolean java_lang_Object_wait(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  DEBUG(("{{{myPC = %x}}}\n", f->myPC));
  DEBUG(( "wait()ing thread...\n" ));
  /*
   * wait() returns true if it threw an exception,
   * false if the thread should terminate
   */ 
  if (((java_object *)argument[0])->wait(f->myThread))
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}

jjBoolean java_lang_Object_start(java_word argument[], frame *f0, resolved_method *rm, int n_args)
{
  java_class* jc = ((java_object *)argument[0])->my_class;

  jc->doLateResolutionIfNecessary(f0->myThread->classLoader(), f0->myThread);
  jc->doStaticInitializationIfNecessary();

  frame* f = jc->method( constant_utf8("run").string(), NULL, argument, n_args); 
	
  if (f == NULL)
    {
      kprintf("unable to spawn thread: no run() method\n");
      return(JJFALSE);
    }
  else
    {
      java_thread* jt = new java_thread( f, f0->myThread->classLoader(), 1 );
      DEBUG(( "start(): %X\n", jt ));
      f->setMyThread( jt );
      jvm->myscheduler()->addThread( jt );
      return(JJTRUE);
    }
}

jjBoolean java_lang_Object_stop(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  /*
   * doing this is going to be at best
   * problematic w/o a native Thread class,
   * because Thread/Runnable object needs
   * to store the thread it's in...
   */
  kprintf( "fixme: stop() unimplemented.\n" );
  return(JJFALSE);
}

#ifndef TARGET_UNIX
jjBoolean jos_system_machine_in8(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  f->op_stack.push_jju32( jvm->jjm()->in8( (jju16)argument[0] ) );
  return(JJTRUE);
}

jjBoolean jos_system_machine_in16(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  f->op_stack.push_jju32( jvm->jjm()->in16( (jju16)argument[0] ) );
  return(JJTRUE);
}

jjBoolean jos_system_machine_in32(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  f->op_stack.push_jju32( jvm->jjm()->in32( (jju16)argument[0] ) );
  return(JJTRUE);
}

jjBoolean jos_system_machine_out8(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jvm->jjm()->out8( (jju16)argument[0], (jju8)argument[1] );
  return(JJTRUE);
}

jjBoolean jos_system_machine_out16(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jvm->jjm()->out16( (jju16)argument[0], (jju16)argument[1] );
  return(JJTRUE);
}

jjBoolean jos_system_machine_out32(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jvm->jjm()->out32( (jju16)argument[0], (jju32)argument[1] );
  return(JJTRUE);
}

jjBoolean jos_system_machine_read8(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jju32 tmp = jaddress.myLow32;
  jju8 *ptr = (jju8 *)tmp;
  jju16 jretval = *ptr;
  DEBUG(("%d read8(%X:%X = %X)\n",
	 jretval,
	 jaddress.myHigh32, jaddress.myLow32,
	 ptr));
  f->op_stack.push_jshort(jretval);	
  return(JJTRUE);
}

jjBoolean jos_system_machine_write8(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jshort jvalue = (jshort)argument[2];
  jju32 tmp = jaddress.myLow32;
  jju8 *ptr = (jju8 *)tmp;
  DEBUG(("write8(%X:%X = %X, %d)\n",
	 jaddress.myHigh32, jaddress.myLow32,
	 ptr,
	 jvalue));
  *ptr = jvalue;
  f->op_stack.push_jshort(jvalue);	
  return(JJTRUE);
}

jjBoolean jos_system_machine_read16(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jju32 tmp = jaddress.myLow32;
  jju16 *ptr = (jju16 *)tmp;
  jju32 jretval = *ptr;
  DEBUG(("%d read16(%X:%X = %X)\n",
	 jretval,
	 jaddress.myHigh32, jaddress.myLow32,
	 ptr));
  f->op_stack.push_jint(jretval);	
  return(JJTRUE);
}


jjBoolean jos_system_machine_write16(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jint jvalue = (jint)argument[2];
  jju32 tmp = jaddress.myLow32;
  jju16 *ptr = (jju16 *)tmp;
  DEBUG(("write16(%X:%X = %X, %d)\n",
	 jaddress.myHigh32, jaddress.myLow32,
	 ptr,
	 jvalue));
  jju16 foobar = (jju16)jvalue;
  *ptr = foobar;
  f->op_stack.push_jint(jvalue);	
  return(JJTRUE);
}


jjBoolean jos_system_machine_read32(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jju32 tmp = jaddress.myLow32;
  jju32 *ptr = (jju32 *)tmp;
  jlong jretval = *ptr;
  DEBUG(("%X:%X read32(%X:%X = %X)\n",
	 jretval.myHigh32, jretval.myLow32,
	 jaddress.myHigh32, jaddress.myLow32,
	 ptr));
  f->op_stack.push_jlong(jretval);	
  return(JJTRUE);
}


jjBoolean jos_system_machine_write32(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  int debug = 1;
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jlong jvalue((jju32)argument[2], (jju32)argument[3]);
  jju32 tmp = jaddress.myLow32;
  jju32 *ptr = (jju32 *)tmp;
  DEBUG(("write32(%X:%X = %X, %X:%X)\n",
	 jaddress.myHigh32, jaddress.myLow32,
	 ptr,
	 jvalue.myHigh32, jvalue.myLow32));
  *ptr = jvalue.myLow32;
  f->op_stack.push_jlong(jvalue);	
  return(JJTRUE);
}

jjBoolean jos_system_machine_mapByteArray(java_word argument[], frame *f, resolved_method *rm, int n_args) {
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jlong jsize((jju32)argument[2], (jju32)argument[3]);

  java_class * jc = f->myThread->classLoader()->getClass( constant_utf8("java/lang/Object").string(), f->myThread );

  java_mapped_array * jmap = new java_mapped_array( jc, jaddress.myLow32, jsize.myLow32 );
  DEBUG(( "mapped at %X for %X.\n", jaddress.myLow32, jsize.myLow32 )); 
  f->op_stack.push_java_object( (java_object*)jmap );

  return (JJTRUE);
} /* end mapByteArray */

jjBoolean jos_platform_driver_consoleNative_setVisible(java_word argument[], frame *f, resolved_method *rm, int n_args){
   jjBoolean v = (jjBoolean)(jint)argument[1];
   jbmp->myConsole->setVisible(v);
   return (JJTRUE);
   }
 
jjBoolean jos_platform_driver_consoleNative_putChar(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jju8 ch = (jju8)(argument[1]);
   jbmp->myConsole->putChar(ch);
   return (JJTRUE);	
   }
 
jjBoolean jos_platform_driver_consoleNative_setChar(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];
   jju8 c = (jju8) argument[3];
   
   jbmp->myConsole->setChar(row, col, c);
   return (JJTRUE);	
   } 

jjBoolean jos_platform_driver_consoleNative_getChar(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];

   /* $: check conversion. */
   f->op_stack.push_jint( (jint)jbmp->myConsole->getChar(row, col) );
   return (JJTRUE);
   }

jjBoolean jos_platform_driver_consoleNative_setAttr(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];
   jju8 a = (jju8) argument[3];
   
   jbmp->myConsole->setAttr(row, col, a);
   return (JJTRUE);	
   } 

jjBoolean jos_platform_driver_consoleNative_getAttr(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];
   
   /* $: check conversion. */
   f->op_stack.push_jint( (jint)jbmp->myConsole->getAttr(row, col) );
   return (JJTRUE);	
   } 

jjBoolean jos_platform_driver_consoleNative_setCursor(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];

   jbmp->myConsole->setCursor( row, col );
   return (JJTRUE);
   }

#else

/* initial state for the keyboard emulation */

#define KBD_INIT 0
#define KBD_READING 1
#define KBD_READ 2
#define KBD_READY 3

int kbd_state = KBD_INIT;
int read_char;

jjBoolean host_machine_in8(java_word argument[], frame *f, resolved_method *rm, int n_args)
{
  int debug = 0;

  /* fetch the port */
  jju32 port = (jju32)argument[0];

#ifdef USE_CURSES
	switch ( port ) {
		case 0x64:
			read_char = getch();
			if ( read_char != ERR ) {
				f->op_stack.push_jju32(	0x01 );	
				}
			else { f->op_stack.push_jju32( 0x00 ); }
			break;
		case 0x60:
			f->op_stack.push_jju32( read_char );
			break;
		default:
			f->op_stack.push_jju32( 0x0 );
			break;
		} /* end switch */
	return JJTRUE;
#endif

  /* emulate the keyboard */
  switch ( port ) {
	case 0x64:
		/* keyboard status */
		switch ( kbd_state ) {
			case KBD_INIT:
				DEBUG(( "in8( 64 ): 0x01\n" ));
				f->op_stack.push_jju32( 0x01 );
				kbd_state = KBD_READING;
				break;
			case KBD_READ:
				DEBUG(( "in8( 64 ): 0x00\n" ));
				f->op_stack.push_jju32( 0x00 );
				kbd_state = KBD_READY;
				break;
			case KBD_READY:
				DEBUG(( "in8( 64 ): 0x04\n" ));
				f->op_stack.push_jju32( 0x04 );
				kbd_state = KBD_INIT;
				break;
			default:
				DEBUG(( "in8( 64 ) [fixme: default]: 0x01\n" ));
				f->op_stack.push_jju32( 0x01 );
				break;
			} /* end switch */
		break;
	case 0x60:
		{
		/* keyboard scancode $: is just character for now. */
		int c = fgetc(stdin);
		DEBUG(( "in8( 60 ): pushing scancode %d\n", c ));
		f->op_stack.push_jju32( c );

		kbd_state = KBD_READ;
		break;
		}
	default:
		// kprintf( "host_machine_in8( %d ): emulation unavailable, pushing 0x0.\n", port );	
		f->op_stack.push_jju32( 0x0 );
		return JJTRUE;
	} /* end switch */  		

  return(JJTRUE);
} /* end host_machine_in8 */

jjBoolean host_machine_out8(java_word argument[], frame *f, resolved_method *rm, int n_args) {
 return(JJTRUE);
} /* end host_machine_out8 */

jjBoolean host_machine_write8(java_word argument[], frame *f, resolved_method *rm, int n_args) {
 /* return the value written */
 f->op_stack.push_jju32( (jju32)argument[0] );
 return(JJTRUE);
} /* end host_machine_write8 */

jjBoolean host_machine_read8(java_word argument[], frame *f, resolved_method *rm, int n_args) {
 f->op_stack.push_jju32( 0x0 );
 return(JJTRUE);
} /* end host_machine_read8 */

jjBoolean jos_system_machine_mapByteArray(java_word argument[], frame *f, resolved_method *rm, int n_args) {
  jlong jaddress((jju32)argument[0], (jju32)argument[1]);
  jlong jsize((jju32)argument[2], (jju32)argument[3]);

  java_class * jc = f->myThread->classLoader()->getClass( constant_utf8("java/lang/Object").string(), f->myThread );

  java_jint_array * jmap = new java_jint_array( jc, jsize.myLow32, java_jint_array::T_BYTE );
  f->op_stack.push_java_object( (java_object*)jmap );

  return (JJTRUE);
} /* end mapByteArray */

jjBoolean jos_platform_driver_console_setCursor(java_word argument[], frame *f, resolved_method *rm, int n_args) { 
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];
   
   jbmp->myConsole->setCursor(row, col);
   return (JJTRUE);	
   }

jjBoolean jos_platform_driver_console_putChar(java_word argument[], frame *f, resolved_method *rm, int n_args) {
  jju8 c = (jju8) argument[1];
  
  jbmp->myConsole->putChar( c );
  return (JJTRUE);
  }

jjBoolean jos_platform_driver_console_setNativeAttr(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];
   jju8 a = (jju8) argument[3];
   
   jbmp->myConsole->setAttr(row, col, a);
   return (JJTRUE);	
   }

jjBoolean jos_platform_driver_console_setNativeChar(java_word argument[], frame *f, resolved_method *rm, int n_args) {
   jint row = (jint) argument[1];
   jint col = (jint) argument[2];
   jju8 c = (jju8) argument[3];
   
   jbmp->myConsole->setChar(row, col, c);
   return (JJTRUE);	
   }

#endif

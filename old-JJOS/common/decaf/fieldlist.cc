/*
 * fieldlist.cc
 * 
 * defines the C++ class
 * representing a list of
 * fields in a Java class.
 */

#include "fieldlist.h"

FieldList * FieldList::generateFieldList( istream & is, JavaClass * jc, FieldList * parent ) {
    return RawFieldList::generateRawFieldList( is, jc, parent );
    } /* end generateFieldList */


FieldInfo * RawFieldList::operator [] ( int index ) {
    if ( index >= 0 && index < myFieldCount ) {
        if ( myFieldInfoList != NULL ) {
            return myFieldInfoList[index];
            } else {
            kprintf( "FieldInfo::operator []() -- myFieldInfoList is NULL, aborting.\n" );
            abort();
            }
        } else {
        kprintf( "FieldInfo::operator []() -- array index out of bounds, aborting.\n" );
        abort();
        } 
     /* should never happen */
     return NULL;
     } /* end operator []() */

FieldList * RawFieldList::append( FieldInfo * fi ) {
	/* generate a clean FieldList */
	RawFieldList * rfl = new RawFieldList();
	    if ( rfl == NULL ) {
        kprintf( "RawFieldList::append() -- unable to allocate rfl, aborting.\n" );
        abort();
        }

	/* allocate myFieldInfoList */
	rfl->myFieldInfoList = new FieldInfo * [this->myFieldCount + 1];
    if ( rfl->myFieldInfoList == NULL ) {
        kprintf( "RawFieldList::append() -- unable to allocate myFieldInfoList, aborting.\n" );
        abort();
        }
	
	/* fill in myFieldInfoList */
	jju16 x = 0;
	for ( x = 0; x < this->myFieldCount; x++ ) {
		rfl->myFieldInfoList[x] = this->myFieldInfoList[x];
		}

	/* append the new field info */
	rfl->myFieldInfoList[x] = fi;
	return rfl;
	} /* end append() */

FieldList * RawFieldList::generateRawFieldList( istream & is, JavaClass * jc, FieldList * parent ) {
    /* generate a clean FieldList */
    RawFieldList * rfl = new RawFieldList();
    if ( rfl == NULL ) {
        kprintf( "RawFieldList::generateRawFieldList() -- unable to allocate rfl, aborting.\n" );
        abort();
        }
    
    /* allocate myFieldInfoList */
    is >> rfl->myFieldCount;
	jju32 parentCount = 0;

	/* do I have a parent? */	
    if ( parent != NULL ) {
    	/* if I do, and I don't add any fields,
    	 * save some time and return my parent. */
		if ( rfl->myFieldCount == 0 ) {
			return parent;
			}
			
		parentCount = parent->getMyFieldCount();

    	/* otherwise, adjust my field count */
		rfl->myFieldCount += parentCount;
		}
    
    rfl->myFieldInfoList = new FieldInfo * [rfl->myFieldCount];
    if ( rfl->myFieldInfoList == NULL ) {
        kprintf( "RawFieldList::generateRawFieldList() -- unable to allocate myFieldInfoList, aborting.\n" );
        abort();
        }

    /* fill myFieldInfoList in */
	if ( parent != NULL ) {
		/* if I have a parent, copy its fields in. */
		for ( jju16 x = 0; x < parentCount; x++ ) {
			rfl->myFieldInfoList[x] = (*parent)[x];
			}
		}

   	/* fill my own fields in from the class file */
    for ( jju16 x = parentCount; x < rfl->myFieldCount; x++ ) {
        rfl->myFieldInfoList[x] = FieldInfo::generateFieldInfo( is, jc );
        }

	rfl->myFieldCount += parentCount;        
    return rfl;
    } /* end generateRawFieldList() */

FieldInfo * FieldInfo::generateFieldInfo( istream & is, JavaClass * jc ) {
    return RawFieldInfo::generateRawFieldInfo( is, jc );
    } /* end generateFieldInfo */
 
RawFieldInfo * RawFieldInfo::generateRawFieldInfo( istream & is, JavaClass * jc ) {
    /* generate a clean FieldInfo */
    RawFieldInfo * rfi = new RawFieldInfo();
    if ( rfi == NULL ) {
        kprintf( "RawFieldInfo::generateRawFieldInfo() -- unable to allocate rfi, aborting.\n" );
        abort();
        }

    is >> rfi->myAccessFlags;
    is >> rfi->myNameIndex >> rfi->myDescriptorIndex;

    rfi->myAttributeList = AttributeList::generateAttributeList( is, jc );
    return rfi;
    } /* end generateRawFieldInfo() */

jju32 ClassFields::getFieldIndex( JavaString * name, JavaString * type ) {
    if ( name == NULL || type == NULL ) {
        kprintf( "JavaClassInstance::getFieldIndex() -- NULL in name/type pair, aborting.\n" );
        abort();
        }

    for ( jint x = 0; x < fl->getMyFieldCount(); x++ ) {
        if ( * (*fl)[x]->getMyName(cp) == * name &&
             * (*fl)[x]->getMyDescriptor(cp) == * type ) {
            return x;
            }
        }
        
    kprintf( "JavaClassInstance::getFieldIndex( %s, %s ) -- no such field, aborting.\n",
                name->c_str(), type->c_str() );
    abort();
    /* shut up compiler */
    return 0;
    } /* end getFieldIndex() */

ClassFields::ClassFields( FieldList * fl, ConstantPool * cp ) {
	/* store our context */
    this->fl = fl;
    this->cp = cp;
    
    /* generate the field array */
    this->fd = new FieldData[ fl->getMyFieldCount() ];
    if ( this->fd == NULL ) {
        kprintf( "ClassFields::ClassFields() -- unable to allocate field data, aborting.\n" );
        abort();
        }
    /* fill in the access flags & initial values of the field array */
    for ( jint x = 0; x < fl->getMyFieldCount(); x++ ) {
        fd[x].flags = ((*fl)[x])->getMyAccessFlags();
        if ( ((*fl)[x])->hasInitialValue(cp) ) {
            /* Fetch the constant value. */
            CPEntry * cpe = (*cp)[((*fl)[x])->getMyValueIndex(cp)];

    	    /* Verify that it's the right type. */
    	    char flc = (*(((*fl)[x])->getMyDescriptor(cp)))[0];
    	    
    	    switch( cpe->type() ) {
    	    
                case TAG_INTEGER:
                    if ( flc == 'B' || flc == 'C' || flc == 'I' || flc == 'S' || flc == 'Z' ) {
#ifdef DONT_USE_DC
    	                fd[x].value = ((CPInteger*)cpe)->getMyInteger();
#else
                        /* if a .type()-checked conversion like this fails,
                         * we've got bigger problems than a NULL pointer... */
                        fd[x].value = (dynamic_cast<CPInteger*>(cpe))->getMyInteger();   	                
#endif                        
    	                } else {
    	                kprintf( "ClassFields::ClassFields() -- attempt to initialize integral field with non-integral constant, aborting\n" );
    	                abort();
    	                }
    	            break;

                case TAG_LONG:
                    if ( flc == 'J' ) {
#ifdef DONT_USE_DC                    
                        fd[x].value = (jju32)(new jlong( ((CPLong*)cpe)->getMyLong() ));
#else
                        /* if a .type()-checked conversion like this fails,
                         * we've got bigger problems than a NULL pointer... */
                        fd[x].value = (jju32)(new jlong( (dynamic_cast<CPLong*>(cpe))->getMyLong() ));
#endif                        
                        } else {
    	                kprintf( "ClassFields::ClassFields() -- attempt to initialize long field with non-long constant, aborting\n" );
    	                abort();
    	                }
    	            break;

                case TAG_FLOAT:
                    if ( flc == 'F' ) {
#ifdef DONT_USE_DC                    
                        fd[x].value = ((CPFloat*)cpe)->getMyFloat();
#else
                        /* if a .type()-checked conversion like this fails,
                         * we've got bigger problems than a NULL pointer... */
                        fd[x].value = (dynamic_cast<CPFloat*>(cpe))->getMyFloat();
#endif                        
                        } else {
    	                kprintf( "ClassFields::ClassFields() -- attempt to initialize float field with non-float constant, aborting\n" );
    	                abort();
    	                }
    	            break;

                case TAG_DOUBLE:
                    if ( flc == 'D' ) {
#ifdef DONT_USE_DC                    
                        fd[x].value = (jju32)(new jdouble( ((CPDouble*)cpe)->getMyDouble() ));
#else
                        /* if a .type()-checked conversion like this fails,
                         * we've got bigger problems than a NULL pointer... */
                        fd[x].value = (jju32)(new jdouble( (dynamic_cast<CPDouble*>(cpe))->getMyDouble() ));
#endif                        
                        } else {
    	                kprintf( "ClassFields::ClassFields() -- attempt to initialize double field with non-double constant, aborting\n" );
    	                abort();
    	                }
    	            break;

                case TAG_STRING:
                    if ( flc == 'L' && * ((*fl)[x])->getMyDescriptor(cp) == JavaString( "java/lang/String" ) /* intern? */ ) {
#ifdef DONT_USE_DC
                        fd[x].value = (jju32)((CPString*)cpe)->getMyJavaString();
#else
                        fd[x].value = (jju32)(dynamic_cast<CPString*>(cpe))->getMyJavaString();
#endif                                                
                        } else {
    	                kprintf( "ClassFields::ClassFields() -- attempt to initialize string field with non-string constant, aborting\n" );
    	                abort();
    	                }
    	            break;

    	        default:
    	            kprintf( "ClassFields::ClassFields() -- illegal constant value type in field list, aborting.\n" );
    	            abort();
    	            break;
    	            
                } /* end switch */
            } else {
 	    fd[x].value = 0;
	    }
        }
    }

JavaWord ClassFields::getClassField( jju32 index ) {
    if ( index < fl->getMyFieldCount() ) {
        if ( !( fd[index].flags & ACC_STATIC ) ) {
            kprintf( "ClassFields::getClassField() -- requested field not a class field, aborting.\n" );
            abort();
            }
        return fd[index].value;
        } else {
        kprintf( "ClassFields::getClassField() -- index out of bounds, aborting.\n" );
        abort();
        }
    /* avoids compiler warning */
    return JavaWord();        
    }
    
void ClassFields::setClassField( jju32 index, JavaWord value ) {
    if ( index < fl->getMyFieldCount() ) {
        if ( (!( fd[index].flags & ACC_STATIC )) || ( fd[index].flags & ACC_FINAL ) ) {
            kprintf( "ClassFields::setClassField() -- requested field not a class field or is final, aborting.\n" );
            abort();
            }
        fd[index].value = value;
        } else {
        kprintf( "ClassFields::setClassField() -- index out of bounds, aborting.\n" );
        abort();
        }
    }

JavaWord InstanceFields::getInstanceField( jju32 index ) {
    if ( index < fl->getMyFieldCount() ) {
        if ( fd[index].flags & ACC_STATIC ) {
            kprintf( "InstanceFields::getInstanceField() -- requested field not an instance field, aborting.\n" );
            abort();
            }
        return fd[index].value;
        } else {
        kprintf( "InstanceFields::getInstanceField() -- index out of bounds, aborting.\n" );
        abort();
        }
    /* avoids compiler warning */
    return JavaWord();
    }

void InstanceFields::setInstanceField( jju32 index, JavaWord value ) {
    if ( index < fl->getMyFieldCount() ) {
        if ( (fd[index].flags & ACC_STATIC) || ( fd[index].flags & ACC_FINAL ) ) {
            kprintf( "InstanceFields::setInstanceField() -- requested field not an instance field or is final, aborting.\n" );
            abort();
            }
        fd[index].value = value;
        } else {
        kprintf( "InstanceFields::setInstanceField() -- index out of bounds, aborting.\n" );
        abort();
        }
    }

bool matchDescriptorAndPrimitiveType( JavaString * desc, PrimitiveType pt ) {
    if ( desc == NULL ) {
        kprintf( "JavaClassInstance::matchDescriptorAndPrimitiveType() -- attempt to match NULL descriptor, aborting.\n" );
        abort();
        }

    /* does desc describe a field of the type pt? */
    switch ( (char)((*desc)[0]) ) {
        case 'B':
            /* byte */
            return pt == PT_BYTE;
            break;

        case 'C':
            /* char */
            return pt == PT_CHAR;
            break;

        case 'D':
            /* double */
            return pt == PT_DOUBLE;
            break;

        case 'F':
            /* float */
            return pt == PT_FLOAT;
            break;

        case 'I':
            /* int */
            return pt == PT_INT;
            break;
         
        case 'J':
            /* long */
            return pt == PT_LONG;
            break;

        case 'L':
            /* class name */
            return pt == PT_CLASS_INSTANCE;
            break;
            
        case 'S':
            /* short */
            return pt == PT_SHORT;
            break;

        case 'Z':
            /* boolean */
            return pt == PT_BOOLEAN;
            break;

        case '[':
            /* array (dimension) */
            return pt == PT_ARRAY;
            break;

        default:
            kprintf( "JavaClassInstance::matchDescriptorAndPrimitiveType() -- unrecognized character in descriptor, aborting.\n" );
            abort();
            /* shut up the compiler */
            return false;
        }
    } /* end mDAP() */

JavaString * RawFieldInfo::getMyName( ConstantPool * cp ) {
	// should this be cached?
    // if ( myName != NULL ) { return myName; }

	JavaString * myName = NULL;
    
#ifdef DONT_USE_DC
    CPEntry * cpe = (*cp)[myNameIndex];
    if ( cpe->type() != TAG_UTF8 ) {
        kprintf( "RawFieldInfo::getMyName() -- erroneous constant pool entry type (%d), aborting.\n", cpe->type() );
        abort();
        }
    myName = ((ConstantUTF8*)cpe)->getMyJavaString();
#else
    if ( ConstantUTF8 * cu8 = dynamic_cast<ConstantUTF8*>( (*cp)[myNameIndex] ) ) {
        myName = cu8->getMyJavaString();
        } else {
        kprintf( "RawFieldInfo::getMyName() -- erroneous constant pool entry type (%d), aborting.\n", ((*cp)[myNameIndex])->type() );
        abort();
        }        
#endif        

    return myName;
	}

JavaString * RawFieldInfo::getMyDescriptor( ConstantPool * cp ) {
	// should this be cached?
    // if ( myType != NULL ) { return myName; }

	JavaString * myType = NULL;
    
#ifdef DONT_USE_DC
    CPEntry * cpe = (*cp)[myDescriptorIndex];
    if ( cpe->type() != TAG_UTF8 ) {
        kprintf( "RawFieldInfo::getMyDescriptor() -- erroneous constant pool entry type (%d), aborting.\n", cpe->type() );
        abort();
        }
    myType = ((ConstantUTF8*)cpe)->getMyJavaString();
#else
    if ( ConstantUTF8 * cu8 = dynamic_cast<ConstantUTF8*>( (*cp)[myNameIndex] ) ) {
        myType = cu8->getMyJavaString();
        } else {
        kprintf( "RawFieldInfo::getMyDescriptor() -- erroneous constant pool entry type (%d), aborting.\n", ((*cp)[myNameIndex])->type() );
        abort();
        }        
#endif        

    return myType;
	}

bool RawFieldInfo::hasInitialValue( ConstantPool * cp ) {
	return ( findMyCV( cp ) != NULL );
	}

jju16 RawFieldInfo::getMyValueIndex( ConstantPool * cp ) {
	return findMyCV( cp )->getMyConstantValueIndex();
	}

RawConstantValueAttribute * RawFieldInfo::findMyCV( ConstantPool * cp ) {
	/* iterate over elements in my attribute list, looking for the "ConstantValue" */
    for ( int x = 0; x < myAttributeList->getMyAttributeCount(); x++ ) {
        if ( * ((*myAttributeList)[x]->getMyName( cp )) == JavaString("ConstantValue") ) { /* intern! */
#ifdef DONT_USE_DC            
            return (RawConstantValueAttribute*)(*myAttributeList)[x];
#else
            return dynamic_cast<RawConstantValueAttribute*>((*myAttributeList)[x]);
#endif                        
            }
        } /* end attribute list iteration */
    return NULL;
	}


/* the type-checking accessors */

JavaClassInstance * ClassFields::getStaticObject( jju32 index ) {
    if ( index < fl->getMyFieldCount() ) {
        if ( !( fd[index].flags & ACC_STATIC ) ) {
            kprintf( "ClassFields::getClassField() -- requested field not a class field, aborting.\n" );
            abort();
            }

		/* verify that the field at that location is a JCI */
		if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_CLASS_INSTANCE ) ) {
			kprintf( "ClassFields::getClassField() -- requested field not an object, aborting.\n" );
			abort();
			}

        return fd[index].value;
        } else {
        kprintf( "ClassFields::getClassField() -- index out of bounds, aborting.\n" );
        abort();
        }
        
    /* avoids compiler warning */
    return NULL;
	}

jbool ClassFields::getStaticBoolean( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_BOOLEAN ) ) {
		kprintf( "ClassFields::getStaticBoolean() -- requested field not a boolean, aborting.\n" );
		abort();
		}
	/* would operator jbool work with static_cast<>? */
	return (jbool)jw;
	}

jbyte ClassFields::getStaticByte( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_BYTE ) ) {
		kprintf( "ClassFields::getStaticByte() -- requested field not a byte, aborting.\n" );
		abort();
		}
	/* would operator jbyte work with static_cast<>? */
	return (jbyte)jw;
	}

jshort ClassFields::getStaticShort( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_SHORT ) ) {
		kprintf( "ClassFields::getStaticShort() -- requested field not a short, aborting.\n" );
		abort();
		}
	/* would operator jshort work with static_cast<>? */
	return (jshort)jw;
	}

jint ClassFields::getStaticInt( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_INT ) ) {
		kprintf( "ClassFields::getStaticInt() -- requested field not an int, aborting.\n" );
		abort();
		}
	/* would operator jint work with static_cast<>? */
	return (jint)jw;
	}

jlong ClassFields::getStaticLong( jju32 index ) {
	if ( index < fl->getMyFieldCount() && (fd[index].flags & ACC_STATIC) ) {
		if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_LONG ) ) {
			kprintf( "ClassFields::getStaticLong() -- requested field not a long, aborting.\n" );
			abort();
			}
		/* would operator jju32 work with static_cast<>? */
		return *(jlong *)(jju32)( fd[index].value );
		}

	kprintf( "ClassFields::getStaticLong() -- requested field not static or index invalid, aborting.\n" );
	abort();

	/* should never happen */
	return jlong(0);
	}

jfloat ClassFields::getStaticFloat( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_FLOAT ) ) {
		kprintf( "ClassFields::getStaticFloat() -- requested field not a float, aborting.\n" );
		abort();
		}
	/* would operator jfloat work with static_cast<>? */
	return (jfloat)jw;
	}

jdouble ClassFields::getStaticDouble( jju32 index ) {
	if ( index < fl->getMyFieldCount() && (fd[index].flags & ACC_STATIC) ) {
		if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_DOUBLE ) ) {
			kprintf( "ClassFields::getStaticDouble() -- requested field not a double, aborting.\n" );
			abort();
			}
		/* would operator jju32 work with static_cast<>? */
		return *(jdouble *)(jju32)( fd[index].value );
		}
		
	kprintf( "ClassFields::getStaticDouble() -- requested field not static or index invalid, aborting.\n" );
	abort();
	
	/* should never happen */
	return jdouble(0);
	}

/* type-checked accessors for InstanceFields */

JavaClassInstance * InstanceFields::getInstanceObject( jju32 index ) {
    if ( index < fl->getMyFieldCount() ) {
        if ( !( fd[index].flags & ACC_STATIC ) ) {
            kprintf( "InstanceFields::getClassField() -- requested field not a class field, aborting.\n" );
            abort();
            }

		/* verify that the field at that location is a JCI */
		if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_CLASS_INSTANCE ) ) {
			kprintf( "InstanceFields::getClassField() -- requested field not an object, aborting.\n" );
			abort();
			}

        return fd[index].value;
        } else {
        kprintf( "InstanceFields::getClassField() -- index out of bounds, aborting.\n" );
        abort();
        }
        
    /* avoids compiler warning */
    return NULL;
	}

jbool InstanceFields::getInstanceBoolean( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_BOOLEAN ) ) {
		kprintf( "InstanceFields::getInstanceBoolean() -- requested field not a boolean, aborting.\n" );
		abort();
		}
	/* would operator jbool work with static_cast<>? */
	return (jbool)jw;
	}

jbyte InstanceFields::getInstanceByte( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_BYTE ) ) {
		kprintf( "InstanceFields::getInstanceByte() -- requested field not a byte, aborting.\n" );
		abort();
		}
	/* would operator jbyte work with static_cast<>? */
	return (jbyte)jw;
	}

jshort InstanceFields::getInstanceShort( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_SHORT ) ) {
		kprintf( "InstanceFields::getInstanceShort() -- requested field not a short, aborting.\n" );
		abort();
		}
	/* would operator jshort work with static_cast<>? */
	return (jshort)jw;
	}

jint InstanceFields::getInstanceInt( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_INT ) ) {
		kprintf( "InstanceFields::getInstanceInt() -- requested field not an int, aborting.\n" );
		abort();
		}
	/* would operator jint work with static_cast<>? */
	return (jint)jw;
	}

jlong InstanceFields::getInstanceLong( jju32 index ) {
	if ( index < fl->getMyFieldCount() && ! (fd[index].flags & ACC_STATIC) ) {
		if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_LONG ) ) {
			kprintf( "InstanceFields::getInstanceLong() -- requested field not a long, aborting.\n" );
			abort();
			}
		/* would operator jju32 work with static_cast<>? */
		return *(jlong *)(jju32)( fd[index].value );
		}

	kprintf( "InstanceFields::getInstanceLong() -- requested field not static or index invalid, aborting.\n" );
	abort();

	/* should never happen */
	return jlong(0);
	}

jfloat InstanceFields::getInstanceFloat( jju32 index ) {
	JavaWord jw = getClassField( index );
	if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_FLOAT ) ) {
		kprintf( "InstanceFields::getInstanceFloat() -- requested field not a float, aborting.\n" );
		abort();
		}
	/* would operator jfloat work with static_cast<>? */
	return (jfloat)jw;
	}

jdouble InstanceFields::getInstanceDouble( jju32 index ) {
	if ( index < fl->getMyFieldCount() && ! (fd[index].flags & ACC_STATIC) ) {
		if ( ! matchDescriptorAndPrimitiveType( ((*fl)[index])->getMyDescriptor( cp ), PT_DOUBLE ) ) {
			kprintf( "InstanceFields::getInstanceDouble() -- requested field not a double, aborting.\n" );
			abort();
			}
		/* would operator jju32 work with static_cast<>? */
		return *(jdouble *)(jju32)( fd[index].value );
		}
		
	kprintf( "InstanceFields::getInstanceDouble() -- requested field not static or index invalid, aborting.\n" );
	abort();
	
	/* should never happen */
	return jdouble(0);
	}

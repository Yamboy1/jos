! first.S  -  primary boot loader
!
! Copyright (C) 1995,1998 Gero Kuhlmann   <gero@gkminix.han.de>
! Copyright (C) 1996,1997 Gero Kuhlmann   <gero@gkminix.han.de>
!                and Markus Gutschke <gutschk@math.uni-muenster.de>
!
!  This program is free software; you can redistribute it and/or modify
!  it under the terms of the GNU General Public License as published by
!  the Free Software Foundation; either version 2 of the License, or
!  any later version.
!
!  This program is distributed in the hope that it will be useful,
!  but WITHOUT ANY WARRANTY; without even the implied warranty of
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!  GNU General Public License for more details.
!
!  You should have received a copy of the GNU General Public License
!  along with this program; if not, write to the Free Software
!  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef ASM_DEBUG
#undef ASM_DEBUG
#endif
#include "first.inc"

! Note on the following code: this has been written to run in real-86-mode.
! The type of processor check is made lateron in the Linux kernel, so let
! this code even work on a 8086 without a crash. The kernel will tell the
! user if a wrong processor is being used. Space and time are not quite
! crucial in this loader for not using raw-86-code.

	.text

	.org	0

	mov	dx,ds
	mov	ax,cs			! set DS and ES
	mov	ds,ax
	mov	oldES,es
	mov	oldDS,dx		! save old register values in case
	mov	oldBP,bp		! we have to return to the boot rom
	mov	oldSI,si
	mov	oldDI,di
	mov	bp,sp
	mov	ax,[bp+4]
	mov	(header+0),ax		! load the address of the boot header
	mov	ax,[bp+6]
	mov	(header+2),ax
	mov	ax,[bp+8]
	mov	(bootp+0),ax		! load the address of the bootp block
	mov	ax,[bp+10]
	mov	(bootp+2),ax

! Tell the user who we are and that we started running

	mov	si,#sigmsg
	call	prnstr

! Show program parameter addresses for debugging

#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	mov	si,#ad1msg
	call	prnstr
	mov	ax,(header+2)
	call	prnwrd
	mov	al,#0x3A
	call	prnchr
	mov	ax,(header+0)		! print header address
	call	prnwrd
	mov	si,#crlf
	call	prnstr

	mov	si,#ad2msg
	call	prnstr
	mov	ax,(bootp+2)
	call	prnwrd
	mov	al,#0x3A
	call	prnchr
	mov	ax,(bootp+0)		! print BOOTP address
	call	prnwrd
	mov	si,#crlf
	call	prnstr
#endif

! Check if the boot image header is correct.

	les	bx,header
	mov	si,#bmerr		! prepare for correct error message
	seg	es
	mov	ax,BOOT_HD_MAGIC+0[bx]
	cmp	ax,(bmagic+0)		! compare boot rom magic numbers
	jne	doerr1
	seg	es
	mov	ax,BOOT_HD_MAGIC+2[bx]
	cmp	ax,(bmagic+2)
	jne	doerr1

	mov	si,#vmerr		! prepare for correct error message
	seg	es
	mov	al,BOOT_HD_LENGTH[bx]
	mov	cl,#4
	shr	al,cl
	and	al,#0x0F
	cmp	al,#VENDOR_SIZE		! check vendor ID size
	jne	doerr1
	xor	di,di
dovmag:	mov	al,vmagic[di]		! check vendor ID
	or	al,al
	jz	chkmem			! vendor ID ok, continue
	seg	es
	cmp	al,BOOT_HD_VENDOR[bx+di]
	jne	doerr1
	inc	di
	jmp	dovmag
doerr1:	jmp	near doerr

! Get the size of the base memory from the BIOS. We need at least
! 616 kB, which is the amount of base RAM below the boot rom memory
! area starting at 0x98000, plus the amount of static RAM needed for
! stack and command line. Note that this code just checks for the
! absolut minimum which is required. The actual amount of memory
! needed is determined lateron after determining the real base of
! the static memory area.

chkmem:	int	0x12			! get memory size in kB
	cmp	ax,#MIN_MEM
	mov	si,#memerr		! check if enough
	jb	doerr1
	mov	avlmem,ax		! save it for later

! Get the size of extended memory from the BIOS. This is needed for computing
! the address of a loaded ramdisk image.

	mov	ah,#0x88
	int	0x15
	mov	extmem,ax

! Looks good. Now find the lowest usable spot of memory and check
! if there is still enough to hold all necessary dynamic memory. This
! ensures that we dont overwrite anything valuable when saving the
! bootp record out of the boot rom memory area.

	mov	bx,#BOOT_LD_ILENGTH	! find the highest used memory
	call	fndmem			! block
#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	push	ax
	mov	dx,ax
	mov	si,#dynmsg
	call	prnstr
	mov	ax,dx
	call	prnwrd			! lets see what we got here
	mov	si,#crlf
	call	prnstr
	pop	ax
#endif
	mov	dx,ax
	mov	ax,cs
	mov	si,#recerr
	cmp	dx,ax			! oops, shouldnt be below us
	jbe	doerr1
	mov	bx,#MAX_MEM - (MAX_DYNAMIC_LEN)/16 - 1
	mov	si,#memerr
	cmp	dx,bx			! check if enough memory
	ja	doerr1

! We can now setup the pointers to all dynamic data structures. Note
! that the bootp memory block comes last so that we can use larger
! bootp records if required (and supported by the server), up until
! the beginning of the boot rom area (0x98000).

setptr:	mov	(adrptr+2),dx		! first set all segments
	mov	(pthptr+2),dx
	mov	(botptr+2),dx
	mov	word ptr (adrptr),#0	! then update the offsets
	mov	word ptr (pthptr),#0+MAX_ADDRS_LEN
	mov	word ptr (botptr),#0+MAX_ADDRS_LEN+MAX_PATH_LEN

! Determine the kernel entry address. This is used to actually call
! the kernel lateron.

	mov	si,#recerr
	mov	al,#VENDOR_SETUP
	call	fndldr			! find load record for kernel
	mov	ax,es			! setup module
	or	ax,di
	jz	doerr4
	seg	es
	mov	al,BOOT_LD_FLAGS[di]		! get load record flags
	test	al,#BOOT_FLAG_B0 + BOOT_FLAG_B1	! we are presently unable
	jnz	doerr4				! to handle relative addrs
	seg	es
	mov	ax,BOOT_LD_ADDR+0[di]	! get base address of setup module
	seg	es
	mov	bx,BOOT_LD_ADDR+2[di]
	mov	dx,ax
	and	dx,#0x000F		! determine offset into setup segment
	mov	cl,#4
	shr	ax,cl
	test	bx,#0xFFF0		! address must be reachable
	jnz	doerr4
	mov	cl,#12
	shl	bx,cl
	or	ax,bx			! determine segment pointer
	mov	(kernel+0),dx		! save pointer
	mov	(kernel+2),ax
#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	push	ax
	mov	si,#kermsg
	call	prnstr
	pop	ax
	call	prnwrd			! let the user know
	mov	al,#0x3A
	call	prnchr
	mov	ax,dx
	call	prnwrd
	mov	si,#crlf
	call	prnstr
#endif
	jmp	chkrd
doerr4:	jmp	near doerr

! Now take care of pre-loaded ramdisk images and kernel images that have been
! loaded into extended memory; these features require Linux V1.3.73 or above.

chkrd:	call	fndcode32		! Kernel entry point for 32bit code
	or	si,si
	jnz	doerr4
	call	fndrd			! Check for pre-loaded ramdisk image
	or	si,si
	jnz	doerr4
	call	initsetup		! Initialize kernel setup data
	or	si,si
	jnz	doerr4

! Before we can use the memory area of the boot rom we have to secure
! all interesting data, e.g. either the bootp record or the path name
! (whichever is given by the boot rom). First check to see if we got
! a bootp reply here, and copy it to the dynamic memory area.

	cld
	xor	dx,dx
	les	di,bootp
	seg	es
	mov	al,BOOTP_OP[di]		! op code must indicate reply
	cmp	al,#BOOTP_REPLY
	jne	dobot2			! it isnt
	add	di,#BOOTP_VEND
	mov	bx,di
	mov	si,#pmagic		! compare vendor ID
dobot1:	mov	di,bx
	mov	cx,#BOOTP_MAGIC_LEN
	repe
	cmpsb
	jz	dobot3			! vendor ID is valid
	add	si,#BOOTP_MAGIC_LEN
	cmp	byte ptr [si],#0	! check next vendor ID
	jne	dobot1
dobot2:
#if defined(ASM_DEBUG)
	mov	si,#nobmsg
	call	prnstr
#endif
	jmp	nobot2			! vendor ID not found

dobot3:	sub	si,#BOOTP_MAGIC_LEN
	sub	si,#pmagic
	mov	botid,si		! save vendor ID offset
	mov	ax,si
	push	ds
	les	di,botptr
	lds	si,bootp
	mov	bx,si
	mov	dx,#BOOTP_SIZE
	or	ax,ax			! if not RFC vendor ID the bootp
	jnz	dobot7			! record has fixed length

	xor	cx,cx
	add	si,#BOOTP_VEND + BOOTP_MAGIC_LEN
dobot4:	lodsb
	cmp	al,#BOOTP_RFC_NOP	! handle NOP tag
	jnz	dobot5
	inc	cx
	cmp	cx,#16			! more than 16 NOP tags is VERY unusual
	jb	dobot4			! so the bootp record maybe broken
	mov	si,#btperr
doerr6:	pop	ds
	jmp	near doerr

dobot5:	cmp	al,#BOOTP_RFC_END	! handle END tag
	jnz	dobot6
	mov	dx,si
	sub	dx,bx			! compute length of bootp record
	cmp	dx,#BOOTP_SIZE
	jae	dobot7
	mov	dx,#BOOTP_SIZE		! use minimum size
	jmp	dobot7

nobot2:	jmp	nobotp

dobot6:	lodsb				! handle all other tags
	mov	cl,al
	xor	ch,ch
	add	si,cx			! jump to next tag
	xor	cx,cx			! reset NOP counter
	jmp	dobot4			! proceed with next tag

dobot7:	mov	si,#btperr		! determine amount of dynamic memory
	mov	ax,#MAX_MEM		! available for bootp record
	mov	cx,es			! (botptr+2)
	sub	ax,cx
	test	ax,#0xF000
	jnz	dobot8
	mov	cl,#4
	shl	ax,cl
	mov	cx,di			! (botptr+0)
	sub	ax,cx
	cmp	dx,ax			! check if bootp record fits into
	jae	doerr6			! dynamic memory area

dobot8:	mov	cx,dx
	mov	si,bx			! restore source pointer
	rep
	movsb				! save the bootp record
	pop	ds

#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	mov	si,#btpmsg
	call	prnstr
	mov	ax,botid
	call	prnwrd			! let the user know
	mov	si,#crlf
	call	prnstr
#endif

nobotp:	mov	botlen,dx		! set length of bootp record
	or	dx,dx
	jnz	setmem			! check path name if bootp length is 0

! Some older versions of the boot rom code dont give the address of the
! bootp record but the address of the name of the boot image file on the
! server. Since we now know that we didnt get a valid bootp record, try
! the path name version next. Unfortunately there is no magic cookie
! available for checking the validity of the string (much older versions
! of the boot rom code didnt even provide this path name information!).
! The strlen routine will take care of checking the string: there shouldnt
! be any non-printable characters before the terminating 0, and the string
! should not be longer than allowed.

	xor	dx,dx
	push	ds
	lds	si,bootp
	call	strlen			! get total string length
	pop	ds
	jcxz	nopath			! abort if string is invalid
	cmp	cx,#MAX_PATH_LEN
	ja	nopath			! string should not be too long

#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	mov	si,#pthmsg
	call	prnstr
#endif
	push	ds
	les	di,pthptr
	lds	si,bootp		! save the path name string
	mov	dx,cx
	rep
	movsb
	pop	ds
nopath:	mov	pthlen,dx		! set length of path name string

! At this point all dynamic data, which is only necessary for this boot
! loader, has been saved. Now we can go ahead and acquire memory for every-
! thing which must remain intact when we call the kernel. This is basically
! the stack and the command line. We can now use the boot rom data area
! for this, so first find the highest used reserved memory segment from
! all load records, and setup the stack and command line.

setmem:	mov	bx,#BOOT_LD_MLENGTH	! find the highest reserved memory
	call	fndmem			! block
	cmp	ax,#MAX_MEM		! should be at least in the boot rom
	jae	setm1			! data area
	mov	ax,#MAX_MEM
setm1:	mov	dx,(botptr+0)		! the static data should not interfere
	add	dx,botlen		! with the dynamic data area. botptr
	add	dx,#0x000F		! is the last data field in the dynamic
	mov	cl,#4			! area, so compute its ending segment
	shr	dx,cl			! and use that as the static base.
	add	dx,(botptr+2)
	inc	dx
	cmp	ax,dx			! set end of dynamic data area as base
	jae	setm3			! of static area in case of conflict
	mov	ax,dx
setm3:
#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	push	ax
	mov	dx,ax
	mov	si,#stamsg
	call	prnstr
	mov	ax,dx
	call	prnwrd			! lets see what we got here
	mov	si,#crlf
	call	prnstr
	pop	ax
#endif

	mov	si,#recerr
	mov	bx,ax			! BX - base of free memory area
	mov	al,#VENDOR_CMDL
	call	fndldr			! find load record for cmd line
	mov	ax,es
	or	ax,di			! unable to find record
	jz	doerr
	seg	es
	mov	ax,BOOT_LD_MLENGTH+0[di]	! get memory length of cmd line
	seg	es
	mov	dx,BOOT_LD_MLENGTH+2[di]
	or	dx,dx			! command line is not allowed to be
	jnz	doerr			! longer than 4 kB
	cmp	ax,#CMDL_SIZE
	ja	doerr
	mov	cmdmax,ax		! save maximum size of command line
	add	ax,#STACK_SIZE		! compute size of static memory
	jc	doerr

	mov	si,#memerr
	add	ax,#0x000F		! round up to nearest segment
	mov	cl,#4
	shr	ax,cl			! convert it into segment number
	mov	dx,ax			! save size for later use
	add	ax,bx			! add it to base segment
	add	ax,#0x003F		! round to nearest kB
	mov	cl,#6
	shr	ax,cl			! convert it into kB
	cmp	ax,avlmem
	jb	setcmd			! we have enough memory

doerr:	call	prnstr			! in case of error return to the
	mov	si,oldSI		! boot rom with registers set
	mov	di,oldDI		! correctly
	mov	bp,oldBP
	mov	es,oldES
	mov	ds,oldDS
	retf

! We can now set the command line pointer. This requires some conversion
! because the kernel expects the command line to be relative to the floppy
! boot loader segment.
! BX contains the first segment usable for static memory, and DX contains
! the required size of static memory in segments.

setcmd:	mov	si,#recerr
	mov	al,#VENDOR_INIT
	call	fndldr			! find load record for the INIT segment
	mov	ax,es
	or	ax,di			! isnt there?
	jz	doerr
	mov	cx,dx
	seg	es
	mov	al,BOOT_LD_FLAGS[di]		! get load record flags
	test	al,#BOOT_FLAG_B0 + BOOT_FLAG_B1	! we are presently unable
	jnz	doerr				! to handle relative addrs
	seg	es
	mov	dx,BOOT_LD_ADDR+0[di]	! get base address of INIT segment
	seg	es
	mov	ax,BOOT_LD_ADDR+2[di]
	test	dx,#0x000F		! must be on a segment boundary
	jnz	doerr
	test	ax,#0xFFF0		! must in in lower MB
	jnz	doerr
	push	cx
	mov	cl,#4			! convert address into segment
	shr	dx,cl
	mov	cl,#12
	shl	ax,cl			!    BX - first usable memory segment
	or	dx,ax			!    DX - segment of floppy boot loader
	pop	cx			!    CX - size of static memory in segs
	sub	bx,dx			! compute offset to first usable memory
	jb	doerr

	mov 	si,#memerr
	mov	ax,bx			! the complete segment should not exceed
	add	ax,cx			! 64 kB
	jc	doerr
	test	ax,#0xF000
	jnz	doerr
	mov	cl,#4
	shl	bx,cl			! convert segment offset into byte offset
	add	bx,#STACK_SIZE
	mov	(cmdptr+0),bx		! save the command line pointer
	mov	(cmdptr+2),dx

#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	mov	si,#cmdmsg
	call	prnstr
	mov	ax,dx
	call	prnwrd
	mov	al,#0x3A
	call	prnchr			! show the new command line pointer
	mov	ax,bx			! on the console
	call	prnwrd
	mov	si,#crlf
	call	prnstr
#endif

! Now set the stack. From this point on there is no return to the boot rom.

	cli				! disable interrupts during stack setup
	dec	bx
	mov	sp,bx			! set the stack
	mov	ss,dx
	sti				! it is safe again

! At this point the memory layout should look like this, assuming default
! values for all load records set by mknbi:
!
! 0x10000-0x8FFFF	512.0 kB	kernel
! 0x90000-0x901FF	  0.5 kB	linux floppy boot sector
! 0x90200-0x911FF	  8.0 kB	first sectors of kernel setup
! 0x92200-0x931FF	  4.0 kB	primary boot loader - this program
! 0x93200-0x933FF	  0.5 kB	boot image header
! 0x93400-0x935FF	  0.5 kB	default command line
! 0x93600-0x937FF	  0.5 kB	dynamic memory - IP address string
! 0x93800-0x93BFF	  1.0 kB	dynamic memory - path name
! 0x93C00-0x93DFF	  0.5 kB	dynamic memory - bootp record
! 0x93E00-0x99FFF	 24.5 kB	(free - used lateron by kernel setup)
! 0x9A000-0x9AFFF	  4.0 kB	static memory - stack
! 0x9B000-0x9B7FF	  2.0 kB	static memory - kernel command line
! 0x9B800-0x9FFFF	 18.0 kB	(free)
!
! The remaining part of this program just deals with setting up the command
! line in the static memory area. First copy it to its rightful place and
! setup the pointers so that the kernel will find the new command line.

	push	ds
	push	bp
	xor	dx,dx			! DX contains length of command line
	mov	bp,cmdmax		! BP contains maximum length of cmd line
	sub	bp,#2			! keep some space for trailing 0
	mov	al,#VENDOR_CMDL
	call	fndldr			! find load record for cmd line
	mov	ax,es
	mov	si,di
	les	di,cmdptr		! load desitination pointer
	mov	ds,ax
	or	ax,si			! unable to find command line
	jz	nocmd1			! should never happen
	mov	al,BOOT_LD_FLAGS[si]		! get load record flags
	test	al,#BOOT_FLAG_B0 + BOOT_FLAG_B1	! we are presently unable
	jnz	nocmd1				! to handle relative addrs

	mov	ax,BOOT_LD_ADDR+0[si]	! load pointer to original command
	mov	bx,BOOT_LD_ADDR+2[si]	! line and convert it into seg:ofs
	push	ax
	and	ax,#0x000F		! compute offset
	mov	si,ax
	pop	ax
	mov	cl,#4
	shr	ax,cl			! compute segment
	test	bx,#0xFFF0
	jnz	nocmd1			! segment is unreachable in real mode
	mov	cl,#12
	shl	bx,cl
	or	ax,bx
	mov	ds,ax			! DS:SI points to old command line

	call	strlen			! get length of string
	jcxz	nocmd1
	cmp	cx,bp			! check length of command line
	jbe	stcmd1
	mov	cx,bp
stcmd1:	mov	dx,cx			! save length for later
	cld
	rep
	movsb				! move it
nocmd1:	xor	al,al
	stosb				! terminate the string with 0
	pop	bp
	pop	ds

	mov	al,#VENDOR_INIT
	call	fndldr			! find load record for floppy boot prg
	mov	ax,es
	or	ax,di			! oops, not there?
	jz	nocmd2			! should never happen
	seg	es
	mov	al,BOOT_LD_FLAGS[di]		! get load record flags
	test	al,#BOOT_FLAG_B0 + BOOT_FLAG_B1	! we are presently unable
	jnz	nocmd2				! to handle relative addrs

	seg	es
	mov	ax,BOOT_LD_ADDR+0[di]	! load pointer to floppy boot sector
	seg	es
	mov	bx,BOOT_LD_ADDR+2[di]
	push	ax
	and	ax,#0x000F		! compute offset
	mov	di,ax
	pop	ax
	mov	cl,#4
	shr	ax,cl			! compute segment
	test	bx,#0xFFF0
	jnz	nocmd2			! segment is unreachable in real mode
	mov	cl,#12
	shl	bx,cl
	or	ax,bx
	mov	es,ax			! ES:DI points to floppy boot sector
	mov	(flopptr+0),di
	mov	(flopptr+2),es

	mov	bx,(cmdptr+0)
	sub	bx,di
	mov	cx,(cmdptr+2)		! compute offset to command line
	sub	cx,ax			! from base of init segment
	jc	nocmd2
	test	cx,#0xF000
	jnz	nocmd2			! offset too large
	mov	ax,cx
	mov	cl,#4
	shl	ax,cl			! convert seg:ofs representation into
	add	ax,bx			! absolute number of bytes

	seg	es			! setup command line descriptor
	mov	word ptr CL_MAGIC_ADDR[di],#CL_MAGIC
	seg	es
	mov	word ptr CL_OFFSET[di],ax
	jmp	stcmd2
nocmd2:	xor	dx,dx			! error with command line
stcmd2:	mov	cmdlen,dx		! save command line length

#ifdef ASM_DEBUG_VERBOSE
	mov	ax,dx
	call	prnwrd
	mov	al,#0x20
	call	prnchr
	push	ds
	lds	si,cmdptr		! print the command line so far
	call	prnstr
	pop	ds
	mov	si,#crlf
	call	prnstr
#endif

! Now determine the name of the root directory on the server by looking
! into the bootp record. If there is no bootp record, the path has probably
! already been set directly from a parameter given by the boot rom. If
! a boot image file name was found, only leave the directory name by
! removing everything after the last slash. There is also the possibility
! to define a root directory name in the vendor area of the bootp record,
! so check for that also.

	cmp	botlen,#BOOTP_SIZE
	jb	bootp3			! bootp record not loaded
	push	ds
	mov	bx,pthlen		! keep old path length for reference
	mov	dx,#MAX_PATH_LEN
	sub	dx,#2			! keep space in path name for trailing 0
	les	di,pthptr
	lds	si,botptr
	add	si,#BOOTP_FILE
	call	strlen			! determine length of file name
	jcxz	bootp2			! no file name, leave path untouched
	cmp	cx,dx
	jbe	bootp1
	mov	cx,dx			! get length of path name
bootp1:	mov	bx,cx			! save length  for later use
	cld
	rep
	movsb				! copy path name to its new location
	xor	al,al
	stosb				! save trailling 0
bootp2:	pop	ds
	mov	pthlen,bx		! save new length of path name

bootp3:	xor	dx,dx
	mov	cx,pthlen		! No file name given
	jcxz	bootp4
	std
	les	di,pthptr
	add	di,cx			! point ES:DI to end of string
	mov	al,#0x2F		! search backwards for last occurrence
	repne				! of a slash
	scasb
	jnz	bootp4			! not found -> no path
	seg	es
	mov	byte ptr [di+1],#0	! replace the slash with 0
	mov	dx,cx
	inc	dx
bootp4:	mov	pthlen,dx		! store length of directory name

	mov	al,#BOOTP_RFC_ROOT	! there is still another possibility:
	call	gettag			! get the directory name from vendor
	jcxz	bootp5			! field of bootp record
	mov	dx,cx
	push	ds
	cld
	mov	ax,es
	mov	si,di
	les	di,pthptr		! copy the directory name from the
	mov	ds,ax			! bootp record
	rep
	movsb
	xor	al,al
	stosb				! terminate string with 0
	pop	ds
	mov	pthlen,dx

bootp5:
#ifdef ASM_DEBUG_VERBOSE
	mov	ax,pthlen
	call	prnwrd
	mov	al,#0x20
	call	prnchr
	push	ds
	lds	si,pthptr		! print out the path
	call	prnstr
	pop	ds
	mov	si,#crlf
	call	prnstr
#endif

! Build the IP address string by looking into the bootp record. If there
! is no bootp record available, just dont do anything, and leave the
! string empty.

	xor	dx,dx
	les	di,adrptr
	cmp	botlen,#BOOTP_SIZE
	jb	noaddr			! no bootp record, no address string

	cld
	push	ds
	lds	si,botptr
	add	si,#BOOTP_YIADDR	! load my own IP address
	call	cvtadr			! put address into destination string
	mov	al,#0x3A		! add ':'
	call	cvtchr
	add	si,#BOOTP_SIADDR - BOOTP_YIADDR
	call	cvtadr			! put server address into dest string
	mov	al,#0x3A		! add ':'
	call	cvtchr
	pop	ds

	push	es
	push	di
	mov	al,#BOOTP_RFC_GWY	! we have to take the gateway address
	call	gettag			! from the vendor info area
	mov	ax,es
	mov	si,di
	pop	di
	pop	es
	jcxz	noadd1			! no netmask found
	push	ds
	mov	ds,ax
	call	cvtadr			! put gateway address into dest string
	pop	ds
noadd1:	mov	al,#0x3A		! add ':'
	call	cvtchr

	push	es
	push	di
	mov	al,#BOOTP_RFC_MSK	! we have to take the netmask from the
	call	gettag			! vendor info area
	mov	ax,es
	mov	si,di
	pop	di
	pop	es
	jcxz	noadd2			! no netmask found
	push	ds
	mov	ds,ax
	call	cvtadr			! put netmask address into dest string
	pop	ds
noadd2:	mov	al,#0x3A		! add ':'
	call	cvtchr

	push	es
	push	di
	mov	al,#BOOTP_RFC_HNAME	! get the host name from the vendor
	call	gettag			! info area
	mov	ax,es
	mov	si,di
	pop	di
	pop	es
	jcxz	noaddr			! no host name found
	push	ds
	mov	ds,ax
	add	dx,cx			! update string length
	rep
	movsb				! move host name into dest string
	pop	ds

noaddr:	xor	al,al			! terminate string with 0
	stosb
	mov	adrlen,dx		! save string length

#ifdef ASM_DEBUG_VERBOSE
	mov	ax,dx
	call	prnwrd
	mov	al,#0x20
	call	prnchr
	push	ds
	lds	si,adrptr		! let the user know
	call	prnstr
	pop	ds
	mov	si,#crlf
	call	prnstr
#endif

! Now we can create the final command line which can be passed to the
! kernel. This includes adding any additional arguments from the BOOTP
! record.

	mov	si,#rtstr
	mov	cx,#rtend - rtstr
	les	di,pthptr
	mov	ax,pthlen		! first handle root directory name
	or	ax,ax
	jnz	setup1
	mov	es,ax
	mov	di,ax
setup1:	call	procv			! setup command line

	mov	si,#adrstr
	mov	cx,#adrend - adrstr
	les	di,adrptr
	mov	ax,adrlen		! then handle the IP address string
	or	ax,ax
	jnz	setup2
	mov	es,ax
	mov	di,ax
setup2:	call	procv			! setup command line
	call	vparms			! process kernel parameters in vendor
					! extension tags
	mov	al,#BOOTP_RFC_CMDAD	! check for application vendor tag
	call	gettag
	jcxz	setup			! tag not available
	mov	si,#nulstr
	call	addarg			! add string to end of command line

! Finally call the setup code of the kernel

setup:
	call	vga			! process "vga=" argument

#ifdef ASM_DEBUG_VERBOSE
	mov	ax,cmdlen
	call	prnwrd
	mov	al,#0x20
	call	prnchr
	push	ds
	lds	si,cmdptr		! Print the new command line for
	call	prnstr			! debugging
	pop	ds
#endif
#ifdef ASM_FREEZE_AFTER_INIT
lop1:	jmp	lop1			! This is a feature, not a bug...
#endif
#ifdef ASM_KEYPRESS_AFTER_INIT
	xor	ah,ah
	int	0x16
#endif
	jmp	far [kernel]		! call the kernel



!====================================================================
!
! Convert an IP address pointed to by DS:SI into a string pointed
! to by ES:DI. The length of the string gets accumulated in DX.
! ES:DI points to first character after string.
!
! Changed registers: AX, DX, DI, ES

cvtadr:	push	bx
	mov	ax,[si]
	or	ax,[si+2]		! if address is 0.0.0.0, dont
	jz	cvtad9			! convert it

	xor	bx,bx
cvtad1:	mov	al,[si+bx]
	call	cvtdec			! convert each byte in turn into a
	cmp	bx,#3			! decimal number
	jae	cvtad9			! abort if at last byte of address
	mov	al,#0x2E		! add '.'
	call	cvtchr
	inc	bx			! proceed with next byte
	jmp	cvtad1

cvtad9:	pop	bx
	ret



!====================================================================
!
! Convert byte in AL into a decimal number string pointed to by
! ES:DI. The length of the string gets accumulated in DX. Return
! a pointer to first character after string in ES:DI.
!
! Changed registers: AX, DX, DI, ES

cvtdec:	push	cx
	xor	ah,ah
	xor	ch,ch
	mov	cl,#100
	div	cl			! get 100th
	or	ch,al			! dont print if zero
	jz	cvtd1
	call	cvtd7			! convert digit into ascii character
cvtd1:	mov	al,ah
	xor	ah,ah
	mov	cl,#10
	div	cl			! get 10th
	or	ch,al			! dont print if zero
	jz	cvtd2
	call	cvtd7			! convert digit into ascii character
cvtd2:	mov	al,ah			! get 1th
	pop	cx
cvtd7:	add	al,#0x30		! convert digit into ascii character
cvtchr:	cmp	dx,#MAX_ADDRS_LEN
	jae	cvtch9
	stosb
	inc	dx
cvtch9:	ret



!====================================================================
!
! Scan all load records, and find the highest used memory address,
! round it up to the nearest segment boundary and return it in AX.
! The offset to the length value in the load records is given in BX.
! Ignore all memory segments that have been loaded into extended memory!
!
! Changed registers: AX, DI, ES

fndmem:	push	cx
	push	dx
	les	di,header
	mov	ax,di
	add	ax,#0x000F
	and	ax,#0xFFF0		! round up to nearest segment
	mov	cl,#4
	shr	ax,cl
	mov	cx,es			! convert address of header into
	add	ax,cx			! segment representation, and add
	jc	fndm7			! the size of the header in order to
	add	ax,#BOOT_SIZE / 16	! get a starting value for the highest
	jnc	fndm8			! memory segment.
fndm7:	xor	ax,ax			! the highest segment is on the stack
fndm8:	push	ax

	seg	es
	mov	al,BOOT_HD_LENGTH[di]	! get length of image header
	call	getlen
	add	di,ax			! get the pointer to first load record
fndm1:	seg	es
	mov	al,BOOT_LD_FLAGS[di]		! get load record flags
	test	al,#BOOT_FLAG_B0 + BOOT_FLAG_B1	! we are presently unable
	jnz	fndm4				! to handle relative addrs
	seg	es
	mov	dx,BOOT_LD_ADDR+0[di]
	seg	es
	mov	ax,BOOT_LD_ADDR+2[di]	! get the load address
	seg	es
	add	dx,0[di+bx]		! add the length (offset in BX)
	seg	es
	adc	ax,2[di+bx]
	add	dx,#0x000F		! round up to the nearest segment
	adc	ax,#0
	and	dx,#0xFFF0
	test	ax,#0xFFF0		! only allow addresses in lower 1 MB
	jnz	fndm4

	mov	cl,#4			! convert address into segment address
	shr	dx,cl			! by shifting everything right by 4 bits
	mov	cl,#12
	shl	ax,cl
	or	ax,dx			! combine higher and lower word of addr
	pop	dx
	cmp	ax,dx			! is new address higher than old one?
	jbe	fndm3
	mov	dx,ax			! yes, save it
fndm3:	push	dx

fndm4:	seg	es
	mov	al,BOOT_LD_FLAGS[di]	! ok, thats it, now check if thats the
	test	al,#BOOT_FLAG_EOF	! last record
	jnz	fndm5
	seg	es
	mov 	al,BOOT_LD_LENGTH[di]	! no, get the address of the next one
	call	getlen
	add	di,ax
	jmp	fndm1
fndm5:	pop	ax			! return the highest used segment
	pop	dx
	pop	cx
	ret



!====================================================================
!
! Find a load record in the boot header. The ID number of the load
! record is in AL, and ES:DI points to requested load record, or is
! the NULL pointer if load record not found.
!
! Changed registers: AX, DI, ES

fndldr:	push	cx
	mov	ch,al
	les	di,header		! examine boot image header
	seg	es
	mov	al,BOOT_HD_LENGTH[di]	! get length of image header
	call	getlen
	add	di,ax			! get the pointer to first load record
fndl1:	seg	es
	cmp	ch,BOOT_LD_TAG1[di]	! is it the desired one ?
	je	fndl3
	seg	es
	mov	al,BOOT_LD_FLAGS[di]	! no, so check if its the last record
	test	al,#BOOT_FLAG_EOF
	jnz	fndl2
	seg	es
	mov 	al,BOOT_LD_LENGTH[di]	! no, get the address of the next one
	call	getlen
	add	di,ax
	jmp	fndl1

fndl2:	xor	ax,ax			! couldnt find the desired record
	mov	es,ax
	mov	di,ax
fndl3:	pop	cx
	ret



!====================================================================
!
! Compute the length of a load record address from a length byte
! in AL. Return the offset in AX.
!
! Changed registers: AX

getlen:	push	cx
	mov 	ah,al
	mov 	cl,#4
	shr	ah,cl
	and	ax,#0x0f0f		! compute the total length in
	add	al,ah			! bytes from the length of the
	xor	ah,ah			! record and that of the vendor
	shl	ax,1			! information.
	shl	ax,1
	pop	cx
	ret



!====================================================================
!
! Find a vendor tag in the bootp record. The tag number is in AL.
! It returns the pointer to the tag string in ES:DI and the length
! of the string in CX. CX is zero if tag not found.
!
! Changed registers: AX, CX, DI, ES

gettag:	cld
	push	si
	push	dx
	xor	dx,dx
	cmp	botlen,#BOOTP_SIZE
	jb	gett8			! no bootp record available
	cmp	botid,#0		! use only RFC vendor information
	jne	gett8
	les	di,botptr
	mov	si,di
	add	si,botlen		! let ES:SI point to last bootp byte
	add	di,#BOOTP_VEND
	add	di,#BOOTP_MAGIC_LEN

	mov	ah,al			! save tag number
	xor	ch,ch
gett1:	seg	es
	mov	al,[di]			! load current vendor tag
	inc	di
	cmp	al,#BOOTP_RFC_NOP	! NOP tag --> continue
	je	gett1
	cmp	al,#BOOTP_RFC_END	! END tag --> abort
	je	gett8
	seg	es
	mov	cl,[di]			! get length of current tag
	inc	di
	cmp	al,ah			! we got our tag
	je	gett9
	add	di,cx			! proceed with next tag
	cmp	di,si
	jae	gett8			! got at end of bootp record
	jmp	gett1

gett8:	mov	cx,dx
gett9:	pop	dx
	pop	si
	ret



!====================================================================
!
! Process an argument on the command line. This routine handles
! basically three cases:
!   - the argument value is "rom", which means to use the value
!     delivered by the boot rom
!   - the argument value is "kernel", which means to just remove
!     the argument from the command line and then let the kernel
!     use its defaults
!   - the argument value contains neither "rom" nor "kernel", so
!     dont touch it.
!
! The name of the argument to look for in the command line is given
! in DS:SI with the its length in CX. The boot rom value is pointed
! to by ES:DI. If ES:DI is the NULL pointer, no boot rom value is known.
!
! Changed registers: AX, BX, CX, DX, SI, DI, ES

procv:	push	bp
	push	es
	push	di
	les	di,cmdptr
	mov	dx,di			! load pointer to command line and
	call	strstr			! find option name
	jcxz	procv3			! option not found in cmd line
	mov	bx,di			!    ES:BX points to option name
	add	di,cx			!    ES:DI points to argument
	add	dx,cmdlen		!    DX    contains remaining cmd line
	sub	dx,di
	jle	procv2			! length of remaining cmd line is zero
	mov	cx,dx
	mov	dx,di
	mov	al,#0x20		! find end of argument (terminated by
	repne				! blank) or end of string (e.g. CX=0)
	scasb
	jcxz	procv1			! argument goes to end of string
	dec	di
procv1:	sub	di,dx
	xchg	dx,di			! compute length of argument in DX
	jg	procv4			! argument length is greater than zero

procv2:	mov	dx,si
	mov	di,bx			! if we got here, we found the option
	call	rmarg			! name but no argument, so just remove
	mov	si,dx			! the option from the cmd line.
procv3:	xor	dx,dx
procv4:	pop	bp
	pop	cx			! get pointer to boot rom value

! At this point the registers are allocated as follows:
!      DS:SI  points to name of option
!      CX:BP  points to boot rom value
!      ES:BX  points to option name in command line
!      ES:DI  points to argument in command line
!      DX     contains length of argument; if 0, BX and DI are invalid
!
! After all that we have a couple of choices for constructing the kernel
! command line:
!
! argument not given in command line:
!   -  if there is no value from the boot rom, just do nothing and let
!      the kernel decide
!   -  if there is a value from the boot rom, add a new argument at the
!      end of the command line using the boot rom value
!
! argument given in command line:
!   -  if the value of the argument is not empty and not "rom" or "kernel",
!      just do nothing
!   -  if the value of the argument is "rom", then remove the argument
!      from the command line and add a new one with the boot rom value, but
!      only if a boot rom value is available
!   -  if the value of the argument is "kernel", then remove the argument
!      from the command line and do nothing, thus letting the kernel decide

	or	dx,dx			! check if argument found in command
	jnz	isarg			! line

	push	ds
	mov	ax,cx
	mov	ds,ax
	or	ax,bp
	jz	procv5
	seg	ds			! if no boot rom value is available,
	mov	al,[bp]			! e.g. the string pointer is NULL or
	or	al,al			! the string length is zero, then
	jz	procv5			! just return and do nothing.
	pop	ds
	jmp	procv6
procv5:	pop	ds
	jmp	procv9

procv6:	mov	es,cx
	mov	di,bp			! if not, add new option with boot rom
	call	addarg			! argument to end of command line
procv9:	pop	bp
	ret

! Original command line contains the option we are looking for. Check whether
! the argument is one of the strings "rom" or "kernel"

isarg:	cld
	push	cx
	push	si
	mov	ax,di
	mov	cx,dx
	cmp	cx,#romend - romstr
	jne	norom
	mov	si,#romstr
	repe
	cmpsb				! compare for "rom"
	mov	di,ax
	jz	dorom
norom:	mov	cx,dx
	cmp	cx,#kerend - kerstr
	jne	nokern
	mov	si,#kerstr
	repe
	cmpsb				! compare for "kernel"
	mov	di,ax
	jz	dokern
nokern:	pop	si
	pop	cx			! not found --> do nothing
	jmp	procv9

! the argument is "rom", so remove the existing argument from the
! command line, and add the boot rom path

dorom:	mov	di,bx
	call	rmarg			! remove old option containing
	pop	si			! "rom" argument
	pop	es
	mov	di,bp

	mov	ax,es
	or	ax,di
	jz	procv9
	seg	es			! if no boot rom value is available,
	mov	al,[di]			! e.g. the string pointer is NULL or
	or	al,al			! the string length is zero, then
	jz	procv9			! just return and do nothing.
	call	addarg			! add new option with boot rom value
	jmp	procv9			! as argument

! the argument is "kernel", so just remove the existing argument from
! the command line and let the kernel decide by itself.

dokern:	mov	di,bx
	call	rmarg			! remove old option containing
	pop	si			! "kernel" argument
	pop	cx
	jmp	procv9



!====================================================================
!
! Add a new option to the end of the command line. DS:SI points to
! name of option, and ES:DI points to argument value. Return ES:DI
! pointing to the trailing 0 of the command line, and the new length
! of the command line in AX.
!
! Changed registers: AX, SI, DI, ES

addarg:	push	cx
	push	dx
	call	strlen			! first compute the new length of
	mov	dx,cx			! the command line. if the new
	xchg	si,di			! argument will not fit into the
	push	ds			! memory provided, then just return
	mov	ax,es			! and leave the command line
	mov	ds,ax			! unchanged.
	call	strlen
	pop	ds
	xchg	si,di
	add	cx,dx
	pop	dx
	add	cx,cmdlen
	inc	cx
	cmp	cx,cmdmax
	pop	cx
	jae	addar9

	cld
	push	bx
	push	ds
	push	di
	push	es
	les	di,cmdptr
	mov	bx,di
	add	di,cmdlen
	mov	al,#0x20		! add a blank
	stosb
addar1:	lodsb				! add the option name
	or	al,al
	jz	addar2
	stosb
	jmp	addar1
addar2:	pop	ds
	pop	si			! load the argument pointer
addar3:	lodsb				! add the argument
	or	al,al
	jz	addar4
	stosb
	jmp	addar3
addar4:	pop	ds			! restore DS and add trailing 0
	xor	al,al
	stosb
	dec	di
	mov	ax,di
	sub	ax,bx
	mov	cmdlen,ax		! adjust length of command line
	pop	bx
addar9:	ret



!====================================================================
!
! Remove an argument pointed to by ES:DI from the command line.
! Return ES:DI pointing to the trailing 0 of the command line, and
! the new length of the command line in AX.
!
! Changed registers: AX, SI, DI, ES

rmarg:	cld
	push	bx
	push	cx
	push	dx
	push	ds
	lds	si,cmdptr
	mov	ax,ds			! get beginning of command line
	mov	es,ax
	mov	bx,si
	mov	si,di
	call	strlen			! get remaining length of cmd line
	mov	ax,#0x20		! find end of argument in command line
	repne				! DS:SI points to first character after
	scasb				!       the argument string
	dec	di			! ES:DI points to start of argument
	xchg	si,di			! ES:BX points to start of cmd line
	or	cx,cx			! argument is not at the end of the
	jnz	rmarg2			! command line

rmarg1:	cmp	di,bx
	je	rmarg4			! empty command line
	dec	di			! remove leading blanks
	seg	es
	cmp	byte ptr [di],#0x20
	je	rmarg1
	inc	di			! point to first blank after preceeding
	jmp	rmarg4			! argument and make it end of string

rmarg2:	lodsb
	cmp	al,#0x20
	je	rmarg2			! remove trailing blanks
rmarg3:	or	al,al
	jz	rmarg1			! end of command line... remove all
	stosb				! trailing blanks. Otherwise copy
	lodsb				! the remainder of the command line
	jmp	rmarg3			! over the argument

rmarg4:	xor	al,al
	stosb				! add trailing 0
	pop	ds
	pop	dx
	pop	cx
	dec	di			! point to trailing 0
	mov	ax,di
	sub	ax,bx			! adjust length of command line
	mov	cmdlen,ax
	pop	bx
	ret



!====================================================================
!
! Find a substring (DS:SI, length CX) in a string (ES:DI). Return
! the substring position in ES:DI. CX is unchanged if substring found,
! otherwise CX is 0.
!
! Changed registers: AX, CX

strstr:	push	bx
	push	dx
	cld				! DX contains length of substring
	mov	dx,cx			! ES:BX points to string
	mov	bx,di			! DS:SI points to substring

strst1:	seg	es
	mov	al,[bx]
	or	al,al			! if at end of string we could not find
	jz	strst2			! the substring
	mov	di,bx
	mov	cx,dx			! reset substring
	mov	ax,si
	repe				! compare substring at current string
	cmpsb				! position
	mov	si,ax
	jz	strst3			! found it!
	inc	bx			! compare cmd line at next char
	jmp	strst1

strst2:	xor	dx,dx			! could not find string
strst3:	mov	cx,dx			! set return values
	mov	di,bx
	pop	dx
	pop	bx
	ret



!====================================================================
!
! Determine length of string (DS:SI), and return it in CX
!
! Changed registers: AL, CX

strlen: push	si
	xor	cx,cx
	cld
strl1:	lodsb
	or	al,al
	jz	strl3
	cmp	al,#0x20		! character should be printable
	jl	strl2
	cmp	al,#0x7E
	ja	strl2
	inc	cx			! count character
	jmp	strl1
strl2:	xor	cx,cx			! return 0 length if string invalid
strl3:	pop	si
	ret



!====================================================================
!
! Print a string in DS:SI onto the console
!
! Changed registers: AL

prnstr:	push	si
	cld
prns1:	lodsb				! loop over all characters of
	or	al,al			! string
	jz	prns2
	call	prnchr			! print character
	jmp	prns1
prns2:	pop	si
	ret



!====================================================================
!
! Print hexadecimal values (in AX or AL) or characters onto the console
!
! Changed registers: AX

prnwrd:	push	ax
	mov	al,ah
	call	prnbyt			! print the upper byte
	pop	ax
prnbyt: push	ax
	shr	al,1			! prepare upper nibble
	shr	al,1
	shr	al,1
	shr	al,1
	call	prnnib			! print it
	pop	ax
prnnib:	and	al,#0x0F		! prepare lower nibble
	add	al,#0x30
	cmp	al,#0x39		! convert it into hex
	jle	prnchr
	add	al,#7
prnchr:	push	bx
	mov	ah,#0x0E		! print it
	mov	bl,#0x07
	xor	bh,bh
	int	0x10
	pop	bx
	ret



!====================================================================
!
! Check whether the kernel has been loaded high; set the load_flags
! accordingly and patch the kernel entry point in the setup code
! if neccessary. If an error occurred, SI contains a pointer to the
! error message.
!
! Changed registers: AX, BX, CX, DX, SI, ES, DI

fndcode32:
	mov	si,#recerr
	mov	al,#VENDOR_KERNEL
	call	fndldr			! find load record for kernel image
	mov	ax,es
	or	ax,di
	jz	f32ret
	seg	es
	mov	al,BOOT_LD_FLAGS[di]		! get load record flags
	test	al,#BOOT_FLAG_B0 + BOOT_FLAG_B1	! we are presently unable
	jnz	f32ret				! to handle relative addrs
	seg	es
	mov	ax,BOOT_LD_ADDR+0[di]	! get base address of setup module
	seg	es
	mov	bx,BOOT_LD_ADDR+2[di]
	mov	dx,#SU_MY_LOAD_FLAGS+1	! "big" kernel
	test	bx,#0xFFF0		! test if the kernel is loaded high
	jnz	bzimg
	mov	cl,#4
	shr	ax,cl			! calculate start address in segment
	mov	dx,bx			! format
	shr	bx,cl
	mov	cl,#12
	shl	dx,cl
	or	ax,dx
	mov	dx,#SU_MY_LOAD_FLAGS	! "small" kernel
bzimg:	mov	lflags,dx		! update loadflags
	mov	(code32+0),ax		! save pointer to 32bit kernel entry
	mov	(code32+2),bx
#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	push	ax
	push	bx
	mov	si,#cd32msg
	call	prnstr
	pop	ax
	call	prnwrd			! let the user know
	pop	ax
	call	prnwrd
	mov	si,#crlf
	call	prnstr
#endif
	xor	si,si			! no error
f32ret:	ret



!====================================================================
!
! Check whether a ramdisk image is attached to the current netboot image;
! if a ramdisk is found, determine load address and size. If an error
! occurred, SI contains a pointer to the error message.
!
! Changed registers: AX, BX, CX, DX, SI, ES, DI

fndrd:	xor	si,si
	mov	al,#VENDOR_RAMDISK
	call	fndldr			! find load record for ramdisk image
	mov	ax,es
	or	ax,di
	jz	rdret2			! no ramdisk image found
	mov	si,#recerr
	seg	es
	mov	ax,BOOT_LD_ILENGTH[di]	! determine size of ramdisk image
	mov	(rdsize+0),ax
	seg	es
	mov	ax,BOOT_LD_ILENGTH+2[di]
	mov	(rdsize+2),ax
	seg	es
	mov	bl,BOOT_LD_FLAGS[di]	! get load record flags
	test	bl,#BOOT_FLAG_B0
	jnz	rdret2			! error: neither RD_FIX/AUTO nor RD_EOM
	mov	ax,extmem		! determine end of physical memory
	test	bl,#BOOT_FLAG_B1
	jnz	rdeom			! ramdisk is at end of memory
	seg	es			! check if ramdisk image needs to be
	mov	bl,BOOT_LD_VENDOR[di]	! relocated to end of memory
	cmp	bl,#RD_AUTO
	jne	rdfix			! ramdisk is at fixed location
	cmp	ax,#0x3c00		! there is more then 16MB
	jbe	rdeom
	mov	ax,#0x3c00		! limit to 16MB for BIOS
rdeom:	xor	bx,bx
	add	ax,#0x400		! add size of conventional memory
	adc	bx,#0
	mov	dx,ax			! multiply by 1024
	mov	cl,#6
	shr	dx,cl
	mov	cl,#10
	shl	ax,cl
	shl	bx,cl
	or	bx,dx			! this is not strictly correct: for
	seg	es			! RD_EOM loading, we should get the
	sub	ax,BOOT_LD_MLENGTH+0[di]! address rather than the memory size,
	seg	es			! but that would make the code for
	sbb	bx,BOOT_LD_MLENGTH+2[di]! RD_AUTO loading far more difficult...
	seg	es			! check if ramdisk image needs to be
	mov	cl,BOOT_LD_VENDOR[di]	! relocated to end of memory
	cmp	cl,#RD_AUTO
	jne	rddone
	seg	es
	mov	cx,BOOT_LD_ADDR+0[di]	! source address
	seg	es
	mov	dx,BOOT_LD_ADDR+2[di]
	call	moverd			! move ramdisk
	or	si,si
	jz	rddone			! check if error
rdret2:	jmp	rdret
rdfix:	seg	es
	mov	ax,BOOT_LD_ADDR+0[di]
	seg	es
	mov	bx,BOOT_LD_ADDR+2[di]
rddone:	mov	(ramdisk+0),ax
	mov	(ramdisk+2),bx
#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	push	ax
	push	bx
	mov	si,#rdmsg
	call	prnstr
	pop	ax
	call	prnwrd			! let the user know
	pop	ax
	call	prnwrd
	mov	al,#0x28
	call	prnchr
	mov	ax,(rdsize+2)
	call	prnwrd
	mov	ax,(rdsize+0)
	call	prnwrd
	mov	ax,#0x29
	call	prnchr
	mov	si,#crlf
	call	prnstr
#endif
	xor	si,si
rdret:	ret



!====================================================================
!
! Move the ramdisk image from the location given in DX/CX to BX/AX. If
! an error occurred, SI contains a pointer to the error message.
!
! Changed registers: CX, DX, SI

moverd:	push	ax			! save all registers that might
	push	bx			! be mangled
	push	es
	push	di
	mov	word ptr rd_dstb,ax	! enter destination address into gdt
	mov	word ptr rd_srcb,cx	! enter source address into gdt
	mov	cx,(rdsize+0)		! number of bytes that need to be moved
	mov	ax,(rdsize+2)
	add	cx,#1			! round up to next word
	adc	ax,#0
	add	bl,al			! move from the top downwards
	add	dl,al
	mov	byte ptr rd_dstb+2,bl
	mov	byte ptr rd_srcb+2,dl
	shr	cx,#1			! INT15 moves words
	jz	mvzero
	push	ax			! save register
	push	cs			! pass gdt in es:si
	pop	es
	mov	si,#rd_gdt
	mov	ax,#0x8700		! have the BIOS move the memory
	int	0x15
	pop	ax			! restore register
	mov	si,#movmsg
	jc	mvret			! oops, something went wrong...
mvzero:	or	al,al
	jz	mvno64
mvloop:	dec	byte ptr rd_srcb+2	! decrement source address
	dec	byte ptr rd_dstb+2	! decrement dest address
	mov	cx,#0x8000		! move in chunks of 64k/2 words
	push	ax			! save register
	mov	si,#rd_gdt
	mov	ax,#0x8700
	int	0x15
	pop	ax
	mov	si,#movmsg
	jc	mvret			! oops, something went wrong...
	dec	al
	jnz	mvloop
mvno64:	xor	si,si
mvret:	pop	di			! restore all registers
	pop	es
	pop	bx
	pop	ax
	ret				! done



!====================================================================
!
! Initialize the information about the kernel image and ramdisk image that
! is stored in the header of the kernel setup code. If an error occurred,
! SI contains a pointer to the error message.
!
! Changed registers: AX, BX, CX, DX, SI, ES, DI

initsetup:
	mov	si,#seterr
	les	di,kernel		! get address of kernel setup header
	seg	es
	mov	ax,SU_MAGIC_ADDR[di]	! check magic number
	cmp	ax,#SU_MAGIC_L
	jne	iserr
	seg	es
	mov	ax,SU_MAGIC_ADDR+2[di]
	cmp	ax,#SU_MAGIC_H
	jne	iserr
	mov	si,#oldkrn
	seg	es
	mov	ax,SU_VERSION_ADDR[di]	! check version number
	cmp	ax,#SU_VERSION
	jb	iserr
	seg	es			! this is "mknbi"
	mov	byte ptr SU_TYPE_OF_LOADER[di],#SU_MY_TYPE_OF_LOADER
	mov	ax,lflags
	seg	es
	mov	SU_LOAD_FLAGS[di],ax	! can use heap, might be loaded high
	mov	ax,(code32+0)
	seg	es
	mov	SU_32_START[di],ax	! update 32bit kernel entry point
	mov	ax,(code32+2)		! if loaded into high memory
	seg	es
	mov	SU_32_START+2[di],ax
	mov	ax,(ramdisk+0)
	seg	es
	mov	SU_RAMDISK_START[di],ax	! tell the kernel about the ramdisk
	mov	ax,(ramdisk+2)
	seg	es
	mov	SU_RAMDISK_START+2[di],ax
	mov	ax,(rdsize+0)
	seg	es
	mov	SU_RAMDISK_SIZE[di],ax
	mov	ax,(rdsize+2)
	seg	es
	mov	SU_RAMDISK_SIZE+2[di],ax
	seg	es			! kernel may use the heap
	mov	word ptr SU_HEAP[di],#SU_HEAP_END
	jmp	isok

iserr:	mov	ax,(ramdisk+0)		! cannot load ramdisk with old
	or	ax,(ramdisk+2)		! kernels
	jnz	isret
	mov	ax,lflags
	test	al,#1			! cannot load high with old kernels
	jnz	isret
isok:	xor	si,si
isret:	ret



!====================================================================
!
! Handle any "vga=" argument that might be on the command line
!
! Changed registers: AX, BX, CX, DX, SI, DI, ES

vga:	mov	si,#vgastr
	mov	cx,#vgaend - vgastr
	les	di,cmdptr
	call	strstr			! ES:DI  = strstr(cmdptr,"vga=")
	jcxz	isret			! not found
	push	di
 	add	di,cx			! ES:DI += strlen("vga=")
	xor	bx,bx

	cld
	mov	ax,di
	mov	cx,#askend - askstr	! strncmp(ES:DI,"ASK",3)
	mov	si,#askstr
	repe
	cmpsb
	mov	di,ax
	jz	doask

	mov	cx,#extend - extstr	! strncmp(ES:DI,"EXTENDED",8)
	mov	si,#extstr
	repe
	cmpsb
	mov	di,ax
	jz	doext

	mov	cx,#nrmend - nrmstr	! strncmp(ES:DI,"NORMAL",6)
	mov	si,#nrmstr
	repe
	cmpsb
	mov	di,ax
	jz	donrm

	mov	cx,#hexend - hexstr	! strncmp(ES:DI,"0x",2)
	mov	si,#hexstr
	repe
	cmpsb
	mov	di,ax
	jz	dohex

	jmp	dodez

doask:	dec	bx			! ASK      == -3
doext:	dec	bx			! EXTENDED == -2
donrm:	dec	bx			! NORMAL   == -1
vgaset:	les	si,flopptr
	seg	es
	mov	word ptr VGA_OFFSET[si],bx

#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
	push	bx
	mov	si,#vgastr
	call	prnstr
	pop	ax
	call	prnwrd
	mov	si,#crlf
	call	prnstr
#endif

	les	di,cmdptr		! remove this argument from commandline
	pop	di
	call	rmarg
	jmp	vga			! handle next argument (if any)

dohex:	mov	cl,#4
	xor	dh,dh
	inc	di
hexloop:inc	di			! ES:DI++
	seg	es
	mov	dl,[di]
	sub	dl,#0x30		! if (*ES:DI < '0') break
	jc	vgaset
	cmp	dl,#0x09		! if (*ES:DI > '9') goto nodigit
	jbe	hexinc
	sub	dl,#0x11		! if (*ES:DI < 'A') break
	jc	vgaset
	cmp	dl,#0x05		! if (*ES:DI > 'F') goto nocapital
	jbe	hexxinc
	sub	dl,#0x20		! if (*ES:DI < 'a') break
	jc	vgaset
	cmp	dl,#0x05		! if (*ES:DI > 'f') break
	ja	vgaset
hexxinc:add	dl,#0x0A
hexinc:	shl	bx,cl			! BX = 16*BX + digit
	add	bx,dx
	jmp	hexloop

vgasgn:	or	al,al			! if (sign) BX = -BX
	jz	vgaset
	neg	bx
	jmp	vgaset

dodez:	xor	al,al
	xor	dh,dh
	seg	es
	cmp	byte ptr [di],#0x2D	! if (*ES:DI == '-') sign++
	jne	dezcnv
	inc	al			! AL = sign
dezlop:	inc	di			! ES:DI++
dezcnv:	seg	es
	mov	dl,[di]
	sub	dl,#0x30		! if (*ES:DI < '0') break
	jc	vgasgn
	cmp	dl,#0x9			! if (*ES:DI > '9') break
	ja	vgasgn
	add	bx,bx			! BX = 10*BX + *ES:DI - '0'
	mov	cx,bx
	add	bx,bx
	add	bx,bx
	add	bx,cx
	add	bx,dx
	jmp	dezlop



!====================================================================
!
! Process kernel parameters in vendor extension tags
!
! Changed registers: AX, BX, CX, DX, SI, DI, ES

vparms:	mov	al,#BOOTP_RFC_VID	! check for 'äEth' vendor ID
	call	gettag
	cmp	cx,#6			! there have to be at least 6 bytes
	jb	vperr1			! in this tag
	seg	es
	cmp	word ptr 0[di],#BOOTP_VID_1
	jnz	vperr0
	seg	es
	cmp	word ptr 2[di],#BOOTP_VID_2
vperr0:	jnz	vperr2

	mov	al,#BOOTP_RFC_VSEL	! get user selection
	call	gettag
	cmp	cx,#1
vperr1:	jb	vperr
	seg	es
	mov	al,byte ptr 0[di]	! get selected image descripton
	call	gettag
	jcxz	vperr

	cld
	seg	es
	mov	cl,byte ptr -1[di]
	mov	bx,#6			! scan for parameter field
	mov	ax,#0x3A		! count colons
vplop1:	repne
	scasb
	jcxz	vperr
	dec	bx
	jnz	vplop1

	mov	si,di
	repne				! scan for end of parameter field
	scasb
	seg	es
	cmp	byte ptr -1[di],al
	jnz	vnocol
	dec	di
vnocol:	mov	cx,di			! compute length of parameter field
	sub	cx,si
	mov	bx,cmdlen
	add	bx,cx
	add	bx,#2
	cmp	bx,cmdmax
	jae	vperr
	dec	bx
	mov	ax,es
	les	di,cmdptr
	add	di,cmdlen
	push	ds
	mov	ds,ax
	mov	al,#0x20		! add a blank
	stosb
vpcpy:	lodsb
	or	al,al
	jnz	vnonul
	sub	bx,cx
	jmp	vpeos

vnonul:	cmp	al,#0x7E		! check for tilde character
	jnz	vnoesc
	dec	bx
	dec	cx
	jz	vpeos
	lodsb
	cmp	al,#0x62		! '~b' -> '\\'
	jb	vnoesc
	jnz	vpnob
	mov	al,#0x5C
	jmp	vnoesc

vperr2:	jmp	vperr

vpnob:	cmp	al,#0x63		! '~c' -> ':'
	jnz	vnoesc
	mov	al,#0x3A
vnoesc:	stosb
	loop	vpcpy			! loop over whole string

vpeos:	xor	al,al
	stosb				! add trailing 0
	pop	ds
	mov	cmdlen,bx
vperr:	ret



!====================================================================
!
! String and constants definitions


! Startup signature

sigmsg:	.byte	0x0D, 0x0A
	.ascii	"Linux Net Boot Image Loader"
	.byte	0x0D, 0x0A
crlf:	.byte	0x0D, 0x0A
	.byte	0


! Magic numbers for boot record and bootp entry

bmagic:	.long	BOOT_MAGIC		! boot image magic number
vmagic:	.ascii	VENDOR_MAGIC		! vendor magic ID
	.byte	0			! end of vendor magic ID
pmagic:	.byte	BOOTP_MAGIC_RFC		! bootp magic ID for RFC 1048
	.byte	BOOTP_MAGIC_CMU		! bootp magic ID for CMU
	.byte	BOOTP_MAGIC_STA		! bootp magic ID for Stanford
	.byte	0


! Possible option names for command line

rtstr:	.ascii	"nfsroot="
rtend:	.byte	0

adrstr:	.ascii	"nfsaddrs="
adrend:	.byte	0

vgastr:	.ascii	"vga="
vgaend:	.byte	0

askstr:	.ascii	"ASK"
askend:	.byte	0

extstr:	.ascii	"EXTENDED"
extend:	.byte	0

nrmstr:	.ascii	"NORMAL"
nrmend:	.byte	0

hexstr:	.ascii	"0x"
hexend:	.byte	0


nulstr:	.byte	0

! Possible arguments for command line options

romstr:	.ascii	"rom"
romend:	.byte	0

kerstr:	.ascii	"kernel"
kerend:	.byte	0


! Error messages

memerr:	.ascii	"Not enough memory"
	.byte	0x0D, 0x0A
	.byte	0

recerr:	.ascii	"Error in load record data"
	.byte	0x0D, 0x0A
	.byte	0

btperr:	.ascii	"BOOTP record too large"
	.byte	0x0D, 0x0A
	.byte	0

seterr:	.ascii	"Error in kernel setup header"
	.byte	0x0D, 0x0A
	.byte	0

bmerr:	.ascii	"Invalid boot header magic number"
	.byte	0x0D, 0x0A
	.byte	0

vmerr:	.ascii	"Invalid vendor magic ID"
	.byte	0x0D, 0x0A
	.byte	0

oldkrn:	.ascii "Linux kernel too old for initrd/bzImage"
	.byte	0x0D, 0x0A
	.byte	0

movmsg:	.ascii	"Failed to move ramdisk image to end of memory"
	.byte	0x0D, 0x0A
	.byte	0


#if defined(ASM_DEBUG) || defined(ASM_DEBUG_VERBOSE)
! Some debug messages

ad1msg:	.ascii	"Header addr = "
	.byte	0

ad2msg:	.ascii	"BOOTP addr = "
	.byte	0

pthmsg:	.ascii	"Using boot file path name"
	.byte	0x0D, 0x0A, 0

nobmsg:	.ascii	"BOOTP record does not contain valid vendor ID"
	.byte	0x0D, 0x0A, 0

btpmsg:	.ascii	"Using bootp record, vendor ID is "
	.byte	0

kermsg:	.ascii	"Kernel start: "
	.byte	0

rdmsg:	.ascii	"Ramdisk start: "
	.byte	0

cd32msg:.ascii	"Kernel image: "
	.byte	0

dynmsg:	.ascii	"Dynamic memory start: "
	.byte	0

stamsg:	.ascii	"Static memory start: "
	.byte	0

cmdmsg:	.ascii	"Address of command line: "
	.byte	0
#endif



!====================================================================
!
! Variable definitions

header:	.long	0			! pointer to boot header from boot rom
bootp:	.long	0			! pointer to bootp block from boot rom
kernel:	.long	0			! pointer to kernel entry
code32:	.long	0			! pointer to kernel start (32bit)
lflags:	.word	0			! load flags

oldDS:	.word	0			! old DS from boot rom
oldES:	.word	0			! old ES from boot rom
oldBP:	.word	0			! old BP from boot rom
oldSI:	.word	0			! old SI from boot rom
oldDI:	.word	0			! old DI from boot rom

avlmem:	.word	0			! available amount of RAM in kB
extmem:	.word	0			! extended memory in kB

ramdisk:.long	0			! pointer to start of ramdisk (32bit)
rdsize:	.long	0			! size of ramdisk

cmdptr:	.long	0			! pointer to start of command line
cmdlen:	.word	0			! length of command line
cmdmax:	.word	0			! maximum length of command line

botptr:	.long	0			! pointer to bootp record
botlen:	.word	0			! length of bootp record
botid:	.word	0			! vendor magic ID (offset into table)

pthptr:	.long	0			! pointer to path string
pthlen:	.word	0			! length of path string

adrptr:	.long	0			! pointer to IP address string
adrlen:	.word	0			! length of IP address string

flopptr:.long	0			! pointer to floppy boot sector

rd_gdt:	.word	0,0,0,0
	.word	0,0,0,0
rd_src:	.word	0xffff			! length
rd_srcb:.byte	0,0,0			! base
	.byte	0x93			! typebyte
	.word	0			! limit16,base24 =0
rd_dst:	.word	0xffff			! length
rd_dstb:.byte	0,0,0			! base
	.byte	0x93			! typebyte
	.word	0			! limit16,base24 =0
	.word	0,0,0,0			! BIOS CS
	.word	0,0,0,0			! BIOS DS


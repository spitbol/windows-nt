	page	80,132
	title	XCREATE - Convert Vector to an External Block
	.386
	name	XCREATE

;	Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
;	Use of this source code is governed by a BSD-style
;	license that can be found in the LICENSE file.

;
;	Usage:
;
;	LOAD("XCREATE(VECTOR)","x.slf")
;
;	Call with:
;		A = ARRAY(1000)
;		XCREATE(A)		;* convert to 1001 slot external block
;
;	Converts the incoming one-dimensional array to an external
;	block with one more 4-byte slots.  These slots can be used to store
;	arbitrary data.  Use XPUT and XGET to save and load data from
;	the external block.
;
;	Returns the null string if successful, fails if incoming arg
;	is not a vector.
;
;
;
INCLUDE system.inc			; select MS-DOS or OS/2 conventions
INCLUDE	extrn386.inc

	preamble _TEXT

;
;  Describe entry stack
;
if nearptr
xcstk	  struc
xcstkebp  dd	?			;save ebp
xcstkret  dd	?			;return pointer
xcpresult dd	?			;result pointer
xcpinfo	  dd	?			;misc info pointer
xcparg	  dd	?			;pointer to array
xcstk	  ends
else
xcstk	  struc
xcstkebp  dd	?			;save ebp
xcstkret  df	?			;return pointer
	  dw	?	       		;pad
xcpresult df	?			;far result pointer
	  dw	?			;pad
xcpinfo	  df	?			;far misc info pointer
	  dw	?			;pad
xcparg	  df	?			;far pointer to array
	  dw	?			;pad
xcstk	  ends
endif

        entry   EXT_XCREATE_

	enter	0,0			;set up stack frame

; Test if arg is a one-dimensional array (vector).
;
	sload	es,edi,[ebp].xcparg	;pointer to arg block
	sload	fs,ecx,[ebp].xcpinfo	;pointer to misc info area
	sload	fs,ecx,sfs:[ecx].ptyptab ;pointer to type table
	mov	eax,ses:[edi].vctyp	;type word
	cmp	eax,vc			;vector?
	jne	short xcfail		;fail if not
	mov	eax,xn     		;XNBLK block type
	mov	ses:[edi].xntyp,eax	;convert to an external block
	mov	eax,ses:[edi].vclen	;size of vector block
	mov	ses:[edi].xnlen,eax	;size of external block

	sload	es,edi,[ebp].xcpresult	;point to result area
	mov	ses:[edi].sclen, 0	;return null string
	mov	eax, BL_SC
	leave
	ret

xcfail:	mov	eax,FAIL
	leave
	ret

EXT_XCREATE_ endp

	title	XGET - Get Integer from an External Block
	name	XGET
;
;	Usage:
;
;	LOAD("XGET(INTEGER,EXTERNAL)INTEGER","x.slf")
;
;	Argument 1 is the subscript of the desired slot, 1 to N.
;	Argument 2 is the external block produced by XCREATE.
;
;	Call with:
;		I = XGET(456, X)	;* retrieve integer X[456]
;
;	Returns integer if successful, fails if incoming
;	argument 1 is out of range, or argument 2 not an external block.
;
;
;
;
;  Describe entry stack
;
if nearptr
xgstk	  struc
xgstkebp  dd	?			;save ebp
xgstkret  dd	?			;return pointer
xgpresult dd	?			;result pointer
xgpinfo	  dd	?			;misc info pointer
xgparg	  dd	?			;pointer to array
xgiarg	  dd	?			;subscript
xgstk	  ends
else
xgstk	  struc
xgstkebp  dd	?			;save ebp
xgstkret  df	?			;return pointer
	  dw	?	       		;pad
xgpresult df	?			;far result pointer
	  dw	?			;pad
xgpinfo	  df	?			;far misc info pointer
	  dw	?			;pad
xgparg	  df	?			;far pointer to array
	  dw	?			;pad
xgiarg	  dd	?			;subscript
xgstk	  ends
endif

        entry   EXT_XGET_

	enter	0,0			;set up stack frame

; Test if parg is an external block.
;
	sload	es,edi,[ebp].xgparg	;pointer to arg block
	sload	fs,ecx,[ebp].xgpinfo	;pointer to misc info area
	sload	fs,ecx,sfs:[ecx].ptyptab ;pointer to type table
	mov	eax,ses:[edi].xntyp	;type word
	cmp	eax,xn			;external block?
	jne	short xgfail		;fail if not

	mov	ebx,[ebp].xgiarg	;subscript
	dec	ebx			;scale 0 to N-1
	mov	eax,ses:[edi].xnlen	;size of block in bytes
	shr	eax,2			;convert to words
	sub	eax,3			;max legal subscript
	cmp	ebx,eax
	ja	short xgfail		;too large

	mov	eax,ses:[edi+ebx*4].xndta	;load value

	sload	es,edi,[ebp].xgpresult	;point to result area
	mov	ses:[edi].icval, eax	;return integer
	mov	eax, BL_IC
	leave
	ret

xgfail:	mov	eax,FAIL
	leave
	ret

EXT_XGET_    endp

	title	XPUT - Put Integer into an External Block
	name	XPUT
;
;	Usage:
;
;	LOAD("XPUT(INTEGER,INTEGER,EXTERNAL)","x.slf")
;
;	Argument 1 is the integer to be stored in the external block.
;	Argument 2 is the subscript of the desired slot, 1 to N.
;	Argument 3 is the external block produced by XCREATE.
;
;	Call with:
;		XPUT(123, 456, X)	;* store 123 in X[456]
;
;	Returns the null string if successful, fails if incoming
;	argument 2 is out of range, or argument 3 not an external block.
;
;
;
;
;  Describe entry stack
;
if nearptr
xpstk	  struc
xpstkebp  dd	?			;save ebp
xpstkret  dd	?			;return pointer
xppresult dd	?			;result pointer
xppinfo	  dd	?			;misc info pointer
xpparg    dd	?			;pointer to array
xpiarg2	  dd	?			;argument 2
xpiarg1	  dd	?			;argument 1
xpstk	  ends
else
xpstk	  struc
xpstkebp  dd	?			;save ebp
xpstkret  df	?			;return pointer
	  dw	?	       		;pad
xppresult df	?			;far result pointer
	  dw	?			;pad
xppinfo	  df	?			;far misc info pointer
	  dw	?			;pad
xpparg	  df	?			;far pointer to array
	  dw	?			;pad
xpiarg2	  dd	?			;argument 2
xpiarg1	  dd	?			;argument 1
xpstk	  ends
endif

        entry   EXT_XPUT_

	enter	0,0			;set up stack frame

; Test if parg is an external block.
;
	sload	es,edi,[ebp].xpparg	;pointer to arg block
	sload	fs,ecx,[ebp].xppinfo	;pointer to misc info area
	sload	fs,ecx,sfs:[ecx].ptyptab ;pointer to type table
	mov	eax,ses:[edi].xntyp	;type word
	cmp	eax,xn			;external block?
	jne	short xpfail		;fail if not

	mov	ebx,[ebp].xpiarg2	;subscript
	dec	ebx			;scale 0 to N-1
	mov	eax,ses:[edi].xnlen	;size of block in bytes
	shr	eax,2			;convert to words
	sub	eax,3			;max legal subscript
	cmp	ebx,eax
	ja	short xpfail		;too large

	mov	eax,[ebp].xpiarg1	;value to store
	mov	ses:[edi+ebx*4].xndta,eax	;store it

	sload	es,edi,[ebp].xppresult	;point to result area
	mov	ses:[edi].sclen, 0	;return null string
	mov	eax, BL_SC
	leave
	ret

xpfail:	mov	eax,FAIL
	leave
	ret

EXT_XPUT_    endp

_TEXT	ends
	end

	page	80,132
        title   ADDRESS - Get memory offset of name or variable
	.386
        name    ADDRESS 

;	Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
;	Use of this source code is governed by a BSD-style
;	license that can be found in the LICENSE file.

;
;       NameToAdr - Get memory address of name-type argument.
;
;       Usage:  (Argument is the name of a variable)
;
;	Sample calls:
;               OFFSET = NameToAdr(.X)           ;* address of variable X
;               OFFSET = NameToAdr(.T["abc"])    ;* address of T["abc"]
;
;       Returns offset in SPITBOL's data segment if successful, fails if
;       incoming argument is not a NAME variable.
;
;       ValToAdr - Get memory address of argument.
;
;       Usage: (Argument is the data value whose address is needed)
;
;	Sample calls:
;        OFFSET = ValToAdr(X)            ;* address of data in variable X
;        OFFSET = ValToAdr(T["abc"])     ;* address of data in T["abc"]
;
;       Returns offset in SPITBOL's data segment.
;
;
INCLUDE system.inc			; select MS-DOS or OS/2 conventions
INCLUDE	extrn386.inc

	preamble _TEXT

;  Describe entry stack
;
if nearptr
stk	struc
stkebp	dd	?			;save ebp
stkret	dd	?			;return pointer
presult	dd	?			;result pointer
pinfo	dd	?			;misc info pointer
parg    dd  ?           ;pointer to argument  
stk	ends
else
stk	struc
stkebp	dd	?			;save ebp
stkret	df	?			;return pointer
	    dw	?	       	;pad
presult	df	?			;far result pointer
	    dw	?			;pad
pinfo	df	?			;far misc info pointer
	    dw	?			;pad
parg    df  ?           ;far pointer to argument
        dw	?			;pad
stk	ends
endif

    entry   EXT_NAMETOADR_
;
	enter	0,0			;set up stack frame

; Test if parg is a name block.
;
	sload	es,edi,[ebp].parg	;pointer to arg block
	sload	fs,ecx,[ebp].pinfo	;pointer to misc info area
	sload	fs,ecx,sfs:[ecx].ptyptab ;pointer to type table
	mov	eax,ses:[edi].nmtyp	;type word
	cmp	eax,nm			;name block?
	jne	short xfail		;fail if not

	mov	eax,ses:[edi].nmbas	;base address of variable
	add	eax,ses:[edi].nmofs	;add in offset

	sload	es,edi,[ebp].presult	;point to result area
	mov	ses:[edi].icval, eax	;return integer
	mov	eax, BL_IC
	leave
	ret

xfail:	mov	eax,FAIL
	leave
	ret

EXT_NAMETOADR_ endp

;

        entry   EXT_VALTOADR_

	enter	0,0			;set up stack frame

	sload	es,eax,[ebp].parg	;pointer to arg block
	sload	es,edi,[ebp].presult	;point to result area
	mov	ses:[edi].icval, eax	;return input ptr as integer
	mov	eax, BL_IC
	leave
	ret

EXT_VALTOADR_ endp

_TEXT	ends
	end

	page	80,132
	title	TESTEF2 - TEST External Function Routines
	.386
	name	TESTEF2

;	Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
;	Use of this source code is governed by a BSD-style
;	license that can be found in the LICENSE file.

;
;  Test access to arguments and return of integer:
;	LOAD("TESTEF2(INTEGER,REAL,STRING)")
;
;	Call with TESTEF2(100, 1000.0, "ABCD")
;
;	Returns arg 1 plus 1 if arguments were found as expected,
;	otherwise a message about which argument was bad.
;
;
INCLUDE system.inc			; select MS-DOS or OS/2 conventions
INCLUDE	extrn386.inc

	preamble _TEXT

;
;  Describe entry stack
;
if nearptr
stk	struc
stkebp	dd	?			;save ebp
stkret	dd	?			;return pointer
presult	dd	?			;result pointer
pinfo	dd	?			;misc info pointer
parg3	dd	?			;pointer to arg 3 string
larg3	dd	?			;length of arg 3 string
rarg2	dq	?			;real arg 2
iarg1	dd	?			;integer arg 1
stk	ends
else
stk	struc
stkebp	dd	?			;save ebp
stkret	df	?			;return pointer
	dw	?	       		;pad
presult	df	?			;far result pointer
	dw	?			;pad
pinfo	df	?			;far misc info pointer
	dw	?			;pad
parg3	df	?			;far pointer to arg 3 string
	dw	?			;pad
larg3	dd	?			;length of arg 3 string
rarg2	dq	?			;real arg 2
iarg1	dd	?			;integer arg 1
stk	ends
endif

	entry	TESTEF2

	enter	0,0			;set up stack frame

	cld
	mov	esi, offset bad1	;prepare bad arg1 message
	mov	eax,[ebp].iarg1		;load arg 1 integer
	cmp	eax,100			;test for correct value
	jne	short test1		;jump if not as expected

	mov	esi, offset bad2	;prepare bad arg2 message
	mov	eax,dword ptr [ebp].rarg2 ;load least sig. half of arg 2
	cmp	eax,dword ptr onethou	;check for correct value 1000.0
	jne	short test1		;jump if not as expected
	mov	eax,dword ptr [ebp].rarg2+4 ;load most sig. half of arg 2
	cmp	eax,dword ptr onethou+4	;check other part of real
	jne	short test1

	mov	esi, offset bad3a;prepare bad arg3 message
	sload	es,edi,[ebp].parg3	;pointer to arg3 string
	mov	ecx,[ebp].larg3		;length of arg3 string
	cmp	ecx,zabcd		;check length
	jne	short test1
	mov	esi,offset abcd	;test characters
	repz	cmpsb			;compare string with expected
	mov	esi, offset bad3b;prepare bad arg3 message
	jne	short test1		;mismatch

	mov	eax,[ebp].iarg1		;reload arg 1
	inc	eax
	sload	es,edi,[ebp].presult	;point to result area
	mov	ses:[edi].icval,eax	;store result
	mov	eax,BL_IC		;return integer result
	leave
	ret

;
; Return ASCIIZ string at DS:ESI.  Copy up to zero.
;
test1:	sload	es,edi,[ebp].presult	;point to result area
	mov	ebx,edi			;save
	lea	edi,[edi].scstr		;point to string data in result area
	xor	edx,edx			;count characters moved

test2:	lodsb				;get next character
	or	al,al			;test for end of string
	je	short test3		;jump if so
	stosb				;move to result area
	inc	edx			;count it
	jmp	test2			;get next character

test3:	mov	ses:[ebx].sclen,edx	;store length in result area
  	mov	eax,BL_SC		;string result

	leave
	ret

TESTEF2	endp
_TEXT	ends

_DATA	segment dword public use32 'DATA'
onethou	dq	1000.0
abcd	db	"ABCD"
zabcd	equ	$-abcd
bad1	db	"First argument not integer 100",0
bad2	db	"Second argument not real 1000.0",0
bad3a	db	"Third argument string length wrong",0
bad3b	db	"Third argument not string 'ABCD'",0
_DATA	ends
	end

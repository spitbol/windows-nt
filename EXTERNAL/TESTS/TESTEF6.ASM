	page	80,132
	title	TESTEF6 - TEST External Function Routines
	.386
	name	TESTEF6

;	Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
;	Use of this source code is governed by a BSD-style
;	license that can be found in the LICENSE file.

;
;  Test access to arguments and return of far external data block:
;	LOAD("TESTEF6(INTEGER,REAL,STRING,OTHER)")
;
;	Call with:
;		X = TESTEF6(100, 1000.0, "ABCD",)
;		TERMINAL = TESTEF6(100, 1000.0, "ABCD", X)
;
;	Returns far external data block if first three arguments were found
;	as expected on first call, "OK" on second call.
;	Otherwise a message about which argument was bad.
;
;
;
INCLUDE system.inc			; select MS-DOS or OS/2 conventions
INCLUDE	extrn386.inc

	preamble _TEXT,_DATA

;
;  Describe entry stack
;
if nearptr
stk	struc
stkebp	dd	?			;save ebp
stkret	dd	?			;return pointer
presult	dd	?			;result pointer
pinfo	dd	?			;misc info pointer
parg4	dd	?			;pointer to arg 4 internal block
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
parg4	df	?			;pointer to arg 4 internal block
	dw	?			;pad
parg3	df	?			;pointer to arg 3 string
	dw	?			;pad
larg3	dd	?			;length of arg 3 string
rarg2	dq	?			;real arg 2
iarg1	dd	?			;integer arg 1
stk	ends
endif

	entry	TESTEF6

	enter	0,0			;set up stack frame

	cld
	mov	esi, offset bad1	;prepare bad arg1 message
	mov	eax,[ebp].iarg1		;load arg 1 integer
	cmp	eax,100			;test for correct value
	jne	test1			;jump if not as expected

	mov	esi, offset bad2	;prepare bad arg2 message
	mov	eax,dword ptr [ebp].rarg2 ;load least sig. half of arg 2
	cmp	eax,dword ptr onethou	;check for correct value 1000.0
	jne	test1 			;jump if not as expected
	mov	eax,dword ptr [ebp].rarg2+4 ;load most sig. half of arg 2
	cmp	eax,dword ptr onethou+4	;check other part of real
	jne	test1

	mov	esi, offset bad3a ;prepare bad arg3 message
	sload	es,edi,[ebp].parg3	;pointer to arg3 string
	mov	ecx,[ebp].larg3		;length of arg3 string
	cmp	ecx,zabcd		;check length
	jne	test1
	mov	esi,offset abcd	;test characters
	repz	cmpsb			;compare string with expected
	mov	esi, offset bad3b ;prepare bad arg3 message
	jne	test1			;mismatch

;
; Test if arg4 is the null string (first call).  Note that arg4
; is unconverted, so we work with raw SPITBOL data blocks.
;
	sload	es,edi,[ebp].parg4	;pointer to arg4 block
	sload	fs,ecx,[ebp].pinfo	;pointer to misc info area
	sload	fs,ecx,sfs:[ecx].ptyptab ;pointer to type table
	mov	eax,ses:[edi].sctyp	;type word
	cmp	eax,sc			;string?
	jne	short test0		;jump if not
	mov	esi, offset bad4	;presume bad arg4
	cmp	ses:[edi].sclen,0	;null string?
	jne	short test1		;if not, send error message

;
; This is the first call, made with null string fourth argument.
; Create 2000-byte external block at end of function, and return
; it as a far external block.
;
xbufsiz	equ	2000			;buffer size

	spush	ds
	spop	es
	lea	edi,buffer		;point to our buffer
	xor	al,al
	mov	ecx,xbufsiz
testa:	stosb				;store next byte
	inc	al			;increment it
	loop	testa			;continue until done

	sload	es,edi,[ebp].presult	;pointer to result area
	mov	ses:[edi].fxlen,xbufsiz	;size of data
	lea	eax,buffer		;address of buffer
	sstore	ses:[edi].fxptr,eax,ds	;return pointer to it
	mov	eax,BL_FX		;return far external data
	leave
	ret

;
; Fourth argument is not null string.  Is this the external data
; we provided on the first call?
;
test0:	mov	esi, offset bad5	;prepare error message
	cmp	eax,xn			;external data
	jne	short test1		;jump if not external
	mov	esi, offset bad6	;prepare error message
	mov	ecx,ses:[edi].xnlen	;length of data
	cmp	ecx,xbufsiz+(xndta-xntyp);check size (+8 for type/length words)
	jne	short test1		;jump if size wrong
	lea	edi,[edi].xndta		;es:edi points to data
	sub	ecx,(xndta-xntyp)	;adjust count to exclude type/length words
	xor	al,al			;first data value
	mov	esi,offset bad7	;prepare error message
testb:	scasb				;compare
	jne	short test1		;if data error
	inc	al			;next data value
	loop	testb			;continue compare

	mov	esi, offset ok	;looking good

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

TESTEF6	endp
_TEXT	ends

_DATA	segment	dword use32 'DATA'
onethou	dq	1000.0
abcd	db	"ABCD"
zabcd	equ	$-abcd
ok	db	"OK",0
bad1	db	"First argument not integer 100",0
bad2	db	"Second argument not real 1000.0",0
bad3a	db	"Third argument string length wrong",0
bad3b	db	"Third argument not string 'ABCD'",0
bad4	db	"Fourth argument was not null string",0
bad5	db	"Fourth argument not external data",0
bad6	db	"Fourth argument (external data) size incorrect",0
bad7	db	"Fourth argument (external data) contents bad",0
buffer	db	xbufsiz dup (?)
_DATA	ends
	end

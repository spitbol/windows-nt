	page	80,132
	title	LTRIM External Function
	.386
	name	LTRIM

; Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
; Use of this source code is governed by a BSD-style
; license that can be found in the LICENSE file.

;
; Perform a left end TRIM function
;
; Load from SPITBOL with statement:
;	LOAD('LTRIM(STRING,STRING)STRING')
;
;	Input:	First Argument = String to trim
;		Second Argument= Character to trim (defaults to
;				 blank and horizontal tab)
;
;	Output:	Left trimmed copy of first argument
;
;	(c) Copyright 1991 - Catspaw, Inc.
;	

INCLUDE system.inc			; select MS-DOS or OS/2 conventions
INCLUDE extrn386.inc

	preamble _TEXT

; Upon entry, the stack contains (all doublewords):
;
;	ESP  -->return pointer
;		arg2 pointer
;		arg1 pointer
;
if nearptr
ltstk	struc
lt_ebp	dd	?			; saved EBP
lt_rip	dd	?			; return EIP
lt_resp	dd	?			; pointer to result area
lt_misp	dd	?			; pointer to misc info area
lt_a2p	dd	?			; arg 2 pointer (offset)
lt_a2s	dd	?			; arg 2 length
lt_a1p	dd	?			; arg 1 pointer (offset)
lt_a1s	dd	?			; arg 1 length
ltstk	ends
else
ltstk	struc
lt_ebp	dd	?			; saved EBP
lt_rip	dd	?			; return EIP
lt_rcs	dd	?			; return CS
lt_resp	df	?			; pointer to result area
	dw	?			; pad
lt_misp	df	?			; pointer to misc info area
	dw	?			; pad
lt_a2p	df	?			; arg 2 pointer (offset:segment)
	dw	?			; filler
lt_a2s	dd	?			; arg 2 length
lt_a1p	df	?			; arg 1 pointer (offset:segment)
	dw	?			; filler
lt_a1s	dd	?			; arg 1 length
ltstk	ends
endif

        entry   EXT_LTRIM_

	enter	0,0			; create stack frame

	sload	gs,ebx,[ebp].lt_resp	; pointer to response area
	sload	ds,esi,[ebp].lt_a1p	; pointer to arg 1 string
	mov	dword ptr sgs:[ebx].fsptr,esi ; copy to far string
	mov	word ptr sgs:[ebx].fsptr+4,ds ;  result area
	mov	ecx,[ebp].lt_a1s	; copy arg 1 string length to
	mov	sgs:[ebx].fslen,ecx	;  result area as well

	jecxz	ltrim4			; return if arg 1 is null

	cld
	mov	edx,ecx			; preserve count
	cmp	[ebp].lt_a2s,0		; is arg 2 null?
	je	short ltrim2
;
; Here to process with trim character given in arg 2
;
	mov	edi,esi			; scasb uses es:edi
	spush	ds
	spop	es
	sload	fs,esi,[ebp].lt_a2p	; pointer to arg 2 string
	mov	al,sfs:[esi]		; get character to trim
  rep	scasb				; search for the character
	je	short ltrim1
	inc	ecx			; if not found, put back in count
	jmp	short ltrim1
	
;
; Here to process default trim characters of blank and tab
;
ltrim2:	lodsb				; load next char
	cmp	al,9
	je	short ltrim3		; jump if tab
	cmp	al,' '
	jne	short ltrim1		; jump if not blank
ltrim3:	loop	ltrim2			; keep searching
ltrim1:	sub	edx,ecx			; compute number of chars removed
	mov	sgs:[ebx].fslen,ecx	; update output count
	add	dword ptr sgs:[ebx].fsptr,edx ; and starting string offset
;
; Here to return
;
ltrim4:	mov	eax,BL_FS               ; return far string result
	leave
	ret
EXT_LTRIM_   endp

_TEXT	ends
	end

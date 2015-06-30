;  Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
;  Use of this source code is governed by a BSD-style
;  license that can be found in the LICENSE file.

;--------------------------  EXTRN386.INC -------------------------------------
; Equates of routines and data available to external function from SPITBOL-386


.xlist
.lall

nearptr=(os2 OR winnt)

if _MASM_ NE 0
	option	m510		; MASM 5.10 compatibility
endif


; Macros for code that must be assembled differently depending upon
; whether this function is being built for OS/2 or MS-DOS.
;
; MS-DOS external functions are reached via FAR calls, and use FAR
; pointers to SPITBOL data.
;
; OS/2 external functions are reached via NEAR calls, and use NEAR
; pointers to SPITBOL data.
;------------------------------------------------------------------
;
; Macro Glossary:
; entry     entry name		Define main procedure for this function.
;
; fstore    fstore arg		Store floating point result after call to
;				SPITBOL floating point function.
;
; preamble  preamble segname	Define group and seg name for 1 seg program.
;	    preamble seg1,seg2  Define group and segs for 2 segment program.
;
;
; procx	    proc name		Define near or far procedure.
;
; sload	    sload sreg,reg,arg	Load pointer into registers.
;
; sstore    sstore arg,reg,sreg Store pointer into memory.
;
; spop	    spop sreg		Pop segment register.
;
; spush	    spush sreg		Push segment register.
;
;
; Equate Glossary:
;
; Name	    Usage               Purpose
; --------  -----------------	-------------------------------------------
;
; rptr	    op rptr arg		Pointer size override.
;
; cbrptr    cbrptr		Size of rptr in bytes.
;
; scs	    op  reg, scs:arg	Code segment override.
;
; ses	    op  reg, ses:arg	ES segment override.
;
; sfs	    op  reg, sfs:arg	FS segment override.
;
; sgs	    op  reg, sgs:arg	GS segment override.
;
;
; Structure Glossary:
;
; Name	    Usage               Purpose
; --------  -----------------	-------------------------------------------
;
; dpt	    dpt <?>		Pointer data declaration.
;
; dps	    dps <?>		Pointer data declaration (on stack, padded
;				to even word boundary).


if os2
fstore	macro	arg
	.387
	fstp	arg
	fwait
	endm

preamble macro		segname1,segname2
	assume		CS:FLAT, DS:FLAT, ES:FLAT, SS:FLAT
CGROUP	equ		FLAT
segname1 segment	dword use32 'CODE'
	endm

elseif dos

fstore	macro	arg
	mov	dword ptr arg,eax
	mov	dword ptr arg+4,edx
	endm

preamble macro		segname1,segname2
ifb <segname2>
CGROUP	group		segname1
else
CGROUP	group		segname1,segname2
endif
	assume		cs:CGROUP,ds:CGROUP,es:CGROUP,ss:CGROUP
segname1 segment	dword use32 'CODE'
	endm
elseif winnt
fstore	macro	arg
	fstp	arg
	fwait
	endm

preamble macro		segname1,segname2
segname1 segment dword public use32 'CODE'
segname1 ends
ifb <segname2>
_DATA	segment dword public use32 'DATA'
_DATA	ends
else
segname2 segment dword public use32 'DATA'
segname2 ends
endif
_BSS	segment dword public use32 'BSS'
_BSS	ends
ifb <segname2>
DGROUP	group	_DATA,_BSS
else
DGROUP	group	segname2,_BSS
endif
	assume	cs:segname1,ds:DGROUP
segname1 segment dword public use32 'CODE'
	endm

endif



if nearptr

rptr	equ	dword ptr
cbrptr	equ	4

scs	equ	DS
ses	equ	DS
sfs	equ	DS
sgs	equ	DS

dpt	struc
o	dd	?
dpt	ends

dps	struc
	dd	?
dps	ends

procx	macro		procnm
procnm	proc		near
	endm

entry	macro		procnm
if winnt AND _MASM_ EQ 0
	publicdll	procnm
else
	public		procnm
endif
	procx		procnm
	endm

sload	macro	sreg,reg,arg 
	mov	reg,arg 
	endm

sstore	macro	arg,reg,sreg
	mov	arg,reg
	endm

spop    macro	sreg
	endm

spush	macro	sreg
	endm

else

rptr	equ	fword ptr
cbrptr	equ	6

scs     equ     CS
ses     equ     ES
sfs     equ     FS
sgs     equ     GS

dpt	struc
o	dd	?
s	dw	?
dpt	ends

dps	struc
	dd	?
	dw	?
	dw	?
dps	ends

procx	macro		procnm
procnm	proc		far
	endm

entry	macro		procnm
	org		0
	procx		procnm
	endm

sload	macro	sreg,reg,arg
	l&sreg	reg,arg
	endm

sstore	macro	arg,reg,sreg
	mov	dword ptr arg,reg 
	mov	word ptr arg+4,sreg
	endm

spop	macro	sreg
	pop	sreg
	endm

spush	macro	sreg
	push	sreg
	endm

endif
.list

INCLUDE block386.inc
.xlist
;	V1.02	10-Sep-92 Add string buffer blocks.
;
;	(c) Copyright 1990 - Catspaw, Incorporated
;
;
; At function entry, the stack looks like this:
;
if nearptr
;		+-----------------------+
; SS:ESP ----->	|     return offset	|
;		|-----------------------|
;		|  result area offset	|
;		|-----------------------|
;		| miscinfo area offset	|
;		|-----------------------|
;		|     last argument	|
;		|-----------------------|
;		|	. . . .		|
;		|-----------------------|
;		|    first argument	|
;		+-----------------------+
else
;		+-----------------------+
; SS:ESP ----->	|     return offset	|
;		|-----------------------|
;		|     return segment	|
;		|-----------------------|
;		|  result area offset	|
;		|-----------------------|
;		|  result area segment	|
;		|-----------------------|
;		| miscinfo area offset	|
;		|-----------------------|
;		| miscinfo area segment	|
;		|-----------------------|
;		|     last argument	|
;		|-----------------------|
;		|	. . . .		|
;		|-----------------------|
;		|    first argument	|
;		+-----------------------+
endif
;
;
; Generally, the items in the miscinfo area can be ignored, and are provided
; only for the benefit of those wishing to operate directly on SPITBOL's
; internal data structures.
;
; However, the result area pointer *must* be used by the external
; function to locate the area in which results are returned.
;
if nearptr
; Note that NEAR pointers on the stack are 4-byte quantities.
else
; Note that FAR pointers on the stack are 6-byte quantities with a 2-byte
; padding to make sure the stack pointer remains aligned at a boundary
; that is a multiple of 4.
endif
;
; Definition of structure of the miscinfo area.  A pointer to this
; area is placed on the stack below the result area pointer.
;
if nearptr

misc	struc
vers	dw	?		;internal version number
env	db	?		;host environment (t_pharlap, t_intel, t_os2)
	db	?		;reserved
nargs	dd	?		;number of arguments to function
ptyptab	dd	?		;pointer to table of data types
pxnblk	dd	?		;pointer to xnblk describing function
pefblk	dd	?		;pointer to efblk describing function
pflttab	dd	?		;pointer to floating point table
spds	dw	?		;SPITBOL's DS
spcs	dw	?		;SPITBOL's CS
misc	ends

else

misc	struc
vers	dw	?		;internal version number
env	db	?		;host environment (t_pharlap, t_intel, t_os2)
	db	?		;reserved
nargs	dd	?		;number of arguments to function
ptyptab	df	?		;pointer to table of data types
pxnblk	df	?		;pointer to xnblk describing function
pefblk	df	?		;pointer to efblk describing function
pflttab	df	?		;pointer to floating point table
spds	dw	?		;SPITBOL's DS
spcs	dw	?		;SPITBOL's CS
misc	ends

endif
; values in env word of misc structure.  Executing under:
t_pharlap equ	0		;PharLap DOS Extender
t_intel	equ	1		;Intel DOS Extender
t_os2	equ	2		;OS/2 2.0
t_tc16  equ	3		;MS-DOS TurboC with 16-bit IA
t_tc32	equ	4		;MS-DOS TurboC with 32-bit IA
t_w1616 equ	5		;16-bit Windows, 16-bit SPITBOL
t_w1632 equ	6		;16-bit Windows, 32-bit SPITBOL
t_wnt8	equ	7		;Windows NT on 386/486
t_sparc	equ	8		;Sun 4 / SPARC
t_mac	equ	9		;Apple Macintosh
t_mips  equ    10               ;MIPS R3000
t_rs6000 equ   11               ;IBM RS/6000

;
; Sample usage.  Example stack frame for function arguments, assuming
;		 the execution of an ENTER instruction to put the stack
;		 pointer in EBP.  This example assumes the function is
;		 called with three arguments of type (INTEGER, REAL, STRING):
;

if nearptr

;	  	struc
;	stk_ebp	dd	?		;save ebp
;	stk_ret	dd	?		;return pointer
;	presult	dd	?		;pointer to result area
;	pmisc	dd	?		;pointer to miscinfo area
;	parg3	dd	?		;pointer to arg 3 string
;	larg3	dd	?		;length of arg 3 string
;	rarg2	dq	?		;real arg 2
;	iarg1	dd	?		;integer arg 1
;		ends

else

;	  	struc
;	stk_ebp	dd	?		;save ebp
;	stk_ret	df	?		;return pointer
;		dw	?		;pad
;	presult	df	?		;pointer to result area
;		dw	?		;pad
;	pmisc	df	?		;pointer to miscinfo area
;		dw	?		;pad
;	parg3	df	?		;pointer to arg 3 string
;		dw	?		;pad
;	larg3	dd	?		;length of arg 3 string
;	rarg2	dq	?		;real arg 2
;	iarg1	dd	?		;integer arg 1
;		ends

endif
;
;		The pointer to the result area could be loaded with:
;
;		sload	fs, edi, [ebp].presult
;
;		The pointer to the type table would be loaded by
;		double indexing:
;
;		sload	fs, ecx, [ebp].pmisc
;		sload	fs, ecx, sfs:[ecx].ptyptab
;


;
; Simple names for datatypes.  Performs a lookup in SPITBOL's type
; table to fetch a 32-bit type word for specific data types.
;
; The address of SPITBOL's type table is passed to the function on the
; stack in ptyptab (see above).  Prior to using these symbols, a
; pointer to this table must be loaded into the registers.  These equates
; assume that the pointer has been loaded into SFS:[ECX].  There is nothing
; special about these registers, and any others could be used
; provided the equates are modified.
;
					;data types in type table at SFS:[ECX]
ar	equ	sfs:[ecx+BL_AR*4]	; Array
bc	equ	sfs:[ecx+BL_BC*4]	; Buffer Control
bf	equ	sfs:[ecx+BL_BF*4]	; String Buffer
cd	equ	sfs:[ecx+BL_CD*4]	; Code
ex	equ	sfs:[ecx+BL_EX*4]	; Expression
ic	equ	sfs:[ecx+BL_IC*4]	; Integer
nm	equ	sfs:[ecx+BL_NM*4]	; Name
rc	equ	sfs:[ecx+BL_RC*4]	; Real
sc	equ	sfs:[ecx+BL_SC*4]	; String
tb	equ	sfs:[ecx+BL_TB*4]	; Table
vc	equ	sfs:[ecx+BL_VC*4]	; Vector
xn	equ	sfs:[ecx+BL_XN*4]	; External

;
; Non-standard block-type values that may be returned in register EAX.
;
FAIL	EQU	-1	       		; Signal function failure
BL_NC	EQU	100			; Unconverted result
BL_FS	EQU	101			; Pointer to string
BL_FX	EQU	102			; Pointer to external block

;
; Length of string area in result buffer
;
buflen	equ	512


; Vectors to real number functions.
;
; Reals are 8-byte, double-precision reals in IEEE format.
; Integers (for fix and float) are 32-bit two's complement numbers.
;
; Function arguments are pushed onto the stack left-to-right.  With 8-byte
; reals, the most-significant half (MSH) is pushed first.  Note that this
; differs from standard C calling conventions.
;
; Integer results are returned in EAX.  Real results are returned in
; EDX:EAX  (EDX = MSH, EAX = LSH).
;
; The caller is responsible for removing arguments from the stack
; after the function returns.
;
; Functions taking two arguments, such as r_sub(r1,r2) order their
; operations as r1-r2.
;
; The address of a table of dispatch vectors is passed to the external
; function on the stack in pflttab (see above).  These equates assume
; that a pointer to the table has been loaded into DS:[EBX] by your code.
;
; Using the example above, this might be done with the following:
;
;			sload	ds, ebx, [ebp].pmisc
;			sload	ds, ebx, [ebx].pflttab
;
ife nearptr
; IMPORTANT:  The routines must be called with DS set to SPITBOL's
;	      data segment.  This will happen automatically if you
;	      preserve your own DS prior to pushing arguments, and
;	      load the address of the floating point vector table
;	      into DS:[EBX].
endif
;
; The real functions preserve EBX, ESI, EDI and EBP, and may destroy
; EAX, ECX, and EDX.
;
; Example.  Divide the real number in location r1 by the number in r2, and
;	    store the result in r3:
;
;	spush	ds			;save our DS
;	push	dword ptr r1+4		;push first arg, msh
;	push	dword ptr r1		;push first arg, lsh
;	push	dword ptr r2+4		;push second arg, msh
;	push	dword ptr r2		;push second arg, lsh
;	sload	ds,ebx,[ebp].pmisc	;get address of miscinfo area
;	sload	ds,ebx,[ebx].pflttab	;get address of vector table
;	call	r_div			;perform division
;	add	esp, 2*8		;remove arguments
;	spop	ds			;restore our DS
;	mov	dword ptr r3, eax	;store result lsh
;	mov	dword ptr r3+4, edx     ;store result msh
;
;
;
fix	equ	rptr ds:[ebx+ 0*cbrptr]	; real to integer	 i = r_2_i(r)
float	equ	rptr ds:[ebx+ 1*cbrptr]	; integer to real	 r = i_2_r(i)
r_add	equ	rptr ds:[ebx+ 2*cbrptr]	; real addition  	 r = r_add(r1,r2)
r_sub	equ	rptr ds:[ebx+ 3*cbrptr]	; real subtraction	 r = r_sub(r1,r2)
r_mul	equ	rptr ds:[ebx+ 4*cbrptr]	; real multiplication	 r = r_mul(r1,r2)
r_div	equ	rptr ds:[ebx+ 5*cbrptr]	; real division  	 r = r_div(r1,r2)
r_neg	equ	rptr ds:[ebx+ 6*cbrptr]	; real negation  	 r = r_neg(r)
r_atn	equ	rptr ds:[ebx+ 7*cbrptr]	; real arc tangent	 r = r_atn(r)
r_chp	equ	rptr ds:[ebx+ 8*cbrptr]	; real chop  		 r = r_chp(r)
r_cos	equ	rptr ds:[ebx+ 9*cbrptr]	; real cosine  		 r = r_cos(r)
r_etx	equ	rptr ds:[ebx+10*cbrptr]	; real e to the x	 r = r_etx(r)
r_lnf	equ	rptr ds:[ebx+11*cbrptr]	; real natural logarithm r = r_lnf(r)
r_sin	equ	rptr ds:[ebx+12*cbrptr]	; real sine 		 r = r_sin(r)
r_sqr	equ	rptr ds:[ebx+13*cbrptr]	; real square root	 r = r_sqr(r)
r_tan	equ	rptr ds:[ebx+14*cbrptr]	; real tangent  	 r = r_tan(r)

.list
;-------------------------- End of EXTRN386.INC -----------------------------

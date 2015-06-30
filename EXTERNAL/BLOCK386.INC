;	Copyright 1994-2003 Mark Emmer, Catspaw, Inc. All rights reserved.
;	Use of this source code is governed by a BSD-style
;	license that can be found in the LICENSE file.

;-------------------------- block386.inc -----------------------------
.xlist
;	Version 0.90	18-Dec-89
;			Initial version
;
;	Version 1.00	28-Feb-90
;			First release version.  Add additional blocks.
;
;	Version 1.10	10-Sep-92
;			Add string buffer blocks.
;
;
;	Copyright 1989 Catspaw, Inc.   All Rights Reserved.
;
;	SPITBOL BLOCKS
;
;	All SPITBOL objects are encapsulated into blocks of memory.
;	The first word of each block identifies the block type in a
;	curious way.  Rather than containing a simple integer type
;	code, it contains the address of a subroutine that performs
;	some particular action on the block, such as placing it on
;	the stack.  Because each block type uses a different subroutine,
;	the subroutine addresses serve as a type identifier.
;
;	Each subroutine is proceeded in memory by a one-byte integer
;	type code (given below).  Thus to obtain a simple ordinal
;	type code for a block pointed to by an address in EBX, the following
;	code sequence will load the ordinal value into register AL:
;
;	....			Block pointer in EBX
;	MOV	EAX,[EBX]	Type word (subroutine pointer) to EAX
;	MOV	AL,[EAX-1]	Load integer type code to AL
;
;	To obtain the type code as a full 32-bit integer, replace the last
;	instruction with:
;
;	MOVZX	EAX,BYTE PTR [EAX-1]	Load integer type code to EAX
;
;	+---------------------+               +------------------ +
;	|      Type Word      |----+          | Integer type byte |
;	+---------------------+    |          +-------------------+--------+
;	|      Block Data     |    +--------->|   Program Code for this    |
;      ...                   ...              |   Block Type               |
;	|                     |               |                            |
;	+---------------------+               +----------------------------+
;
;
;	Given an integer type, the type word can be obtained by
;	looking it up in a table provided to external functions in ptyptab
;	(provided in the miscellaneous area).
;	For example, if the ptyptab pointer is loaded into ES:ECX, then given
;	a small integer type code in EAX, the type word is obtained with:
;
;	MOV	EAX,ES:[ECX+EAX*4]     Obtain type word
;
;
;	BLOCK CODES FOR ACCESSIBLE DATATYPES
;
;	These blocks may appear in an argument list if left untranslated
;	by the LOAD function definition.
;
BL_AR	EQU	0		;ARBLK	ARRAY
BL_CD	EQU	BL_AR+1		;CDBLK	CODE
BL_EX	EQU	BL_CD+1		;EXBLK	EXPRESSION
BL_IC	EQU	BL_EX+1		;ICBLK	INTEGER
BL_NM	EQU	BL_IC+1		;NMBLK	NAME
BL_P0	EQU	BL_NM+1		;P0BLK	PATTERN, 0 args
BL_P1	EQU	BL_P0+1		;P1BLK	PATTERN, 1 arg
BL_P2	EQU	BL_P1+1		;P2BLK	PATTERN, 2 args
BL_RC	EQU	BL_P2+1		;RCBLK	REAL
BL_SC	EQU	BL_RC+1		;SCBLK	STRING
BL_SE	EQU	BL_SC+1		;SEBLK	EXPRESSION
BL_TB	EQU	BL_SE+1		;TBBLK	TABLE
BL_VC	EQU	BL_TB+1		;VCBLK	VECTOR (array)
BL_XN	EQU	BL_VC+1		;XNBLK	EXTERNAL, non-relocatable contents
BL_XR	EQU	BL_XN+1		;XRBLK	EXTERNAL, relocatable contents
BL_BC	EQU	BL_XR+1		;BCBLK	BUFFER CONTROL
BL_PD	EQU	BL_BC+1		;PDBLK	PROGRAM DEFINED DATATYPE

BL__D	EQU	BL_PD+1		;NUMBER OF BLOCK CODES FOR DATA
;
;
;	OTHER BLOCK CODES
;
;	These blocks will never appear in an argument list, but are
;	listed here for completeness.
;
BL_TR	EQU	BL_PD+1		;TRBLK	TRACE
BL_BF	EQU	BL_TR+1		;BFBLK	STRING BUFFER
BL_CC	EQU	BL_BF+1		;CCBLK	CODE CONSTRUCTION
BL_CM	EQU	BL_CC+1		;CMBLK	COMPILER TREE NODE
BL_CT	EQU	BL_CM+1		;CTBLK	CHARACTER TABLE
BL_DF	EQU	BL_CT+1		;DFBLK	DATATYPE FUNCTION
BL_EF	EQU	BL_DF+1		;EFBLK	EXTERNAL FUNCTION
BL_EV	EQU	BL_EF+1		;EVBLK	EXPRESSION VARIABLE
BL_FF	EQU	BL_EV+1		;FFBLK	FIELD FUNCTION
BL_KV	EQU	BL_FF+1		;KVBLK	KEYWORD VARIABLE
BL_PF	EQU	BL_KV+1		;PFBLK	PROGRAM-DEFINED FUNCTIN
BL_TE	EQU	BL_PF+1		;TEBLK	TABLE ELEMENT
;
;
;
;  	Structure of common SPITBOL blocks:
;  	  Integer, Real, String, and File blocks.
;
;  	These structures can be applied to the result area to determine
;  	where to store required return information.
;
;
;  	Structure of ICBLK (integers)
;
icblk	struc
ictyp	dd	?		;type word
icval	dd	?		;integer value
icblk	ends
;
;
;  	Structure of RCBLK (reals)
;
rcblk	struc
rctyp	dd	?		;type word
rcval	dq	?		;real value
rcblk	ends
;
;  	Structure of SCBLK (strings)
;
scblk	struc
sctyp	dd	?		;type word
sclen	dd	?		;string length
scstr	db	?		;start of string
scblk	ends
;
;  	Structure for returning a string via a pointer
;
if nearptr
fsblk	struc
fstyp	dd	?		;type word
fslen	dd	?		;string length
fsptr	dd	?		;near pointer to string
fsblk	ends
else
fsblk	struc
fstyp	dd	?		;type word
fslen	dd	?		;string length
fsptr	df	?		;far pointer to string
fsblk	ends
endif
;
;  	Structure for returning an external block via a pointer
;
if nearptr
fxblk	struc
fxtyp	dd	?		;type word
fxlen	dd	?		;external data length
fxptr	dd	?		;near pointer to external data
fxblk	ends
else
fxblk	struc
fxtyp	dd	?		;type word
fxlen	dd	?		;external data length
fxptr	df	?		;far pointer to external data
fxblk	ends
endif
;
;
;	FILE CONTROL BLOCK
;
;	The user may provide the word "FILE" for any argument in
;	the LOAD function prototype.  When a call is made to the
;	external function with an I/O associated variable in this argument
;	position, SPITBOL will provide a pointer to the file control
;	block instead of the value of the variable.
;
;	The file control block (FCB) points to an I/O block with
;	additional information.  In turn, the I/O block points to any
;	buffer used by the file.
;
;	This block is obtained for every file except those associated
;	with INPUT, OUTPUT, or TERMINAL.  Note that these FCB's are
;	unrelated to MS-DOS FCB's.  File control blocks do not have
;	their own type word, but appear as XRBLKs with the following structure:

;
fcblk	struc
fcbtyp	dd	?		;type word (XRBLK)
fcblen	dd	?		;size of block, in bytes
fcbrsz	dd	?		;SPITBOL record size and mode
				;positive if text mode, negative if binary
fcbiob	dd	?		;pointer to IOBLK
fcbmod  dd      ?               ;1 if text mode, 0 if binary mode
fcblk   ends
;
;	I/O BLOCK
;
;	An I/O block is pointed to by the fcbiob field of a file control block.
;
ioblk	struc
iobtyp	dd	?		;type word (XRBLK)
ioblen	dd	?		;size of IOBLK in bytes
iobfnm	dd	?		;pointer to SCBLK holding filename
iobpid	dd	?		;process id for pipe (not used for DOS)
iobbfb	dd	?		;pointer to BFBBLK
iobfdn	dd	?		;MS-DOS file descriptor number
iobflg1	dd	?		;flags 1 (see below)
iobflg2	dd	?		;flags 2 (see below)
iobeol1	dd	?		;end of line character 1
iobeol2	dd	?		;end of line character 2
iobshare dd     ?               ;sharing mode
iobaction dd    ?;              ;file open actions
ioblk   ends

;  	Bits in iobflg1 dword:
;
IO_INP	equ	00000001h	;input file
IO_OUP	equ	00000002h	;output file
IO_APP	equ	00000004h	;append output to end of existing file
IO_OPN	equ	00000008h	;file is open
IO_COT	equ	00000010h	;console output to non-disk device
IO_CIN	equ	00000020h	;console input from non-disk device
IO_SYS	equ	00000040h	;system file descriptor used instead of name
IO_WRC	equ	00000080h	;output without buffering
IO_EOT	equ     00000100h       ;ignore end-of-text (control-Z) character

;  	Bits in iobflg2 dword:
;
IO_PIP	equ	00000001h	;pipe (not used in MS-DOS)
IO_DED	equ	00000002h	;dead pipe (not used in MS-DOS)
IO_ILL	equ	00000004h	;illegal I/O association
IO_RAW	equ	00000008h	;binary I/O
IO_LF	equ	00000010h	;ignore line feed if next character
IO_NOE	equ	00000020h	;no echo input
IO_ENV	equ	00000040h	;filearg1 maps to filename thru environment var
IO_DIR  equ	00000080h	;buffer is dirty (needs to be written)
IO_BIN	equ	00000100h	;binary I/O

;       Bits in iobshare dword:
IO_COMPATIBILITY   equ     00h
IO_DENY_READWRITE  equ     01h
IO_DENY_WRITE      equ     02h
IO_DENY_READ       equ     03h
IO_DENY_NONE       equ     04h
IO_DENY_MASK       equ     07h  ;mask for above deny mode bits
IO_EXECUTABLE      equ     40h  ;file to be marked executable
IO_PRIVATE         equ     80h  ;file is private to current process

;       Bits in iobaction dword:
IO_FAIL_IF_EXISTS      equ 00h
IO_OPEN_IF_EXISTS      equ 01h
IO_REPLACE_IF_EXISTS   equ 02h
IO_FAIL_IF_NOT_EXIST   equ 00h
IO_CREATE_IF_NOT_EXIST equ 10h
IO_EXIST_ACTION_MASK   equ 13h  ;mask for above bits
IO_WRITE_THRU          equ 20h  ;writes complete before return
                             
;
;	I/O BUFFER BLOCK
;
;	An I/O buffer block (BFBBLK) is pointed to by an IOBLK.
;
bfbblk	struc
bfbtyp	dd	?		;type word (XNBLK)
bfblen	dd	?		;size of BFBBLK, in bytes
bfbsiz	dd	?		;size of buffer in bytes
bfbfil	dd	?		;number of bytes currently in buffer
bfbnxt	dd	?		;offset of next buffer char to r/w
if SETREAL eq 1
bfboff  dq      ?               ;file position of first byte in buf
bfbcur  dq      ?               ;physical file position
else
bfboff	dd	?		;file position of first byte in buf
bfbcur	dd	?		;physical file position
endif
bfbbuf	db	?		;start of buffer
bfbblk	ends
;
;
;  	Structure of EFBLK (external function).  A pointer to this block
;  	is passed to the external function in the miscellaneous area
;	in pefblk.
;
efblk	struc
fcode	dd	?		;type word
fargs	dd	?		;number of arguments
eflen	dd	?		;block length
efuse	dd	?		;usage count
efcod	dd	?		;pointer to XNBLK, see below
efvar	dd	?		;pointer to VRBLK with function name
efrsl	dd	?		;result type  (see below)
eftar	dd	?		;argument types, one dword per arg (see below)
efblk	ends
;
;  efrsl and eftar[] dwords contain small integer type codes as follows:
;  0 - unconverted
;  1 - convert to string
;  2 - convert to integer
;  3 - convert to real
;  4 - produce as file control block associated with variable
;
;
;  	Structure of XNBLK allocated for external function
;  	A pointer to this structure is passed to the external function
;  	in the miscellaneous area in pxnblk.
;

if winnt
xnblk	struc			;keep as multiple of dwords
xntyp	dd	?		;type word
xnlen	dd	?		;length of this block
xnhand  dd      ?               ;module handle
xnpfn   dd      ?               ;pointer to function
xn1st   dd      ?               ;non-zero = first-ever call
xnsave  dd      ?               ;non-zero = first call after reload of save file
xncbp   dd      ?               ;call back address prior to saving or exiting
xnblk   ends
else
xnblk	struc			;keep as multiple of dwords
xntyp	dd	?		;type word
xnlen	dd	?		;length of this block
xnoff   dd      ?               ;base offset of function image
xnsiz	dd	?		;size of function in bytes
xneip	dd	?		;transfer EIP
xncs	dw	?		;transfer CS
xnesp	dd	?		;transfer ESP, 0 to use SPITBOL's stack
xnss	dw	?		;transfer SS, 0 to use SPITBOL's stack
xnds	dw	?		;transfer DS
xnes	dw	?		;transfer ES
xnfs	dw	?		;transfer FS
xngs	dw	?		;transfer GS
xn1st   dw      ?               ;non-zero = first-ever call
xnsave	dw	?		;non-zero = first call after reload of save file
xncbp	df	?		;call back address prior to saving or exiting
	dw	?		;pad
xnblk   ends
endif
xndta   equ     (xnlen+4)       ;start of user data region in XNBLK
;

;
;  Other selected blocks of interest:
;
;
;	ARRAY BLOCK
;
;	An array block (ARBLK) represents an array value other than one
;	with one dimension whose lower bound is one (see VCBLK).
;
arblk	struc
artyp	dd	?		;type word (ARBLK)
aridv	dd	?		;identifier value
arlen	dd	?		;length of ARBLK in bytes
arofs	dd	?		;offset in arblk to arpro field
arndm	dd	?		;number of dimensions
arlbd	dd	?		;low bound (first subscript)
ardim	dd	?		;dimension (first subscript)
; above two dwords are repeated for each additional dimension
arpro	dd	?		;array prototype string
arvls	dd	?		;start of values in row-wise order
arblk	ends

;       BUFFER CONTROL BLOCK
;
;	A buffer control block (BCBLK) is created by the BUFFER
;	function, and serves as an indirect control header for the
;	buffer. It contains the number of characters currently
;	stored in the buffer.
;
bcblk	struc
bctyp	dd	?		;type word (BCBLK)
bcidv	dd	?		;identifier value
bclen	dd	?		;number of chars in use in bfblk
bcbuf	dd	?		;pointer to bfblk
bcblk	ends

;	STRING BUFFER BLOCK
;
;	A string buffer block (BFBLK) contains the actual buffer
;	memory area. It specifies the largest string that can be
;	stored in the buffer.
;
bfblk	struc
bftyp	dd	?		;type word (BFBLK)
bfalc	dd	?		;allocated size of buffer
bfchr	db	?		;characters of string
bfblk	ends

;
;	CODE BLOCK
; 
;	A code block (CDBLK) is present for every source statement.
;
cdblk	struc
cdjmp	dd	?		;ptr to routine to execute statement
cdstm	dd	?		;statement number
cdsln	dd	?		;source file line number
cdlen	dd	?		;length of CDBLK in bytes
cdfal	dd	?		;failure exit
cdcod	dd	?		;start of executable pseudo-code
cdblk	ends

;
;	NAME BLOCK
;
;	A name block (NMBLK) is used whereever a name must be stored as
;	a value following use of the unary dot operator.
;
nmblk	struc
nmtyp	dd	?		;type word (NMBLK)
nmbas	dd	?		;base pointer for variable
nmofs	dd	?		;offset for variable
nmblk	ends

;
;	TABLE BLOCK
;
;	A table block (TBBLK) is used to represent a table value.
;	It comprises a list of buckets, each of which may point to
;	a chain of TEBLKs.  TBBUK entries either point to the first
;	TEBLK on the chain or they point to the TBBLK itself to
;	indicate the end of the chain.  The number of buckets can
;	be deduced from tblen.
;
tbblk	struc
tbtyp	dd	?		;type word (TBBLK)
tbidv	dd	?		;identifier value
tblen	dd	?		;length of TBBLK in bytes
tbinv	dd	?		;default initial lookup value
tbbuk	dd	?		;start of hash bucket pointers
tbblk	ends

;
;	TABLE ELEMENT BLOCK
;
;	A table element (TEBLK) is used to represent a single entry in
;	a table.
;
teblk	struc
tetyp	dd	?		;type word (TEBLK)
tesub	dd	?		;subscript value
teval	dd	?		;table element value
tenxt	dd	?		;link to next TEBLK or TBBLK if end of chain
teblk	ends

;	VECTOR BLOCK
;
;	A vector block (VCBLK) is used to represent an array value which has
;	one dimension whose lower bound is one. All other arrays are
;	represented by ARBLKs.  The number of elements can be deduced
;       from vclen.
;
vcblk	struc
vctyp	dd	?		;type word (VCBLK)
vcidv	dd	?		;identifier value
vclen	dd	?		;length of vcblk in bytes
vcvls	dd	?		;start of vector values
vcblk	ends

.list
;---------------------  end of block386.inc -----------------------

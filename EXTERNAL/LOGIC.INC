*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

*  LOGIC.INC
*
* Perform logical or arithmetic operations on integers and strings
*
* Usage:
*       Result = Logic(ARG1, ARG2, ARG3)
*
*       Input:  Arg1= Operation Specifier:
*                 1 = Return Logical NOT of ARG2
*                 2 = Return ARG2 AND  Arg3
*                 3 = Return ARG2 OR   Arg3
*                 4 = Return ARG2 XOR  Arg3
*                 5 = Return ARG2 NAND Arg3
*                 6 = Return ARG2 NOR  Arg3
*                 7 = Return ARG2 + ARG3 (overflow ignored)
*                 8 = Return ARG2 - ARG3 (overflow ignored)
*                 9 = Return ARG2 * ARG3 (unsigned)
*                10 = Return ARG2 / ARG3 (unsigned)
*                11 = Return ARG2 shifted left ARG3 bits
*                12 = Return ARG2 shifted right ARG3 bits (unsigned)
*                13 = Return ARG2 shifted right ARG3 bits (sign extension)
*                14 = Return ARG2 rotated left ARG3 bits
*                15 = Return ARG2 rotated right ARG3 bits
*                16 = Return integer from digit string in ARG2, base ARG3
*                17 = Return digit string from integer in ARG2, base ARG3
*                       (conversion base defaults to 16 if ARG3 is 0)  
*
*	ARG2 and ARG3 may be two strings, two integers, or an integer
*			and a string.
*
*	Output:	Result of operation.
*		Function fails if:
*		1. Operation specifier out of range
*		2. Arg2 and Arg3 are strings of different length
*		3. Arg2 or Arg3 are strings longer than 512 characters.
*		4. The arguments are not strings or integers
*
*
*	The common names for these functions are also defined:
*
*	NOT, AND, OR, XOR, NAND, NOR, UPLUS, UMINUS, UMUL, UDIV
*
*	Additional conversion functions included:
*	  HI(X)		hex digit string to integer
*	  IH(X)		integer to hex digit string
*	  DIB(X,B)	digit string to integer according to base B
*	  IDB(X,B)	integer to digit string according to base B
*
        LOAD('EXT_LOGIC_(INTEGER,ARG2,ARG3)', 'spitbol.slf')
        OPSYN("Logic", "EXT_LOGIC_")

        DEFINE('LogicDef(PROTO)NAME,ARGS')      :(LogicDef_END)
LogicDef PROTO ? BREAK('(') . NAME  BAL . ARGS  =  NAME
	CODE(NAME  ' '  PROTO  '		:S(RETURN)F(FRETURN)')
	DEFINE(NAME  ARGS)			:(RETURN)
LogicDef_END
*
        LogicDef("NOT(X)        = Logic(1,X)")
        LogicDef("AND(X1,X2)    = Logic(2,X1,X2)")
        LogicDef("OR(X1,X2)     = Logic(3,X1,X2)")
        LogicDef("XOR(X1,X2)    = Logic(4,X1,X2)")
        LogicDef("NAND(X1,X2)   = Logic(5,X1,X2)")
        LogicDef("NOR(X1,X2)    = Logic(6,X1,X2)")
        LogicDef("UPLUS(X1,X2)  = Logic(7,X1,X2)")
        LogicDef("UMINUS(X1,X2) = Logic(8,X1,X2)")
        LogicDef("UMUL(X1,X2)   = Logic(9,X1,X2)")
        LogicDef("UDIV(X1,X2)   = Logic(10,X1,X2)")
        LogicDef("SHL(X1,X2)    = Logic(11,X1,X2)")
        LogicDef("SHR(X1,X2)    = Logic(12,X1,X2)")
        LogicDef("SAR(X1,X2)    = Logic(13,X1,X2)")
        LogicDef("ROL(X1,X2)    = Logic(14,X1,X2)")
        LogicDef("ROR(X1,X2)    = Logic(15,X1,X2)")
        LogicDef("HI(X)         = Logic(16,CONVERT(X,'STRING'))")
        LogicDef("IH(X)         = Logic(17,CONVERT(X,'INTEGER'))")
        LogicDef("DIB(X,B)      = Logic(16,CONVERT(X,'STRING'),B)")
        LogicDef("IDB(X,B)      = Logic(17,CONVERT(X,'INTEGER'),B)")
*
        UNLOAD("LogicDef")

* end of LOGIC.INC

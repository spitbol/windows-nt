* Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
* Use of this source code is governed by a BSD-style
* license that can be found in the LICENSE file.

* HOST.INC - Definitions for SPITBOL HOST functions
*
* Define helper function to make definitions concise.
*
* This curious piece of code is called with a prototype string
* and defines a function in terms of another.  For example, called with:
*
*	HOSTDEF("GOTO(I1,I2) = HOST(6,I1,I2)")
*
* it codes a statement of the form:
*
*	GOTO	GOTO = HOST(6,I1,I2)		:S(RETURN)F(FRETURN)
*
* and defines the function GOTO by executing the equivalent of:
*
*	DEFINE("GOTO(I1,I2)")
*
* This idea is based upon the DEXP function in Algorithms in SNOBOL4,
* by James Gimpel.
*
* This code is written so that most of it disappears on the first
* garbage collection.
*
	:(HOSTGO)			;* Optimize for collect()

HOSTGO	DEFINE('HOSTDEF(PROTO)NAME,ARGS')	:(HOSTDEF_END)
HOSTDEF	PROTO ? BREAK('(') . NAME  BAL . ARGS  =  NAME
	CODE(NAME  ' '  PROTO  '		:S(RETURN)F(FRETURN)')
	DEFINE(NAME  ARGS)			:(RETURN)
HOSTDEF_END


	HOSTDEF("EXECUTE(S) = HOST(1,S)")
	HOSTDEF("GETARG(I) = HOST(2,I)")
	HOSTDEF("FIRSTARG() = HOST(3)")
	HOSTDEF("PARM() = HOST(0)")
	HOSTDEF("SHELLVAR(S) = HOST(4,S)")
	HOSTDEF("SYSTEM() = HOST()")

* PC-SPITBOL and SPITBOL-386 specific functions
*
	HOST() ? FENCE ("80386" | "IBM PC")		:F(HOSTXIT)
	HOSTDEF("CLEARSCN() = HOST(10)")
        HOSTDEF("COPYLINE(I) = HOST(33,I)")
        HOSTDEF("COPYSCN() = HOST(34)")
	HOSTDEF("CURSOR(I,J) = HOST(5,I,J)")
	HOSTDEF("DELETECHR() = HOST(11)")
	HOSTDEF("ERASEEOL() = HOST(12)")
	HOSTDEF("GETCOLS() = HOST(202)")
        HOSTDEF("GETCURCOL() = HOST(212)")
        HOSTDEF("GETCURROW() = HOST(213)")
	HOSTDEF("GETETYPE() = HOST(203)")
	HOSTDEF("GETFSIZE() = HOST(204)")
	HOSTDEF("GETLINES() = HOST(205)")
	HOSTDEF("GETMODE() = HOST(206)")
	HOSTDEF("GETPAGE() = HOST(207)")
	HOSTDEF("GETTYPE() = HOST(26)")
	HOSTDEF("GOTO(LINE,COL) = HOST(6,LINE,COL)")
	HOSTDEF("INSERTCHR() = HOST(13)")
        HOSTDEF("KEYSTATE() = HOST(36)")
        HOSTDEF("MOUSE(I) = HOST(214,I)")
	HOSTDEF("NORMATR() = HOST(24)")
	HOSTDEF("PEEK() = HOST(28)")
	HOSTDEF("PLAY(S) = HOST(201,S)")
	HOSTDEF("POKE(I) = HOST(29,I)")
        HOSTDEF("READATRSTRING(I) = HOST(31,I)")
	HOSTDEF("READCHR() = HOST(15)")
	HOSTDEF("READFIELD(I) = HOST(9,I)")
	HOSTDEF("READKEY() = HOST(8)")
        HOSTDEF("READKEYSPECIAL() = HOST(30)")
        HOSTDEF("READSTR(I) = HOST(14,I)")
	HOSTDEF("READY() = HOST(7)")
	HOSTDEF("REVATR() = HOST(25)")
        HOSTDEF("SCROLLALL(L1,C1,L2,C2,N,ATR) = "
+               "HOST(35,L1 * 256 + C1, L2 * 256 + C2,N,ATR)")
        HOSTDEF("SCROLLDN(TOP,BOT) = HOST(16,TOP,BOT)")
	HOSTDEF("SCROLLUP(TOP,BOT) = HOST(17,TOP,BOT)")
        HOSTDEF("SETADR(I1,I2) = HOST(27,I1,I2)")
	HOSTDEF("SETCHRATR(N,ATR) = HOST(18,N,ATR)")
	HOSTDEF("SETLINEATR(LINE,ATR) = HOST(19,LINE,ATR)")
	HOSTDEF("SETOFFSET(I) = HOST(208,I)")
	HOSTDEF("SETSIZE(I1,I2) = HOST(209,I1,I2)")
	HOSTDEF("SOUND(FREQ,MS) = HOST(200,FREQ,MS)")
	HOSTDEF("UPDATELINE(I) = HOST(20,I)")
	HOSTDEF("UPDATESCN() = HOST(21)")
	HOSTDEF("VIDEOINT(AX,BX,CX,DX) = HOST(210,AX,BX,CX,DX)")
        HOSTDEF("WRITEATRSTRING(S) = = HOST(32,S)")
        HOSTDEF("WRITECHR(S) = HOST(23,S)")
	HOSTDEF("WRITESTR(S) = HOST(22,S)")

* PC-SPITBOL (8086) specific functions
*
	HOST() ? FENCE "IBM PC"				:F(HOSTXIT)
	HOSTDEF("PARM() = HOST(1)")
	HOSTDEF("SHELLVAR(S) = HOST(2,S)")
	HOSTDEF("SOUND(I1,I2) = HOST(3,I1,I2)")
	HOSTDEF("PLAY(S) = HOST(4,S)")

HOSTXIT	UNLOAD("HOSTDEF")
	CODE("HOSTGO;HOSTDEF;HOSTDEF_END;HOSTXIT;")

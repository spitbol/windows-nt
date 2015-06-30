/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*	logic.c
 *
 * Perform logical or arithmetic operations on integers and strings
 *
 * Load from SPITBOL with statement:
 *
 *   LOAD('EXT_LOGIC_(INTEGER,ARG2,ARG3)', 'logic.slf')
 *
 * or if all your external functions are gathered into a single library:
 *
 *   LOAD('EXT_LOGIC_(INTEGER,ARG2,ARG3)', 'spitbol.slf')
 *
 *	Input:	First Argument = Operation Specifier:
 *			1 = Return Logical NOT of ARG2
 *			2 = Return ARG2 AND  Arg3
 *			3 = Return ARG2 OR   Arg3
 *			4 = Return ARG2 XOR  Arg3
 *			5 = Return ARG2 NAND Arg3
 *			6 = Return ARG2 NOR  Arg3
 *			7 = Return ARG2 + ARG3
 *			8 = Return ARG2 - ARG3
 *			9 = Return ARG2 * ARG3 (unsigned)
 *		       10 = Return ARG2 / ARG3 (unsigned)
 *		       11 = Return ARG2 shifted left ARG3 bits
 *		       12 = Return ARG2 shifted right ARG3 bits (unsigned)
 *		       13 = Return ARG2 shifted right ARG3 bits (sign extension)
 *		       14 = Return ARG2 rotated left ARG3 bits
 *		       15 = Return ARG2 rotated right ARG3 bits
 *		       16 = Return integer from digit string in ARG2, base ARG3
 *		       17 = Return digit string from integer in ARG2, base ARG3
 *			      (conversion base defaults to 16 if null string)
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
 * Use logic.inc to provide a mnemonic front-end to this function.
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M logic.c extrnlib.obj
 *
 * To build under AIX 3:
 *	 cc -o logic.slf -bM:SRE -bE:logic.exp -e _nostart logic.c
 *
 * To build under AIX 4:
 *	 cc -o logic.slf -bM:SRE -bE:logic.exp -bnoentry logic.c
 *
 * To build under SunOS with GNU:
 *  gcc -sun4 -nostdlib -o logic.slf logic.c extrnlib.o
 *
 */

enum {
	op_not = 1,
	op_and,
	op_or,
	op_xor,
	op_nand,
	op_nor,
	op_plus,
	op_sub,
	op_mult,
	op_div,
	op_shl,
	op_shr,
	op_sar,
	op_rol,
	op_ror,
	op_itod,
	op_dtoi
	};	

#include "system.h"
#include "extern32.h"

static char s_op(int op, char c2, char c3);

entry(EXT_LOGIC_)(union block *presult, misc *pinfo,
 union block *a3p, union block *a2p, int op)
{
	unsigned int	base;
	char	*sresult;
	mword	a2type, a3type, iresult;
	muword	a2, a3;
	unsigned char	cvbuf[32];
	
	if (op < op_not || op > op_dtoi)
		return FAIL;

	a2type = a2p->scb.sctyp;		/* get types of argument */
	a3type = a3p->scb.sctyp;

	/* check for special conversion cases */
	if (op == op_dtoi)
	{
		/* convert integer to digit string in a particular base */
		unsigned char	c, *p;

		if (a3type == ic)
		 	base = a3p->icb.icval;
		else if (a3type == sc && a3p->scb.sclen == 0)
			base = 0;				/* null string 3rd arg */
		else
			return FAIL;			/* arg 3 not int or null string */

		if (!base)
			base = 16;
		if (base < 2 || base > 36)
			return FAIL;

		if (a2type != ic)
			return FAIL;			/* arg2 not integer */

		a2 = a2p->icb.icval;
		p = &cvbuf[32];				/* produce digits backwards */
		while (a2)
		{
			a3 = a2 / base;
			c = (unsigned char )(a2 - (a3 * base));	/* faster than another division to get modulus */
			a2 = a3;
			if (c > 9)
				c += (unsigned char)('A' - '9' - 1);
			c += (unsigned char)'0';
			*--p = c;
		}
		if (p == &cvbuf[32])
			*--p = '0';			 
		
		presult->fsb.fsptr = (char *)p;
		presult->fsb.fslen = &cvbuf[32]  - p;
		return BL_FS;
		}
		  
	if (op == op_itod)
	{
		/* convert digit string in a particular base to integer */
		int 	length;
		mword	result;
		unsigned char	c, *p;

		if (a3type == ic)
		 	base = a3p->icb.icval;
		else if (a3type == sc && a3p->scb.sclen == 0)
			base = 0;				/* null string 3rd arg */
		else
			return FAIL;			/* arg 3 not int or null string */

		if (!base)
			base = 16;
		if (base < 2 || base > 36)
			return FAIL;

		if (a2type != sc)
			return FAIL;			/* arg2 not string */

		length = a2p->scb.sclen;
		p = (unsigned char *)a2p->scb.scstr;
		result = 0;
		while (length--)
		{
			c = *p++;
			if (c >= 'a' && c <= 'z')
				c -= (unsigned char)('a' - 'A');

			if (c < '0' || c > 'Z' || (c > '9' && c < 'A'))
				return FAIL;

			if (c >= 'A')
				c -= (unsigned char)('A' - '9' - 1);		/* map letters to range above digits */

			c -= (unsigned char)'0';
			if (c >= base)
				return FAIL;

			result = result * base + c;
		}
		presult->icb.icval = result;
		return BL_IC;
	}	

	if (a2type != ic)
	{
		union block *temp;
		if (a2type != sc)
			return FAIL;			/* fail if arg2 not integer or string */
		
		/* arg2 is string.  check arg3 */
		if (a3type == sc)
		{
			/* string - string */
			mword length;
			char *s2, *s3;
			if (op == op_not)
				a3p = a2p;			/* dup arg2 to arg3 */
			length = a3p->scb.sclen;
			if (length > buflen || length != a2p->scb.sclen)
				return FAIL;		/* too long or unequal string lengths */
			presult->scb.sclen = length;
			sresult = presult->scb.scstr;
			s2 = a2p->scb.scstr;
			s3 = a3p->scb.scstr;
			while (length--)
				*sresult++ = s_op(op, *s2++, *s3++);
			return BL_SC;
		}	
		if (a3type != ic)
			return FAIL;			/* fail if arg3 not integer or string */
		/* arg3 integer, arg2 string.  Swap. */
		a2type = ic;
		a3type = sc;
		temp = a2p;
		a2p = a3p;
		a3p = temp;
	}	
	
	/* arg2 is an integer.  test arg3 */
	a2 = a2p->icb.icval;
	if (a3type == ic)
 		a3 = a3p->icb.icval;
	else		   
	{
		if (a3type != sc)
			return FAIL;			/* fail if not I-S */
		if (op != op_not)
		{
			/* process arg2 integer, arg3 string */
			mword length;
			char c, *s;

			length = a3p->scb.sclen;
			if (length > buflen)
				return FAIL;		/* too long */
			presult->scb.sclen = length;
			sresult = presult->scb.scstr;
			s = a3p->scb.scstr;
			c = (unsigned char)a2;
			while (length--)
				*sresult++ = s_op(op, *s++, c);
			return BL_SC;
		}	
		/* op = not, arg2 = int, arg3 probably null string, ignore */
	}

	/* here if both arguments integer */
	switch (op)
	{
		case op_not:
			iresult = ~a2;
			break;
		case op_and:
			iresult = a2 & a3;
			break;
		case op_or:
			iresult = a2 | a3;
			break;
		case op_xor:
			iresult = a2 ^ a3;
			break;
		case op_nand:
			iresult = ~(a2 & a3);
			break;
		case op_nor:
			iresult = ~(a2 | a3);
			break;
		case op_plus:
			iresult = a2 + a3;
			break;
		case op_sub:
			iresult = a2 - a3;
			break;
		case op_mult:
			iresult = a2 * a3;
			break;
		case op_div:
			iresult = a2 / a3;
			break;
		case op_shl:
			iresult = a2 << a3;
			break;
		case op_shr:
			iresult = a2 >> a3;
			break;
		case op_sar:
			iresult = (mword)a2 >> a3;
			break;
		case op_rol:
			iresult = (a2 << a3) | (a2 >> (32-a3));
			break;
		case op_ror:
			iresult = (a2 >> a3) | (a2 << (32-a3));
			break;
	}
	presult->icb.icval = iresult;
	return BL_IC;	  
}


static char s_op(int op, char c2, char c3)
{
	/* here to process two character arguments */
	switch (op)
	{
		case op_not:
			return (char)~c2;
		case op_and:
			return c2 & c3;
		case op_or:
			return c2 | c3;
		case op_xor:
			return c2 ^ c3;
        case op_nand:
			return (char)~(c2 & c3);
		case op_nor:
			return (char)~(c2 | c3);
		case op_plus:
			return (char)(c2 + c3);
		case op_sub:
			return (char)(c2 - c3);
		case op_mult:
			return (char)((unsigned char)c2 * (unsigned char)c3);
		case op_div:
			return (char)((unsigned char)c2 / (unsigned char)c3);
		case op_shl:
			return (char)(c2 << c3);
		case op_shr:
			return (char)((unsigned char)c2 >> c3);
		case op_sar:
			return (char)(c2 >> c3);
		case op_rol:
			return (char)((c2 << c3) | ((unsigned char)c2 >> (32-c3)));
		case op_ror:
			return (char)(((unsigned char)c2 >> c3) | (c2 << (32-c3)));
    }
    return 0;
}

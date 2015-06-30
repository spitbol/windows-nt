/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/
/*
 * cvt.c
 *
 * EXT_CVTNS_ - convert integer or real number to a 4 or 8 character string.
 * EXT_CVTSN_ - convert a 4 or 8 character string to an integer or real number.
 *
 * Load either from SPITBOL with statements:
 *   LOAD('EXT_CVTNS_(NUMERIC)STRING', 'cvt.slf')
 *   LOAD('EXT_CVTSN_(STRING)', 'cvt.slf')
 *
 * or if all your external functions are gathered into a single library:
 *
 *   LOAD('EXT_CVTNS_(NUMERIC)STRING', 'spitbol.slf')
 *   LOAD('EXT_CVTSN_(STRING)', 'spitbol.slf')
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M cvt.c 
 *
 * To build under AIX 3:
 *	 cc -o cvt.slf -bM:SRE -bE:cvt.exp -e _nostart cvt.c
 *
 * To build under AIX 4:
 *	 cc -o cvt.slf -bM:SRE -bE:cvt.exp -bnoentry cvt.c
 *
 * To build under SunOS with GNU:
 *  gcc -sun4 -nostdlib -o cvt.slf cvt.c 
 *
 * To build under Solaris with GNU:
 *  gcc -sun4 -shared -o cvt.slf cvt.c
 *
 *
 * Function specifics:
 *
 * EXT_CVTNS_:
 *  Convert integer or real number to 4 or 8 character string respectively.
 *  Useful for writing numeric values in an internal form to a binary
 *  (raw mode) file.
 *
 *  This function is the inverse of EXT_CVTSN_(), which converts 4 or 8 character
 *  strings to internal numeric values.
 *
 *	Note:	We use the unrecognized keyword "NUMERIC" so that SPITBOL passes
 *  a pointer to the unconverted argument.  The function will then figure out
 *  if it was passed an integer or real number.  DO NOT SUBSTITUTE A SPECIFIC
 *  DATATYPE (SUCH AS "INTEGER" OR "REAL").
 *
 *	Input:	Integer or double precision real in internal form.
 *	Output:	Four or Eight byte string with internal representation
 *  		of number.
 *
 * 	Failure if input is any datatype except real or integer. Note that
 *  EXT_CVTNS_("1.23") will fail because the input argument is a string.  Use 
 *  EXT_CVTNS_(+"1.23") to convert a string to a true numeric before calling function.
 *
 *
 *
 * EXT_CVTSN_:
 *  Convert 4 or 8 character string to integer or real number respectively.
 *  Useful for reading numeric values in an internal form from a binary
 *  (raw mode) file.
 *
 *  This function is the inverse of EXT_CVTNS_(), which converts internal
 *  numeric values to 4 or 8 character strings.
 *
 *	Input:	Four or eight character string with internal representation
 *		of number.
 *	Output:	Integer or double precision real in internal form.
 *
 * 	Failure if input is not a four or eight character string.
 *
 *	Internal representation is big-endian.  That is, the integer 1 could be
 *  created using:
 *
 *    EXT_CVTSN_(CHAR(0) CHAR(0) CHAR(0) CHAR(1))
 *
 *  or equivalently,
 *
 *    EXT_CVTSN_(CVTNS(1))
 *
 * Usage:
 * -include "cvt.inc"
 *   Use names CvtNS() and CvtSN()
 *
 */
#include "system.h"
#include "extern32.h"


entry(EXT_CVTNS_)(union block *presult, misc *pinfo, union block *argp)
{
	if (argp->icb.ictyp == ic)
	{
		*(mword *)presult->scb.scstr = argp->icb.icval;
		presult->scb.sclen = sizeof(mword);
		return BL_SC;
	}
	else if (argp->rcb.rctyp == rc)
	{
#if sparc
		*(mword *)&presult->scb.scstr[0] = argp->rcb.rcvals[0];
		*(mword *)&presult->scb.scstr[4] = argp->rcb.rcvals[1];
#else
		*(double *)presult->scb.scstr = argp->rcb.rcval;
#endif
		presult->scb.sclen = sizeof(double);
		return BL_SC;
	}
	else
		return FAIL;
}


entry(EXT_CVTSN_)(union block *presult, misc *pinfo, char *s, int length)
{
	if (length == 4)
	{
		presult->icb.icval = *(mword *)s;
		return BL_IC;
	}
	else if (length == 8)
	{
#if sparc
	   presult->rcb.rcvals[0] = *(mword *)&s[0];
	   presult->rcb.rcvals[1] = *(mword *)&s[4];
#else
		presult->rcb.rcval = *(double *)s;
#endif
		return BL_RC;
	}
	else
		return FAIL;
}


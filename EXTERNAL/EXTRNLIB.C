/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*---------------------------  extrnlib.c  -------------------------------*/

/*
 * Handy support routines for 32-bit SPITBOL external functions.
 *
 * V1.00	02/17/90 04:31pm
 * 			Initial version for 80386 SPITBOL
 *
 * V1.01	10-19-91 06:51am
 * 			Modify for OS/2, where far mem* and str* function are not
 * 			needed (they are provided by normal C library functions).
 *
 * V1.02	07-04-92 08:32am
 * 			Modify for SPARC SPITBOL.  No far or near distinctions needed.
 *
 * V1.03	05-17-95 06:54am
 *			Modify for RS/6000 SPITBOL.
 *
 * V1.04    12-30-96 06:52am
 *          Modify for WinNT SPITBOL.
 * 
 * V1.05    1-20-97  11:16am
 *          Added retnstrt and retstrt to return strings that are allocated
 *          on the caller's stack and will not survive the return of the
 *          external function back to SPITBOL.  String length must be
 *          less than buflen (currently 512 bytes).
 *
 * V1.06    033-04-97 2:15pm
 *          Tweak for SPARC SPITBOL.
 * 
 * For WinNT using Borland C++, compile with:
 *   bcc32 -c -WXD -I\bc4\include -a1 -d -w-par -c extrnlib.c
 * 
 * For SPARC, compile with
 *   cc -Bdynamic -misalign -c extrnlib.o extrnlib.c
 *
 * Then use extrnlib.o or extrnlib.obj to link with your external
 * function.
 * 
 * (c) Copyright 1997 Catspaw, Inc.  All Rights Reserved.
 *
 * Contents:
 *
 * mword 	  retint(val, presult)		return integer result
 * mword 	  retnstrf(s, n, presult)	return n-char string
 * mword      retnstrt(s, n, presult)   return n-char temp string
 * mword      retnxdtf(s, n, presult)   return n-chars of external data
 * mword 	  retreal(val, presult)		return real result
 * mword 	  retstrf(s, presult)		return C string
 * mword      retstrt(s, presult)       return temp C string
 */

#include "system.h"
#include "extern32.h"


/*
 * retint(val, presult) - Return integer
 */

mword retint(val, presult)
int 		val;	 	   			/* integer to return		*/
union block *presult;				/* result pointer			*/
{
	presult->icb.icval = val;
	return BL_IC;
}



/*
 * retnstrf(s, n, presult) - Return n-char string.
 * 
 * String must be permanently allocated -- if it is in dynamic memory,
 * use retnstrt instead.
 */

mword retnstrf(s, n, presult)
char 		*s; 	   				/* string to return			*/
size_t		n;						/* number of characters in s*/
union block *presult;				/* result pointer 			*/
{
	presult->fsb.fsptr = s;			/* return string pointer	*/
	presult->fsb.fslen = n;			/* set string length		*/
	return BL_FS;
}


/*
 * retnstrt(s, n, presult) - Return n-char temporary string
 * 
 * Use when the string to be returned is a dynamic variable on the external
 * functions stack, and will not survive the return of the function.
 */

mword retnstrt(s, n, presult)
char 		*s; 	   				/* string to return			*/
size_t		n;						/* number of characters in s*/
union block *presult;				/* result pointer 			*/
{
    if (n > buflen)
        return FAIL;

    presult->scb.sclen = n;
    memcpy(presult->scb.scstr, s, n);
    return BL_SC;
}


/*
 * retnxdtf(s, n, presult) - Return n-char external data.
 * 
 * External data must be permanently allocated -- it may not be in dynamic
 * memory, such as the stack.
 */


mword retnxdtf(s, n, presult)
void 		*s; 	   				/* data to return			*/
size_t		n;						/* number of bytes at s		*/
union block *presult;				/* result pointer 			*/
{
	presult->fxb.fxptr = s;			/* return data pointer		*/
	presult->fxb.fxlen = n;			/* set data length			*/
	return BL_FX;
}


/*
 * retreal(val, presult) - Return real number
 */
mword retreal(val, presult)
double 			val; 	   			/* real number to return 	*/
union block 	*presult;			/* result pointer			*/
{
#if sparc
    /* Needed to prevent double-word misaligned store. */
    union rv {
        double r;
        mword  rw[2];
    };
    presult->rcb.rcvals[0] = ((union rv *)&val)->rw[0];
    presult->rcb.rcvals[1] = ((union rv *)&val)->rw[1];
#else
    presult->rcb.rcval = val;
#endif
    return BL_RC;
}


/*
 * retstrf(s, presult) - Return C string.
 *
 * String must be permanently allocated -- if it is in dynamic memory,
 * use retstrt instead.
 */

mword retstrf(s, presult)
char 		*s;		 	   			/* string to return			*/
union block *presult;				/* result pointer			*/
{
	presult->fsb.fsptr = s;			/* return string pointer	*/
	presult->fsb.fslen = strlen(s);	/* set string length		*/
	return BL_FS;
}

/*
 * retstrt(s, presult) - Return temporary C string
 * 
 * Use when the string to be returned is a dynamic variable on the external
 * functions stack, and will not survive the return of the function.
 */

mword retstrt(s, presult)
char 		*s;		 	   			/* string to return			*/
union block *presult;				/* result pointer			*/
{
    presult->scb.sclen = strlen(s);
    if (presult->scb.sclen > buflen)
        return FAIL;

    strcpy(presult->scb.scstr, s);  /* copy string over to result area */
    return BL_SC;
}

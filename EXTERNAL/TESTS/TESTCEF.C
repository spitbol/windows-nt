/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * testcef.c
 *
 * Contains several external functions used for testing SPITBOL's
 * external function interface.
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M testcef.c extrnlib.obj
 *
 * To build under SunOS with GNU:
 *  gcc -sun4 -nostdlib -o testcef.slf testcef.c extrnlib.o
 *
 */

#include "system.h"
#include "extern32.h"
#include <stdio.h>
#include <stdlib.h>

/* TESTCEF1
 *
 * Test access to arguments and return of integer:
 * 	LOAD("TESTCEF1(INTEGER,REAL,STRING)","testcef.slf")
 *
 * 	Call with TESTCEF1(100, 1000.0, "ABCD")
 *
 * 	Returns the string "OK" if arguments were found as expected,
 * 	otherwise a message about which argument was bad.
 */

#define testarg1 100
#define testarg2 1000.0
#define testarg3 "ABCD"
#define xbufsiz  6000
static char buffer[xbufsiz];


entry(TESTCEF1)(presult, pinfo, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer			*/
double	   		rarg2;			 /* arg2 real number		*/
mword	   		larg3;			 /* arg3 length				*/
far	char   		*parg3;			 /* pointer to arg3 string	*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area	*/
{

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	return retstrn("OK", presult);		/* Arguments are OK, return "OK" */
}




/*
 * TESTCEF2
 *
 * Test access to arguments and return of integer:
 * 	LOAD("TESTCEF2(INTEGER,REAL,STRING)","testcef.slf")
 *
 * 	Call with TESTCEF2(100, 1000.0, "ABCD")
 *
 * 	Returns arg 1 plus 1 if arguments were found as expected,
 * 	otherwise a message about which argument was bad.
 */

entry(TESTCEF2)(presult, pinfo, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer			*/
double	   		rarg2;			 /* arg2 real number		*/
mword	   		larg3;			 /* arg3 length				*/
far	char   		*parg3;			 /* pointer to arg3 string	*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area	*/
{

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	return retint(iarg1 + 1, presult);	/* Arguments are OK, return arg1+1	*/
}



/*
 * TESTCEF3
 *
 * Test access to arguments and return of real number:
 * 	LOAD("TESTCEF3(INTEGER,REAL,STRING)","testcef.slf")
 *
 * 	Call with TESTCEF3(100, 1000.0, "ABCD")
 *
 * 	Returns arg 2 minus 1.0 if arguments were found as expected,
 * 	otherwise a message about which argument was bad.
 */

entry(TESTCEF3)(presult, pinfo, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer			*/
double	   		rarg2;			 /* arg2 real number		*/
mword	   		larg3;			 /* arg3 length				*/
far	char   		*parg3;			 /* pointer to arg3 string	*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area	*/
{

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	return retreal(rarg2 - 1.0, presult);	/* Arguments are OK, return arg2-1	*/
}




/*
 * TESTCEF4
 *
 * Test access to arguments and return of a far string.
 * 	LOAD("TESTCEF4(INTEGER,REAL,STRING)","testcef.slf")
 *
 * 	Call with TESTCEF4(100, 1000.0, "ABCD")
 *
 * 	Returns far string result if arguments were found as expected,
 * 	otherwise a message about which argument was bad.
 */

entry(TESTCEF4)(presult, pinfo, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer			*/
double	   		rarg2;			 /* arg2 real number		*/
mword	   		larg3;			 /* arg3 length				*/
far	char   		*parg3;			 /* pointer to arg3 string	*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area	*/
{

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	return retstrf((far char *)"This is a far string result.",
					presult);
}




/*
 * TESTCEF5
 *
 * Test access to arguments and return of a external data block.
 * 	LOAD("TESTCEF5(INTEGER,REAL,STRING,OTHER)","testcef.slf")
 *
 * 	Call with TESTCEF5(100, 1000.0, "ABCD")
 *
 * 	Returns external data block if first three arguments were found
 *  as expected on the first call, "OK" on second call.
 * 	Otherwise a message about which argument was bad.
 */

entry(TESTCEF5)(presult, pinfo, parg4, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer			*/
double	   		rarg2;			 /* arg2 real number		*/
mword	   		larg3;			 /* arg3 length				*/
far	char   		*parg3;			 /* pointer to arg3 string	*/
far union block *parg4;			 /* pointer to unconverted arg4 block	*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area				*/
{
	far char *p;
	int		 i;

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	/*
	 *  Test if arg4 is the null string (first call).  Note that arg4
	 *  is unconverted, so we work with raw SPITBOL data blocks.
	 */

	if (parg4->scb.sctyp == sc) {
		if (parg4->scb.sclen != 0)
			return retstrn("Fourth argument was not null string", presult);
		/*
		 * This is the first call, and with null string fourth argument.
		 * Create buflen-byte external block.
		 */
		p = (far char *)presult->xnb.xnu.xndta;
		for (i = 0; i < buflen; i++)
			*p++ = (char) i;
		presult->xnb.xnlen = buflen;
		return BL_XN;
		}
	else if (parg4->xnb.xntyp == xn) {
		/*
		 * Fourth argument is external data type.
		 */
		if (parg4->xnb.xnlen != buflen + 2*sizeof(mword))
			return retstrn("Fourth argument (external data) size incorrect", presult);
		p = (far char *)parg4->xnb.xnu.xndta;
		for (i = 0; i < buflen; i++)
			if (*p++ != (char)i)
				return retstrn("Fourth argument (external data) contents bad", presult);
		return retstrn("OK", presult);
		}
   return retstrn("Fourth argument not external data", presult);
}




/*
 * TESTCEF6
 *
 * Test access to arguments and return of far external data block.
 * 	LOAD("TESTCEF6(INTEGER,REAL,STRING,OTHER)","testcef.slf")
 *
 * 	Call with TESTCEF6(100, 1000.0, "ABCD")
 *
 * 	Returns far external data block if first three arguments were found
 *  as expected on the first call, "OK" on second call.
 * 	Otherwise a message about which argument was bad.
 */

entry(TESTCEF6)(presult, pinfo, parg4, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer						*/
double	   		rarg2;			 /* arg2 real number					*/
mword	   		larg3;			 /* arg3 length							*/
far	char   		*parg3;			 /* pointer to arg3 string				*/
far union block *parg4;			 /* pointer to unconverted arg4 block	*/
far misc     	*pinfo;		     /* pointer to misc info				*/
far union block	*presult;		 /* pointer to result area				*/
{
	far char *p;
	int		 i;

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	/*
	 *  Test if arg4 is the null string (first call).  Note that arg4
	 *  is unconverted, so we work with raw SPITBOL data blocks.
	 */

	if (parg4->scb.sctyp == sc) {
		if (parg4->scb.sclen != 0)
			return retstrn("Fourth argument was not null string", presult);
		/*
		 * This is the first call, and with null string fourth argument.
		 * Create xbufsiz-byte far external block.
		 */
		p = (far char *)buffer;
		for (i = 0; i < xbufsiz; i++)
			*p++ = (char) i;
		presult->fxb.fxptr = (far void *)buffer;	/* return pointer to data */
		presult->fxb.fxlen = xbufsiz;
		return BL_FX;
		}
	else if (parg4->xnb.xntyp == xn) {
		/*
		 * Fourth argument is external data type.
		 */
		if (parg4->xnb.xnlen != xbufsiz + 2*sizeof(mword))
			return retstrn("Fourth argument (external data) size incorrect", presult);
		p = (far char *)parg4->xnb.xnu.xndta;
		for (i = 0; i < xbufsiz; i++)
			if (*p++ != (char)i)
				return retstrn("Fourth argument (external data) contents bad", presult);
		return retstrn("OK", presult);
		}
   return retstrn("Fourth argument not external data", presult);
}



/*
 * TESTCEF7
 *
 * Test access to arguments and return of far external data block.
 * This also tests heap expansion by using malloc().
 *
 * 	LOAD("TESTCEF7(INTEGER,REAL,STRING,OTHER)","testcef.slf")
 *
 * 	Call with TESTCEF7(100, 1000.0, "ABCD")
 *
 * 	Returns far external data block if first three arguments were found
 *  as expected on the first call, "OK" on second call.
 * 	Otherwise a message about which argument was bad.
 */

entry(TESTCEF7)(presult, pinfo, parg4, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer						*/
double	   		rarg2;			 /* arg2 real number					*/
mword	   		larg3;			 /* arg3 length							*/
far	char   		*parg3;			 /* pointer to arg3 string				*/
far union block *parg4;			 /* pointer to unconverted arg4 block	*/
far misc     	*pinfo;		     /* pointer to misc info				*/
far union block	*presult;		 /* pointer to result area				*/
{
	far char *p;
	char	 *q;
	int		 i;

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

	/*
	 *  Test if arg4 is the null string (first call).  Note that arg4
	 *  is unconverted, so we work with raw SPITBOL data blocks.
	 */

	if (parg4->scb.sctyp == sc) {
		if (parg4->scb.sclen != 0)
			return retstrn("Fourth argument was not null string", presult);
		/*
		 * This is the first call, and with null string fourth argument.
		 * Create xbufsiz-byte far external block.
		 */
		p = (far char *)(q = malloc(xbufsiz));
		if (!p)
			return retstrn("malloc() call failed", presult);
		for (i = 0; i < xbufsiz; i++)
			*q++ = (char) i;
		presult->fxb.fxptr = (far void *)p;	/* return pointer to data */
		presult->fxb.fxlen = xbufsiz;
		return BL_FX;
		}
	else if (parg4->xnb.xntyp == xn) {
		/*
		 * Fourth argument is external data type.
		 */
		if (parg4->xnb.xnlen != xbufsiz + 2*sizeof(mword))
			return retstrn("Fourth argument (external data) size incorrect", presult);
		p = (far char *)parg4->xnb.xnu.xndta;
		for (i = 0; i < xbufsiz; i++)
			if (*p++ != (char)i)
				return retstrn("Fourth argument (external data) contents bad", presult);
		return retstrn("OK", presult);
		}
   return retstrn("Fourth argument not external data", presult);
}




/*
 * TESTCEF8
 *
 * Test usage of malloc() and free(), and corresponding heap expansion.
 *
 * 	LOAD("TESTCEF8(INTEGER)","testcef.slf")
 *
 * 	Call with TESTCEF8(n), where n is negative to allocate -n bytes,
 *  positive to free block n.  Returns block allocated.
 *
 */

entry(TESTCEF8)(presult, pinfo, iarg)
mword	   		iarg;			 /* arg integer							*/
far misc     	*pinfo;		     /* pointer to misc info				*/
far union block	*presult;		 /* pointer to result area				*/
{
	int		 i;

	if (iarg < 0) {
		i = (int)malloc((size_t)(-iarg));
		return retint(i != 0 ? i : FAIL, presult);
		}
	else {
		free((void *)iarg);
        return retstrn("free done", presult);
		}
}





/*
 * TESTCEF9
 *
 * Test access to arguments and ability to perform printf().
 * 	LOAD("TESTCEF1(INTEGER,REAL,STRING)","testcef.slf")
 *
 * 	Call with TESTCEF1(100, 1000.0, "ABCD")
 *
 *  Prints expected argument values using printf().  (printf
 *  can't handle far pointer to string).
 *
 * 	Returns the string "OK" if arguments were found as expected,
 * 	otherwise a message about which argument was bad.
 */
entry(TESTCEF9)(presult, pinfo, parg3, larg3, rarg2, iarg1)
mword	   		iarg1;			 /* arg1 integer			*/
double	   		rarg2;			 /* arg2 real number		*/
mword	   		larg3;			 /* arg3 length				*/
far	char   		*parg3;			 /* pointer to arg3 string	*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area	*/
{

	if (iarg1 != testarg1)
		return retstrn("First argument not integer 100", presult);

	if (rarg2 != testarg2)
		return retstrn("Second argument not real 1000.0", presult);

	if (larg3 != strlen(testarg3))
		return retstrn("Third argument string length wrong", presult);

	if (strncmpfn(parg3, testarg3, larg3))
		return retstrn("Third argument not string 'ABCD'", presult);

    printf("via printf: %d %lf %s\n", testarg1, testarg2, testarg3);
	return retstrn("OK", presult);		/* Arguments are OK, return "OK" */
}




/*
 * TESTCEFA
 *
 * Test ability to open file and perform fprintf().
 *
 * 	LOAD("TESTCEFA(INTEGER)","testcef.slf")
 *
 * 	Call with TESTCEFA(n)
 *
 *  Writes integer n to file testcefa.dat.
 *
 * 	Returns the string "OK".
 */

#define END_FLAG 1000

entry(TESTCEFA)(presult, pinfo, iarg)
mword	   		iarg;			 /* integer	argument		*/
far misc     	*pinfo;		     /* pointer to misc info	*/
far union block	*presult;		 /* pointer to result area	*/
{
   static FILE *fp;
   if ( first_call || reload_call ) {

		/* first call after initial load or after save file reload */
		if ((fp = fopen("testcefa.dat","w")) == (FILE *)NULL)
			return retstrn("Couldn't open file testcefa.dat", presult);

		fprintf(fp, "%s\n", first_call ?
			"Opened on first call after initial load." :
			"Opened on first call after reload of save file.");
		}

	fprintf(fp, "%d\n", iarg);

	if (iarg == END_FLAG)
		fclose(fp);

	return retstrn("OK", presult);		/* Arguments are OK, return "OK" */
}

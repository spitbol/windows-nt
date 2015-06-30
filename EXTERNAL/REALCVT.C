/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * realcvt.c
 *
 * Converts a real number to a string with some control over the number
 * of digits produced and their format.  The control parameters are modeled
 * on those provided in SNOBOL4+.  For many users, it will be simpler
 * to merely use sprintf to control formatting.
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M realcvt.c extrnlib.obj
 *
 * To build under AIX 3:
 *	 cc -o realcvt.slf -bM:SRE -bE:realcvt.exp -e _nostart realcvt.c
 *
 * To build under AIX 4:
 *	 cc -o realcvt.slf -bM:SRE -bE:realcvt.exp -bnoentry realcvt.c
 *
 * To build under SunOS with GNU:
 *  gcc -sun4 -nostdlib -o realcvt.slf realcvt.c extrnlib.o
 *
 * Load in spitbol with:
 *  LOAD("EXT_REALCVT_(REAL,INTEGER,INTEGER)", "realcvt.slf")
 *
 * or if all your external functions are gathered into a single library:
 *  LOAD("EXT_REALCVT_(REAL,INTEGER,INTEGER)", "spitbol.slf")
 *
 * Call as follows:
 * -include "realcvt.inc"
 * result = RealCvt(r, p, f)
 *
 * where
 *  r		the real number to convert to a string
 *	p		number of significant digits to produce. 1 <= p <= 16
 *	f		format: if f is negative, produce E format notation
 *					if f is greater than zero, produce F format notation
 *					if f is zero, use G format (E or F depending upon r's magnitude)
 */

#include "system.h"
#include "extern32.h"

static int dscale(int round, far double *pdw);

entry(EXT_REALCVT_)(presult, pinfo, flotcl, preccl, real)
double			real;
int				preccl;			/* precision */
int				flotcl;			/* <0 - E, =0 - G, >0 - F */
far misc     	*pinfo;		    /* pointer to misc info	*/
far union block	*presult;		/* pointer to result area */
{
  char	*cp;
  int   j, fplace, iplace, mode, temp, exp;
  char	realbuf[316];

  cp = realbuf;

  mode = flotcl;
  exp = dscale(0, &real);
  if (mode == 0  && exp < 5  &&  exp > -5)
    mode++;
  temp = (mode > 0 ? exp : 0);

  j = dscale(preccl, &real);
  exp += j;
  temp += (mode > 0 ? j : 0);
  iplace = temp + 1;
  fplace = (preccl < 1 || preccl > 16 ? 16 : preccl);

  if (real < 0.0)
  {
    real = -real;
    *cp++ = '-';
  }
  if (temp < 0)
  {
    *cp++ = '0';
    *cp++ = '.';
    while (iplace++ < 0)
	  *cp++ = '0';
  }
  else
  {
    do
	{
      if (fplace-- > 0)
	  {
        j = (int)real;
		  *cp++ = (char)(j + '0');
        real = (real - j) * 10.0;
      }
      else
	    *cp++ = '0';
    } while (--iplace);
    *cp++ = '.';
  }
  while (fplace-- > 0)
  {
    j = (int)real;			/* get the integer part */
	 *cp++ = (char)(j + '0');
    real = (real - j) * 10.0;
  }

  /* remove trailing zeros from mantissa if G format */
  if (flotcl == 0)
  {
  	while (cp[-1] == '0')
		cp--;
  }

  if (mode <= 0)
  {
    *cp++ = 'E';
    if (exp < 0)
	{
      *cp++ = '-';
      exp = -exp;
    }
    else
	  *cp++ = '+';
    temp = exp / 100;
	 exp = exp - temp * 100;
	 *cp++ = (char)(temp + '0');
    temp = exp / 10;
    exp = exp - temp * 10;
	 *cp++ = (char)(temp + '0');
	 *cp++ = (char)(exp + '0');
  }
  presult->scb.sclen = cp - realbuf;	/* string length */
  memcpy(presult->scb.scstr, realbuf, presult->scb.sclen);
  return BL_SC;
}

static double pgten[]={1e1,1e2,1e4,1e8,1e16,1e32,1e64,1e128,1e256};
static double plten[]={1e-1,1e-2,1e-4,1e-8,1e-16,1e-32,1e-64,1e-128,1e-256};
static double rounds[]={5e-1,5e-2,5e-3,5e-4,5e-5,5e-6,5e-7,5e-8,
			 5e-9,5e-10,5e-11,5e-12,5e-13,5e-14,5e-15};

/*	DSCALE		Scale *pdw into range 1.00000 to 9.999999.
 *				If round > 0, add 5.0E-round to *pdw before scaling.
 *				Power of 10 needed to restore result returned in as result.
 */
static int dscale(int round, far double *pdw)
{
  int pow=0, sign=1, j;

  if (*pdw < 0.0)
    *pdw = -*pdw;
  else
    sign = 0;
  if (*pdw == 0.0)
    return 0;
  if (round < 0)
    return 0;
  if (round > 0 && round <= 15)
    *pdw += rounds[round-1];
  if (*pdw - 10. >= 0.)
  {
    for (j=9; j--;)
	{
      pow <<= 1;
      if (*pdw - pgten[j] >= 0.)
	  {
	    *pdw *= plten[j];
	    pow += 1;
      }
    }
  }
  else if (*pdw - 1. < 0.)
  {
    for (j=9; j--;)
	{
      pow <<= 1;
      if (*pdw - plten[j] < 0.)
	  {
	    *pdw *= pgten[j];
	    pow += 1;
      }
    }
    pow = -pow;
    *pdw *= 10.0;
    --pow;
  }
  if (sign)
    *pdw = -*pdw;
  return pow;
}

/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/
/*----------------------------  extern32.h  ------------------------------*/
#ifndef __extern32__
#define __extern32__

/*
 *  Definitions of routines and data available to C-language
 *  external function to be called from 32-bit versions of SPITBOL.
 *
 * 	V1.00  02/17/90 01:52pm
 * 		   Initial version
 *
 *  V1.01  10-18-91 04:53pm
 *         Modified for OS/2 2.0 SPITBOL-386.
 * 		   Switch to Pascal calling conventions.
 *
 *  V1.02  03-29-92 09:11am
 * 		   IBM's OS/2 C Set/2 compiler does not allow pascal calling
 * 		   conventions for 32-bit functions.  Therefore, users will
 * 		   have to specify the external function's arguments in reverse order.
 *
 *  V1.03  07-28-92 06:56am
 * 		   Customize for SPARC.
 *
 *	V1.04  09-12-94 07:13pm
 *		   Add definitions for buffers
 *
 *  V1.05  04-25-95 10:05pm
 *		   Customize for RS/6000
 *
 *  V1.06  12-29-96 06:05pm
 *         Customize for Windows NT
 *
 *  V1.07  03-04-97 12:45pm
 *			  Tweak for SPARC.
 *
 *  Definition of information placed on stack prior to pushing arguments to
 *  an external function.
 *
 *  Many of these items can be ignored, and are provided only for the
 *  benefit of those wishing to operate directly on SPITBOL's internal
 *  data structures.
 *
 *  However, the pointer in presult *must* be used by the external
 *  function to locate the area in which results are returned.
 *
 * (c) Copyright 1990-97 Catspaw, Inc.   All Rights Reserved.
 */

#include "system.h"
typedef int mword;				/* MINIMAL word	*/
typedef unsigned int muword;	/* MINIMAL unsigned word	*/
#ifndef far
#define far
#endif

#ifndef near
#define near
#endif

#ifndef Params
#if PROTOTYPES
#define Params(a) a
#else
#define Params(a) ()
#endif
#endif

#define _pascal           /* Available under Borland, but don't use it now. */
#if WINNT && __BORLANDC__
#define entry(x) mword _pascal __export x
#elif WINNT && _MSC_VER
#define entry(x) mword _pascal __declspec(dllexport) x
#else
#define entry(x) mword _pascal x
#endif

#include "blocks32.h"
#include <string.h>


/* array of pointers to double functions */
typedef double (*APDF[])();


/*
 * Miscellaneous information provided by SPITBOL in pointer above the arguments.
 */

typedef struct misc {
	short			 vers;			/* version number of interface			*/
	unsigned char	 ext;			/* host environment, see ext_type below	*/
	unsigned char	 spare;			/* reserved 							*/
	mword			 nargs;		    /* number of args to function			*/
 	mword		   (*ptyptab)[];	/* pointer to table of data types		*/
 	struct xnblk 	*pxnblk;	    /* ptr to xnblk describing function		*/
 	struct efblk 	*pefblk;		/* ptr to efblk describing function		*/
	APDF            *pflttab;		/* ptr to array of floating point fncs	*/
#if WINNT
    short            spds;          /* SPITBOL's DS segment selector        */
	short			 spcs;			/* SPITBOL's CS segment selector		*/
#endif
    } misc;

enum ext_type {						/* Executing under:						*/
	t_pharlap,						/*  PharLap DOS Extender				*/
	t_intel,						/*  Intel DOS Extender					*/
	t_os2,							/*  OS/2 2.0							*/
	t_tc16,						 	/*  MS-DOS TurboC with 16-bit IA		*/
	t_tc32,							/*  MS-DOS TurboC with 32-bit IA		*/
	t_w1616,						/*  16-bit Windows, 16-bit SPITBOL		*/
	t_w1632,						/*  16-bit Windows, 32-bit SPITBOL		*/
	t_wnt8,							/*  Windows NT on 386/486				*/
	t_sparc,						/*  Sun 4 / SPARC						*/
	t_mac,							/*  Apple Macintosh						*/
	t_mips,							/*  MIPS R3000							*/
	t_rs6000						/*  IBM RS/6000							*/
	};

/*
 * Sample usage.  Definition for function arguments, assuming
 * calling function in SPITBOL with:
 *
 * 	 F(INTEGER,REAL,STRING)
 *
 * Because SPITBOL pushes arguments left to right, a Pascal
 * calling sequence should be used.  The could be supplied by
 * adding the __pascal keyword to the entry macro.  
 *
 * However, because the SPARC, RS/6000 and OS/2 C compilers do not support
 * Pascal calling sequences, and we would like to move external function
 * source files easily between systems, the function definition will have
 * to manually reverse the arguments:
 *   entry(F)(presult, pinfo, parg3, larg3, rarg2, iarg1)
 *     union block	   *presult;		 pointer to result area
 *     misc		   	   *pinfo;		     miscellaneous info
 *     char	   		   *parg3;			 pointer to arg3 string
 *     mword	 		larg3;			 arg3 length
 *     double		   	rarg2;			 arg2 real number
 *     mword	   		iarg1;			 arg1 integer
 * {
 *    ....  start of function body
 */


/*
 * Simple names for datatypes.  Performs a lookup in SPITBOL's type
 * table to fetch a 32-bit type word for specific data types.
 */

#define ar	(*((*pinfo).ptyptab))[BL_AR]	/* Array		*/
#define bc	(*((*pinfo).ptyptab))[BL_BC]	/* Buffer Control	*/
#define bf	(*((*pinfo).ptyptab))[BL_BF]	/* String Buffer	*/
#define cd	(*((*pinfo).ptyptab))[BL_CD]	/* Code			*/
#define ex	(*((*pinfo).ptyptab))[BL_EX]	/* Expression	*/
#define ic	(*((*pinfo).ptyptab))[BL_IC]	/* Integer		*/
#define nm	(*((*pinfo).ptyptab))[BL_NM]	/* Name			*/
#define rc	(*((*pinfo).ptyptab))[BL_RC]	/* Real			*/
#define sc	(*((*pinfo).ptyptab))[BL_SC]	/* String		*/
#define tb	(*((*pinfo).ptyptab))[BL_TB]	/* Table		*/
#define vc	(*((*pinfo).ptyptab))[BL_VC]	/* Vector		*/
#define xn	(*((*pinfo).ptyptab))[BL_XN]	/* External		*/


/*
 * Non-standard block-type values that may be returned as a result:
 */

#define FAIL	(-1)			/* Signal function failure	*/
#define	BL_NC	100				/* Unconverted result		*/
#define BL_FS	101				/* Far string				*/
#define	BL_FX	102				/* Far external block		*/

/*
 * Length of string area in result buffer
 */

#define	buflen	512


/*
 * SPITBOL's Real Number Functions are not available to external
 * functions coded in C.  Use the normal C floating point library
 * to provide such support.
 */

/*
 * Function definitions for routines in extrnlib.c
 */
#if sparc | aix
#include <memory.h>
#endif

/* All of this near/far stuff is remnant from an earlier implementation
 * using the PharLap DOS Extender that loaded external functions into different
 * memory segments.
 */
#define	  memcmpfn(s1,s2,n) memcmp(s1,s2,n)
#define	  memcpyff(s1,s2,n) memcpy(s1,s2,n)
#define	  memcpyfn(s1,s2,n) memcpy(s1,s2,n)
#define	  strcpyff(s1,s2) strcpy(s1,s2)
#define	  strcpyfn(s1,s2) strcpy(s1,s2)
#define	  strcpynf(s1,s2) strcpy(s1,s2)
#define	  strlenf(s) strlen(s)
#define	  strncmpfn(s1,s2,n) strncmp(s1,s2,n)
#define	  strncpyff(s1,s2,n) strncpy(s1,s2,n)
#define	  strncpyfn(s1,s2,n) strncpy(s1,s2,n)
#define	  strncpynf(s1,s2,n) strncpy(s1,s2,n)

mword 	  retint Params((int val, union block *presult));
mword 	  retnstrf Params((char *s, size_t n, union block *presult));
#define	  retnstrn(s,n,presult) retnstrf(s,n,presult)
mword     retnstrt Params((char *s, size_t n, union block *presult));
mword     retnxdtf Params((void *s, size_t n, union block *presult));
#define	  retnxdtn(s,n,presult) retnxdtf(s,n,presult)
mword 	  retreal Params((double val, union block *presult));
mword 	  retstrf Params((char *s, union block *presult));
#define   retstrn(s,presult) retstrf(s,presult)
mword     retstrt Params((char *s, union block *presult));

#endif          /* __extern32__ */
/*-------------------------- end of extern32.h ------------------------*/

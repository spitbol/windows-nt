/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * strftime.c
 *
 *
 * Format the current time according to a C format string:
 *  LOAD('EXT_STRFTIME_(STRING)STRING','strftime.slf')
 *
 * or if all your external functions are gathered into a single library:
 *  LOAD('EXT_STRFTIME_(STRING)STRING','spitbol.slf')
 *
 * Usage:
 *  -include "strftime.inc"
 *  Result = StrfTime(S)
 *
 *  Input:  A format string consisting of zero of more directives and
 *      ordinary charcters.  A directive consists of the % character
 *      followed by a character that determines the substitution
 *      that is to take place.  All ordinary characters are copied
 *      unchanged.
 * 
 * The following directives are supported:
 * 
 * %%   Character %
 * %a   Abbreviated weekday name
 * %A   Full weekday name
 * %b   Abbreviated month name
 * %B   Full month name
 * %c   Date and time
 * %d   Two-digit day of the month (01 to 31)
 * %H   Two-digit hour (00 to 23)
 * %I   Two-digit hour (01 to 12)
 * %j   Three-digit day of the year (001 to 366)
 * %m   Two-digit month as a decimal number (1-12)
 * %M   Two-digit minute (00 to 59)
 * %p   AM or PM
 * %S   Two-digit second (00 to 59)
 * %U   Two-digit week number where Sunday is the first day of the week (00 to 53)
 * %w   Weekday where 0 is Sunday (0 to 6)
 * %W   Two-digit week number where Monday is the first dayof the week (00 to 53)
 * %x   Date
 * %X   Time
 * %y   Two-digit year without century (00 to 99)
 * %Y   Year with century
 * %Z   Time zone name, or no characters if no time zone
 * 
 *
 *  Output: Resultant formated string.
 *
 * To build under AIX 3:
 *   cc -o strftime.slf -bM:SRE -bE:strftime.exp -e _nostart strftime.c
 *
 * To build under AIX 4:
 *   cc -o strftime.slf -bM:SRE -bE:strftime.exp -bnoentry strftime.c
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M strftime.c extrnlib.obj
 *
 * To build under SunOS with GNU:
 *  gcc -sun4 -nostdlib -o strftime.slf strftime.c extrnlib.o
 *
 */
#include <time.h>
#include "system.h"
#include "extern32.h"

#define MAX_RESULT 512

entry(EXT_STRFTIME_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   struct tm    *t;
   time_t       timer;
   char         result[MAX_RESULT];
	size_t		 n;

   if (larg >= buflen)
      return FAIL;
   
   strncpy(presult->scb.scstr, parg, larg);
	presult->scb.scstr[larg] = '\0'; 	 /* create C string */
   time(&timer);
   t = localtime(&timer);
   n = strftime(result, MAX_RESULT-1, presult->scb.scstr, t);
   return retnstrt(result, n, presult);
}

*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* STRFTIME.INC
*
* Format the current time according to a C format string.
*
* Usage:
*  Result = StrfTime(S)
*
* Input:  A format string consisting of zero of more directives and
*         ordinary charcters.  A directive consists of the % character
*         followed by a character that determines the substitution
*         that is to take place.  All ordinary characters are copied
*         unchanged.
* 
* The following directives are supported in string S:
* 
* %%   Literal character %
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
        LOAD('EXT_STRFTIME_(STRING)STRING','spitbol.slf')
        OPSYN("StrfTime", "EXT_STRFTIME_")

* end of STRFTIME.INC

* Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
* Use of this source code is governed by a BSD-style
* license that can be found in the LICENSE file.

* CVT.INC
*
* CvtNS - convert integer or real number to a 4 or 8 character string.
* CvtSN - convert a 4 or 8 character string to an integer or real number.
*
* Function specifics:
*
* CvtNS - Convert Numeric to String
*  Convert integer or real number to 4 or 8 character string respectively.
*  Useful for writing numeric values in an internal form to a binary
*  (raw mode) file.
*
*  This function is the inverse of CvtSN(), which converts 4 or 8 character
*  strings to internal numeric values.
*
*      Note:   We use the unrecognized keyword "NUMERIC" so that SPITBOL passes
*  a pointer to the unconverted argument.  The function will then figure out
*  if it was passed an integer or real number.
*
*      Input:  Integer or double precision real in internal form.
*      Output: Four or Eight byte string with internal representation
*              of number.
*
*      Failure if input is any datatype except real or integer. Note that
*  CvtNS("1.23") will fail because the input argument is a string.  Use 
*  CvtNS(+"1.23") to convert a string to a true numeric before calling function.
*
*
*
* CvtSN - Convert String to Numeric
*  Convert 4 or 8 character string to integer or real number respectively.
*  Useful for reading numeric values in an internal form from a binary
*  (raw mode) file.
*
*  This function is the inverse of CvtNS(), which converts internal
*  numeric values to 4 or 8 character strings.
*
*      Input:  Four or eight character string with internal representation
*              of number.
*      Output: Integer or double precision real in internal form.
*
*      Failure if input is not a four or eight character string.
*
*      Internal representation is machine=dependent.  That is, integer 1
*      could be created using:
*
*              CVTSN(CHAR(0) CHAR(0) CHAR(0) CHAR(1))
*
*      on big-endian systems such as the RS/6000, or with:
*
*              CVTSN(CHAR(1) CHAR(0) CHAR(0) CHAR(0))
*
*      on little-endian systems such as the 80386/486/Pentium.
*
*  or equivalently,
*
*      CVTSN(CVTNS(1))

* Load functions using their external names.
        LOAD('EXT_CVTNS_(NUMERIC)STRING', 'spitbol.slf')
        LOAD('EXT_CVTSN_(STRING)', 'spitbol.slf')

* Make them available with nicer names.
        OPSYN("CvtNS", "EXT_CVTNS_")
        OPSYN("CvtSN", "EXT_CVTSN_")

* end of CVT.INC

*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* X.INC
*
* Functions to maintain 4-byte integers in an external block.
*
* An array is converted to an external block.
* Integers are stored as pure 32-bit values in slots of the external block.
* XPUT and XGET to and from the external block follow standard array
* conventions of failing if the subscript is out of range.
*
* Functions available to caller:
*
* X = XMake(N) - create and return an N-element external block.
* XPut(I,J,X) - store I in X[J].
* I = XGet(J,X) - retrieve integer in X[J].
*
*
        LOAD("EXT_XCREATE_(VECTOR)","spitbol.slf")
        LOAD("EXT_XGET_(INTEGER,EXTERNAL)INTEGER","spitbol.slf")
        LOAD("EXT_XPUT_(INTEGER,INTEGER,EXTERNAL)","spitbol.slf")
*
        OPSYN("XCreate","EXT_XCREATE_")
        OPSYN("XGet", "EXT_XGET_")
        OPSYN("XPut", "EXT_XPUT_")
        DEFINE("XMake(N)")                      :(XMake_END)
XMake   XMake = ARRAY(N - 1)
        XCreate(XMake)                          :S(RETURN)F(FRETURN)
XMake_END

* end of X.INC

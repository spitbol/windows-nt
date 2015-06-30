* Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
* Use of this source code is governed by a BSD-style
* license that can be found in the LICENSE file.

* ADDRESS.INC
*
* Get memory offset of name or variable
*
*   NameToAdr - Get memory address of name-type argument.
*
*   Usage:  (Argument is the name of a variable)
*
*   Sample calls:
*           OFFSET = NameToAdr(.X)           ;* address of variable X
*           OFFSET = NameToAdr(.T["abc"])    ;* address of T["abc"]
*
*   Returns offset in SPITBOL's data segment if successful, fails if
*   incoming argument is not a NAME variable.
*
*   ValToAdr - Get memory address of argument.
*
*   Usage: (Argument is the data value whose address is needed)
*
*   Sample calls:
*    OFFSET = ValToAdr(X)            ;* address of data in variable X
*    OFFSET = ValToAdr(T["abc"])     ;* address of data in T["abc"]
*
*   Returns offset in SPITBOL's data segment.
*
*

        LOAD("EXT_NAMEToADR_(NAME)INTEGER", "spitbol.slf")
        LOAD("EXT_VALToADR_(VALUE)INTEGER", "spitbol.slf")
        OPSYN("NameToAdr", "EXT_NAMETOADR_")
        OPSYN("ValToAdr", "EXT_VALTOADR_")

* end of ADDRESS.INC

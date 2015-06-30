*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* REALCVT.INC
*
* Converts a real number to a string with some control over the number
* of digits produced and their format.  The control parameters are modeled
* on those provided in SNOBOL4+.  For many users, it will be simpler
* to merely use sprintf to control formatting.
*
* Usage:
*   result = RealCVT(r, p, f)
*
* where
*  r  the real number to convert to a string
*  p  number of significant digits to produce. 1 <= p <= 16
*  f  format: if f is negative, produce E format notation
*        if f is greater than zero, produce F format notation
*        if f is zero, use G format (E or F depending upon r's magnitude)

        LOAD("EXT_REALCVT_(REAL,INTEGER,INTEGER)", "spitbol.slf")
        OPSYN("RealCVT", "EXT_REALCVT_")

* end of REALCVT.INC

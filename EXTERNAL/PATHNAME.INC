*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* PATHNAME.INC
*
* Emulates SNOBOL4+'s PATHNAME function.
*
* Returns the file name of the file associated with an I/O channel.
*
*
*  Usage:
*       Result = Pathname(channel)
*
*  where channel is the 2nd argument to the INPUT or OUTPUT function.
*
*  Bug: Does not return the name of a file that has been specified on
*	the command line or via an environment variable.
*

        LOAD("EXT_PATHNAME_(FILE)STRING", "spitbol.slf")
        OPSYN("Pathname", "EXT_PATHNAME_")

* end of PATHNAME.INC

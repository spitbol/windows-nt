*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* WINDOWS.INC
*
* Usage:
* -include "windows.inc"
* S = GetSystemDirectory()     Get path to Windows system directory
* S = GetWindowsDirectory()    Get path to Windows directory
* N = GetLastError()           Retrieve code for last system error
* 
        LOAD('EXT_GETSYSTEMDIRECTORY_()STRING','spitbol.slf')
        LOAD('EXT_GETWINDOWSDIRECTORY_()STRING','spitbol.slf')
        LOAD('EXT_GETLASTERROR_()INTEGER','spitbol.slf')

        OPSYN("GetSystemDirectory","EXT_GETSYSTEMDIRECTORY_")
        OPSYN("GetWindowsDirectory","EXT_GETWINDOWSDIRECTORY_")
        OPSYN("GetLastError","EXT_GETLASTERROR_")

* end of WINDOWS.inc

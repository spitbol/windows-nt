* Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
* Use of this source code is governed by a BSD-style
* license that can be found in the LICENSE file.

* DIRECTS.INC
*
* File directory functions.
*
* Useful for manipulating a file system directory tree.
*
* Usage:
* 
* CreateDir(STRING)     Create the specified directory.
* S = GetCurrentDir()   Retrieve the current working directory.
* RemoveDir(STRING)     Remove the specified directory.
* SetCurrentDir(STRING) Change to the specified directory.
* 

* Load functions using their external names.
        LOAD('EXT_CREATEDIR_(STRING)','spitbol.slf')
        LOAD('EXT_GETCURRENTDIR_()','spitbol.slf')
        LOAD('EXT_REMOVEDIR_(STRING)','spitbol.slf')
        LOAD('EXT_SETCURRENTDIR_(STRING)','spitbol.slf')

* Make them available with nicer names.
        OPSYN("CreateDir","EXT_CREATEDIR_")
        OPSYN("GetCurrentDir","EXT_GETCURRENTDIR_")
        OPSYN("RemoveDir","EXT_REMOVEDIR_")
        OPSYN("SetCurrentDir","EXT_SETCURRENTDIR_")

* end of DIRECTS.INC

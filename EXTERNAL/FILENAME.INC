* Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
* Use of this source code is governed by a BSD-style
* license that can be found in the LICENSE file.

* FILENAME.INC
*
* Lookup pathname in disk directory.
* Allows ? and * as arbitrary character matches. 
* Useful for reading a disk file directory.
*
* Note: This function is provided for compatibility with SNOBOL4+ only.
* It is limited to the 8.3 name format of MS-DOS.
*
* NEW PROGRAMS SHOULD USE THE FUNCTIONS IN FINDNAME.INC INSTEAD.
*
*  Input:  A pathname string with optional drive specifier.
*      If the filename contains wildcard character(s) (? or *),
*      successive calls may be made with a null string argument to
*      obtain additional matching file directory entries.  The
*      function fails if there is no matching filename, or when
*      the list of wildcard matches is exhausted, or if the
*      pathname string is too long or invalid.
*
*  Output: A pathname string, with drive specifier.  The string is
*      in the correct format for use in the fourth argument
*      position of the INPUT and OUTPUT functions.
*
*  Sample program to print a file directory from directory C:\TOOLS\BIN:
*
*  -INCLUDE "filename.inc"
*          OUTPUT = FileName('C:\TOOLS\BIN\*.*')   :F(NONE)
*  LOOP    OUTPUT = FileName()                     :S(LOOP)
*
*
* Note: Program contains code to filter out file directories.
* 
* The FINDNAME set of functions is the preferred way to scan for files
* and directories, and should be used instead of this function.
* 
        LOAD('EXT_FILENAME_(STRING)STRING','spitbol.slf')
        OPSYN("FileName", "EXT_FILENAME_")

* end of FILENAME.INC

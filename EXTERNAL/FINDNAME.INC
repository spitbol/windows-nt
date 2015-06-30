*  Copyright 1994-2003, Mark Emmer, Catspaw, Inc.  All rights reserved.
*  Use of this source code is governed by a BSD-style
*  license that can be found in the LICENSE file.

* FINDNAME.INC
*
*
* Lookup pathname in disk directory.
* Allows ? and * as arbitrary character matches. 
*
* Useful for reading a disk file directory.
*
* Usage:
*
*  x = FindFirst(S)             :F(No_Match)
*  Input:  A pathname string with optional drive specifier.
*      If the filename contains wildcard character(s) (? or *),
*      successive calls may be made to FindNext() to obtain
*      additional matching file directory entries.  The FindFirst
*      function fails if there is no matching filename, or if the
*      pathname string is too long or invalid.
*
*  Output: An external data block containing the search result information.
*      This block can be passed to FindNext to find the next matching
*      entry, or to FindInfo to extract specific information from the block.
*
*  FindNext(x)                  :F(No_More)
*  Input: an external block from a previous call to FindFirst or FindNext.
*      Find the next file or directory meeting the search criteria.  Fail if
*      no more matches.
*  Output: Returns null string, with updated state information in the external
*      block x.
*
*  FindClose(x)
*  Input: an external block from a previous call to FindFirst or FindNext.
*  Output: Null string.
*  Action: The system resources reserved by the file search are releaesd.
*      Use only if you do not make FindNext calls until one fails.  If
*      FindNext fails, the system resources are released automatically.
*
*  result = FindInfo(x,S)
*  Input: x = an external block from a previous call to FindFirst or FindNext,
*      s = a one-character string describing the item of information to
*      extract and report from the state information. The following letters
*      are allowed:
*      "N" or "n" -- file or directory name for this entry.
*      "D" or "d" -- alternative MS-DOS 8.3 filename if different than
*                    regular filename.  Otherwise, it is the null string.
*      "A" or "a" -- return attributes as concatenated string of:
*                      null = normal file with no other attributes
*                      'A'  = File needs to be Archived
*                      'D'  = Directory
*                      'H'  = Hidden
*                      'R'  = Read-only
*                      'S'  = System
*                      'T'  = Temporary
*      "C" or "c" -- creation date in the form "mm/dd/yyyy hh:mm:ss.xxx"
*                    where xxx is milliseconds.  If the operating system
*                    does not maintain creation date, FindInfo fails.
*      "L" or "l" -- last access date in the form "mm/dd/yyyy hh:mm:ss.xxx"
*                    where xxx is milliseconds.  If the operating system
*                    does not maintain last access date, FindInfo fails.
*      "W" or "w" -- last write date in the form "mm/dd/yyyy hh:mm:ss.xxx"
*                    where xxx is milliseconds.
*      "S" or "s" -- file size in bytes.  If less than 2,147,483,648 it is
*                    returned as an integer.  If larger, it is returned as
*                    a real number.
*
*  Sample program to print a file directory from directory C:\TOOLS\BIN:
*
*  -INCLUDE "findname.inc"
*          State = FindFirst('C:\TOOLS\BIN\*.*')   :F(NONE)
*  LOOP    OUTPUT   = FindInfo(State,'N')
*          FindNext(State)                         :S(LOOP)
*  NONE
*

* Load functions using their external names.
        LOAD("EXT_FINDFIRST_(STRING)EXTERNAL", "spitbol.slf")
        LOAD("EXT_FINDINFO_(EXTERNAL,STRING)", "spitbol.slf")
        LOAD("EXT_FINDCLOSE_(EXTERNAL)",       "spitbol.slf")
        LOAD("EXT_FINDNEXT_(EXTERNAL)EXTERNAL","spitbol.slf")

* Make them available with nicer names.
        OPSYN("FindFirst", "EXT_FINDFIRST_")
        OPSYN("FindInfo",  "EXT_FINDINFO_")
        OPSYN("FindClose", "EXT_FINDCLOSE_")
        OPSYN("FindNext",  "EXT_FINDNEXT_")

* end of FINDNAME.INC

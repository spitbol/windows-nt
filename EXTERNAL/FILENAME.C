/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * filename.c
 *
 *
 * Lookup pathname in disk directory.
 * Allows ? and * as arbitrary character matches. 
 *
 * Useful for reading a disk file directory.
 *
 * Load from SPITBOL with:
 *   LOAD('EXT_FILENAME_(STRING)STRING','filename.slf')
 *
 * or if all your external functions are gathered into a single library:
 *   LOAD('EXT_FILENAME_(STRING)STRING','spitbol.slf')
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
 *  -include "filename.inc"
 *          OUTPUT = FileName('C:\TOOLS\BIN\*.*')   :F(NONE)
 *  LOOP    OUTPUT = FileName()                     :S(LOOP)
 *
 *
 * Note: Program contains code to filter out file directories.
 * 
 * The FindName set of functions is the preferred way to scan for files
 * and directories, and should be used instead of this function.
 * 
 * To build under AIX 3:
 *   cc -o filename.slf -bM:SRE -bE:filename.exp -e _nostart filename.c
 *
 * To build under AIX 4:
 *   cc -o filename.slf -bM:SRE -bE:filename.exp -bnoentry filename.c
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M filename.c extrnlib.obj
 *
 */
#include <windows.h>
#include <winnt.h>
#include "system.h"
#include "extern32.h"

static HANDLE h = INVALID_HANDLE_VALUE;
static WIN32_FIND_DATA  result;

entry(EXT_FILENAME_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   char             buf[MAX_PATH];

   if (larg != 0 && h != INVALID_HANDLE_VALUE)
   {
      FindClose(h);                 /* New search, so close old search */
      h = INVALID_HANDLE_VALUE;
   }
   
   if (larg >= MAX_PATH)
      return FAIL;
   
   while (TRUE)
   {

      if (larg != 0)
      {
         /* new search */
         strncpy(buf, parg, larg); buf[larg] = '\0';    /* create C string */
         h = FindFirstFile(buf, &result);
         if (h == INVALID_HANDLE_VALUE)
            return FAIL;
         larg = 0;                              /* in case found directory */
      }
      else
      {
         /* Null string.  Wants to continue old search */
         if (h == INVALID_HANDLE_VALUE)
            return FAIL;
         if (!FindNextFile(h, &result))
         {
            FindClose(h);                          /* no more file matches */
            h = INVALID_HANDLE_VALUE;
            return FAIL;
         }
      }

      /* Return if found object is not a directory, otherwise, keep searching */
      if ((result.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
         return retstrf(result.cFileName, presult);
   }
}

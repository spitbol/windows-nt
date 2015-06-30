/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * WINDOWS.C
 *
 *
 * Get basic Windows information.
 *
 * Load in SPITBOL with:
 *  LOAD('EXT_GETSYSTEMDIRECTORY_()STRING','windows.slf')
 *  LOAD('EXT_GETWINDOWSDIRECTORY_()STRING','windows.slf')
 *  LOAD('EXT_GETLASTERROR_()INTEGER','windows.slf')
 *
 * or if all your external functions are gathered into a single library:
 *  LOAD('EXT_GETSYSTEMDIRECTORY_()STRING','spitbol.slf')
 *  etc.
 *
 *
 * Usage:
 * -include "windows.inc"
 * S = GetSystemDirectory()     Get path to Windows system directory
 * S = GetWindowsDirectory()    Get path to Windows directory
 * N = GetLastError()           Retrieve code for last system error
 * 
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M windows.c extrnlib.obj
 *
 */
#include <windows.h>
#include <winnt.h>
#include <stdio.h>
#include "system.h"
#include "extern32.h"


/* GETSYSTEMDIRECTORY() -- Get Path to Windows System Directory
 */
entry(EXT_GETSYSTEMDIRECTORY_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   if (!GetSystemDirectory(presult->scb.scstr, buflen))
      return FAIL;

   presult->scb.sclen = strlen(presult->scb.scstr);
   return BL_SC;
}       

/* GETWINDOWSDIRECTORY() -- Get Path to Windows Directory
 */
entry(EXT_GETWINDOWSDIRECTORY_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   if (!GetWindowsDirectory(presult->scb.scstr, buflen))
      return FAIL;

   presult->scb.sclen = strlen(presult->scb.scstr);
   return BL_SC;
}       

/* GETLASTERROR() -- Get Windows Error Code for Last Failing Windows Call
 */
entry(EXT_GETLASTERROR_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   return retint(GetLastError(), presult);
}       

/* ----------------------------  end of windows.c ----------------------*/

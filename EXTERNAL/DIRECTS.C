/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * DIRECTS.C
 *
 *
 * File directory functions.
 *
 * Useful for reading a disk file directory.  Load from SPITBOL with:
 *  LOAD('EXT_CREATEDIR_(STRING)','directs.slf')
 *  LOAD('EXT_GETCURRENTDIR_()','directs.slf')
 *  LOAD('EXT_REMOVEDIR_(STRING)','directs.slf')
 *  LOAD('EXT_SETCURRENTDIR_(STRING)','directs.slf')
 *
 * or if all your external functions are gathered into a single library:
 *  LOAD('EXT_CREATEDIR_(STRING)','spitbol.slf')
 *  etc.
 *
 *
 * Usage:
 * -include "directs.inc"
 * CreateDir(STRING)     Create the specified directory.
 * GetCurrentDir()       Retrieve the current working directory.
 * RemoveDir(STRING)     Remove the specified directory.
 * SetCurrentDir(STRING) Change to the specified directory.
 * 
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M directs.c extrnlib.obj
 *
 */
#include <windows.h>
#include <winnt.h>
#include <stdio.h>
#include "system.h"
#include "extern32.h"

/* CREATEDIR(PATH) -- Create the specified directory.
 */
entry(EXT_CREATEDIR_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   char                 buf[MAX_PATH];
   SECURITY_ATTRIBUTES  sa;

   if (!larg || larg >= MAX_PATH)
      return FAIL;
   
   sa.nLength = sizeof(SECURITY_ATTRIBUTES);
   sa.lpSecurityDescriptor = NULL;
   sa.bInheritHandle = TRUE;

   strncpy(buf, parg, larg); buf[larg] = '\0';    /* create C string */
   if (!CreateDirectory(buf, &sa))
      return FAIL;
   presult->scb.sclen = 0;
   return BL_SC;
}       

/* GETCURRENTDIR() -- Retrieve the current working directory.
 */
entry(EXT_GETCURRENTDIR_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   DWORD            nDir;
   char             buf[MAX_PATH];

   nDir = GetCurrentDirectory(MAX_PATH, buf);
   if (!nDir || nDir >= MAX_PATH)
      return FAIL;
   
   return retnstrt(buf, nDir, presult);
}       



/* REMOVEDIR(PATH) -- Remove the specified directory.
 */
entry(EXT_REMOVEDIR_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   char             buf[MAX_PATH];

   if (!larg || larg >= MAX_PATH)
      return FAIL;
   
   strncpy(buf, parg, larg); buf[larg] = '\0';    /* create C string */
   if (!RemoveDirectory(buf))
      return FAIL;
   presult->scb.sclen = 0;
   return BL_SC;
}       


/* SETCURRENTDIR(PATH) -- Change to the specified directory.
 */
entry(EXT_SETCURRENTDIR_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   char             buf[MAX_PATH];

   if (!larg || larg >= MAX_PATH)
      return FAIL;
   
   strncpy(buf, parg, larg); buf[larg] = '\0';    /* create C string */
   if (!SetCurrentDirectory(buf))
      return FAIL;
   presult->scb.sclen = 0;
   return BL_SC;
}       


/* ----------------------------  end of findname.c ----------------------*/

/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/
/*
 * CLIPBRD.C
 *
 *
 * Clipboard functions for Windows 95/Windows NT.
 *
 * Useful for transferring data to and from the clipboard.
 * Load in SPITBOL with:
 *  LOAD('EXT_SETCLIPBOARDDATA_(STRING)','clipbrd.slf')
 *  LOAD('EXT_GETCLIPBOARDDATA_()','clipbrd.slf')
 *
 * or if all your external functions are gathered into a single library:
 *  LOAD('EXT_SETCLIPBOARDDATA_(STRING)','spitbol.slf')
 *  etc.
 *
 *
 * Usage:
 * -include "clipbrd.inc"
 * SetClipboardData(S)          Put text string into clipboard.
 * S = GetClipboardData()       Retrieve text string from clipboard.
 * 
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M clipbrd.c extrnlib.obj
 *
 * Note: Because these functions rely on import32.lib, it will not
 * load or run under MS-DOS or Windows 3.11.
 *
 */
#include <windows.h>
#include <winnt.h>
#include <stdio.h>
#include "system.h"
#include "extern32.h"

/* SETCLIPBOARDDATA(TEXT) -- Place text into clipboard.
 */
entry(EXT_SETCLIPBOARDDATA_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   HGLOBAL hglb;
   LPTSTR  lptstr;
   
   if (!OpenClipboard(NULL))
      return FAIL;

   if (!EmptyClipboard())
   {
      CloseClipboard();
      return FAIL;
   }

   if (larg)
   {
      hglb = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, larg+1);
      if (hglb == NULL)
      {
        CloseClipboard();
        return FAIL;
      }
      lptstr = GlobalLock(hglb);
      memcpy(lptstr, parg, larg);
      lptstr[larg] = '\0';
      GlobalUnlock(hglb);
      SetClipboardData(CF_TEXT, hglb);
   }
   
   CloseClipboard();
   presult->scb.sclen = 0;
   return BL_SC;
}       

/* GETCLIPBOARDDATA() -- Retrieve text from clipboard.
 */
entry(EXT_GETCLIPBOARDDATA_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   HGLOBAL hglb;
   LPTSTR  lptstr;
   mword   result;
   
   if (!IsClipboardFormatAvailable(CF_TEXT))
      return FAIL;

   if (!OpenClipboard(NULL))
      return FAIL;

   hglb = GetClipboardData(CF_TEXT);
   if (hglb == NULL || (lptstr = GlobalLock(hglb)) == NULL)
   {
     CloseClipboard();
     return FAIL;
   }
   result = retstrt(lptstr, presult);
   if (result == FAIL)                    /* Too long to return via temp area */
     result = retstrf(lptstr, presult);   /* Sure hope it doesn't move on us */
   GlobalUnlock(hglb);
   CloseClipboard();
   return result;
}       

/* ----------------------------  end of clipbrd.c ----------------------*/

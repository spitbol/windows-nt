/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/
* CLIPBRD.INC
*
* Clipboard functions for Windows 95/Windows NT.
*
* Useful for transferring data to and from the clipboard.
*
* Usage:
* 
* SetClipboardData(STRING)     Put text string into clipboard.
* S = GetClipboardData()       Retrieve text string from clipboard.
* 
* Note: Because these functions rely on import32.lib, they will not
* load or run under MS-DOS or Windows 3.11.
*

* Verify that not running under MS-DOS or Windows 3.11.
*
        HOST() ? FENCE BREAKX('M') 'MS-DOS'             :F(CLIP_BRD_1)
        TERMINAL = "Clipbuard functions not available under MS-DOS"
        &CODE = 1                                       :(END)

* Load functions using their external names.
CLIP_BRD_1
        LOAD('EXT_GETCLIPBOARDDATA_()','nt95only.slf')
        LOAD('EXT_SETCLIPBOARDDATA_(STRING)','nt95only.slf')

* Make them available with nicer names.
        OPSYN("GetClipboardData", "EXT_GETCLIPBOARDDATA_")
        OPSYN("SetClipboardData", "EXT_SETCLIPBOARDDATA_")

* end of CLIPBRD.INC

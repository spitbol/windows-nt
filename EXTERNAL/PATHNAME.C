/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * pathname.c
 *
 * PATHNAME(f) -- return file name associated with a channel.
 *
 * Load in spitbol with:
 *   LOAD("EXT_PATHNAME_(FILE)STRING", "pathname.slf")
 *
 * or if all your external functions are gathered into a single library:
 *   LOAD("EXT_PATHNAME_(FILE)STRING", "spitbol.slf")
 *
 * Usage:
 *  -include "pathname.inc"
 *  RESULT = PathName(channel)
 *
 *  FILE is the channel used in the second argument to the INPUT or OUTPUT
 *  function.
 *
 *  Limitation: Does not return the name of a file that has been specified on
 *	the command line or via an environment variable.
 *
 * To build under AIX 3:
 *	 cc -o pathname.slf -bM:SRE -bE:pathname.exp -e _nostart pathname.c
 *
 * To build under AIX 4:
 *	 cc -o pathname.slf -bM:SRE -bE:pathname.exp -bnoentry pathname.c
 *
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M pathname.c extrnlib.obj
 
 * To build under SunOS with GNU:
 *  gcc -sun4 -nostdlib -o pathname.slf pathname.c extrnlib.o
 *
 * Use include file pathname.inc to load.
 *
 */
#include "system.h"
#include "extern32.h"


entry(EXT_PATHNAME_)(union block *presult, misc *pinfo, struct fcblk *fcbptr)
{
   strncpy(presult->scb.scstr, fcbptr->fcbiob->iobfnm->scstr, fcbptr->fcbiob->iobfnm->sclen);
   presult->scb.sclen = fcbptr->fcbiob->iobfnm->sclen;
   return BL_SC;
}

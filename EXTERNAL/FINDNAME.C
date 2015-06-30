/*
  Copyright 1994-2003 Mark Emmer, Catspaw, Inc.  All Rights Reserved.
  Use Of this source code is governed by a BSD-style
  license that can be found in the LICENSE file.
*/

/*
 * FINDNAME.C
 *
 *
 * Lookup pathname in disk directory.
 * Allows ? and * as arbitrary character matches. 
 *
 * Useful for reading a disk file directory.  Load from SPITBOL with:
 *
 *  LOAD('EXT_FINDFIRST_(STRING)','findname.slf')
 *  LOAD('EXT_FINDNEXT_(EXTERNAL)','findname.slf')
 *  LOAD('EXT_FINDINFO_(EXTERNAL,STRING)','findname.slf')
 *  LOAD('EXT_FINDCLOSE_(EXTERNAL)','findname.slf')
 *
 * or if all your external functions are gathered into a single library,
 * substitute the library name:
 *  LOAD('EXT_FINDFIRST_(STRING)','spitbol.slf')
 *   etc.
 *
 *  Input:  A pathname string with optional drive specifier.
 *      If the filename contains wildcard character(s) (? or *),
 *      successive calls may be made to FindNext() to obtain
 *      additional matching file directory entries.  The FindFirst
 *      function fails if there is no matching filename, or if the
 *      pathname string is too long or invalid.  FindNext fails when
 *      the list of matching entries is exhausted.
 *
 *  Output: An external data block containing the search result information.
 *      This block can be passed to FindNext to find the next matching
 *      entry, or to FindInfo to extract specific information from the block.
 *
 *      If the search does not continue until FindNext fails, the search must
 *      be terminated with an explicit call to FindClose.
 *
 *  Sample program to print a file directory from directory C:\TOOLS\BIN:
 *
 *  -include "findname.inc"
 *          FILEINFO = FindFirst('C:\TOOLS\BIN\*.*')   :F(NONE)
 *  LOOP    OUTPUT   = FindInfo(FILEINFO,'N')
 *          FindNext(FILEINFO)                         :S(LOOP)
 *  NONE
 *
 *  FindInfo accepts a one-character string as its second argument.  The
 *  following letters are allowed:
 *      "N" or "n" -- full file or directory name for this entry.
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
 *                      'W'  = Atomic write
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
 * To build under Borland C++ 4.x:
 *   bcc32 -WXD -a1 -d -w-par -M findname.c extrnlib.obj
 *
 * Coding caution -- Because the external block passed in to FindNext and
 * FindInfo is itself relocatable within the heap, there is a subtle trap
 * for the unwary.  You cannot return a pointer to a string within the block,
 * because the return path to SPITBOL may trigger a garbage collection when it
 * allocates memory for the string.  Such a garbage collection could leave the
 * string pointer invalid.  Therefore, we use the retstrt function which copies
 * the string to a safe, non-relocatable result area prior to allocating the
 * memory on the heap.
 *
 * Further, the initial draft of this code had FindNext return the external
 * block as its result.  The same problem appeared -- the pointer to the
 * incoming argument became invalid when the return code caused a garbage
 * collection when allocating an area to copy the block to.
 *
 */
#include <windows.h>
#include <winnt.h>
#include <stdio.h>
#include "system.h"
#include "extern32.h"

static mword filetime(FILETIME *ft, union block *presult);

typedef struct fileinfo {
	HANDLE      		h;
	WIN32_FIND_DATA	result;
	}	FILEINFO;

#define AlignSize sizeof(mword)
#define xnblkSize ((sizeof(FILEINFO) + 2*sizeof(mword) + AlignSize - 1) & ~(AlignSize - 1))

/* FINDFIRST -- lookup user-supplied name and return external data block if
 *              found.  The external data block maintains state information
 *              on the search.
 */
entry(EXT_FINDFIRST_)(union block *presult, misc *pinfo, far char *parg, mword larg)
{
   char             buf[MAX_PATH];
   FILEINFO         *fip;

   if (!larg || larg >= MAX_PATH || sizeof(FILEINFO) > buflen)
      return FAIL;
   
   strncpy(buf, parg, larg); buf[larg] = '\0';    /* create C string */
   fip = (FILEINFO*)presult->xnb.xnu.xndta;
   fip->h = FindFirstFile(buf, &fip->result);
   if (fip->h == INVALID_HANDLE_VALUE)
      return FAIL;
   presult->xnb.xnlen = sizeof(FILEINFO);
   return BL_XN;
}       

/* FINDNEXT -- Given an external data block containing FindFile state 
 *             information, find the next file meeting the search criteria.
 */
entry(EXT_FINDNEXT_)(union block *presult, misc *pinfo, far union block *parg)
{
	FILEINFO *fip;

	if (parg->xnb.xntyp != xn ||
       parg->xnb.xnlen != xnblkSize)
      return FAIL;
	
	fip = (FILEINFO *)parg->xnb.xnu.xndta;
   if (fip->h == INVALID_HANDLE_VALUE)
      return FAIL;

   if (!FindNextFile(fip->h, &fip->result))
   {
      FindClose(fip->h);                          /* no more file matches */
      fip->h = INVALID_HANDLE_VALUE;
      return FAIL;
   }

   presult->scb.sclen = 0;
   return BL_SC;
}       


/* FINDCLOSE -- Given an external block containing FindFile state information,
 *              closes the search and returns the handle to the system.
 */        
entry(EXT_FINDCLOSE_)(union block *presult, misc *pinfo, far union block *parg)
{
	FILEINFO *fip;

	if (parg->xnb.xntyp != xn ||
       parg->xnb.xnlen != xnblkSize)
      return FAIL;
	
	fip = (FILEINFO *)parg->xnb.xnu.xndta;
   if (fip->h != INVALID_HANDLE_VALUE)
   {
      FindClose(fip->h);                          /* no more file matches */
      fip->h = INVALID_HANDLE_VALUE;
   }

   presult->scb.sclen = 0;
   return BL_SC;
}		

/* FINDINFO -- given an external block with state information from a previous
 *             FINDFIRST or FINDNEXT call, extracts a particular piece of
 *             information from it.  The information desired is specified in
 *             a one-character string second argument.
 */        
entry(EXT_FINDINFO_)(union block *presult, misc *pinfo, far char *parg2, mword larg2, far union block *parg1)
{
	FILEINFO *fip;
	char     *p;
    char     attr[16];
    DWORD    attributes;

	if (parg1->xnb.xntyp != xn ||
       parg1->xnb.xnlen != xnblkSize ||
		 !larg2)
      return FAIL;
	
	fip = (FILEINFO *)parg1->xnb.xnu.xndta;
   if (fip->h == INVALID_HANDLE_VALUE)
        return FAIL;

   switch (*parg2)
   {
      case 'A':                                     /* attributes */
      case 'a':
         p = attr;
         attributes = fip->result.dwFileAttributes;
         if (attributes & FILE_ATTRIBUTE_ARCHIVE)
            *p++ = 'A';
         if (attributes & FILE_ATTRIBUTE_DIRECTORY)
            *p++ = 'D';
         if (attributes & FILE_ATTRIBUTE_HIDDEN)
            *p++ = 'H';
         if (attributes & FILE_ATTRIBUTE_READONLY)
            *p++ = 'R';
         if (attributes & FILE_ATTRIBUTE_SYSTEM)
            *p++ = 'S';
         if (attributes & FILE_ATTRIBUTE_TEMPORARY)
            *p++ = 'T';
         *p = '\0';
         return retstrt(attr, presult);

      case 'C':                                     /* creation time */
      case 'c':
         return filetime(&fip->result.ftCreationTime, presult);

      case 'L':                                     /* last access time */
      case 'l':
         return filetime(&fip->result.ftLastAccessTime, presult);

      case 'W':                                     /* last write time */
      case 'w':
         return filetime(&fip->result.ftLastWriteTime, presult);

      case 'S':                                     /* file size */
      case 's':
         if (fip->result.nFileSizeHigh || fip->result.nFileSizeLow & 0x80000000)
         {
            WORD exp;
            union {
            double  r;
            DWORD   l[2];
            WORD    w[4];
            } cvt;

            /* Too large for a positive SPITBOL integer.
             * Convert 53-bit integer to real.  That's the most we can do.
             */
            if (fip->result.nFileSizeHigh & 0xFFE00000)
               return FAIL;
            exp = 0x3FF + 52;                       /* Biased IEEE exponent */
            cvt.l[0] = fip->result.nFileSizeLow;
            cvt.l[1] = fip->result.nFileSizeHigh;
            while ((cvt.l[1] & 0x00100000) == 0)    /* Normalize */
            {
                cvt.l[1] <<= 1;
                if (cvt.l[0] & 0x80000000)
                    cvt.l[1] += 1;
                cvt.l[0] <<= 1;
                exp--;
            }
				cvt.w[3] = (WORD)((exp << 4) | (cvt.w[3] & 0x000F));
            return retreal(cvt.r, presult);
         }
         else
            return retint((int)fip->result.nFileSizeLow, presult);
      
      case 'N':                                     /* file/directory name */
      case 'n':
         return retstrt(fip->result.cFileName, presult);
        
      case 'D':                                     /* MS-DOS 8.3 alternate name */
      case 'd':
         return retstrt(fip->result.cAlternateFileName, presult);

      default:
         return FAIL;
   }       
}		


/* filetime -- helper function to convert file times to printable form.
 */
static mword filetime(FILETIME *ft, union block *presult)
{
    SYSTEMTIME sysTime;
    char buf[40];
	 FILETIME   localtime;
         
    if (!ft->dwLowDateTime && !ft->dwHighDateTime)
       return FAIL;
	 if (!FileTimeToLocalFileTime(ft, &localtime))
	 	 return FAIL;
    if (!FileTimeToSystemTime(&localtime, &sysTime))
       return FAIL;
    sprintf(buf, "%.2hd/%.2hd/%.4hd %.2hd:%.2hd:%.2hd.%.3hd",
        sysTime.wMonth, sysTime.wDay,    sysTime.wYear,
        sysTime.wHour,  sysTime.wMinute, sysTime.wSecond,
        sysTime.wMilliseconds);
    return retstrt(buf,presult);
}

/* ----------------------------  end of findname.c ----------------------*/

##           SPITBOL for Windows NT - Version 1.30.22

                      December 24, 2003


This version of SPITBOL uses the Windows NT API for O/S support.
It does not make any DOS or INT xx calls, and runs in a Win NT
command window.

Because Windows 95 command windows also support the Windows NT
API, you will discover that this program also runs under
Windows 95, and provides long-filename support as well.  A few
anomalies have been noted when running under Windows 95, which
appear to be due to bugs in the operating system.  These appear
in the areas of HOST-function screen control and creating pipes
within SPITBOL to older, 16-bit applications.

The program also runs on native MS-DOS and Windows 3.1 systems
using the separate DOS Extender files provided.  The three files
32RTM.EXE, DPMI32VM.OVL, and WINDPMI.386 must be installed on
your system (instructions below).  THESE FILES DO NOT NEED TO BE
INSTALLED IF YOU ARE RUNNING WINDOWS 95 OR WINDOWS NT.

This version provides all of the features of MS-DOS SPITBOL-386
including the generation of EXE files and the use of external
functions.  The following limitations apply at this time:

 * The PEEK and POKE functions are not available.

 * External functions loaded with the LOAD function are not saved
   in any SPITBOL SAVE (.spx) files.  Your program must reload all
   external functions when a SAVE file is resumed.

This version of SPITBOL provides these features which are not
present in the Intel or Pharlap DOS Extended SPITBOL-386:

 * Pipes may be opened as files when running on multi-tasking systems
   like Windows NT.  See the description of the pipes at the end of the
   description of the INPUT function in chapter 19 of the manual.

 * Rudimentary support is provided for mouse input via HOST functions.

The following other items should be noted:

 * Long file names are supported.  Names with embedded blanks
   must be quoted on the command line.  Because blank is a
   valid filename character, it may no longer be used as a
   separator between the file name and file processing options
   in calls to the INPUT and OUTPUT function.  Use square
   brackets to delimit options.  That is, use:

```
	INPUT(.in, .in, "filename[-r5000]")
	INPUT(.in, .in, "a long file name[-r5000]")
```

   instead of

```
	INPUT(.in, .in, "filename -r5000")
	INPUT(.in, .in, "a long file name -r5000")
```

* The file names "con" and "conin$" are both acceptable for keyboard
   input, as are "con" and "conout$" for screen output.

* File ids 0, 1, and 2 may be used with the -f option in the INPUT and
   OUTPUT functions, and retain their normal meaning of Standard Input,
   Standard Output, and Standard Error respectively.

* The BUFFER data type that was added as a test in version 1.27 has
   been removed.

* The maximum size file that can be read or written is 2 GB, which arises
   from the largest positive integer that can be stored in a SPITBOL signed
   32-bit integer.  If you need to access files as large as 4 GB (Windows 95)
   or larger than 4 GB (Windows NT), please contact Catspaw.


## Contents of the release when delivered electronically:

``
SPITBOL.EXE               SPITBOL compiler and interpreter
CODE.SPT                  Interactive statement execution program
HOST.INC                  SPITBOL include file for HOST functions
MOUSE.SPT                 Test program demonstrating mouse input
EXTERNAL.TXT              Information on using external functions
HISTORY.TXT		  System changes through version 1.30
README.TXT                This file
SPITBOL.SLF               External functions for all environments
NT95ONLY.SLF              External functions specific to Windows 95 and NT
32RTM.EXE                 Borland DOS Extender to run NT SPITBOL under MS-DOS
DPMI32VM.OVL              Borland DOS Extender overlay
WINDPMI.386               Borland DOS Extender to run under Windows 3.11
EXTERNAL\*.*              External functions -- source and include files
EXTERNAL\TESTS\*.*        External function tests
```


## Installing SPITBOL under Windows NT or Windows 95:

Merely copy the SPITBOL.EXE file to any directory specified in your
PATH environment variable.  No other configuration or installation
action is required.

SPITBOL can then be executed from any command window or MS-DOS prompt.
Even though you use an "MS-DOS prompt", SPITBOL is not running as an
MS-DOS program -- it is a true, native, 32-bit Windows console application.


Installing the Borland DOS Extender to run NT SPITBOL under MS-DOS or
Windows 3.11 or Windows for Workgroups:

1. Copy the files 32RTM.EXE, DPMI32VM.OVL, and WINDPMI.386 to a directory
on your hard disk that is also specified in your PATH environment variable.

2. Edit the SYSTEM.INI file in your \WINDOWS directory.  Find the
section that begins:

```
[386Enh]
```

and add the following line:

```
device=C:\DOS\WINDPMI.386
```

If you copied WINDPMI.386 to another directory, modify the path accordingly.


## Changes in 1.30.22:

1. Add Mouse Events 16 and 32 to signal mouse wheel moved up or down.

## Changes in 1.30.21:
1. Correct problem of not intercepting trap generated by 0.0 / 0.0.

## Changes in 1.30.20:

1. Correct column number displayed in error messages.

2. When reloading a SAVE file with additional parameters on the command line,
   SPITBOL was attempting to open the first parameter following the SAVE file
   name as an input file.  This has been corrected; parameters following
   the SAVE file name are available to the HOST function, but are otherwise
   ignored.

3. Default stack size increased from 32 KBytes to 128 KBytes.  The stack
   size can be explicitly set with the -s##K command line option.

## Changes in 1.30.19:

1. Fixed bug converting a real number with value "minus zero"
   to a string.  Such a value is created by the X86 numeric
   coprocessor from operations such as -1.0 * 0.0 and 0.0 / -1.0.
   It should convert to the string "0." and the minus nature of
   the zero should not be visible to the user.

## Changes in 1.30.18:

1. Correct problem with backspace character during keyboard input under
   Windows 95/98 by removing all special keyboard input code that was
   added in version 1.30.14 for Microsoft console input bugs in early version
   of Windows 95.

2. Distinguish Windows 95 from Windows 98 in HOST() result string:
   "80386:Win95:Macro SPITBOL 3.7(ver 1.30.18) #2xxxx"
   "80386:Win98:Macro SPITBOL 3.7(ver 1.30.18) #2xxxx"

## Changes in 1.30.17:

1. Defend system from GOTO SCONTINUE loops (error 321).  Limit use of
   SCONTINUE to resuming execution following a user interrupt ONLY (error
   331).

2. Defend system from GOTO CONTINUE following an error evaluating an
   expression within a complex failure goto (error 332).

3. All of the above errors are now classified as fatal.  Three fatal errors
   will cause execution to terminate.

4. Resumed the acceptance of '/' as a option specifier on the command line.

5. Fixed bug when converting a negative real number to an integer.


## Changes in 1.30.16:

1. Fixed bug that crashed system with expressions like
      DATATYPE(CONVERT("","PATTERN"))


## Changes in 1.30.15:

1. Generalized the [-fn] option in the INPUT and OUTPUT functions to allow
   values of n other than 0, 1, and 2 (standard input, output, and error).
   A program could create a file handle in an external function, and then
   associate it with a SPITBOL I/O channel.  The file open.c (and open.inc)
   demonstrates this technique.


## Changes in 1.30.14:

1. Corrected problem with pipes created with SPITBOL (via the INPUT or OUTPUT
   function).  Pipes had not been working in the last 6 versions.

2. Added workaround for console input of control-Z character under Windows 95.
   The Win95 console driver was blindly echoing all input, including
   control-Z, which in turn was causing the screen output channel to shut
   down.  Under Win95, SPITBOL now reads from console input in binary, one-
   character-at-a-time mode, and only echos a character if it is not a
   control-Z.

3. Added workaround for console input of numeric keypad characters with CAPS-
   LOCK on under Windows 95.  Yet another Microsoft console bug...


## Changes in 1.30.13:

1. Added -T=file command-line option to force TERMINAL output to a file
   other than the system standard error file.

2. Display copyright notice, version and serial numbers in addition
   to options list in response to typing "spitbol" on the command line
   without specifying a program file name.


## Changes in 1.30.12:

1. When writing a string containing a control-Z character to the TERMINAL
   variable under MS-DOS, eliminate unrecoverable I/O error message.  Output
   now stops short on the control-Z (EOF) character, as expected.


## Changes in 1.30.11:

1. Previously, SPITBOL would attempt to open an INCLUDE file in the current
   directory and in the directories specified with the SNOLIB environment
   variable.  It would fail if not found in either location.

In this version, SPITBOL will now additionally search for an INCLUDE file
   in the directory where the SPITBOL executable resides, and in the
   directory of the first SPITBOL source file.

2. The external library functions have been revised to permit compilation
   with both Borland C and Microsoft Visual C++.  See the file external.txt
   for more information on external functions.

3. Added HOST function 215.  P = HOST(215) returns the path of the
   Windows 95 or NT directory, or the directory containing the 32RTM.EXE
   DOS Extender if running under MS-DOS or Windows 3.1.

4. Split the external function library into two files:

*     spitbol.slf  - external functions that run in all environments --
                    MS-DOS, Windows, Windows 95, Windows NT.
*     nt95only.slf - external functions that only run in the Windows 95
                    or Windows NT environment (clipboard, sockets).


## Changes in 1.30.10:

1. Fixed bug when execution of the LOAD function triggered a garbage
   collection.

2. Fixed bug in FINDNAME.C that reported file times in UTC instead of
   local time.

3. To support year 2000 and the need for 4-digit years, we have added
   an optional integer argument to DATE() function to specify the form
   of the date string:

| Call        |Format |
|:------------|:------------------------------------------------------|
|   DATE()    |MM/DD/YY HH:MM:SS     (the default, as before)|
|   DATE(0)   |MM/DD/YY HH:MM:SS     (the default, as before)|
|   DATE(1)   |MM/DD/YYYY HH:MM:SS   (existing style with 4-digit year)|
|   DATE(2)   |YYYY-MM-DD HH:MM:SS   (ISO standard with 4-digit year)|


    Other integer values outside the range 0-2 behave like 0.


## Changes in 1.30.9:

1. Fixed bug when INCLUDE statement appeared within a CODE function
   call and followed by a goto:

```
        C = CODE("-INCLUDE 'foo.inc'; :(DONE)")  :S<C>F(ERROR)
   DONE ...
```

2. Removed '/' as a command line option specifier character.  All command
   line options must be specified with '-'.


## Changes in 1.30.8:

1. Implemented the INTCALL host function when running under MS-DOS or
   Windows 3.1.  This function will fail if running under Windows 95
   or Windows NT.

2. Reorganized the EXTERNAL directory and combined all functions into a
   single library file, SPITBOL.SLF.

3. Added a HOST(-1, N) call to obtain information about SPITBOL's memory
   usage.  Allowable values for N and the items returned are:

*   0 = The increment (in bytes) by which memory is expanded when SPITBOL's
       work space becomes full.  Command line parameter -i.

*   1 = The maximum size (in bytes) for the workspace.  Command line
       parameter -d.

*   2 = Lowest memory address used by the workspace.

*   3 = Current highest address used by the workspace.

*   4 = Maximum stack size (in bytes).  Command line parameter -s minus
       400 bytes for a safety margin.

*   5 = Current amount of stack space in use (bytes).

*   6 = Size of a SPITBOL word, in bytes.

Note that the maximum object size (command line parameter -m) can be
   obtained from &MAXLNGTH.

During garbage collection, each recursion of the garbage collector to
   follow a pointer will use (2 * HOST(-1,6)) bytes of stack space.  This
   has ramifications for users creating data structures with long chains
   of linked nodes.


## Changes in 1.30.7:

1. Fixed problem with Execute (HOST(1)) that allowed the sub-process to 
   run in parallel with SPITBOL.  Now SPITBOL waits until the sub-process
   completes execution.

2. Fixed problem when more than one pipe was active at once, and the pipes
   terminated in an order different than the user's ENDFILE() function calls.
   The number of pipes that may be active at one time is limited by 
   the operating system (64 under Windows NT).

3. There are substantial problems with a pipe created within 32-bit SPITBOL
   when the other end of the pipe is a 16-bit DOS application.  These
   problems are most pronounced within Windows 95, less so in Windows NT.
   Pipes between SPITBOL and other 32-bit applications work properly in both
   environments.  The problems with 16-bit applications appear to be within
   the operating system.

   Consequences:  Consider the simple input pipe designed to read from the
   DIR (directory) command:

```
        INPUT(.in, 1, "!!dir!")
```

Under Windows NT, the dir command is executed by the command processor,
   cmd.exe.  cmd.exe is a 32-bit application, and the pipe works properly.

Under Windows 95, the command processor is command.com, a 16-bit DOS
   application.  The pipe hangs, and the system must be rebooted.

A search of Usenet postings revealed that this is a known problem with
   Windows 95.  It appears to be related to the fact that both SPITBOL and
   the 16-bit application are sharing the same console.  Creating a new
   console window for the DOS application helps, but other problems remain. 
   We had considered simply eliminating pipes when running under Windows 95,
   but that removes the possibility of connecting a pipe to a 32-bit
   application.  The HOST() function (no arguments) can be used to determine
   which operating system the program is executing under.  Do not pipe to or
   from a 16-bit application or the command processor if operating under
   Windows 95!

4. The HOST() function further identifies the operating system environment
   that the program is running in.  Three results are possible:

   "80386:MS-DOS(Borland Extended):Macro SPITBOL 3.7(ver 1.30.10) #28001"
   "80386:Win95:Macro SPITBOL 3.7(ver 1.30.10) #28001"
   "80386:WinNT:Macro SPITBOL 3.7(ver 1.30.10) #28001"


## Changes in 1.30.6:

1. Fixed uninitialized variable that caused system crash when transferring
   to END label after loading an external function.

2. Added workarounds for bugs in the Console Win32 API when operating under
   Windows 95:

*   a) clearing screen by scrolling lines offscreen replaced by writing blanks
   to screen.

*   b) no longer rely on operating system remembering cursor height when
   cursor is turned off.

*   c) fix keyboard input (HOST(8)) with CapsLock on.  Windows 95 is shifting
   all characters, not just characters a-z as it should.

3. No longer clear screen when switching between conventional
   screen/keyboard I/O (TERMINAL, INPUT, OUTPUT) and HOST functions.  The
   user should call the ClearScn (HOST(10)) function to explicitly clear the
   screen as desired.
 

## Changes in 1.30.5:

1. The cursor was not made visible by the Ready (HOST(7)) function.

2. The Cursor (HOST(5)) function now returns the cursor type set
   from the previous HOST(5) call.

3. Support for mouse input is provided:

*   a) The new function MOUSE(I) (HOST(214,I)) can be used to enable
   or disable mouse input.  Mouse input is disabled by default.  Set
   I to 1 to enable mouse input, to 0 to disable it.

*   b) When mouse input is enabled, the Ready (HOST(7)) function will
   succeed if a keystroke is available OR if a mouse input event is
   available.

*   c) The ReadKey (HOST(8)) and ReadKeySpecial (HOST(30)) functions
   will return a keystroke OR a mouse event, depending on which is
   next in the application's event queue. The returned value will
   be less than 65536 if it is a keystroke.  It will be greater than
   65536 if it is a mouse event.  The mouse event is encoded as a 32
   bit integer as follows:

```
        bits 0-7        column position of mouse cursor
        bits 15-8       row position of mouse cursor
        bits 23-16      button action:  bit 16 - left mouse button pressed
                                        bit 17 - right mouse button pressed
                                        bit 18 - middle mouse button pressed
        bits 26-24      event type:     0 - mouse button pressed or released
                                        1 - change in mouse position
                                        2 - second click of double click
        bit  30         always set
        bit  31         always reset
```

A mouse event can be most easily processed in SPITBOL with a decoding
   function such as the following:

```
   * Obtain information encoded in a mouse event returned by HOST(8)
   *
   * key = host(8)
   * key is 0-65535 for keyboard character, >=65536 for mouse event
   * Event = GE(key,65536)
   * +       MouseEvent(key, .row, .column, .buttons) :s(do_mouse)
   *
   *    key    = value >= 65536 returned by host(8)
   *    row    = row value of mouse cursor
   *    column = column value of mouse cursor
   *    buttons= String of buttons currently pressed:
   *             'L' = left mouse button
   *             'R' = right mouse button
   *             'M' = middle mouse button
   *    Event  = 0 - mouse button change (pressed or released)
   *             1 - mouse moved
   *             2 - second click (button pressed) of a double-click occurred
   *             
           define('MouseEvent(key,prow,pcol,pbutton)b')   :(MouseEvent_end)
   MouseEvent
           $pcol = remdr(key,256)
           $prow = remdr(key,65536) / 256
           b = remdr(key,16777216) / 65536
           $pbutton = (GE(b,4) 'M','')
           $pbutton = GE(remdr(b,4),2) 'R' $pbutton
           $pbutton = EQ(remdr(b,2),1) 'L' $pbutton
           MouseEvent = remdr(key,1073741824) / 16777216 :(return)
   MouseEvent_end
```
   
*  d) It is often necessary to know the state of some keyboard keys
  such as SHIFT, CONTROL, and ALT during a mouse event.  The new
  KeyState (HOST(36)) function returns the state of these keys at
  the time of the most recently reported keystroke or mouse event.
  The integer value returned by HOST(36) is encoded as follows:

```
       SHIFT           01h
       CONTROL         04h
       ALT             08h
       SCROLL LOCK     10h
       NUM LOCK        20h
       CAPS LOCK       40h
```

*  e) The sample program MOUSE.SPT demonstrates use of these
  functions.  Type 'x' to exit.

*  f) Known Bug:  When operating under MS-DOS, or under Windows
  3.1 in full screen mode (using the Borland DOS Extender), the
  following problem is present.  This problem appears to be in
  Borland DOS Extender, and cannot be circumvented.

In full screen mode, the mouse pointer is shown by using reverse
  video attributes, giving the appearance of a full block cursor.
  Unfortunately, Borland appears to save and restore the character
  at the mouse position as well as the video attributes.

If your program writes a character to the screen AT THE MOUSE
POSITION, Borland will overwrite it with its saved character when
  the mouse moves to another position.  The character your program
  wrote "under" the mouse cursor is lost.

  One work around would be for your program to issue an UpdateLine
  (HOST(20,row)) for the row containing the PREVIOUS mouse cursor
  whenever the mouse moves to a different location.  This is shown
  at label CHECK in the sample program MOUSE.SPT.


## Changes in 1.30.4:

1. 
* a) The ReadKey (HOST(8)) function returns proper codes.

* b) The cursor is restored when SPITBOL exits.

* c) HOST(16) and HOST(17) no longer fail when supplied with
      null arguments.

2. The Cursor (HOST(5,I)) function now accepts an additional
   argument to specify the visibility of the cursor:

HOST(5,I) or HOST(5,I,0) sets the cursor type to I, and
     turns the cursor on when input is requested of the user,
     and off when input has completed.

HOST(5,I,1) leaves the cursor on at all times.

3. The new ReadAtrString (HOST(31,N)) function returns an N
   byte string of screen attributes beginning at the current
   cursor position.  The attributes are read directly from the
   video buffer.  The cursor position remains unchanged.

4. The new WriteAtrString (HOST(32,S) function writes attribute
   string S directly to the video buffer at the current cursor
   position.  The cursor position remains unchanged.

5. The new CopyLine (HOST(33,LINE)) function copies the characters
   of the specified line from the video buffer to the internal
   screen buffer maintained by SPITBOL.  The characters can be
   retrieved from the screen buffer with ReadStr (HOST(14,N)).

6. The new CopyScn (HOST(34)) function copies the entire video
   buffer to the internal screen buffer maintained by SPITBOL.

7. The new ScrollAll (HOST(35,L1*256+C1,L2*256+C2,N,ATR)) function
   scrolls a rectangular region of the video buffer up or down.
   N is positive to scroll up N lines, negative to scroll down
   -N lines.  The upper left corner of the scroll region is
   at line L1, column C1.  The lower right corner is given by
   line L2, column C2 (inclusive).  N is zero to blank the entire
   region.  ATR is the character attribute assigned to the blank
   lines created by the scroll operation.  L2 must be > L1, and
   C2 must be > C1.

Note that this operation effects the video buffer only, not
   SPITBOL's internal screen buffer.  The scrolling operation
   affects both characters and color attributes within the
   scroll region.  The user should follow this function with a
   call of CopyScn (HOST(34)) to bring the internal screen buffer
   back into synch with the video display.


## Changes in 1.30.3:

1. Conversion of a real to an integer value could cause an application
exception or an incorrect result if the real was larger than the maximum
integer value allowed in SPITBOL.  This case is now properly detected,
and the conversion fails.

```
        a = convert(2147483647.0, "integer")    ;* succeeds
        a = convert(2147483652.0, "integer")    ;* fails
```


## Changes in 1.30.2:

1. The  -o=file[.lst] command line option was ignored.  That is,
under DOS this would give a "file.lst" listing file and a "dump"
trace file, but under NT both the listing and the trace output
go to "dump" and "file.lst" was not created:

```
  spitbol -a -o=file.lst program.spt > dump
```

Note:  because "-" in place of a file name on the command line
represents either the standard input or output file, it is possible
to have the program listing, dump, and all program output written
to a single file:

```
  spitbol -a -o=- program.spt >program.lst
```

2. When starting SPITBOL at the same time with many other programs,
an out of memory condition would sometimes be generated, even though
enough virtual memory was present.




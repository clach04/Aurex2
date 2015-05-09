# Aurex2 - Seiko Ruputer / OnHand PC Emulator

Written by Fumihiko MACHIDA

NOTE whilst this is an English readme:
  * *some* of the content of this text is machine translated from Google!
  * some content came from the original (now defunct) website http://megadriver.yi.org/~fumi/aurex_en

License is LGPLv2 (per source code and https://web.archive.org/web/20071111195400/http://megadriver.yi.org/~fumi/aurex/snapshot/COPYING)

For more information about Ruputer see http://en.wikipedia.org/wiki/Ruputer

Aurex uses host directory for emulating Ruputer/onHand flash memory. See option "virtual root directory". 

## Build Requirements

  * wigdet library (one of following)
      * w32api of mingw
      * (n)curses
      * qte
      * x11 
  * gcc
  * gmake
  * other unix tool (sh, sed, etc) 


## Windows Build Instructions

NOTE tested under Windows 7 with:
  * gcc version 4.8.3 (from http://StrawberryPerl.com/)
  * Bash (from Git for Windows) - building in a regular CMD window
    fails, tools like "mkdir" are expected.

Issue:

    cd windows
    gmake depend
    gmake
    gmake  # second time!
    gmake  # third time! :-(

Build has undergone some clean up but there is still some work to
be done in that area.

## Usage

Run emulator and load an file (e.g. an EXF), under Windows right mouse click
and select "Load".

For example try "Temp Convert" from http://www.pconhand.com/software.asp


### Keys

Enter : w
EL : s
Menu : x
Filer : z
lower pointer : up/down/left/right arrow

### Config file - aurex.ini

Sample aurex.ini which should be located in the same directory as the emulator binary:

    # This file is a alternative file for FILER.INI
    #
    # format:
    # "*.extend" "executable program path(virtual path)"
    #
    # Follow lines are examples.
    #
    *.txt \SYSTEM\APPS\txtVIEW.EXF
    *.tbl \SYSTEM\APPS\TBLVIEW.EXF
    *.scl \SYSTEM\APPS\SCLVIEW.EXF
    *.adr \SYSTEM\APPS\ADRVIEW.EXF
    *.dfm \SYSTEM\APPS\DFMVIEW.EXF
    *.txm \SYSTEM\APPS\TXMVIEW.EXF
    *.tdo \SYSTEM\APPS\TDOVIEW.EXF
    *.mmp \SYSTEM\APPS\mmpVIEW.EXF
    *.snd \SYSTEM\APPS\SNDVIEW.EXF


In this example, Aurex open "hoge.txt", so aurex execute \SYSTEM\APPS\TXTVIEW.EXF . The first column in aurex.ini is not a distinction from case. On the other hand, the second column is a distinction case (in ext2). 

### Virtual root directory

If virtual root directory is define "c:\foo" in option.

  * When A emulation program open "b:\system\ini\foo.ini".
    Aurex add "Virtual root directory" to it. aurex open "c:\foo\system\ini\foo.ini" in host disk.

  * When execute "c:\foo\bar.txt"
    Aurex changes c:\foo\bar.txt to "b:\foo.txt" and writes "b:\foo.txt" in PSP. 

### Options

Option	description
Warn odd access (for developper)	A MN102XX of Ruputer's CPU fail to move multi-byte from odd address (effective address. At that time, no interrupt is occured, therefore out application cannot notice that failure. This option enable warning message (for developper).
Enable sound	Enable sound. But, a little noise
Enable CPU Timer	Enable CPU Timer. When enabling, count 1/10, 1/100 second in Chronograph.
Disable plural processes	When a enabling, we don't execute plural Aurex.
Show status bar	Showing status bar and print fps. a fps "graphics FPS"/"emulate FPS"
Hide title bard	hide title bar
Use ISO8859-1	Aurex uses iso-8859-1 character sets instead of jisx0201.

#### LCD

Option	description
LCD size 102x64	Window size is 102x64, enabling gv_place
Flip	flip window (deg 90)
Enable a like lcd showing (enable afterimage)	emulate afterimage effect.
afterimage value	set afterimage effective value.
Enable anti aliases	drawing functions (font and gv_line ...) enable antialiases.
Lcd expansion	Window magnification.
Enable transparency	
Enable translucency	
Translucent value	

#### Speed

Option	description
CPU speed (%)	Specify CPU speed.
Timer Speed	Specify Timer interrupt speed. 1/64 faster to the cause interrupted a timer interrupt (negative value), and late (positive value).

#### Directory

Option	description
Virtual root directory	Specify virtual root directory

#### Log

Option	description
no output	no log output
stderr	log output stderr. you'll get log by executing aurex in cygwin bash.
file	log output to file
log level	specify log level. lesser value is more detail.  -1: Debug, 0: Information 1: Warning 2: It is an error

### Restrictions

2006/01/21 19:30 updated

  * A function gv_circle cannot draw an ellipse.
  * Drawing text in curses need to BDF font.
  * A sound implementation is supported only Windows.
  * A onHand feature (no Ruputer) has restrictions.
      * apisys
        Now Aurex support getFilerVersion
      * apimenu
        minimum implement
      * apitool
        No support
      * code set iso8859-1 
        No support (only ASCII)

2007/07/31 15:47 updated

####  Ruputer / onHand common development situation

-  CPU-related
    -  Instruction 100%
    -  CPU TIMER 50% interrupt address when the CPU timer has underflow is unknown (BugTrack / 7). 

-  Screen around
    -  Most of GV_ instruction support (lc instruction map to GV). (Circle is still BugTrack / 5)
    -  drawing support of direct transfer to the lcd port
    -  Character related functions Windows, X11, QT / E, supported by curses. However, curses will BDF need. 
    -  Screen 102x64 mode depends on the widget
    -  blink, reverse support

-  File input-output
    -  Possible. However, there is a condition.
    -  Directory search possible findfirst, findnext system (Windows), opendir (Unix system) support. 

-  rupsys function system
    -  Keyin, event ??? implementation
    -  aprMenu system implementation
    -  TIMERStart implementation
    -  dispXXX system

-  wbios
    -  key system Implemented
    -  tmset, tmdel implementation
    -  dtread, tmread implementation
    -  Sound-related Windows only (BugTrack / 8 for Unix-based OS support) 
    -  Others I returns its appropriate value.

-  Timer-related
    -  1Hz timer implementation (event timer to exit the Keyin)
    -  already timer implementation in bios
    -  There is an update of the RTC

-  Options
    -  I will output as at the end option.txt. (Change schedule file name)

####  onHand specific developments

-  apisys
    -  getFilerVersion? only (this alone, quite onHand app moves)

-  apimenu
    -  20% (a minimum of implementation)

-  apitool
    -  50% (str *** is already implemented)

-  Support of character set iso8859-1
    -  Support to you. (See options)</textarea>


 Copyright (C) 1990 Texas Instruments Incorporated.

 Permission is granted to any individual or institution to use, copy, modify,
 and distribute this software, provided that this complete copyright and
 permission notice is maintained, intact, in all copies and supporting
 documentation.

 Texas Instruments Incorporated provides this software "as is" without express
 or implied warranty.

 InterViews library and demo programs were built using Glockenspiel C++
 2.0 Ver C and MicroSoft C compiler Ver 6.0 on OS/2 Ver 1.2. InterViews
 directory was created on a HPFS partition to keep the original filenames
 length, some of which are longer than the 8.3 FAT filename format.

 For CCXX (Glockenspiels driver program) to accept these longer filenames,
 CCXX.EXE, GCPP.EXE, CFXX.EXE, and MXX.EXE need to be modified by typing the
 following:

	 EXEHDR /NEW CCXX.EXE
	 EXEHDR /NEW GCPP.EXE
	 EXEHDR /NEW CFXX.EXE
	 EXEHDR /NEW MXX.EXE

 These executables usually reside in the \CXX20\BINP directory.

 To compile InterViews files on OS/2 you need to do the following:

 1) The InterViews directory structure should be as follows:

                                    iv
                                     |
                      -----------------------------------------
                      |                                       |
                     src                                    demo
                      |                                       |
               ------------------              ------------------------
               |                |              |       |      |       |     
          InterViews      libInterViews      hello  iclock  logo   squares
               |                |                
               |            Generated
       -----------------             
       |       |       |  
    Bitmaps   Std     pm

 2) Filename extension has to be changed from .C to .CXX (Glockenspiels C++
    driver needs this to distintguish between a C and C++ file). Type the
    following command in "libInterViews" and "Generated" dirs (this needs to
    be done once only). 

         RENAME  *.C  *.CXX 

 3) There are two Makefiles in the "libInterViews" directory, "Makefile" (unix)
    and "Makefile.pm" (OS/2). On OS/2, rename "Makefile.pm" to "Makefile".

 4) Directories in which CCXX.EXE and CL.EXE reside, need to be in your
    PATH environment.

 5) Type NMAKE (MSC make utility) in the "libInterViews" directory to build
    the InterViews library (intervw.lib).

 6) For hello demo program, type NMAKE in the "hello" directory. Check IVLIB
    variable in the Makefile to make sure it points to the correct drive and
    directory containing intervw.lib.
 
 Transfering InterViews files to FAT partition will cause the filenames to be
 truncated to the 8.3 format. CAUTION: files pm_paint.c and pm_painter.c will
 map to the same filename (pm_paint.c), rename pm_painter.c to pm_pntr.c. The
 filenames, in the Makefile, will have to be manually changed to match the
 truncated filenames.

 This is only a partial InterViews port, this is NOT a complete InterViews
 port to PM.
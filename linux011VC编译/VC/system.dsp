# Microsoft Developer Studio Project File - Name="system" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=system - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "system.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "system.mak" CFG="system - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "system - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# SUBTRACT MTL /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=bin\ld -T .\bin\ld_script.txt -s -x --file-alignment=1 --section-alignment=1 -Ttext=0 --image-base=0 -nostdlib -Map=.\release\system.map -o .\release\system   ".\Release\boot\head.o" ".\Release\init\main.o"  ".\Release\kernel\exit.o"  ".\Release\kernel\fork.o"  ".\Release\kernel\math\math_emulate.o"  ".\Release\kernel\mktime.o"  ".\Release\kernel\panic.o"  ".\Release\kernel\printk.o" ".\Release\kernel\sched.o"   ".\Release\kernel\signal.o" ".\Release\kernel\sys.o"  ".\Release\kernel\system_call.o"  ".\Release\kernel\traps.o"  ".\Release\kernel\vsprintf.o" ".\Release\kernel\asm.o"   ".\Release\kernel\blk_drv\floppy.o" ".\Release\kernel\blk_drv\hd.o"   ".\Release\kernel\blk_drv\ll_rw_blk.o" ".\Release\kernel\blk_drv\ramdisk.o"   ".\Release\kernel\chr_drv\console.o" ".\Release\kernel\chr_drv\keyboard.o"   ".\Release\kernel\chr_drv\rs_io.o" ".\Release\kernel\chr_drv\serial.o"   ".\Release\kernel\chr_drv\tty_io.o" ".\Release\kernel\chr_drv\tty_ioctl.o"   ".\Release\mm\memory.o" ".\Release\mm\page.o" ".\Release\fs\bitmap.o"   ".\Release\fs\block_dev.o" ".\Release\fs\buffer.o" ".\Release\fs\char_dev.o"   ".\Release\fs\exec.o"  ".\Release\fs\fcntl.o"\
       ".\Release\fs\file_dev.o"  ".\Release\fs\file_table.o"   ".\Release\fs\inode.o" ".\Release\fs\ioctl.o"   ".\Release\fs\namei.o" ".\Release\fs\open.o" ".\Release\fs\pipe.o"   ".\Release\fs\read_write.o" ".\Release\fs\stat.o" ".\Release\fs\super.o"  ".\Release\fs\truncate.o" ".\Release\lib\_exit.o" ".\Release\lib\close.o"  ".\Release\lib\ctype.o" ".\Release\lib\dup.o" ".\Release\lib\errno.o"  ".\Release\lib\execve.o" ".\Release\lib\malloc.o" ".\Release\lib\open.o"  ".\Release\lib\setsid.o" ".\Release\lib\string.o" ".\Release\lib\wait.o"  ".\Release\lib\write.o"
# End Special Build Tool
# Begin Target

# Name "system - Win32 Release"
# Begin Group "boot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\boot\head.s
# PROP Intermediate_Dir "Release\boot"
# Begin Custom Build
IntDir=.\Release\boot
InputPath=..\boot\head.s
InputName=head

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bin\ml.exe -c -coff -nologo -Cp /X /Fo "$(IntDir)\$(InputName).o" $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "init"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\init\main.c
# PROP Intermediate_Dir "Release\init"
# Begin Custom Build
IntDir=.\Release\init
InputPath=..\init\main.c
InputName=main

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "asm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\asm\io.h
# End Source File
# Begin Source File

SOURCE=..\include\asm\memory.h
# End Source File
# Begin Source File

SOURCE=..\include\asm\segment.h
# End Source File
# Begin Source File

SOURCE=..\include\asm\system.h
# End Source File
# End Group
# Begin Group "linux"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\linux\config.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\fdreg.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\fs.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\hdreg.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\head.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\kernel.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\mm.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\sched.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\sys.h
# End Source File
# Begin Source File

SOURCE=..\include\linux\tty.h
# End Source File
# End Group
# Begin Group "sys"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\sys\stat.h
# End Source File
# Begin Source File

SOURCE=..\include\sys\times.h
# End Source File
# Begin Source File

SOURCE=..\include\sys\types.h
# End Source File
# Begin Source File

SOURCE=..\include\sys\utsname.h
# End Source File
# Begin Source File

SOURCE=..\include\sys\wait.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\a.out.h
# End Source File
# Begin Source File

SOURCE=..\include\const.h
# End Source File
# Begin Source File

SOURCE=..\include\ctype.h
# End Source File
# Begin Source File

SOURCE=..\include\errno.h
# End Source File
# Begin Source File

SOURCE=..\include\fcntl.h
# End Source File
# Begin Source File

SOURCE=..\include\set_seg.h
# End Source File
# Begin Source File

SOURCE=..\include\signal.h
# End Source File
# Begin Source File

SOURCE=..\include\stdarg.h
# End Source File
# Begin Source File

SOURCE=..\include\stddef.h
# End Source File
# Begin Source File

SOURCE=..\include\string.h
# End Source File
# Begin Source File

SOURCE=..\include\termios.h
# End Source File
# Begin Source File

SOURCE=..\include\time.h
# End Source File
# Begin Source File

SOURCE=..\include\unistd.h
# End Source File
# Begin Source File

SOURCE=..\include\utime.h
# End Source File
# End Group
# Begin Group "kernel"

# PROP Default_Filter ""
# Begin Group "math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\kernel\math\math_emulate.c
# PROP Intermediate_Dir "Release\kernel\math"
# Begin Custom Build
IntDir=.\Release\kernel\math
InputPath=..\kernel\math\math_emulate.c
InputName=math_emulate

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "blk_drv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\kernel\blk_drv\blk.h
# PROP Intermediate_Dir "Release\kernel\blk_drv"
# End Source File
# Begin Source File

SOURCE=..\kernel\blk_drv\floppy.c
# PROP Intermediate_Dir "Release\kernel\blk_drv"
# Begin Custom Build
IntDir=.\Release\kernel\blk_drv
InputPath=..\kernel\blk_drv\floppy.c
InputName=floppy

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\blk_drv\hd.c
# PROP Intermediate_Dir "Release\kernel\blk_drv"
# Begin Custom Build
IntDir=.\Release\kernel\blk_drv
InputPath=..\kernel\blk_drv\hd.c
InputName=hd

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\blk_drv\ll_rw_blk.c
# PROP Intermediate_Dir "Release\kernel\blk_drv"
# Begin Custom Build
IntDir=.\Release\kernel\blk_drv
InputPath=..\kernel\blk_drv\ll_rw_blk.c
InputName=ll_rw_blk

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\blk_drv\ramdisk.c
# PROP Intermediate_Dir "Release\kernel\blk_drv"
# Begin Custom Build
IntDir=.\Release\kernel\blk_drv
InputPath=..\kernel\blk_drv\ramdisk.c
InputName=ramdisk

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "chr_drv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\kernel\chr_drv\console.c
# PROP Intermediate_Dir "Release\kernel\chr_drv"
# Begin Custom Build
IntDir=.\Release\kernel\chr_drv
InputPath=..\kernel\chr_drv\console.c
InputName=console

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\chr_drv\keyboard.S
# PROP Intermediate_Dir "Release\kernel\chr_drv"
# Begin Custom Build
IntDir=.\Release\kernel\chr_drv
ProjDir=.
InputPath=..\kernel\chr_drv\keyboard.S
InputName=keyboard

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /X /I "..\include" /EP /P $(InputPath) 
	bin\ml.exe -c -coff -nologo -Cp /X /Fo "$(IntDir)\$(InputName).o" $(ProjDir)\$(InputName).i 
	
# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\chr_drv\rs_io.s
# PROP Intermediate_Dir "Release\kernel\chr_drv"
# Begin Custom Build
IntDir=.\Release\kernel\chr_drv
InputPath=..\kernel\chr_drv\rs_io.s
InputName=rs_io

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bin\ml.exe -c -coff -nologo -Cp /X /Fo "$(IntDir)\$(InputName).o" $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\chr_drv\serial.c
# PROP Intermediate_Dir "Release\kernel\chr_drv"
# Begin Custom Build
IntDir=.\Release\kernel\chr_drv
InputPath=..\kernel\chr_drv\serial.c
InputName=serial

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\chr_drv\tty_io.c
# PROP Intermediate_Dir "Release\kernel\chr_drv"
# Begin Custom Build
IntDir=.\Release\kernel\chr_drv
InputPath=..\kernel\chr_drv\tty_io.c
InputName=tty_io

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\chr_drv\tty_ioctl.c
# PROP Intermediate_Dir "Release\kernel\chr_drv"
# Begin Custom Build
IntDir=.\Release\kernel\chr_drv
InputPath=..\kernel\chr_drv\tty_ioctl.c
InputName=tty_ioctl

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Source File

SOURCE=..\kernel\asm.s
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\asm.s
InputName=asm

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bin\ml.exe -c -coff -nologo -Cp /X /Fo "$(IntDir)\$(InputName).o" $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\exit.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\exit.c
InputName=exit

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\fork.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\fork.c
InputName=fork

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\mktime.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\mktime.c
InputName=mktime

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\panic.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\panic.c
InputName=panic

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\printk.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\printk.c
InputName=printk

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\sched.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\sched.c
InputName=sched

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\signal.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\signal.c
InputName=signal

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\sys.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\sys.c
InputName=sys

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\system_call.s
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\system_call.s
InputName=system_call

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bin\ml.exe -c -coff -nologo -Cp /X /Fo "$(IntDir)\$(InputName).o" $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\traps.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\traps.c
InputName=traps

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\kernel\vsprintf.c
# PROP Intermediate_Dir "Release\kernel"
# Begin Custom Build
IntDir=.\Release\kernel
InputPath=..\kernel\vsprintf.c
InputName=vsprintf

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "mm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\mm\memory.c
# PROP Intermediate_Dir "Release\mm"
# Begin Custom Build
IntDir=.\Release\mm
InputPath=..\mm\memory.c
InputName=memory

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\mm\page.s
# PROP Intermediate_Dir "Release\mm"
# Begin Custom Build
IntDir=.\Release\mm
InputPath=..\mm\page.s
InputName=page

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bin\ml.exe -c -coff -nologo -Cp /X /Fo "$(IntDir)\$(InputName).o" $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "fs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\fs\bitmap.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\bitmap.c
InputName=bitmap

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\block_dev.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\block_dev.c
InputName=block_dev

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\buffer.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\buffer.c
InputName=buffer

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\char_dev.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\char_dev.c
InputName=char_dev

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\exec.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\exec.c
InputName=exec

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\fcntl.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\fcntl.c
InputName=fcntl

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\file_dev.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\file_dev.c
InputName=file_dev

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\file_table.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\file_table.c
InputName=file_table

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\inode.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\inode.c
InputName=inode

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\ioctl.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\ioctl.c
InputName=ioctl

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\namei.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\namei.c
InputName=namei

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\open.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\open.c
InputName=open

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\pipe.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\pipe.c
InputName=pipe

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\read_write.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\read_write.c
InputName=read_write

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\stat.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\stat.c
InputName=stat

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\super.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\super.c
InputName=super

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\fs\truncate.c
# PROP Intermediate_Dir "Release\fs"
# Begin Custom Build
IntDir=.\Release\fs
InputPath=..\fs\truncate.c
InputName=truncate

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lib\_exit.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\_exit.c
InputName=_exit

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\close.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\close.c
InputName=close

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\ctype.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\ctype.c
InputName=ctype

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\dup.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\dup.c
InputName=dup

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\errno.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\errno.c
InputName=errno

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\execve.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\execve.c
InputName=execve

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\malloc.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\malloc.c
InputName=malloc

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\open.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\open.c
InputName=open

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\setsid.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\setsid.c
InputName=setsid

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\string.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\string.c
InputName=string

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\wait.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\wait.c
InputName=wait

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\lib\write.c
# PROP Intermediate_Dir "Release\lib"
# Begin Custom Build
IntDir=.\Release\lib
InputPath=..\lib\write.c
InputName=write

"$(IntDir)\$(InputName).o" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo  /W3 /vd0 /GX- /Ob1 /Ot /Oa /Og /Op /X /I "..\include" /u /FAs /Fa"$(IntDir)/" /Fp"Release/system.pch" /YX /Fo"$(IntDir)/$(InputName).o" /Fd"$(IntDir)/" /FD /c $(InputPath)

# End Custom Build
# End Source File
# End Group
# End Target
# End Project

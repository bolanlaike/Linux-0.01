# Microsoft Developer Studio Project File - Name="setup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=setup - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "setup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "setup.mak" CFG="setup - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "setup - Win32 Release" (based on "Win32 (x86) Generic Project")
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
# PROP Intermediate_Dir "Release\boot"
# PROP Target_Dir ""
# SUBTRACT MTL /nologo
# Begin Special Build Tool
IntDir=.\Release\boot
OutDir=.\Release
SOURCE="$(InputPath)"
PostBuild_Cmds=link.exe /VXD /NOLOGO /NOENTRY /NODEFAULTLIB /OUT:$(OutDir)\setup $(IntDir)\setup.obj 
# End Special Build Tool
# Begin Target

# Name "setup - Win32 Release"
# Begin Source File

SOURCE=..\boot\setup.s
# Begin Custom Build
IntDir=.\Release\boot
WkspDir=.
InputPath=..\boot\setup.s
InputName=setup

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bin\ml.exe /Cp /c /nologo /X /coff /Fo "$(IntDir)\$(InputName)" $(InputPath)

# End Custom Build
# End Source File
# End Target
# End Project

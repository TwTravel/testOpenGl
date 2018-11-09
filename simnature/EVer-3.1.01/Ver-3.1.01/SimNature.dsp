# Microsoft Developer Studio Project File - Name="SimNature" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SimNature - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimNature.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimNature.mak" CFG="SimNature - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimNature - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SimNature - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SimNature - Win32 Static Link Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimNature - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SimNature - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/SimNatureD.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SimNature - Win32 Static Link Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SimNature___Win32_Static_Link_Release"
# PROP BASE Intermediate_Dir "SimNature___Win32_Static_Link_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StaticRelease/"
# PROP Intermediate_Dir "StaticRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /Zp4 /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS _STATIC_LIB" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imagedec.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SimNature - Win32 Release"
# Name "SimNature - Win32 Debug"
# Name "SimNature - Win32 Static Link Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BillBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\BillBoardMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Configure.cpp
# End Source File
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# Begin Source File

SOURCE=.\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\frustum.cpp
# End Source File
# Begin Source File

SOURCE=.\GamaControl.cpp
# End Source File
# Begin Source File

SOURCE=.\GameApp.cpp
# End Source File
# Begin Source File

SOURCE=.\GLMain.cpp
# End Source File
# Begin Source File

SOURCE=.\InitGLext.cpp
# End Source File
# Begin Source File

SOURCE=.\InitSence.cpp
# End Source File
# Begin Source File

SOURCE=.\Input.cpp
# End Source File
# Begin Source File

SOURCE=.\LensFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\LOD.cpp
# End Source File
# Begin Source File

SOURCE=.\MMath.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionBlur.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGLCom.cpp
# End Source File
# Begin Source File

SOURCE=.\PageArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# Begin Source File

SOURCE=.\SkyBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewerCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewFrustum.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BillBoard.h
# End Source File
# Begin Source File

SOURCE=.\BillBoardMgr.h
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\CharArray.h
# End Source File
# Begin Source File

SOURCE=.\CommGLHeader.h
# End Source File
# Begin Source File

SOURCE=.\Configure.h
# End Source File
# Begin Source File

SOURCE=.\Console.h
# End Source File
# Begin Source File

SOURCE=.\Data.h
# End Source File
# Begin Source File

SOURCE=.\Effect.h
# End Source File
# Begin Source File

SOURCE=.\FreeCamera.h
# End Source File
# Begin Source File

SOURCE=.\frustum.h
# End Source File
# Begin Source File

SOURCE=.\GamaControl.h
# End Source File
# Begin Source File

SOURCE=.\GameApp.h
# End Source File
# Begin Source File

SOURCE=.\glext.h
# End Source File
# Begin Source File

SOURCE=.\glparam.h
# End Source File
# Begin Source File

SOURCE="..\..\..\我的程序\MySDK\include\gltexture.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\我的程序\MySDK\include\Image.h"
# End Source File
# Begin Source File

SOURCE=.\InitSence.h
# End Source File
# Begin Source File

SOURCE=.\Input.h
# End Source File
# Begin Source File

SOURCE=.\LensFare.h
# End Source File
# Begin Source File

SOURCE=.\LOD.h
# End Source File
# Begin Source File

SOURCE=.\MMath.h
# End Source File
# Begin Source File

SOURCE=.\MotionBlur.h
# End Source File
# Begin Source File

SOURCE=.\MotionCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OpenGLCom.h
# End Source File
# Begin Source File

SOURCE=.\PageArray.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SkyBox.h
# End Source File
# Begin Source File

SOURCE=.\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\TextureMgr.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\ViewerCamera.h
# End Source File
# Begin Source File

SOURCE=.\ViewFrustum.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\data\map\map1024.ini
# End Source File
# Begin Source File

SOURCE=".\data\map\Map2048-4.ini"
# End Source File
# Begin Source File

SOURCE=.\data\map\map2048.ini
# End Source File
# Begin Source File

SOURCE=.\data\map\map4096.ini
# End Source File
# Begin Source File

SOURCE=.\data\map\map512.ini
# End Source File
# Begin Source File

SOURCE=.\data\map\Map8192.ini
# End Source File
# Begin Source File

SOURCE=.\simnature.ini
# End Source File
# Begin Source File

SOURCE=.\data\map\SPEMAP.INI
# End Source File
# End Group
# End Target
# End Project

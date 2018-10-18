; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CControlPane
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "opengl.h"
LastPage=0

ClassCount=5
Class1=COpenGLApp
Class2=CAboutDlg
Class3=COpenGLDlg
Class4=COpenGLWnd

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDD_OPENGL_DIALOG
Resource3=IDD_GLWnd
Class5=CControlPane
Resource4=IDD_Control

[CLS:COpenGLApp]
Type=0
BaseClass=CWinApp
HeaderFile=OpenGL.h
ImplementationFile=OpenGL.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=OpenGLDlg.cpp
ImplementationFile=OpenGLDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CAboutDlg

[CLS:COpenGLDlg]
Type=0
BaseClass=CDialog
HeaderFile=OpenGLDlg.h
ImplementationFile=OpenGLDlg.cpp
LastObject=COpenGLDlg
Filter=D
VirtualFilter=dWC

[CLS:COpenGLWnd]
Type=0
BaseClass=CDialog
HeaderFile=OpenGLWnd.h
ImplementationFile=OpenGLWnd.cpp
LastObject=COpenGLWnd
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_OPENGL_DIALOG]
Type=1
Class=COpenGLDlg
ControlCount=0

[DLG:IDD_GLWnd]
Type=1
Class=COpenGLWnd
ControlCount=0

[DLG:IDD_Control]
Type=1
Class=CControlPane
ControlCount=13
Control1=IDC_ControlItem,listbox,1344340225
Control2=IDC_STATIC,button,1342177287
Control3=IDC_Status,edit,1342244868
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_Slider,msctls_trackbar32,1342242834
Control7=IDC_Max,static,1342308352
Control8=IDC_Min,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_Current,edit,1342244992

[CLS:CControlPane]
Type=0
HeaderFile=ControlPane.h
ImplementationFile=ControlPane.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_Current


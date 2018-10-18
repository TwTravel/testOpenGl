; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CETreeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "eTree.h"

ClassCount=4
Class1=CETreeApp
Class2=CETreeDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ETREE_DIALOG

[CLS:CETreeApp]
Type=0
HeaderFile=eTree.h
ImplementationFile=eTree.cpp
Filter=N

[CLS:CETreeDlg]
Type=0
HeaderFile=eTreeDlg.h
ImplementationFile=eTreeDlg.cpp
Filter=D

[CLS:CAboutDlg]
Type=0
HeaderFile=eTreeDlg.h
ImplementationFile=eTreeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_ETREE_DIALOG]
Type=1
Class=CETreeDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352


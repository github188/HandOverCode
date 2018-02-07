// CBKVideoDeCode.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CBKVideoDeCode.h"
#include "CBKVideoDeCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoDeCodeApp

BEGIN_MESSAGE_MAP(CCBKVideoDeCodeApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKVideoDeCodeApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoDeCodeApp construction

CCBKVideoDeCodeApp::CCBKVideoDeCodeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKVideoDeCodeApp object

CCBKVideoDeCodeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoDeCodeApp initialization

BOOL CCBKVideoDeCodeApp::InitInstance()
{
	AfxEnableControlContainer();
	HANDLE hObject = CreateMutex(NULL,FALSE,"CCBKVideoDeCodeApp");	
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hObject);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	CString strPath,Filepath;
	TCHAR exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strPath=exeFullPath;
	strPath=strPath.Left(strPath.ReverseFind('\\'));
	Filepath.Format("%s\\log",strPath);
	//	AfxMessageBox(Filepath);
	DWORD dwAttr=GetFileAttributes(Filepath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(Filepath,NULL);//LOG文件夹
	}
	COleDateTime TodayTime;
	TodayTime = COleDateTime::GetCurrentTime();
	strPath.Format("%s\\%s",Filepath,TodayTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//今天日期文件夹
	}
	logFileName.Format("%s\\VideoDeCode.log",strPath);
	WriteLog("InitInstance DeCode V1.5.8.5 2015-05-20更新");
	//////////////////////////////////////////////////////////////////////////
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCBKVideoDeCodeDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

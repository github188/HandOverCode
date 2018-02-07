// CBKVideoBMPBack.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CBKVideoBMPBack.h"
#include "CBKVideoBMPBackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoBMPBackApp

BEGIN_MESSAGE_MAP(CCBKVideoBMPBackApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKVideoBMPBackApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoBMPBackApp construction

CCBKVideoBMPBackApp::CCBKVideoBMPBackApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKVideoBMPBackApp object

CCBKVideoBMPBackApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoBMPBackApp initialization

BOOL CCBKVideoBMPBackApp::InitInstance()
{
	AfxEnableControlContainer();
	if (__argc !=2 )
	{
		AfxMessageBox("非法操作,请打开主程序!");
		return FALSE;
	}	
	HANDLE hObject = CreateMutex(NULL,FALSE,"CCBKVideoBMPBackApp");	
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hObject);
		//ExitProcess(0);
		return FALSE;
	}	
	CoInitialize(NULL);	
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
	logFileName.Format("%s\\VideoBMPBack.log",strPath);
	WriteLog("InitInstance VideoBMPBack Ver:1.5.9-2 20171008 1010更新");
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

	CCBKVideoBMPBackDlg dlg;
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

int CCBKVideoBMPBackApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();
	return CWinApp::ExitInstance();
}
void CCBKVideoBMPBackApp::WriteLog(CString sstr)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%H:%M:%S");
	FILE *fp=fopen(logFileName,"a+");
	fprintf(fp,"[%s]:%s theApp#\n",Data,sstr);
	fclose(fp);
}
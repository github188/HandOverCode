// CJMQ.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CJMQ.h"
#include "CJMQDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJMQApp

BEGIN_MESSAGE_MAP(CCJMQApp, CWinApp)
	//{{AFX_MSG_MAP(CCJMQApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJMQApp construction

CCJMQApp::CCJMQApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCJMQApp object

CCJMQApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCJMQApp initialization

BOOL CCJMQApp::InitInstance()
{
	AfxEnableControlContainer();
	//////////////////////////////////////////////////////////////////////////
	HANDLE hObject = CreateMutex(NULL,FALSE,"CCJMQApp");	
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox("开启失败!合码器程序已启动!\n可能最小化在右下角!");
		CloseHandle(hObject);
		return FALSE;
	}
	//GDI+ INIT
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CoInitialize(NULL);//ADO in
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
	try
	{
		CCJMQDlg dlg;
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
	}
	catch (...)
	{
		CTime NowTime=CTime::GetCurrentTime();
		CString strTime;
		strTime.Format("[%s]:CCJMQDlg异常",NowTime.Format("%y-%m-%d %H:%M:%S"));
		
		FILE *fp=fopen("异常Error.log","a+");
		fprintf(fp,"%s#\n",strTime);
		fclose(fp);	
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCJMQApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	GdiplusShutdown(gdiplusToken);//GDI+ exit
	CoUninitialize();//ADO un
	return CWinApp::ExitInstance();
}

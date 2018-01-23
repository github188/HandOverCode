// CBKDigitalMap.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapApp

BEGIN_MESSAGE_MAP(CCBKDigitalMapApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKDigitalMapApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapApp construction

CCBKDigitalMapApp::CCBKDigitalMapApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKDigitalMapApp object

CCBKDigitalMapApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapApp initialization

BOOL CCBKDigitalMapApp::InitInstance()
{
	AfxEnableControlContainer();
	CoInitialize(NULL);  //数据库 初始过程中需要初始化组件


	if (!OnInitSQL())
	{
		return FALSE;
	}
		
		// Standard initialization
		// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCBKDigitalMapDlg dlg;
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
//////////////////////////////////////////////////////////////////////////
BOOL CCBKDigitalMapApp::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\search.cfg");
	m_strInEdit.ReleaseBuffer();
	if (!m_strInEdit.IsEmpty())
	{
		int iLen =m_strInEdit.GetLength();
		
		for(int i=0;i<iLen;i++)
		{
			m_strInEdit.SetAt(i,m_strInEdit[i]-1);
		}
		m_strInEdit.MakeReverse();
	}
	//////////////////////////////////////////////////////////////////////////
	try
	{
		//建立数据库连接
		
		CString Sqlstr;
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		
		//res = m_pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=abc.mdb","","",adModeUnknown);
		
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
			WriteErr("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			WriteErr("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		ShellExecute(NULL,"open",".\\sqlLink.exe","search.cfg",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}
// BOOL CCBKDigitalMapApp::OnInitSQL()
// {
// 	//////////////////////////////////////////////////////////////////////////
// 	m_pConn.CreateInstance("ADODB.Connection");
// 	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
// 	{
// 		TRACE("初始化连接对象失败\n");
// 		return FALSE;
// 	}
// 	HRESULT res=-1;
// 	try
// 	{
// 		//建立数据库连接
// 		CString Sqlstr;
// 	//	Sqlstr.Format("Provider=SQLOLEDB.1;server=%s;UID=%s;PWD=%s;database=%s","127.0.0.1\\YZH","sa","","2ndDrivingTestSystem");
// 		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
// 		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
// 
// 		//res = m_pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=abc.mdb","","",adModeUnknown);
// 		
// 		if(res==S_OK)//0
// 		{
// 			TRACE("打开数据库成功!");
// 		}
// 		else
// 		{
// 			TRACE("打开数据库失败!");
// 			return FALSE;
// 		}
// 	}
// 	catch(_com_error e)		//捕捉异常
// 	{
// 		CString errormessage;
// 		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
// 		WriteErr(errormessage);
// 
// 		ShellExecute(NULL,"open",".\\Loaddatadll.exe","search.cfg",NULL,SW_SHOW); 
// 
// 		return FALSE;
// 	}
// 	return TRUE;
// 
// }

void CCBKDigitalMapApp::WriteErr(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

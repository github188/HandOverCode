// CBKPlatSDKz.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKPlatSDKz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCBKPlatSDKzApp

BEGIN_MESSAGE_MAP(CCBKPlatSDKzApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKPlatSDKzApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKPlatSDKzApp construction
#define DLL_EXPORTZ extern "C" _declspec(dllexport)

CCBKPlatSDKzApp::CCBKPlatSDKzApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKPlatSDKzApp object

CCBKPlatSDKzApp theApp;

BOOL CCBKPlatSDKzApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
//	AfxMessageBox("InitInstance");
	CString tempPath,strPath;
	tempPath.Format("D:\\Temp");
	DWORD dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	strPath.Format("%s\\%s",tempPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	logfilenamez.Format("%s\\CBKPlatSDKz.log",strPath);
//	WriteLogz("InitInstance() 20170606 0959");
	m_PlatSDKz.Start();
	WriteLogz("InitInstance() 20170705 1654");
	return CWinApp::InitInstance();
}

int CCBKPlatSDKzApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_PlatSDKz.Stop();
	WriteLogz("ExitInstance() 20170604 1754");

	return CWinApp::ExitInstance();
}

void CCBKPlatSDKzApp::WriteLogz(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	CString Data;
	SYSTEMTIME st;
	GetLocalTime(&st);
	Data.Format("%02d:%02d:%02d.%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	FILE *fp=fopen(logfilenamez,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}


DLL_EXPORTZ BOOL PlayVideoPlat(int iwndid,long lWnd)
{
	return theApp.m_PlatSDKz.PlayVideo(iwndid,lWnd);
}

DLL_EXPORTZ void StopVideoPlat(int iwndid)
{
	theApp.m_PlatSDKz.StopPlay(iwndid);
}

DLL_EXPORTZ void SetStreamCamer(LPCTSTR csCameraID,int iWnd)
{
	theApp.m_PlatSDKz.SetStreamCamer(csCameraID,iWnd);
}

DLL_EXPORTZ void SetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser)
{
	theApp.m_PlatSDKz.SetCallBack(cbMessageProc,dwMessageUser);
}

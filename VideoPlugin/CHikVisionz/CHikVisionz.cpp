// CHikVisionz.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CHikVisionz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DLL_EXPORTZ extern "C" _declspec(dllexport)
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
// CCHikVisionzApp

BEGIN_MESSAGE_MAP(CCHikVisionzApp, CWinApp)
	//{{AFX_MSG_MAP(CCHikVisionzApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCHikVisionzApp construction

CCHikVisionzApp::CCHikVisionzApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCHikVisionzApp object

CCHikVisionzApp theApp;

BOOL CCHikVisionzApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempPath,strPath;
// 	TCHAR   exeFullPath[MAX_PATH];
// 	::GetModuleFileName(AfxGetInstanceHandle(),exeFullPath,MAX_PATH);
//	tempPath=exeFullPath;
//	strPath.Format("%s",tempPath.Left(tempPath.ReverseFind('\\')));
// 	AfxMessageBox(strPath);
// 	SetCurrentDirectory(strPath);
//	tempPath.Format("%s\\log",strPath);//当前目录
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
	logfilenamez.Format("%s\\CHikVisionz.log",strPath);
	WriteLogz("InitInstance Build 20170619 1027 V1.0");

	return CWinApp::InitInstance();
}

int CCHikVisionzApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	for (int i=0;i<MAX_WNDZ;i++)
	{
		m_HCNETSDKz[i].fnStopPlay();
	}
	WriteLogz("ExitInstance Build 20170626 1025 V1.0");
	return CWinApp::ExitInstance();
}

void CCHikVisionzApp::WriteLogz(LPCTSTR pstrFormat, ... )
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format("[%02d:%02d:%02d.%03d]",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	
	FILE *fp=fopen(logfilenamez,"a+");
	fprintf(fp,"%s:%s#\n",strTime,logstr);
	fclose(fp);	
}

//初始化
DLL_EXPORTZ void fnInitWnd(HWND hwndz, int wMSG, int nWndID)
{
	try
	{
		theApp.m_HCNETSDKz[nWndID].InitHwnd(hwndz,wMSG);
	}
	catch (...)
	{
		theApp.WriteLogz("export fnInitWnd catch...nWndID=%d",nWndID);
	}	
}

//登录设备 (IP,用户名,密码,端口号,通道号,窗口ID)
DLL_EXPORTZ BOOL fnDVRLogin(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel, int nWndID,int itype)
{	
	BOOL BRState=FALSE;
	try
	{
		switch (itype)
		{
		case 1://登录设备
			BRState=theApp.m_HCNETSDKz[nWndID].fnDVR_Login_V30(strDvrIP,strUseName,strPwd,nPort,nChannel);
			break;
		case 2://打开语音对讲
			BRState=theApp.m_HCNETSDKz[nWndID].fnStartVoiceCom_V30(strDvrIP,strUseName,strPwd,nPort,nChannel);
			break;			
		default:
			break;
		}		
	}
	catch (...)
	{
		theApp.WriteLogz("export fnDVRLogin catch...nWndID=%d,itype=%d",nWndID,itype);
	}
	return BRState;
}


DLL_EXPORTZ BOOL fnSetState(int nWndID,int itype)
{	
	BOOL BRState=FALSE;
	try
	{
		switch(itype)
		{
		case 1://开启实时预览
			BRState=theApp.m_HCNETSDKz[nWndID].fnRealPlay();
			break;
		case 2://停止实时预览
			theApp.m_HCNETSDKz[nWndID].fnStopPlay();
			break;
		case 3://停止对讲
			theApp.m_HCNETSDKz[nWndID].fnStopVoiceCom();
			break;
		case 4://打开声音
			BRState=theApp.m_HCNETSDKz[nWndID].fnOpenSoundShare();
			break;
		case 5://关闭声音
			BRState=theApp.m_HCNETSDKz[nWndID].fnCloseSoundShare();
			break;
		default:
			break;
		} 
	}
	catch (...)
	{
		theApp.WriteLogz("export fnDVRLogin catch...nWndID=%d,itype=%d",nWndID,itype);
	}
	return BRState;
}

//开启录像回放
DLL_EXPORTZ BOOL fnPlayBackByTime(LPCTSTR strSTime,LPCTSTR strETime,int nWndID)
{
	BOOL BRState=FALSE;
	try
	{
		BRState=theApp.m_HCNETSDKz->fnPBByTime(strSTime,strETime,nWndID);
	}
	catch (...)
	{
		theApp.WriteLogz("export fnPlayBackByTime catch...nWndID=%d",nWndID);
	}
	return BRState;
}
//停止录像回放

// CBKTcpKM2.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKTcpKM2.h"

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
// CCBKTcpKM2App
//#define  DLLEXPORTZ
BEGIN_MESSAGE_MAP(CCBKTcpKM2App, CWinApp)
	//{{AFX_MSG_MAP(CCBKTcpKM2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKTcpKM2App construction

CCBKTcpKM2App::CCBKTcpKM2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKTcpKM2App object

CCBKTcpKM2App theApp;

BOOL CCBKTcpKM2App::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempPath,strPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));//当前目录下的LOG文件路径
	DWORD dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	tempPath.Format("%s\\%s",strPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}

// 	//////////////////////////////////////////////////////////////////////////
// 	dllHandleMAPTCP = LoadLibrary(TEXT("CMapTCPDLL.dll")); 
// 	if (dllHandleMAPTCP == NULL) 
// 	{ 
// 		AfxMessageBox("加载CMapTCPDLL.dll失败!"); 
// 		TerminateProcess(GetCurrentProcess(),0); 
// 		return FALSE; 
// 	}  
// 	
// 	dllMapTCPSend = (DLLMAPTCPSENG) GetProcAddress(dllHandleMAPTCP,TEXT("MAPTCPSENG"));
// 	if (dllHandleMAPTCP ==NULL)
// 	{
// 		AfxMessageBox("加载MAPTCPSENG失败!");
// 		return FALSE;
// 	}
	//////////////////////////////////////////////////////////////////////////
	dllHJMQGnss =LoadLibrary(TEXT("CSendDataToJMQ.dll")); 
	if (dllHJMQGnss == NULL)
	{
		AfxMessageBox("CSendDataToJMQ.dll失败!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE; 
	}
	dllSendGnssData =(DLLSendGnssData) GetProcAddress(dllHJMQGnss,TEXT("SendGnssData"));
	if (dllSendGnssData ==NULL)
	{
		AfxMessageBox("加载 SendGnssData 失败!");
	}
// 	//////////////////////////////////////////////////////////////////////////
// 	dllHzjjg =LoadLibrary(TEXT("CBKJGUdpClient.dll")); 
// 	if (dllHzjjg == NULL)
// 	{
// 		AfxMessageBox("CBKJGUdpClient.dll 失败!"); 
// 		TerminateProcess(GetCurrentProcess(),0); 
// 		return FALSE; 
// 	}
// 	dllzjjgGnssData =(DLLSendGnssData) GetProcAddress(dllHzjjg,TEXT("SendGnssData"));
// 	if (dllzjjgGnssData ==NULL)
// 	{
// 		AfxMessageBox("加载CBKJGUdpClient SendGnssData 失败!");
// 	}
// 	//////////////////////////////////////////////////////////////////////////
	lSendTime=timeGetTime();
	return CWinApp::InitInstance();
}

int CCBKTcpKM2App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(dllHJMQGnss !=NULL)
	{
		FreeLibrary(dllHJMQGnss);
	}
// 	if(dllHzjjg !=NULL)
// 	{
// 		FreeLibrary(dllHzjjg);
// 	}
// 	if(dllHandleMAPTCP !=NULL)
// 	{
// 		FreeLibrary(dllHandleMAPTCP);
// 	}
	return CWinApp::ExitInstance();
}
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec( dllexport ) BOOL DInitSocket(HWND hwndd)
{
	return TRUE;
}

extern "C" __declspec( dllexport ) BOOL SendString(double gnssx,double gnssy,float gnsslc,float gnsssd)
{
	if (timeGetTime()-theApp.lSendTime<900)
	{
		return FALSE;
	}
	theApp.lSendTime=timeGetTime();
//	theApp.dllMapTCPSend(gnssy*100000,gnssx*100000,1,0);
	theApp.dllSendGnssData(gnssx,gnssy,gnsssd,0,gnsslc);
//	theApp.dllzjjgGnssData(gnssx,gnssy,gnsssd,0,gnsslc);
	return TRUE;
}
extern "C" __declspec( dllexport ) LRESULT OnSockTGPS(WPARAM wParam,LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam); //消息类别 
	int iErrorCode = WSAGETSELECTERROR(lParam);//错误代码 
	SOCKET CurSock = (SOCKET)wParam; //目标socket 
	switch(iEvent) 
	{ 
	case FD_CONNECT: //客户端连接请求事件 
//		theApp.OnConnect(CurSock,iErrorCode);
		return 0;
	case  FD_READ:
//		theApp.OnReceive(CurSock);
		return 0;
	case FD_CLOSE:
//		theApp.OnClose(CurSock);
		return 0;
	default: 
		return 0;
	}
	return 0;
}
// CMapTCPDLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CMapTCPDLL.h"

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
// CCMapTCPDLLApp

BEGIN_MESSAGE_MAP(CCMapTCPDLLApp, CWinApp)
	//{{AFX_MSG_MAP(CCMapTCPDLLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMapTCPDLLApp construction

CCMapTCPDLLApp::CCMapTCPDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCMapTCPDLLApp object

CCMapTCPDLLApp theApp;
CString tempSend;
HANDLE	hThreadEvent;
/*
DWORD WINAPI SendThreadProc(LPVOID param)
{
	theApp.WriteLog("数字地图发送线程创建!");
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	if (sockClient ==INVALID_SOCKET)
	{
		theApp.WriteLog("sockClient socket faild!");
		return 0;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(theApp.strMapIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(9800);//9800
// 	bool bconn;
// 	bconn=false;
	while (TRUE)
	{
		
//  		if (bconn==false)
//  		{
			if (connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))== SOCKET_ERROR)
			{
				theApp.WriteLog("连接失败!等待重连!");
				closesocket(sockClient);
				sockClient=socket(AF_INET,SOCK_STREAM,0);
				if (sockClient ==INVALID_SOCKET)
				{
					theApp.WriteLog("sockClient socket faild!");
				}
				Sleep(1500);
				continue;
			}
// 			theApp.WriteLog("连接成功!");
// 			bconn=true;
// 		}
		WaitForSingleObject(hThreadEvent,INFINITE);
		if (SOCKET_ERROR==send(sockClient,tempSend.GetBuffer(0),strlen(tempSend)+1,0))
		{
			closesocket(sockClient);
			sockClient=socket(AF_INET,SOCK_STREAM,0);
			if (sockClient ==INVALID_SOCKET)
			{
				theApp.WriteLog("sockClient socket faild!");
				return 0;
			}
			theApp.WriteLog("发送失败!等待重连再发送!");
			Sleep(1000);
			//continue;
		}	
		closesocket(sockClient);
		sockClient=socket(AF_INET,SOCK_STREAM,0);
		if (sockClient ==INVALID_SOCKET)
		{
			theApp.WriteLog("sockClient socket faild!");
		}
	}
	closesocket(sockClient);
	theApp.WriteLog("退出数字地图发送线程!");
	return 0;
}
*/

DWORD WINAPI SendThreadProc(LPVOID param)
{
	theApp.WriteLog("数字地图发送线程创建! 20160519 1334");
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	if (sockClient ==INVALID_SOCKET)
	{
		theApp.WriteLog("sockClient socket faild!");
		return 0;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(theApp.strMapIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(9800);//9800
	bool bconn;
	bconn=false;
	while (TRUE)
	{
		
		if (bconn==false)
		{
			if (connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))== SOCKET_ERROR)
			{
				theApp.WriteLog("连接失败!等待重连!");
				closesocket(sockClient);
				sockClient=socket(AF_INET,SOCK_STREAM,0);
				if (sockClient ==INVALID_SOCKET)
				{
					theApp.WriteLog("sockClient socket faild!");
				}
				Sleep(1500);
				continue;
			}
			theApp.WriteLog("连接成功!");
			bconn=true;
		}
		WaitForSingleObject(hThreadEvent,INFINITE);
		if (SOCKET_ERROR==send(sockClient,tempSend.GetBuffer(0),strlen(tempSend)+1,0))
		{
			closesocket(sockClient);
			sockClient=socket(AF_INET,SOCK_STREAM,0);
			if (sockClient ==INVALID_SOCKET)
			{
				theApp.WriteLog("sockClient socket faild!");
				return 0;
			}
			theApp.WriteLog("发送失败!等待重连再发送!");
			bconn=false;
			Sleep(1000);
			continue;
		}
		else
		{
			CString log;
			log.Format("发送成功，tempSend=%s", tempSend.GetBuffer(0));
			theApp.WriteLog(log);
		}
	}
	closesocket(sockClient);
	theApp.WriteLog("退出数字地图发送线程! 20160519 1334");
	return 0;
}

BOOL CCMapTCPDLLApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempPath,tempt;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	tempt.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));//当前目录
	DWORD dwAttr=GetFileAttributes(tempt);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempt,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	tempPath.Format("%s\\%s",tempt,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	logfilename.Format("%s\\数字地图日志文件.log",tempPath);
	GPSlogf.Format("%s\\GPSDate.txt",tempPath);


	WriteLog("InitInstance START MapTCPDLL DLL!");
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		WriteLog("WSAStartup failed!");
		return FALSE;
	}
	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		
		WSACleanup();
		WriteLog("WSAStartup Version failed!");
		return FALSE; 
	}
	
	GetPrivateProfileString("CONFIG","GNSSMAPServer","",strMapIP.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
	strMapIP.ReleaseBuffer();
	UINT tempcardnum;
	tempcardnum=GetPrivateProfileInt("ICODE","Icode",99,".\\config.dat");
	CardNumi=tempcardnum+1;
	CString tempp;
	tempp.Format("数字地图IP地址:[%s],考车号:%d",strMapIP,CardNumi);
	WriteLog(tempp);	
	lSendTime=timeGetTime();
	hThreadEvent =CreateEvent(NULL,FALSE,FALSE,NULL);
	if (!strMapIP.IsEmpty())
	{
		::CreateThread(NULL,0,SendThreadProc,NULL,0,0);
	}	
	isendCount=0;
	return CWinApp::InitInstance();
}

int CCMapTCPDLLApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	WSACleanup();
	WriteLog("ExitInstance MapTCPDLL DLL!");
	return CWinApp::ExitInstance();
}
void CCMapTCPDLLApp::WriteLog(CString sstr)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen(logfilename,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,sstr);
	fclose(fp);
}
void CCMapTCPDLLApp::WriteGPSDate(CString sstr)
{
	FILE *fp=fopen(GPSlogf,"a+");
	fprintf(fp,"%s\n",sstr);
	fclose(fp);
}

extern "C" __declspec( dllexport ) BOOL MAPTCPSENG(double dLongitude,double dLatitude,int zt,int xm)
{
	tempSend.Format("[GPSInfo]\r\nCAR%dlon=%lf\r\nCAR%ddla=%lf\r\nZT%d=%d\r\nErrID%d=%d\r\n",theApp.CardNumi,dLatitude,theApp.CardNumi,dLongitude,theApp.CardNumi,zt,theApp.CardNumi,xm);
	SetEvent(hThreadEvent);
	if (theApp.isendCount<1000)
	{
		CString temp;
		temp.Format("%lf,%lf,%d,%d",dLongitude,dLatitude,zt,xm);
		theApp.WriteGPSDate(temp);
		theApp.isendCount+=1;
	}
	return TRUE;
}
extern "C" __declspec( dllexport ) void MATSENDC(double gpsx,double gpsy)
{
 	tempSend.Format("11%d,%lf,%lf,%lf",theApp.CardNumi,gpsx,gpsy,0);
 	SetEvent(hThreadEvent);
//	tempSend.Format("[GPSInfo]\r\nCAR%dlon=%lf\r\nCAR%ddla=%lf\r\nZT%d=%d\r\nErrID%d=%d\r\n",theApp.CardNumi,gpsx,theApp.CardNumi,gpsy,theApp.CardNumi,1,theApp.CardNumi,1);
//	SetEvent(hThreadEvent);
}
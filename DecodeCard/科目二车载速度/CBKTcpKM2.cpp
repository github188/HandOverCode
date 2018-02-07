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

void CCBKTcpKM2App::WriteLog(CString sstr)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen(logfilename,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,sstr);
	fclose(fp);
}

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
		CreateDirectory(tempPath,NULL);//今天日期文件夹
	}
	logfilename.Format("%s\\CBKUDPSD速度.log",tempPath);
	//////////////////////////////////////////////////////////////////////////
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
	if(m_socketz != NULL)
    {
		closesocket(m_socketz);
		m_socketz = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	CString JMKIP;
	GetPrivateProfileString("CONFIG","DECODEIP","",JMKIP.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
	JMKIP.ReleaseBuffer();
	if (JMKIP.IsEmpty())
	{
		AfxMessageBox("解码卡IP未配置!请配置udpconfig.ini 文件!");
		return FALSE;
	}
	addrSrv.sin_addr.S_un.S_addr=inet_addr(JMKIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(5288);
	WriteLog("解码卡IP:"+JMKIP+":5288");
	//////////////////////////////////////////////////////////////////////////
	UINT tempcardnum;
	CString temp;
	tempcardnum=GetPrivateProfileInt("ICODE","Icode",99,".\\config.dat");
	CardNumi=tempcardnum+1;
	temp.Format("考车%d",CardNumi);
	dischannel=GetPrivateProfileInt("CONFIG",temp,999,".\\udpconfig.ini");
	if (dischannel ==999)
	{
		AfxMessageBox("显示通道未配置!请配置udpconfig.ini 文件!");
	}
	temp.Format("显示通道号=%d,考车号=%d",dischannel,CardNumi);
	WriteLog(temp);
	//CoInitialize(NULL);
	return CWinApp::InitInstance();
}

int CCBKTcpKM2App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_socketz)
		closesocket(m_socketz);
	WSAAsyncSelect(m_socketz, m_hWndz, 0, 0);	//释放套接字与窗口句柄的关联
	WSACleanup();
	//CoUninitialize();
	WriteLog("ExitInstance CBKTcpKM2 DLL!");
	return CWinApp::ExitInstance();
}
BOOL CCBKTcpKM2App::InitSocketz(HWND hwndz)
{
	m_hWndz =hwndz;
	m_socketz =WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);//UDP
	if(m_socketz == INVALID_SOCKET)
	{
		AfxMessageBox("创建套接字失败!");
		WriteLog("socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSock.sin_family=AF_INET;
	addrSock.sin_port=htons(5286);
	if(SOCKET_ERROR==bind(m_socketz,(SOCKADDR*)&addrSock,sizeof(SOCKADDR)))
	{
		AfxMessageBox("绑定失败!");
		WriteLog("bind failed !");
		return FALSE;
	}
// 	if(SOCKET_ERROR==WSAAsyncSelect(m_socketz,hwndz,UM_SOCKETZ,FD_READ))
// 	{
// 		AfxMessageBox("注册网络读取事件失败!");
// 		WriteLog("WSAAsyncSelect failed !");
// 		return FALSE;
// 	}	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec( dllexport ) BOOL DInitSocket(HWND hwndd)
{
	if (!theApp.InitSocketz(hwndd))
	{
		theApp.WriteLog("InitSocketz faild!");
		return FALSE;
	}
	return TRUE;
}

extern "C" __declspec( dllexport ) BOOL SendString(UINT packi,LPTSTR sendstr)
{
	WSABUF wsabuf;
	DWORD dwSend;
	int len;
	CString msendbuf;
	msendbuf.Format("%02d%s,%d",packi,sendstr,theApp.dischannel);
	len=msendbuf.GetLength();	
	wsabuf.buf=msendbuf.GetBuffer(len);	
	wsabuf.len=len+1;		
	if(SOCKET_ERROR==WSASendTo(theApp.m_socketz,&wsabuf,1,&dwSend,0,(SOCKADDR*)&theApp.addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		theApp.WriteLog("WSASendTo failed!");
		return FALSE;
	}
	theApp.WriteLog(msendbuf);
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
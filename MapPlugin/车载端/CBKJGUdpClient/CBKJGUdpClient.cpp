// CBKJGUdpClient.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKJGUdpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define EXPORTAPI __declspec( dllexport )
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
// CCBKJGUdpClientApp

BEGIN_MESSAGE_MAP(CCBKJGUdpClientApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKJGUdpClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKJGUdpClientApp construction

CCBKJGUdpClientApp::CCBKJGUdpClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKJGUdpClientApp object

CCBKJGUdpClientApp theApp;

BOOL CCBKJGUdpClientApp::InitInstance() 
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
	logfilename.Format("%s\\车载到自建监管.log",tempPath);
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
	m_socketz =WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);//UDP
	if(m_socketz == INVALID_SOCKET)
	{
		AfxMessageBox("CCBKJGUdpClientApp 创建套接字失败!");
		WriteLog("socket failed !创建套接字失败!");
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}
	lSendTime=timeGetTime();
	//////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("CONFIG","KCHP","",m_strKchp.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
	m_strKchp.ReleaseBuffer(MAX_PATH);
	GetPrivateProfileString("CONFIG","ZJJGIP","",m_ZJJGIP.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
	m_ZJJGIP.ReleaseBuffer(MAX_PATH);
	addrSrv.sin_addr.S_un.S_addr=inet_addr(m_ZJJGIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(9701);
// 	GNSSXY=GetPrivateProfileInt("CONFIG","GNSSXY",0,".\\udpconfig.ini");
// 	GNSSR=GetPrivateProfileInt("CONFIG","GNSSR",0,".\\udpconfig.ini");
	//////////////////////////////////////////////////////////////////////////	
	WriteLog("InitInstance Build 20170531 1130 0605 2001 V1.3 考车号牌[%s],IP=%s:9701",m_strKchp,m_ZJJGIP);
	return CWinApp::InitInstance();
}

int CCBKJGUdpClientApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}

void CCBKJGUdpClientApp::WriteLog(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	CTime curTime =CTime::GetCurrentTime();
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	TRACE(logstr);
	va_end(avlist);
	FILE *fp=fopen(logfilename,"a+");
	fprintf(fp,"[%s]:%s#\n",curTime.Format(_T("%H:%M:%S")),logstr);
	fclose(fp);
}

extern "C" EXPORTAPI void UDPSendToJGSer(LPTSTR strs)
{
	theApp.SendUDPToSer(strs);
}

//extern "C" EXPORTAPI void SendGnssData(double x,double y,float sd,float fx,float lc)
//{
//	theApp.SendGnssDataz(x,y,fx,sd,lc);
//}
/*
void CCBKJGUdpClientApp::SendGnssDataz(double x, double y, float fx, float sd, float lc)
{
	if (timeGetTime()-lSendTime<500)
	{
		return ;
	}
	double gnssX,gnssY;
	if (theApp.GNSSXY==1)
	{
		gnssX=x*100000;
		gnssY=y*100000;
	}
	else if (theApp.GNSSXY==2)
	{
		gnssX=y*100000;
		gnssY=x*100000;
	}
	else if (theApp.GNSSXY==3)
	{
		gnssX=y;
		gnssY=x;
	}
	else
	{
		gnssX=x;
		gnssY=y;
	}
	CString strSend;
	//strSend.Format("11,%s,%lf,%lf,%f,%f,%f,#",m_strKchp,gnssX,gnssY,fx,sd,lc);
	//#BEGIN,车牌号,经度,纬度,航向角,x坐标,y坐标,速度,里程,信号,时间(yyyy-mm-dd hh24:mi:ss),END&
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	strSend.Format("#BEGIN,%s,%lf,%lf,%f,0,0,%f,%f,0,%s,END&",m_strKchp,gnssX,gnssY,fx,sd,lc,curTime.Format("%y-%m-%d %H:%M:%S"));
	WriteLog(strSend);
	int len;
	len=strSend.GetLength();
	WSABUF wsabuf;
	wsabuf.buf=strSend.GetBuffer(len);	
	wsabuf.len=len+1;
	
	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(m_socketz,&wsabuf,1,&dwSend,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		//TRACE(_T("Warning: SendPack 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		WriteLog("Warning: SGnssData 方法发生异常. 错误信息:%d",WSAGetLastError());
	}
	lSendTime=timeGetTime();
 	//delete[] wsabuf.buf;
}
*/

void CCBKJGUdpClientApp::SendUDPToSer(CString str)
{
	if (timeGetTime()-lSendTime<800)
	{
		return ;
	}
	CString strSend;
	strSend.Format("#%s&",str);
	WriteLog(strSend);
	int len;
	len=strSend.GetLength();
	WSABUF wsabuf;
	wsabuf.buf=strSend.GetBuffer(len);	
	wsabuf.len=len+1;	
	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(m_socketz,&wsabuf,1,&dwSend,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		WriteLog("Warning: SGnssData 方法发生异常. 错误信息:%d",WSAGetLastError());
	}
	lSendTime=timeGetTime();
}

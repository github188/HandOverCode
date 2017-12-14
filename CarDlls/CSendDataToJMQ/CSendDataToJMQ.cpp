// CSendDataToJMQ.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CSendDataToJMQ.h"
#include <math.h>
#include "afxmt.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define EXPORTAPI __declspec( dllexport )
#define RECV_MAXSIZE 512

CEvent g_SENDLIST_EVT;
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
// CCSendDataToJMQApp

BEGIN_MESSAGE_MAP(CCSendDataToJMQApp, CWinApp)
	//{{AFX_MSG_MAP(CCSendDataToJMQApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSendDataToJMQApp construction

CCSendDataToJMQApp::CCSendDataToJMQApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCSendDataToJMQApp object

CCSendDataToJMQApp theApp;

DWORD WINAPI SendThreadProc(LPVOID param)
{
	theApp.WriteLog("SendThreadProc Create!");
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	if (sockClient ==INVALID_SOCKET)
	{
		theApp.WriteLog("sockClient socket faild!");
		AfxMessageBox("创建失败!请重启程序!");
		return 0;
	}
	CStringList *pBuf=(CStringList *)param;
	Sleep(1000);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(theApp.JMQIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6708);//9800
	bool bconn;
	bconn=false;
	CString tempSend,tempRecv;
	char recvBuf[RECV_MAXSIZE];

	while (TRUE)
	{		
		if (bconn==false)
		{
			theApp.WriteLog("bconn==false 20170615 1107 add send timeout and revc time");
			if (connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))== SOCKET_ERROR)
			{
				closesocket(sockClient);
				sockClient=socket(AF_INET,SOCK_STREAM,0);
				if (sockClient ==INVALID_SOCKET)
				{
					theApp.WriteLog("connect SOCKET_ERROR sockClient socket faild!");
				}
				theApp.WriteLog("连接失败!等待重连!...6708");
				Sleep(1000);
				continue;
			}
			int timeout = 3000; //3s
			setsockopt(sockClient,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));
			setsockopt(sockClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(int));
			bconn=true;
		}
		BOOL bIsListEmpty;
		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		bIsListEmpty = pBuf->IsEmpty();
		g_SENDLIST_EVT.SetEvent();
		
		if (bIsListEmpty)
		{
			TRACE("Empty!\n");
			Sleep(1000);
			continue;
		}
		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		tempSend.Format("%s",pBuf->GetHead());
		g_SENDLIST_EVT.SetEvent();
		TRACE("begin send:[%s]\n",tempSend);
		if (SOCKET_ERROR==send(sockClient,tempSend.GetBuffer(0),strlen(tempSend)+1,0))
		{
			bconn=false;
			Sleep(1000);
			continue;
		}
		TRACE("end send \n");
		tempSend.ReleaseBuffer();
		TRACE("tempSend=%s\n",tempSend);
		memset(recvBuf,0,RECV_MAXSIZE);
		if (SOCKET_ERROR==recv(sockClient,recvBuf,RECV_MAXSIZE,0))
		{
			bconn=false;
			Sleep(1000);
			continue;
		}		
		tempRecv.Format("%s",recvBuf);
		TRACE("recv data:[%s]\n",tempRecv);		
		if (tempRecv ==tempSend)
		{
			WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
			pBuf->RemoveHead();//删除第一条
			g_SENDLIST_EVT.SetEvent();
			theApp.WriteLog("%s",tempRecv);
		}
		Sleep(1000);
	}
	closesocket(sockClient);
	theApp.WriteLog("SendThreadProc Close!");
	return 0;
}

BOOL CCSendDataToJMQApp::InitInstance() 
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
	logfilename.Format("%s\\解码器数据.log",tempPath);
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
	
	GetPrivateProfileString("CONFIG","JMQDNIP","",JMQIP.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
	JMQIP.ReleaseBuffer();
	addrSrv.sin_addr.S_un.S_addr=inet_addr(JMQIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6709);
	GNSSXY=GetPrivateProfileInt("CONFIG","GNSSXY",0,".\\udpconfig.ini");
	GNSSR=GetPrivateProfileInt("CONFIG","GNSSR",0,".\\udpconfig.ini");
	oldX[0]=0;
	oldY[0]=0;
	oldX[1]=0;
	oldY[1]=0;
	oldX[2]=0;
	oldY[2]=0;
	//////////////////////////////////////////////////////////////////////////
	UINT tempcardnum;
	tempcardnum=GetPrivateProfileInt("ICODE","Icode",99,".\\config.dat");
	m_iKCH=tempcardnum+1;	
	lSendTime=timeGetTime();
	g_SENDLIST_EVT.SetEvent();
	::CreateThread(NULL,0,SendThreadProc,&m_sendList,0,0);
	WriteLog("InitInstance Successful !!! Build 20170615 1024 V2.4 考车%d,IP=%s:6709,GNSSXY=%d,GNSSR=%d",m_iKCH,JMQIP,GNSSXY,GNSSR);
	ignsssend=0;
	//////////////////////////////////////////////////////////////////////////
	iGNssMap=GetPrivateProfileInt("CONFIG","GNSSMAP",1,".\\udpconfig.ini");
	if (iGNssMap==0)
	{	
		dllHandleMAPTCP = LoadLibrary(TEXT("CMapTCPDLL.dll")); 
		if (dllHandleMAPTCP == NULL) 
		{ 
			AfxMessageBox("加载CMapTCPDLL.dll失败!"); 
			TerminateProcess(GetCurrentProcess(),0); 
			return FALSE; 
		}  
		
		dllMapSendC = (DLLMATSENDC) GetProcAddress(dllHandleMAPTCP,TEXT("MATSENDC"));
		if (dllMapSendC ==NULL)
		{
			AfxMessageBox("加载MAPTCPSENG失败!");
			return FALSE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	m_socketz =WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);//UDP
	if(m_socketz == INVALID_SOCKET)
	{
		AfxMessageBox("创建套接字失败!");
		WriteLog("socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}

	return CWinApp::InitInstance();
}

int CCSendDataToJMQApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	WriteLog("ExitInstance Successful !!! Build 20170615 1024 V2.4");
	return CWinApp::ExitInstance();
}

void CCSendDataToJMQApp::WriteLog(LPCTSTR pstrFormat, ...)
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

BOOL CCSendDataToJMQApp::InitSocketz(HWND hwndz)
{

	m_socketz =WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);//UDP
	if(m_socketz == INVALID_SOCKET)
	{
		AfxMessageBox("创建套接字失败!");
		WriteLog("socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}
// 	SOCKADDR_IN addrSock;
// 	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
// 	addrSock.sin_family=AF_INET;
// 	addrSock.sin_port=htons(8604);
//  	if(SOCKET_ERROR==bind(m_socketz,(SOCKADDR*)&addrSock,sizeof(SOCKADDR)))
//  	{
//  		AfxMessageBox("绑定失败!");
//  		WriteLog("bind failed !");
//  		return FALSE;
//  	}
//  	if(SOCKET_ERROR==WSAAsyncSelect(m_socketz,hwndz,UM_JMQDATA,FD_READ))
//  	{
//  		AfxMessageBox("注册网络读取事件失败!");
//  		WriteLog("WSAAsyncSelect failed !");
//  		return FALSE;
//  	}	
	return TRUE;
}

extern "C" EXPORTAPI BOOL DInitSocket(HWND hwndd)
{
// 	if (!theApp.InitSocketz(hwndd))
// 	{
// 		return FALSE;
// 	}
	return TRUE;
}

extern "C" EXPORTAPI void SendJGPTData(UINT type,LPCTSTR msgz)
{
	theApp.SendXMData(type,msgz);
}


/**
* @brief 车载向四合一发送GPS、信号数据
*
* @param[in]  x         经度
* @param[in]  y   纬度
* @param[in]  sd    速度
* @param[in] fx          方向
* @param[in]  lc   里程
* @param[in]  aqd   安全带（0：没插；1：插上）	
* @param[in]  js   脚刹（0：无信号；1：有信号）
* @param[in]  ss   手刹（0：无信号；1：有信号）
* @param[in]  fs   副刹（0：无信号；1：有信号）
* @param[in]  lh   离合（0：无信号；1：有信号）
* @param[in]  lb   喇叭（0：无信号；1：有信号）
* @param[in]  zzx   左转向（0：无信号；1：有信号）
* @param[in]  yzx   右转向（0：无信号；1：有信号）
* @param[in]  ygd   远光灯（0：无信号；1：有信号）
* @param[in]  jgd   近光灯（0：无信号；1：有信号）
* @param[in]  jsd   警示灯（0：无信号；1：有信号）
* @param[in]  xh   熄火（0：无信号；1：有信号）
* @param[in]  kgm   开关门（0：关门；1：开门）
* @param[in]  dw   档位
* @return 是否成功
*/
extern "C" EXPORTAPI void SendGnssData(double x, double y, float sd, float fx, float lc, int aqd=0, 
		int js=0, int ss=0, int fs=0, int lh=0, int lb=0, int zzx=0, int yzx=0, int ygd=0, int jgd=0, int jsd=0, int xh=0, int kgm=0, int dw=0)
{
	if (timeGetTime()-theApp.lSendTime<500)
	{
		return ;
	}
	GNSSMSG myGnss;
	if (theApp.GNSSXY==1)
	{
		myGnss.gnssX=x*100000;
		myGnss.gnssY=y*100000;
	}
	else if (theApp.GNSSXY==2)
	{
		myGnss.gnssX=y*100000;
		myGnss.gnssY=x*100000;
	}
	else if (theApp.GNSSXY==3)
	{
		myGnss.gnssX=y;
		myGnss.gnssY=x;
	}
	else
	{
		myGnss.gnssX=x;
		myGnss.gnssY=y;
	}
	if (theApp.GNSSR==1)
	{
		myGnss.gnssR=theApp.GetGnssFx(myGnss.gnssX,myGnss.gnssY);
	}
	else
	{
		myGnss.gnssR=fx;
	}
	myGnss.gnssSD=sd;

	//huangqiwei temp
	myGnss.aqd = aqd;
	myGnss.js = js;
	myGnss.ss = ss;
	myGnss.fs = fs;
	myGnss.lh = lh;
	myGnss.lb = lb;
	myGnss.zzx = zzx;
	myGnss.yzx = yzx;
	myGnss.ygd = ygd;
	myGnss.jgd = jgd;
	myGnss.jsd = jsd;
	myGnss.xh = xh;
	myGnss.kgm = kgm;
	myGnss.dw = dw;
	
	myGnss.gnssLC=lc;
	theApp.SGnssData(GNSSDATA,(char *)&myGnss,sizeof(GNSSMSG));
	if (theApp.ignsssend<1000)
	{
		theApp.WriteLog("x=%lf,y=%lf,sd=%f,fx=%f,lc=%f",x,y,sd,fx,lc);
		theApp.WriteLog("aqd=%d,js=%d,ss=%d,fs=%d,lh=%d,lb=%d,zzx=%d,yzx=%d,ygd=%d,jgd=%d,jsd=%d,xh=%d,kgm=%d,dw=%d",
			aqd,js,ss,fs,lh,lb,zzx,yzx,ygd,jgd,jsd,xh,kgm,dw);
		theApp.ignsssend++;
	}
	if (theApp.iGNssMap==0)
	{		
		theApp.dllMapSendC(myGnss.gnssX,myGnss.gnssY);
	}
}

void CCSendDataToJMQApp::SGnssData(PACKTYPE packtype, char *msg,int msglen)
{
	WSABUF wsabuf;
	wsabuf.len=msglen+PACKHAND;
	wsabuf.buf=new char[wsabuf.len];
	memcpy(wsabuf.buf, &packtype, sizeof(PACKTYPE));
	memcpy(wsabuf.buf + sizeof(PACKTYPE), &m_iKCH, sizeof(int));
	memcpy(wsabuf.buf + PACKHAND,msg,msglen);
	
	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(m_socketz,&wsabuf,1,&dwSend,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		TRACE(_T("Warning: SendPack 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		WriteLog("Warning: SGnssData 方法发生异常. 错误信息:");
		if (m_socketz!=NULL)
		{
			closesocket(m_socketz);
		}
		m_socketz =WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);//UDP
		if(m_socketz == INVALID_SOCKET)
		{
			WriteLog("WSASocket socket failed !");
		}
// 		GetPrivateProfileString("CONFIG","JMQDNIP","",JMQIP.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
// 		JMQIP.ReleaseBuffer();
// 		addrSrv.sin_addr.S_un.S_addr=inet_addr(JMQIP);
// 		addrSrv.sin_family=AF_INET;
// 		addrSrv.sin_port=htons(6709);
	}
	lSendTime=timeGetTime();
 	delete[] wsabuf.buf;
}

void CCSendDataToJMQApp::SendXMData(UINT type,LPCTSTR msgz)
{	
	CString msendbuf;
	msendbuf.Format("XMD*%d*%d*%s",m_iKCH,type,msgz);
	WriteLog("接口:%s",msendbuf);
	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	theApp.m_sendList.AddTail(msendbuf);
	g_SENDLIST_EVT.SetEvent();
}

double CCSendDataToJMQApp::CalculateRotateAngle(double dBeginx,double dBeginy,double dNextx,double dNexty)
{
    double dRotateAngle = ((int)(atan2(fabs(dBeginx-dNextx),fabs(dBeginy-dNexty))*180/3.1415926*100))/100;
    //double dRotateAngle = atan2(fabs(dBeginx-dNextx),fabs(dBeginy-dNexty))*180/PI;
	//如果下一点的横坐标大于前一点(在第一和第四象限)
	if (dNextx>=dBeginx)
	{
		//在第一象限(0<=dRotateAngle<=90)
		if (dNexty>=dBeginy)
		{
			dRotateAngle=90.0f-dRotateAngle;
		}
		else//第四
		{
			dRotateAngle=270.0f+dRotateAngle;
		}
	}
	else//(在第二和第三象限)
	{
		//第二象限
		if (dNexty>=dBeginy)
		{
			dRotateAngle=90.0f+dRotateAngle;
		}
		else//第三象限
		{
			dRotateAngle=270.0f-dRotateAngle;
		}
	}
	return dRotateAngle;
}

float CCSendDataToJMQApp::GetGnssFx(double x, double y)
{
	float fFxj=0.0f;

	fFxj=(float)CalculateRotateAngle(oldX[0],oldY[0],x,y)-90;

	oldX[0]=oldX[1];
	oldX[1]=oldX[2];
	oldX[2]=x;

	oldY[0]=oldY[1];
	oldY[1]=oldY[2];
	oldY[2]=y;
	return fFxj;
}




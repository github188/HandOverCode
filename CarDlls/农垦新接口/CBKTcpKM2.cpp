// CBKTcpKM2.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKTcpKM2.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SAVEDAYS 15 //LOG保存天数
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

/////////////////////////////////////////////////////////////////////////////
//HANDLE HsendThread;
//int conniT=0;
DWORD WINAPI SendThreadProc(LPVOID param)
{
	theApp.WriteLogOne("发送线程创建 ! SendThreadProc Create!");
	while (TRUE)
	{
		if (theApp.ConnOK==FALSE)
		{
			break;
		}
  		if (theApp.SendData()==FALSE)
		{
			Sleep(3000);
		}
		else
		{
			Sleep(1000);
		}
	}
	theApp.WriteLogOne("发送线程关了 ! SendThreadProc Close!");
	return 0;
}

void CCBKTcpKM2App::WriteLogOne(LPCTSTR pstrFormat, ...)
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
	FILE *fp=fopen(logFileOne,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

void CCBKTcpKM2App::WriteLogTwo(LPCTSTR pstrFormat, ...)
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
	FILE *fp=fopen(logFileTwo,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

void CCBKTcpKM2App::WriteLogThree(LPCTSTR pstrFormat, ...)
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
	FILE *fp=fopen(logFileThree,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

BOOL CCBKTcpKM2App::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strPath,tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));
	strPathvoice.Format("%s\\voice\\errorcode.wav",tempPath.Left(tempPath.ReverseFind('\\')));
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
	logFileOne.Format("%s\\One日志文件一.log",tempPath);
	logFileTwo.Format("%s\\Two日志文件二.log",tempPath);
	logFileThree.Format("%s\\Three日志文件三.log",tempPath);
	//////////////////////////////////////////////////////////////////////////
	WriteLogOne("InitInstance START CBKTcpKM2 DLL! 20180118 09:35");	
	//////////////////////////////////////////////////////////////////////////	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		WriteLogOne("WSAStartup failed!");
		return FALSE;
	}
	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		
		WSACleanup();
		WriteLogOne("WSAStartup Version failed!");
		return FALSE; 
	}
	if(m_socketz != NULL)
    {
		closesocket(m_socketz);
		m_socketz = NULL;
	}
	CString JGPTIP;
	GetPrivateProfileString("CONFIG","Xmip","",JGPTIP.GetBuffer(MAX_PATH),MAX_PATH,".\\config.dat");
	JGPTIP.ReleaseBuffer();
	addrSrv.sin_addr.S_un.S_addr=inet_addr(JGPTIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8889);
	WriteLogOne("%s,18889 20170313 09:58",JGPTIP);
//	WriteLog(strPathvoice);
	sendi=1;
	ConnOK=FALSE;
	UINT tempcardnum;
	tempcardnum=GetPrivateProfileInt("ICODE","Icode",99,".\\config.dat");
	CardNumi=tempcardnum+1;
	r11=FALSE;
	r55=FALSE;
	fzb=TRUE;
	//////////////////////////////////////////////////////////////////////////
	CString linestr,getstr;
	if (GetPrivateProfileString("DATA","DATA","",getstr.GetBuffer(MAX_PATH),MAX_PATH,".\\CSTRNOTSEND.dll"))
	{
		linestr.Format("%s",curTime.Format("%Y-%m-%d"));
		getstr.ReleaseBuffer();
		if (getstr !=linestr)
		{
			CFileFind FindLogFile;
			if (FindLogFile.FindFile("CSTRNOTSEND.dll"))
			{
				CFile::Remove("CSTRNOTSEND.dll");
			}
			FindLogFile.Close();
		}
	}
	bIPPWD=FALSE;
	CString msendbuf;
// 	msendbuf.Format("&PWDDD-%d-%d*PWDDD****#",CardNumi,rand()%100);
// 	m_sendList.AddTail(msendbuf);
// 	msendbuf.Format("&IPPPP-%d-%d*IPPPP****#",CardNumi,rand()%100);
// 	m_sendList.AddTail(msendbuf);
	msendbuf.Format("&TIME9-%d-%d*TIME9*****#",CardNumi,rand()%100);
	m_sendList.AddTail(msendbuf);
	int forcount=GetPrivateProfileInt("DATA","COUNTL",0,".\\CSTRNOTSEND.dll");
	for (int li=0;li<forcount;li++)
	{
		linestr.Format("%d",li);
		if(GetPrivateProfileString("LISTSTR",linestr,"",getstr.GetBuffer(MAX_PATH),MAX_PATH,".\\CSTRNOTSEND.dll"))
		{
			getstr.ReleaseBuffer();
			msendbuf=Encode(getstr);
			WriteLogOne("未上传数据:&"+msendbuf);
			if (msendbuf.Left(2)!="&1")
			{
				msendbuf.Format("未上传数据不正确,%d=%s",li,getstr);
				WriteLogOne(msendbuf);
				continue;
			}
			m_sendList.AddTail(msendbuf);
		}
	}
	CFileFind FindLogFile;
	if (FindLogFile.FindFile("CSTRNOTSEND.dll"))
	{
		CFile::Remove("CSTRNOTSEND.dll");
	}
	//////////////////////////////////////////////////////////////////////////
	g_SENDLIST_EVT.SetEvent();
	for(int i=0;i<20;i++)
	{
		mytime[i].T52ST=1000;
		mytime[i].T52SPAN=1000;
		mytime[i].T53ST=1000;
		mytime[i].T53SPAN=1000;
		mytime[i].T55ST=1000;
		mytime[i].T55SPAN=1000;
		mytime[i].T52OK=1000;
		mytime[i].T55OK=1000;
	}
	TC53=timeGetTime();
	sendtime=timeGetTime();
	XMi=0;
	SXMi=0;
	connSumi=0;
	iSendCount=0;
//	AfxBeginThread(CCBKTcpKM2App::SendThread,this);
//	HsendThread=::CreateThread(NULL,0,SendThreadProc,NULL,0,0);

	dllHandleJMQ = LoadLibrary(TEXT("CSendDataToJMQ.dll")); 
	if (dllHandleJMQ == NULL) 
	{ 
		//WriteLog("加载 CSendDataToJMQ.dll 失败!");
		return FALSE;
	}
	dllsendtojmq = (DLLSendDataToJMQ) GetProcAddress(dllHandleJMQ,TEXT("SendJGPTData"));
	dllInitjmqSocket =(DLLDInitSocket) GetProcAddress(dllHandleJMQ,TEXT("DInitSocket"));

	return CWinApp::InitInstance();
}

int CCBKTcpKM2App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if (dllHandleJMQ!=NULL)
	{
		FreeLibrary(dllHandleJMQ);
	}

	if(m_socketz)
		closesocket(m_socketz);
	WSAAsyncSelect(m_socketz, m_hWndz, 0, 0);	//释放套接字与窗口句柄的关联
	WSACleanup();
	//////////////////////////////////////////////////////////////////////////
	CFileFind FindLogFile;
	if (FindLogFile.FindFile("CSTRNOTSEND.dll"))
	{
		CFile::Remove("CSTRNOTSEND.dll");
	}
	//////////////////////////////////////////////////////////////////////////
	CString linestr,writestr,temp;
	int countk=m_sendList.GetCount();
	if (countk>0)
	{
		COleDateTime curTime =COleDateTime::GetCurrentTime();
		linestr.Format("%s",curTime.Format("%Y-%m-%d"));
		::WritePrivateProfileString("DATA","DATA",linestr,".\\CSTRNOTSEND.dll");
		linestr.Format("%d",countk);
		::WritePrivateProfileString("DATA","COUNTL",linestr,".\\CSTRNOTSEND.dll");
	}
	for (int li=0;li<countk;li++)
	{
		linestr.Format("%d",li);
		temp.Format("%s",m_sendList.GetHead());
		writestr=Decode(temp);
		::WritePrivateProfileString("LISTSTR",linestr,writestr,".\\CSTRNOTSEND.dll");		
		m_sendList.RemoveHead();
	}
	//////////////////////////////////////////////////////////////////////////
	CString tempPath,strPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log\\*",tempPath.Left(tempPath.ReverseFind('\\')));//当前目录下的LOG文件路径    
	
	CString sDayFileName[SAVEDAYS];
	COleDateTime tempDT;	
	COleDateTime TodayTime;
	COleDateTimeSpan  ts;
	TodayTime = COleDateTime::GetCurrentTime();	
	//记录需要保留的文件夹名称
	for (int i=0; i<SAVEDAYS; i++)
	{
		ts.SetDateTimeSpan(i,0,0,0);
		tempDT = TodayTime - ts; 
		sDayFileName[i].Format("%s",tempDT.Format("%Y-%m-%d"));
	}
	//遍历文件夹内的文件或文件夹，不属于保留范围内的，一律删除
	CFileFind filefind;	
	BOOL bFound = filefind.FindFile(strPath);
	int bDelFlag = 1;
	while(bFound)     
	{     
		bFound = filefind.FindNextFile();     
		if(filefind.IsDots())     
		{
			continue;
		}		
		//去掉文件(夹)只读等属性     
		SetFileAttributes(filefind.GetFilePath(),FILE_ATTRIBUTE_NORMAL);		
		bDelFlag = 1;
		for (i=0; i<SAVEDAYS; i++)
		{
			if (filefind.GetFileName() == sDayFileName[i])
			{
				bDelFlag = 0;
			}
		}		
		if (bDelFlag == 1)
		{
			if(filefind.IsDirectory())     
			{     
				//递归删除子文件夹     
				DeleteDirectory(filefind.GetFilePath());     
				RemoveDirectory(filefind.GetFilePath());     
			}     
			else     
			{     
				//删除文件     
				DeleteFile(filefind.GetFilePath());     
			}     
		}
	}     
	filefind.Close(); 
	//////////////////////////////////////////////////////////////////////////
	WriteLogOne("ExitInstance CBKTcpKM2 DLL!");
	return CWinApp::ExitInstance();
}
//////////////////////////////////////////////////////////////////////////
void CCBKTcpKM2App::DeleteDirectory(CString strDir)
{
	if(!strDir.IsEmpty())
	{		
		CFileFind ff;     
		BOOL bFound = ff.FindFile(strDir+"\\*");     
		while(bFound)     
		{     
			bFound = ff.FindNextFile();     
			if(ff.IsDots())     
			{
				continue;
			}    
			//去掉文件(夹)只读等属性     
			SetFileAttributes(ff.GetFilePath(),   FILE_ATTRIBUTE_NORMAL);     
			if(ff.IsDirectory())     
			{     
				//递归删除子文件夹     
				DeleteDirectory(ff.GetFilePath());     
				RemoveDirectory(ff.GetFilePath());     
			}     
			else     
			{     
				//删除文件     
				DeleteFile(ff.GetFilePath());     
			}     
		}     
		ff.Close();   	
		//然后删除该文件夹     
		RemoveDirectory(strDir);    
	}
}
//////////////////////////////////////////////////////////////////////////

int conniii=0;
UINT CCBKTcpKM2App::ConnentThread(LPVOID Pparam)
{

	if(conniii>1)
	{
		return 1;
	}
	conniii+=1;
	CCBKTcpKM2App *dlg=(CCBKTcpKM2App *)Pparam;
	while (TRUE)
	{
		if (dlg->ConnOK==TRUE)
		{
			break;
		}
		connect(dlg->m_socketz,(SOCKADDR*)&dlg->addrSrv,sizeof(SOCKADDR));
		dlg->WriteLogOne("Connect .....Wait!");		
		Sleep(3000);
	}
	dlg->WriteLogOne("ConnentThread .....break!");
	conniii-=1;
	return 0;
}


/*

UINT CCBKTcpKM2App::SendThread(LPVOID Pparam)
{
	CCBKTcpKM2App *dlg=(CCBKTcpKM2App *)Pparam;
	
	while (TRUE)
	{
		if (dlg->SendData()==FALSE)
		{
			Sleep(3000);
		}
		else
		{
			Sleep(1000);
		}
	}
	return 0;
}
*/
//////////////////////////////////////////////////////////////////////////
// HANDLE conhThread;
// DWORD WINAPI ConThreadProc(LPVOID param)
// {
// 	while(TRUE)
// 	{
// 		connect(theApp.m_socketz,(SOCKADDR*)&theApp.addrSrv,sizeof(SOCKADDR));
// 		theApp.WriteLog("Connect .....监管平台主机版!");
// 		Sleep(3000);
// 	}	
// 	theApp.WriteLog("Connect OK!");
// 	return 0;
// }
//////////////////////////////////////////////////////////////////////////
BOOL CCBKTcpKM2App::InitSocketz(HWND hwndz)
{
	m_hWndz =hwndz;
	m_socketz = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//流类型的套接字 SOCK_STREAM
	//ASSERT(m_socketz != NULL);
	if(m_socketz == INVALID_SOCKET)
	{
		AfxMessageBox("创建套接字失败!");
		WriteLogOne("socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}
	//将网络事件与事件对象关联起来
	if(WSAAsyncSelect(m_socketz, m_hWndz, UM_SOCKETZ, FD_CONNECT|FD_READ|FD_CLOSE) ==SOCKET_ERROR)
	{
		WriteLogOne("WSAAsyncSelect failed !");
		return FALSE;
	}
	ConnOK=FALSE;
	AfxBeginThread(CCBKTcpKM2App::ConnentThread,this);
	//conhThread= ::CreateThread(NULL,0,ConThreadProc,NULL,0,0);	

	if (!dllInitjmqSocket(hwndz))
	{
		WriteLogOne("初始化解码器套接字失败!");
	}
	else
	{
		WriteLogOne("初始化解码器套接字OK 201710131034!");
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
void CCBKTcpKM2App::OnConnect(SOCKET CurSock, int error)
{
	if(0 ==error)
	{
		if(CurSock == m_socketz)
		{
			ConnOK=TRUE;
			WriteLogOne("成功连接到服务器!");
			::CreateThread(NULL,0,SendThreadProc,NULL,0,0);
			/*
			HsendThread=::CreateThread(NULL,0,SendThreadProc,NULL,0,0);
			BOOL bIsListEmpty;

			WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
			bIsListEmpty = m_sendList.IsEmpty();
			g_SENDLIST_EVT.SetEvent();

			if(!bIsListEmpty)
			{
				SendData();
			}
			*/
// 			TerminateThread(conhThread,5403);
// 			CloseHandle(conhThread);
		}
	}
	else
	{
		WriteLogOne("连接到服务器失败!");
		//PostMessage(HWND_BROADCAST,WM_CONNFILD,0,0);
//		sndPlaySound(strPathvoice,SND_SYNC);
	}
}
void CCBKTcpKM2App::OnClose(SOCKET CurSock)
{
//结束与相应的客户端的通信，释放相应资源
	WriteLogOne("Socket is close ...%d 准备重连...",CurSock);	
	if(m_socketz)
		closesocket(m_socketz);
	WSAAsyncSelect(m_socketz, m_hWndz, 0, 0);
	m_socketz = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//流类型的套接字 SOCK_STREAM
	if(m_socketz == INVALID_SOCKET)
	{
		WriteLogOne("OnClose socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return ;
	}
	if(WSAAsyncSelect(m_socketz, m_hWndz, UM_SOCKETZ, FD_CONNECT|FD_READ/*|FD_WRITE*/|FD_CLOSE) ==SOCKET_ERROR)
	{
		WriteLogOne("WSAAsyncSelect failed !");
	}
	ConnOK=FALSE;
	CString JGPTIP;
	GetPrivateProfileString("CONFIG","Xmip","",JGPTIP.GetBuffer(MAX_PATH),MAX_PATH,".\\config.dat");
	JGPTIP.ReleaseBuffer();
	addrSrv.sin_addr.S_un.S_addr=inet_addr(JGPTIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8889);
//	conhThread= ::CreateThread(NULL,0,ConThreadProc,NULL,0,0);
	
	AfxBeginThread(CCBKTcpKM2App::ConnentThread,this);
}
void CCBKTcpKM2App::OnReceive(SOCKET CurSock)
{
//读出网络缓冲区中的数据包
	WSABUF wsabuf;
	wsabuf.buf=new char[200];
	wsabuf.len=200;
	memset(wsabuf.buf,0,200);
	DWORD dwRead;
	DWORD dwFlag=0;
	SOCKADDR_IN addrFrom;
	int len=sizeof(SOCKADDR);
	CString strTemp;
	if(SOCKET_ERROR==WSARecvFrom(CurSock,&wsabuf,1,&dwRead,&dwFlag,
		(SOCKADDR*)&addrFrom,&len,NULL,NULL))
	{
		WriteLogOne("接收数据失败!");
		return ;
	}
	CString temp;
	temp.Format("%s",wsabuf.buf);	
	
	if (1 ==temp.Replace("*","*"))
	{
		CString tmp1,tmp2;
		tmp1.Format("%s",temp.Left(temp.Find("*")));

		BOOL bIsListEmpty;

		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		bIsListEmpty = m_sendList.IsEmpty();
		g_SENDLIST_EVT.SetEvent();

		if(!bIsListEmpty)
		{
			WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
			tmp2=m_sendList.GetHead();
			g_SENDLIST_EVT.SetEvent();

			if (tmp2.Find(tmp1) != -1)
			{
				iSendCount=0;//返回就清零
				WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
				m_sendList.RemoveHead();//删除第一条
				g_SENDLIST_EVT.SetEvent();

				if (tmp1.Find("17C51")!= -1)
				{
					dllsendtojmq(1,tmp2);
				}
				
				if (tmp1.Find("17C52")!= -1)
				{
					mytime[SXMi].T52OK =timeGetTime();
				}
				if (tmp1.Find("17C55")!= -1)
				{
					mytime[SXMi].T55OK =timeGetTime();
					SXMi+=1;
				}
					
				int errori=GetLastErrorStr(temp);
	
			}
			else
			{
				CString logstr;
				logstr.Format("返回值与列表值不一样:[%s!=%s]",tmp1,tmp2);
				WriteLogOne(logstr);
			}
		}
		
	}
	else
	{
		BOOL bIsListEmpty;				
		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		bIsListEmpty = m_sendList.IsEmpty();
		g_SENDLIST_EVT.SetEvent();
				
		if(!bIsListEmpty)
		{
			CString tmp2;
			WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
			tmp2=m_sendList.GetHead();
			g_SENDLIST_EVT.SetEvent();
			
			if (tmp2.Find("PWDDD") != -1)//密码可能有*号
			{
				iSendCount=0;//返回就清零
				WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
				m_sendList.RemoveHead();//删除第一条
				g_SENDLIST_EVT.SetEvent();
				GetLastErrorStr(temp);
			}
		}
		g_SENDLIST_EVT.SetEvent();
		WriteLogOne("返回值*号大于一个");
	}
	delete[] wsabuf.buf;//删除
	connSumi=0;

}
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec( dllexport ) BOOL DInitSocket(HWND hwndd)
{
	if (!theApp.InitSocketz(hwndd))
	{
		theApp.WriteLogOne("InitSocketz faild!");
		return FALSE;
	}
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
		theApp.OnConnect(CurSock,iErrorCode);
		return 0;
	case  FD_READ:
		theApp.OnReceive(CurSock);
		return 0;
	case FD_CLOSE:
		theApp.OnClose(CurSock);
		return 0;
// 	case FD_WRITE:
// 		theApp.OnSend(CurSock);
// 		break;
	default: 
		return 0;
	}
	return 0;
}

extern "C" __declspec( dllexport ) int SendString(UINT packi,LPTSTR packstr,LPTSTR strs)
{
//	CString strlog;
	if (111==packi)
	{
		return theApp.r111;
	}
	if(119==packi)
	{
		return theApp.bIPPWD;
	}
	if (123==packi)
	{
		if (theApp.ConnOK==TRUE)//连接成功
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	if (11==packi)
	{
		return theApp.r11;
	}
	if (45==packi)
	{
		return theApp.D45;
	}
	if (666==packi)
	{
		if (theApp.DataIsEmpty()==FALSE)
		{
			theApp.WriteLogThree("66 date TRUE");
			return TRUE;
		}
		else
		{
			theApp.WriteLogThree("66 date FALSE");
			return FALSE;
		}
	}
	if (55==packi)
	{
		return FALSE;
	}
	if (999==packi)
	{
		return theApp.fzb;
	}
	if (22==packi)
	{
		return theApp.fzb;
	}
	if (1==packi)
	{
		theApp.b17C56fz=FALSE;
		theApp.SaveTime(1);
		theApp.r11=FALSE;
		theApp.r111=FALSE;
	}
	if (2==packi)
	{
		theApp.b17C56fz=FALSE;
		theApp.D45 = FALSE;
		theApp.SaveTime(2);//保存17C52的时间
		theApp.r55=FALSE;
	}
	if(3==packi)
	{
		theApp.SaveTime(3);
	}
	if (5==packi)
	{
		theApp.SaveTime(5);//
		theApp.r55=FALSE;
	}


	CString strHead;
	CString msendbuf;
	theApp.sendi+=1;
	int rSendi=theApp.sendi;
	strHead.Format("&%s-%d-%d",packstr,theApp.CardNumi,rSendi);
	msendbuf.Format("%s%s#",strHead,strs);
	::WritePrivateProfileString("JGPT",strHead,NULL,".\\CSTRNOTSEND.dll");
	theApp.WriteLogThree(msendbuf);

	


	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	theApp.m_sendList.AddTail(msendbuf);
	g_SENDLIST_EVT.SetEvent();	

	CString jmqbuf;
	jmqbuf.Format("%s",msendbuf);
	if (packi >1 && packi<10)
	{
		if (9 == packi)
		{
			packi = 2;
		}
		theApp.dllsendtojmq(packi,jmqbuf);
	}

	return rSendi;
}

extern "C" __declspec( dllexport ) int GetRCode(LPTSTR packstr,int packi)
{
	int icode;
	CString strHead,temp;
	strHead.Format("%s-%d-%d",packstr,theApp.CardNumi,packi);
	GetPrivateProfileString("JGPT",strHead,"2017",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\CSTRNOTSEND.dll");
//	temp.ReleaseBuffer(0);
	icode=atoi(temp);
	return icode;
}

BOOL CCBKTcpKM2App::SendData()
{	
	BOOL bIsListEmpty;
	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	bIsListEmpty = m_sendList.IsEmpty();
	g_SENDLIST_EVT.SetEvent();

	if (bIsListEmpty)
	{
		//WriteLog("没有要发送的数据!,size=0");
		return FALSE;
	}
	if (timeGetTime()-sendtime<900)
	{
		return TRUE;
	}
	sendtime=timeGetTime();

	CString msendbuf;

	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	msendbuf.Format("%s",m_sendList.GetHead());
	g_SENDLIST_EVT.SetEvent();

	if (msendbuf.Left(6)=="&17C52")
	{
		int tempii;
		if(0==SXMi)
		{
			tempii=0;
		}
		else
		{
			tempii=SXMi-1;
		}
		if(timeGetTime()-mytime[tempii].T55OK < mytime[SXMi].T52SPAN)
		{
			return TRUE;
		}
	}
	if (msendbuf.Left(6)=="&17C53")
	{
		if(timeGetTime()-mytime[SXMi].T52OK < mytime[SXMi].T53SPAN)
		{
			return TRUE;
		}
		if(timeGetTime()-TC53<1000)
		{
			return TRUE;
		}
		TC53=timeGetTime();
	}
	if (msendbuf.Left(6)=="&17C55")
	{
		if(timeGetTime()-mytime[SXMi].T52OK < mytime[SXMi].T55SPAN+1000)
		{
			return TRUE;
		}
	}
	if (msendbuf.Left(6)=="&17C56")
	{
		if(timeGetTime()-TC53<3000)
		{
			return TRUE;
		}
		TC53=timeGetTime();
	}
	CString enSendStr;
	enSendStr=msendbuf;//Encode(msendbuf);//加密20151010
	int len;
	len=enSendStr.GetLength();	
	WSABUF wsabuf;
	DWORD dwSend;
	wsabuf.buf=enSendStr.GetBuffer(len);	
	wsabuf.len=len+1;
	iSendCount+=1;
	if (iSendCount >100)
	{
		iSendCount=0;
		OnClose(0);
		WriteLogOne("连续发送100条没有接收到返回");
		return TRUE;
	}

	if(SOCKET_ERROR==WSASendTo(m_socketz,&wsabuf,1,&dwSend,0,
		(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		WriteLogOne("发送的数据失败!WSASendTo:"+msendbuf);
		connSumi+=1;
		if(connSumi>100)
		{
			iSendCount=0;
			connSumi=0;
			OnClose(0);
			WriteLogOne("OnClose(0)");
		}
		return TRUE;
	}
	WriteLogTwo("发送"+msendbuf);
	return TRUE;
}

int CCBKTcpKM2App::GetLastErrorStr(CString str)
{
	CString templ,tempr,msgstr,tempkey;
	templ.Format("%s",str.Left(5));
	tempr.Format("%s",str.Mid(str.Find("*")+1));
	tempkey.Format("%s",str.Left(str.Find("*")));
	GetPrivateProfileString(templ,tempr,"未知返回值",msgstr.GetBuffer(MAX_PATH),MAX_PATH,".\\CBKTcpKM2.dat");
	msgstr.ReleaseBuffer();
	::WritePrivateProfileString("JGPT",tempkey,tempr,".\\CSTRNOTSEND.dll");
	if ("PWDDD"!=templ)
	{
		WriteLogTwo(str+msgstr);
	}
	fzb=TRUE;
	if(!strcmp(tempr,"1"))
	{	
		if (templ=="17C51")
		{
			r11=TRUE;
		}
		if (templ=="17C55")
		{
			r55=TRUE;
		}
		return 1;
	}
	if (!strcmp(tempr,"-90"))
	{	
		if (templ=="17C51")
		{
			r11=TRUE;
		}
		if (templ=="17C55")
		{
			r55=TRUE;
		}
		return -90;
	}
	if (!strcmp(tempr,"100"))
	{
		if (templ=="17C51")
		{
			r111=TRUE;
			return 1;
		}
	}
	if (!strcmp(tempr,"-2"))
	{
		if (templ=="17C52")
		{
			D45 = TRUE;
			return 1;
		}
	}
	if ("TIME9"==templ)
	{
		SetSysTime(tempr);
		return 1;	
	}
	if ("PWDDD"==templ)
	{
		SetIPPWD(tempr,1);
		return 1;	
	}
	if ("IPPPP"==templ)
	{
		SetIPPWD(tempr,2);
		return 1;	
	}
	fzb=FALSE;
	if (!strcmp(tempr,"$E"))//$E
	{
		AfxMessageBox(msgstr);
		return -1000;
	}
//	sndPlaySound(strPathvoice,SND_SYNC);
	WriteLogTwo("ERROR_MSG:"+msgstr);
	AfxMessageBox(msgstr);
	return atoi(tempr);
}

void CCBKTcpKM2App::SaveTime(int timei)
{
	CString strlog;
	switch(timei)
	{
	case 1:
		XMi=0;
		SXMi=0;
		mytime[0].T55ST=timeGetTime();
		mytime[0].T55OK=timeGetTime();
		break;
	case 2:
		{
		mytime[XMi].T52ST=timeGetTime();
		int tempi;
		if(XMi ==0)
		{
			tempi=0;
		}
		else
		{
			tempi=XMi-1;
		}
		mytime[XMi].T52SPAN =(mytime[XMi].T52ST - mytime[tempi].T55ST)/2;

		strlog.Format("T52SPAN= %ld,XMi=%d,tempi=%d",mytime[XMi].T52SPAN,XMi,tempi);
		WriteLogThree(strlog);
		break;
		}
	case 3:
		{
		mytime[XMi].T53ST=timeGetTime();
		mytime[XMi].T53SPAN =mytime[XMi].T53ST - mytime[XMi].T52ST;

		strlog.Format("T53SPAN= %ld,XMi=%d",mytime[XMi].T53SPAN,XMi);
		WriteLogThree(strlog);
		break;
		}
	case 4:
		break;
	case 5:
		{
		mytime[XMi].T55ST=timeGetTime();
		mytime[XMi].T55SPAN =mytime[XMi].T55ST - mytime[XMi].T52ST;
		strlog.Format("T55SPAN= %ld,XMi=%d",mytime[XMi].T55SPAN,XMi);
		WriteLogThree(strlog);

		XMi+=1;
		break;
		}
	case 6:
		break;
	default:
		break;
	}
}

BOOL CCBKTcpKM2App::SetSysTime(CString strTime)
{
	COleDateTime tm; // 
	SYSTEMTIME st;
	tm.ParseDateTime(strTime); //转换为COleDtaTime时间
	tm.GetAsSystemTime(st); // 获取SYSTEMTIME时间
	CString logstr;
	if(0==SetLocalTime(&st))//设置系统时间
	{		
		logstr.Format("设置本机时间为:%s 失败!",strTime);
		WriteLogOne(logstr);
		return FALSE;
	}	
	logstr.Format("设置本机时间为:%s 成功!",strTime);
	WriteLogOne(logstr);
	return TRUE;
}

CString CCBKTcpKM2App::Decode(CString str)
{
	CString outStr=_T("");
	if (str.IsEmpty())
	{
		return outStr;
	}
	outStr=str;
	outStr.MakeReverse();
	int iLen =outStr.GetLength();	
	for(int i=0;i<iLen;i++)
	{		
		if (i%3==0)
		{
			outStr.SetAt(i,outStr[i]+2);
		}
		else
		{
			outStr.SetAt(i,outStr[i]+1);
		}
	}
	return outStr;
}

CString CCBKTcpKM2App::Encode(CString str)
{
	CString outStr=_T("");
	if (str.IsEmpty())
	{
		return outStr;
	}
	outStr=str;
	int iLen =outStr.GetLength();
	
	for(int i=0;i<iLen;i++)
	{
		if (i%3==0)
		{
			outStr.SetAt(i,outStr[i]-2);
		}
		else
		{
			outStr.SetAt(i,outStr[i]-1);
		}
	}
	outStr.MakeReverse();
	return outStr;
}


BOOL CCBKTcpKM2App::DataIsEmpty()
{
	BOOL bIsListEmpty;
	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	bIsListEmpty = m_sendList.IsEmpty();
	g_SENDLIST_EVT.SetEvent();
	
	if (bIsListEmpty)
	{
		return FALSE;
	}
	return TRUE;
}

void CCBKTcpKM2App::SetIPPWD(CString str,int itype)
{
	CString temp;
	temp=Decode(str);
	if (itype==1)
	{
		::WritePrivateProfileString("PassWord","value",temp,".\\cfg.ini");//IP
		WriteLogOne("设置数据库密码成功!");
	}
	else
	{
		::WritePrivateProfileString("IPAddress","value",temp,".\\cfg.ini");//IP
		WriteLogOne("设置数据库IP成功!");
		bIPPWD=TRUE;
	}	
}

extern "C" __declspec( dllexport ) int GetSendSurPlus()
{
	return theApp.GetListCount();
}

int CCBKTcpKM2App::GetListCount()
{
	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	int iListCount= m_sendList.GetCount();
	g_SENDLIST_EVT.SetEvent();	
	return iListCount;
}

// TCPSockz.cpp: implementation of the CTCPSockz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "cjmq.h"
#include "TCPSockz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPSockz::CTCPSockz()
{
	recvwsabuf.buf=new char[PACK_SIZE];
	recvwsabuf.len=PACK_SIZE;
}

CTCPSockz::~CTCPSockz()
{
	delete [] recvwsabuf.buf;
}

BOOL CTCPSockz::InitSockS(HWND hwndz, UINT uPort, u_int wMSG,DWORD flogid)
{
	flogID=flogid;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		logz.WriteLog(flogID,"WSAStartup failed!");
		return FALSE;
	}	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		
		WSACleanup();
		logz.WriteLog(flogID,"WSAStartup Version failed!");
		return FALSE; 
	}
	//////////////////////////////////////////////////////////////////////////
	m_socketz=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);
	if(INVALID_SOCKET==m_socketz)
	{
		logz.WriteLog(flogID,"创建套接字失败! socket failed !%d",WSAGetLastError());
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSock.sin_family=AF_INET;
	addrSock.sin_port=htons(uPort);
	if(SOCKET_ERROR==bind(m_socketz,(SOCKADDR*)&addrSock,sizeof(SOCKADDR)))
	{
		logz.WriteLog(flogID,"bind failed ! 绑定失败!Port:%d,LastError=%d",uPort,WSAGetLastError());
		return FALSE;
	}
	
	if(SOCKET_ERROR == listen(m_socketz, 5))//这里的第二个参数表示最多支持的客户连接数
	{
		logz.WriteLog(flogID,"Listen error ! 监听失败!");
		return FALSE;
	}
	
	if(SOCKET_ERROR==WSAAsyncSelect(m_socketz,hwndz,wMSG, FD_ACCEPT|FD_READ|FD_CLOSE ))
	{
		logz.WriteLog(flogID,"WSAAsyncSelect failed ! 注册网络读取事件失败!");
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////	

	return TRUE;
}

// void CTCPSockz::RecvPack(PACKTYPE &packtype, char **recvmsg, int &ikch,SOCKET cursock)
// {
// 	memset(recvwsabuf.buf,0,PACK_SIZE);
// 	DWORD dwRead;
// 	DWORD dwFlag=0;
// 	SOCKADDR_IN addrFrom;
// 	int len=sizeof(SOCKADDR);
// 	CString strTemp;
// 	if(SOCKET_ERROR==WSARecvFrom(cursock,&recvwsabuf,1,&dwRead,&dwFlag,(SOCKADDR*)&addrFrom,&len,NULL,NULL))
// 	{
// 		TRACE(_T("Warning: RecvFromSock 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
// 		ikch=0;
// 		logz.WriteLog(flogID,"WSARecvFrom SOCKET_ERROR %d",WSAGetLastError());
// 		return ; 
// 	}
// 
// }


void CTCPSockz::OnFDAccept()
{
	SOCKET ClientSock;
	ClientSock=INVALID_SOCKET;
	ClientSock=accept(m_socketz,NULL,NULL);
	if (INVALID_SOCKET ==ClientSock)
	{
		logz.WriteLog(flogID,"TCP 客户端连接失败! WSAGetLastError=%d",WSAGetLastError());
	}
	else
	{
		logz.WriteLog(flogID,"TCP 客户端连接成功! %d",ClientSock);
	}
	
}

void CTCPSockz::CloseSock()
{
	if(m_socketz != NULL)
    {
		closesocket(m_socketz);
		m_socketz = NULL;
	}
}

CString CTCPSockz::RecvFromSock(SOCKET cursock)
{
	memset(recvwsabuf.buf,0,PACK_SIZE);
	DWORD dwRead;
	DWORD dwFlag=0;
	SOCKADDR_IN addrFrom;
	int len=sizeof(SOCKADDR);
	CString strTemp=_T("");
	if(SOCKET_ERROR==WSARecvFrom(cursock,&recvwsabuf,1,&dwRead,&dwFlag,(SOCKADDR*)&addrFrom,&len,NULL,NULL))
	{
 		TRACE(_T("Warning: RecvFromSock 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
 		logz.WriteLog(flogID,"WSARecvFrom SOCKET_ERROR %d",WSAGetLastError());
 		return strTemp; 
 	}
	strTemp.Format("%s",recvwsabuf.buf);
	return strTemp;
}

void CTCPSockz::SendToClient(SOCKET cursock,CString strSend)
{
	int len;
	len=strSend.GetLength();	
	WSABUF wsabuf;
	DWORD dwSend;
	wsabuf.buf=strSend.GetBuffer(len);	
	wsabuf.len=len+1;
	
	if(SOCKET_ERROR==WSASendTo(cursock,&wsabuf,1,&dwSend,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		TRACE(_T("Warning: WSASendTo 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		logz.WriteLog(flogID,"WSASendTo SOCKET_ERROR %d",WSAGetLastError());
	}
}

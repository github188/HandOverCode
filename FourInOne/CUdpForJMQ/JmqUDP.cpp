// JmqUDP.cpp: implementation of the CJmqUDP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JmqUDP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJmqUDP::CJmqUDP()
{
	m_socket = NULL;
	
	recvwsabuf.buf=new char[PACK_SIZE];
	recvwsabuf.len=PACK_SIZE;
}

CJmqUDP::~CJmqUDP()
{
	if(m_socket)
		closesocket(m_socket);
	WSACleanup();
}

BOOL CJmqUDP::InitSockU(HWND hwndz, UINT uPort,u_int wMSG)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		AfxMessageBox("WSAStartup failed!");
		return FALSE;
	}
	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		
		WSACleanup();
		AfxMessageBox("WSAStartup Version failed!");
		return FALSE; 
	}
	if(m_socket != NULL)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}
	
	m_socket=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);//UDP
	if(INVALID_SOCKET==m_socket)
	{
		AfxMessageBox("创建套接字失败!");
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSock.sin_family=AF_INET;
	addrSock.sin_port=htons(uPort);
	if(SOCKET_ERROR==bind(m_socket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR)))
	{
		AfxMessageBox("绑定失败!");
		return FALSE;
	}
	if(SOCKET_ERROR==WSAAsyncSelect(m_socket,hwndz,wMSG,FD_READ))
	{
		AfxMessageBox("注册网络读取事件失败!");
		return FALSE;
	}
	return TRUE;
}

BOOL CJmqUDP::RecvFromSock(PACKTYPE &packtype,char** recvmsg,int &ikch)
{
	memset(recvwsabuf.buf,0,PACK_SIZE);
	DWORD dwRead;
	DWORD dwFlag=0;
	SOCKADDR_IN addrFrom;
	int len=sizeof(SOCKADDR);
	CString strTemp;
	if(SOCKET_ERROR==WSARecvFrom(m_socket,&recvwsabuf,1,&dwRead,&dwFlag,(SOCKADDR*)&addrFrom,&len,NULL,NULL))
	{
		TRACE(_T("Warning: RecvFromSock 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		return FALSE; 
	}
//	addrTo =addrFrom;

	memcpy(&packtype,recvwsabuf.buf,sizeof(PACKTYPE));
	memcpy(&ikch,recvwsabuf.buf+sizeof(PACKTYPE),sizeof(int));
 	*recvmsg=/*(char*)*/recvwsabuf.buf+sizeof(PACKTYPE)+sizeof(int);	

	return TRUE;
}

BOOL CJmqUDP::SendPack(PACKTYPE packtype, char *msg,int msglen,int kch)
{
	WSABUF wsabuf;
	wsabuf.len=msglen+PACKHAND;
	wsabuf.buf=new char[wsabuf.len];
	memcpy(wsabuf.buf, &packtype, sizeof(PACKTYPE));
	memcpy(wsabuf.buf + sizeof(PACKTYPE), &kch, sizeof(int));
	memcpy(wsabuf.buf + PACKHAND,msg,msglen);

 	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(m_socket,&wsabuf,1,&dwSend,0,(SOCKADDR*)&sendPAddr,sizeof(SOCKADDR),NULL,NULL))
	{
		TRACE(_T("Warning: SendPack 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		delete[] wsabuf.buf;
		return FALSE;
	}
 	delete[] wsabuf.buf;
	return TRUE;
}


void CJmqUDP::SetSendAddr(CString serIP, UINT uPort)
{
	sendPAddr.sin_addr.S_un.S_addr=inet_addr(serIP);
	sendPAddr.sin_family=AF_INET;
	sendPAddr.sin_port=htons(uPort);
}

BOOL CJmqUDP::SendPack(PACKTYPE packtype, char *msg,int msglen,char *IP,UINT uPort,int kch)
{
	WSABUF wsabuf;
	wsabuf.len=msglen+PACKHAND;
	wsabuf.buf=new char[wsabuf.len];
	memcpy(wsabuf.buf, &packtype, sizeof(PACKTYPE));
	memcpy(wsabuf.buf + sizeof(PACKTYPE), &kch, sizeof(int));
	memcpy(wsabuf.buf + PACKHAND,msg,msglen);

	SOCKADDR_IN sendAddr;
	sendAddr.sin_addr.s_addr = inet_addr(IP);
	sendAddr.sin_port = htons(uPort);
	sendAddr.sin_family = AF_INET;

 	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(m_socket,&wsabuf,1,&dwSend,0,(SOCKADDR*)&sendAddr,sizeof(SOCKADDR),NULL,NULL))
	{
		TRACE(_T("Warning: SendPack 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		return FALSE;
	}
 	delete[] wsabuf.buf;
	return TRUE;
}



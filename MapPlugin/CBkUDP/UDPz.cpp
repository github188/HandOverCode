// UDPz.cpp: implementation of the UDPz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UDPz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define PACKSIZE 512
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UDPz::UDPz()
{
	m_socket = NULL;
	recvwsabuf.buf=new char[PACKSIZE];
	recvwsabuf.len=PACKSIZE;
}

UDPz::~UDPz()
{
	if(m_socket)
		closesocket(m_socket);
	delete[] recvwsabuf.buf;
//	WSACleanup();
}

BOOL UDPz::InitSockU(HWND hwndz, UINT uPort, u_int wMSG)
{
	if (InitSockUC()==FALSE)
	{
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

BOOL UDPz::SendPack(char *msg, int msglen, char *IP, UINT uPort)
{
	WSABUF wsabuf;
	wsabuf.len=msglen;
	wsabuf.buf=new char[wsabuf.len];
	memcpy(wsabuf.buf,msg,msglen);
	
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

void UDPz::SetSendAddr(CString serIP, UINT uPort)
{
	sendPAddr.sin_addr.S_un.S_addr=inet_addr(serIP);
	sendPAddr.sin_family=AF_INET;
	sendPAddr.sin_port=htons(uPort);
}

BOOL UDPz::SendPack(char *msg, int msglen)
{
	WSABUF wsabuf;
	wsabuf.len=msglen;
	wsabuf.buf=new char[wsabuf.len];
	memcpy(wsabuf.buf,msg,msglen);
		
	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(m_socket,&wsabuf,1,&dwSend,0,(SOCKADDR*)&sendPAddr,sizeof(SOCKADDR),NULL,NULL))
	{
		TRACE(_T("Warning: SendPack 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		return FALSE;
	}
	delete[] wsabuf.buf;
	return TRUE;
}

BOOL UDPz::RecvFromSock(char** recvmsg)
{
	memset(recvwsabuf.buf,0,PACKSIZE);
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
	*recvmsg=/*(char*)*/recvwsabuf.buf;		
	return TRUE;
}

BOOL UDPz::RecvFromSock(CString& strRecv)
{
	memset(recvwsabuf.buf,0,PACKSIZE);
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
	strRecv.Format("%s",recvwsabuf.buf);		
	return TRUE;
}

BOOL UDPz::InitSockUC()
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
	TRACE("m_socket=%d \n",m_socket);
	return TRUE;
}

BOOL UDPz::RecvFromSockStru(SOCKETLISTZ & strusock)
{
	memset(recvwsabuf.buf,0,PACKSIZE);
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
	memcpy(&strusock.kchpsu,recvwsabuf.buf,sizeof(struKCHP));
	TRACE("RecvFromSockStru %s,%d \n",strusock.kchpsu.cKchp,strusock.kchpsu.index);
	strusock.socketclient=m_socket;
	memcpy(&strusock.addrToclient, &addrFrom, sizeof(sockaddr_in));	
	return TRUE;
}

BOOL UDPz::SendPack(char *msg, int msglen,SOCKETLISTZ strusock)
{
	WSABUF wsabuf;
	wsabuf.len=msglen;
	wsabuf.buf=new char[wsabuf.len];
	memcpy(wsabuf.buf,msg,msglen);	
	DWORD dwSend;	
	if(SOCKET_ERROR==WSASendTo(strusock.socketclient,&wsabuf,1,&dwSend,0,(SOCKADDR*)&strusock.addrToclient,sizeof(SOCKADDR),NULL,NULL))
	{
		TRACE(_T("Warning: SendPack 方法发生异常. 错误信息: %d; 文件: %s; 行: %d\n"),WSAGetLastError() , __FILE__, __LINE__);
		return FALSE;
	}
	delete[] wsabuf.buf;
	return TRUE;
}

BOOL UDPz::WSAAsyncSelectz(HWND hwndz, u_int wMSG)
{
	if(m_socket == NULL)
	{
		return FALSE;
	}
	if(SOCKET_ERROR==WSAAsyncSelect(m_socket,hwndz,wMSG,FD_READ))
	{
		AfxMessageBox("注册网络读取事件失败!");
		return FALSE;
	}
	return TRUE;
}


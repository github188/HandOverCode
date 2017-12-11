// TCPSockz.h: interface for the CTCPSockz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSOCKZ_H__26065CBC_31CF_4C39_B1DA_268E92F7E2E8__INCLUDED_)
#define AFX_TCPSOCKZ_H__26065CBC_31CF_4C39_B1DA_268E92F7E2E8__INCLUDED_

#include "WriteLog.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Winsock2
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define  PACK_SIZE			256			//包数据大小

class CTCPSockz  
{
public:
	void SendToClient(SOCKET cursock,CString strSend);
	CString RecvFromSock(SOCKET cursock);
	void CloseSock();
	void OnFDAccept();
	BOOL InitSockS(HWND hwndz,UINT uPort,u_int wMSG,DWORD flogid);//初始化服务端
	CTCPSockz();
	virtual ~CTCPSockz();
private:
	SOCKET m_socketz;
	DWORD flogID;
	CWriteLog logz;
	SOCKADDR_IN addrSrv;
	WSABUF recvwsabuf;
};

#endif // !defined(AFX_TCPSOCKZ_H__26065CBC_31CF_4C39_B1DA_268E92F7E2E8__INCLUDED_)

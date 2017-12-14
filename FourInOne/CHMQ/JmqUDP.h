// JmqUDP.h: interface for the CJmqUDP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JMQUDP_H__13748D28_598B_476C_B253_910B87EA695C__INCLUDED_)
#define AFX_JMQUDP_H__13748D28_598B_476C_B253_910B87EA695C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
//////////////////////////////////////////////////////////////////////////
#define  PACK_SIZE			256			//包数据大小
#define  PACKHAND  sizeof(PACKTYPE)+sizeof(int)
extern enum PACKTYPE;
//////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CJmqUDP  
{
public:
	void SetSendAddr(CString serIP, UINT uPort);
	BOOL SendPack(PACKTYPE packtype, char *msg,int msglen,int kch);
	//////////////////////////////////////////////////////////////////////////
	BOOL SendPack(PACKTYPE packtype, char *msg,int msglen,char *IP,UINT uPort,int kch);
	BOOL RecvFromSock(PACKTYPE &packtype,char** recvmsg,int &ikch);
	BOOL InitSockU(HWND hwndz,UINT uPort,u_int wMSG);
	CJmqUDP();
	virtual ~CJmqUDP();
private:
	SOCKET m_socket;
	SOCKADDR_IN sendPAddr;
	WSABUF recvwsabuf;
};

#endif // !defined(AFX_JMQUDP_H__13748D28_598B_476C_B253_910B87EA695C__INCLUDED_)

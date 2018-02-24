// UDPz.h: interface for the UDPz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDPZ_H__F4ADB751_4EAB_465A_95F2_F6D0FA044DE6__INCLUDED_)
#define AFX_UDPZ_H__F4ADB751_4EAB_465A_95F2_F6D0FA044DE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")


//GNSS信息结构体
typedef struct _stru_GNSSMSG
{
	double	gnssX;	//经度
	double	gnssY;	//纬度
	float	gnssR;	//方向角
	float	gnssSD;	//速度
	float	gnssLC;	//里程
}struGnssMsg;

typedef struct _stru_KCHP
{
	char cKchp[10];
	int index;
}struKCHP;

typedef struct _stru_SOCKETZ
{
	struKCHP kchpsu;
	SOCKET socketclient;
	SOCKADDR_IN addrToclient;
	_stru_SOCKETZ()
	{
		socketclient=NULL;
	}
}SOCKETLISTZ;

typedef struct _stru_STU
{
	char bStuSFZ[20];	//身份证明编号
	char bXMZT[20];		//项目状态	
}struSTU;

typedef struct _stru_RBODY
{
	struKCHP    sKchp;
	struSTU		sStu;
	struGnssMsg sGnss;
}struRBody;

//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS UDPz  
{
public:
	UINT GetSocket();
	BOOL WSAAsyncSelectz(HWND hwndz,u_int wMSG);
	BOOL RecvFromSock(char** recvmsg);
	BOOL RecvFromSock(CString& strRecv);
	BOOL RecvFromSockStru(SOCKETLISTZ & strusock);

	BOOL SendPack(char *msg,int msglen,char *IP,UINT uPort);
	BOOL SendPack(char *msg,int msglen);
	BOOL SendPack(char *msg, int msglen,SOCKETLISTZ strusock);
	
	BOOL InitSockUC();
	BOOL InitSockU(HWND hwndz,UINT uPort,u_int wMSG);	

	void SetSendAddr(CString serIP, UINT uPort);
	UDPz();
	virtual ~UDPz();
private:
	SOCKET m_socket;
	SOCKADDR_IN sendPAddr;
	WSABUF recvwsabuf;
};

#endif // !defined(AFX_UDPZ_H__F4ADB751_4EAB_465A_95F2_F6D0FA044DE6__INCLUDED_)

// TcpSockKm3.h: interface for the CTcpSockKm3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSOCKKM3_H__63D6B6B5_63D0_4AE8_81E5_9F96A3A65546__INCLUDED_)
#define AFX_TCPSOCKKM3_H__63D6B6B5_63D0_4AE8_81E5_9F96A3A65546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
//////////////////////////////////////////////////////////////////////////
#define UM_SOCKZ		WM_USER+9
enum
{
	MSG_START=10,//开始考试
	MSG_STOP=11,//结束考试
	MSG_GPS=12,//GPS数据
	MSG_KCH=13,//考车号
	MSG_CLOSE=14, //关闭子系统 
	MSG_CONFIG=15,
	MSG_PLAYWND=16
};

//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CTcpSockKm3  
{
public:	
	void CloseSocket();
	BOOL SendString(UINT Cmdd,CString sendstr);
	BOOL SendString(char *ip,UINT Cmdd,CString sendstr,UINT dst_port=5286);
	CString RecvFromSock(int ilen=200);
	CString GetHostIP();
	BOOL InitSock(HWND hwndz,UINT dst_port=5286);
	CTcpSockKm3();
	virtual ~CTcpSockKm3();

private:
	SOCKET m_socket;
	WSABUF wsabuf;
	DWORD dwSend;
	int len;
	SOCKADDR_IN addrTo;
};

#endif // !defined(AFX_TCPSOCKKM3_H__63D6B6B5_63D0_4AE8_81E5_9F96A3A65546__INCLUDED_)

// CBKTcpKM2.h : main header file for the CBKTCPKM2 DLL
//

#if !defined(AFX_CBKTCPKM2_H__43B44C0B_9A48_4B57_AC63_5B1C428DB8CF__INCLUDED_)
#define AFX_CBKTCPKM2_H__43B44C0B_9A48_4B57_AC63_5B1C428DB8CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <list>
using namespace std;
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#define UM_SOCKETZ WM_USER+1986
/////////////////////////////////////////////////////////////////////////////
// CCBKTcpKM2App zsz 2014-05-26
// See CBKTcpKM2.cpp for the implementation of this class
//

typedef struct SPAN_TIME
{
	DWORD T52ST;
	DWORD T52SPAN;
	DWORD T53ST;
	DWORD T53SPAN;
	DWORD T55ST;
	DWORD T55SPAN;
	DWORD T52OK;
	DWORD T55OK;
}MYKZTIME;


class CCBKTcpKM2App : public CWinApp
{
public:	
	HINSTANCE dllHandleJMQ;
	typedef void (* DLLSendDataToJMQ)(UINT,LPCTSTR);
	DLLSendDataToJMQ dllsendtojmq;
	typedef BOOL (* DLLDInitSocket)(HWND);
	DLLDInitSocket dllInitjmqSocket;
	int r171;
	BOOL rCB3,r111,r62;
	void Send17C63(CString str);
	BOOL b17C56fz;
	BOOL DataIsEmpty();
	BOOL B91;
	DWORD sendtime;
	BOOL SetSysTime(CString strTime);
	DWORD TC53;
	int connSumi,iSendCount;
	void SaveTime(int timei);
	CString strPathvoice;
	BOOL r11,r55,fzb,D45;
//	DWORD T52OK[7],T52Send[7],TdeltaT[7],T55OKtime;	
	MYKZTIME mytime[20];
	UINT CardNumi;//考车号
	BOOL ConnOK;//是否连接成功
	static UINT ConnentThread(LPVOID Pparam);
//	static UINT SendThread(LPVOID Pparam);
	int sendi,XMi,SXMi;
	SOCKADDR_IN addrSrv;
	CStringList m_sendList;
//	int TOK52i,TSend52i,T55deltali;
	//////////////////////////////////////////////////////////////////////////
	void OnConnect(SOCKET CurSock, int error);
	void OnReceive(SOCKET CurSock);
	void OnClose(SOCKET CurSock);
	//////////////////////////////////////////////////////////////////////////
	BOOL SendData();
	int GetLastErrorStr(CString str);
	void WriteLog(CString sstr);
	void WriteSendRecvLog(CString sstr);
	void WriteLogThree(CString sstr);

	BOOL InitSocketz(HWND hwndz);
	void DeleteDirectory(CString strDir);
	CCBKTcpKM2App();
	SOCKET m_socketz;
private:
	//SOCKET m_socketz;
	HWND m_hWndz;
	SOCKADDR_IN addrSock;
	CString logFileOne,logFileTwo,logFileThree;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKTcpKM2App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCBKTcpKM2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKTCPKM2_H__43B44C0B_9A48_4B57_AC63_5B1C428DB8CF__INCLUDED_)

// CBKJGUdpClient.h : main header file for the CBKJGUDPCLIENT DLL
//

#if !defined(AFX_CBKJGUDPCLIENT_H__21250384_DCC4_4B14_885F_E3745173B167__INCLUDED_)
#define AFX_CBKJGUDPCLIENT_H__21250384_DCC4_4B14_885F_E3745173B167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
/////////////////////////////////////////////////////////////////////////////
// CCBKJGUdpClientApp
// See CBKJGUdpClient.cpp for the implementation of this class
//

class CCBKJGUdpClientApp : public CWinApp
{
public:
	void SendUDPToSer(CString str);
//	void SendGnssDataz(double x, double y, float fx, float sd, float lc);
	DWORD lSendTime;
	CCBKJGUdpClientApp();
	void WriteLog(LPCTSTR pstrFormat, ...);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKJGUdpClientApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCBKJGUdpClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//	int GNSSXY;
//	int GNSSR;

	CString m_ZJJGIP;
	CString m_strKchp;
	CString logfilename;
	SOCKET m_socketz;
	SOCKADDR_IN addrSrv;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKJGUDPCLIENT_H__21250384_DCC4_4B14_885F_E3745173B167__INCLUDED_)

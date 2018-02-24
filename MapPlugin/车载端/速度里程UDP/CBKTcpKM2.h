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
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define UM_SOCKETZ WM_USER+1988
/////////////////////////////////////////////////////////////////////////////
// CCBKTcpKM2App zsz 2014-05-26
// See CBKTcpKM2.cpp for the implementation of this class
//


class CCBKTcpKM2App : public CWinApp
{
public:	
	DWORD lSendTime;
	//////////////////////////////////////////////////////////////////////////
	HINSTANCE dllHJMQGnss;
	typedef void (* DLLSendGnssData)(double ,double ,float ,float ,float );
	DLLSendGnssData dllSendGnssData;

// 	HINSTANCE dllHzjjg;
// 	typedef void (* DLLUDPSendToJGSer)(LPTSTR);
// 	DLLUDPSendToJGSer dllzjjgSendData;
	//typedef void (* DLLSendGnssData)(double ,double ,float ,float ,float );
	
	//////////////////////////////////////////////////////////////////////////
// 	HINSTANCE dllHandleMAPTCP;
// 	typedef BOOL (* DLLMAPTCPSENG)(DOUBLE,DOUBLE,int,int);
// 	DLLMAPTCPSENG dllMapTCPSend;
	//////////////////////////////////////////////////////////////////////////
	CCBKTcpKM2App();
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

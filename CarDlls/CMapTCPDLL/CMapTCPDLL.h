// CMapTCPDLL.h : main header file for the CMAPTCPDLL DLL
//

#if !defined(AFX_CMAPTCPDLL_H__821F65F0_9A6D_41CF_888B_EB97CCEB5A64__INCLUDED_)
#define AFX_CMAPTCPDLL_H__821F65F0_9A6D_41CF_888B_EB97CCEB5A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
/////////////////////////////////////////////////////////////////////////////
// CCMapTCPDLLApp
// See CMapTCPDLL.cpp for the implementation of this class
//
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
class CCMapTCPDLLApp : public CWinApp
{
public:
	int isendCount;
	CString logfilename,GPSlogf;
	long lSendTime;
	void WriteGPSDate(CString sstr);
	UINT CardNumi;
	CString strMapIP;
	CCMapTCPDLLApp();
	void WriteLog(CString sstr);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMapTCPDLLApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCMapTCPDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMAPTCPDLL_H__821F65F0_9A6D_41CF_888B_EB97CCEB5A64__INCLUDED_)

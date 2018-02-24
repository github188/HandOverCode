// CVIDEO.h : main header file for the CVIDEO DLL
//

#if !defined(AFX_CVIDEO_H__479B5D2E_2AD1_4B15_BE16_06C842E1607B__INCLUDED_)
#define AFX_CVIDEO_H__479B5D2E_2AD1_4B15_BE16_06C842E1607B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
/////////////////////////////////////////////////////////////////////////////
// CCVIDEOApp
// See CVIDEO.cpp for the implementation of this class
//

class CCVIDEOApp : public CWinApp
{
public:
	CCVIDEOApp();
public:
	BOOL exGet;
	BOOL OnShowStringEx(CString str, UINT uLine,int iTD);
	BOOL GetDVRConfig();
	BOOL OnShowString(CString str,UINT uLine);
	DWORD lSendTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVIDEOApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCVIDEOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString Videocfg;
	int strX[8];
	int strY[8];
	LONG m_lLoginID;
	CString sIPaddr,sUser,sPwd,sTD;
	BOOL OnLoginHK();
	NET_DVR_SHOWSTRING_V30 m_struShowString;
	NET_DVR_SHOWSTRING_V30 m_struShowStringEX;

	int m_iChanShowNum;
	CString logfilename;
	void WriteLog(LPCTSTR pstrFormat, ...);	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIDEO_H__479B5D2E_2AD1_4B15_BE16_06C842E1607B__INCLUDED_)

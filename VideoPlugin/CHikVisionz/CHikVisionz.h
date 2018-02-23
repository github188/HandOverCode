// CHikVisionz.h : main header file for the CHIKVISIONZ DLL
//

#if !defined(AFX_CHIKVISIONZ_H__D2BD59F9_F96F_4ACB_B9A7_903503139D64__INCLUDED_)
#define AFX_CHIKVISIONZ_H__D2BD59F9_F96F_4ACB_B9A7_903503139D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "HCNETSDKz.h"
/////////////////////////////////////////////////////////////////////////////
// CCHikVisionzApp
// See CHikVisionz.cpp for the implementation of this class
//
#define MAX_WNDZ 16
class CCHikVisionzApp : public CWinApp
{
public:
	CHCNETSDKz m_HCNETSDKz[MAX_WNDZ];
	CCHikVisionzApp();
	void WriteLogz(LPCTSTR pstrFormat, ... );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCHikVisionzApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCHikVisionzApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString logfilenamez;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHIKVISIONZ_H__D2BD59F9_F96F_4ACB_B9A7_903503139D64__INCLUDED_)

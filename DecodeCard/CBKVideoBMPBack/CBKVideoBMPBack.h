// CBKVideoBMPBack.h : main header file for the CBKVIDEOBMPBACK application
//

#if !defined(AFX_CBKVIDEOBMPBACK_H__739BAA29_68A6_475C_96A9_9A9CAA81316E__INCLUDED_)
#define AFX_CBKVIDEOBMPBACK_H__739BAA29_68A6_475C_96A9_9A9CAA81316E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoBMPBackApp:
// See CBKVideoBMPBack.cpp for the implementation of this class
//

class CCBKVideoBMPBackApp : public CWinApp
{
public:
	CCBKVideoBMPBackApp();
	void WriteLog(CString sstr);
	CString logFileName;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKVideoBMPBackApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCBKVideoBMPBackApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKVIDEOBMPBACK_H__739BAA29_68A6_475C_96A9_9A9CAA81316E__INCLUDED_)

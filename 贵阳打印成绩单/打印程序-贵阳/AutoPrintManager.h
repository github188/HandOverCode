// AutoPrintManager.h : main header file for the AUTOPRINTMANAGER application
//

#if !defined(AFX_AUTOPRINTMANAGER_H__BB95CD67_3BDD_4187_A8EC_93ACAE3871EF__INCLUDED_)
#define AFX_AUTOPRINTMANAGER_H__BB95CD67_3BDD_4187_A8EC_93ACAE3871EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoPrintManagerApp:
// See AutoPrintManager.cpp for the implementation of this class
//

class CAutoPrintManagerApp : public CWinApp
{
public:
	CAutoPrintManagerApp();
	CString sVersion;
	int iCJ;
	//CListCtrl *m_Listall;

	CString sESer,sUser,sPwd;
	CStringArray strZKZM,strKSCS,stri;
	BOOL boolui;
	BOOL BOOLConSQL;  //只能开1格线程
	int iwz;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoPrintManagerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAutoPrintManagerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOPRINTMANAGER_H__BB95CD67_3BDD_4187_A8EC_93ACAE3871EF__INCLUDED_)

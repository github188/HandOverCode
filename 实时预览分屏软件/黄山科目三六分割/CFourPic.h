// CFourPic.h : main header file for the CFOURPIC application
//

#if !defined(AFX_CFOURPIC_H__7223D583_DACD_48DB_863A_87E96C1F33AB__INCLUDED_)
#define AFX_CFOURPIC_H__7223D583_DACD_48DB_863A_87E96C1F33AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCFourPicApp:
// See CFourPic.cpp for the implementation of this class
//

class CCFourPicApp : public CWinApp
{
public:
//	void ModifyRecord(CString sCard, CString sModify, CString sBZ, CString sOperator);
	CCFourPicApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFourPicApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCFourPicApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFOURPIC_H__7223D583_DACD_48DB_863A_87E96C1F33AB__INCLUDED_)

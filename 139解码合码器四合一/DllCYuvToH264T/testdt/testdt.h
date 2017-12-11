// testdt.h : main header file for the TESTDT application
//

#if !defined(AFX_TESTDT_H__DD16620A_D143_45A7_833D_3F89AA3EFCB8__INCLUDED_)
#define AFX_TESTDT_H__DD16620A_D143_45A7_833D_3F89AA3EFCB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestdtApp:
// See testdt.cpp for the implementation of this class
//

class CTestdtApp : public CWinApp
{
public:
	CTestdtApp();
	ULONG_PTR   gdiplusToken;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestdtApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestdtApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDT_H__DD16620A_D143_45A7_833D_3F89AA3EFCB8__INCLUDED_)

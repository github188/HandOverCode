// playwndtest.h : main header file for the PLAYWNDTEST application
//

#if !defined(AFX_PLAYWNDTEST_H__205AE17E_4B9D_4E7F_AB3E_803D3AE75A6F__INCLUDED_)
#define AFX_PLAYWNDTEST_H__205AE17E_4B9D_4E7F_AB3E_803D3AE75A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlaywndtestApp:
// See playwndtest.cpp for the implementation of this class
//

class CPlaywndtestApp : public CWinApp
{
public:
	CPlaywndtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaywndtestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPlaywndtestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYWNDTEST_H__205AE17E_4B9D_4E7F_AB3E_803D3AE75A6F__INCLUDED_)

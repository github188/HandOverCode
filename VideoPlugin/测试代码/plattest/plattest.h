// plattest.h : main header file for the PLATTEST application
//

#if !defined(AFX_PLATTEST_H__FB25EFF5_262C_4EC4_82BC_08D781734292__INCLUDED_)
#define AFX_PLATTEST_H__FB25EFF5_262C_4EC4_82BC_08D781734292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlattestApp:
// See plattest.cpp for the implementation of this class
//

class CPlattestApp : public CWinApp
{
public:
	CPlattestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlattestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPlattestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATTEST_H__FB25EFF5_262C_4EC4_82BC_08D781734292__INCLUDED_)

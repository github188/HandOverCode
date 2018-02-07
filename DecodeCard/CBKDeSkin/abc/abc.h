// abc.h : main header file for the ABC application
//

#if !defined(AFX_ABC_H__9C4BCEFE_891B_4F73_8AD4_3225613DE583__INCLUDED_)
#define AFX_ABC_H__9C4BCEFE_891B_4F73_8AD4_3225613DE583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAbcApp:
// See abc.cpp for the implementation of this class
//

class CAbcApp : public CWinApp
{
public:
	CAbcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAbcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABC_H__9C4BCEFE_891B_4F73_8AD4_3225613DE583__INCLUDED_)

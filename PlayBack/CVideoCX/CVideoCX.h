// CVideoCX.h : main header file for the CVIDEOCX application
//

#if !defined(AFX_CVIDEOCX_H__02B944F2_52C0_416A_B3AC_B0D52A33D158__INCLUDED_)
#define AFX_CVIDEOCX_H__02B944F2_52C0_416A_B3AC_B0D52A33D158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCVideoCXApp:
// See CVideoCX.cpp for the implementation of this class
//

class CCVideoCXApp : public CWinApp
{
public:
	CCVideoCXApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVideoCXApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCVideoCXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIDEOCX_H__02B944F2_52C0_416A_B3AC_B0D52A33D158__INCLUDED_)

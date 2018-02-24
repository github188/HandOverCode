// CPlayBackMap.h : main header file for the CPLAYBACKMAP application
//

#if !defined(AFX_CPLAYBACKMAP_H__979035EF_E1D9_419F_A49E_0DF9D41959AA__INCLUDED_)
#define AFX_CPLAYBACKMAP_H__979035EF_E1D9_419F_A49E_0DF9D41959AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackMapApp:
// See CPlayBackMap.cpp for the implementation of this class
//

class CCPlayBackMapApp : public CWinApp
{
public:
	CCPlayBackMapApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlayBackMapApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCPlayBackMapApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKMAP_H__979035EF_E1D9_419F_A49E_0DF9D41959AA__INCLUDED_)

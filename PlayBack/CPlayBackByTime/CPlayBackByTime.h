// CPlayBackByTime.h : main header file for the CPLAYBACKBYTIME application
//

#if !defined(AFX_CPLAYBACKBYTIME_H__085FA0D3_415E_4F49_97C4_59DBF8DEB467__INCLUDED_)
#define AFX_CPLAYBACKBYTIME_H__085FA0D3_415E_4F49_97C4_59DBF8DEB467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackByTimeApp:
// See CPlayBackByTime.cpp for the implementation of this class
//

class CCPlayBackByTimeApp : public CWinApp
{
public:
	CCPlayBackByTimeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlayBackByTimeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCPlayBackByTimeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKBYTIME_H__085FA0D3_415E_4F49_97C4_59DBF8DEB467__INCLUDED_)

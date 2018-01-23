// CDMap.h : main header file for the CDMAP application
//

#if !defined(AFX_CDMAP_H__B51D0D1A_9B9C_448D_A169_8168BDB71CBF__INCLUDED_)
#define AFX_CDMAP_H__B51D0D1A_9B9C_448D_A169_8168BDB71CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCDMapApp:
// See CDMap.cpp for the implementation of this class
//

class CCDMapApp : public CWinApp
{
public:
	CCDMapApp();
	ULONG_PTR gdiplusToken;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDMapApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCDMapApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDMAP_H__B51D0D1A_9B9C_448D_A169_8168BDB71CBF__INCLUDED_)

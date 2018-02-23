// mapTest.h : main header file for the MAPTEST application
//

#if !defined(AFX_MAPTEST_H__88743855_1770_4D62_9909_A2C06C6C1AC7__INCLUDED_)
#define AFX_MAPTEST_H__88743855_1770_4D62_9909_A2C06C6C1AC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMapTestApp:
// See mapTest.cpp for the implementation of this class
//

class CMapTestApp : public CWinApp
{
public:
	CMapTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMapTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPTEST_H__88743855_1770_4D62_9909_A2C06C6C1AC7__INCLUDED_)

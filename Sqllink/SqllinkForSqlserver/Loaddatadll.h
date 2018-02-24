// Loaddatadll.h : main header file for the LOADDATADLL application
//

#if !defined(AFX_LOADDATADLL_H__746F7A77_F690_49A6_A68E_11804B507D39__INCLUDED_)
#define AFX_LOADDATADLL_H__746F7A77_F690_49A6_A68E_11804B507D39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLoaddatadllApp:
// See Loaddatadll.cpp for the implementation of this class
//

class CLoaddatadllApp : public CWinApp
{
public:
	CLoaddatadllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaddatadllApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLoaddatadllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADDATADLL_H__746F7A77_F690_49A6_A68E_11804B507D39__INCLUDED_)

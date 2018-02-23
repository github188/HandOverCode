// HikVisionTest.h : main header file for the HIKVISIONTEST application
//

#if !defined(AFX_HIKVISIONTEST_H__FD10AFC1_59B4_40E8_8707_EA75D7566F00__INCLUDED_)
#define AFX_HIKVISIONTEST_H__FD10AFC1_59B4_40E8_8707_EA75D7566F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHikVisionTestApp:
// See HikVisionTest.cpp for the implementation of this class
//

class CHikVisionTestApp : public CWinApp
{
public:
	CHikVisionTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHikVisionTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHikVisionTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIKVISIONTEST_H__FD10AFC1_59B4_40E8_8707_EA75D7566F00__INCLUDED_)

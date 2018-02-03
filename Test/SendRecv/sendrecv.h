// sendrecv.h : main header file for the SENDRECV application
//

#if !defined(AFX_SENDRECV_H__7B006EC7_DD01_420E_8298_FF336727FC6C__INCLUDED_)
#define AFX_SENDRECV_H__7B006EC7_DD01_420E_8298_FF336727FC6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSendrecvApp:
// See sendrecv.cpp for the implementation of this class
//

class CSendrecvApp : public CWinApp
{
public:
	CSendrecvApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendrecvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSendrecvApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDRECV_H__7B006EC7_DD01_420E_8298_FF336727FC6C__INCLUDED_)

// CUDPServer.h : main header file for the CUDPSERVER application
//

#if !defined(AFX_CUDPSERVER_H__8FA07A3B_F298_4F1F_A344_0406223038BA__INCLUDED_)
#define AFX_CUDPSERVER_H__8FA07A3B_F298_4F1F_A344_0406223038BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCUDPServerApp:
// See CUDPServer.cpp for the implementation of this class
//

class CCUDPServerApp : public CWinApp
{
public:
	CCUDPServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCUDPServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCUDPServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUDPSERVER_H__8FA07A3B_F298_4F1F_A344_0406223038BA__INCLUDED_)

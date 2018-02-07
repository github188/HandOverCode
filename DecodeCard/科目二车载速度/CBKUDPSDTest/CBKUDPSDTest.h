// CBKUDPSDTest.h : main header file for the CBKUDPSDTEST application
//

#if !defined(AFX_CBKUDPSDTEST_H__55C883D5_64B6_4DF8_91AE_0C25FEEFFA84__INCLUDED_)
#define AFX_CBKUDPSDTEST_H__55C883D5_64B6_4DF8_91AE_0C25FEEFFA84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKUDPSDTestApp:
// See CBKUDPSDTest.cpp for the implementation of this class
//

class CCBKUDPSDTestApp : public CWinApp
{
public:
	CCBKUDPSDTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKUDPSDTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCBKUDPSDTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKUDPSDTEST_H__55C883D5_64B6_4DF8_91AE_0C25FEEFFA84__INCLUDED_)

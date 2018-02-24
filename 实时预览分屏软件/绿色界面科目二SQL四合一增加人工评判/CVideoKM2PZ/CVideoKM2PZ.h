// CVideoKM2PZ.h : main header file for the CVIDEOKM2PZ application
//

#if !defined(AFX_CVIDEOKM2PZ_H__F1B66048_546C_4584_8831_CDF1FE07A2C9__INCLUDED_)
#define AFX_CVIDEOKM2PZ_H__F1B66048_546C_4584_8831_CDF1FE07A2C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCVideoKM2PZApp:
// See CVideoKM2PZ.cpp for the implementation of this class
//

class CCVideoKM2PZApp : public CWinApp
{
public:
	CCVideoKM2PZApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVideoKM2PZApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCVideoKM2PZApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIDEOKM2PZ_H__F1B66048_546C_4584_8831_CDF1FE07A2C9__INCLUDED_)

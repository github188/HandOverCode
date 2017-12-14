// CJMQ.h : main header file for the CJMQ application
//

#if !defined(AFX_CJMQ_H__C3930382_0EBB_4D1C_8759_F48C02A59D4E__INCLUDED_)
#define AFX_CJMQ_H__C3930382_0EBB_4D1C_8759_F48C02A59D4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCJMQApp:
// See CJMQ.cpp for the implementation of this class
//

class CCJMQApp : public CWinApp
{
public:
	CCJMQApp();
	ULONG_PTR   gdiplusToken;//GDI+
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJMQApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCJMQApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CJMQ_H__C3930382_0EBB_4D1C_8759_F48C02A59D4E__INCLUDED_)

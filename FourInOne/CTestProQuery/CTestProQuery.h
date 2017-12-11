// CTestProQuery.h : main header file for the CTESTPROQUERY application
//

#if !defined(AFX_CTESTPROQUERY_H__49F34C24_C1F3_4FCD_BA5A_BA2844708006__INCLUDED_)
#define AFX_CTESTPROQUERY_H__49F34C24_C1F3_4FCD_BA5A_BA2844708006__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCTestProQueryApp:
// See CTestProQuery.cpp for the implementation of this class
//

class CCTestProQueryApp : public CWinApp
{
public:
	CCTestProQueryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCTestProQueryApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCTestProQueryApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTESTPROQUERY_H__49F34C24_C1F3_4FCD_BA5A_BA2844708006__INCLUDED_)

// CBKVideoDeCode.h : main header file for the CBKVIDEODECODE application
//

#if !defined(AFX_CBKVIDEODECODE_H__EC70CE00_6E49_4BBF_AC60_6CEA94538B7F__INCLUDED_)
#define AFX_CBKVIDEODECODE_H__EC70CE00_6E49_4BBF_AC60_6CEA94538B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoDeCodeApp:
// See CBKVideoDeCode.cpp for the implementation of this class
//

class CCBKVideoDeCodeApp : public CWinApp
{
public:
	CCBKVideoDeCodeApp();
	//////////////////////////////////////////////////////////////////////////
	CString logFileName;

	//////////////////////////////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKVideoDeCodeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCBKVideoDeCodeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKVIDEODECODE_H__EC70CE00_6E49_4BBF_AC60_6CEA94538B7F__INCLUDED_)

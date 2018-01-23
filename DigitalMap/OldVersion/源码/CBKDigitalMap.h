// CBKDigitalMap.h : main header file for the CBKDIGITALMAP application
//

#if !defined(AFX_CBKDIGITALMAP_H__D8DAD45B_32E0_4AEB_9461_AC6FE8B2E850__INCLUDED_)
#define AFX_CBKDIGITALMAP_H__D8DAD45B_32E0_4AEB_9461_AC6FE8B2E850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "VideoInterFace.h"

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapApp:
// See CBKDigitalMap.cpp for the implementation of this class
//

class CCBKDigitalMapApp : public CWinApp
{
public:
	void WriteErr(CString szERR);
	BOOL OnInitSQL();
	CCBKDigitalMapApp();
//	CString SQLSer,SQLUSER,SQLPWD,SQLDB;
	_ConnectionPtr m_pConn;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKDigitalMapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCBKDigitalMapApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKDIGITALMAP_H__D8DAD45B_32E0_4AEB_9461_AC6FE8B2E850__INCLUDED_)

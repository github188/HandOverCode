// CBKFourPicKM2.h : main header file for the CBKFOURPICKM2 application
//

#if !defined(AFX_CBKFOURPICKM2_H__B3AD2609_FDF5_48BC_986A_D57B22604264__INCLUDED_)
#define AFX_CBKFOURPICKM2_H__B3AD2609_FDF5_48BC_986A_D57B22604264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKFourPicKM2App:
// See CBKFourPicKM2.cpp for the implementation of this class
//

class CCBKFourPicKM2App : public CWinApp
{
public:
	CCBKFourPicKM2App();
	//////////////////////////////////////////////////////////////////////////
	typedef BOOL (* DRAWBKGND)(HDC, CRect&,UINT);
	DRAWBKGND lpfnDrawBkgnd;
	typedef BOOL (* MessageBoxbmp)(LPTSTR );
	MessageBoxbmp zbmpmessagebox;
	HINSTANCE dllHandle;
	//////////////////////////////////////////////////////////////////////////
	BOOL OnInitSQL();
	int SM_CXSCREEN_x,SM_CYSCREEN_y;
	void SetDevMode();
	//////////////////////////////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKFourPicKM2App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCBKFourPicKM2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKFOURPICKM2_H__B3AD2609_FDF5_48BC_986A_D57B22604264__INCLUDED_)

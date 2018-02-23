#if !defined(AFX_CBKPLAYZWNDX_H__A98EA8F4_C3BA_4FE8_BDE9_BC854FE34241__INCLUDED_)
#define AFX_CBKPLAYZWNDX_H__A98EA8F4_C3BA_4FE8_BDE9_BC854FE34241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CBKPlayzWndX.h : main header file for CBKPLAYZWNDX.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXApp : See CBKPlayzWndX.cpp for implementation.

class CCBKPlayzWndXApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKPLAYZWNDX_H__A98EA8F4_C3BA_4FE8_BDE9_BC854FE34241__INCLUDED)

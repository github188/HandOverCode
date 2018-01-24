#if !defined(AFX_CPLAYBACKZOCX_H__D679A198_A5D0_4B9E_85F0_720F6250C90F__INCLUDED_)
#define AFX_CPLAYBACKZOCX_H__D679A198_A5D0_4B9E_85F0_720F6250C90F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CPlayBackzOcx.h : main header file for CPLAYBACKZOCX.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxApp : See CPlayBackzOcx.cpp for implementation.

class CCPlayBackzOcxApp : public COleControlModule
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

#endif // !defined(AFX_CPLAYBACKZOCX_H__D679A198_A5D0_4B9E_85F0_720F6250C90F__INCLUDED)

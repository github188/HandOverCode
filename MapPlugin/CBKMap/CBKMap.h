#if !defined(AFX_CBKMAP_H__BCCABC32_AAB6_40B0_823A_AB02A15E92E9__INCLUDED_)
#define AFX_CBKMAP_H__BCCABC32_AAB6_40B0_823A_AB02A15E92E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CBKMap.h : main header file for CBKMAP.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKMapApp : See CBKMap.cpp for implementation.

class CCBKMapApp : public COleControlModule
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

#endif // !defined(AFX_CBKMAP_H__BCCABC32_AAB6_40B0_823A_AB02A15E92E9__INCLUDED)

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1463DA5D_1C28_437B_8FEC_519D3E693C0B__INCLUDED_)
#define AFX_STDAFX_H__1463DA5D_1C28_437B_8FEC_519D3E693C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(disable: 4786)//����4786����
//����ADO�ľ��� 
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
//ADO
#import "../../3rdparty/ADO/msado15.dll" no_namespace rename("EOF", "adoEOF")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1463DA5D_1C28_437B_8FEC_519D3E693C0B__INCLUDED_)

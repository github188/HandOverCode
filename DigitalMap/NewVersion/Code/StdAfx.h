// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__125D15F1_0810_4159_BB34_E55000AA32F0__INCLUDED_)
#define AFX_STDAFX_H__125D15F1_0810_4159_BB34_E55000AA32F0__INCLUDED_

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
//////////////////////////////////////////////////////////////////////////
#define UNICODE
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#include "C:\\gdiplus\\Include\\gdiplus.h" 
using namespace Gdiplus; 
#pragma comment(lib, "C:\\gdiplus\\lib\\gdiplus.lib") 
//////////////////////////////////////////////////////////////////////////
//日志类
#include "WriteLog.h"
#pragma comment(lib,"CLogFilez.lib")

#pragma warning(disable: 4786)//忽略4786警告

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__125D15F1_0810_4159_BB34_E55000AA32F0__INCLUDED_)

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
// by zhengshizeng

#if !defined(AFX_STDAFX_H__443603AF_B28B_42CD_865D_4A00AF53099D__INCLUDED_)
#define AFX_STDAFX_H__443603AF_B28B_42CD_865D_4A00AF53099D__INCLUDED_

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

//日志类
#include "WriteLog.h"
#pragma comment(lib,"CLogFilez.lib")
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4786)//忽略4786警告
//忽略ADO的警告 
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
//ADO
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF")
//////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__443603AF_B28B_42CD_865D_4A00AF53099D__INCLUDED_)

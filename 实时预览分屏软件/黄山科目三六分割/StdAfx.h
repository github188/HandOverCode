// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__562140C9_7119_4956_96F9_DA94086442A7__INCLUDED_)
#define AFX_STDAFX_H__562140C9_7119_4956_96F9_DA94086442A7__INCLUDED_

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

#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")

extern CString logFileName;
void WriteLog(LPCTSTR pstrFormat, ...);

#define WM_STARTALLVIDEO_MSG WM_USER+120

//////////////////////////////////////////////////////////////////////////
/*忽略ADO的警告*/ 
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#import "../../3rdparty/ADO/msado15.dll" no_namespace rename("EOF", "adoEOF")

CString VariantToStr(_variant_t var);
BOOL ChangeToVariant(CString str, _variant_t *var, DataTypeEnum type);
BOOL ChangeToVariant(int value, _variant_t * var, DataTypeEnum type);
BOOL ChangeToVariant(COleDateTime time, _variant_t* var, DataTypeEnum type);//DataTypeEnum数据库
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__562140C9_7119_4956_96F9_DA94086442A7__INCLUDED_)



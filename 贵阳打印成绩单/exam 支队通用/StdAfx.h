// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FF94AD6A_1263_4CB6_91F0_8E31CD5B4BD1__INCLUDED_)
#define AFX_STDAFX_H__FF94AD6A_1263_4CB6_91F0_8E31CD5B4BD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

/*忽略ADO的警告*/
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
/*ADO需要导入的库*/
#import "C:\Program Files\Common Files\SYSTEM\ADO\msado15.dll" no_namespace rename("EOF","adoEOF") \
	rename("DataTypeEnum","adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFielAttributeEnum") \
	rename("EditModeEnum", "adoEditModeEnum") \
	rename("LockTypeEnum", "adoLockTypeEnum") \
	rename("RecordStatusEnum", "adoRecordStatusEnum") \
rename("ParameterDirectionEnum", "adoParameterDirectionEnum")
BOOL CreateDir(CString P);
void WriteLogPPTXT(CString xmname,CString zkzmBH,CString sstr);  //写入日志文件
BOOL FolderExists(CString s);
//////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FF94AD6A_1263_4CB6_91F0_8E31CD5B4BD1__INCLUDED_)

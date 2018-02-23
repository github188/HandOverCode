#if !defined(AFX_STDAFX_H__5E75FEFA_59F5_4C08_B818_2AD82469F4AA__INCLUDED_)
#define AFX_STDAFX_H__5E75FEFA_59F5_4C08_B818_2AD82469F4AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes
#define UNICODE
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#include "../../3rdparty/gdiplus/Include/gdiplus.h" 
using namespace Gdiplus; 
#pragma comment(lib, "../../3rdparty/gdiplus/lib/gdiplus.lib") 

//日志类
#include "WriteLog.h"
#pragma comment(lib,"CLogFilez.lib")
/*
//忽略ADO的警告 
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
//ADO
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF") \
rename("DataTypeEnum","adoDataTypeEnum") \
rename("FieldAttributeEnum", "adoFielAttributeEnum") rename("EditModeEnum", "adoEditModeEnum") \
rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum") \
rename("ParameterDirectionEnum", "adoParameterDirectionEnum")*/
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4786)//忽略4786警告
//////////////////////////////////////////////////////////////////////////
#define WM_TOOLDLG_MSG WM_USER+100

typedef struct _struCarMsg
{
	UINT uIndex;
	bool isOnLine;
}CARMSG;

typedef struct _struiStrMsg
{
	UINT uIndex;
	CString strMsg;
}ISTRMSGZ;

typedef struct _struGNSSz
{
	UINT uIndex;
	double cx;
	double cy;
	double cr;
	double sd;
	double lc;
}GNSSMSGz;

typedef enum _enumCarCtrl
{
	MGNSSMSG,
	MCARMSG,
	MSHOWDLG,
	MINITCAR,	//初始化考车
	MTIPBOXSHOW,//提示框显示
	MTIPBOXHIDE, //提示框隐藏
	MZKZMBHCHANGE, //准考证明编号改变
	MXMZTCHANGE		//项目状态改变	
}EMCARCTRL;

void DrawStringz(Graphics *graphics,const TCHAR *str, const Font &font, const Brush &brush, const RectF &rect, StringFormat &fmt);
void DrawStringz(Graphics *graphics,int istr, const Font &font, const Brush &brush, const RectF &rect, StringFormat &fmt);	 


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5E75FEFA_59F5_4C08_B818_2AD82469F4AA__INCLUDED_)

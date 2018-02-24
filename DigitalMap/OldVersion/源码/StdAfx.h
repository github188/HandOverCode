// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__587A29EE_0814_4D33_84A9_3AEACEC08E02__INCLUDED_)
#define AFX_STDAFX_H__587A29EE_0814_4D33_84A9_3AEACEC08E02__INCLUDED_

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
#include <afxmt.h>
#include "SOCKET.h"


#define WM_BTN_MSGZ WM_USER+86
#define WM_BTN_MSGZLeft WM_USER+87
#define WM_BTN_MSG_CAR WM_USER+88

#define WM_TEXTMESSAGE  WM_USER + 100
#define WM_TEXTMESSAGET  WM_USER + 101
#define WM_TEXTMESSAGES  WM_USER + 102


/*ºöÂÔADOµÄ¾¯¸æ*/ 
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#pragma warning(disable: 4786)
#import "../../../3rdparty/ADO/msado15.dll" no_namespace rename("EOF", "adoEOF")
#include <icrsint.h> 

#pragma comment (lib,"vfw32.lib")
#pragma comment (lib,"Winmm.lib") 
#include "HCNetSDK.h"
#pragma comment(lib,"DsSdk.lib")
#pragma comment(lib,"GdiPlus.lib")
#pragma comment(lib,"HCNetSDK.lib")
#pragma comment(lib,"PlayCtrl.lib")


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__587A29EE_0814_4D33_84A9_3AEACEC08E02__INCLUDED_)

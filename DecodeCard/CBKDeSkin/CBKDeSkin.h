// CBKDeSkin.h : main header file for the CBKDESKIN DLL
//

#if !defined(AFX_CBKDESKIN_H__6CD2AD4F_C83C_4394_82FB_1363D7ABAC0F__INCLUDED_)
#define AFX_CBKDESKIN_H__6CD2AD4F_C83C_4394_82FB_1363D7ABAC0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCBKDeSkinApp
// See CBKDeSkin.cpp for the implementation of this class
//

class CCBKDeSkinApp : public CWinApp
{
public:
	UINT xmlinei[7];
	void DrawImageBK();
	void DrawImage17C56(UINT isave,UINT zt);
	void DrawImage17C55(UINT isave,UINT xmcode);
	void DrawImage17C53(UINT isave,UINT eline);
	void DrawImage17C52(UINT isave,UINT xmcode);
	void DrawImage17C51(UINT isave);
	CCBKDeSkinApp();
	//////////////////////////////////////////////////////////////////////////
	HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lpRect);
	BOOL SaveBmp(HBITMAP hBitmap, CString FileName);
	//////////////////////////////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKDeSkinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCBKDeSkinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKDESKIN_H__6CD2AD4F_C83C_4394_82FB_1363D7ABAC0F__INCLUDED_)

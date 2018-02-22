// CBKPlatSDKz.h : main header file for the CBKPLATSDKZ DLL
//

#if !defined(AFX_CBKPLATSDKZ_H__3A219DE8_B861_4564_A789_21CF96FA8724__INCLUDED_)
#define AFX_CBKPLATSDKZ_H__3A219DE8_B861_4564_A789_21CF96FA8724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Plat_SDKz.h"
/////////////////////////////////////////////////////////////////////////////
// CCBKPlatSDKzApp
// See CBKPlatSDKz.cpp for the implementation of this class
//


class CCBKPlatSDKzApp : public CWinApp
{
public:
	CPlat_SDKz m_PlatSDKz;
	void WriteLogz(LPCTSTR pstrFormat, ...);
	CCBKPlatSDKzApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKPlatSDKzApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCBKPlatSDKzApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString logfilenamez;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKPLATSDKZ_H__3A219DE8_B861_4564_A789_21CF96FA8724__INCLUDED_)

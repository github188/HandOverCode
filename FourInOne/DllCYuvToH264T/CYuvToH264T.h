// CYuvToH264T.h : main header file for the CYUVTOH264T DLL
//

#if !defined(AFX_CYUVTOH264T_H__49E9CB6F_F5F8_4059_9784_0465B20FB045__INCLUDED_)
#define AFX_CYUVTOH264T_H__49E9CB6F_F5F8_4059_9784_0465B20FB045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCYuvToH264TApp
// See CYuvToH264T.cpp for the implementation of this class
//
#include "ThirdPicJMQ.h"
#include "FourthPicJMQ.h"

#define MAX_CAR 510
class CCYuvToH264TApp : public CWinApp
{
public:
	DWORD dwSleepTime; 
	UINT uKSKM,uLoadMap,uIsqlororacle;
	CThirdPicJMQ m_TPic[MAX_CAR];
	CFourthPicJMQ m_FPic[MAX_CAR];
	CCYuvToH264TApp();
	CString exePath;//³ÌÐòÄ¿Â¼
	CString NowDataPath;//

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCYuvToH264TApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCYuvToH264TApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYUVTOH264T_H__49E9CB6F_F5F8_4059_9784_0465B20FB045__INCLUDED_)

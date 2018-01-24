#if !defined(AFX_CPLAYBACKZOCXCTL_H__30095A31_7515_4E39_97EE_D73551C566AF__INCLUDED_)
#define AFX_CPLAYBACKZOCXCTL_H__30095A31_7515_4E39_97EE_D73551C566AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <objsafe.h>
#include "PlayBackDlgz.h"	// Added by ClassView
// CPlayBackzOcxCtl.h : Declaration of the CCPlayBackzOcxCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl : See CPlayBackzOcxCtl.cpp for implementation.

class CCPlayBackzOcxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCPlayBackzOcxCtrl)
		//ISafeObject
		DECLARE_INTERFACE_MAP()
		
		BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)
		STDMETHOD_(HRESULT, GetInterfaceSafetyOptions) (
		REFIID riid,
		DWORD __RPC_FAR *pdwSupportedOptions,
		DWORD __RPC_FAR *pdwEnabledOptions
		);
	
	STDMETHOD_(HRESULT, SetInterfaceSafetyOptions) (
		REFIID riid,
		DWORD dwOptionSetMask,
		DWORD dwEnabledOptions
		);
	END_INTERFACE_PART(ObjSafe);
	//ISafeObject
	// Constructor
public:
	CCPlayBackzOcxCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlayBackzOcxCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCPlayBackzOcxCtrl();

	DECLARE_OLECREATE_EX(CCPlayBackzOcxCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCPlayBackzOcxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCPlayBackzOcxCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCPlayBackzOcxCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCPlayBackzOcxCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCPlayBackzOcxCtrl)
	afx_msg BOOL LoginDevice(LPCTSTR strIP, LPCTSTR strUser, LPCTSTR strPwd, short nPort, short nChannel);
	afx_msg BOOL SetPlayBackByTime(LPCTSTR strSTime, LPCTSTR strETime);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCPlayBackzOcxCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	CPlayBackDlgz m_DlgPlayBackz;
	enum {
	//{{AFX_DISP_ID(CCPlayBackzOcxCtrl)
	dispidLoginDevice = 1L,
	dispidSetPlayBackByTime = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKZOCXCTL_H__30095A31_7515_4E39_97EE_D73551C566AF__INCLUDED)

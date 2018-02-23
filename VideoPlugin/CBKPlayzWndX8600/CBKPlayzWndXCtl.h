#if !defined(AFX_CBKPLAYZWNDXCTL_H__01F95E6E_29B4_4B80_920B_7E70614DE467__INCLUDED_)
#define AFX_CBKPLAYZWNDXCTL_H__01F95E6E_29B4_4B80_920B_7E70614DE467__INCLUDED_

#include "ScreenPannel.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <objsafe.h> //IE安全接口 一
// CBKPlayzWndXCtl.h : Declaration of the CCBKPlayzWndXCtrl ActiveX Control class.
#include "LoadPlatSDK.h"
#include "LoadHikSDK.h"
/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl : See CBKPlayzWndXCtl.cpp for implementation.



//////////////////////////////////////////////////////////////////////////

class CCBKPlayzWndXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCBKPlayzWndXCtrl)
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
	CCBKPlayzWndXCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKPlayzWndXCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCBKPlayzWndXCtrl();

	DECLARE_OLECREATE_EX(CCBKPlayzWndXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCBKPlayzWndXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCBKPlayzWndXCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCBKPlayzWndXCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCBKPlayzWndXCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnThreadDLLMsg(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCBKPlayzWndXCtrl)
	afx_msg BOOL StopPlay();
	afx_msg void SetMultiWnd(short nSplit);
	afx_msg BOOL SetRealStreamID(LPCTSTR strKchp, LPCTSTR csCameraID);
	afx_msg void ShowRgppDlg(LPCTSTR strKchp, LPCTSTR strStuName, LPCTSTR strStuZkzm, LPCTSTR strReturn);
	afx_msg void SetDeviceInfo(LPCTSTR strIP, LPCTSTR strUser, LPCTSTR strPwd, short nPort, short nChannel, short nType);
	afx_msg BOOL SetPlayBackByTime(LPCTSTR strSTime, LPCTSTR strETime);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCBKPlayzWndXCtrl)
	void FireRenGongPingPan(LPCTSTR CarName, long XMCode)
		{FireEvent(eventidRenGongPingPan,EVENT_PARAM(VTS_BSTR  VTS_I4), CarName, XMCode);}
	void FireClickRgppMenu(LPCTSTR strKchp, long lWndId)
		{FireEvent(eventidClickRgppMenu,EVENT_PARAM(VTS_BSTR  VTS_I4), strKchp, lWndId);}
	void FireRClickMenu(LPCTSTR strKchp, short uWndID, short uParam)
		{FireEvent(eventidRClickMenu,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2), strKchp, uWndID, uParam);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	void MessageProc(UINT nWndID,WPARAM wParam,LPARAM lParam);
	enum {
	//{{AFX_DISP_ID(CCBKPlayzWndXCtrl)
	dispidStopPlay = 1L,
	dispidSetMultiWnd = 2L,
	dispidSetRealStreamID = 3L,
	dispidShowRgppDlg = 4L,
	dispidSetDeviceInfo = 5L,
	dispidSetPlayBackByTime = 6L,
	eventidRenGongPingPan = 1L,
	eventidClickRgppMenu = 2L,
	eventidRClickMenu = 3L,
	//}}AFX_DISP_ID
	};
private:
	//////////////////////////////////////////////////////////////////////////
	CString m_strKchp[16];//保存考车号牌
	CScreenPannel m_screenPannel;
	CRect m_screenRect;		//储存屏幕区域的位置
	//////////////////////////////////////////////////////////////////////////
//	CPlatSDKz m_PlatSDKz;		//海康8600平台
//	CHCNETSDKz m_HCNetSDKz;		
	CLoadPlatSDK m_LoadPlatSdk;	//海康8600平台
	CLoadHikSDK m_LoadHikSdk;//海康SDK
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKPLAYZWNDXCTL_H__01F95E6E_29B4_4B80_920B_7E70614DE467__INCLUDED)

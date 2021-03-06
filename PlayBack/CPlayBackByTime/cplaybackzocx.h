#if !defined(AFX_CPLAYBACKZOCX_H__B640E36F_080A_42C6_A8C9_59787082FCA4__INCLUDED_)
#define AFX_CPLAYBACKZOCX_H__B640E36F_080A_42C6_A8C9_59787082FCA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcx wrapper class

class CCPlayBackzOcx : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCPlayBackzOcx)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xa166bb8f, 0x89c7, 0x473f, { 0x9b, 0x15, 0x1c, 0x9e, 0x5e, 0x2f, 0xde, 0x67 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	BOOL LoginDevice(LPCTSTR strIP, LPCTSTR strUser, LPCTSTR strPwd, short nPort, short nChannel);
	BOOL SetPlayBackByTime(LPCTSTR strSTime, LPCTSTR strETime);
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKZOCX_H__B640E36F_080A_42C6_A8C9_59787082FCA4__INCLUDED_)

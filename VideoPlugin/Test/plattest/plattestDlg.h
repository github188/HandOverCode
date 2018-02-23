// plattestDlg.h : header file
//

#if !defined(AFX_PLATTESTDLG_H__1EA6621A_1CED_4EBD_B7D2_D6D77F847F2A__INCLUDED_)
#define AFX_PLATTESTDLG_H__1EA6621A_1CED_4EBD_B7D2_D6D77F847F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPlattestDlg dialog
//typedef void (CALLBACK *OnMessageProcFunc)(int nWndID, UINT message, DWORD dwUser);
#include "LoadPlatSDK.h"
class CPlattestDlg : public CDialog
{
// Construction
public:
	CLoadPlatSDK m_LoadPlatSdk;	//º£¿µ8600Æ½Ì¨
/*	typedef BOOL (* DPlayVideoPlat)(int ,long );
	typedef void (* DStopVideoPlat)(int );	
	typedef void (* DSetStreamCamer)(LPCTSTR ,int );
	typedef void (* DSetCallBack)(OnMessageProcFunc, DWORD);
	HINSTANCE dllPLATHIN;
	DPlayVideoPlat dllPlayVideoPlat;
	DStopVideoPlat dllStopVideoPlat;
	DSetStreamCamer dllSetStreamCamer;
	DSetCallBack dllSetCallBack;
	*/
	//////////////////////////////////////////////////////////////////////////

	void MessageProc(UINT nWndID,WPARAM wParam,LPARAM lParam);
	CPlattestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPlattestDlg)
	enum { IDD = IDD_PLATTEST_DIALOG };
	CString	m_Cid;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlattestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPlattestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATTESTDLG_H__1EA6621A_1CED_4EBD_B7D2_D6D77F847F2A__INCLUDED_)

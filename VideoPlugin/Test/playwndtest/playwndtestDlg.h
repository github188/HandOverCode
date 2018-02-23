// playwndtestDlg.h : header file
//
//{{AFX_INCLUDES()
#include "cbkplayzwndx.h"
//}}AFX_INCLUDES

#if !defined(AFX_PLAYWNDTESTDLG_H__93E5C496_5952_4A1F_80A8_9C5B42CD3C26__INCLUDED_)
#define AFX_PLAYWNDTESTDLG_H__93E5C496_5952_4A1F_80A8_9C5B42CD3C26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPlaywndtestDlg dialog

class CPlaywndtestDlg : public CDialog
{
// Construction
public:
	CPlaywndtestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPlaywndtestDlg)
	enum { IDD = IDD_PLAYWNDTEST_DIALOG };
	CString	m_IP;
	CString	m_User;
	CString	m_PWd;
	short	m_nPort;
	short	m_nChannel;
	CCBKPlayzWndX	m_PlayControl;
	CString	m_StrKchp;
	CString	m_strLID;
	CString	m_Stime;
	CString	m_Etime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaywndtestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPlaywndtestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnClickRgppMenuCbkplayzwndxctrl1(LPCTSTR strKchp, long lWndId);
	afx_msg void OnRClickMenuCbkplayzwndxctrl1(LPCTSTR strKchp, short uWndID, short uParam);
	afx_msg void OnRenGongPingPanCbkplayzwndxctrl1(LPCTSTR CarName, long XMCode);
	afx_msg void OnButton7();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYWNDTESTDLG_H__93E5C496_5952_4A1F_80A8_9C5B42CD3C26__INCLUDED_)

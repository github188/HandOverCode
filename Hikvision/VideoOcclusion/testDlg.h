// testDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__8D892F09_6C8D_4627_B33D_F39F14A3C637__INCLUDED_)
#define AFX_TESTDLG_H__8D892F09_6C8D_4627_B33D_F39F14A3C637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")
/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	
	LONG m_lPlayHandle;
	int itypeSPZD;
	DWORD dwTimeSPZD;
	CTestDlg(CWnd* pParent = NULL);	// standard constructor
	LONG lUserID;
	LONG lHandle;
// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CListCtrl	m_List1;
	CString	m_IP;
	CString	m_User;
	CString	m_PWD;
	CString	m_Edit4;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__8D892F09_6C8D_4627_B33D_F39F14A3C637__INCLUDED_)

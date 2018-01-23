// CVideoCXDlg.h : header file
//

#if !defined(AFX_CVIDEOCXDLG_H__0684EB5A_C37F_4B5B_8C82_F63D533C4969__INCLUDED_)
#define AFX_CVIDEOCXDLG_H__0684EB5A_C37F_4B5B_8C82_F63D533C4969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCVideoCXDlg dialog

class CCVideoCXDlg : public CDialog
{
// Construction
public:
	void WriteConfig();
	CString strBfStime,strBfEtime;
	CString strKCH;
	BOOL UpdateList(CString temp);
	_ConnectionPtr m_pConn;//数据库
	BOOL OnInitSQL();//初始化数据库连接s
	CCVideoCXDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCVideoCXDlg)
	enum { IDD = IDD_CVIDEOCX_DIALOG };
	CListCtrl	m_ListCtrl;
	CComboBox	m_ComBoTJ;
	CTime	m_StartTime;
	CTime	m_StopTime;
	CString	m_tiaoj;
	CString	m_strSM;
	CTime	m_StartTimes;
	CTime	m_StopTimes;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVideoCXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCVideoCXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton2();
	afx_msg void OnBtnCx();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPz();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIDEOCXDLG_H__0684EB5A_C37F_4B5B_8C82_F63D533C4969__INCLUDED_)

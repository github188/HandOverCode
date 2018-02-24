// KCHPPZDlg.h : header file
//

#if !defined(AFX_KCHPPZDLG_H__E4280E6F_48C9_4BE2_BA25_0FFBB7D08F3C__INCLUDED_)
#define AFX_KCHPPZDLG_H__E4280E6F_48C9_4BE2_BA25_0FFBB7D08F3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKCHPPZDlg dialog

class CKCHPPZDlg : public CDialog
{
// Construction
public:
	CKCHPPZDlg(CWnd* pParent = NULL);	// standard constructor
	//数据库
	_ConnectionPtr m_pConn;//数据库
	BOOL OnInitSQL();//初始化数据库连接
	CStringArray ComboboxArray;
	BOOL UpdateCombox();
	BOOL UpdateList();
// Dialog Data
	//{{AFX_DATA(CKCHPPZDlg)
	enum { IDD = IDD_KCHPPZ_DIALOG };
	CListCtrl	m_ListCtrl;
	CComboBox	m_ComboBoxBH;
	CString	m_EditKCHP;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKCHPPZDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKCHPPZDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KCHPPZDLG_H__E4280E6F_48C9_4BE2_BA25_0FFBB7D08F3C__INCLUDED_)

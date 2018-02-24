// test2Dlg.h : header file
//

#if !defined(AFX_TEST2DLG_H__A1523DEB_AFCF_4F2A_9FA9_38E7E69EAE46__INCLUDED_)
#define AFX_TEST2DLG_H__A1523DEB_AFCF_4F2A_9FA9_38E7E69EAE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib,"HCNetSDK.lib")
#pragma comment(lib, "ws2_32.lib")

/////////////////////////////////////////////////////////////////////////////
// CTest2Dlg dialog

class CTest2Dlg : public CDialog
{
// Construction
public:
	CTest2Dlg(CWnd* pParent = NULL);	// standard constructor
	BOOL Oninittext();
	void OinitextSecond(CString m_num);
	LONG lUserID;
	int m_Row;
	int m_Col;
	void SetListItemText(void) ;
// Dialog Data
	//{{AFX_DATA(CTest2Dlg)
	enum { IDD = IDD_TEST2_DIALOG };
	CComboBox	m_boxthree;
	CComboBox	m_boxtwo;
	CComboBox	m_boxone;
	CEdit	m_Secedit;
	CIPAddressCtrl	m_mIP;
	CListCtrl	m_SecList;
	CListCtrl	m_JLLlist;
	CString	m_user;
	CString	m_pwd;
	CString	m_port;
	CString	m_num;
	CString	m_carnum;
	CString	m_passway;
	CString	m_total;
	int		m_play1;
	int		m_play2;
	int		m_play3;
	int		m_play4;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTest2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void ButtonSave();
	afx_msg void OnResearch();
	afx_msg void OnDelete();
	afx_msg void OnConnectBUTTON();
	afx_msg void OnClickLISTSecond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCarAddButton();
	afx_msg void OnCarDeleteBUTTON();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLISTSecond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusSecEDIT();
	afx_msg void OnBtnpz();
	afx_msg void OnSelchangeComboOne();
	afx_msg void OnSelchangeComboTwo();
	afx_msg void OnSelchangeComboThree();
	afx_msg void OnChangeEDITplay1();
	afx_msg void OnChangeEDITplay2();
	afx_msg void OnChangeEDITplay3();
	afx_msg void OnChangeEDITplay4();
	afx_msg void OnSaveplayBUTTON();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST2DLG_H__A1523DEB_AFCF_4F2A_9FA9_38E7E69EAE46__INCLUDED_)

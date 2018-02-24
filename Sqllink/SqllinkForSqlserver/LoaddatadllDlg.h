// LoaddatadllDlg.h : header file
//

#if !defined(AFX_LOADDATADLLDLG_H__1DDD7323_70D7_4367_87B7_0143E6D4C26E__INCLUDED_)
#define AFX_LOADDATADLLDLG_H__1DDD7323_70D7_4367_87B7_0143E6D4C26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLoaddatadllDlg dialog

class CLoaddatadllDlg : public CDialog
{
// Construction
public:
	CLoaddatadllDlg(CWnd* pParent = NULL);	// standard constructor
	CString SQLdatabase;
	BOOL isfrist;
	CString Decode(CString str);
	CString Encode(CString str);
// Dialog Data
	//{{AFX_DATA(CLoaddatadllDlg)
	enum { IDD = IDD_LOADDATADLL_DIALOG };
	CComboBox	m_Combo;
	CString	m_ESer;
	CString	m_EUser;
	CString	m_EPwd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaddatadllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoaddatadllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonlz();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButtonjm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADDATADLLDLG_H__1DDD7323_70D7_4367_87B7_0143E6D4C26E__INCLUDED_)

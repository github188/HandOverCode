// CDownMapFDBDlg.h : header file
//

#if !defined(AFX_CDOWNMAPFDBDLG_H__E6FFBDED_854D_41A3_9DEE_39A41A1579C5__INCLUDED_)
#define AFX_CDOWNMAPFDBDLG_H__E6FFBDED_854D_41A3_9DEE_39A41A1579C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CCDownMapFDBDlg dialog

class CCDownMapFDBDlg : public CDialog
{
// Construction
public:

	CCDownMapFDBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCDownMapFDBDlg)
	enum { IDD = IDD_CDOWNMAPFDB_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDownMapFDBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCDownMapFDBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strKcdm;
	CString m_MapFileName;
	CString m_MapConfig;
	//数据库
	_ConnectionPtr m_pConn;//数据库
	BOOL OnInitSQL();//初始化数据库连接

	void GetKchp(CString strkcdm);
	void SetKCMC(CString strkcdm);
	BOOL DownMapFromDB(CString strKCDM);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDOWNMAPFDBDLG_H__E6FFBDED_854D_41A3_9DEE_39A41A1579C5__INCLUDED_)

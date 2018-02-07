// CBKUDPSDTestDlg.h : header file
//

#if !defined(AFX_CBKUDPSDTESTDLG_H__BB9F940D_661D_41D7_A1E7_FC2C8ADD6F92__INCLUDED_)
#define AFX_CBKUDPSDTESTDLG_H__BB9F940D_661D_41D7_A1E7_FC2C8ADD6F92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCBKUDPSDTestDlg dialog

class CCBKUDPSDTestDlg : public CDialog
{
// Construction
public:
	CCBKUDPSDTestDlg(CWnd* pParent = NULL);	// standard constructor

	HINSTANCE dllHandleTCP;
	typedef BOOL (* DLLInitSocket)(HWND);
	DLLInitSocket dllInitSocket;
	
	typedef BOOL (* DLLSendString)(UINT,LPTSTR);
	DLLSendString dllsendstring;
// Dialog Data
	//{{AFX_DATA(CCBKUDPSDTestDlg)
	enum { IDD = IDD_CBKUDPSDTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKUDPSDTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCBKUDPSDTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKUDPSDTESTDLG_H__BB9F940D_661D_41D7_A1E7_FC2C8ADD6F92__INCLUDED_)

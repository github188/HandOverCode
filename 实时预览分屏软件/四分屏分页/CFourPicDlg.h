// CFourPicDlg.h : header file
//

#if !defined(AFX_CFOURPICDLG_H__09F8543B_5086_4615_9DD4_C52B9A21A9B5__INCLUDED_)
#define AFX_CFOURPICDLG_H__09F8543B_5086_4615_9DD4_C52B9A21A9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PLAYWND.h"
/////////////////////////////////////////////////////////////////////////////
// CCFourPicDlg dialog


class CCFourPicDlg : public CDialog
{
// Construction
public:
	int isSqlOrOralce;
	int nowPageI,PageCounti;
	CString KSCYstr;
	void OnCountHGL();
	int OnSelectCount(CString str);
	BOOL OnInitSQL();
	_ConnectionPtr m_pConn;
	BOOL GetMyHostIP();
	void OnSelectSQL(CString strIP);
	HBRUSH   hbr1;
	CCFourPicDlg(CWnd* pParent = NULL);	// standard constructor
	CPLAYWND myPlaywnd[6];
// Dialog Data
	//{{AFX_DATA(CCFourPicDlg)
	enum { IDD = IDD_CFOURPIC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFourPicDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCFourPicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnStartAllVideo(WPARAM wParam,LPARAM lParam);
	afx_msg void OnRGPPDLL(WPARAM wParam,LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBTNPageDown();
	afx_msg void OnBTNPageUP();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect CliRc;
	POINT CliRcMin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFOURPICDLG_H__09F8543B_5086_4615_9DD4_C52B9A21A9B5__INCLUDED_)

// CBKVideoDeCodeDlg.h : header file
//

#if !defined(AFX_CBKVIDEODECODEDLG_H__F7E01FFB_0797_4AD1_8EB1_DF48608C132D__INCLUDED_)
#define AFX_CBKVIDEODECODEDLG_H__F7E01FFB_0797_4AD1_8EB1_DF48608C132D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BKJieMaz.h"
#include "TcpSockKm3.h"
#pragma comment(lib,"KM3VideoTcp.lib")
/////////////////////////////////////////////////////////////////////////////
// CCBKVideoDeCodeDlg dialog

class CCBKVideoDeCodeDlg : public CDialog
{
// Construction
public:
	CCBKVideoDeCodeDlg(CWnd* pParent = NULL);	// standard constructor
	CBKJieMaz jiemak;	
	CTcpSockKm3 mysockz;//套按字服务器
	void OnReadSock();//数据处理
	void OnMenuExit();
	void OnMenuShow();
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CCBKVideoDeCodeDlg)
	enum { IDD = IDD_CBKVIDEODECODE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKVideoDeCodeDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	NOTIFYICONDATA m_NotifyIcon;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCBKVideoDeCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSock(WPARAM,LPARAM);//TCP
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKVIDEODECODEDLG_H__F7E01FFB_0797_4AD1_8EB1_DF48608C132D__INCLUDED_)

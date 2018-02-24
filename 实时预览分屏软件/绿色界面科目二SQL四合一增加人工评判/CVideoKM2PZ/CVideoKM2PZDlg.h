// CVideoKM2PZDlg.h : header file
//

#if !defined(AFX_CVIDEOKM2PZDLG_H__77E450A3_783C_4067_8CBF_02ADADDBCD81__INCLUDED_)
#define AFX_CVIDEOKM2PZDLG_H__77E450A3_783C_4067_8CBF_02ADADDBCD81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ZSZButton.h"
#include "ListCtrlz.h"
#pragma comment(lib,"CMFC5286D.lib")
/////////////////////////////////////////////////////////////////////////////
// CCVideoKM2PZDlg dialog

class CCVideoKM2PZDlg : public CDialog
{
// Construction
public:
	int m_Btnshowid;
	void OnShowMenu(int showid);
	CCVideoKM2PZDlg(CWnd* pParent = NULL);	// standard constructor
	typedef BOOL (* MessageBoxbmp)(LPTSTR );
	MessageBoxbmp zbmpmessagebox;
	HINSTANCE dllHandle;
	typedef BOOL (* DLGPZVideo)();
	DLGPZVideo pzvideodlg;

	//////////////////////////////////////////////////////////////////////////
	//加个菜单
	CMenu m_menu;
	CStringArray m_menuItem;
	int IDArray[100];//最多支持100项
	//////////////////////////////////////////////////////////////////////////
	BOOL OnInitSQL();
	_ConnectionPtr m_pConn;
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CCVideoKM2PZDlg)
	enum { IDD = IDD_CVIDEOKM2PZ_DIALOG };
	CZSZButton	m_BtnPZ;
	CZSZButton	m_BtnFinsh;
	CZSZButton	m_BTNREFRESH;
	CListCtrlz	m_ListCtrlKCH;
	CZSZButton	m_BTNClose;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCVideoKM2PZDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCVideoKM2PZDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBtnExit();
	afx_msg void OnBtnPic1();
	afx_msg void OnBtnPic2();
	afx_msg void OnBtnPic3();
	afx_msg void OnBtnPic4();
	afx_msg void OnBtnfinsh();
	afx_msg void OnBtnRefresh();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMenuSelect(UINT nID);
	afx_msg void OnupdateMenu(CCmdUI *pCmdUI);
	afx_msg void OnBTNPZVideo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIDEOKM2PZDLG_H__77E450A3_783C_4067_8CBF_02ADADDBCD81__INCLUDED_)

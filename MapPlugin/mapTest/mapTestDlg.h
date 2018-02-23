// mapTestDlg.h : header file
//
//{{AFX_INCLUDES()
#include "cbkmap.h"
//}}AFX_INCLUDES

#if !defined(AFX_MAPTESTDLG_H__EDFD5F39_3256_4526_97FE_46E0D7AF08B8__INCLUDED_)
#define AFX_MAPTESTDLG_H__EDFD5F39_3256_4526_97FE_46E0D7AF08B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMapTestDlg dialog

class CMapTestDlg : public CDialog
{
// Construction
public:
	CMapTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMapTestDlg)
	enum { IDD = IDD_MAPTEST_DIALOG };
	CCBKMap	m_map;
	CString	m_kcdm;
	CString	m_date;
	CString	m_zkzm;
	short	m_kscs;
	short	m_drcs;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMapTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPTESTDLG_H__EDFD5F39_3256_4526_97FE_46E0D7AF08B8__INCLUDED_)

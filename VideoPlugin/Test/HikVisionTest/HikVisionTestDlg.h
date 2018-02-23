// HikVisionTestDlg.h : header file
//

#if !defined(AFX_HIKVISIONTESTDLG_H__EB0B1E7D_4602_4B48_97AE_8FE81ACB580F__INCLUDED_)
#define AFX_HIKVISIONTESTDLG_H__EB0B1E7D_4602_4B48_97AE_8FE81ACB580F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LoadHikSDK.h"
/////////////////////////////////////////////////////////////////////////////
// CHikVisionTestDlg dialog
#define MSG_HIKSDK WM_USER+214
class CHikVisionTestDlg : public CDialog
{
// Construction
public:
	CLoadHikSDK m_LodaHikSdk;
	CHikVisionTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHikVisionTestDlg)
	enum { IDD = IDD_HIKVISIONTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHikVisionTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHikVisionTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIKVISIONTESTDLG_H__EB0B1E7D_4602_4B48_97AE_8FE81ACB580F__INCLUDED_)

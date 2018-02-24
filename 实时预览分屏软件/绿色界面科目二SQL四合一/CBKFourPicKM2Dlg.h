// CBKFourPicKM2Dlg.h : header file
//

#if !defined(AFX_CBKFOURPICKM2DLG_H__C3354B47_FB36_42F1_B410_DD77D9B77AFF__INCLUDED_)
#define AFX_CBKFOURPICKM2DLG_H__C3354B47_FB36_42F1_B410_DD77D9B77AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DlgPlayWnd.h"
#define WND_NUM 4
/////////////////////////////////////////////////////////////////////////////
// CCBKFourPicKM2Dlg dialog

class CCBKFourPicKM2Dlg : public CDialog
{
// Construction
public:
	CCBKFourPicKM2Dlg(CWnd* pParent = NULL);	// standard constructor
	CDlgPlayWnd PlayWnd[WND_NUM];
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CCBKFourPicKM2Dlg)
	enum { IDD = IDD_CBKFOURPICKM2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKFourPicKM2Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCBKFourPicKM2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnButtonDown(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect CliRc;
	POINT CliRcMin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKFOURPICKM2DLG_H__C3354B47_FB36_42F1_B410_DD77D9B77AFF__INCLUDED_)

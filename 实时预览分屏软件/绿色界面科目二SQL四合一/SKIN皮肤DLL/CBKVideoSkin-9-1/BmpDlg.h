#if !defined(AFX_BMPDLG_H__2A49CCF2_0D8A_4974_85BA_3A5AA8EB195F__INCLUDED_)
#define AFX_BMPDLG_H__2A49CCF2_0D8A_4974_85BA_3A5AA8EB195F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpDlg dialog
#include "ZSZButton.h"

class CBmpDlg : public CDialog
{
// Construction
public:
	CBmpDlg(CWnd* pParent = NULL);   // standard constructor
	CString dlgMessage;
// Dialog Data
	//{{AFX_DATA(CBmpDlg)
	enum { IDD = IDD_DLG_MESSAGE };
	CZSZButton	m_BIDO;
	CZSZButton	m_BIDC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT lineNum;//ÐÐÊý
	UINT LineHeight;
	CStringArray msg_dest;
	void DoHaoCString(CString source,  CStringArray& dest, char division);
	// Generated message map functions
	//{{AFX_MSG(CBmpDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPDLG_H__2A49CCF2_0D8A_4974_85BA_3A5AA8EB195F__INCLUDED_)

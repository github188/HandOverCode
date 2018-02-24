#if !defined(AFX_DLGXMCHANGE_H__97876E8C_4646_4295_BF92_A065F11BF27E__INCLUDED_)
#define AFX_DLGXMCHANGE_H__97876E8C_4646_4295_BF92_A065F11BF27E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgXMChange.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgXMChange dialog

class CDlgXMChange : public CDialog
{
// Construction
public:
	HBRUSH   hbr1;
	CDlgXMChange(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgXMChange)
	enum { IDD = IDD_DLG_XM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgXMChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgXMChange)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBTNSelect(UINT nID);
};
//	afx_msg void OnBTNSelect(UINT nID);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGXMCHANGE_H__97876E8C_4646_4295_BF92_A065F11BF27E__INCLUDED_)

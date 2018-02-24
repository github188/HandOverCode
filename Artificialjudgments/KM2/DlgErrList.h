#if !defined(AFX_DLGERRLIST_H__28E2EFA8_77CA_4A53_AE54_89472254B61C__INCLUDED_)
#define AFX_DLGERRLIST_H__28E2EFA8_77CA_4A53_AE54_89472254B61C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgErrList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgErrList dialog

class CDlgErrList : public CDialog
{
// Construction
public:


	HBRUSH   hbr1;
	CString m_GAWDM;
	void SetXMCode(int icode);
	CDlgErrList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgErrList)
	enum { IDD = IDD_DLG_ELIST };
	CButtonz	m_BtnKF;
	CListCtrl	m_ListError;
	CString	m_AddMSG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgErrList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgErrList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnReturn();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnItemchangedErrorList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnCf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGERRLIST_H__28E2EFA8_77CA_4A53_AE54_89472254B61C__INCLUDED_)

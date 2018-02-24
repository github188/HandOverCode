#if !defined(AFX_RGPPKM3_H__24CFA2DB_9EC3_4B94_8E9F_9CA2F910DEF5__INCLUDED_)
#define AFX_RGPPKM3_H__24CFA2DB_9EC3_4B94_8E9F_9CA2F910DEF5__INCLUDED_

#include "DlgXMChange.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RgppKm3.h : header file
//
#include "DlgErrList.h"
/////////////////////////////////////////////////////////////////////////////
// CRgppKm3 dialog

class CRgppKm3 : public CDialog
{
// Construction
public:
	int m_ixmcode;
	void SetXMCodez(LPTSTR strCar,LPTSTR strName,LPCTSTR strNum,int ixmcode);
	void SetMsg(LPTSTR strCar,LPTSTR strName,LPCTSTR strNum);
	CRgppKm3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRgppKm3)
	enum { IDD = IDD_DLGRGPP };
	CButtonzEx	m_BtnExit;
	CStaticz	m_CT3;
	CStaticz	m_CT2;
	CStaticz	m_CT1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRgppKm3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRgppKm3)
	afx_msg void OnBtnReturn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnXMDlgMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnELDlgMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDlgXMChange dlgxm;
	CDlgErrList  dlgerrlist;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RGPPKM3_H__24CFA2DB_9EC3_4B94_8E9F_9CA2F910DEF5__INCLUDED_)

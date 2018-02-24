#if !defined(AFX_LISTCTRLZ_H__EB78BF1A_4364_4F88_ABB0_C180D3E661F1__INCLUDED_)
#define AFX_LISTCTRLZ_H__EB78BF1A_4364_4F88_ABB0_C180D3E661F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlz.h : header file
//
#include "HeaderCtrlz.h"
/////////////////////////////////////////////////////////////////////////////
// CListCtrlz window

class AFX_EXT_CLASS CListCtrlz : public CListCtrl
{
// Construction
public:
	CListCtrlz();
	CHeaderCtrlz m_HeaderCtrlz;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlz)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetHeadColor(COLORREF bkcolor,COLORREF textcolor,COLORREF linecolor);
	void SetLineColor(COLORREF linecolor,COLORREF linecolor2=RGB(252,252,252));
	virtual ~CListCtrlz();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlz)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_LineColor;
	COLORREF m_LineColortwo;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLZ_H__EB78BF1A_4364_4F88_ABB0_C180D3E661F1__INCLUDED_)

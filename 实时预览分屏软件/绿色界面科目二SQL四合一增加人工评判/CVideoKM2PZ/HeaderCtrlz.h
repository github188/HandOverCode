#if !defined(AFX_HEADERCTRLZ_H__4EE18B60_CCBB_4C74_B390_8CF05569A8E7__INCLUDED_)
#define AFX_HEADERCTRLZ_H__4EE18B60_CCBB_4C74_B390_8CF05569A8E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlz window

class CHeaderCtrlz : public CHeaderCtrl
{
// Construction
public:
	CHeaderCtrlz();

// Attributes
public:
	COLORREF m_bkcolor;
	COLORREF m_textColor;
	COLORREF m_linecolor;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlz)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CHeaderCtrlz();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlz)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERCTRLZ_H__4EE18B60_CCBB_4C74_B390_8CF05569A8E7__INCLUDED_)

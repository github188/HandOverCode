#if !defined(AFX_STATICZEX_H__6CEF764F_B194_4988_981C_9BB21CE209A1__INCLUDED_)
#define AFX_STATICZEX_H__6CEF764F_B194_4988_981C_9BB21CE209A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticzEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticzEx window

class CStaticzEx : public CStatic
{
// Construction
public:
	CStaticzEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticzEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_iTypeFont;
	void SetXmlbColor(UINT uType);
	void SetTextnFormat(UINT nfm);//设置字体掩码 123 左 中 右
	void SetCaptionSW(CString strCaption);//刷新
	void SetTextColor(COLORREF TextColor);//字体颜色
	void SetBkColor(COLORREF bkColor,BOOL isbkcolor=TRUE);//背景颜色
	void SetCaption(CString strCaption);
	void SetCaption(CString strCaption,COLORREF TextColor);
	virtual ~CStaticzEx();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticzEx)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CString m_strCaption;
	COLORREF m_TextColor;
	COLORREF m_BackColor;
	UINT nFormat;
	BOOL m_isBkcolor;
	UINT m_typee;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICZEX_H__6CEF764F_B194_4988_981C_9BB21CE209A1__INCLUDED_)

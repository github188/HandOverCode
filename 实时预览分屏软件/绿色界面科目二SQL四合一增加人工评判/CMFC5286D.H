// MFC5286D.h: interface for the CMFC5286D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFC5286D_H__FF81F8AC_DB69_4867_A88C_ED8908B1102B__INCLUDED_)
#define AFX_MFC5286D_H__FF81F8AC_DB69_4867_A88C_ED8908B1102B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Buttonz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonz window

class AFX_EXT_CLASS CButtonz : public CBitmapButton
{
	// Construction
public:
	CButtonz();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonz)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	BOOL LoadBitmap(LPCTSTR lpszName);
	BOOL LoadBitmap(UINT bitmapid);
	virtual ~CButtonz();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonz)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl* m_pToolTip;
	CString		  m_tooltext;
	
	CSize m_ButtonSize;//按钮尺寸
	CBitmap mybitmap;  //保存图片的变量
	
	//按钮的状态
	BOOL m_bTracking;  //按钮是否跟踪到鼠标
	BOOL m_bHover;     //指示鼠标是否在按钮上面
	// 	BOOL m_bSelected;	//按钮被按下是该值为true
	// 	BOOL m_bFocus;	    //按钮为当前焦点所在时该值为true
};

/////////////////////////////////////////////////////////////////////////////
// ButtonzEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonzEx window

class AFX_EXT_CLASS CButtonzEx : public CBitmapButton
{
	// Construction
public:
	CButtonzEx();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonzEx)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void SetBtnDrawNum(UINT bmpNormal, UINT bmpHover, UINT bmpFocus, UINT bmpDown, UINT bmpDisable);
	UINT m_bmpn;
	UINT m_BdrawX[5];
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	BOOL LoadBitmap(LPCTSTR lpszName,UINT bmpn = 3);
	BOOL LoadBitmap(UINT bitmapid,UINT bmpn = 3);
	virtual ~CButtonzEx();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonzEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl* m_pToolTip;
	CString		  m_tooltext;
	
	CSize m_ButtonSize;//按钮尺寸
	CBitmap mybitmap;  //保存图片的变量
	//按钮的状态
	BOOL m_bTracking;  //按钮是否跟踪到鼠标
	BOOL m_bHover;     //指示鼠标是否在按钮上面
};

/////////////////////////////////////////////////////////////////////////////
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
// ListCtrlz.h : header file
//
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
// Staticz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticz window

class AFX_EXT_CLASS CStaticz : public CStatic
{
	// Construction
public:
	CStaticz();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticz)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void SetTextnFormat(UINT nfm);//设置字体掩码 123 左 中 右
	void SetCaptionSW(CString strCaption);//刷新
	void SetTextColor(COLORREF TextColor);//字体颜色
	void SetBkColor(COLORREF bkColor,BOOL isbkcolor=TRUE);//背景颜色
	void SetCaption(CString strCaption);
	void SetCaption(CString strCaption,COLORREF TextColor);
	virtual ~CStaticz();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticz)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
private:
	CString m_strCaption;
	COLORREF m_TextColor;
	COLORREF m_BackColor;
	UINT nFormat;
	BOOL m_isBkcolor;
};

/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_MFC5286D_H__FF81F8AC_DB69_4867_A88C_ED8908B1102B__INCLUDED_)

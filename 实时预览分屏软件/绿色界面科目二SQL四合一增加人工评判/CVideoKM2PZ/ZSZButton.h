#if !defined(AFX_ZSZBUTTON_H__85F7B95C_7A64_49B6_AE3D_EE12B79A9304__INCLUDED_)
#define AFX_ZSZBUTTON_H__85F7B95C_7A64_49B6_AE3D_EE12B79A9304__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZSZButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZSZButton window

class CZSZButton : public CBitmapButton
{
// Construction
public:
	CZSZButton();

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZSZButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadBitmap(UINT bitmapid);


	CSize m_ButtonSize;//按钮尺寸
	CBitmap mybitmap;  //保存图片的变量

	//按钮的状态
	BOOL m_bTracking;  //按钮是否跟踪到鼠标
	BOOL m_bHover;     //指示鼠标是否在按钮上面
// 	BOOL m_bSelected;	//按钮被按下是该值为true
// 	BOOL m_bFocus;	    //按钮为当前焦点所在时该值为true
	virtual ~CZSZButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CZSZButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZSZBUTTON_H__85F7B95C_7A64_49B6_AE3D_EE12B79A9304__INCLUDED_)

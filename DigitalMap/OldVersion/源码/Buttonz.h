#if !defined(AFX_BUTTONZ_H__8665D7B3_3FE2_462C_A7D0_6168DCFB6A65__INCLUDED_)
#define AFX_BUTTONZ_H__8665D7B3_3FE2_462C_A7D0_6168DCFB6A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Buttonz.h : header file
//
//#include "DD.h"
/////////////////////////////////////////////////////////////////////////////
// CButtonz window

class  CButtonz : public CBitmapButton
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
	int CurCar_Xpoint; //当前的考车坐标经纬度
	int CurCar_Ypoint;
	UINT CurCar_ZT;
	UINT CurCar_EID;

	UINT ThreadNumber;  //考车对应的线程号  把这个变量放在 UINT examCarNumber;  //考车号  变量后面定义，效果会不一样
	//  出现的效果是刷新当前的考车号会不显示出来  在这里就不会了  不知道为什么
	int isCircle;//是否处于中心点 1 为是 0 为不是
	int ExamCarState;  //考车状态  在线（1） 还是离线（0）
	DOUBLE Cur_yPoint;  //当前按钮x坐标
	DOUBLE Cur_xPoint;  //当前按钮y坐标


	DOUBLE Cur_yPointRelyOnParent;  //当前按钮x坐标  相对于父对话框
	DOUBLE Cur_xPointRelyOnParent;  //当前按钮y坐标
	BOOL bTrack;//代表这辆考车是否被跟踪 TRUE 被跟踪 FALSE 没被跟踪

	UINT examCarNumber;  //考车号
	
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl* m_pToolTip;
	CString		  m_tooltext;

	CSize m_ButtonSize;//按钮尺寸
	CBitmap mybitmap;  //保存图片的变量

public:	
	void WriteLog(CString szERR);
	//按钮的状态
	BOOL m_bTracking;  //按钮是否跟踪到鼠标
	BOOL m_bHover;     //指示鼠标是否在按钮上面
	// 	BOOL m_bSelected;	//按钮被按下是该值为true
	// 	BOOL m_bFocus;	    //按钮为当前焦点所在时该值为true
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONZ_H__8665D7B3_3FE2_462C_A7D0_6168DCFB6A65__INCLUDED_)

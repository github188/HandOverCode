#if !defined(AFX_EXAMCARINFODLG_H__1CA81F05_06E4_47FD_BD05_170D47B07A8D__INCLUDED_)
#define AFX_EXAMCARINFODLG_H__1CA81F05_06E4_47FD_BD05_170D47B07A8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExamCarInfoDlg.h : header file
//
#include "ExamCarDlg.h"
#include "DetailsDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CExamCarInfoDlg dialog

class CExamCarInfoDlg : public CDialog
{
// Construction
public:
	int MouseIsThisRect;  //判断鼠标是否是在这个区域 默认为不在这个区域（0）  如果是在这个区域则为1
	int ShowDlgState;  //显示对话框的状态标志位  为1时 将对话框全部显示出来(正在显示出来) 为0时将对话框只显示一部分(正在收回去) 相当于只显示底边 当为2时表示伸张或收缩结束 
	CPoint StartDlgPoint; //开始显示的点 
	CPoint EndDlgPoint;//结束显示的点
	CBitmap bmpBackground;
	CExamCarInfoDlg(CWnd* pParent = NULL);   // standard constructor
	CDetailsDlg DetailsDlg; //详情类
	CExamCarDlg ExamCarDlg;//考车类  ExamCarInfo.ExamCarDlg

// Dialog Data
	//{{AFX_DATA(CExamCarInfoDlg)
	enum { IDD = IDD_DIALOG_ONLINECARINFO };
	CTabCtrl	m_ctrlTab;
	CStatic	m_Time;
	CStatic	m_Date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExamCarInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExamCarInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeTabOption(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonmain();
	afx_msg void OnButtoncopilot();
	afx_msg void OnButtonoutside();
	afx_msg void OnButtonall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CBitmap m_bmpMap;
	CDC	 m_dcMem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMCARINFODLG_H__1CA81F05_06E4_47FD_BD05_170D47B07A8D__INCLUDED_)

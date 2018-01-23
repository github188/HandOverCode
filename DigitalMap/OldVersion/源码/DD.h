#if !defined(AFX_DD_H__560E327A_044D_4DDD_9319_3129DAA133ED__INCLUDED_)
#define AFX_DD_H__560E327A_044D_4DDD_9319_3129DAA133ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDD dialog

class CDD : public CDialog
{
	// Construction
public:
	CString strCarModel;  //考试车型  
	CString strzkzm;  // 准考证明编号
	void OnShowPHO(); // 将考生的照片贴上去
	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc);  //读取考生信息 --照片
	CString szExamineeName;  //考生姓名
	BOOL ReadExamInfoFromDB(CDC *pDC, CString sCard, CRect *rc);
	CDD(CWnd* pParent = NULL);   // standard constructor
	void SetupRegion(CDC *pDC, CBitmap &cBitmap, COLORREF TransColor);
//	CBitmap  m_bmp;
	CString CSCAR;
	// Dialog Data
	//{{AFX_DATA(CDD)
	enum { IDD = IDD_DIALOG1 };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDD)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDD)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButton1();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  //将控件的背景改为透明
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DD_H__3D6C9338_982E_441C_B341_C3B87461ED15__INCLUDED_)

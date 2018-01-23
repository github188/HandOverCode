#if !defined(AFX_DETAILSDLG_H__FE2A24DE_6245_4DED_85E8_F515A0BBC110__INCLUDED_)
#define AFX_DETAILSDLG_H__FE2A24DE_6245_4DED_85E8_F515A0BBC110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg dialog
// 考车的详情类 用于对话框
class CDetailsDlg : public CDialog
{
// Construction
public:
	BOOL ReadExamInfoFromDB(CDC *pDC, CString sCard, CRect *rc,char *pzkzh);
	HTREEITEM m_hDevItem;
	CImageList *m_imagelist;
	BOOL InitMytree();
	CDetailsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailsDlg)
	enum { IDD = IDD_DIALOG_DETAILS };
	CTreeCtrl	m_details;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILSDLG_H__FE2A24DE_6245_4DED_85E8_F515A0BBC110__INCLUDED_)

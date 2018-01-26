#if !defined(AFX_YLDLG_H__A98ACB45_A96D_4AAC_BDD4_D8197912FA3D__INCLUDED_)
#define AFX_YLDLG_H__A98ACB45_A96D_4AAC_BDD4_D8197912FA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YLDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYLDlg dialog

class CYLDlg : public CDialog
{
// Construction
public:
	int dx,dy;
// 	CString zkzmbhm;
// 	int nKSCS;
// 	CString m_ESer,m_EUser,m_EPwd;
	CYLDlg(CWnd* pParent = NULL);   // standard constructor
	CBitmap m_bmpMap;
	CDC	 m_dcMem;
// Dialog Data
	//{{AFX_DATA(CYLDlg)
	enum { IDD = IDD_DLGYL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYLDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YLDLG_H__A98ACB45_A96D_4AAC_BDD4_D8197912FA3D__INCLUDED_)

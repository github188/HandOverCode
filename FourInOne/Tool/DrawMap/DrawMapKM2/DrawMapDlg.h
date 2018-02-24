// DrawMapDlg.h : header file
//

#if !defined(AFX_DRAWMAPDLG_H__9D10DED3_24EE_4BFB_9361_9C1C022C8C0A__INCLUDED_)
#define AFX_DRAWMAPDLG_H__9D10DED3_24EE_4BFB_9361_9C1C022C8C0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDrawMapDlg dialog

class CDrawMapDlg : public CDialog
{
// Construction
public:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	CDrawMapDlg(CWnd* pParent = NULL);	// standard constructor
	int m_BSb;
// Dialog Data
	//{{AFX_DATA(CDrawMapDlg)
	enum { IDD = IDD_DRAWMAP_DIALOG };
	CString	m_GpsFilePath;
	double	m_ebs;
	double	m_edh;
	double	m_edw;
	double	m_emaxx;
	double	m_emaxy;
	double	m_eminx;
	double	m_eminy;
	BYTE	m_CB;
	BYTE	m_CG;
	BYTE	m_CR;
	BYTE	m_CSIZE;
	BYTE	m_ER2;
	BYTE	m_EG2;
	BYTE	m_EB2;
	BYTE	m_ESIZE2;
	CString	m_TText;
	int		m_Ejwdbs;
	BOOL	m_Check;
	BOOL	m_Check2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDrawMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoadFile();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton8();
	afx_msg void OnButton7();
	afx_msg void OnCheck1();
	afx_msg void OnButton9();
	afx_msg void OnCheck2();
	afx_msg void OnBtnCarm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWMAPDLG_H__9D10DED3_24EE_4BFB_9361_9C1C022C8C0A__INCLUDED_)

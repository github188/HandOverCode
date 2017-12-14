// testdtDlg.h : header file
//

#if !defined(AFX_TESTDTDLG_H__F59CDF67_7147_4441_9BA7_64B10034AC06__INCLUDED_)
#define AFX_TESTDTDLG_H__F59CDF67_7147_4441_9BA7_64B10034AC06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestdtDlg dialog
#define UM_JGPTDATA	   WM_USER + 256
class CTestdtDlg : public CDialog
{
// Construction
public:
	_ConnectionPtr m_pConn;//Êý¾Ý¿â
	BOOL OnInitSQL();
	void WriteLog(LPCTSTR pstrFormat, ...);
	CTestdtDlg(CWnd* pParent = NULL);	// standard constructor

	typedef void (* TFInit)(int /*ikch*/,int /*wMSG*/,HWND /*hwndz*/);
	typedef void (* TFPassH)(LONG /*lPassHandle*/,int /*ikch*/,int /*itf*/);
	typedef void (* TF17C51)(int /*ikch*/,CString /*zkzm*/,int /*ikscs*/,int /*idrcs*/);
	typedef void (* TF17C52)(int /*ikch*/,CString /*zkzm*/, int /*ic*/, CString /*msgz*/);
	typedef void (* TF17C53)(int /*ikch*/,CString /*timestr*/, CString /*msgz*/, int /*ikcfs*/);
	typedef void (* TF17C54)(int /*ikch*/,LPVOID /*msgz*/);
	typedef void (* TF17C55)(int /*ikch*/,int /*ic*/, CString /*msgz*/);
	typedef void (* TF17C56)(int /*ikch*/,int /*itype*/, int /*ikscj*/);
	
	TFInit dllTFInit;
	TFPassH dTFPassH;
	TF17C51 dTF17C51;
	TF17C52 dTF17C52;
	TF17C53 dTF17C53;
	TF17C54 dTF17C54;
	TF17C55 dTF17C55;
	TF17C56 dTF17C56;
	HINSTANCE dllYuvToH264TH;
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CTestdtDlg)
	enum { IDD = IDD_TESTDT_DIALOG };
	CString	m_zkzm;
	int		m_ikch;
	int		m_iCode;
	int		m_ikcfs;
	int		m_ikscj;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestdtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestdtDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnJGPTDataMsg(WPARAM wParam,LPARAM lParam);//256
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDTDLG_H__F59CDF67_7147_4441_9BA7_64B10034AC06__INCLUDED_)

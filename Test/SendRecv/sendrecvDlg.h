// sendrecvDlg.h : header file
//

#if !defined(AFX_SENDRECVDLG_H__2CC6EBB0_5F68_4D39_912B_46966B06A2EA__INCLUDED_)
#define AFX_SENDRECVDLG_H__2CC6EBB0_5F68_4D39_912B_46966B06A2EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSendrecvDlg dialog

class CSendrecvDlg : public CDialog
{
// Construction
public:
	CWinThread* pThread;
	CSendrecvDlg(CWnd* pParent = NULL);	// standard constructor
	//////////////////////////////////////////////////////////////////////////
	//JGPTDLL
	HINSTANCE dllHandleTCP;
	typedef BOOL (* DLLInitSocket)(HWND);
	DLLInitSocket dllInitSocket;
	typedef LRESULT (* DLLSockTGPS)(WPARAM,LPARAM);
	DLLSockTGPS dllSockTGPS;
	//////////////////////////////////////////////////////////////////////////
	//20170313
	typedef int (* DLLSendString)(UINT,LPTSTR,LPTSTR);
	DLLSendString dllsendstring;
	typedef int (* DLLGetRCode)(LPTSTR,int);
	DLLGetRCode dllGetRCode;
	//////////////////////////////////////////////////////////////////////////
	HINSTANCE dllHJMQGnss;
	typedef void (* DLLSendGnssData)(double ,double ,float ,float ,float );
	DLLSendGnssData dllSendGnssData;
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CSendrecvDlg)
	enum { IDD = IDD_SENDRECV_DIALOG };
	CString	m_Ed1;
	CString	m_Ed2;
	CString	m_Ed3;
	CString	m_Ed4;
	double	m_Ejd;
	double	m_Ewd;
	CString	m_strHead;
	int		m_sendi;
	int		m_rcode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendrecvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSendrecvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg LRESULT OnSockGPS(WPARAM,LPARAM);//JGPTDLL
	afx_msg LRESULT OnJGPTMSG(WPARAM,LPARAM);//JGPTDLL
	afx_msg void OnClose();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDRECVDLG_H__2CC6EBB0_5F68_4D39_912B_46966B06A2EA__INCLUDED_)

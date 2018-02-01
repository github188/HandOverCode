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
	typedef BOOL (* DLLSendString)(UINT,LPTSTR);
	DLLSendString dllsendstring;
	//////////////////////////////////////////////////////////////////////////
	HINSTANCE dllHJMQGnss;
	typedef void (* DLLSendGnssData)(double x, double y, float sd, float fx, float lc, int aqd=0, int js=0, int ss=0, int fs=0, 
		int lh=0, int lb=0, int zzx=0, int yzx=0, int ygd=0, int jgd=0, int jsd=0, int xh=0, int kgm=0, 
									   int dw=0, int yg = 0, int wd = 0, int skd = 0,int dh=0, float ycsd = 0, float zs=0);
	DLLSendGnssData dllSendGnssData;
	//////////////////////////////////////////////////////////////////////////
//	CKM3UDPClient.dll
	//SendString(double gnssx,double gnssy,float gnsslc,float gnsssd)

	HINSTANCE dllHKM3UDPC;
	typedef void (* DLLUDPSendString)(double ,double ,float ,float );
	DLLUDPSendString dllUdpSendStr;
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
	afx_msg void OnButton9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDRECVDLG_H__2CC6EBB0_5F68_4D39_912B_46966B06A2EA__INCLUDED_)

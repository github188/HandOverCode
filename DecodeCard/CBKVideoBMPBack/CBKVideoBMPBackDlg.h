// CBKVideoBMPBackDlg.h : header file
//

#if !defined(AFX_CBKVIDEOBMPBACKDLG_H__49013A52_ECDB_4C6E_BD24_C706082E11CE__INCLUDED_)
#define AFX_CBKVIDEOBMPBACKDLG_H__49013A52_ECDB_4C6E_BD24_C706082E11CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoBMPBackDlg dialog
#include "TcpSockKm3.h"
#pragma comment(lib,"KM3VideoTcp.lib")
#define WM_MY_MSG WM_USER+93
typedef struct _XMCODE
{
	int errori;
	int thi;
}XMcode;

class CCBKVideoBMPBackDlg : public CDialog
{
// Construction
public:
	void OnReadSock();
	int pr_count;
	int uXMStop;
	CTime xmStartTime[16];
	int ztxmii[16];
	typedef BOOL (* DrawImage)(UINT,UINT,UINT);
	DrawImage DrawLogo;
	HINSTANCE CBKDeSkinH;
	//////////////////////////////////////////////////////////////////////////
	int iItemLine;
	BOOL ReadMJPhotoFromDB(CDC *pDC, CString sCard, CRect *rc);
	HANDLE hThread;
	CCBKVideoBMPBackDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CCBKVideoBMPBackDlg)
	enum { IDD = IDD_CBKVIDEOBMPBACK_DIALOG };
	CListCtrl	m_ListCard;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKVideoBMPBackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCBKVideoBMPBackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBTNReList();
	afx_msg void OnBtnRecard();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg LRESULT OnQHMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBtnYpls();
	afx_msg void OnDestroy();
	afx_msg void OnSock(WPARAM,LPARAM);//TCP
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL SelectKFLX(int ErrorID,int listi);
	int m_pr_errorint;
	BOOL ISRunTime;
	BOOL OnprErrorN();
	void OnDrawLogo(int Listi,int logoi);
	void SetOSDKSCC();
	void EmptyList(UINT linei);
	BOOL OnprError();
	BOOL OnprErrorALL();
	void DvrIPandChannel(CString bh, CString &dIP, CString &lChannel);
	void ReadDVRPZ();

	void OnListNtoZero();
	int CardNumInList(CString kch);
	BOOL OnSelectProc();
	BOOL OnInitSQL();
	_ConnectionPtr m_pConn;
	//////////////////////////////////////////////////////////////////////////
	void WriteLog(CString sstr);
	CTcpSockKm3 mysockz;//Ì×°´×Ö·þÎñÆ÷
	//////////////////////////////////////////////////////////////////////////

	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc);
	HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lpRect); 
	BOOL SaveBmp(HBITMAP hBitmap, CString FileName);
	BOOL SaveBmp2(HBITMAP hBitmap, CString FileName);
	void SavePho(UINT isave);

	//////////////////////////////////////////////////////////////////////////
	void ReadDisPlayChannel();
private:
	void DvrWriteIPChannel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKVIDEOBMPBACKDLG_H__49013A52_ECDB_4C6E_BD24_C706082E11CE__INCLUDED_)

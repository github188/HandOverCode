#if !defined(AFX_DLGSTUMSG_H__1E02EB2E_0550_4763_B049_973061E3EA86__INCLUDED_)
#define AFX_DLGSTUMSG_H__1E02EB2E_0550_4763_B049_973061E3EA86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStuMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStuMsg dialog
#include "TcpSockKm3.h"
#pragma comment(lib,"KM3VideoTcp.lib")
#include "CMFC5286D.H"
#pragma comment(lib,"CMFC5286D.lib")

class CDlgStuMsg : public CDialog
{
// Construction
public:	
	CString m_Strkscj;
	CString m_StuName;
	BOOL m_TRun;
	int m_errLine;
	int m_Ejlbh;
	void GetErrorRecords();
	CStaticz m_text[8];
	CString m_strZkzm;
	CString m_drcs;
	void OnStuCJ(CString sqlstr);
	void OnStu();
	BOOL OnInitSQL();
	_ConnectionPtr m_pConn;
	void OnReadSock();
	CTcpSockKm3 mysockz;//Ì×°´×Ö·þÎñÆ÷
	int m_ikch;
	CDlgStuMsg(CWnd* pParent = NULL);   // standard constructor
	void SetCliRcSize(int x,int y,int kch);
// Dialog Data
	//{{AFX_DATA(CDlgStuMsg)
	enum { IDD = IDD_DLGSTU };
	CButtonz m_BntRgpp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStuMsg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStuMsg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSock(WPARAM,LPARAM);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnRgpp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	POINT CliRcMin;
	HBRUSH   hbr1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTUMSG_H__1E02EB2E_0550_4763_B049_973061E3EA86__INCLUDED_)

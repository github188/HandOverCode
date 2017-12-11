// CJMQDlg.h : header file
//

#if !defined(AFX_CJMQDLG_H__F2AB3B89_5106_4156_B173_51B2F273FAA1__INCLUDED_)
#define AFX_CJMQDLG_H__F2AB3B89_5106_4156_B173_51B2F273FAA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JMQManager.h"
#include "JmqUDP.h"
#pragma comment(lib,"CUdpForJMQ.lib")
#include "TCPSockz.h"
/////////////////////////////////////////////////////////////////////////////
// CCJMQDlg dialog
typedef struct _SQLDate
{
	int	iKch;	
	char m_Zkzmbh[20];//准考证明编号	
	char m_Msg[20];
	_SQLDate()
	{
		iKch=0;
		m_Zkzmbh[0]='\0';
		m_Msg[0]='\0';
	}
}SQLDATE;

class CCJMQDlg : public CDialog
{
// Construction
public:
	void MOneToSex(int ikch,int type,char *msg);
	CCJMQDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CCJMQDlg)
	enum { IDD = IDD_CJMQ_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJMQDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_NotifyIcon;
	// Generated message map functions
	//{{AFX_MSG(CCJMQDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnMenuExit();//251
	afx_msg void OnMenuShow();//252
	afx_msg void OnHKLastError(WPARAM wParam,LPARAM lParam);//253
	afx_msg void OnSocketUDPZ(WPARAM wParam,LPARAM lParam);//UDP 254
	afx_msg void OnTFPicMsg(WPARAM wParam,LPARAM lParam);//256
	afx_msg LRESULT OnSocketZ(WPARAM wParam,LPARAM lParam);//TCP
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnQHMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool bQueryStart;
	void OnClientStart();
//	void CutOutString(CString source,  CStringArray& dest, char division);
	CString LastErrorFile;
	//////////////////////////////////////////////////////////////////////////
	//日志
	CWriteLog logz;
	DWORD dwlogID,dwFLogID,dwTPlog,dwFPlog;
	//////////////////////////////////////////////////////////////////////////
	CJMQManager jmqmanager;
	CJmqUDP jmqudp;//UDP
	CTCPSockz m_tcps;//TCP
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CJMQDLG_H__F2AB3B89_5106_4156_B173_51B2F273FAA1__INCLUDED_)

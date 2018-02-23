// CUDPServerDlg.h : header file
//

#if !defined(AFX_CUDPSERVERDLG_H__D0621D4E_AD6E_4441_B443_E05E68A65722__INCLUDED_)
#define AFX_CUDPSERVERDLG_H__D0621D4E_AD6E_4441_B443_E05E68A65722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <map>
using namespace std;

#include <afxtempl.h>
#include "UDPz.h"
#pragma comment(lib,"CBkUDP.lib")
#define UM_SOCKUDPZR WM_USER+100
#define UM_SOCKUDPZS WM_USER+101


typedef CList <SOCKETLISTZ,SOCKETLISTZ&> SOCKET_ARRAY;	//自定义了一个套接字LIST
typedef map<CString,struRBody> MAP_STR_RBODYMSG;			//保存结构体 MAP
/////////////////////////////////////////////////////////////////////////////
// CCUDPServerDlg dialog

class CCUDPServerDlg : public CDialog
{
// Construction
public:
	void UpGnssMap();
	static UINT SendThread(LPVOID pParam);
	static UINT RecvThread(LPVOID pParam);
	CCUDPServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCUDPServerDlg)
	enum { IDD = IDD_CUDPSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCUDPServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCUDPServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSocketUDPZR(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSocketUDPZS(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UDPz m_Udpzr;
	UDPz m_Udpzs;
	HANDLE rListhEvent,sListhEvent;

	CStringList m_rstrList;
	MAP_STR_RBODYMSG m_rBodyMap;
	SOCKET_ARRAY m_SockArray;

	void DoHaoCString(CString source,  CStringArray& dest, char division);
	CWriteLog logz;
	DWORD dwlogID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUDPSERVERDLG_H__D0621D4E_AD6E_4441_B443_E05E68A65722__INCLUDED_)

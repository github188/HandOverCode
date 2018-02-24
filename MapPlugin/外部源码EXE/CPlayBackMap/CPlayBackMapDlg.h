// CPlayBackMapDlg.h : header file
//

#if !defined(AFX_CPLAYBACKMAPDLG_H__B79CDC19_D4D2_4EEC_8D86_725C2B2916D7__INCLUDED_)
#define AFX_CPLAYBACKMAPDLG_H__B79CDC19_D4D2_4EEC_8D86_725C2B2916D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Tlhelp32.h"
#include "Shlwapi.h"

#include "UDPz.h"
#pragma comment(lib,"CBkUDP.lib")
/////////////////////////////////////////////////////////////////////////////
// CCPlayBackMapDlg dialog

class CCPlayBackMapDlg : public CDialog
{
// Construction
public:
	void DoHaoCString(CString source,  CStringArray& dest, char division);
	CString fileName;
	static UINT SendThread(LPVOID pParam);
	BOOL bDemonsPorisRun(DWORD* pdwPID,CString strName);
	BOOL DownMapLog();
	CCPlayBackMapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCPlayBackMapDlg)
	enum { IDD = IDD_CPLAYBACKMAP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlayBackMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCPlayBackMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString strPath;
	//数据库
	_ConnectionPtr m_pConn;//数据库
	BOOL OnInitSQL();//初始化数据库连接
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKMAPDLG_H__B79CDC19_D4D2_4EEC_8D86_725C2B2916D7__INCLUDED_)

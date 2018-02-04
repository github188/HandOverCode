// CPlayBackByTimeDlg.h : header file
//
//{{AFX_INCLUDES()
#include "cplaybackzocx.h"
//}}AFX_INCLUDES

#if !defined(AFX_CPLAYBACKBYTIMEDLG_H__705A457E_A1AB_4EF0_849E_E97BAB3D262C__INCLUDED_)
#define AFX_CPLAYBACKBYTIMEDLG_H__705A457E_A1AB_4EF0_849E_E97BAB3D262C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackByTimeDlg dialog

class CCPlayBackByTimeDlg : public CDialog
{
// Construction
public:
	void OnStartPlay();
	CCPlayBackByTimeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCPlayBackByTimeDlg)
	enum { IDD = IDD_CPLAYBACKBYTIME_DIALOG };
	CCPlayBackzOcx	m_PlayBackC;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPlayBackByTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCPlayBackByTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SplitString(const std::string &srcString, const std::string &separator, std::vector<std::string> &strList);

private:
	CString m_sIP;
	CString m_sUsername;
	CString m_sPassword;
	CString m_sBeginTime;
	CString m_sEndTime;
	WORD m_dwPort;
	LONG m_lChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKBYTIMEDLG_H__705A457E_A1AB_4EF0_849E_E97BAB3D262C__INCLUDED_)

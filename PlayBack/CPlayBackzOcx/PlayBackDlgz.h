#if !defined(AFX_PLAYBACKDLGZ_H__D4DAE3FD_205E_4F77_ADD8_25AE08EFB257__INCLUDED_)
#define AFX_PLAYBACKDLGZ_H__D4DAE3FD_205E_4F77_ADD8_25AE08EFB257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayBackDlgz.h : header file
//
#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")
/////////////////////////////////////////////////////////////////////////////
// CTaaDlg dialog
#define PLAYBYTIME_TIMER	12		// 按时间回放
#define PLAYWNDL 2
#define PLAYWNDT 2


#define PLAY_STOP   0
#define PLAY_NORMAL 1
#define PLAY_PAUSE  2
#define PLAY_FAST   3
#define PLAY_SLOW   4
/////////////////////////////////////////////////////////////////////////////
// CPlayBackDlgz dialog

class CPlayBackDlgz : public CDialog
{
// Construction
public:
	void OnStartPlay();
	BOOL SetPlayBackTime(LPCTSTR strSTime,LPCTSTR strETime);	
	BOOL fnDVR_Login_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel);
	CPlayBackDlgz(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayBackDlgz)
	enum { IDD = IDD_DLGPlayBack };
	CSliderCtrl	m_sliderPlay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayBackDlgz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayBackDlgz)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnFast();
	afx_msg void OnBtnSlow();
	afx_msg void OnBtnSound();
	afx_msg void OnBtnPic();
	afx_msg void OnReleasedcaptureSLIDERPlay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	HBRUSH   hbr1;
	HICON m_hPlayEnableIcon;
	HICON m_hPlayDisableIcon;
	
	HICON m_hPauseEnableIcon;
	HICON m_hPauseDisableIcon;
	
	HICON m_hStopEnableIcon;
	HICON m_hStopDisableIcon;
	
	
	HICON m_hFastDisableIcon;
	HICON m_hFastEnableIcon;
	
	HICON m_hSlowDisableIcon;
	HICON m_hSlowEnableIcon;
	
	HICON m_hSoundStartIcon;
	HICON m_hSoundStopIcon;
	
	HICON m_hCaptureIcon;

private:
	BOOL m_bSound;
	BOOL m_bChangeSpeed;
	BOOL m_IsPause;
	int m_iNowPos;
	DWORD m_dwTotalTime;
	CTime m_StopTime;
	CTime m_StartTime;
	void MoveUIz();
	BOOL StrToDVRTime(NET_DVR_TIME &rTime, LPCTSTR lTime);
	
	LONG m_lPlayBackHandle;		//回放句柄
	LONG m_lUserID;			//登录ID
	WORD m_dwChannel;			//通道号
	NET_DVR_TIME struStartTime, struStopTime;
	CString logfilenamez;
	void WriteLogz(LPCTSTR pstrFormat, ...);
	
	void SetPlayState(int iState);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYBACKDLGZ_H__D4DAE3FD_205E_4F77_ADD8_25AE08EFB257__INCLUDED_)

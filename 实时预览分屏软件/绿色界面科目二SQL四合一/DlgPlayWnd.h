#if !defined(AFX_DLGPLAYWND_H__D0204B54_C94E_49D9_8C39_C794E19036FD__INCLUDED_)
#define AFX_DLGPLAYWND_H__D0204B54_C94E_49D9_8C39_C794E19036FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlayWnd.h : header file
//
#include "StaticzEx.h"
#include "CMFC5286D.H"
#pragma comment(lib,"CMFC5286D.lib")

#include "BKPlayWnd.h"

#define INITWNDTIME 1 //初始化
#define ERRORCODETIME 2 
#define STUZKZTIME 3 //
#define VIDEOPLAY_TIME 4 //AVI使用
/////////////////////////////////////////////////////////////////////////////
// CDlgPlayWnd dialog
typedef int (* ShowRGPPDlg)(LPTSTR,LPTSTR,LPTSTR);

class CDlgPlayWnd : public CDialog
{
// Construction
public:
	DWORD logid,logxmid;
	CWriteLog logz;
	void OnQHPlay2();
	static UINT QHThreadFuncc(LPVOID Pparam);
	CString m_errorCodez;
	UINT m_iDBModeT;
	HINSTANCE m_dllHRGPP;
	ShowRGPPDlg m_dllRgppdlg;
	BOOL OnCloseVoiceCom();
	void OnCloseSpeak2();
	void CountExam(CString strCarNum);
	UINT ErrListShow;
	UINT m_WndNum;//标识第几个窗口
	BOOL OnStuZT();
	void WriteFF(CString str);
	time_t m_startavitime;
	BOOL isStartAvi;
	CString aviname,avipath;
	BOOL OnInitSQLw();
	_ConnectionPtr m_pConnwnd;
	void OnEPlaywnd2(CString errocode);
	UINT uKch,uXmlb;//考车号 项目列表
	CString m_StuName,m_StuSFZM,m_StuZKZ;//姓名 身份证 准考证
	int iKscs,iDrcs;//考试次数,当日次数
	CString m_xmsbip,m_xmsbport,m_xmsbuser,m_xmsbpwd,m_xmsbtd;

	CDlgPlayWnd(CWnd* pParent = NULL);   // standard constructor
	CBKPlayWnd playwnd1,playwnd2;
// Dialog Data
	//{{AFX_DATA(CDlgPlayWnd)
	enum { IDD = IDD_DLGPWND };
	CButtonz	m_BTNJudge;
	CButtonz	m_BNTVoice2;
	CButtonz	m_BNTVoice;
	CButtonz	m_BNTSpeak2;
	CButtonz	m_BNTSpeak;
	CButtonz	m_BTNClose;
	CStaticzEx	m_SKCH;
	CStaticzEx	m_Stext4;
	CStaticzEx	m_Stext3;
	CStaticzEx	m_Stext2;
	CStaticzEx	m_Stext1;
	CStaticzEx	m_xmlb7;
	CStaticzEx	m_xmlb6;
	CStaticzEx	m_xmlb5;
	CStaticzEx	m_xmlb4;
	CStaticzEx	m_xmlb3;
	CStaticzEx	m_xmlb2;
	CStaticzEx	m_xmlb1;
	CListCtrlz	m_ListCtrlError;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPlayWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPlayWnd)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnExit();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnVoiceIn();
	afx_msg void OnVoiceClose();
	afx_msg void OnBTNJudge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnStopAvi();
	void OnStartAvi();
	void OnResult();
//	BOOL IsPLAYWND2;
	void OnKsxmE100();
	void OnSwitchXMLB(CString errorcode);
	void OnMoveandUI();
	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc);
	void OnShowPHO();
	BOOL SearchTDH(CString errorcode);
	void OnStuzkzTime();
	void OnErrorcodeTime();
	void OnInitTime();
	int uRecordCount;
	int uksRs;//考车的总考试人数
	int uksCS;//考车的总考试次数
	//////////////////////////////////////////////////////////////////////////
	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps; 
	PAVISTREAM pComStream;
	int nFrames;// = 0; 
	HRESULT hr; 
	AVICOMPRESSOPTIONS pCompressOption;   
	AVICOMPRESSOPTIONS FAR * opts[1];// = {&pCompressOption};
	BOOL m_IsTime;				//是否保存完一帧
	void OnRecord();
	//////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYWND_H__D0204B54_C94E_49D9_8C39_C794E19036FD__INCLUDED_)

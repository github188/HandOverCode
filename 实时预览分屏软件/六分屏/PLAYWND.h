#if !defined(AFX_PLAYWND_H__4C891B85_92AA_4AAA_9905_072A3DDB15B4__INCLUDED_)
#define AFX_PLAYWND_H__4C891B85_92AA_4AAA_9905_072A3DDB15B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PLAYWND.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPLAYWND dialog

class CPLAYWND : public CDialog
{
// Construction
public:
	bool DvrRealPlayNoShow();
	void StartRealPlay();
	CMenu menu,* pSubMenu;
	//////////////////////////////////////////////////////////////////////////
	bool DvrInitLogin(int pzi);
	bool DvrRealPlay(int pzi);
	int iKch;
	void SetCliRcSize(int x,int y);
	CPLAYWND(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CPLAYWND)
	enum { IDD = IDD_DLGPLAY };
	CString	m_MSG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLAYWND)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLAYWND)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMrealplay();
	afx_msg void OnMsoundshare();
	afx_msg void OnMswitch();
	afx_msg void OnMvoicecom();
	afx_msg void OnMExit();
	afx_msg void OnMstartall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bRealPlay;// ”∆µø™∆Ù
	bool m_bSwitch;// ”∆µ«–ªª
	bool m_bVoiceCom;//∂‘Ω≤
	bool m_bPlaySound;//…˘“Ù
	//////////////////////////////////////////////////////////////////////////
	HBRUSH   hbr1,hbr2;
	//////////////////////////////////////////////////////////////////////////
	LONG m_lUserID[3];
	LONG m_lChannel[3];
	LONG m_lPlayHandle[3];
	BYTE m_byStartDTalkChan[3]; 
	LONG m_lVoiceHandle[3];

	POINT CliRcMin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYWND_H__4C891B85_92AA_4AAA_9905_072A3DDB15B4__INCLUDED_)

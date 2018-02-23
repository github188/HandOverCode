#if !defined(AFX_PLAYWNDZ_H__F17730FF_4B8E_4A99_8E2A_938A25AFAEB0__INCLUDED_)
#define AFX_PLAYWNDZ_H__F17730FF_4B8E_4A99_8E2A_938A25AFAEB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayWndz.h : header file
//
#define		VIDEO_BACK_COLOR	RGB(17,20,26) //背景颜色
#define     VIDEO_TEXT_COLOR    RGB(255,10,10)
//右键菜单自定义消息
#define		VIDEO_MENU_BASE				WM_USER + 1800  
#define		VIDEO_MENU_FULLSCREEN		WM_USER + 1801  //全屏显示
#define		VIDEO_MENU_MULTISCREEN		WM_USER + 1802	//多屏
#define		VIDEO_MENU_AUTOADJUST		WM_USER + 1803	//自动调整
#define		VIDEO_MENU_VoiceCom			WM_USER + 1804	//对讲
#define		VIDEO_MENU_PlaySound		WM_USER + 1805	//声音
#define		VIDEO_MENU_RENGONGPP		WM_USER + 1806	//人工评判

#define     VIDEO_MENU_PIC1				WM_USER + 1807	
#define     VIDEO_MENU_PIC4				WM_USER + 1808
#define     VIDEO_MENU_PIC9				WM_USER + 1809
#define     VIDEO_MENU_PIC16			WM_USER + 1810

#define		VIDEO_MENU_STOPPlay 		WM_USER + 1811	//关闭播放
#define		VIDEO_MENU_END				WM_USER + 1812
/////////////////////////////////////////////////////////////////////////////
// CPlayWndz window
// typedef enum enPlayState
// {
// 	NoPlayz,
// 	WaitPlatRealPlayz,
// 	PlatRealPlayz,
// 	WaitHCSDKPlayz,
// 	HCSDKPlayz
// };
#define BNoPlayZ			0x0000
#define BPlatRealPlayz		0x0001
#define	BSDKRealPlayz		0x0002
#define BSDKPlayBackz		0x0004
#define BSDKVoiceCom		0x0008
#define BSDKPlaySound		0x0010
#define BWSDKPlayBackz      0x0020
class CPlayWndz : public CWnd
{
// Construction
public:
	CPlayWndz();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayWndz)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlayWndz();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlayWndz)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnVideoMenu(UINT nID);//z
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
// 	bool m_bPlaySound;
// 	bool m_bVoiceCom;
	int m_PlayState;
	CString m_TiShiText;
	BYTE m_bType;
	int m_nWndID;
	int m_nPreSplit;
public:
//	void SetVoiceSound(int ivoice,bool btype);
	void SetPlayStatez(int playstate,bool isAdd);
	int GetPlayStatez();
	void SetTiShiText(CString str);
	void SetWinID(int ID);
	int  GetWinID(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYWNDZ_H__F17730FF_4B8E_4A99_8E2A_938A25AFAEB0__INCLUDED_)

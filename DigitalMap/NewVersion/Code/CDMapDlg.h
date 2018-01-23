// CDMapDlg.h : header file
//

#if !defined(AFX_CDMAPDLG_H__02603455_71E5_498B_8741_70A3CC008193__INCLUDED_)
#define AFX_CDMAPDLG_H__02603455_71E5_498B_8741_70A3CC008193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define UM_SOCKETZ	   WM_USER + 257
#include "TCPSockz.h"
#include "CarImage.h"
#include <map>
using namespace std;
typedef map<int,SOCKET> MAP_INT_SOCKET;
#define MAXCAR	120
/////////////////////////////////////////////////////////////////////////////
// CCDMapDlg dialog

class CCDMapDlg : public CDialog
{
// Construction
public:
	CCarImage carimg[MAXCAR];
	MAP_INT_SOCKET m_CarMapN;
	void OnGnssData(CString recvtemp,SOCKET csocket);
	BOOL OnFindMAP(int ikch);
	void OnSocketClose(SOCKET CSock);
	bool InitMap();
	CCDMapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCDMapDlg)
	enum { IDD = IDD_CDMAP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCDMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnExits();
	afx_msg void OnShowall();
	afx_msg LRESULT OnSocketZ(WPARAM wParam,LPARAM lParam);//TCP
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static UINT InvaliDateMap_thd(LPVOID p);

	int m_LBx,m_LBy;//左键按下坐标
	bool m_isLBDown;//左键是否按下
	CRect ClientRect;//窗口大小

	CRect m_imgRc;

	CDC	 m_dcMem;
	CBitmap m_bmpMap;
	Image *m_pImageMap;	
	CSize m_imageSize;

	
	double m_Mapx,m_Mapy,m_bs;	//经纬度 和倍数
	float m_ZoomLevel,m_ZoomLevelOld,m_MinZoomLevel; //缩放级别
	float m_ImageL,m_ImageT,m_ImageW,m_ImageH,m_ImgLOld,m_ImgTOld;
	float m_ShowAllL,m_ShowAllT,m_ShowAllW,m_ShowAllH;
	float m_DestL,m_DestT,m_DestW,m_DestH;
	//////////////////////////////////////////////////////////////////////////
	void AssertImageLT();
	//日志
	CWriteLog logz;
	DWORD dwlogID;
	CTCPSockz m_tcps;//TCP
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDMAPDLG_H__02603455_71E5_498B_8741_70A3CC008193__INCLUDED_)

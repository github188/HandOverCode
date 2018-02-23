#if !defined(AFX_TOOLZDLG_H__0F1EF713_3C9E_464D_9F81_2EA9945C11B2__INCLUDED_)
#define AFX_TOOLZDLG_H__0F1EF713_3C9E_464D_9F81_2EA9945C11B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolzDlg.h : header file
//
#include <map>
using namespace std;

#include "CMFC5286D.H"
#pragma comment(lib,"CMFC5286D.lib")

#include "UDPz.h"
#pragma comment(lib,"CBkUDP.lib")
#define UM_SOCKUDPZ WM_USER+86
/////////////////////////////////////////////////////////////////////////////
// CToolzDlg dialog
// typedef struct STRU_GNSSMSGRecv
// {
// 	char cKchp[10];
// 	double	gnssX;//经度
// 	double	gnssY;//纬度
// 	float	gnssR;//方向角
// 	float	gnssSD;//速度
// 	float	gnssLC;//里程
// 	int type;
// }GnssMsgRecv;

typedef map<int,struRBody> MAP_INT_sRBODY; //保存MAP

class CToolzDlg : public CDialog
{
// Construction
public:
	void SetMode(UINT utype);
	void InitKchp(CString strkcdm);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void DrawTipBoxForz(CString strZkzm,CString stuName,CString strKchp,int uIndex);

//	void GetTongJiMsg();
	bool isGetGnssFS;
//	CString GetKCMC() const;
//	bool isRunNowGPSData;
//	bool OnNowGPSData();
//	BOOL DownMapFromDB(CString strKCDM);
//	void SetNewCar(int ikch);
	CToolzDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolzDlg)
	enum { IDD = IDD_DLG_TOOL };
	CButtonzEx	m_BtnShow;
	CListCtrl	m_ListCarZ;
	CString	m_strKch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolzDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolzDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnTest();
	afx_msg void OnDblclkListCar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnZhuizong();
	afx_msg void OnItemchangedListCar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSocketUDPZ(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iPBModeIndex;
	UINT m_uPBType;//状态  1 预览 2 回放
	CWriteLog logz;
	DWORD dwLogid;

	CString strPath;
	void GetGnssMsgFromS();
	CString m_UdpServerIP;
	UDPz m_Udpz;
	void SetKCMC(CString strkcdm);
	CString m_strKCMC,m_strkcdm;
//	CBrush m_brush;		//背景画刷
	Image *m_pImageBK;
	Image *m_pImgTipBox;
	BOOL LoadImageFromIDR(UINT nID, Image * &pImg);
	//////////////////////////////////////////////////////////////////////////
// 	void UpdateDisplayDlg(int Transparent=255);
// 	BLENDFUNCTION m_Blend;
 	
// 	int m_BakWidth , m_BakHeight;
// 	HDC m_hdcMemory;
// 	HINSTANCE hFuncInst ;
// 	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
// 	MYFUNC UpdateLayeredWindow;
	//////////////////////////////////////////////////////////////////////////
	//数据库
//	_ConnectionPtr m_pConn;//数据库
//	BOOL OnInitSQL();//初始化数据库连接
	double m_dMaxX,m_dMaxY,m_dMinX,m_dMinY,m_dXC,m_dYC,m_dZoomIn;//
	bool m_bToolDlgShow;

	MAP_INT_sRBODY m_sRBodyMap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLZDLG_H__0F1EF713_3C9E_464D_9F81_2EA9945C11B2__INCLUDED_)

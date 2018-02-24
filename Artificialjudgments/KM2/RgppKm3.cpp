// RgppKm3.cpp : implementation file
//

#include "stdafx.h"
#include "CKM3RGPP.h"
#include "RgppKm3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRgppKm3 dialog


CRgppKm3::CRgppKm3(CWnd* pParent /*=NULL*/)
	: CDialog(CRgppKm3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRgppKm3)

	//}}AFX_DATA_INIT
}


void CRgppKm3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRgppKm3)
	DDX_Control(pDX, IDC_BTN_RETURN, m_BtnExit);
	DDX_Control(pDX, IDC_STATIC_T3, m_CT3);
	DDX_Control(pDX, IDC_STATIC_T2, m_CT2);
	DDX_Control(pDX, IDC_STATIC_T1, m_CT1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRgppKm3, CDialog)
	//{{AFX_MSG_MAP(CRgppKm3)
	ON_BN_CLICKED(IDC_BTN_RETURN, OnBtnReturn)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_XMDLG_MSG,OnXMDlgMsg)
	ON_MESSAGE(WM_ELDLG_MSG,OnELDlgMsg)	
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRgppKm3 message handlers
void CRgppKm3::OnELDlgMsg(WPARAM wParam,LPARAM lParam)
{
	int tempID=int(wParam);
	if (tempID>10000 && tempID<50000)
	{
		EndDialog(tempID);
	}
}

void CRgppKm3::OnXMDlgMsg(WPARAM wParam,LPARAM lParam)
{
	int tempID=int(wParam);
	if (tempID<3000)
	{
		dlgerrlist.ShowWindow(SW_HIDE);
		dlgxm.ShowWindow(SW_SHOW);
		dlgxm.CenterWindow();
	}
	else
	{
		dlgxm.ShowWindow(SW_HIDE);
		dlgerrlist.SetXMCode(tempID);
		dlgerrlist.ShowWindow(SW_SHOW);
		dlgerrlist.CenterWindow();
	}	
}

void CRgppKm3::OnBtnReturn() 
{
	// TODO: Add your control notification handler code here
//	EndDialog(40001);
	CDialog::OnCancel();
}

void CRgppKm3::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,640,480,SWP_NOMOVE | SWP_NOOWNERZORDER );
}

BOOL CRgppKm3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_STATIC_WND)->MoveWindow(4,90,632,380);
	GetDlgItem(IDC_BTN_RETURN)->MoveWindow(590,8,47,22);

	GetDlgItem(IDC_STATIC_T1)->MoveWindow(10,14,600,36);
	GetDlgItem(IDC_STATIC_T2)->MoveWindow(10,50,260,36);
	GetDlgItem(IDC_STATIC_T3)->MoveWindow(280,50,300,36);

	CWnd * outBroad=GetDlgItem(IDC_STATIC_WND);

	dlgxm.Create(IDD_DLG_XM,outBroad);
	dlgerrlist.Create(IDD_DLG_ELIST,outBroad);
	dlgxm.ShowWindow(SW_SHOW);
 	dlgxm.CenterWindow();	

	m_BtnExit.LoadBitmap(IDB_BMP_CLOSE,4);
	m_BtnExit.SetBtnDrawNum(1,0,2,3,3);
	m_CT1.SetTextnFormat(2);
	dlgxm.EnableWindowz(m_ixmcode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRgppKm3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
// 	if(pWnd->GetDlgCtrlID()==IDC_STATIC_T1 || pWnd->GetDlgCtrlID()==IDC_STATIC_T2 || pWnd->GetDlgCtrlID()==IDC_STATIC_T3 )
//  	{  
//  		pDC->SetBkMode(TRANSPARENT);  
//  		pDC->SetTextColor(RGB(10,10,10));
//  		return   hbr1;  
//     }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CRgppKm3::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC dc(this); 
// 	CRect rect; 
// 	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BMP_BK); 
// 	BITMAP bitmap; 
// 	bmpBackground.GetBitmap(&bitmap);
// 	int nRight=bitmap.bmWidth;
// 	int nBtonn=bitmap.bmHeight;
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
 	dc.BitBlt(0,0,640,480,&dcMem,0,0,SRCCOPY);
// 	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, 
// 		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CRgppKm3::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
void CRgppKm3::SetXMCodez(LPTSTR strCar,LPTSTR strName,LPCTSTR strNum,int ixmcode)
{
	CString	m_T1;
	CString	m_T2;
	CString	m_T3;
	m_T1.Format("%s 后台人工评判",strCar);
	m_T2.Format("姓名:%s",strName);
	m_T3.Format("证件号:%s",strNum);
	m_CT1.SetCaption(m_T1,RGB(239,69,61));
	m_CT2.SetCaption(m_T2,RGB(15,5,15));
	m_CT3.SetCaption(m_T3,RGB(15,5,15));
	m_ixmcode=ixmcode;
}

void CRgppKm3::SetMsg(LPTSTR strCar,LPTSTR strName,LPCTSTR strNum)
{
	CString	m_T1;
	CString	m_T2;
	CString	m_T3;

	m_T1.Format("%s 后台人工评判",strCar);
	m_T2.Format("姓名:%s",strName);
	m_T3.Format("证件号:%s",strNum);
	m_CT1.SetCaption(m_T1,RGB(239,69,61));
	m_CT2.SetCaption(m_T2,RGB(15,5,15));
	m_CT3.SetCaption(m_T3,RGB(15,5,15));
//	UpdateData(FALSE);

}



void CRgppKm3::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

// DlgXMChange.cpp : implementation file
//

#include "stdafx.h"
#include "CKM3RGPP.h"
#include "DlgXMChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgXMChange dialog


CDlgXMChange::CDlgXMChange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgXMChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgXMChange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgXMChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgXMChange)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgXMChange, CDialog)
	//{{AFX_MSG_MAP(CDlgXMChange)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_COMMAND_RANGE(IDC_BTN_1,IDC_BTN_1+18,OnBTNSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_COMMAND_RANGE(IDC_BTN_1,IDC_BTN_1+16,OnBTNSelect)
/////////////////////////////////////////////////////////////////////////////
// CDlgXMChange message handlers
void CDlgXMChange::OnBTNSelect(UINT nID)
{
// 	CString temp;
// 	temp.Format("%d",);
// 	switch (nID)
// 	{
// 	case IDC_BTN_1:
// 		break;
// 	}
//	AfxMessageBox(temp);
	int iBtnid=nID-IDC_BTN_1;
	int iSendID=0;
	if (iBtnid==18)
	{
		iSendID=10100;
		::PostMessage(GetParent()->GetParent()->m_hWnd,WM_XMDLG_MSG,iSendID,0);
	}
	else
	{
		iSendID=iBtnid*100+20100;
		::PostMessage(GetParent()->GetParent()->m_hWnd,WM_XMDLG_MSG,iSendID,0);
	}
	
}

void CDlgXMChange::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,632,380,SWP_NOMOVE | SWP_NOOWNERZORDER );
}

BOOL CDlgXMChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	for (int i=0;i<5;i++)
	{
		for (int j=0;j<4;j++)
		{			
// 			if (i==4 && j!=0)
// 			{
// 				continue;
// 			}
			GetDlgItem(IDC_BTN_1+(i*4+j))->MoveWindow(20+j*150,50+i*60,130,40);
			//TRACE("i*4+j=%d (%d,%d,%d,%d)\n",i*4+j,40+j*120,40+i*40,100,30);
		}
	}
	hbr1=CreateSolidBrush(RGB(255,255,255)); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgXMChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG)
	{
		return   hbr1; 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgXMChange::EnableWindowz(int xmcode)
{
	switch(xmcode)
	{
	case 20100:
		GetDlgItem(IDC_BTN_1)->EnableWindow(TRUE);
		break;
	case 20300:
		GetDlgItem(IDC_BTN_3)->EnableWindow(TRUE);
		break;
	case 20400:
		GetDlgItem(IDC_BTN_4)->EnableWindow(TRUE);
		break;
	case 20600:
		GetDlgItem(IDC_BTN_6)->EnableWindow(TRUE);
		break;
	case 20700:
		GetDlgItem(IDC_BTN_7)->EnableWindow(TRUE);
		break;
	default:
		break;
	}
}
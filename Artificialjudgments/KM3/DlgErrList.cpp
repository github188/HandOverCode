// DlgErrList.cpp : implementation file
//

#include "stdafx.h"
#include "CKM3RGPP.h"
#include "DlgErrList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgErrList dialog


CDlgErrList::CDlgErrList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgErrList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgErrList)
	m_AddMSG = _T("");
	//}}AFX_DATA_INIT
}


void CDlgErrList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgErrList)
	DDX_Control(pDX, IDC_BTN_CF, m_BtnKF);
	DDX_Control(pDX, IDC_ErrorList, m_ListError);
	DDX_Text(pDX, IDC_STATIC_KFM, m_AddMSG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgErrList, CDialog)
	//{{AFX_MSG_MAP(CDlgErrList)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_RETURN, OnBtnReturn)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ErrorList, OnItemchangedErrorList)
	ON_BN_CLICKED(IDC_BTN_CF, OnBtnCf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgErrList message handlers

void CDlgErrList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,632,380,SWP_NOMOVE | SWP_NOOWNERZORDER );
}

void CDlgErrList::OnBtnReturn() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(GetParent()->GetParent()->m_hWnd,WM_XMDLG_MSG,0,0);
}

void CDlgErrList::SetXMCode(int icode)
{
	CString temp,rTemp,strkcfs,strN;
	temp.Format("%d",icode);
	GetPrivateProfileString("KFLX",temp,"错误",rTemp.GetBuffer(MAX_PATH),MAX_PATH,".\\ErrorData.dat");
	GetDlgItem(IDC_STATIC_XMNAME)->SetWindowText(rTemp);

	int iXmNum=GetPrivateProfileInt("CONFIG",temp,0,".\\ErrorData.dat");
	int m_Item;
	m_ListError.DeleteAllItems();
	if (iXmNum!=0)
	{
		for (int i=1;i<=iXmNum;i++)
		{
			temp.Format("%d",icode+i);
			GetPrivateProfileString("KFLX",temp,"...",rTemp.GetBuffer(MAX_PATH),MAX_PATH,".\\ErrorData.dat");
			GetPrivateProfileString("CONFIG",temp,"...",strkcfs.GetBuffer(MAX_PATH),MAX_PATH,".\\ErrorData.dat");
			m_Item=m_ListError.GetItemCount();
			strN.Format("%d",m_Item+1);
			m_ListError.InsertItem(m_Item,strN);
			m_ListError.SetItemText(m_Item,1,rTemp);			
			m_ListError.SetItemText(m_Item,2,strkcfs);
			m_ListError.SetItemText(m_Item,3,temp);
		}
	}
	if (icode==30100)
	{
		iXmNum=GetPrivateProfileInt("CONFIG","30200",0,".\\ErrorData.dat");
		if (iXmNum!=0)
		{
			for (int i=1;i<=iXmNum;i++)
			{
				temp.Format("%d",30200+i);
				GetPrivateProfileString("KFLX",temp,"...",rTemp.GetBuffer(MAX_PATH),MAX_PATH,".\\ErrorData.dat");
				GetPrivateProfileString("CONFIG",temp,"...",strkcfs.GetBuffer(MAX_PATH),MAX_PATH,".\\ErrorData.dat");
				m_Item=m_ListError.GetItemCount();
				strN.Format("%d",m_Item+1);
				m_ListError.InsertItem(m_Item,strN);
				m_ListError.SetItemText(m_Item,1,rTemp);			
				m_ListError.SetItemText(m_Item,2,strkcfs);
				m_ListError.SetItemText(m_Item,3,temp);
			}
		}
	}
}

BOOL CDlgErrList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_STATIC_KFM)->MoveWindow(10,316,550,60);
	GetDlgItem(IDC_BTN_CF)->MoveWindow(562,316,60,60);
	GetDlgItem(IDC_BTN_RETURN)->MoveWindow(460,10,140,30);
	m_ListError.MoveWindow(10,50,610,260);
	m_ListError.SetExtendedStyle(LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP /*|LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE*/);
	m_ListError.InsertColumn(0,_T("N"),LVCFMT_LEFT,40,-1);
	m_ListError.InsertColumn(1,_T("评判内容"),LVCFMT_LEFT,450,-1);
	m_ListError.InsertColumn(2,_T("扣分"),LVCFMT_LEFT,60,-1);
	m_ListError.InsertColumn(3,_T("代码"),LVCFMT_LEFT,-20,-1);

	hbr1=CreateSolidBrush(RGB(251,251,251)); 
	m_BtnKF.LoadBitmap(IDB_BMP_KF);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgErrList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG || nCtlColor==CTLCOLOR_STATIC || pWnd->GetDlgCtrlID()==IDC_STATIC_KFM)
	{
		pDC->SetBkMode(TRANSPARENT);  
  		pDC->SetTextColor(RGB(239,69,61));
		return   hbr1; 
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgErrList::OnItemchangedErrorList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString temp1,temp2,temp;
	temp1=m_ListError.GetItemText(pNMListView->iItem,1);
	temp2=m_ListError.GetItemText(pNMListView->iItem,2);
	m_GAWDM=m_ListError.GetItemText(pNMListView->iItem,3);

	m_AddMSG.Format("%s 扣%s分",temp1,temp2);
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgErrList::OnBtnCf() 
{
	// TODO: Add your control notification handler code here
	if (m_GAWDM.IsEmpty())
	{
		AfxMessageBox("请选择扣分项");
		return ;
	}
	UpdateData(TRUE);
	CString temp;
	temp.Format("%s",m_AddMSG);
	if (MessageBox(temp,"确定扣分",MB_OKCANCEL) == IDOK)
	{
		int posid=atoi(m_GAWDM);
		::PostMessage(GetParent()->GetParent()->m_hWnd,WM_ELDLG_MSG,posid,0);
	}
	m_GAWDM=_T("");
	m_AddMSG=_T("");
	UpdateData(FALSE);
}

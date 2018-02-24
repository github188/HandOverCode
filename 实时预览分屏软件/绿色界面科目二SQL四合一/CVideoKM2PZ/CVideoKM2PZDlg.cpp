// CVideoKM2PZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CVideoKM2PZ.h"
#include "CVideoKM2PZDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCVideoKM2PZDlg dialog

CCVideoKM2PZDlg::CCVideoKM2PZDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCVideoKM2PZDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCVideoKM2PZDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Btnshowid=1;
}

void CCVideoKM2PZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCVideoKM2PZDlg)
	DDX_Control(pDX, IDC_BTNPZVideo, m_BtnPZ);
	DDX_Control(pDX, IDC_BTNFINSH, m_BtnFinsh);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_BTNREFRESH);
	DDX_Control(pDX, IDC_LIST_KCH, m_ListCtrlKCH);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BTNClose);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCVideoKM2PZDlg, CDialog)
	//{{AFX_MSG_MAP(CCVideoKM2PZDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_PIC1, OnBtnPic1)
	ON_BN_CLICKED(IDC_BTN_PIC2, OnBtnPic2)
	ON_BN_CLICKED(IDC_BTN_PIC3, OnBtnPic3)
	ON_BN_CLICKED(IDC_BTN_PIC4, OnBtnPic4)
	ON_BN_CLICKED(IDC_BTNFINSH, OnBtnfinsh)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNPZVideo, OnBTNPZVideo)
	ON_COMMAND_RANGE(IMENU_ITEM_ID,IMENU_ITEM_ID+100,OnMenuSelect)
	ON_UPDATE_COMMAND_UI_RANGE(IMENU_ITEM_ID,IMENU_ITEM_ID+100,OnupdateMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_COMMAND_RANGE(IMENU_ITEM_ID,IMENU_ITEM_ID+100,OnMenuSelect)
//	ON_UPDATE_COMMAND_UI_RANGE(IMENU_ITEM_ID,IMENU_ITEM_ID+100,OnupdateMenu)
/////////////////////////////////////////////////////////////////////////////
// CCVideoKM2PZDlg message handlers

BOOL CCVideoKM2PZDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//////////////////////////////////////////////////////////////////////////
	dllHandle = LoadLibrary(TEXT("CBKVideoSkin.dll")); 
	if (dllHandle == NULL) 
	{ 
		AfxMessageBox("加载CBKVideoSkin.dll失败!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE; 
	}  	
    zbmpmessagebox = (MessageBoxbmp) GetProcAddress(dllHandle,TEXT("MessageBoxbmp"));
//	lpfnDrawBkgnd = (DRAWBKGND) GetProcAddress(dllHandle,TEXT("DrawImage"));
	//////////////////////////////////////////////////////////////////////////
	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	m_menu.CreatePopupMenu();
	GetDlgItem(IDC_BTN_EXIT)->MoveWindow(663,0,47,22);
	GetDlgItem(IDC_LIST_KCH)->MoveWindow(540,100,166,210);
	
	GetDlgItem(IDC_BTNPZVideo)->MoveWindow(607,321,102,35);
	GetDlgItem(IDC_BTN_REFRESH)->MoveWindow(607,364,102,35);
	GetDlgItem(IDC_BTNFINSH)->MoveWindow(607,407,102,35);

	GetDlgItem(IDC_BTN_PIC1)->MoveWindow(20,100,120,80);
	GetDlgItem(IDC_BTN_PIC2)->MoveWindow(142,100,120,80);
	GetDlgItem(IDC_BTN_PIC3)->MoveWindow(20,182,120,80);
	GetDlgItem(IDC_BTN_PIC4)->MoveWindow(142,182,120,80);
	m_BTNClose.LoadBitmap(IDB_BMPCLOSE);
	m_BTNREFRESH.LoadBitmap(IDB_BITMAP_SX);
	m_BtnFinsh.LoadBitmap(IDB_BITMAPFINSH);
	m_BtnPZ.LoadBitmap(IDB_BMP_PZ);

	m_ListCtrlKCH.SetExtendedStyle(LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP /*|LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE*/);
	m_ListCtrlKCH.InsertColumn(0,_T("序号"),LVCFMT_LEFT,40,-1);
	m_ListCtrlKCH.InsertColumn(1,_T("考车号"),LVCFMT_LEFT,100,-1);
	m_ListCtrlKCH.SetLineColor(RGB(223,223,223),RGB(238,238,238));
	m_ListCtrlKCH.SetHeadColor(RGB(251,251,251),RGB(63,135,0),RGB(85,185,73));
	m_ListCtrlKCH.SetTextColor(RGB(51,51,51));

// 	CString tempk;
// 	for (int i=0;i<20;i++)
// 	{
// 		tempk.Format("%d",i+1);
// 		m_ListCtrlKCH.InsertItem(i,tempk);
// 		m_ListCtrlKCH.SetItemText(i,1,"考车"+tempk);
// 
// 	}
	OnBtnRefresh();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCVideoKM2PZDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCVideoKM2PZDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCVideoKM2PZDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCVideoKM2PZDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,720,450,SWP_NOMOVE | SWP_NOOWNERZORDER );
	CRgn m_rgn;
	CRect rc;	
	GetClientRect(&rc);
	rc-=rc.TopLeft();
	m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);
	SetWindowRgn(m_rgn,TRUE);
}

BOOL CCVideoKM2PZDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAP_BACK); 
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,720,450,SRCCOPY); 
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

void CCVideoKM2PZDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CCVideoKM2PZDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if (zbmpmessagebox("退出提示,确定要退出程序么?"))
	{
		CCVideoKM2PZDlg::OnCancel();
	}
}
void CCVideoKM2PZDlg::OnupdateMenu(CCmdUI *pCmdUI)
{

}
void CCVideoKM2PZDlg::OnMenuSelect(UINT nID)
{
	//再这里可以处理
	int i=nID-IMENU_ITEM_ID;
	CString str=m_menuItem.GetAt(i);
	CString strtemp;
	int nPos=str.Find("考车");
	if (nPos != -1 )
	{
		strtemp=str.Mid(4);
	}
	else
	{
		strtemp="999";
	}
	switch (m_Btnshowid)
	{
	case 1:
		GetDlgItem(IDC_BTN_PIC1)->SetWindowText(str);
		::WritePrivateProfileString("kch","pic1",strtemp,".\\search.cfg");
		break;
	case 2:
		GetDlgItem(IDC_BTN_PIC2)->SetWindowText(str);
		::WritePrivateProfileString("kch","pic2",strtemp,".\\search.cfg");
		break;
	case 3:
		GetDlgItem(IDC_BTN_PIC3)->SetWindowText(str);
		::WritePrivateProfileString("kch","pic3",strtemp,".\\search.cfg");
		break;
	case 4:
		GetDlgItem(IDC_BTN_PIC4)->SetWindowText(str);
		::WritePrivateProfileString("kch","pic4",strtemp,".\\search.cfg");
		break;
		}
	
}

void CCVideoKM2PZDlg::OnBtnPic1() 
{
	// TODO: Add your control notification handler code here
	OnShowMenu(1);
}

void CCVideoKM2PZDlg::OnBtnPic2() 
{
	// TODO: Add your control notification handler code here
	OnShowMenu(2);
}

void CCVideoKM2PZDlg::OnBtnPic3() 
{
	// TODO: Add your control notification handler code here
	OnShowMenu(3);
}

void CCVideoKM2PZDlg::OnBtnPic4() 
{
	// TODO: Add your control notification handler code here
	OnShowMenu(4);
}

void CCVideoKM2PZDlg::OnShowMenu(int showid)
{
	m_Btnshowid=showid;
	if(m_menu.m_hMenu)
	{
		int b=m_menu.GetMenuItemCount();
		for(int j=0;j<b;j++)
		{
			m_menu.RemoveMenu(0,MF_BYPOSITION);
		}
		
		m_menuItem.RemoveAll();
		int icount=m_ListCtrlKCH.GetItemCount();
		//下面动态创建菜单
		for(int i=0;i<icount;i++)
		{
			CString strTemp;
			strTemp=m_ListCtrlKCH.GetItemText(i,1);
			BOOL bRet=m_menu.AppendMenu(MF_STRING,IMENU_ITEM_ID+i,strTemp);	
			//	IDArray[i]=IMENU_ITEM_ID+i;//纪录id号，简单的情况可以不要
			if(bRet) m_menuItem.Add(strTemp);//纪录字符串
		}
		CRect rc;
		switch (showid)
		{
		case 1:
			GetDlgItem(IDC_BTN_PIC1)->GetWindowRect(rc);
			break;
		case 2:
			GetDlgItem(IDC_BTN_PIC2)->GetWindowRect(rc);
			break;
		case 3:
			GetDlgItem(IDC_BTN_PIC3)->GetWindowRect(rc);
			break;
		case 4:
			GetDlgItem(IDC_BTN_PIC4)->GetWindowRect(rc);
			break;
		}
		int x = (rc.left+rc.right)/2;
		int y = rc.top+20;
		//显示菜单
		m_menu.TrackPopupMenu(TPM_CENTERALIGN/*TPM_LEFTALIGN|TPM_LEFTBUTTON*/,x,y,this);
	}
}

void CCVideoKM2PZDlg::OnBtnfinsh() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open",".\\CBKFourPicKM2.exe","z",NULL,SW_SHOW); 
	CCVideoKM2PZDlg::OnCancel();
}

void CCVideoKM2PZDlg::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
	CString temp,tempk,xm;
	m_ListCtrlKCH.DeleteAllItems();
	int k=0;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("select * from syscfg where 项目 like '考车%%' and 状态='1'");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("项目");
				if (vat.vt != VT_NULL)
				{
					xm = (LPCSTR)_bstr_t(vat);
				}
				tempk.Format("%d",k+1);
				m_ListCtrlKCH.InsertItem(k,tempk);
				m_ListCtrlKCH.SetItemText(k,1,xm);
				k++;
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		tempk.Format("错误提示,查询考车信息失败!");
		zbmpmessagebox(tempk.GetBuffer(0));
		TRACE(comError.Description());
	}
}

BOOL CCVideoKM2PZDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	try
	{
		//建立数据库连接
		CString Sqlstr;
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		CString m_strInEdit;
		GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
		m_strInEdit.ReleaseBuffer();
		if (!m_strInEdit.IsEmpty())
		{
			int iLen =m_strInEdit.GetLength();
			
			for(int i=0;i<iLen;i++)
			{
				m_strInEdit.SetAt(i,m_strInEdit[i]-1);
			}
			m_strInEdit.MakeReverse();
		}
		Sqlstr=m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		
		//res = m_pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=abc.mdb","","",adModeUnknown);
		
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		
		zbmpmessagebox(errormessage.GetBuffer(0));
		ShellExecute(NULL,"open",".\\sqllink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}



HBRUSH CCVideoKM2PZDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
// 	if ( nCtlColor== CTLCOLOR_BTN )
// 	{
// 		pDC->SetBkMode(TRANSPARENT);  
// 		pDC->SetTextColor(RGB(255,255,255));
// 		HBRUSH   hbr1=CreateSolidBrush(RGB(0,113,169)); 
// 		return   hbr1; 
// 	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CCVideoKM2PZDlg::OnBTNPZVideo() 
{
	// TODO: Add your control notification handler code here
	dllHandle = LoadLibrary(TEXT("CBKVideoPZ.dll")); 
	if (dllHandle == NULL) 
	{ 
		AfxMessageBox("加载CBKVideoPZ.dll失败!"); 
		TerminateProcess(GetCurrentProcess(),0); 
//		return FALSE; 
	}  
	pzvideodlg = (DLGPZVideo) GetProcAddress(dllHandle,TEXT("DLGPZVideo"));
	pzvideodlg();
}

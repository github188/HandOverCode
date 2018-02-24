// CBKFourPicKM2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKFourPicKM2.h"
#include "CBKFourPicKM2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKFourPicKM2App theApp;
#define SPACE 0
#define WNDTOP 0
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
// CCBKFourPicKM2Dlg dialog

CCBKFourPicKM2Dlg::CCBKFourPicKM2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBKFourPicKM2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCBKFourPicKM2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBKFourPicKM2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCBKFourPicKM2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCBKFourPicKM2Dlg, CDialog)
	//{{AFX_MSG_MAP(CCBKFourPicKM2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_BUTTOND_MSG,OnButtonDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKFourPicKM2Dlg message handlers

BOOL CCBKFourPicKM2Dlg::OnInitDialog()
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
	ShowWindow(SW_SHOWMAXIMIZED);// 全屏
	GetClientRect(&CliRc);
	CliRcMin.x=CliRc.Width()/2;
	CliRcMin.y=CliRc.Height()/2;
	GetDlgItem(IDC_STATIC_WND1)->MoveWindow(SPACE,SPACE+WNDTOP,CliRcMin.x-SPACE,CliRcMin.y-SPACE-WNDTOP);
	GetDlgItem(IDC_STATIC_WND2)->MoveWindow(CliRcMin.x+SPACE,SPACE+WNDTOP,CliRcMin.x-SPACE*2,CliRcMin.y-SPACE-WNDTOP);
	GetDlgItem(IDC_STATIC_WND3)->MoveWindow(SPACE,CliRcMin.y+SPACE,CliRcMin.x-SPACE,CliRcMin.y-SPACE*2-WNDTOP);
 	GetDlgItem(IDC_STATIC_WND4)->MoveWindow(CliRcMin.x+SPACE,CliRcMin.y+SPACE,CliRcMin.x-SPACE*2,CliRcMin.y-SPACE*2-WNDTOP);


	CString tempp;
	GetPrivateProfileString("UIskin","avipath","C:\\",tempp.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
	tempp.ReleaseBuffer();
	//////////////////////////////////////////////////////////////////////////
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d");
	
	CString file;
	file.Format("%s%s",tempp,Data);
	DWORD dwAttr=GetFileAttributes(file);
	//若文件夹不存在，创建文件夹
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(file,NULL);
	}
	else if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		//AfxMessageBox("场考照片文件夹已存在!");
	}
	//////////////////////////////////////////////////////////////////////////
	tempp.Format("%s\\\\",file);

	//////////////////////////////////////////////////////////////////////////
	//设置各屏考车号
	CString temp;
	for (int i=0;i<WND_NUM;i++)
	{	
		temp.Format("pic%d",i+1);
		PlayWnd[i].uKch=GetPrivateProfileInt("KCH",temp,0,".\\config.ini");
		PlayWnd[i].avipath=tempp;
		CWnd * outBroad=GetDlgItem(IDC_STATIC_WND1+i);
		PlayWnd[i].m_WndNum = i+1;
		PlayWnd[i].Create(IDD_DLGPWND,outBroad);
		PlayWnd[i].ShowWindow(SW_SHOW);
		PlayWnd[i].CenterWindow();

	}
//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCBKFourPicKM2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCBKFourPicKM2Dlg::OnPaint() 
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
HCURSOR CCBKFourPicKM2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH CCBKFourPicKM2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG)
	{
		// 		pDC->SetBkMode(TRANSPARENT);  
		//		pDC->SetTextColor(RGB(10,255,255));
		HBRUSH   hbr1=CreateSolidBrush(RGB(238,238,238)); 
		return   hbr1; 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CCBKFourPicKM2Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//MessageBox(_T("回车键按下"));
			//			return TRUE;
		case VK_ESCAPE:
			//MessageBox(_T("ESC键按下"));
			if (theApp.zbmpmessagebox("退出提示,确定要退出程序么?"))
			{
				//return TRUE;
				//::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
				this->SendMessage(WM_CLOSE);
			}
			else
			{
				return TRUE;
			}
			break;
			//return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CCBKFourPicKM2Dlg::OnButtonDown(WPARAM wParam,LPARAM lParam)
{
	UINT iwndNum=(UINT) wParam;

	if (lParam==0)
	{
		for (int i=0;i<WND_NUM;i++)
		{
			if (iwndNum!=PlayWnd[i].m_WndNum)
			{
				PlayWnd[i].OnCloseSpeak2();
			}
		}
	}
	else
	{

	}
	return 0;
}

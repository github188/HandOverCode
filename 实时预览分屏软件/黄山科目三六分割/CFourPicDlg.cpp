// CFourPicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CFourPic.h"
#include "CFourPicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WSPACE 1
#define HSPACE 1
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
// CCFourPicDlg dialog

CCFourPicDlg::CCFourPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCFourPicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCFourPicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCFourPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCFourPicDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCFourPicDlg, CDialog)
	//{{AFX_MSG_MAP(CCFourPicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTALLVIDEO_MSG,OnStartAllVideo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFourPicDlg message handlers
void CCFourPicDlg::OnStartAllVideo(WPARAM wParam, LPARAM lParam)
{
	for (int i=0;i<4;i++)
	{
		myPlaywnd[i].StartRealPlay();
	}
	//return 0;
}
BOOL CCFourPicDlg::OnInitDialog()
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
	hbr1=CreateSolidBrush(RGB(60,60,60));
	ShowWindow(SW_SHOWMAXIMIZED);// 全屏
	NET_DVR_Init();// 初始化

	CString strPath,tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));
	DWORD dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	tempPath.Format("%s\\%s",strPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	logFileName.Format("%s\\日志.log",tempPath);
	//////////////////////////////////////////////////////////////////////////
	GetClientRect(&CliRc);
	CliRcMin.x=CliRc.Width()/3-2*WSPACE;
	CliRcMin.y=CliRc.Height()/2-2*HSPACE;
	GetDlgItem(IDC_STATIC_WND1)->MoveWindow(WSPACE,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND2)->MoveWindow(WSPACE*3+CliRcMin.x,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND5)->MoveWindow(WSPACE*5+CliRcMin.x*2,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND3)->MoveWindow(WSPACE,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND4)->MoveWindow(WSPACE*3+CliRcMin.x,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND6)->MoveWindow(WSPACE*5+CliRcMin.x*2,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);

	CString temp;
	CWnd * outBroad;
	int i=0;
	for (i=0;i<4;i++)
	{
		if(i%2==0)
		{
			myPlaywnd[i].m_bFirst=false;			
		}
		else
		{
			myPlaywnd[i].m_bFirst=true;
		}
		temp.Format("画面%d",i+1);
		myPlaywnd[i].SetCliRcSize(CliRcMin.x,CliRcMin.y);		
		myPlaywnd[i].iKch=GetPrivateProfileInt("画面",temp,0,".\\aconfig.ini");
		outBroad=GetDlgItem(IDC_STATIC_WND1+i);
		myPlaywnd[i].Create(IDD_DLGPLAY,outBroad);
		myPlaywnd[i].ShowWindow(SW_SHOW);
		myPlaywnd[i].CenterWindow();
	}
	myStuMsg[0].SetCliRcSize(CliRcMin.x,CliRcMin.y,myPlaywnd[0].iKch);
	outBroad=GetDlgItem(IDC_STATIC_WND5);
	myStuMsg[0].Create(IDD_DLGSTU,outBroad);
	myStuMsg[0].ShowWindow(SW_SHOW);
	myStuMsg[0].CenterWindow();

	myStuMsg[1].SetCliRcSize(CliRcMin.x,CliRcMin.y,myPlaywnd[2].iKch);
	outBroad=GetDlgItem(IDC_STATIC_WND6);
	myStuMsg[1].Create(IDD_DLGSTU,outBroad);
	myStuMsg[1].ShowWindow(SW_SHOW);
	myStuMsg[1].CenterWindow();
	WriteLog("更新版本 20170803 0953");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCFourPicDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCFourPicDlg::OnPaint() 
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
HCURSOR CCFourPicDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCFourPicDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	NET_DVR_Cleanup();//释放
	CDialog::OnClose();
}

BOOL CCFourPicDlg::PreTranslateMessage(MSG* pMsg) 
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
			if (MessageBox("是否退出程序!","提示",MB_OKCANCEL) == IDOK)
			{
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

HBRUSH CCFourPicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG )
	{
		return hbr1;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

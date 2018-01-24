// CPlayBackByTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CPlayBackByTime.h"
#include "CPlayBackByTimeDlg.h"

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
// CCPlayBackByTimeDlg dialog

CCPlayBackByTimeDlg::CCPlayBackByTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPlayBackByTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPlayBackByTimeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCPlayBackByTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPlayBackByTimeDlg)
	DDX_Control(pDX, IDC_CPLAYBACKZOCXCTRL1, m_PlayBackC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCPlayBackByTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CCPlayBackByTimeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackByTimeDlg message handlers

BOOL CCPlayBackByTimeDlg::OnInitDialog()
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
	SetTimer(1,100,NULL);
	CString temp;
	temp.Format("[%s] 正在回放...",__targv[1]);
	SetWindowText(temp);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCPlayBackByTimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCPlayBackByTimeDlg::OnPaint() 
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
HCURSOR CCPlayBackByTimeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CCPlayBackByTimeDlg::OnStartPlay()
{   
	CString temp,temp1,temp2;
	CString DSIP,DSUser,Dspwd,strStime,strEtime;
	WORD DSport;
	LONG Channel;
	//temp2.Format("CONFIG");
	temp2.Format("%s",__targv[1]);
	
	GetPrivateProfileString(temp2,"xmsbip","192.168.0.68",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	DSIP.Format("%s",temp);
	GetPrivateProfileString(temp2,"xmsbuser","admin",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	DSUser.Format("%s",temp);
	GetPrivateProfileString(temp2,"xmsbpwd","12345",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	Dspwd.Format("%s",temp);
	GetPrivateProfileString(temp2,"xmsbport","8000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	temp1.Format("%s",temp);
	DSport =_ttoi(temp1);
	GetPrivateProfileString(temp2,"xmsbtd","1",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	temp1.Format("%s",temp);
	Channel =atol(temp1);
	
	GetPrivateProfileString(temp2,"xmstime","1",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	strStime.Format("%s",temp);

	
	GetPrivateProfileString(temp2,"xmetime","1",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
	strEtime.Format("%s",temp);


	m_PlayBackC.LoginDevice(DSIP,DSUser,Dspwd,DSport,(short)Channel);
	m_PlayBackC.SetPlayBackByTime(strStime,strEtime);

}

void CCPlayBackByTimeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	OnStartPlay();
	CDialog::OnTimer(nIDEvent);
}

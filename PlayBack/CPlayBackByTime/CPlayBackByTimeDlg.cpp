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

	m_sIP = "";
	m_sUsername = "";
	m_sPassword = "";
	m_sBeginTime = "";
	m_sEndTime = "";
	m_lChannel = 0;
	m_dwPort = 0;
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
	try
	{
		//参数定义
		//考车编号*设备IP*用户名*密码*端口号*通道号*开始时间*结束时间
		//考车1*192.168.0.92*admin*hk12345678*8000*33*2018-02-03 14:10:35*2018-02-03 14:10:35
		CString csCmdLine1 = __targv[1];
		CString csCmdLine2 = __targv[2];
		CString csCmdLine3 = __targv[3];
		CString csCmdLine = csCmdLine1 + " " + csCmdLine2 + " " + csCmdLine3;

		vector<string> vecParams;
		SplitString(csCmdLine.GetBuffer(csCmdLine.GetLength()), "*", vecParams);
		if (8 != vecParams.size())
		{
			AfxMessageBox("参数异常");
			return false;
		}

		CString sCarNo = vecParams[0].c_str();
		m_sIP = vecParams[1].c_str();
		m_sUsername = vecParams[2].c_str();
		m_sPassword = vecParams[3].c_str();
		m_dwPort = _ttoi(vecParams[4].c_str());
		m_lChannel = atol(vecParams[5].c_str());
		m_sBeginTime = vecParams[6].c_str();
		m_sEndTime = vecParams[7].c_str();

		if (sCarNo.IsEmpty() || m_sIP.IsEmpty() || m_sUsername.IsEmpty() || m_sPassword.IsEmpty() || m_sBeginTime.IsEmpty() || m_sEndTime.IsEmpty()
			|| m_dwPort <= 0 || m_lChannel <= 0)
		{
			AfxMessageBox("参数异常");
			return FALSE;
		}

		SetTimer(1,100,NULL);
		CString temp;
		temp.Format("[%s] 正在回放...", sCarNo);
		SetWindowText(temp);
	}
	catch (...)
	{
		AfxMessageBox("参数异常");
		return FALSE;
	}
	
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

void CCPlayBackByTimeDlg::SplitString(const std::string &srcString, const std::string &separator, std::vector<std::string> &strList)
{
	size_t pos = 0;
	for (;;)
	{
		size_t  tpos = 0;
		std::string substr;
		tpos = srcString.find(separator.c_str(), pos);
		if (tpos == std::string::npos)
		{
			substr = srcString.substr(pos);
			if (substr.size())
			{
				strList.push_back(substr);
			}
			break;
		}
		//if (tpos != pos)		//分隔符隔出来的空字符串也取出来
		{
			substr = srcString.substr(pos, tpos - pos);
			strList.push_back(substr);
		}
		pos = tpos + separator.length();
		
	}
}

void CCPlayBackByTimeDlg::OnStartPlay()
{   
// 	CString temp,temp1,temp2;
// 	CString DSIP,DSUser,Dspwd,strStime,strEtime;
// 	WORD DSport;
// 	LONG Channel;
// 	//temp2.Format("CONFIG");
// 	temp2.Format("%s",__targv[1]);
// 	
// 	GetPrivateProfileString(temp2,"xmsbip","192.168.0.68",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	DSIP.Format("%s",temp);
// 	GetPrivateProfileString(temp2,"xmsbuser","admin",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	DSUser.Format("%s",temp);
// 	GetPrivateProfileString(temp2,"xmsbpwd","12345",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	Dspwd.Format("%s",temp);
// 	GetPrivateProfileString(temp2,"xmsbport","8000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	temp1.Format("%s",temp);
// 	DSport =_ttoi(temp1);
// 	GetPrivateProfileString(temp2,"xmsbtd","1",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	temp1.Format("%s",temp);
// 	Channel =atol(temp1);
// 	
// 	GetPrivateProfileString(temp2,"xmstime","1",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	strStime.Format("%s",temp);
// 
// 	
// 	GetPrivateProfileString(temp2,"xmetime","1",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\VIDEOPLAYBACK.dat");
// 	strEtime.Format("%s",temp);


	m_PlayBackC.LoginDevice(m_sIP,m_sUsername,m_sPassword,m_dwPort,(short)m_lChannel);
	m_PlayBackC.SetPlayBackByTime(m_sBeginTime,m_sEndTime);

}

void CCPlayBackByTimeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	OnStartPlay();
	CDialog::OnTimer(nIDEvent);
}

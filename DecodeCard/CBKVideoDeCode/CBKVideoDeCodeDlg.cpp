// CBKVideoDeCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKVideoDeCode.h"
#include "CBKVideoDeCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ICONMESSAGE WM_USER + 210
#define ID_MENU_EXIT   WM_USER + 211
#define ID_MENU_SHOW   WM_USER + 212
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
// CCBKVideoDeCodeDlg dialog

CCBKVideoDeCodeDlg::CCBKVideoDeCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBKVideoDeCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCBKVideoDeCodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBKVideoDeCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCBKVideoDeCodeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCBKVideoDeCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CCBKVideoDeCodeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_SOCKZ,OnSock)//TCP
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)//
	ON_COMMAND(ID_MENU_SHOW, OnMenuShow)//
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoDeCodeDlg message handlers
void CCBKVideoDeCodeDlg::OnMenuShow()
{
	if(IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_SHOW);
	}
}

void CCBKVideoDeCodeDlg::OnMenuExit()
{
	if (MessageBox("是否退出程序!","提示",MB_OKCANCEL) == IDOK)
	{
		KillTimer(1);
		HWND hWnd=::FindWindow(NULL,_T("CBKVideoBMPBack"));
		if (NULL !=hWnd)
		{
			::SendMessage(hWnd,WM_CLOSE,0,0);
			//Sleep(100);
		}
		CCBKVideoDeCodeDlg::OnCancel();
	}
}
BOOL CCBKVideoDeCodeDlg::OnInitDialog()
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
	if (!mysockz.InitSock(m_hWnd,5287))
	{
		WriteLog("UDP 初始化失败!端口5287");
		CDialog::OnCancel();
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	ShellExecute(NULL,"open",".\\CWelComeDlg.exe","start",NULL,SW_SHOW);
	if (!jiemak.InitDsp(m_hWnd))
	{
		HWND hWnd=::FindWindow(NULL,_T("WelCome..."));
		if (NULL !=hWnd)
		{
			::SendMessage(hWnd,WM_CLOSE,0,0);
		}
		CDialog::OnCancel();
		return FALSE;
	}
	WriteLog("初始化解码卡成功!V1.5.8");//初始化解码卡
	jiemak.SetDisRGN();//画面分割
	//////////////////////////////////////////////////////////////////////////
	int ti;
	ti=GetPrivateProfileInt("CONFIG","debug",0,".\\search.cfg");
	if (ti == 0 )
	{
		HWND hWnd=::FindWindow(NULL,_T("CBKVideoBMPBack"));
		if (NULL !=hWnd)
		{
			::SendMessage(hWnd,WM_CLOSE,0,0);
			Sleep(100);
		}
		
		ShellExecute(NULL,"open",".\\CBKVideoBMPBack.exe","start",NULL,SW_SHOW);
		SetTimer(1,10000,NULL); //守护进程
	}	
	//托盘图标初始化
	m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIcon.hWnd = this->GetSafeHwnd();
	strcpy(m_NotifyIcon.szTip, _T("解码卡主程序"));//托盘程序显示名称
	m_NotifyIcon.uCallbackMessage = WM_ICONMESSAGE;
	m_NotifyIcon.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON;
	m_NotifyIcon.hIcon = m_hIcon;
	m_NotifyIcon.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);
	PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
	//////////////////////////////////////////////////////////////////////////
	HWND hWnd=::FindWindow(NULL,_T("WelCome..."));
	if (NULL !=hWnd)
	{
		::SendMessage(hWnd,WM_CLOSE,0,0);
	}
	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCBKVideoDeCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCBKVideoDeCodeDlg::OnPaint() 
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
HCURSOR CCBKVideoDeCodeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CCBKVideoDeCodeDlg::OnSock(WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
	case FD_READ:
		OnReadSock();
		break;
	}
}
void CCBKVideoDeCodeDlg::OnReadSock()
{
	CString strTemp;
	strTemp=mysockz.RecvFromSock();
	if (strTemp.IsEmpty() || strTemp.GetLength()<=2)
	{
		return ;
	}
	UINT uCmd=atoi(strTemp.Left(2));
	strTemp =strTemp.Mid(2);
	UINT disChannel,nChannel;
	switch(uCmd)
	{
	case DisLOGO:
		disChannel=_ttoi(strTemp);
		jiemak.SetDispChanLogo(disChannel);
		break;
	case NoDisLOGO:
		disChannel=_ttoi(strTemp);
		jiemak.SetDispChanLogo(disChannel,FALSE);
		break;
	case SCardVideo:
		{
			int pos=strTemp.Find(",");
			if (pos >=0)
			{
				CString dvrIP;
				nChannel=atoi(strTemp.Left(pos));
				strTemp =strTemp.Mid(pos+1);
				pos=strTemp.Find(",");
				dvrIP.Format("%s",strTemp.Left(pos));
				strTemp =strTemp.Mid(pos+1);
				disChannel=atoi(strTemp);
				jiemak.DvrPlayChannel(nChannel,dvrIP,disChannel,0);//0车载 1项目
				nChannel=disChannel+5403;
				jiemak.SetDispChanLogo(nChannel);
			}
			break;
		}		
	case ECardVideo:
		break;
	case SXMVideo:
		{
			int pos=strTemp.Find(",");
			if (pos >=0)
			{
				CString dvrIP;
				nChannel=atoi(strTemp.Left(pos));
				strTemp =strTemp.Mid(pos+1);
				pos=strTemp.Find(",");
				dvrIP.Format("%s",strTemp.Left(pos));
				strTemp =strTemp.Mid(pos+1);
				disChannel=atoi(strTemp);
				jiemak.DvrPlayChannel(nChannel,dvrIP,disChannel,1);//0车载 1项目
			}
			break;
		}
	case EXMVideo:
		{
			disChannel=_ttoi(strTemp);
			jiemak.DvrPlayStopXM(disChannel);
			break;
		}
	case OSDSuDu:
		{
			int pos=strTemp.Find(",");
			if (pos >=0)
			{
				CString temp;
				temp=strTemp.Left(pos);
				strTemp =strTemp.Mid(pos+1);
				disChannel=atoi(strTemp);
				jiemak.DvrSetDisPOSD(disChannel,temp,1);
// 				CString ltemp;
// 				ltemp.Format("%d,%s,%d",disChannel,temp,1);
// 				WriteLog(ltemp);
			}
			break;
		}
	case OSDSC:
		{
			int pos=strTemp.Find(",");
			if (pos >=0)
			{
				CString temp;
				temp=strTemp.Left(pos);
				strTemp =strTemp.Mid(pos+1);
				disChannel=atoi(strTemp);
				jiemak.DvrSetDisPOSD(disChannel,temp,0);
			}
			break;
		}
	case DVRLOGIN:
		{
			CString temp,temp1,temp2;
			CString DSIP,DSUser,Dspwd;//设备IP 用户 密码
			WORD wPort;//端口号
			temp2.Format("%s",strTemp);
			GetPrivateProfileString(temp2,"xmsbip","192.168.0.68",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\search.cfg");
			DSIP.Format("%s",temp);
			GetPrivateProfileString(temp2,"xmsbuser","admin",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\search.cfg");
			DSUser.Format("%s",temp);
			GetPrivateProfileString(temp2,"xmsbpwd","12345",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\search.cfg");
			Dspwd.Format("%s",temp);
			GetPrivateProfileString(temp2,"xmsbport","8000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\search.cfg");
			temp1.Format("%s",temp);
			wPort =_ttoi(temp1); 
			jiemak.F_Login(DSIP.GetBuffer(0),wPort,DSUser.GetBuffer(0),Dspwd.GetBuffer(0));			
			break;
		}
	default:
		break;
	}
}

void CCBKVideoDeCodeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	HANDLE hObject = CreateMutex(NULL,FALSE,"CCBKVideoBMPBackApp");	
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hObject);
		TRACE("已存在\n");
	}
	else
	{
		CloseHandle(hObject);
		TRACE("不存在\n");
		ShellExecute(NULL,"open",".\\CBKVideoBMPBack.exe","restart",NULL,SW_SHOW); 
		WriteLog("启动程序CBKVideoBMPBack.exe");
	}
	CDialog::OnTimer(nIDEvent);
}

void CCBKVideoDeCodeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)
	{ 
		ShowWindow(SW_HIDE); // 当最小化时，隐藏主窗口 
	}
}

BOOL CCBKVideoDeCodeDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon);//删除托盘图标
	return CDialog::DestroyWindow();
}

LRESULT CCBKVideoDeCodeDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_ICONMESSAGE:
		if(lParam == WM_RBUTTONDOWN)
		{
			CMenu menu;
			CPoint point;
			GetCursorPos(&point);
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, ID_MENU_SHOW, _T("显示/隐藏"));
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_MENU_EXIT, _T("退出程序"));
			
			menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
			menu.DestroyMenu();
		}
		break;
	default:
		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

// CJMQDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CJMQ.h"
#include "CJMQDlg.h"

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
// CCJMQDlg dialog

CCJMQDlg::CCJMQDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCJMQDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCJMQDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCJMQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCJMQDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCJMQDlg, CDialog)
	//{{AFX_MSG_MAP(CCJMQDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)//251
	ON_COMMAND(ID_MENU_SHOW, OnMenuShow)//252
	ON_MESSAGE(UM_HKLASTERROR,OnHKLastError)//253
	ON_MESSAGE(UM_SOCKUDPZ,OnSocketUDPZ)//UDP
	ON_MESSAGE(UM_JGPTDATA,OnTFPicMsg)//256
	ON_MESSAGE(UM_SOCKETZ,OnSocketZ)//TCP 257
	ON_WM_SIZE()
	ON_MESSAGE(WM_MY_MSG,OnQHMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJMQDlg message handlers
LRESULT CCJMQDlg::OnQHMsg(WPARAM wParam, LPARAM lParam)
{
	//////////////////////////////////////////////////////////////////////////
	CString sendstr;
	sendstr.Format("%d",lParam);
	int ikch;
	ikch=atoi(sendstr);
	jmqmanager.OnQH17C52(ikch,(int)wParam);
	return 0;
}

LRESULT CCJMQDlg::OnSocketZ(WPARAM wParam,LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam); //消息类别 
	int iErrorCode = WSAGETSELECTERROR(lParam);//错误代码 
	SOCKET CurSock = (SOCKET)wParam; //目标socket 
	CString recvTemp;
	switch(iEvent) 
	{ 
	case FD_ACCEPT:
		m_tcps.OnFDAccept();
		return 0;		
	case FD_READ:{
		recvTemp=m_tcps.RecvFromSock(CurSock);
		//jmqmanager.OnJGPTData(recvTemp);//处理TCP数据 项目数据
		m_tcps.SendToClient(CurSock,recvTemp);
		return 0;}
	case FD_CLOSE:{
		closesocket(CurSock);
		logz.WriteLog(dwlogID,"TCP 客户端退出! Port:%d",CurSock);
		return 0;}
	default: 
		return 0;
	}
	return 0;
}

void CCJMQDlg::OnSocketUDPZ(WPARAM wParam,LPARAM lParam)//UDP
{
	if ((LOWORD(lParam) & FD_READ)==FD_READ)
	{
		PACKTYPE type;
		int ikch;
		char *recvbuf=NULL;
		jmqudp.RecvFromSock(type,&recvbuf,ikch);	
		switch (type)
		{
		case GNSSDATA:
			jmqmanager.OnGnssData(recvbuf,ikch);//处理UDP数据 经纬度等信息
			break;
		case QueryStart:
			OnClientStart();
			break;
		case M17C51:
			MOneToSex(ikch,1,recvbuf);
			break;
		case M17C52:
			MOneToSex(ikch,2,recvbuf);
			break;
		case M17C53:
			MOneToSex(ikch,3,recvbuf);
			break;
		case M17C54:
			MOneToSex(ikch,4,recvbuf);
			break;
		case M17C55:
			MOneToSex(ikch,5,recvbuf);
			break;
		case M17C56:
			MOneToSex(ikch,6,recvbuf);
			break;
		default:
			break;
		}
	}
}

void CCJMQDlg::OnClientStart()
{
	CStringArray kcharr;
	jmqmanager.GetKCHArray(kcharr);
	for (int i=0;i<kcharr.GetSize();i++)
	{
		jmqudp.SendPack(AllCarNum,"",0,"127.0.0.1",6710,atoi(kcharr.GetAt(i)));
	}
}

void CCJMQDlg::OnTFPicMsg(WPARAM wParam,LPARAM lParam)
{	
	CString *pmsg=(CString *)lParam;
	CString temp=*pmsg;
	if ((int )wParam ==3)
	{
		logz.WriteLog(dwTPlog,"%s",temp);
	}
	else if((int )wParam ==4)
	{
		logz.WriteLog(dwFPlog,"%s",temp);
	}
	else
	{
		logz.WriteLog(dwFLogID,"%s",temp);
	}
	delete pmsg;
}

BOOL CCJMQDlg::OnInitDialog()
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
	dwlogID=logz.Register("解码器主程序.log");
	dwFLogID=logz.Register("JMQError.log");	
	dwTPlog=logz.Register("ThirdPic.log");
	dwFPlog=logz.Register("FourthPic.log");
	//////////////////////////////////////////////////////////////////////////
	CString tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	LastErrorFile.Format("%s\\HCNetSDKE.DAT",tempPath.Left(tempPath.ReverseFind('\\')));
	//////////////////////////////////////////////////////////////////////////		
	if (!jmqmanager.OnInitSQL())//连接数据库
	{
		CDialog::OnCancel();
		return FALSE;
	}
	if (!jmqudp.InitSockU(m_hWnd,6709,UM_SOCKUDPZ))
	{
		CDialog::OnCancel();
		return FALSE;
	}
	jmqmanager.SetTerminalPZ();	//读取 设备通道对应配置
	jmqmanager.SetErrorData();	//读取 表ErrorData
	//////////////////////////////////////////////////////////////////////////
	//托盘图标初始化
	m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIcon.hWnd = this->GetSafeHwnd();
	strcpy(m_NotifyIcon.szTip, _T("解码器 V2.0"));//托盘程序显示名称
	m_NotifyIcon.uCallbackMessage = WM_ICONMESSAGE;
	m_NotifyIcon.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON;
	m_NotifyIcon.hIcon = m_hIcon;
	m_NotifyIcon.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);
	PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
	logz.WriteLog(dwlogID,"解码器 InitInstance V2.0 Build 20171028 1131");
	//////////////////////////////////////////////////////////////////////////
	jmqmanager.TermailDEInit();//初始化
	jmqmanager.RunMap();//运行
	jmqudp.SendPack(JMQStart,"",0,"127.0.0.1",6710,0);
// 	if (!m_tcps.InitSockS(m_hWnd,6708,UM_SOCKETZ,dwlogID))//接收考车发送TCP数据
// 	{
// 		CDialog::OnCancel();
// 		return FALSE;
// 	}
// 	if (__argc <2 )
// 	{
// 		CString ClientFile;
// 		ClientFile.Format("%s\\CJMQDemons.exe",tempPath.Left(tempPath.ReverseFind('\\')));
// 		ShellExecute(NULL,"open",ClientFile,"CJMQ.exe",NULL,SW_HIDE);
// 	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CCJMQDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCJMQDlg::OnPaint() 
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
HCURSOR CCJMQDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCJMQDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon);//删除托盘图标
	CDialog::OnClose();
}

LRESULT CCJMQDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
void CCJMQDlg::OnMenuShow()
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

void CCJMQDlg::OnMenuExit()
{
	if (MessageBox("是否退出程序!","提示",MB_OKCANCEL) == IDOK)
	{
		::WritePrivateProfileString("CONFIG","Demons","1",".\\config.ini");
		logz.WriteLog(dwlogID,"菜单退出程序!");
		this->SendMessage(WM_CLOSE);
	}
}

void CCJMQDlg::OnHKLastError(WPARAM wParam,LPARAM lParam)
{
	CString temp,keyname;
	keyname.Format("%d",wParam);
	TRACE("OnHKLastError=[%s]\n",keyname);
	GetPrivateProfileString("LASTERROR",keyname,"",temp.GetBuffer(MAX_PATH),MAX_PATH,LastErrorFile);
	temp.ReleaseBuffer();
	logz.WriteLog(dwFLogID,"HKLastError ID:%s,内容:%s",keyname,temp);
}

void CCJMQDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)
	{ 
		ShowWindow(SW_HIDE); // 当最小化时，隐藏主窗口 
	}
}

void CCJMQDlg::MOneToSex(int ikch, int type, char *msg)
{
	if (jmqmanager.OnFindCar(ikch))
	{
		SQLDATE temp;
		memcpy((char *)&temp,msg,sizeof(SQLDATE));
		CString Zkzmbh,strmsg;
		Zkzmbh.Format("%s",temp.m_Zkzmbh);
		strmsg.Format("%s",temp.m_Msg);
		logz.WriteLog(dwlogID,"MOneToSix ikch=%d,zkzm=%s,msg=%s,17C5%d",ikch,Zkzmbh,strmsg,type);
		switch(type)
		{
		case 1:
			jmqmanager.OnJ17C51(ikch,Zkzmbh);
			break;
		case 2:
			jmqmanager.OnJ17C52(ikch,Zkzmbh,strmsg);
			break;
		case 3:
			jmqmanager.OnJ17C53(ikch,Zkzmbh,strmsg);
			break;
		case 4:
			break;
		case 5:
			jmqmanager.OnJ17C55(ikch,Zkzmbh,strmsg);
			break;
		case 6:
			jmqmanager.OnJ17C56(ikch,Zkzmbh,strmsg);
			break;
		default:
			break;
		}
	}
}

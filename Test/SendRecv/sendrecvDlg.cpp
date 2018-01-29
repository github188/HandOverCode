// sendrecvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sendrecv.h"
#include "sendrecvDlg.h"

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
// CSendrecvDlg dialog

CSendrecvDlg::CSendrecvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendrecvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendrecvDlg)
	m_Ed1 = _T("");
	m_Ed2 = _T("522400042064");
	m_Ed3 = _T("401");
	m_Ed4 = _T("");
	m_Ejd = 0.0;
	m_Ewd = 0.0;
	m_strHead = _T("");
	m_sendi = 0;
	m_rcode = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendrecvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendrecvDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Ed1);
	DDX_Text(pDX, IDC_EDIT2, m_Ed2);
	DDX_Text(pDX, IDC_EDIT3, m_Ed3);
	DDX_Text(pDX, IDC_EDIT4, m_Ed4);
	DDX_Text(pDX, IDC_EDIT5, m_Ejd);
	DDX_Text(pDX, IDC_EDIT6, m_Ewd);
	DDX_Text(pDX, IDC_EDIT7, m_strHead);
	DDX_Text(pDX, IDC_EDIT8, m_sendi);
	DDX_Text(pDX, IDC_EDIT9, m_rcode);
	//}}AFX_DATA_MAP
}
//////////////////////////////////////////////////////////////////////////
#define UM_SOCKGPS WM_USER+1986//JGPTDLL
#define UM_JGPTMSG WM_USER+1987//JGPTDLL
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSendrecvDlg, CDialog)
	//{{AFX_MSG_MAP(CSendrecvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_MESSAGE(UM_SOCKGPS,OnSockGPS)//JGPTDLL
	ON_MESSAGE(UM_JGPTMSG,OnJGPTMSG)//JGPTDLL
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendrecvDlg message handlers

BOOL CSendrecvDlg::OnInitDialog()
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
	dllHandleTCP = LoadLibrary(TEXT("CBKTcpKM2.dll")); 
	if (dllHandleTCP == NULL) 
	{ 
		AfxMessageBox("加载CBKTcpKM2.dll失败!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE; 
	}  
	
    dllInitSocket = (DLLInitSocket) GetProcAddress(dllHandleTCP,TEXT("DInitSocket"));
	dllSockTGPS= (DLLSockTGPS) GetProcAddress(dllHandleTCP,TEXT("OnSockTGPS"));
	dllsendstring=(DLLSendString) GetProcAddress(dllHandleTCP,TEXT("SendString"));
	dllGetRCode=(DLLGetRCode) GetProcAddress(dllHandleTCP,TEXT("GetRCode"));
	if (dllInitSocket==NULL || dllSockTGPS ==NULL || dllsendstring ==NULL || dllGetRCode==NULL)
	{
		AfxMessageBox("ERROR");
	}
	dllInitSocket(m_hWnd);

// 	dllHJMQGnss =LoadLibrary(TEXT("CSendDataToJMQ.dll")); 
// 	if (dllHJMQGnss == NULL)
// 	{
// 		AfxMessageBox("CSendDataToJMQ.dll失败!"); 
// 		TerminateProcess(GetCurrentProcess(),0); 
// 		return FALSE; 
// 	}
// 	dllSendGnssData =(DLLSendGnssData) GetProcAddress(dllHJMQGnss,TEXT("SendGnssData"));
// 	if (dllSendGnssData ==NULL)
// 	{
// 		AfxMessageBox("333");
// 	}
	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//////////////////////////////////////////////////////////////////////////
//JGPTDLL
LRESULT CSendrecvDlg::OnSockGPS(WPARAM wParam,LPARAM lParam)
{
	dllSockTGPS(wParam,lParam);
	return 0;
}
LRESULT CSendrecvDlg::OnJGPTMSG(WPARAM wParam,LPARAM lParam)
{
	int iHandle=(int )wParam;

	CString *pmsg=(CString *)lParam;
	m_Ed1=*pmsg;		
	TRACE(m_Ed1+"\n");		
	UpdateData(FALSE);
	delete pmsg;	
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CSendrecvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSendrecvDlg::OnPaint() 
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
HCURSOR CSendrecvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSendrecvDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString senddate;
	senddate.Format("*17C51**%s*%d-%02d-%02d %02d:%02d:%02d*",m_Ed2,curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),\
		curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());//时钟
	m_sendi=dllsendstring(1,"17C51",senddate.GetBuffer(0));
	m_strHead=_T("17C51");
	UpdateData(FALSE);

}

void CSendrecvDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString senddate;
	senddate.Format("*17C52*%s*%s*%d-%02d-%02d %02d:%02d:%02d*",m_Ed3,m_Ed2,curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),\
		curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());//时钟
	m_sendi=dllsendstring(2,"17C52",senddate.GetBuffer(0));
	m_strHead=_T("17C51");
	UpdateData(FALSE);
}

void CSendrecvDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString senddate;
	senddate.Format("*17C53*%s@%s*%s*%d-%02d-%02d %02d:%02d:%02d*",m_Ed3,m_Ed4,m_Ed2,curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),\
		curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());//时钟
	m_sendi=dllsendstring(3,"17C53",senddate.GetBuffer(0));
	m_strHead=_T("17C53");
	UpdateData(FALSE);
}

void CSendrecvDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString senddate;
	senddate.Format("*17C54*%s*%s*%d-%02d-%02d %02d:%02d:%02d*",m_Ed3,m_Ed2,curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),\
		curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());//时钟
	m_sendi=dllsendstring(4,"17C54",senddate.GetBuffer(0));
	m_strHead=_T("17C54");
	UpdateData(FALSE);
}

void CSendrecvDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString senddate;
	senddate.Format("*17C55*%s*%s*%d-%02d-%02d %02d:%02d:%02d*",m_Ed3,m_Ed2,curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),\
		curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());//时钟

	m_sendi=dllsendstring(5,"17C55",senddate.GetBuffer(0));
	m_strHead=_T("17C55");
	UpdateData(FALSE);
}

void CSendrecvDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString senddate;
	senddate.Format("*17C56*%s*%s*%d-%02d-%02d %02d:%02d:%02d*",m_Ed4,m_Ed2,curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),\
		curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());//时钟
	m_sendi=dllsendstring(6,"17C56",senddate.GetBuffer(0));
	m_strHead=_T("17C56");
	UpdateData(FALSE);
}

void CSendrecvDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(dllHandleTCP !=NULL)
	{
		FreeLibrary(dllHandleTCP);//JGPTDLL
	}
	CDialog::OnClose();
}

void CSendrecvDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	dllSendGnssData(m_Ejd,m_Ewd,10,10,10);
}
UINT MyTheard(LPVOID pParam)
{
	CSendrecvDlg *pInfo = (CSendrecvDlg *)pParam;
	CString strPathName = "GPS连续数据.txt";
	CStdioFile file;
	
	if (!file.Open(strPathName, CFile::modeRead))
	{
		::AfxMessageBox(_T("文件打开失败"));
		return 0;
	}
	CString strText = _T("");
	double gnssx,gnssy;
	float gnsslc,gnsssd;
	gnsssd=0.2f;
	while (file.ReadString(strText))
	{	
		int pos=strText.Find(",");
		gnssx=atof(strText.Left(pos));
		strText=strText.Mid(pos+1);
		pos=strText.Find(",");
		gnssy=atof(strText.Left(pos));
		strText=strText.Mid(pos+1);
		gnsslc=(float)atof(strText);
		//gnsssd=0.2f;
		pInfo->dllSendGnssData(gnssx,gnssy,gnsslc,gnsssd,gnsslc);
		Sleep(1000);
	}
	file.Close();
	return 0;
}
void CSendrecvDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
//	hThread= CreateThread(NULL,0,MyThreadProc,NULL,0,&threadID);
	pThread=AfxBeginThread(MyTheard,(void*)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
}



void CSendrecvDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_rcode=dllGetRCode(m_strHead.GetBuffer(0),m_sendi);
	UpdateData(FALSE);
}

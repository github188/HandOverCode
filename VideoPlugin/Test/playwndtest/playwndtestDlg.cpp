// playwndtestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "playwndtest.h"
#include "playwndtestDlg.h"

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
// CPlaywndtestDlg dialog

CPlaywndtestDlg::CPlaywndtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaywndtestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlaywndtestDlg)
	m_IP = _T("192.168.0.96");
	m_User = _T("admin");
	m_PWd = _T("hk12345678");
	m_nPort = 8000;
	m_nChannel = 33;
	m_StrKchp = _T("闽A123456");
	m_strLID = _T("298");
	m_Stime = _T("2017-07-12 10:10:10");
	m_Etime = _T("2017-07-12 10:30:30");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlaywndtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaywndtestDlg)
	DDX_Text(pDX, IDC_EDIT1, m_IP);
	DDX_Text(pDX, IDC_EDIT2, m_User);
	DDX_Text(pDX, IDC_EDIT3, m_PWd);
	DDX_Text(pDX, IDC_EDIT4, m_nPort);
	DDX_Text(pDX, IDC_EDIT5, m_nChannel);
	DDX_Control(pDX, IDC_CBKPLAYZWNDXCTRL1, m_PlayControl);
	DDX_Text(pDX, IDC_EDIT6, m_StrKchp);
	DDX_Text(pDX, IDC_EDIT7, m_strLID);
	DDX_Text(pDX, IDC_EDIT8, m_Stime);
	DDX_Text(pDX, IDC_EDIT9, m_Etime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlaywndtestDlg, CDialog)
	//{{AFX_MSG_MAP(CPlaywndtestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaywndtestDlg message handlers

BOOL CPlaywndtestDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlaywndtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlaywndtestDlg::OnPaint() 
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
HCURSOR CPlaywndtestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPlaywndtestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_PlayControl.SetRealStreamID(m_StrKchp,m_strLID);
// 	if(m_PlayControl.SetDvrInfo(m_IP,m_User,m_PWd,m_nPort,m_nChannel)==TRUE)
// 	{
// 		m_PlayControl.StartPlay();
// 	}
}

void CPlaywndtestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_PlayControl.SetMultiWnd(0);
}

void CPlaywndtestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	m_PlayControl.SetMultiWnd(1);
}

void CPlaywndtestDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	m_PlayControl.SetMultiWnd(2);
}

void CPlaywndtestDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	m_PlayControl.SetMultiWnd(3);	
}

void CPlaywndtestDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
		m_PlayControl.SetDeviceInfo(m_IP,m_User,m_PWd,m_nPort,m_nChannel,1);
}

BEGIN_EVENTSINK_MAP(CPlaywndtestDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPlaywndtestDlg)
	ON_EVENT(CPlaywndtestDlg, IDC_CBKPLAYZWNDXCTRL1, 2 /* ClickRgppMenu */, OnClickRgppMenuCbkplayzwndxctrl1, VTS_BSTR VTS_I4)
	ON_EVENT(CPlaywndtestDlg, IDC_CBKPLAYZWNDXCTRL1, 3 /* RClickMenu */, OnRClickMenuCbkplayzwndxctrl1, VTS_BSTR VTS_I2 VTS_I2)
	ON_EVENT(CPlaywndtestDlg, IDC_CBKPLAYZWNDXCTRL1, 1 /* RenGongPingPan */, OnRenGongPingPanCbkplayzwndxctrl1, VTS_BSTR VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()








void CPlaywndtestDlg::OnClickRgppMenuCbkplayzwndxctrl1(LPCTSTR strKchp, long lWndId) 
{
	// TODO: Add your control notification handler code here
	m_PlayControl.ShowRgppDlg("闽A123456789","张三","123456789","R123456789*1*2");

}

void CPlaywndtestDlg::OnRClickMenuCbkplayzwndxctrl1(LPCTSTR strKchp, short uWndID, short uParam) 
{
	// TODO: Add your control notification handler code here
	CString temp;
	temp.Format("RClickMenu [%s][%d][%d]",strKchp,uWndID,uParam);
	AfxMessageBox(temp);
	if (uParam==1) //调用实时预览接口
	{
		UpdateData(TRUE);
		//m_PlayControl.SetDeviceInfo(m_IP,m_User,m_PWd,m_nPort,m_nChannel,uParam);

		m_PlayControl.SetDeviceInfo(m_IP,m_User,m_PWd,m_nPort,m_nChannel,1);
	}
	else if (uParam ==3)
	{
		UpdateData(TRUE);
		m_PlayControl.SetDeviceInfo(m_IP,m_User,m_PWd,m_nPort,m_nChannel,uParam);
	}
}

void CPlaywndtestDlg::OnRenGongPingPanCbkplayzwndxctrl1(LPCTSTR CarName, long XMCode) 
{
	// TODO: Add your control notification handler code here
	CString temp;
	temp.Format("人工评判返回[%s][%d]",CarName,XMCode);
	AfxMessageBox(temp);
}

void CPlaywndtestDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_PlayControl.SetDeviceInfo(m_IP,m_User,m_PWd,m_nPort,m_nChannel,2);

	m_PlayControl.SetPlayBackByTime(m_Stime,m_Etime);

	

}

// HikVisionTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HikVisionTest.h"
#include "HikVisionTestDlg.h"

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
// CHikVisionTestDlg dialog

CHikVisionTestDlg::CHikVisionTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHikVisionTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHikVisionTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHikVisionTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHikVisionTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHikVisionTestDlg, CDialog)
	//{{AFX_MSG_MAP(CHikVisionTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHikVisionTestDlg message handlers

BOOL CHikVisionTestDlg::OnInitDialog()
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
//	m_LodaHikSdk.InitLoadDll("D:\\bkzyVideoOcx\\hikVision\\CHikVisionz.dll");
		m_LodaHikSdk.InitLoadDll("E:\\work201706\\¼à¹ÜÊÓÆµ¿Ø¼þOCX\\CHikVisionz\\Debug\\CHikVisionz.dll");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHikVisionTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHikVisionTestDlg::OnPaint() 
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
HCURSOR CHikVisionTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHikVisionTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_LodaHikSdk.SDKInitWnd(GetDlgItem(IDC_STATIC_WND)->m_hWnd,MSG_HIKSDK,0);
	if(m_LodaHikSdk.SDKDVRLogin("192.168.0.195","admin","12345",8000,1,0)==TRUE)
	{
		m_LodaHikSdk.SDKRealPlay(0);
	}

	m_LodaHikSdk.SDKInitWnd(GetDlgItem(IDC_STATIC_WND2)->m_hWnd,MSG_HIKSDK,1);
	if(m_LodaHikSdk.SDKDVRLogin("192.168.0.192","admin","hk12345678",8000,36,1)==TRUE)
	{
		m_LodaHikSdk.SDKRealPlay(1);
	}
	
}

void CHikVisionTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
//	m_LodaHikSdk.SDKInitWnd(GetDlgItem(IDC_STATIC_WND)->m_hWnd,MSG_HIKSDK,0);
	m_LodaHikSdk.SDKStartVoiceCom_V30("192.168.0.115","admin","hk12345678",8000,1,1);
}

void CHikVisionTestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	m_LodaHikSdk.SDKStopVoiceCom(1);
}

void CHikVisionTestDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	m_LodaHikSdk.SDKInitWnd(GetDlgItem(IDC_STATIC_WND)->m_hWnd,MSG_HIKSDK,0);
	if(m_LodaHikSdk.SDKDVRLogin("192.168.0.195","admin","12345",8000,1,0)==TRUE)
	{
		m_LodaHikSdk.SDKPlayBackbyTime("2017-07-12 10:10:10","2017-07-12 10:30:10",0);
	}
}

void CHikVisionTestDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	m_LodaHikSdk.SDKOpenSoundShare(1);
}

void CHikVisionTestDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	m_LodaHikSdk.SDKCloseSoundShare(1);
}

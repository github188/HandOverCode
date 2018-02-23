// plattestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "plattest.h"
#include "plattestDlg.h"

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
// CPlattestDlg dialog

CPlattestDlg::CPlattestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlattestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlattestDlg)
	m_Cid = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlattestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlattestDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Cid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlattestDlg, CDialog)
	//{{AFX_MSG_MAP(CPlattestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlattestDlg message handlers
void CALLBACK  MessageProcFunc(UINT nWndID,WPARAM wParam,LPARAM lParam,DWORD dwUser)
{
	if(dwUser != 0)
	{
		CPlattestDlg *dlg = (CPlattestDlg *)dwUser;
		dlg->MessageProc(nWndID,wParam,lParam);
	}	
}

#define PLAT_URLOK	WM_USER + 1905
#define PLAT_URLFAILD	WM_USER + 1906

void CPlattestDlg::MessageProc(UINT nWndID,WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case PLAT_URLOK:
		{			
			AfxMessageBox("PLAT_URLOK");
			m_LoadPlatSdk.PLATPlayVideo(0,(long)GetDlgItem(IDC_STATIC_WND)->m_hWnd);
//			dllPlayVideoPlat(0,(long)GetDlgItem(IDC_STATIC_WND)->m_hWnd);
		}
		break;
	case PLAT_URLFAILD:
		AfxMessageBox("PLAT_URLFAILD");
		break;
	default:
		break;
	}
	
}

BOOL CPlattestDlg::OnInitDialog()
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
// 	SetCurrentDirectory("D:\\bkzyVideoOcx\\IVMS8600");
// 	dllPLATHIN = LoadLibrary(TEXT("D:\\bkzyVideoOcx\\IVMS8600\\CBKPlatSDKz.dll")); 
// 	if (dllPLATHIN == NULL) 
// 	{ 
// 		AfxMessageBox("加载 8600数据 失败!"); 
// 		return FALSE; 
// 	}	
// 	dllPlayVideoPlat = (DPlayVideoPlat) GetProcAddress(dllPLATHIN,TEXT("PlayVideoPlat"));
// 	dllStopVideoPlat = (DStopVideoPlat) GetProcAddress(dllPLATHIN,TEXT("StopVideoPlat"));
// 	dllSetStreamCamer = (DSetStreamCamer) GetProcAddress(dllPLATHIN,TEXT("SetStreamCamer"));
// 	dllSetCallBack = (DSetCallBack) GetProcAddress(dllPLATHIN,TEXT("SetCallBack"));
// 	if (dllPlayVideoPlat==NULL || dllStopVideoPlat==NULL || dllSetStreamCamer==NULL || dllSetCallBack==NULL)
// 	{			
// 		AfxMessageBox("加载 8600接口 失败!"); 
// 		return FALSE;
// 	}
// 	dllSetCallBack(MessageProcFunc,(DWORD)this);
	m_LoadPlatSdk.InitLoadDll();
	m_LoadPlatSdk.PLATSetCallBack(MessageProcFunc,(DWORD)this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlattestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlattestDlg::OnPaint() 
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
HCURSOR CPlattestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPlattestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	dllSetStreamCamer(m_Cid,0);
	m_LoadPlatSdk.PLATSetStreamCamer(m_Cid,0);
}

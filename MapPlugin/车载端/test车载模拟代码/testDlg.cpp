// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"

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
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
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
	//.cpp 加载
	//////////////////////////////////////////////////////////////////////////
	dllHzjjg =LoadLibrary(TEXT("CBKJGUdpClient.dll")); 
	if (dllHzjjg == NULL)
	{
		AfxMessageBox("CBKJGUdpClient.dll 失败!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE; 
	}
	dllzjjgSendData =(DLLUDPSendToJGSer) GetProcAddress(dllHzjjg,TEXT("UDPSendToJGSer"));
	if (dllzjjgSendData ==NULL)
	{
		AfxMessageBox("加载CBKJGUdpClient UDPSendToJGSer 失败!");
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDlg::OnPaint() 
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

UINT MyTheard(LPVOID pParam)
{
	CTestDlg *pInfo = (CTestDlg *)pParam;
	CString strPathName = "GPS连续数据.txt";
	CStdioFile file;
	
	if (!file.Open(strPathName, CFile::modeRead))
	{
		::AfxMessageBox(_T("文件打开失败"));
		return 0;
	}
	CString strText = _T("");
	double gnssx,gnssy;
	float gnsslc,gnsssd,gnssfx;
	gnsssd=0.2f;
	gnsslc=0.2f;
	int ilne=0;
	CString sendstr;
	CString m_strKchp;
	GetPrivateProfileString("CONFIG","KCHP","",m_strKchp.GetBuffer(MAX_PATH),MAX_PATH,".\\udpconfig.ini");
	m_strKchp.ReleaseBuffer(MAX_PATH);
	while (file.ReadString(strText))
	{	
		int pos=strText.Find(",");
		gnssx=atof(strText.Left(pos));
		strText=strText.Mid(pos+1);
		pos=strText.Find(",");
		gnssy=atof(strText.Left(pos));
		strText=strText.Mid(pos+1);
		pos=strText.Find(",");
		gnssfx=(float)atof(strText.Left(pos));
		//gnsslc=(float)atof(strText);
		//gnsssd=0.2f;
		//		pInfo->dllSendGnssData(gnssx,gnssy,(float)(ilne*0.2),gnssfx-90,(float)(ilne*1.2));
		//pInfo->dllUdpSendStr(gnssx,gnssy,(float)(ilne*0.2),(float)(ilne*1.2));
		sendstr.Format("BEGIN,3,52012,%s,0,2011111,5555555,90,10:10:10,0,268435457,%f,%f,250,2,2,4,%lf,%lf,60,0,0,0,航向角,0,0,00001111111111111123,40100,0,0,0,,END",m_strKchp,(float)(ilne*0.2),(float)(ilne*1.2),gnssx*100000,gnssy*100000);
		pInfo->dllzjjgSendData(sendstr.GetBuffer(0));

		ilne++;
		Sleep(1000);
	}
	file.Close();
	return 0;
}

void CTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	pThread=AfxBeginThread(MyTheard,(void*)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	AfxMessageBox("开启线程");
}

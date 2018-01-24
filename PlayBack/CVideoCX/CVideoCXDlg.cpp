// CVideoCXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CVideoCX.h"
#include "CVideoCXDlg.h"

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
// CCVideoCXDlg dialog

CCVideoCXDlg::CCVideoCXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCVideoCXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCVideoCXDlg)
	m_StartTime = 0;
	m_StopTime = 0;
	m_tiaoj = _T("");
	m_strSM = _T("");
	m_StartTimes = 0;
	m_StopTimes = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bOracle = false;
	m_bSql = false;
	m_bEncrypt = false;
}

void CCVideoCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCVideoCXDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_ComBoTJ);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_StopTime);
	DDX_Text(pDX, IDC_EDIT1, m_tiaoj);
	DDX_Text(pDX, IDC_EDIT2, m_strSM);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_StartTimes);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_StopTimes);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCVideoCXDlg, CDialog)
	//{{AFX_MSG_MAP(CCVideoCXDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BTN_CX, OnBtnCx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_BTN_PZ, OnBtnPz)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCVideoCXDlg message handlers

BOOL CCVideoCXDlg::OnInitDialog()
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
	m_StartTime=CTime::GetCurrentTime();
	m_StopTime=CTime::GetCurrentTime();
	UpdateData(FALSE);
	m_ComBoTJ.InsertString(0,"全部");
	m_ComBoTJ.InsertString(1,"姓名"); 
	m_ComBoTJ.InsertString(2,"身份证");
	m_ComBoTJ.InsertString(3,"考车编号");
	m_ComBoTJ.SetCurSel(0);

	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0,_T("编号"),LVCFMT_LEFT,40,-1);
	m_ListCtrl.InsertColumn(1,_T("姓名"),LVCFMT_LEFT,100,-1);
	m_ListCtrl.InsertColumn(2,_T("身份证编号"),LVCFMT_LEFT,180,-1);
	m_ListCtrl.InsertColumn(3,_T("考试次数"),LVCFMT_LEFT,60,-1);
	m_ListCtrl.InsertColumn(4,_T("当日次数"),LVCFMT_LEFT,60,-1);
	m_ListCtrl.InsertColumn(5,_T("考车号"),LVCFMT_LEFT,70,-1);	
	m_ListCtrl.InsertColumn(6,_T("开始时间"),LVCFMT_LEFT,140,-1);
	m_ListCtrl.InsertColumn(7,_T("结束时间"),LVCFMT_LEFT,140,-1);
	m_strSM.Format("使用说明:默认查询某个时间段的全部信息,要精确查找,先选择查询条件.eg:选择姓名.则在精确查找后面输入 张三 ,就可以精确查询张三。\
		查询完后选择下面列表中的信息点播放即可。");
	
	CTime tempStime(513360000);//00:00:00
	CTime tempEtime(513446399);//23:59:59
	m_StartTimes=tempStime;
	m_StopTimes=tempEtime;
	UpdateData(FALSE);
	WriteConfig();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCVideoCXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCVideoCXDlg::OnPaint() 
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
HCURSOR CCVideoCXDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCVideoCXDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (strBfStime.IsEmpty() || strBfEtime.IsEmpty())
	{
		AfxMessageBox("错误提示,请先查询数据然后,选择列表中的信息,再播放!");
		return ;
	}
	CString temp;
	temp.Format("%s",strKCH);
	::WritePrivateProfileString(temp,"xmstime",strBfStime,".\\VIDEOPLAYBACK.dat");
	::WritePrivateProfileString(temp,"xmetime",strBfEtime,".\\VIDEOPLAYBACK.dat");

	ShellExecute(NULL,"open","CPlayBackByTime.exe",temp,NULL,SW_SHOW); 
	strBfStime=_T("");
	strBfEtime=_T("");
	UpdateData(FALSE);
}

BOOL CCVideoCXDlg::UpdateList(CString temp)
{	
	int k=0;
	CString tempk;
	m_ListCtrl.DeleteAllItems();//清空列表
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		CString strbh,strName,strsfzm,strdrcs,strkscs,strkch,strstime,stretime;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
					strbh.Format("%d",k+1);
					vat =pSet->GetCollect("姓名");
					if (vat.vt != VT_NULL)
					{
						strName = (LPCSTR)_bstr_t(vat);
					}else{strName=_T("");}

					vat =pSet->GetCollect("身份证明编号");
					if (vat.vt != VT_NULL)
					{
						strsfzm = (LPCSTR)_bstr_t(vat);
					}else{strsfzm=_T("");}

					vat =pSet->GetCollect("考试次数");
					if (vat.vt != VT_NULL)
					{
						strkscs = (LPCSTR)_bstr_t(vat);
					}else{strkscs=_T("");}

					vat =pSet->GetCollect("当日次数");
					if (vat.vt != VT_NULL)
					{
						strdrcs = (LPCSTR)_bstr_t(vat);
					}else{strdrcs=_T("");}

					vat =pSet->GetCollect("考车号");
					if (vat.vt != VT_NULL)
					{
						strkch = (LPCSTR)_bstr_t(vat);
					}else{strkch=_T("");}
					vat =pSet->GetCollect("开始时间");
					if (vat.vt != VT_NULL)
					{
						COleDateTime strtimeDT = COleDateTime(vat.date);
						// 						COleDateTimeSpan timespan(0,0,2,1);
						// 						COleDateTime strtimestr;
						strstime.Format("%s",strtimeDT.Format("%Y-%m-%d %H:%M:%S"));
					}else{strstime=_T("");}

					vat =pSet->GetCollect("考试时间");
					if (vat.vt != VT_NULL)
					{
						COleDateTime strtimeDT = COleDateTime(vat.date);
// 						COleDateTimeSpan timespan(0,0,2,1);
// 						COleDateTime strtimestr;
						stretime.Format("%s",strtimeDT.Format("%Y-%m-%d %H:%M:%S"));

					}else{stretime=_T("");}

					m_ListCtrl.InsertItem(k,strbh);
					m_ListCtrl.SetItemText(k,1,strName);
					m_ListCtrl.SetItemText(k,2,strsfzm);
					m_ListCtrl.SetItemText(k,3,strkscs);
					m_ListCtrl.SetItemText(k,4,strdrcs);
					m_ListCtrl.SetItemText(k,5,strkch);
					m_ListCtrl.SetItemText(k,6,strstime);
					m_ListCtrl.SetItemText(k,7,stretime);
					k++;
					pSet->MoveNext();
					
			}//while
		}//if
		else
		{				
			TRACE("查询到记录为空\n");
			return FALSE;
		}
	}
	catch (_com_error &comError)
	{
		//TRACE("打开数据库异常：%s!\n",comError.ErrorMessage());
		tempk.Format("查询数据出错!UpdateList,错误[%s]",comError.Description());
		AfxMessageBox(tempk);
	}
	return TRUE;
}


BOOL CCVideoCXDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
//		logz.WriteLog(dwlogID,"初始化连接对象失败!CoInitialize NULL");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;

	//数据库类型
	int nDBType = GetPrivateProfileInt("CONFIG", "DBTYPE", 0, ".\\config.ini");
	if (0 == nDBType)
	{
		m_bOracle = true;
	}
	else if (1 == nDBType)
	{
		m_bSql = true;
	}

	//数据库字段是否加密
	int nEncrypt = GetPrivateProfileInt("CONFIG", "ENCRYPT", 0, ".\\config.ini");
	if (1 == nEncrypt)
	{
		m_bEncrypt = true;
	}
	
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
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
//			logz.WriteLog(dwlogID,"成功连接到数据库!");
		}
		else
		{
			TRACE("打开数据库失败!");
//			logz.WriteLog(dwlogID,"连接数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

void CCVideoCXDlg::OnBtnCx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CString strSQL,temp;
	CString strST,strET;
	strST.Format("%s %s",m_StartTime.Format("%Y-%m-%d"),m_StartTimes.Format("%H:%M:%S"));
	strET.Format("%s %s",m_StopTime.Format("%Y-%m-%d"),m_StopTimes.Format("%H:%M:%S"));

	TRACE("%s   %s\n",strST,strET);
	int iComboxS=m_ComBoTJ.GetCurSel();
	if (!m_bEncrypt)
	{
		if (m_bOracle)
		{
			temp.Format("select * from examrecordindetail where (开始时间 between to_date('%s','yyyy-MM-dd hh24:mi:ss') and to_date('%s','yyyy-MM-dd hh24:mi:ss') ) ",strST,strET);
		}
		else if (m_bSql)
		{
			temp.Format("select * from examrecordindetail where (开始时间 between convert(datetime, '%s') and convert(datetime, '%s'))", strST, strET);
		}

		switch(iComboxS)
		{
		case 1:
			strSQL.Format("%s and 姓名='%s' ",temp,m_tiaoj);
			break;
		case 2:
			strSQL.Format("%s and 身份证明编号='%s' ",temp,m_tiaoj);
			break;
		case 3:
			strSQL.Format("%s and 考车号='%s' ",temp,m_tiaoj);
			break;
		default:
			strSQL.Format("%s",temp);
			break;
		}
	}
	else
	{
		if (m_bOracle)
		{
			temp.Format("select charDecode(姓名),charDecode(身份证明编号),考试次数,当日次数,charDecode(考车号),dateDecode(开始时间),dateDecode(考试时间) from examrecordindetail where (开始时间 between dateEncode(to_date('%s','yyyy-MM-dd hh24:mi:ss')) and dateEncode(to_date('%s','yyyy-MM-dd hh24:mi:ss')))",strST,strET);
		}
		else if (m_bSql)
		{
			temp.Format("select charDecode(姓名),charDecode(身份证明编号),考试次数,当日次数,charDecode(考车号),dateDecode(开始时间),dateDecode(考试时间) from examrecordindetail where (开始时间 between dateEncode(convert(datetime, '%s')) and dateEncode(convert(datetime, '%s')))", strST, strET);
		}
		
		switch(iComboxS)
		{
		case 1:
			strSQL.Format("%s and 姓名=charEncode('%s') ",temp,m_tiaoj);
			break;
		case 2:
			strSQL.Format("%s and 身份证明编号=charEncode('%s') ",temp,m_tiaoj);
			break;
		case 3:
			strSQL.Format("%s and 考车号=charEncode('%s') ",temp,m_tiaoj);
			break;
		default:
			strSQL.Format("%s",temp);
			break;
		}
	}

	UpdateList(strSQL);
}

void CCVideoCXDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMListView->uChanged==LVIF_STATE)
	{
		if(pNMListView->uNewState & LVIS_SELECTED)
		{
			strKCH=m_ListCtrl.GetItemText(pNMListView->iItem,5);
			strBfStime=m_ListCtrl.GetItemText(pNMListView->iItem,6);			
			strBfEtime=m_ListCtrl.GetItemText(pNMListView->iItem,7);
			UpdateData(FALSE);			
		}
	}
	*pResult = 0;
}


void CCVideoCXDlg::WriteConfig()
{
	CString BOOLFPath;
	BOOLFPath.Format(".\\VIDEOPLAYBACK.dat");
	_RecordsetPtr pRecordset;
//	_variant_t vIP, vDKH,vNAME,vPASS,vKCH,vTDH,vNID;
	_variant_t vat;
	CString strIP,strName,strPWD,strKCH,strTDH,strDKH;
	CString strSQL;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		strSQL.Format("select * from TBKVideo where Nid=6 or Nid is null ");
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		while (!pRecordset->adoEOF)
		{
			vat =pRecordset->GetCollect("编号");
			if (vat.vt != VT_NULL)
			{
				strKCH = (LPCSTR)_bstr_t(vat);
			}else{strKCH=_T("");}

			vat =pRecordset->GetCollect("设备IP");
			if (vat.vt != VT_NULL)
			{
				strIP = (LPCSTR)_bstr_t(vat);
			}else{strIP=_T("");}

			vat =pRecordset->GetCollect("用户名");
			if (vat.vt != VT_NULL)
			{
				strName = (LPCSTR)_bstr_t(vat);
			}else{strName=_T("");}

			vat =pRecordset->GetCollect("密码");
			if (vat.vt != VT_NULL)
			{
				strPWD = (LPCSTR)_bstr_t(vat);
			}else{strPWD=_T("");}


			vat =pRecordset->GetCollect("端口号");
			if (vat.vt != VT_NULL)
			{
				strDKH = (LPCSTR)_bstr_t(vat);
			}else{strDKH=_T("");}

			vat =pRecordset->GetCollect("通道号");
			if (vat.vt != VT_NULL)
			{
				strTDH = (LPCSTR)_bstr_t(vat);
			}else{strTDH=_T("");}

			if (!strKCH.IsEmpty())
			{
				WritePrivateProfileString(strKCH,"xmsbip",strIP,BOOLFPath);
				WritePrivateProfileString(strKCH,"xmsbuser",strName,BOOLFPath);
				WritePrivateProfileString(strKCH,"xmsbpwd",strPWD,BOOLFPath);
				WritePrivateProfileString(strKCH,"xmsbport",strDKH,BOOLFPath);
				WritePrivateProfileString(strKCH,"xmsbtd",strTDH,BOOLFPath);
			}

			
			pRecordset->MoveNext();
		}
		pRecordset->Close();
		pRecordset.Release();
		
	}
	catch (_com_error e) 
	{
		CString strErr;
		strErr.Format("读取TBKVideoDown列表失败!%s",e.Description());
		AfxMessageBox(strErr);
	}
	
}

void CCVideoCXDlg::OnBtnPz() 
{
	// TODO: Add your control notification handler code here
	typedef BOOL (* DLGPZVideo)();
	DLGPZVideo pzvideodlg;
	HINSTANCE dllHandlet;
	
	dllHandlet = LoadLibrary(TEXT("CBKVideoPZ.dll")); 
	if (dllHandlet == NULL) 
	{ 
		AfxMessageBox("加载CBKVideoPZ.dll失败!");
		return ; 
	}  
	pzvideodlg = (DLGPZVideo) GetProcAddress(dllHandlet,TEXT("DLGPZVideo"));	
	pzvideodlg();
	FreeLibrary(dllHandlet);
}

// KCHPPZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KCHPPZ.h"
#include "KCHPPZDlg.h"

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
// CKCHPPZDlg dialog

CKCHPPZDlg::CKCHPPZDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKCHPPZDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKCHPPZDlg)
	m_EditKCHP = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKCHPPZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKCHPPZDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_BH, m_ComboBoxBH);
	DDX_Text(pDX, IDC_EDIT1, m_EditKCHP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKCHPPZDlg, CDialog)
	//{{AFX_MSG_MAP(CKCHPPZDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKCHPPZDlg message handlers

BOOL CKCHPPZDlg::OnInitDialog()
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
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0,_T("编号"),LVCFMT_LEFT,80,-1);
	m_ListCtrl.InsertColumn(1,_T("考车号"),LVCFMT_LEFT,120,-1);
	m_ListCtrl.InsertColumn(2,_T("状态"),LVCFMT_LEFT,100,-1);
	m_ListCtrl.InsertColumn(3,_T("车牌"),LVCFMT_LEFT,200,-1);

	UpdateList();
	UpdateCombox();

	m_ComboBoxBH.SetCurSel(0);//
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKCHPPZDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKCHPPZDlg::OnPaint() 
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
HCURSOR CKCHPPZDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CKCHPPZDlg::OnInitSQL()
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
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
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

BOOL CKCHPPZDlg::UpdateCombox()
{
	int k=0;
	CString tempk;
	//	m_ComboBoxBH.ResetContent();
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		CString temp,errorcode;
		temp.Format("select 项目 from SysCfg where 项目 like '考车%%'");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("项目");
				if (vat.vt != VT_NULL)
				{
					errorcode = (LPCSTR)_bstr_t(vat);
				}
				m_ComboBoxBH.InsertString(k,errorcode);
				ComboboxArray.Add(errorcode);
				k++;
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		//TRACE("打开数据库异常：%s!\n",comError.ErrorMessage());
		tempk.Format("错误提示,更新Combox列表失败!,%s",comError.Description());
	}
	return TRUE;
}

BOOL CKCHPPZDlg::UpdateList()
{	
	int k=0;
	CString tempk;
	m_ListCtrl.DeleteAllItems();//清空列表
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		CString temp;
		temp.Format("select 项目,状态,备注 from SysCfg where 项目 like '考车%%'");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		CString strbh,strkch,strzt,strbz;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("项目");
				if (vat.vt != VT_NULL)
				{
					strkch = (LPCSTR)_bstr_t(vat);
				}
				else
				{
					strkch=_T("");
				}
				vat =pSet->GetCollect("状态");
				if (vat.vt != VT_NULL)
				{
					strzt = (LPCSTR)_bstr_t(vat);
				}
				else
				{
					strzt=_T("");
				}
				vat =pSet->GetCollect("备注");
				if (vat.vt != VT_NULL)
				{
					strbz = (LPCSTR)_bstr_t(vat);
				}
				else
				{
					strbz=_T("");
				}
				
				strbh.Format("%d",k+1);
				m_ListCtrl.InsertItem(k,strbh);
				m_ListCtrl.SetItemText(k,1,strkch);
				m_ListCtrl.SetItemText(k,2,strzt);
				m_ListCtrl.SetItemText(k,3,strbz);
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
		tempk.Format("错误提示,更新通道对应列表失败!,错误[%s]",comError.Description());
		AfxMessageBox(tempk);
	}
	return TRUE;
}

void CKCHPPZDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString temp;
	temp=m_ListCtrl.GetItemText(pNMListView->iItem,1);
	int nIndex = 0;
	nIndex=m_ComboBoxBH.FindString(nIndex,temp);
	m_ComboBoxBH.SetCurSel(nIndex);	//编号
	m_EditKCHP=m_ListCtrl.GetItemText(pNMListView->iItem,3);//用户
	
	UpdateData(FALSE);
	*pResult = 0;
}

void CKCHPPZDlg::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_EditKCHP.TrimLeft();
	m_EditKCHP.TrimRight();
	//////////////////////////////////////////////////////////////////////////
	_RecordsetPtr pRecordset;
	_variant_t vtemp;
	CString temp,temp1;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		int Count=m_ComboBoxBH.GetCurSel();		
		int n=m_ComboBoxBH.GetLBTextLen(Count);
		m_ComboBoxBH.GetLBText(Count,temp.GetBuffer(n));

		strSQL.Format("select * from Syscfg where 项目 = '%s' ", temp);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		if(pRecordset->RecordCount == 1)
		{
				pRecordset->PutCollect("项目",(_variant_t)temp);
				pRecordset->PutCollect("备注",(_variant_t)m_EditKCHP);
				
				pRecordset->Update();
			
		}	
		pRecordset->Close();
		pRecordset.Release();	
		AfxMessageBox("修改完成!");
	}
	catch (_com_error e) 
	{
		temp1.Format("错误提示,添加通道信息失败!!,%s",e.Description());
		AfxMessageBox(temp1);
	}
	UpdateList();
	
}

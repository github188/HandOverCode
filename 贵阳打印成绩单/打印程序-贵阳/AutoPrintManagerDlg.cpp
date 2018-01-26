// AutoPrintManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoPrintManager.h"
#include "AutoPrintManagerDlg.h"
#include "YLDlg.h"
#include "Pass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int iZJ=0;
HANDLE hEvent;
CString sdate;
BOOL BTNprint;//判断自动打印按键
BOOL BTnCheck;//勾选名单确认
extern CAutoPrintManagerApp theApp;

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
	afx_msg void OnMenuitem32772();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoPrintManagerDlg dialog


CAutoPrintManagerDlg::CAutoPrintManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoPrintManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoPrintManagerDlg)
	m_ESer = _T("127.0.0.1");
	m_EPwd = _T("");
	m_KCMC = _T("");
	m_Ezkzm5 = _T("");
	m_CheckSet1 = FALSE;
	BTnCheck=FALSE;
	BTNMake=FALSE;
	m_CheckSet2 = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//CAutoPrintManagerDlg teApp;

void CAutoPrintManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoPrintManagerDlg)
	DDX_Control(pDX, IDC_COMBO1, m_ZDprint);
	DDX_Control(pDX, IDC_CHECK1_KSY1, m_checkKSY1);
	DDX_Control(pDX, IDC_STATIC_JS, m_SText);
	DDX_Control(pDX, IDC_LIST_ALLKC2, m_ListMake);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_sDate);
	DDX_Control(pDX, IDC_COMBO3, m_CombDr);
	DDX_Control(pDX, IDC_LIST_ALLKC, m_ListAllKC);
	DDX_Text(pDX, IDC_EDIT2, m_ESer);
	DDX_Text(pDX, IDC_EDIT3, m_EUser);
	DDX_Text(pDX, IDC_EDIT4, m_EPwd);
	DDX_Text(pDX, IDC_EDIT1, m_KCMC);
	DDX_Text(pDX, IDC_EDIT5, m_Ezkzm5);
	DDX_Check(pDX, IDC_CHECK_ALL2, m_CheckSet1);
	DDX_Check(pDX, IDC_CHECK_ALL3, m_CheckSet2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAutoPrintManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoPrintManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_AutoPrint, OnBTNAutoPrint)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ALLKC, OnItemchangedListAllkc)
	ON_BN_CLICKED(IDC_BTNPrint, OnBTNPrint)
	ON_MESSAGE(WM_PRINTTHR_MSG,OnPrintThrMsg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNYL, OnBtnyl)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_AutoPrint2, OnMakeBtnStudent)
	ON_BN_CLICKED(IDC_CHECK_ALL2, OnCheckAll2)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonNAMEConfirmed)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALLKC2, OnRclickListAllkc2)
	ON_COMMAND(ID_MENUITEM32773, OnMenuitem32773)
	ON_COMMAND(ID_MENUITEM32774, OnMenuitem32774)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALLKC2, OnDblclkListAllkc2)
	ON_COMMAND(ID_MENUITEM32782, OnMenuitem32782)
	ON_COMMAND(ID_MENUITEM32783, OnMenuitem32783)
	ON_COMMAND(ID_MENUITEM32784, OnMenuitem32784)
	ON_COMMAND(ID_MENUITEM32785, OnMenuitem32785)
	ON_COMMAND(ID_MENUITEM32789, OnMenuitem32789)
	ON_COMMAND(ID_MENUITEM32786, OnMenuitem32786)
	ON_COMMAND(ID_MENUITEM32787, OnMenuitem32787)
	ON_COMMAND(ID_MENUITEM32788, OnMenuitem32788)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENUITEM32793, OnMenuitem32793)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALLKC, OnRclickListAllkc)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALLKC, OnDblclkListAllkc)
	ON_BN_CLICKED(IDC_CHECK1_KSY1, OnCheckKsy1)
	ON_COMMAND(ID_MENUITEM32800, OnMenuitem32800)
	ON_BN_CLICKED(IDC_CHECK_ALL3, OnCheckAll3)
	ON_BN_CLICKED(IDC_BTN_AutoPrint3, OnBTNAutoPrint1J)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoPrintManagerDlg message handlers


UINT ConSQLThread(LPVOID lpParam)
{
	//CoInitialize(NULL);
	WaitForSingleObject(hEvent,INFINITE);

	CListCtrl *m_List=(CListCtrl *)lpParam;
	
//	CComboBox *m_Combo=(CComboBox *)lpParam;
	int k=0;
	VARIANT cnt;
	_ConnectionPtr sqlpConn;
	sqlpConn.CreateInstance("ADODB.Connection");
	if(sqlpConn==NULL)
	{
		AfxMessageBox("初始化连接对象失败\n");
		return 0;
	}
	HRESULT res=-1;
	try
	{
		CString strLink;
		strLink.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=master",theApp.sESer,theApp.sUser,theApp.sPwd);
		res = sqlpConn->Open((_bstr_t)strLink,"","",0);
		if(res==S_OK)//0
		{
			cnt.vt = VT_INT;
			char *p ="select name from master..sysdatabases where name not in('master','model','msdb','Northwind','pubs','tempdb')";
			
			_RecordsetPtr pSet = sqlpConn->Execute(p,&cnt,adCmdUnknown);
			//m_Combo->ResetContent();
			if(pSet!=NULL)
			{
				while(!pSet->adoEOF)
				{
					_bstr_t spk = pSet->GetCollect("name");
					//::PostMessage(AfxGetMainWnd()->m_hWnd,WM_PRINTTHR_MSG,0,thri);
				//	m_Combo->InsertString(k,(char*)spk);
					pSet->MoveNext();
					k=k+1;
				}
				
				m_List->SetItemText(theApp.iwz,1,"连接数据库成功!");
				
			}
			else
			{
				AfxMessageBox("查询到记录为空\n");
				m_List->SetItemText(theApp.iwz,1,"查询到记录为空!");
			}
			sqlpConn->Close();
			//m_Combo->SetCurSel(0);
		}
		else
		{
			AfxMessageBox("连接数据库失败!");
			m_List->SetItemText(theApp.iwz,1,"连接数据库失败!");
			return 0;
		}
	}
	catch (_com_error )
	{
		AfxMessageBox("连接数据库失败! \n确认服务器名用户名及密码是否正确");
		m_List->SetItemText(theApp.iwz,1,"用户名及密码是否正确");
		//return 0;
	}	
	theApp.BOOLConSQL=FALSE;
	m_List->EnableWindow(TRUE);
	SetEvent(hEvent);
	return 0;
}



UINT AllPrintThread(LPVOID lpParam)
{
	CListCtrl *m_List=(CListCtrl *)lpParam;
	int len=theApp.strZKZM.GetSize();
	int Nkscs=0;
	CString strSQL;
	for (int k=0;k<len;k++)
	{
	//	AfxBeginThread(AllPrintThread,&m_info[i]);
		WaitForSingleObject(hEvent,INFINITE);
		HINSTANCE dllPrintSheetHandle;
			//////////////////////////////////////////////////////////////////////////
		dllPrintSheetHandle = LoadLibrary(TEXT("BKPrintSheet.dll")); 
		if (dllPrintSheetHandle) 
		{ 
			typedef BOOL (* DLLPrintSheet)(LPTSTR , int ,LPTSTR ,LPTSTR ,LPTSTR );
			DLLPrintSheet PrintSheetz=NULL;
			PrintSheetz = (DLLPrintSheet) GetProcAddress(dllPrintSheetHandle,TEXT("PrintSheet"));
			if (PrintSheetz)
			{
				Nkscs=atoi(theApp.strKSCS[k]);
				PrintSheetz(theApp.strZKZM[k].GetBuffer(0),Nkscs,theApp.sESer.GetBuffer(0),theApp.sUser.GetBuffer(0),theApp.sPwd.GetBuffer(0));	
			}
			FreeLibrary(dllPrintSheetHandle);
		} 
		SetEvent(hEvent);
		m_List->SetCheck(atoi(theApp.stri[k]),FALSE);
		m_List->SetRedraw(TRUE);
		m_List->Invalidate();
		m_List->UpdateWindow();
	//	m_List->SetCheck(atoi(theApp.stri[k]),FALSE);
	}
	m_List->EnableWindow(TRUE);
	BTnCheck=FALSE;
	//theApp.listend=TRUE;
	return 0;
}


UINT PrintThread(LPVOID lpParam)
{
	SQLThreadInfo* pInfo = (SQLThreadInfo*)lpParam;
	int thri=pInfo->Thri;

	Sleep(1000);
	CString  m_strTime;
	m_strTime=sdate;
	CString       strZKZM;
	int           nKSCS,nDRCS;
	_variant_t    var;	
	_ConnectionPtr m_pConn;
	while (TRUE)
	{
		CString Sqlstr;
		try
		{
			HRESULT res=-1;
			m_pConn.CreateInstance("ADODB.Connection");
			if(m_pConn==NULL)//CoInitialize（NULL）未初始化
				return FALSE;
			Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",pInfo->strSQLIP,pInfo->strUser,pInfo->strPWD,theApp.sVersion);
			res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
			if(res==S_OK)//0
			{
				TRACE("打开数据库成功!");
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_PRINTTHR_MSG,0,thri);
			}
			else
			{
				TRACE("打开数据库失败!");
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_PRINTTHR_MSG,1,thri);
				return FALSE;
			}
		}
		catch(_com_error e)		//捕捉异常
		{		
			if (m_pConn->GetState() == adStateOpen)
			{
				m_pConn->Close();
			}
			if (m_pConn)
			{
				m_pConn.Release();
				m_pConn = NULL;
			}
			CTime curTime =CTime::GetCurrentTime();
			CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
			FILE *fp=fopen("log.txt","a+");
			fprintf(fp,"[%s]连接数据库失败!\n",Data);
			fclose(fp);
		}
		try
		{
			if (pInfo->Bstop==FALSE)
			{	
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_PRINTTHR_MSG,4,thri);
				Sleep(0);
				return 0;
			}
			_RecordsetPtr pRecordset; 
			pRecordset.CreateInstance("ADODB.Recordset"); 
			pRecordset->CursorLocation = adUseClient; 
			if (iZJ==0)
				Sqlstr.Format("Set ARITHABORT ON;select top 1 dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数,打印状态 from studentinfo where 预考日期=dbo.dateEncode('%s') and 状态='1' and 打印状态 ='0' order by 考试日期 ",m_strTime);
			else if (iZJ==1)
				Sqlstr.Format("Set ARITHABORT ON;select top 1 dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数,打印状态 from studentinfo where 预考日期=dbo.dateEncode('%s') and (状态='2' and 当日次数='2')  and 打印状态 ='0'  order by  考试日期",m_strTime);
			else
				Sqlstr.Format("Set ARITHABORT ON;select top 1 dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数,打印状态 from studentinfo where 预考日期=dbo.dateEncode('%s') and (状态='1' or(状态='2' and 当日次数='2'))   and 打印状态 ='0'  order by  考试日期",m_strTime);
			pRecordset->Open(_variant_t(Sqlstr),_variant_t((IDispatch*)m_pConn, true),adOpenDynamic,adLockOptimistic, adCmdText);
			if (pRecordset->RecordCount == 0)
			{
				pRecordset->Close();
				pRecordset.Release();
				Sleep(1000);
				continue;
			}
			else if (pRecordset->RecordCount == 1)
			{
				var = pRecordset->GetCollect("准考证明编号");
				if ( var.vt != VT_NULL )
				{
					strZKZM = (LPCSTR)_bstr_t(var);
				}else{strZKZM=_T("");}
				var = pRecordset->GetCollect("考试次数");
				if ( var.vt != VT_NULL )
				{
					nKSCS = var.iVal;
				}else{nKSCS=0;}
				var = pRecordset->GetCollect("当日次数");
				if ( var.vt != VT_NULL )
				{
					nDRCS = var.iVal;
				}else{nDRCS=0;}
				pRecordset->PutCollect("打印状态",(_variant_t)"1");
				pRecordset->Update();
			}
			pRecordset->Close();
			pRecordset.Release();
		}
		catch(_com_error e)		//捕捉异常
		{
			CTime curTime =CTime::GetCurrentTime();
			CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
			FILE *fp=fopen("log.txt","a+");
			fprintf(fp,"[%s]OPEN失败[%s]!\n",Data,Sqlstr);
			fclose(fp);
			Sleep(10000);		
		}
		//////////////////////////////////////////////////////////////////////////
		//动态加载dll
		WaitForSingleObject(hEvent,INFINITE);
		//Sleep(10000);
		HINSTANCE dllPrintSheetHandle;
		//////////////////////////////////////////////////////////////////////////
		dllPrintSheetHandle = LoadLibrary(TEXT("BKPrintSheet.dll")); 
		if (dllPrintSheetHandle) 
		{ 
			typedef BOOL (* DLLPrintSheet)(LPTSTR , int ,LPTSTR ,LPTSTR ,LPTSTR );
			DLLPrintSheet PrintSheetz=NULL;
			PrintSheetz = (DLLPrintSheet) GetProcAddress(dllPrintSheetHandle,TEXT("PrintSheet"));
			if (PrintSheetz)
			{
				if(PrintSheetz(strZKZM.GetBuffer(0),nKSCS,pInfo->strSQLIP.GetBuffer(0),pInfo->strUser.GetBuffer(0),pInfo->strPWD.GetBuffer(0)))
				{
					CString strSQL;
					try
					{
						VARIANT cnt;
						cnt.vt = VT_INT;
						strSQL.Format("Set ARITHABORT ON;update studentinfo set 打印状态 ='2' where 准考证明编号=dbo.charEncode('%s')",strZKZM);
						m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
						::PostMessage(AfxGetMainWnd()->m_hWnd,WM_PRINTTHR_MSG,3,thri);
					}
					catch (_com_error e)
					{
						CTime curTime =CTime::GetCurrentTime();
						CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
						FILE *fp=fopen("log.txt","a+");
						fprintf(fp,"[%s]update error[%s]!\n",Data,strSQL);
						fclose(fp);
					}					
				}
			}
			FreeLibrary(dllPrintSheetHandle);
		} 
		//////////////////////////////////////////////////////////////////////////	
		SetEvent(hEvent);
		if (m_pConn->GetState() == adStateOpen)
		{
			m_pConn->Close();
		}
		Sleep(10000);
		
	}//while
	
	if (m_pConn->GetState() == adStateOpen)
	{
		m_pConn->Close();
	}
	if (m_pConn)
	{
		m_pConn.Release();
		m_pConn = NULL;
	}
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("log.txt","a+");
	fprintf(fp,"[%s]线程退出!\n",Data);
	fclose(fp);
	return 0;
}

LRESULT CAutoPrintManagerDlg::OnPrintThrMsg(WPARAM wParam, LPARAM lParam)
{
	int wpi =(int)wParam;
	int lpi =(int)lParam;
	CString temp;
	switch (wpi)
	{
	case 0:
		m_ListAllKC.SetItemText(lpi,1,"连接数据库成功!");
		break;
	case 1:
			m_ListAllKC.SetItemText(lpi,1,"连接数据库失败!");
	/*		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_PRINTTHR_MSG,2,lpi);*/
			SetTimer(lpi,30000,NULL);
			break;
	case 2:
		{
			m_ListAllKC.SetItemText(lpi,1,"读数据库出错,终止打印!");
	//		AfxMessageBox("a");
			m_info[lpi].strSQLIP=m_ListAllKC.GetItemText(lpi,3);
			m_info[lpi].strUser=m_ListAllKC.GetItemText(lpi,4);
			m_info[lpi].strPWD=m_ListAllKC.GetItemText(lpi,5);
			m_info[lpi].Thri=lpi;
			m_info[lpi].Bstop=TRUE;
			AfxBeginThread(PrintThread,&m_info[lpi]);
			CTime curTime =CTime::GetCurrentTime();
			CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
			FILE *fp=fopen("log.txt","a+");
			fprintf(fp,"[%s]线程开始!\n",Data);
			fclose(fp);
			break;
		   }
	case 3:
		{		
		//m_ListAllKC.SetItemText(lpi,1,"连接数据库失败!");
			dyrc[lpi]+=1;
			temp.Format("%d次",dyrc[lpi]);
			m_ListAllKC.SetItemText(lpi,2,temp);
			break;
		}
	case 4:
		m_ListAllKC.SetItemText(lpi,1,"停止自动打印!");
		//GetDlgItem(IDC_BTN_AutoPrint)->SetWindowText("恢复打印中");
		break;
	default :
		break;
	}
	return 0;
}
BOOL CAutoPrintManagerDlg::OnInitDialog()
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
	m_Set=FALSE;
	theApp.boolui=FALSE;
	theApp.BOOLConSQL=FALSE;


	GetCurrentDirectory(MAX_PATH,sstrPath.GetBuffer(MAX_PATH));  //获取当前绝对路径
	sstrPath.ReleaseBuffer();   

	::GetCurrentDirectory(MAX_PATH, FPath);
	strcat(FPath, "\\cfg.ini");
	CFile ConfigFile;
	if(!ConfigFile.Open(FPath, CFile::modeCreate||CFile::modeRead, NULL))
	{
		//AfxMessageBox("找不到配置文件!");
		TRACE("找不到配置文件!");
		return FALSE;
	}
	ConfigFile.Close();
	CString sTemp;
	::GetPrivateProfileString("config", "KGprint", NULL, sTemp.GetBuffer(50), 50, FPath); 
	sTemp.ReleaseBuffer();
	if (sTemp==_T("0"))
	{
		m_checkKSY1.SetCheck(TRUE); //默认打印考试员
		m_checkKSY1.SetWindowText("关闭考官签字");
	} 
	else
	{
		m_checkKSY1.SetCheck(FALSE); //默认打印考试员
		m_checkKSY1.SetWindowText("启用考官签字");
	}

	GetPrivateProfileString("Switch","data","",theApp.sVersion.GetBuffer(100),100,"./cfg.ini");
	theApp.sVersion.ReleaseBuffer();
	if (theApp.sVersion==_T(""))
	{
		::WritePrivateProfileString("Switch","data","2ndDrivingTestSystem","./cfg.ini"); 
		theApp.sVersion=_T("2ndDrivingTestSystem");
		theApp.iCJ=80;
	}else if (theApp.sVersion==_T("2ndDrivingTestSystem"))
	{
		theApp.iCJ=80;
	}else if (theApp.sVersion==_T("3rdDrivingTestSystem"))
		theApp.iCJ=90;
	STRS=0;
	hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	SetEvent(hEvent);
	GetWindowRect(&m_LargerRect);
	//GetDlgItem(IDC_SEP)->GetWindowRect(&SepRect);
	m_SmallRect.left=m_LargerRect.left;
	m_SmallRect.top= m_LargerRect.top;
	m_SmallRect.right=m_LargerRect.right/1.8;//右边
	m_SmallRect.bottom=m_LargerRect.bottom;		
	SetWindowPos(NULL,0,0,m_SmallRect.Width(),m_SmallRect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	
/*
	m_ListMake.SetExtendedStyle(LVS_EX_FLATSB       //扁平风格显示滚动条
// 		|LVS_EX_FULLROWSELECT              //允许整行选中 
// 		|LVS_EX_HEADERDRAGDROP           //允许整列拖动
		|LVS_EX_ONECLICKACTIVATE          //单击选中项
		|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);                 //画出网格线
		*/
	m_ListMake.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	//m_ListMake.InsertColumn(0,_T("序号"),LVCFMT_LEFT,0,-1);
	m_ListMake.InsertColumn(0,_T("准考证明编号"),LVCFMT_LEFT,120,-1);
	m_ListMake.InsertColumn(1,_T("考生姓名"),LVCFMT_LEFT,80,-1);
	m_ListMake.InsertColumn(2,_T("身份证明编号"),LVCFMT_LEFT,150,-1);
	m_ListMake.InsertColumn(3,_T("打印状态"),LVCFMT_LEFT,0,-1);
	m_ListMake.InsertColumn(4,_T("考试科目"),LVCFMT_LEFT,70,-1);
	m_ListMake.InsertColumn(5,_T("成绩"),LVCFMT_LEFT,50,-1);
	m_ListMake.InsertColumn(6,_T("考试次数"),LVCFMT_LEFT,0,-1);
//	m_commd.InsertString(0,"合格");
//	m_commd.InsertString(1,"不合格");
//	m_commd.SetCurSel(0);

	if (!CreatDBCon())
	{
		CDialog::OnCancel();
	}
// 	m_ListAllKC.SetExtendedStyle(LVS_EX_FLATSB       //扁平风格显示滚动条
// 		|LVS_EX_FULLROWSELECT              //允许整行选中
// 	//	|LVS_EX_HEADERDRAGDROP           //允许整列拖动
// 		|LVS_EX_ONECLICKACTIVATE          //单击选中项
// 		|LVS_EX_GRIDLINES);                 //画出网格线
	m_ListAllKC.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_ListAllKC.InsertColumn(0,_T("考场名称"),LVCFMT_LEFT,160,-1);
	m_ListAllKC.InsertColumn(1,_T("状态"),LVCFMT_LEFT,220,-1);
	m_ListAllKC.InsertColumn(2,_T("打印次数"),LVCFMT_LEFT,80,-1);
	m_ListAllKC.InsertColumn(3,_T("IP"),LVCFMT_LEFT,0,-1);
	m_ListAllKC.InsertColumn(4,_T("U"),LVCFMT_LEFT,0,-1);
	m_ListAllKC.InsertColumn(5,_T("P"),LVCFMT_LEFT,0,-1);
	
	m_CombDr.InsertString(0,"身份证明编号");
	m_CombDr.InsertString(1,"准考证明编号");
	m_CombDr.SetCurSel(0);
	
	m_ZDprint.InsertString(0,"合格考生");
	m_ZDprint.InsertString(1,"不合格考生");
	m_ZDprint.InsertString(2,"全部考生");

	::GetPrivateProfileString("config", "ZJprint", "0", sTemp.GetBuffer(50), 50, FPath); 
	sTemp.ReleaseBuffer();
	if (sTemp==_T("0"))
		m_ZDprint.SetCurSel(0);
	else if (sTemp==_T("1"))
		m_ZDprint.SetCurSel(1);
	else 
		m_ZDprint.SetCurSel(2);


	UpdateList();
	for (int i=0;i<50;i++)
	{
		dyrc[i]=0;
	}

	hmodule=LoadLibrary("excel004.dll"); //显示加载DLL001.dll文件   //防止DLL路径出错
	if (hmodule==NULL)
	{
		AfxMessageBox("加载excel004.dll失败！");
		return FALSE;
	}

	//得到DLL001.dll中口函数Add()的入口地址
	lpfnadd =(ADDFUNC*)GetProcAddress(hmodule,"SetDVRHDVI");
	if (lpfnadd==NULL)
	{
		AfxMessageBox("在excel004.dll为找到SetDVRHDVI()函数");
		return FALSE;
		
	}


	hmodule2=LoadLibrary("Excel2014.dll"); //显示加载Excel2014.dll文件   //防止DLL路径出错
	if (hmodule2==NULL)
	{
		AfxMessageBox("加载Excel2014.dll失败！");
		return FALSE;
	}
	
	//得到DLL001.dll中口函数Add()的入口地址
	lpfnadd2 =(ADDFUNCNUM*)GetProcAddress(hmodule2,"SetDVRHDVI");
	if (lpfnadd2==NULL)
	{
		AfxMessageBox("在Excel2014.dll为找到SetDVRHDVI()函数");
		return FALSE;
		
	}



	hmodule3=LoadLibrary("Excel2015.dll"); //显示加载Excel2014.dll文件   //防止DLL路径出错
	if (hmodule3==NULL)
	{
		AfxMessageBox("加载Excel2015.dll失败！");
		return FALSE;
	}
	
	//得到DLL001.dll中口函数Add()的入口地址
	lpfnadd3 =(ADDFUNCNUM*)GetProcAddress(hmodule3,"SetDVRHDVI");
	if (lpfnadd3==NULL)
	{
		AfxMessageBox("在Excel2015.dll为找到SetDVRHDVI()函数");
		return FALSE;
		
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoPrintManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAutoPrintManagerDlg::OnPaint() 
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
HCURSOR CAutoPrintManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CAutoPrintManagerDlg::OnBTNAutoPrint() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty() || m_KCMC.IsEmpty())
	{
		AfxMessageBox("考场名称,IP,用户名不能为空!");
		return ;
	}
	iZJ=m_ZDprint.GetCurSel();
	CString strZJ;
	strZJ.Format("%d",iZJ);
	WritePrivateProfileString("config","ZJprint",strZJ,FPath);
//	return ;
	if (BTnCheck==TRUE)
	{
		MessageBox("已有考生在打印中,请稍等");
	}else
	{
	
		if (BTNprint==FALSE)
		{
			BTNprint=TRUE;//判断自动打印按键
			GetDlgItem(IDC_BTN_AutoPrint)->SetWindowText("停止自动打印");
			//GetDlgItem(IDC_BTN_AutoPrint)->EnableWindow(FALSE);
			int nItem=m_ListAllKC.GetItemCount();
			CTime time;
			m_sDate.GetTime(time);
			sdate=time.Format("'%Y-%m-%d'");  
			for(int i = 0; i <nItem ; i++)
			{
				if (m_ListAllKC.GetCheck(i))//
				{
					m_info[i].strSQLIP=m_ListAllKC.GetItemText(i,3);
					m_info[i].strUser=m_ListAllKC.GetItemText(i,4);
					m_info[i].strPWD=m_ListAllKC.GetItemText(i,5);
					m_info[i].Thri=i;
					m_info[i].Bstop=TRUE;
					//AfxMessageBox("1111");
					AfxBeginThread(PrintThread,&m_info[i]);
					Sleep(100);
				}
			}
			GetDlgItem(IDC_LIST_ALLKC)->EnableWindow(FALSE);
		} 
		else
		{
			int nItem=m_ListAllKC.GetItemCount();
			int iCount=0;
			CTime time;
			m_sDate.GetTime(time);
			sdate=time.Format("'%Y-%m-%d'");  
			for(int i = 0; i <nItem ; i++)
			{
				if (m_ListAllKC.GetCheck(i))//
				{
					m_info[i].Bstop=FALSE;
					//m_ListAllKC.SetCheck(i,FALSE);
				}	
				else	
				{
					iCount++;
					m_info[i].Bstop=TRUE;
				}
			}
			BTNprint=FALSE;
			GetDlgItem(IDC_LIST_ALLKC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_AutoPrint)->SetWindowText("开始自动打印");
		}             
	} 
}



void CAutoPrintManagerDlg::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	if (m_KCMC.IsEmpty() )
	{
		AfxMessageBox("请选择要删除的考场!");
		return ;
	}	
	CPass dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		try
		{
			_RecordsetPtr pRecordset;
			CString strSQL;	
			_variant_t vTemp;
			CString sPW;
			BOOL bFlag = TRUE;
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;		
			strSQL.Format("select * from KCConfig where 考场名称 = '%s'", m_KCMC);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnDB, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			if(pRecordset->RecordCount == 0)
			{
				AfxMessageBox("数据库中无该记录!");
				return;
			}
			else
			{
				pRecordset->Delete(adAffectCurrent);		
				pRecordset->UpdateBatch(adAffectAll);
			}		
			
			pRecordset->Close();
			pRecordset.Release();
			temp.Format("删除考场[%s]成功!",m_KCMC);
			AfxMessageBox(temp);
			m_KCMC = _T("");
			m_ESer = _T("");
			//m_EUser = _T("");
			m_EPwd = _T("");
			UpdateData(FALSE);
			UpdateList();
		}
		catch (_com_error e) 
		{		
			temp.Format("错误提示,删除考场失败!,%s",e.Description());
			AfxMessageBox(temp);
		}
	}else if(nResponse == IDCANCEL)
	{
		MessageBox("密码错误");
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	
}

void CAutoPrintManagerDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	

	if (m_ESer.IsEmpty() || m_EUser.IsEmpty() || m_KCMC.IsEmpty())
	{
		AfxMessageBox("考场名称,IP,用户名不能为空!");
		return ;
	}
	CPass dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		_RecordsetPtr pRecordset;
		_variant_t vtemp;
		
		CString temp,temp1;
		try
		{
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;
			CString strSQL;	
			strSQL.Format("select * from KCConfig where 考场名称 = '%s'", m_KCMC);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnDB, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			temp1.Format("911%s",m_EPwd);
			temp1.MakeReverse();
			
			if(pRecordset->RecordCount == 1)
			{
				pRecordset->PutCollect("考场名称",(_variant_t)m_KCMC);
				pRecordset->PutCollect("IP",(_variant_t)m_ESer);
				pRecordset->PutCollect("SUSER",(_variant_t)m_EUser);
				pRecordset->PutCollect("SPWD",(_variant_t)temp1);
				//pRecordset->PutCollect("SPWD",(_variant_t)m_EPwd);
				pRecordset->Update();
			}
			else if(pRecordset->RecordCount == 0)
			{
				pRecordset->AddNew();
				pRecordset->PutCollect("考场名称",(_variant_t)m_KCMC);
				pRecordset->PutCollect("IP",(_variant_t)m_ESer);
				pRecordset->PutCollect("SUSER",(_variant_t)m_EUser);
				pRecordset->PutCollect("SPWD",(_variant_t)temp1);
				pRecordset->Update();
			}
			
			pRecordset->Close();
			pRecordset.Release();
			UpdateList();
			
		}
		catch (_com_error e) 
		{
			temp1.Format("错误提示,添加考场失败!!,%s",e.Description());
			AfxMessageBox(temp1);
		}	
//	GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if(nResponse == IDCANCEL)
	{
		MessageBox("密码错误");
		// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
	}
	
}


BOOL CAutoPrintManagerDlg::CreatDBCon()
{
	m_pConnDB.CreateInstance("ADODB.Connection");
	if(m_pConnDB==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	try
	{
		//建立数据库连接
		res = m_pConnDB->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=kc.mdb;User ID='admin';Password=;Jet OLEDB:Database Password='ustbzy'","","",adModeUnknown);		
		//res = m_pConnDB->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=kc.mdb ","","",adModeUnknown);		
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
		errormessage.Format("连接mdb数据库失败!\r\n错误信息:%s",e.ErrorMessage());
		AfxMessageBox(errormessage);
		return FALSE;
	}	
	return TRUE;
}











void CAutoPrintManagerDlg::UpdateList()
{
	int k=0;
	CString tempk;
	m_ListAllKC.DeleteAllItems();//清空列表
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		CString temp;
		temp.Format("SELECT * FROM KCConfig");
		_RecordsetPtr pSet =m_pConnDB->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		CString strName,strIP,strUser,strPWD;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("考场名称");
				if (vat.vt != VT_NULL)
				{
					strName = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("IP");
				if (vat.vt != VT_NULL)
				{
					strIP = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("SUSER");
				if (vat.vt != VT_NULL)
				{
					strUser = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("SPWD");
				if (vat.vt != VT_NULL)
				{
					temp = (LPCSTR)_bstr_t(vat);
					temp.MakeReverse();
					strPWD=temp.Mid(3);
				}
				m_ListAllKC.InsertItem(k,strName);
				m_ListAllKC.SetItemText(k,1,"未开始打印");
				m_ListAllKC.SetItemText(k,2,"0");
				m_ListAllKC.SetItemText(k,3,strIP);
				m_ListAllKC.SetItemText(k,4,strUser);
				m_ListAllKC.SetItemText(k,5,strPWD);
				k++;
				pSet->MoveNext();
			}//while
		}//if
		
	}
	catch (_com_error &comError)
	{
		//TRACE("打开数据库异常：%s!\n",comError.ErrorMessage());
		tempk.Format("错误提示,更新驾校列表失败!,%s",comError.Description());
	}
}

void CAutoPrintManagerDlg::OnItemchangedListAllkc(NMHDR* pNMHDR, LRESULT* pResult) 
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//TODO: Add your control notification handler code here
	m_KCMC=m_ListAllKC.GetItemText(pNMListView->iItem,0);
	m_ESer=m_ListAllKC.GetItemText(pNMListView->iItem,3);
	m_EUser=m_ListAllKC.GetItemText(pNMListView->iItem,4);
	m_EPwd=m_ListAllKC.GetItemText(pNMListView->iItem,5);
	theApp.iwz=pNMListView->iItem;
	theApp.sESer=m_ESer;
	theApp.sUser=m_EUser;
	theApp.sPwd=m_EPwd;
	SetWindowPos(NULL,0,0,m_SmallRect.Width(),m_SmallRect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	m_Set=FALSE;
	UpdateData(FALSE);
	*pResult = 0;
}

void CAutoPrintManagerDlg::OnBTNPrint() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty())
	{
		MessageBox("请选择考场,数据库IP或用户不能为空!");
		return ;
	}
	if (m_Ezkzm5.IsEmpty())
	{
		MessageBox("请输入准考证明编号!");
		return ;
	}
	PrintSchoolReport();
}


void CAutoPrintManagerDlg::PrintSchoolReport()
{
	CString Sqlstr;
	_ConnectionPtr m_pConn;
	try
	{
		
		m_pConn.CreateInstance("ADODB.Connection");
		if(m_pConn==NULL)//CoInitialize（NULL）未初始化
		{
			TRACE("初始化连接对象失败\n");
			return ;
		}
		HRESULT res=-1;
		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",m_ESer,m_EUser,m_EPwd,theApp.sVersion);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			return ;
		}
	}
	catch(_com_error e)		//捕捉异常
	{		
		AfxMessageBox("连接数据库失败!");
		return ;
	}
	CString zkzmbhm;
	int           nKSCS,nDRCS;
	_variant_t    var;
	int zkzorsfzi =m_CombDr.GetCurSel();
	try
	{
		_RecordsetPtr pRecordset; 
		pRecordset.CreateInstance("ADODB.Recordset"); 
		pRecordset->CursorLocation = adUseClient;   
		if (zkzorsfzi ==0)
		{
			Sqlstr.Format("Set ARITHABORT ON;select dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数 from studentinfo where 身份证明编号=dbo.charEncode('%s') ",m_Ezkzm5);
		}
		else
		{
			Sqlstr.Format("Set ARITHABORT ON;select dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数 from studentinfo where 准考证明编号=dbo.charEncode('%s') ",m_Ezkzm5);
		}		
		TRACE(Sqlstr);
		pRecordset->Open(_variant_t(Sqlstr),_variant_t((IDispatch*)m_pConn, true),adOpenDynamic,adLockOptimistic, adCmdText);
		
		if (pRecordset->RecordCount == 1)
		{
			var = pRecordset->GetCollect("考试次数");
			if ( var.vt != VT_NULL )
			{
				nKSCS = var.iVal;
			}
			else{nKSCS=0;}
			var = pRecordset->GetCollect("当日次数");
			if ( var.vt != VT_NULL )
			{
				nDRCS = var.iVal;
			}
			else{nDRCS=0;}
			var = pRecordset->GetCollect("准考证明编号");
			if ( var.vt != VT_NULL )
			{
				zkzmbhm = (LPCSTR)_bstr_t(var);
			}
		}
		if (pRecordset->RecordCount == 0)
		{
			if (zkzorsfzi ==0)
			{
				Sqlstr.Format("未找到身份证明编号为%s 的考生\n请输入正确的身份证明编号",m_Ezkzm5);
			}
			else
			{
				Sqlstr.Format("未找到准考证明编号为%s 的考生\n请输入正确的准考证明编号",m_Ezkzm5);
			}
			AfxMessageBox(Sqlstr);
			pRecordset->Close();
			pRecordset.Release();
			if (m_pConn->GetState() == adStateOpen)
			{
				m_pConn->Close();
			}
			if (m_pConn)
			{
				m_pConn.Release();
				m_pConn = NULL;
			}
			return ;
		}
		pRecordset->Close();
		pRecordset.Release();
	}
	catch(_com_error e)		//捕捉异常
	{
		AfxMessageBox("查询数据库失败!请稍后重试!");
		return ;
	}
	if (m_pConn->GetState() == adStateOpen)
	{
		m_pConn->Close();
	}
	if (m_pConn)
	{
		m_pConn.Release();
		m_pConn = NULL;
	}
// 	int ikscs,idrcs;
// 	ikscs =m_CombKs.GetCurSel()+1;
// 	idrcs =m_CombDr.GetCurSel()+1;
	//////////////////////////////////////////////////////////////////////////	
	WaitForSingleObject(hEvent,INFINITE);
	HINSTANCE dllPrintSheetHandle;
	//////////////////////////////////////////////////////////////////////////
	dllPrintSheetHandle = LoadLibrary(TEXT("BKPrintSheet.dll")); 
	if (dllPrintSheetHandle) 
	{ 
		typedef BOOL (* DLLPrintSheet)(LPTSTR , int ,LPTSTR ,LPTSTR ,LPTSTR );
		DLLPrintSheet PrintSheetz=NULL;
		PrintSheetz = (DLLPrintSheet) GetProcAddress(dllPrintSheetHandle,TEXT("PrintSheet"));
		if (PrintSheetz)
		{
			PrintSheetz(zkzmbhm.GetBuffer(0),nKSCS,m_ESer.GetBuffer(0),m_EUser.GetBuffer(0),m_EPwd.GetBuffer(0));				
		}
		FreeLibrary(dllPrintSheetHandle);
	} 
	SetEvent(hEvent);
}

/*
void CAutoPrintManagerDlg::OnBtngaoji() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty())
	{
		MessageBox("请选择考场,数据库IP或用户不能为空!");
		return ;
	}
	CString temp,temp1,Sqlstr;
	temp.Format("是否要查询%s的数据!",m_KCMC);
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		if(!(GetPrivateProfileString(m_KCMC,"ServerPZ","",temp1.GetBuffer(MAX_PATH),MAX_PATH,".\\SQLCFG.DLL")))
		{
			Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",m_ESer,m_EUser,m_EPwd,"2ndDrivingTestSystem");
			Sqlstr= Decode(Sqlstr);
			CTime curTime =CTime::GetCurrentTime();
			CTime cerrtime(1446096854);
			if (cerrtime <= curTime )
			{
				Sqlstr+="SQLLINKERROR";
			}
			WritePrivateProfileString(m_KCMC,"ServerPZ",Sqlstr,".\\SQLCFG.DLL");
		}
		ShellExecute(NULL,"open","CBKMFSearch.exe",m_KCMC,NULL,SW_SHOW);
		
	}
}
*/
CString CAutoPrintManagerDlg::Decode(CString str)
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
//刷新考试状态栏
void CAutoPrintManagerDlg::UpDateExamStut()
{

	_RecordsetPtr pRecordset;
	CString strSQL;
	CString sExamStut;
	
	int iExamSuc = 0; //合格人数    zt=1
	int iExamFla = 0; //不合格人数  zt=2
	int iExaming = 0; //在考人数    zt=0
	int iExamAll = 0; //所有人数    RecordCount
	int iLogNumb = 0; //已登陆人数  "登录状态"
	
	int iWaitNumb= 0; //未考人数    iLogNumb - (iExamSuc+iExamFla+iExaming)
	int iExamEnd = 0; //考试结束人数  iExamFla + iExamSuc
	//////////////////////////////////////////////////////////////////////////
	HRESULT hr;		//记录操作结果
	CString temp;

	CTime curTimes = CTime::GetCurrentTime();
	_ConnectionPtr m_Connection;
	try
	{
		hr = m_Connection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			//strSQL.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",IPAddress,"2ndDrivingTestSystem","sa","ustbzy");
			strSQL.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",m_ESer,theApp.sVersion,m_EUser,m_EPwd);
			m_Connection->Open((_bstr_t)strSQL,"","",adModeUnknown); 
		}
	}
	catch (_com_error e)
	{
		CString Data=curTimes.Format("%Y-%m-%d %H:%M:%S");
		FILE *fp=fopen("log.txt","a+");
		fprintf(fp,"[%s]连接数据库失败!\n",Data);
		fclose(fp);
		Data.Format("连接%s数据库失败!",m_KCMC);
		AfxMessageBox(Data);
		return ;
	}
	_variant_t vZT,vLog,vksy;
	_variant_t vTemp;
	CString tempv,stime,sksy;
// 	CString m_strTime;
// 	m_strTime.Format("%d-%d-%d",curTimes.GetYear(),curTimes.GetMonth(),curTimes.GetDay());
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	try
	{
		/////////////////////////////////////
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("Set ARITHABORT ON;SELECT 状态,考试员1,登录状态 FROM StudentInfo WHERE 预考日期 = dbo.dateEncode('%s') ",stime);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_Connection, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		iExamAll = pRecordset->RecordCount; //所有人数
		while(!pRecordset->adoEOF)
		{
			vZT = pRecordset->GetCollect("状态");
			tempv=(LPCSTR)_bstr_t(vZT);
			if(strcmp(tempv,"1")==0)
			{
				iExamSuc++;  //合格
			} 
			else if(strcmp(tempv,"2")==0)
			{
				iExamFla++;  //不合格
			}
			else if(strcmp(tempv,"3")==0)
			{
				iExaming++;  //考试中
			}
			vksy = pRecordset->GetCollect("考试员1");
			sksy=(LPCSTR)_bstr_t(vLog);
			vLog = pRecordset->GetCollect("登录状态");
			tempv=(LPCSTR)_bstr_t(vLog);
			if(strcmp(tempv,"已登录")==0)
			{
				iLogNumb++;  //已登陆人数
			}
			pRecordset->MoveNext();
		}
		
		iWaitNumb = iLogNumb - (iExamSuc+iExamFla+iExaming); //未考人数 
		iExamEnd  = iExamFla + iExamSuc;   //考试结束人数
		
		pRecordset->Close();
		pRecordset.Release();
		
	}
	catch (_com_error e)
	{
 		AfxMessageBox("计算考试动态结果出现错误!");
// 		AfxMessageBox(e.Description());
		return ;
	}
	if (m_Connection->GetState() == adStateOpen)
	{
		m_Connection->Close();
	}
	if (m_Connection)
	{
		m_Connection.Release();
		m_Connection = NULL;
	}
	int ccc=iExamEnd?iExamEnd:1;
	sExamStut.Format("考场%s:共%d名考生,已登录%d考生,考试中%d人,已考%d人,合格%d人,合格率%2.2f%%,考试员:%d",m_KCMC,iExamAll,iLogNumb,iExaming,iExamEnd,iExamSuc,(float)((float)(iExamSuc*100)/ccc),sksy);
	GetDlgItem(IDC_EXAM_STAT)->SetWindowText(sExamStut);
	
}

void CAutoPrintManagerDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty())
	{
		MessageBox("请选择考场,数据库IP或用户不能为空!");
		return ;
	}
	CString temp;
	temp.Format("是否要查询考场名称为 [%s] 的考试合格数据!",m_KCMC);
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		UpDateExamStut();
	}
}

void CAutoPrintManagerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	CString temp;
	temp.Format("%d",nIDEvent);
	//AfxMessageBox(temp);
	m_info[nIDEvent].strSQLIP=m_ListAllKC.GetItemText(nIDEvent,3);
	m_info[nIDEvent].strUser=m_ListAllKC.GetItemText(nIDEvent,4);
	m_info[nIDEvent].strPWD=m_ListAllKC.GetItemText(nIDEvent,5);
	m_info[nIDEvent].Thri=nIDEvent;
	AfxBeginThread(PrintThread,&m_info[nIDEvent]);
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("log.txt","a+");
	fprintf(fp,"[%s]OnTimer线程开始!\n",Data);
	fclose(fp);
	CDialog::OnTimer(nIDEvent);
}

void CAutoPrintManagerDlg::OnBtnyl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty())
	{
		MessageBox("请选择考场,数据库IP或用户不能为空!");
		return ;
	}
	if (m_Ezkzm5.IsEmpty())
	{
		MessageBox("请输入准考证明编号!");
		return ;
	}
	CString Sqlstr;
	_ConnectionPtr m_pConn;
	try
	{
		
		m_pConn.CreateInstance("ADODB.Connection");
		if(m_pConn==NULL)//CoInitialize（NULL）未初始化
		{
			TRACE("初始化连接对象失败\n");
			return ;
		}
		HRESULT res=-1;
		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",m_ESer,m_EUser,m_EPwd,theApp.sVersion);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			return ;
		}
	}
	catch(_com_error e)		//捕捉异常
	{		
		AfxMessageBox("连接数据库失败!");
		return ;
	}
	CString zkzmbhm;
	int           nKSCS,nDRCS;
	_variant_t    var;
	int zkzorsfzi =m_CombDr.GetCurSel();
	try
	{
		_RecordsetPtr pRecordset; 
		pRecordset.CreateInstance("ADODB.Recordset"); 
		pRecordset->CursorLocation = adUseClient;   
		if (zkzorsfzi ==0)
		{
			Sqlstr.Format("Set ARITHABORT ON;select dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数 from studentinfo where 身份证明编号=dbo.charEncode('%s') ",m_Ezkzm5);
		}
		else
		{
			Sqlstr.Format("Set ARITHABORT ON;select dbo.charDecode(准考证明编号) as 准考证明编号,考试次数,当日次数 from studentinfo where 准考证明编号=dbo.charEncode('%s') ",m_Ezkzm5);
		}		
		TRACE(Sqlstr);
		pRecordset->Open(_variant_t(Sqlstr),_variant_t((IDispatch*)m_pConn, true),adOpenDynamic,adLockOptimistic, adCmdText);
		
		if (pRecordset->RecordCount == 1)
		{
			var = pRecordset->GetCollect("考试次数");
			if ( var.vt != VT_NULL )
			{
				nKSCS = var.iVal;
			}
			else{nKSCS=0;}
			var = pRecordset->GetCollect("当日次数");
			if ( var.vt != VT_NULL )
			{
				nDRCS = var.iVal;
			}
			else{nDRCS=0;}
			
			var = pRecordset->GetCollect("准考证明编号");
			if ( var.vt != VT_NULL )
			{
				zkzmbhm = (LPCSTR)_bstr_t(var);
			}
		}
		if (pRecordset->RecordCount == 0)
		{
			if (zkzorsfzi ==0)
			{
				Sqlstr.Format("未找到身份证明编号为%s 的考生\n请输入正确的身份证明编号",m_Ezkzm5);
			}
			else
			{
				Sqlstr.Format("未找到准考证明编号为%s 的考生\n请输入正确的准考证明编号",m_Ezkzm5);
			}
			AfxMessageBox(Sqlstr);
			pRecordset->Close();
			pRecordset.Release();
			if (m_pConn->GetState() == adStateOpen)
			{
				m_pConn->Close();
			}
			if (m_pConn)
			{
				m_pConn.Release();
				m_pConn = NULL;
			}
			return ;
		}
		pRecordset->Close();
		pRecordset.Release();
	}
	catch(_com_error e)		//捕捉异常
	{
		AfxMessageBox("查询数据库失败!请稍后重试!");
		return ;
	}
	if (m_pConn->GetState() == adStateOpen)
	{
		m_pConn->Close();
	}
	if (m_pConn)
	{
		m_pConn.Release();
		m_pConn = NULL;
	}

// 	int ikscs,idrcs;
// 	ikscs =m_CombKs.GetCurSel()+1;
// 	idrcs =m_CombDr.GetCurSel()+1;
	//////////////////////////////////////////////////////////////////////////	
// 	WaitForSingleObject(hEvent,INFINITE);
	HINSTANCE dllPrintSheetHandle;
// 	//////////////////////////////////////////////////////////////////////////
	dllPrintSheetHandle = LoadLibrary(TEXT("BKPrintSheetEx.dll")); 
	if (dllPrintSheetHandle) 
	{ 
		typedef BOOL (* DLLPrintSheet)(LPTSTR , int ,LPTSTR ,LPTSTR ,LPTSTR );
		DLLPrintSheet PrintSheetz=NULL;
		PrintSheetz = (DLLPrintSheet) GetProcAddress(dllPrintSheetHandle,TEXT("PrintSheetEx"));
		if (PrintSheetz)
		{
			PrintSheetz(zkzmbhm.GetBuffer(0),nKSCS,m_ESer.GetBuffer(0),m_EUser.GetBuffer(0),m_EPwd.GetBuffer(0));				
		}
		FreeLibrary(dllPrintSheetHandle);
	} 
	CYLDlg yldlg;
	yldlg.DoModal();
// 	SetEvent(hEvent);
}

void CAutoPrintManagerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	FreeLibrary(hmodule); //卸载动态链接库DLL001.dll
	CDialog::OnClose();
}


BOOL CAutoPrintManagerDlg::ConSQLCon(_ConnectionPtr &m_pConn)
{
	
	HRESULT res=-1;
	CString Sqlstr;
	try
	{
		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",m_ESer,m_EUser,m_EPwd,theApp.sVersion);
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
		CTime curTime =CTime::GetCurrentTime();
		CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
		FILE *fp=fopen("log.txt","a+");
		fprintf(fp,"[%s]连接数据库失败!\n",Data);
		fclose(fp);
		return FALSE;
	}
	return TRUE;
}



BOOL CAutoPrintManagerDlg::ReadSQLINFO(CString strSQL,_ConnectionPtr m_pConn)
{
	CString   strZKZM,strXM,strTEXT,strKSKM,strDYZT,strSFZM;
	int       NKSCJ,NKSCS,k=0;
	_variant_t var;
	try
	{
		_RecordsetPtr pRecordset; 
		pRecordset.CreateInstance("ADODB.Recordset"); 
		pRecordset->CursorLocation = adUseClient;   
		pRecordset->Open(_variant_t(strSQL),_variant_t((IDispatch*)m_pConn, true),adOpenDynamic,adLockOptimistic, adCmdText);
		if (pRecordset->RecordCount == 0)
		{
			STRS=k;
			pRecordset->Close();
			pRecordset.Release();
			MessageBox("不存在考试信息");
			return FALSE;
		}
		m_ListMake.DeleteAllItems();
		if(pRecordset != NULL)
		{
			while(!pRecordset->adoEOF)
			{
// 				strTEXT.Format("%d",k+1);
// 				m_ListMake.InsertItem(k,strTEXT);
				var = pRecordset->GetCollect("准考证明编号");
				if ( var.vt != VT_NULL )
					strZKZM = (LPCSTR)_bstr_t(var);
				else
					strZKZM=_T("");
				m_ListMake.InsertItem(k,strZKZM);
				//m_ListMake.SetItemText(k,1,strZKZM);
				var = pRecordset->GetCollect("姓名");
				if ( var.vt != VT_NULL )
					strXM = (LPCSTR)_bstr_t(var);
				else
					strXM=_T("");
				m_ListMake.SetItemText(k,1,strXM);
				var = pRecordset->GetCollect("身份证明编号");
				if ( var.vt != VT_NULL )
					strSFZM = (LPCSTR)_bstr_t(var);
				else
					strSFZM=_T("");
				m_ListMake.SetItemText(k,2,strSFZM);
				/*
				var = pRecordset->GetCollect("打印状态");
				if ( var.vt != VT_NULL )
				{
					strDYZT = (LPCSTR)_bstr_t(var);
					if (strDYZT==_T("0") || strDYZT==_T("1") )
						strTEXT.Format("未打印");
					else 
						strTEXT.Format("已打印");
				}	
				m_ListMake.SetItemText(k,3,strTEXT);
				*/
				var = pRecordset->GetCollect("考试科目");
				if ( var.vt != VT_NULL )
					strKSKM = (LPCSTR)_bstr_t(var);
				else
					strKSKM=_T("");
				strTEXT.Format("科目%s",strKSKM);	
				m_ListMake.SetItemText(k,4,strTEXT);
				var = pRecordset->GetCollect("考试成绩");
				if ( var.vt != VT_NULL )
				{
					int RSS = var.iVal;
					if (RSS>0)
					{
						int top=strZKZM.GetLength();
						CString Date=strZKZM.Mid(top-2,top);
						NKSCJ=RSS-atoi(Date)-45;
					}else
						NKSCJ=0;
				}
				else
					NKSCJ=0;
				strTEXT.Format("%d分",NKSCJ);	
				m_ListMake.SetItemText(k,5,strTEXT);
				var = pRecordset->GetCollect("考试次数");
				if ( var.vt != VT_NULL )
				{
					NKSCS = var.iVal;
				}
				else
					NKSCS=0;
				strTEXT.Format("%d",NKSCS);	
				m_ListMake.SetItemText(k,6,strTEXT);
				k++;
				pRecordset->MoveNext();
				}
			}
// 				pRecordset->PutCollect("打印状态",(_variant_t)"1");
// 				pRecordset->Update();
			pRecordset->Close();
			pRecordset.Release();
			STRS=k;
			strTEXT.Format("补打考生列表 共%d人",k);
			m_SText.SetWindowText(strTEXT);
		}
		catch(_com_error e)		//捕捉异常
		{
			CTime curTime =CTime::GetCurrentTime();
			CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
			FILE *fp=fopen("log.txt","a+");
			fprintf(fp,"[%s]OPEN失败!\n",Data);
			fclose(fp);	
			return FALSE;
		}
	return TRUE;
}





void CAutoPrintManagerDlg::OnMakeBtnStudent() 
{
	// TODO: Add your control notification handler code here
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty())
	{
		MessageBox("请选择考场,数据库IP或用户不能为空!");
		return ;
	}
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,考试次数 \
		from examrecordindetail where 考试日期=dbo.dateEncode('%s') and ((考试成绩-right(dbo.charDecode(准考证明编号),2)-45)>=%d or ((考试成绩-right(dbo.charDecode(准考证明编号),2)-45)<%d and 当日次数=2))  order by 考试时间 ",stime,theApp.iCJ);
	AllStudent(strSQL);
	Btnname.Format("全部考生 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
	if (BTNMake==TRUE)
	{
		m_Set = m_Set?false:true;
		if (m_Set == TRUE)
			SetWindowPos(NULL,0,0,m_LargerRect.Width(),m_LargerRect.Height(),SWP_NOZORDER|SWP_NOMOVE);	
		else
			SetWindowPos(NULL,0,0,m_SmallRect.Width(),m_SmallRect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}
	
}





void CAutoPrintManagerDlg::AllStudent(CString strSQL)
{
	
	_ConnectionPtr m_pConn;
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return ;
	}
	if (!ConSQLCon(m_pConn))
	{
		TRACE("连接数据库失败\n");
		return ;
	}
	if (!ReadSQLINFO(strSQL,m_pConn))
		BTNMake=FALSE;
	else
		BTNMake=TRUE;
	m_ListMake.SetRedraw(TRUE);
	m_ListMake.Invalidate();
	m_ListMake.UpdateWindow();
	// TODO: Add your control notification handler code here
	if (m_pConn!=NULL)
	{
		m_pConn->Close();
	}
}



void CAutoPrintManagerDlg::OnCheckAll2() 
{
	// TODO: Add your control notification handler code here
	//MessageBox("密码错误");
	m_CheckSet1 = m_CheckSet1?false:true;
	int nItem=m_ListMake.GetItemCount();
	
	for(int i = 0; i <nItem ; i++)
	{
		m_ListMake.SetCheck(i,m_CheckSet1);
	}
}





void CAutoPrintManagerDlg::OnButtonNAMEConfirmed() //勾选
{
	// TODO: Add your control notification handler code here
	theApp.strKSCS.RemoveAll();
 	theApp.strZKZM.RemoveAll();
 	theApp.stri.RemoveAll();
	UpdateData(TRUE);
	BTnCheck=TRUE;  
	CString strcount=_T("");
	int nItem=m_ListMake.GetItemCount();
	//m_ListMake.SetExtendedStyle(LVS_EX_CHECKBOXES);
	//CStringArray  strZKZM,strKSCS;
	BOOL Bcheck=FALSE;  //判断是否勾选考生
	for(int i = 0; i <nItem ; i++)
	{
		if(/* m_ListMake.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED||*/m_ListMake.GetCheck(i))
		{
			theApp.strZKZM.Add(m_ListMake.GetItemText(i,0));
			theApp.strKSCS.Add(m_ListMake.GetItemText(i,6));
			strcount.Format("%d",i);
			theApp.stri.Add(strcount);
			Bcheck=TRUE;
		}
    }
	if (Bcheck==FALSE)
	{
		MessageBox("未选择打印考生");
		return ;
	}
	m_ListMake.EnableWindow(FALSE);
	int len=theApp.strZKZM.GetSize();
	if (BTNprint)
	{
		for (int k=0;k<len;k++)
		{
			UpdateSQL(theApp.strZKZM[k]);
		}
		for (int u=0;u<len;u++)
		{
			m_ListMake.SetCheck(atoi(theApp.stri[u]),FALSE);
		}
		m_ListMake.EnableWindow(TRUE);
	}else
	{
		
		AfxBeginThread(AllPrintThread,&m_ListMake);
		Sleep(0);

	}
	UpdateData(FALSE);
	//m_ListMake.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	
}


void CAutoPrintManagerDlg::UpdateSQL(CString sZKZM)
{
	/*m_ListMake.DeleteItem(atoi())*/
	_ConnectionPtr m_pConn;
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return ;
	}
	if (!ConSQLCon(m_pConn))
	{
		TRACE("连接数据库失败\n");
		return ;
	}
	try
	{
		CString strSQL;
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("Set ARITHABORT ON;update studentinfo set 打印状态='0' where 准考证明编号=dbo.charEncode('%s')",sZKZM);		
		m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);		
	}
	catch (_com_error e) 
	{
		CTime curTime =CTime::GetCurrentTime();
		CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
		FILE *fp=fopen("log.txt","a+");
		fprintf(fp,"[%s]修改%s失败!\n",Data,sZKZM);
		fclose(fp);	
		return ;
	}	
}






void CAutoPrintManagerDlg::OnRclickListAllkc2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		if(pNMListView->iItem != -1)
		{
			DWORD dwPos = GetMessagePos();
			CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
			CMenu menu;
			VERIFY( menu.LoadMenu( IDR_MENU1 ) );
			CMenu* popup = menu.GetSubMenu(0);
			ASSERT( popup != NULL );
			popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
		} 
      /*  *pResult = 0;*/
	
}

void CAutoPrintManagerDlg::OnMenuitem32773() //打印
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString sZKZM=_T(""),sKSCS=_T("");
	int Nkscs=0;
	int nItem=m_ListMake.GetItemCount();
	for(int i = 0; i <nItem ; i++)
	{
		if(m_ListMake.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{		
			sZKZM=m_ListMake.GetItemText(i,0);
			sKSCS=m_ListMake.GetItemText(i,6);
			WaitForSingleObject(hEvent,INFINITE);
			HINSTANCE dllPrintSheetHandle;
			//////////////////////////////////////////////////////////////////////////
			dllPrintSheetHandle = LoadLibrary(TEXT("BKPrintSheet.dll")); 
			if (dllPrintSheetHandle) 
			{ 
				typedef BOOL (* DLLPrintSheet)(LPTSTR , int ,LPTSTR ,LPTSTR ,LPTSTR );
				DLLPrintSheet PrintSheetz=NULL;
				PrintSheetz = (DLLPrintSheet) GetProcAddress(dllPrintSheetHandle,TEXT("PrintSheet"));
				if (PrintSheetz)
				{
					Nkscs=atoi(sKSCS);
					PrintSheetz(sZKZM.GetBuffer(0),Nkscs,m_ESer.GetBuffer(0),m_EUser.GetBuffer(0),m_EPwd.GetBuffer(0));	
				}
				FreeLibrary(dllPrintSheetHandle);
			} 
			SetEvent(hEvent);
			break;
		}
    }
	
}



void CAutoPrintManagerDlg::OnDblclkListAllkc2(NMHDR* pNMHDR, LRESULT* pResult) //list双击消息  预览
{
	UpdateData(TRUE);
	CString sZKZM=_T(""),sKSCS=_T("");
	int Nkscs=0;
	int nItem=m_ListMake.GetItemCount();
	for(int i = 0; i <nItem ; i++)
	{
		if(m_ListMake.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{		
			sZKZM=m_ListMake.GetItemText(i,0);
			sKSCS=m_ListMake.GetItemText(i,6);
			WaitForSingleObject(hEvent,INFINITE);
			HINSTANCE dllPrintSheetHandle;
			// 	//////////////////////////////////////////////////////////////////////////
			dllPrintSheetHandle = LoadLibrary(TEXT("BKPrintSheetEx.dll")); 
			if (dllPrintSheetHandle) 
			{ 
				typedef BOOL (* DLLPrintSheet)(LPTSTR , int ,LPTSTR ,LPTSTR ,LPTSTR );
				DLLPrintSheet PrintSheetz=NULL;
				PrintSheetz = (DLLPrintSheet) GetProcAddress(dllPrintSheetHandle,TEXT("PrintSheetEx"));
				if (PrintSheetz)
				{
					Nkscs=atoi(sKSCS);
					PrintSheetz(sZKZM.GetBuffer(0),Nkscs,m_ESer.GetBuffer(0),m_EUser.GetBuffer(0),m_EPwd.GetBuffer(0));				
				}
				FreeLibrary(dllPrintSheetHandle);
			}  
			SetEvent(hEvent);
			CYLDlg yldlg;
			yldlg.DoModal();
			break;
		}
		
	}
	*pResult = 0;
}

void CAutoPrintManagerDlg::OnMenuitem32782() //合格未打印
{
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,打印状态,考试次数 \
		from studentinfo where 预考日期=dbo.dateEncode('%s') and 状态='1' and 打印状态='0'order by 考试日期 ",stime);
	AllStudent(strSQL);
	Btnname.Format("合格未打印 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
}

void CAutoPrintManagerDlg::OnMenuitem32783() //合格已打印
{
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,打印状态,考试次数 \
		from studentinfo where 预考日期=dbo.dateEncode('%s') and 状态='1' and 打印状态='2' order by 考试日期 ",stime);	
	AllStudent(strSQL);
	Btnname.Format("合格已打印 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
}

void CAutoPrintManagerDlg::OnMenuitem32784() //不合格名单
{
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,考试次数 \
		from examrecordindetail where 考试日期=dbo.dateEncode('%s') and (考试成绩-right(dbo.charDecode(准考证明编号),2)-45)<%d and 当日次数=2 order by 考试时间",stime,theApp.iCJ);
	AllStudent(strSQL);
	Btnname.Format("不合格名单 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
}

void CAutoPrintManagerDlg::OnMenuitem32785() //全部考生
{
	UpdateData(TRUE);
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,考试次数 \
		from examrecordindetail where 考试日期=dbo.dateEncode('%s') and 当日次数=1 order by 考试日期 ",stime);
	AllStudent(strSQL);
	Btnname.Format("全部考生 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
}

void CAutoPrintManagerDlg::OnMenuitem32774() //合格名单
{
	UpdateData(TRUE);
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,考试次数 \
		from examrecordindetail where 考试日期=dbo.dateEncode('%s') and (考试成绩-right(dbo.charDecode(准考证明编号),2)-45)>=%d order by 考试时间 ",stime,theApp.iCJ);
	AllStudent(strSQL);
	Btnname.Format("合格名单 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
	
}





void CAutoPrintManagerDlg::OnMenuitem32789() //合格率打印
{
	//调用DLL001.dll的出口函数add()
	UpdateData(TRUE);
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	int nretval=(*lpfnadd2)(stime.GetBuffer(0),m_ESer.GetBuffer(0),m_EPwd.GetBuffer(0),m_KCMC.GetBuffer(0));
	stime.ReleaseBuffer();
	m_ESer.ReleaseBuffer();
	m_EPwd.ReleaseBuffer();
	m_KCMC.ReleaseBuffer();
	UpdateData(FALSE);
	SetCurrentDirectory(sstrPath.GetBuffer(MAX_PATH));    //重新载入软件的绝对路径
}


void CAutoPrintManagerDlg::OnMenuitem32786()  //合格机单
{
	UpdateData(TRUE);
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	int nretval=(*lpfnadd)(stime,m_ESer,m_EPwd,m_KCMC,0);
	UpdateData(FALSE);
	SetCurrentDirectory(sstrPath.GetBuffer(MAX_PATH));    //重新载入软件的绝对路径
}

void CAutoPrintManagerDlg::OnMenuitem32787() //不合格机单
{
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	int nretval=(*lpfnadd)(stime,m_ESer,m_EPwd,m_KCMC,1);
	UpdateData(FALSE);
	SetCurrentDirectory(sstrPath.GetBuffer(MAX_PATH));    //重新载入软件的绝对路径
}

void CAutoPrintManagerDlg::OnMenuitem32788() //全部考生机单
{
	UpdateData(TRUE);
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	int nretval=(*lpfnadd)(stime,m_ESer,m_EPwd,m_KCMC,2);
	UpdateData(FALSE);
	SetCurrentDirectory(sstrPath.GetBuffer(MAX_PATH));    //重新载入软件的绝对路径
}

BOOL CAutoPrintManagerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE) 
	{
			
		if (IDYES == MessageBox(_T("是否退出打印程序!"), NULL, MB_YESNO))
		{
			CDialog::OnClose();
		}
		return TRUE;
	}
	if (pMsg->wParam == VK_RETURN) 
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CAutoPrintManagerDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
 //	CRect rc;
	/*
	CMenu menu;
	CPoint oPoint;
	GetCursorPos( &oPoint);
	menu.LoadMenu(IDR_MENU1);
	CMenu *pMenu=menu.GetSubMenu(0);
	ASSERT(pMenu!=NULL);
//	CMenu *pMenu1 = GetMenu();


	//菜单变灰，不可用
	pMenu->EnableMenuItem(ID_MENUITEM32774, MF_DISABLED|MF_GRAYED); //合格名单
	pMenu->EnableMenuItem(ID_MENUITEM32782, MF_DISABLED|MF_GRAYED); //合格未打印
	pMenu->EnableMenuItem(ID_MENUITEM32783, MF_DISABLED|MF_GRAYED); //合格已打印
	pMenu->EnableMenuItem(ID_MENUITEM32784, MF_DISABLED|MF_GRAYED); //不合格名单
	pMenu->EnableMenuItem(ID_MENUITEM32785, MF_DISABLED|MF_GRAYED); //全部考生
	pMenu->EnableMenuItem(ID_MENUITEM32773, MF_DISABLED|MF_GRAYED);  //打印考生
	pMenu->TrackPopupMenu(TPM_LEFTALIGN,oPoint.x,oPoint.y,this);
	 //将打开菜单项禁用，变灰，记得将CMainFrame源文件函数中加入m_bAutoMenuEnable=FALSE;
	//GetMenu()->GetSubMenu(0)->EnableMenuItem(1,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	*/
	CDialog::OnRButtonDown(nFlags, point);
}


void CAutoPrintManagerDlg::OnMenuitem32793() //补打列表
{
	// TODO: Add your command handler code here
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty())
	{
		MessageBox("请选择考场,数据库IP或用户不能为空!");
		return ;
	}
	
	CString strSQL,Btnname;
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	strSQL.Format("Set ARITHABORT ON;select dbo.charDecode(身份证明编号) as 身份证明编号,dbo.charDecode(准考证明编号) as 准考证明编号,dbo.charDecode(姓名) as 姓名,考试科目,考试成绩,考试次数 from \
		examrecordindetail where 考试日期=dbo.dateEncode('%s') order by 考试日期 ",stime);	
	AllStudent(strSQL);
	Btnname.Format("合格已打印 共%d人",STRS);
	GetDlgItem(IDC_STATIC_JS)->SetWindowText(Btnname);
	
	if (BTNMake==TRUE)
	{
		m_Set = m_Set?false:true;
		if (m_Set == TRUE)
			SetWindowPos(NULL,0,0,m_LargerRect.Width(),m_LargerRect.Height(),SWP_NOZORDER|SWP_NOMOVE);
		else
			SetWindowPos(NULL,0,0,m_SmallRect.Width(),m_SmallRect.Height(),SWP_NOZORDER|SWP_NOMOVE);	
	}
	
}




void CAutoPrintManagerDlg::OnRclickListAllkc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//MessageBox("111111");
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_MENU2 ) );
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	} 

}

void CAutoPrintManagerDlg::OnDblclkListAllkc(NMHDR* pNMHDR, LRESULT* pResult)  //双击list开启数据库连接
{
	m_ListAllKC.EnableWindow(FALSE);
	AfxBeginThread(ConSQLThread,&m_ListAllKC);
	Sleep(100);
}



void CAutoPrintManagerDlg::OnCheckKsy1() 
{
	// TODO: Add your control notification handler code here

	
	if(BST_CHECKED!=m_checkKSY1.GetCheck())
	{
		m_checkKSY1.SetWindowText("启用考官签字");
		WritePrivateProfileString("config","KGprint","1",FPath);
		return ;

	} 
	else //选中
	{
		m_checkKSY1.SetWindowText("关闭考官签字");
		WritePrivateProfileString("config","KGprint","0",FPath);
		return ;
	}
	
}

void CAutoPrintManagerDlg::OnMenuitem32800() 
{
	// TODO: Add your command handler code here
	//MessageBox("11111");
	//调用DLL001.dll的出口函数add()
	UpdateData(TRUE);
	CString stime;
	CTime time;
	m_sDate.GetTime(time);
    stime=time.Format("%Y-%m-%d"); 
	int nretval=(*lpfnadd3)(stime.GetBuffer(0),m_ESer.GetBuffer(0),m_EPwd.GetBuffer(0),m_KCMC.GetBuffer(0));
	stime.ReleaseBuffer();
	m_ESer.ReleaseBuffer();
	m_EPwd.ReleaseBuffer();
	m_KCMC.ReleaseBuffer();
	UpdateData(FALSE);
	SetCurrentDirectory(sstrPath.GetBuffer(MAX_PATH));    //重新载入软件的绝对路径
}



void CAutoPrintManagerDlg::OnCheckAll3() 
{
	// TODO: Add your control notification handler code here
	m_CheckSet2 = m_CheckSet2?false:true;
	int nItem=m_ListAllKC.GetItemCount();
	for(int i = 0; i <nItem ; i++)
	{
		m_ListAllKC.SetCheck(i,m_CheckSet2);
	}
}





void CAutoPrintManagerDlg::OnBTNAutoPrint1J() 
{

	UpdateData(TRUE);
	if (m_ESer.IsEmpty() || m_EUser.IsEmpty() || m_KCMC.IsEmpty())
	{
		AfxMessageBox("考场名称,IP,用户名不能为空!");
		return ;
	}
	iZJ=m_ZDprint.GetCurSel();
	CString strZJ,strSQL;
	strZJ.Format("%d",iZJ);
	WritePrivateProfileString("config","ZJprint",strZJ,FPath);
	//	return ;
	if (BTnCheck==TRUE)
	{
		MessageBox("已有考生在打印中,请稍等");
	}else
	{
		
		//return ;
		if (BTNprint==FALSE)
		{
			BTNprint=TRUE;//判断自动打印按键
			GetDlgItem(IDC_BTN_AutoPrint3)->SetWindowText("停止自动打印");
			//GetDlgItem(IDC_BTN_AutoPrint)->EnableWindow(FALSE);
			int nItem=m_ListAllKC.GetItemCount();
			CTime time;
			m_sDate.GetTime(time);
			sdate=time.Format("'%Y-%m-%d'");  
			for(int i = 0; i <nItem ; i++)
			{
				if (m_ListAllKC.GetCheck(i))//
				{
					m_info[i].strSQLIP=m_ListAllKC.GetItemText(i,3);
					m_info[i].strUser=m_ListAllKC.GetItemText(i,4);
					m_info[i].strPWD=m_ListAllKC.GetItemText(i,5);
					m_info[i].Thri=i;
					m_info[i].Bstop=TRUE;
					//AfxMessageBox("1111");
					CTime time;
					m_sDate.GetTime(time);
					sdate=time.Format("'%Y-%m-%d'"); 
					strSQL.Format("Set ARITHABORT ON;update studentinfo set 打印状态='0' where 预考日期=dbo.dateEncode('%s')",sdate);
					UpdateStudent(strSQL);
					AfxBeginThread(PrintThread,&m_info[i]);
					Sleep(100);
					//k+=1;
				}
			}
			GetDlgItem(IDC_LIST_ALLKC)->EnableWindow(FALSE);
		} 
		else
		{
			int nItem=m_ListAllKC.GetItemCount();
			int iCount=0;
			CTime time;
			m_sDate.GetTime(time);
			sdate=time.Format("'%Y-%m-%d'");  
			for(int i = 0; i <nItem ; i++)
			{
				if (m_ListAllKC.GetCheck(i))//
				{
					m_info[i].Bstop=FALSE;
					//m_ListAllKC.SetCheck(i,FALSE);
				}	
				else	
				{
					iCount++;
					m_info[i].Bstop=TRUE;
				}
			}
			BTNprint=FALSE;
			GetDlgItem(IDC_LIST_ALLKC)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_AutoPrint3)->SetWindowText("一键自动打印");
		}             
	} 
}





void CAutoPrintManagerDlg::UpdateStudent(CString strSQL)
{
	
	_ConnectionPtr m_pConn;
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return ;
	}
	if (!ConSQLCon(m_pConn))
	{
		TRACE("连接数据库失败\n");
		return ;
	}
	if (!updateSQLINFO(strSQL,m_pConn))
		BTNMake=FALSE;
	else
		BTNMake=TRUE;
	m_ListMake.SetRedraw(TRUE);
	m_ListMake.Invalidate();
	m_ListMake.UpdateWindow();
	// TODO: Add your control notification handler code here
	if (m_pConn!=NULL)
	{
		m_pConn->Close();
	}
}




BOOL CAutoPrintManagerDlg::updateSQLINFO(CString strSQL,_ConnectionPtr m_pConn)
{
	try
	{
		_RecordsetPtr pRecordset; 
		pRecordset.CreateInstance("ADODB.Recordset"); 
		pRecordset->CursorLocation = adUseClient;   
		pRecordset->Open(_variant_t(strSQL),_variant_t((IDispatch*)m_pConn, true),adOpenDynamic,adLockOptimistic, adCmdText);
	}
	catch(_com_error e)		//捕捉异常
	{
		CTime curTime =CTime::GetCurrentTime();
		CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
		FILE *fp=fopen("log.txt","a+");
		fprintf(fp,"[%s]OPEN失败!\n",Data);
		fclose(fp);	
		return FALSE;
	}
	return TRUE;
}






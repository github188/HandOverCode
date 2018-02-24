// CFourPicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CFourPic.h"
#include "CFourPicDlg.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WSPACE 1
#define HSPACE 1
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
// CCFourPicDlg dialog

CCFourPicDlg::CCFourPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCFourPicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCFourPicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCFourPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCFourPicDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCFourPicDlg, CDialog)
	//{{AFX_MSG_MAP(CCFourPicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTALLVIDEO_MSG,OnStartAllVideo)
	ON_MESSAGE(WM_RGPPDLL_MSG,OnRGPPDLL)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNPageDown, OnBTNPageDown)
	ON_BN_CLICKED(IDC_BTNPageUP, OnBTNPageUP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFourPicDlg message handlers
void CCFourPicDlg::OnRGPPDLL(WPARAM wParam,LPARAM lParam)
{
	CString strKchp,strStuName,strStuZkzm;	
	strKchp.Format("考车%d",wParam);
//	AfxMessageBox(str);
	CString temp;
	//1.根据考车号读取考试中考生
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("SELECT 姓名,准考证明编号 FROM StudentInfo WHERE 状态='3' and 考车号='%s' ",strKchp);
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			vat =pSet->GetCollect("姓名");
			if (vat.vt != VT_NULL)
			{
				strStuName.Format("%s",(LPCSTR)_bstr_t(vat));	
			}
			vat =pSet->GetCollect("准考证明编号");
			if (vat.vt != VT_NULL)
			{
				strStuZkzm.Format("%s",(LPCSTR)_bstr_t(vat));	
			}
			else
			{
				strStuZkzm=_T("");
			}
			pSet->Close();
			pSet.Release();
		}	
	}
	catch (_com_error &comError)
	{
		temp.Format("读取考试中考生失败! %s",comError.Description());
		AfxMessageBox(temp);
	}
	//////////////////////////////////////////////////////////////////////////
	//2.动态加载人工评判 DLL
	if (strStuZkzm.IsEmpty())
	{
		temp.Format("%s -->无考试中考生!",strKchp);
		MessageBox(temp,"人工评判提示");
		return ;
	}
	
	temp.Format("%s是否评判考生%s\n考试不合格!",strKchp,strStuName);
	if (MessageBox(temp,"人工评判提示",MB_OKCANCEL) == IDOK)
	{

	
	int iRgppCode=0;
	try
	{
		//人工评判 DLL
		typedef int (* ShowRGPPDlg)(LPTSTR,LPTSTR,LPTSTR);
		ShowRGPPDlg dllRgppdlg;
		HINSTANCE dllHRGPP;
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		dllHRGPP = LoadLibrary(TEXT("CKM3RGPP.dll")); 
		if (dllHRGPP == NULL) 
		{ 
			AfxMessageBox("加载 人工评判 DLL 失败!"); 
			//WriteLogz("加载 D:\\bkzyVideoOcx\\CKM3RGPP.dll 失败!"); 
			return ; 
		}	
		dllRgppdlg = (ShowRGPPDlg) GetProcAddress(dllHRGPP,TEXT("ShowRGPPDlg"));
		if (dllRgppdlg!=NULL)
		{
			iRgppCode=dllRgppdlg((LPSTR)(LPCSTR)strKchp,(LPSTR)(LPCSTR)strStuName,(LPSTR)(LPCSTR)strStuZkzm);
		}
		FreeLibrary(dllHRGPP);
	}
	catch (...)
	{
		temp.Format("Exception of ShowRgppDlg(%s,%s,%s)",strKchp,strStuName,strStuZkzm);
		AfxMessageBox(temp);
		return ;
	}
	
	if (iRgppCode!=0)
	{				
		//4.根据公安网代码从数据库读取错误编号
		int iErrorNum;
		iErrorNum=0;
		try
		{
			
			CString temp1;
			VARIANT cnt1;
			cnt1.vt = VT_INT;
			temp1.Format("SELECT * FROM ErrorData WHERE 公安网代码='%d' and  备注='人工评判' ",iRgppCode);	
			_RecordsetPtr pSet1 =m_pConn->Execute((_bstr_t)temp1,&cnt1,adCmdUnknown);
			_variant_t vat1;
			
			if(pSet1 != NULL && (!pSet1->adoEOF))
			{
				while(!pSet1->adoEOF)
				{
					vat1 = pSet1->GetCollect("错误编号");
					iErrorNum  = vat1.intVal;
					pSet1->MoveNext();	
				}					
			}
			else
			{
				temp.Format("数据库ErrorData表中无该人工评判项目!!!公安网编号:%d",iRgppCode);
				AfxMessageBox(temp);
				return;
			}		
			pSet1->Close();	
			pSet1.Release();
		}
		catch (_com_error &comError)
		{
			temp.Format("人工评判失败! 读取错误编号 %s",comError.Description());
			AfxMessageBox(temp);
			return ;
		}
		if (iErrorNum==0)
		{
			temp.Format("数据库ErrorData表中无该人工评判项目!!!iErrorNum==0 公安网编号:%d",iRgppCode);
			AfxMessageBox(temp);
			return;
		} 
		//3.将DLL返回的公安网代码写入数据库
// 		int iErrorNum;
// 		iErrorNum=514;
		try
		{
			VARIANT cnt;
			cnt.vt = VT_INT;
			temp.Format("SELECT 结束考试 FROM StudentInfo WHERE 准考证明编号='%s' ",strStuZkzm);
			_RecordsetPtr pSet;
			pSet.CreateInstance("ADODB.Recordset");
			pSet->CursorLocation = adUseClient;
			pSet->Open((_variant_t)_bstr_t(temp), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
			if(pSet != NULL && (!pSet->adoEOF))
			{
				pSet->PutCollect("结束考试",_variant_t( (long)(iErrorNum)) );	
				pSet->Update();
			}
			pSet->Close();
			
			temp.Format("%s\n姓名:%s\n证件号%s\n评判完成!结束考试=%d",strKchp,strStuName,strStuZkzm,iErrorNum);
			MessageBox(temp,"人工评判提示");
		}
		catch (_com_error &comError)
		{
			temp.Format("人工评判失败! _com_error结束考试  %s",comError.Description());
			AfxMessageBox(temp);
		}
	}
	}
}
void CCFourPicDlg::OnStartAllVideo(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format("%d",wParam);
	int ai=atoi(str);
	if (ai==1)
	{
		for (int i=0;i<4;i++)
		{
			myPlaywnd[i].SetPlaySound();
		}
	}
	else
	{
		for (int i=0;i<4;i++)
		{
			myPlaywnd[i].StartRealPlay();
		}
	}

	//return 0;
}
BOOL CCFourPicDlg::OnInitDialog()
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
	PageCounti=0;

	hbr1=CreateSolidBrush(RGB(60,60,60));
	ShowWindow(SW_SHOWMAXIMIZED);// 全屏
	NET_DVR_Init();// 初始化

	CString strPath,tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));
	DWORD dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	tempPath.Format("%s\\%s",strPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	logFileName.Format("%s\\日志.log",tempPath);
	//////////////////////////////////////////////////////////////////////////
	isSqlOrOralce=GetPrivateProfileInt("CONFIG","SqlOROralce",0,".\\aconfig.ini");
	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	GetMyHostIP();
	//////////////////////////////////////////////////////////////////////////
	GetClientRect(&CliRc);
	CliRcMin.x=CliRc.Width()/2-2*WSPACE;
	CliRcMin.y=(CliRc.Height()-35)/2-2*HSPACE;
	GetDlgItem(IDC_STATIC_WND1)->MoveWindow(WSPACE,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND2)->MoveWindow(WSPACE*3+CliRcMin.x,HSPACE,CliRcMin.x,CliRcMin.y);
//	GetDlgItem(IDC_STATIC_WND3)->MoveWindow(WSPACE*5+CliRcMin.x*2,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND3)->MoveWindow(WSPACE,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND4)->MoveWindow(WSPACE*3+CliRcMin.x,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);
//	GetDlgItem(IDC_STATIC_WND6)->MoveWindow(WSPACE*5+CliRcMin.x*2,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);

	GetDlgItem(IDC_STATIC_WND5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_WND6)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_WND7)->MoveWindow(0,CliRc.Height()-35,CliRc.Width()-200,35);
	CString temp;
	CWnd * outBroad;
	int i=0;
	for (i=0;i<4;i++)
	{
		myPlaywnd[i].SetCliRcSize(CliRcMin.x,CliRcMin.y);
		temp.Format("画面%d",i+1);
		myPlaywnd[i].iKch=GetPrivateProfileInt("画面",temp,0,".\\aconfig.ini");
		outBroad=GetDlgItem(IDC_STATIC_WND1+i);
		myPlaywnd[i].Create(IDD_DLGPLAY,outBroad);
		myPlaywnd[i].ShowWindow(SW_SHOW);
		myPlaywnd[i].CenterWindow();
	}
	GetDlgItem(IDC_STATIC_PAGE)->MoveWindow(CliRc.Width()-160,CliRc.Height()-30,50,35);
	GetDlgItem(IDC_BTNPageUP)->MoveWindow(CliRc.Width()-100,CliRc.Height()-30,40,25);
	GetDlgItem(IDC_BTNPageDown)->MoveWindow(CliRc.Width()-50,CliRc.Height()-30,40,25);
	nowPageI=1;
	if (PageCounti>4)
	{
		PageCounti=(PageCounti-1)/4+1;
	}
	else
	{
		PageCounti=1;
	}
	
	temp.Format("%d/%d",nowPageI,PageCounti);
	GetDlgItem(IDC_STATIC_PAGE)->SetWindowText(temp);
	OnCountHGL();
	SetTimer(1,10000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCFourPicDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCFourPicDlg::OnPaint() 
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
HCURSOR CCFourPicDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCFourPicDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	NET_DVR_Cleanup();//释放
	CDialog::OnClose();
}

BOOL CCFourPicDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//MessageBox(_T("回车键按下"));
			//			return TRUE;
		case VK_ESCAPE:
			//MessageBox(_T("ESC键按下"));
			if (MessageBox("是否退出程序!","提示",MB_OKCANCEL) == IDOK)
			{
				this->SendMessage(WM_CLOSE);
			}
			else
			{
				return TRUE;
			}
			break;
			//return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCFourPicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG )
	{
		return hbr1;
	}
	if (pWnd->GetDlgCtrlID()==IDC_STATIC_WND7 || pWnd->GetDlgCtrlID()==IDC_STATIC_PAGE)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return hbr1;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}



BOOL CCFourPicDlg::GetMyHostIP()  
{  
	WSADATA wsadata;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsadata))   //初始化
	{
		AfxMessageBox("初始化网络环境失败!");
		return FALSE;
	}
	char szHostName[MAX_PATH + 1];
	gethostname(szHostName, MAX_PATH);  //得到计算机名
	hostent *p = gethostbyname(szHostName); //从计算机名得到主机信息
	if(p == NULL)
	{
		AfxMessageBox("得到本机网络信息失败!");
		return FALSE;
	}
	//	m_strName = szHostName;       //保存主机名
	int nCount = 0;                 //保存主机地址
	char *pIP = inet_ntoa(*(in_addr *)p->h_addr_list[0]);//将32位IP转化为字符串IP
	CString strIP;
	strIP = pIP;             //保存主机IP地址
    //UpdateData(FALSE);          //显示各个数据
	OnSelectSQL(strIP);
	WSACleanup();               //释放Winsock API
	return TRUE;
}  


void CCFourPicDlg::OnSelectSQL(CString strIP)
{
	_RecordsetPtr pRecordset;
	CString strSQL,strtemp,strKCH,strtempkch,strJM1,strJM2,strJM0;
	int pos=0;
	_variant_t var;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM liuheoneconfig where 电脑序号='%s'",strIP);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		int swd =pRecordset->RecordCount;
		if (swd==0)
		{
			AfxMessageBox("读取数据为空!");		
		}
		else
		{
			var = pRecordset->GetCollect("考试区域");
			if (var.vt != VT_NULL)
			{	
				KSCYstr.Format("%s",(LPCSTR)_bstr_t(var));
			}		
			while (!pRecordset->adoEOF)
			{
				var = pRecordset->GetCollect("画面");
				if (var.vt != VT_NULL)
				{	
					strtemp=(LPCSTR)_bstr_t(var);	
				}else
				{
					strtemp=_T("");
				}
				var = pRecordset->GetCollect("考车号");
				if (var.vt != VT_NULL)
				{	
					strKCH=(LPCSTR)_bstr_t(var);
					strtempkch.Format("考车%s",strKCH);
				}else
				{
					strKCH=_T("");	
				}
				WritePrivateProfileString("画面",strtemp,strKCH,".\\aconfig.ini");
				var = pRecordset->GetCollect("界面1");
				if (var.vt != VT_NULL)
				{	
					strJM1=(LPCSTR)_bstr_t(var);
					//strtempkch.Format("考车%s",strKCH);
				}else
				{
					strJM1=_T("");
					//strtempkch.Format("考车%s",strKCH);
				}
				WritePrivateProfileString(strtempkch,"1",strJM1,".\\aconfig.ini");
				var = pRecordset->GetCollect("界面2");
				if (var.vt != VT_NULL)
				{	
					strJM2=(LPCSTR)_bstr_t(var);
					//strtempkch.Format("考车%s",strKCH);
				}else
				{
					strJM2=_T("");
					//strtempkch.Format("考车%s",strKCH);
				}
				
				pos=strJM2.ReverseFind(',');
				if (pos >0)
				{
					strJM0.Format("%s,1",strJM2.Left(pos));
				}
				else
				{
					strJM0=_T("");
				}
				
				WritePrivateProfileString(strtempkch,"2",strJM2,".\\aconfig.ini");
				WritePrivateProfileString(strtempkch,"0",strJM0,".\\aconfig.ini");
				PageCounti+=1;
				pRecordset->MoveNext();
			}
		}
		pRecordset->Close();
		pRecordset.Release();
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("liuheoneconfig 配置读取出错!",e.ErrorMessage());
		AfxMessageBox(errormessage);
	}
	//CDialog::OnOK();
}

BOOL CCFourPicDlg::OnInitSQL()
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
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\aconfig.ini");
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
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		TRACE(Sqlstr);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
			WriteLog("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			WriteLog("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		ShellExecute(NULL,"open",".\\sqlLink.exe","aconfig.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

int CCFourPicDlg::OnSelectCount(CString str)
{
	TRACE(str+"\n");
	int rcounti=0;
	CString sqltemp;
	try
	{
		sqltemp=str;
		VARIANT cnt;
		cnt.vt = VT_INT;
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			vat =pSet->GetCollect("countN");
			if (vat.vt != VT_NULL)
			{
				rcounti=atoi((LPCSTR)_bstr_t(vat));				
			}
			pSet->Close();
			pSet.Release();
		}
	}
	catch (_com_error &comError)
	{
		sqltemp.Format("OnSelectCount Error ! %s,%s",comError.Description(),comError.ErrorMessage());
		AfxMessageBox(sqltemp);
		return 0;
	}
	return rcounti;
}

void CCFourPicDlg::OnCountHGL()
{
	int iExamSuc = 0; //合格人数    zt=1
	int iExamFla = 0; //不合格人数  zt=2
	int iExaming = 0; //在考人数    zt=3
	int iExamAll = 0; //所有人数    RecordCount
	int iLogNumb = 0; //已登陆人数  "登录状态"
	
	int iWaitNumb= 0; //未考人数    iLogNumb - (iExamSuc+iExamFla+iExaming)
	int iExamEnd = 0; //考试结束人数  iExamFla + iExamSuc
	CString temp;
	CString temp1;
	if (!KSCYstr.IsEmpty())
	{
		temp1.Format(" and 考场区域='%s' ",KSCYstr);
	}
	if (isSqlOrOralce==0)//SQL or Oracle
	{
		temp.Format("SELECT count(*) as countN from studentinfo where to_char(预考日期,'yyyy-mm-dd')=to_char(sysdate,'yyyy-mm-dd') %s",temp1);
	}
	else
	{
		temp.Format("SELECT count(*) as countN from studentinfo where (Select CONVERT(varchar(100), 预考日期, 23)) = (Select CONVERT(varchar(100), GETDATE(), 23)) %s",temp1);
	}
	
	iExamSuc =OnSelectCount(temp+" and 状态='1' ");
	iExamFla =OnSelectCount(temp+" and 状态='2' ");
	iExaming =OnSelectCount(temp+" and 状态='3' ");
	iExamAll =OnSelectCount(temp);
	iLogNumb =OnSelectCount(temp+" and 登录状态='已登录' ");

	iWaitNumb = iLogNumb - (iExamSuc+iExamFla+iExaming); //未考人数 
	iExamEnd  = iExamFla + iExamSuc;   //考试结束人数

	int iExamEndT=iExamEnd;
	if (iExamEndT==0)
	{
		iExamEndT=1;
	}
	temp.Format("共%d名考生,已登录%d考生,考试中%d人,已考%d人,合格%d人,合格率%2.2f%%  ,考试区域[%s]",iExamAll,iLogNumb,iExaming,iExamEnd,iExamSuc,(float)((float)(iExamSuc*100)/iExamEndT),KSCYstr);
	GetDlgItem(IDC_STATIC_WND7)->SetWindowText(temp);
}

void CCFourPicDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnCountHGL();
	CDialog::OnTimer(nIDEvent);
}

void CCFourPicDlg::OnBTNPageDown() 
{
	// TODO: Add your control notification handler code here
	if (nowPageI<PageCounti)
	{
		nowPageI++;
		myPlaywnd[0].SetPageCarNum(nowPageI*4-3);
		myPlaywnd[1].SetPageCarNum(nowPageI*4-2);
		myPlaywnd[2].SetPageCarNum(nowPageI*4-1);
		myPlaywnd[3].SetPageCarNum(nowPageI*4);
		
	}
	CString temp;
	temp.Format("%d/%d",nowPageI,PageCounti);
	GetDlgItem(IDC_STATIC_PAGE)->SetWindowText(temp);
}

void CCFourPicDlg::OnBTNPageUP() 
{
	// TODO: Add your control notification handler code here
	if (nowPageI>1)
	{
		nowPageI--;
		myPlaywnd[0].SetPageCarNum(nowPageI*4-3);
		myPlaywnd[1].SetPageCarNum(nowPageI*4-2);
		myPlaywnd[2].SetPageCarNum(nowPageI*4-1);
		myPlaywnd[3].SetPageCarNum(nowPageI*4);
	}
	CString temp;
	temp.Format("%d/%d",nowPageI,PageCounti);
	GetDlgItem(IDC_STATIC_PAGE)->SetWindowText(temp);
}

// test2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "test2.h"
#include "test2Dlg.h"
#include "HCNetSDK.h"
#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString NochangeNum;

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
// CTest2Dlg dialog

CTest2Dlg::CTest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest2Dlg)
	m_user = _T("admin");
	m_pwd = _T("12345");
	m_port = _T("8000");
	m_num = _T("1");
	m_carnum = _T("");
	m_total = _T("");
	m_play1 = 0;
	m_play2 = 0;
	m_play3 = 0;
	m_play4 = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest2Dlg)
	DDX_Control(pDX, IDC_COMBO_THREE, m_boxthree);
	DDX_Control(pDX, IDC_COMBO_TWO, m_boxtwo);
	DDX_Control(pDX, IDC_COMBO_ONE, m_boxone);
	DDX_Control(pDX, IDC_SecEDIT, m_Secedit);
	DDX_Control(pDX, IDC_IPADDRESS1, m_mIP);
	DDX_Control(pDX, IDC_LIST_Second, m_SecList);
	DDX_Control(pDX, IDC_LIST1, m_JLLlist);
	DDX_Text(pDX, IDC_EDIT2, m_user);
	DDX_Text(pDX, IDC_EDIT3, m_pwd);
	DDX_Text(pDX, IDC_EDIT4, m_port);
	DDX_Text(pDX, IDC_EDIT5, m_num);
	DDX_Text(pDX, IDC_EDIT_Carnum, m_carnum);
	DDX_Text(pDX, IDC_STATIC_NUMtotal, m_total);
	DDX_Text(pDX, IDC_EDIT_play1, m_play1);
	DDV_MinMaxInt(pDX, m_play1, 0, 3);
	DDX_Text(pDX, IDC_EDIT_play2, m_play2);
	DDV_MinMaxInt(pDX, m_play2, 0, 3);
	DDX_Text(pDX, IDC_EDIT_play3, m_play3);
	DDV_MinMaxInt(pDX, m_play3, 0, 3);
	DDX_Text(pDX, IDC_EDIT_play4, m_play4);
	DDV_MinMaxInt(pDX, m_play4, 0, 3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest2Dlg, CDialog)
	//{{AFX_MSG_MAP(CTest2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, ButtonSave)
	ON_BN_CLICKED(IDC_Research, OnResearch)
	ON_BN_CLICKED(IDC_Delete, OnDelete)
	ON_BN_CLICKED(IDC_ConnectBUTTON, OnConnectBUTTON)
	ON_NOTIFY(NM_CLICK, IDC_LIST_Second, OnClickLISTSecond)
	ON_BN_CLICKED(IDC_BUTTON2, OnCarAddButton)
	ON_BN_CLICKED(IDC_CarDeleteBUTTON, OnCarDeleteBUTTON)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Second, OnDblclkLISTSecond)
	ON_EN_KILLFOCUS(IDC_SecEDIT, OnKillfocusSecEDIT)
	ON_BN_CLICKED(IDC_BTNPZ, OnBtnpz)
	ON_CBN_SELCHANGE(IDC_COMBO_ONE, OnSelchangeComboOne)
	ON_CBN_SELCHANGE(IDC_COMBO_TWO, OnSelchangeComboTwo)
	ON_CBN_SELCHANGE(IDC_COMBO_THREE, OnSelchangeComboThree)
	ON_EN_CHANGE(IDC_EDIT_play1, OnChangeEDITplay1)
	ON_EN_CHANGE(IDC_EDIT_play2, OnChangeEDITplay2)
	ON_EN_CHANGE(IDC_EDIT_play3, OnChangeEDITplay3)
	ON_EN_CHANGE(IDC_EDIT_play4, OnChangeEDITplay4)
	ON_BN_CLICKED(IDC_SaveplayBUTTON, OnSaveplayBUTTON)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest2Dlg message handlers

BOOL CTest2Dlg::OnInitDialog()
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

	//初始化程序的数据，显示到程序当中
	//////////////////////////////////////////////////////////////////////////

	/////配置文件第一个列表
	m_JLLlist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_JLLlist.InsertColumn(0,_T("解码器"),LVCFMT_LEFT,55,-1);
	m_JLLlist.InsertColumn(1,_T("IP"),LVCFMT_LEFT,130,-1);
	m_JLLlist.InsertColumn(2,_T("用户"),LVCFMT_LEFT,100,-1);
	m_JLLlist.InsertColumn(3,_T("密码"),LVCFMT_LEFT,100,-1);
	m_JLLlist.InsertColumn(4,_T("端口"),LVCFMT_LEFT,100,-1);

	////显示考车的列表
	m_SecList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
//	m_SecList.InsertColumn(0,_T("序号"),LVCFMT_LEFT,50,-1);
//	m_SecList.InsertColumn(1,_T("解码器"),LVCFMT_LEFT,120,-1);
	m_SecList.InsertColumn(0,_T("通道号"),LVCFMT_LEFT,150,-1);
	m_SecList.InsertColumn(1,_T("考车号"),LVCFMT_LEFT,150,-1);
	
	//隐藏一个编辑框，用于在列表控件中，让列表控件变成可编辑的控件
	m_Secedit.ShowWindow(SW_HIDE);

	m_boxone.ResetContent();
	m_boxone.AddString(_T("隔行解码eg:1,3,5,7"));
	m_boxone.AddString(_T("不隔行解码,全部都解"));
	int z = GetPrivateProfileInt("JMQ","EVEN",0,".//config.ini");
	m_boxone.SetCurSel(z);

	m_boxtwo.ResetContent();
	m_boxtwo.AddString(_T("2"));
	m_boxtwo.AddString(_T("3"));

	int i = GetPrivateProfileInt("CONFIG","KSKM",0,".//config.ini");
	int m = 2,n;
	n = i-m;
	m_boxtwo.SetCurSel(n);

	m_boxthree.ResetContent();
	m_boxthree.AddString(_T("1"));
	m_boxthree.AddString(_T("2"));
	m_boxthree.AddString(_T("3"));
	m_boxthree.AddString(_T("4"));
	int q = GetPrivateProfileInt("CONFIG","VIDEOWND",0,".//config.ini");
	int l;
	l = q-1;
	m_boxthree.SetCurSel(l);

	int play1 = GetPrivateProfileInt("CONFIG","DISPLAY1",0,".//config.ini");
	int play2 = GetPrivateProfileInt("CONFIG","DISPLAY2",0,".//config.ini");
	int play3 = GetPrivateProfileInt("CONFIG","DISPLAY3",0,".//config.ini");
	int play4 = GetPrivateProfileInt("CONFIG","DISPLAY4",0,".//config.ini");

	CString temp1="",temp2="",temp3="",temp4="";
	temp1.Format("%d",play1);
	temp2.Format("%d",play2);
	temp3.Format("%d",play3);
	temp4.Format("%d",play4);
	GetDlgItem(IDC_EDIT_play1)->SetWindowText(temp1);
	GetDlgItem(IDC_EDIT_play2)->SetWindowText(temp2);
	GetDlgItem(IDC_EDIT_play3)->SetWindowText(temp3);
	GetDlgItem(IDC_EDIT_play4)->SetWindowText(temp4);

	Oninittext();

	//////////////////////////////////////////////////////////////////////////
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////左边列表控见的数据显示
BOOL CTest2Dlg::Oninittext()
{
	UpdateData(TRUE);
	UINT inum;
	int m_num=0,pos=0,i=1;
	CString temp,temp1,temp2,temp4,sIPaddr,sUser,sPwd,sPort;
	
	m_JLLlist.DeleteAllItems();

	inum = GetPrivateProfileInt("JMQ","NUM",0,".//config.ini");	

	for (i;i <= (int)inum;i++)
	{
		temp4.Format("%d",i);
		GetPrivateProfileString("JMQ",temp4,"",temp.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
		temp.ReleaseBuffer();

		temp1.Format("%d",m_num+1);
		m_JLLlist.InsertItem(m_num,temp1);
		/////把配置文件当中的数据分开获得	

		pos = temp.Find(",");
		sIPaddr = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,1,sIPaddr);
			
		temp2 = temp.Mid(pos+1);
		pos = temp2.Find(",");
		sUser = temp2.Left(pos);
		m_JLLlist.SetItemText(m_num,2,sUser);
			
		temp = temp2.Mid(pos+1);
		pos = temp.Find(",");
		sPwd = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,3,sPwd);
			
		sPort = temp.Mid(pos+1);
		m_JLLlist.SetItemText(m_num,4,sPort);
			
		m_num++;	
	}
	m_total.Format("%d",inum);
	
	SetDlgItemText(IDC_STATIC_NUMtotal,m_total);
	return true;
}
void CTest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTest2Dlg::OnPaint() 
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
HCURSOR CTest2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////左边列表控件的数据的保存和增加
void CTest2Dlg::ButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp,temp1,temp9,strIP,temp6,inum,temp5;

	if (m_num.IsEmpty() || m_port.IsEmpty() || m_pwd.IsEmpty() ||  m_user.IsEmpty() )
	{
		AfxMessageBox("错误提示:请填满所有信息，才能修改或添加");
		return ;
	}

	BYTE nField0,nField1,nField2,nField3;
	m_mIP.GetAddress(nField0,nField1,nField2,nField3);
	temp6.Format("%u.%u.%u.%u",nField0,nField1,nField2,nField3);

//	temp6.Format("%s",strIP);

	temp9.Format("是否要修改或添加： \n 解码器:%s\n IP:%s\n 用户名:%s\n 密码:%s\n 端口:%s\n\n若解码器不变，看做修改！",m_num,temp6,m_user,m_pwd,m_port);
	if (MessageBox(temp9,"提示",MB_OKCANCEL) == IDOK)
	{	//修改
		temp1.Format("%s",m_num);
		temp.Format("%s,%s,%s,%s",temp6,m_user,m_pwd,m_port);
		::WritePrivateProfileString("JMQ",temp1,temp,".//config.ini");		
		GetPrivateProfileString("JMQ","NUM","", inum.GetBuffer(MAX_PATH), MAX_PATH,".//config.ini");
		if ( _ttoi(m_num)>_ttoi(inum))
		{	//当出现多的数据时候增加的会有一个默认值
			///////////////////////////////////////////////////
			for (int i = _ttoi(inum)+1 ; i <= _ttoi(m_num) ; i++)
			{
				temp5.Format("%d",i);
				::WritePrivateProfileString("JMQ",temp5,temp,".//config.ini");	
			}		
			WritePrivateProfileString("JMQ","NUM",m_num,".//config.ini");
		}
	}
	Oninittext();
}
///自动查询更新配置文件当中的数据到列表控件当中
void CTest2Dlg::OnResearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str,temp9,temp,temp1,temp2,temp4,sIPaddr,sUser,sPwd,sPort;
	UINT inum;
	int m_num=0,pos=0,i=1;

	temp9.Format("已是最新内容");
	AfxMessageBox(temp9);

	m_JLLlist.DeleteAllItems();

	inum = GetPrivateProfileInt("JMQ","NUM",0,".//config.ini");
		
	for (i;i <= (int)inum;i++)
	{
		temp4.Format("%d",i);
		GetPrivateProfileString("JMQ",temp4,"0",temp.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
		temp.ReleaseBuffer();
			
		temp1.Format("%d",m_num+1);
		m_JLLlist.InsertItem(m_num,temp1);
		/////把配置文件中的数据分开显示			

		pos = temp.Find(",");
		sIPaddr = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,1,sIPaddr);
			
		temp2 = temp.Mid(pos+1);
		pos = temp2.Find(",");
		sUser = temp2.Left(pos);
		m_JLLlist.SetItemText(m_num,2,sUser);
			
		temp = temp2.Mid(pos+1);
		pos = temp.Find(",");
		sPwd = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,3,sPwd);
			
		sPort = temp.Mid(pos+1);
		m_JLLlist.SetItemText(m_num,4,sPort);
		
		m_num++;
	}		
}
//////左边列表数据的删除
void CTest2Dlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_num.IsEmpty() || m_port.IsEmpty() || m_pwd.IsEmpty() ||  m_user.IsEmpty())
	{
		AfxMessageBox("错误提示:所写入得数据已经为空");
		return ;
	}
	CString str,temp,strIP,temp6,temp2;
	int a=0,b=0,i=0,inum=0;
	unsigned char *pIP;
	DWORD dwIP;

	m_mIP.GetAddress(dwIP);
	pIP = (unsigned char*)&dwIP;
	strIP.Format("%u.%u.%u.%u",*(pIP+3),*(pIP+2),*(pIP+1),*pIP);
	temp6.Format("%s",strIP);

	temp.Format("是否要删除:\n 解码器:%s\n IP:%s\n 用户名:%s\n 密码:%s\n 端口:%s\n",m_num,temp6,m_user,m_pwd,m_port);
	str.Format("");
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{ 
		//实现在删除的时候删除一行，并自动补齐
	    //////////////////////////////////////////////////////////////////
		::WritePrivateProfileString("JMQ",m_num,str,".//config.ini");
		inum = GetPrivateProfileInt("JMQ","NUM",0,".//config.ini");			
		b = _ttoi(m_num)+1;	
		for(i = b;i <= inum;i++)
		{
			temp.Format("%d", i);
			GetPrivateProfileString("JMQ",temp,"",temp.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
			temp2.Format("%d", b-1);
			WritePrivateProfileString("JMQ",temp2,temp,".//config.ini");
			b++;
		}
		temp.Format("%d", inum-1);
	    ::WritePrivateProfileString("JMQ","NUM",temp,".//config.ini");
		temp.Format("%d", inum);
		WritePrivateProfileString("JMQ",temp,NULL,".//config.ini");	
		///////////////////////////////////////////////////////////////
		m_num = _T("");
		m_port = _T("");
		m_pwd = _T("");
	//	m_mIP=_T("");
		m_user = _T("");
	}
	////删除之后，清空控件内容
	GetDlgItem(IDC_EDIT2)->SetWindowText("");
	GetDlgItem(IDC_EDIT3)->SetWindowText("");
	GetDlgItem(IDC_EDIT5)->SetWindowText("");
	GetDlgItem(IDC_EDIT4)->SetWindowText("");
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText("");

	Oninittext();
	UpdateData(FALSE);
}
///////与数据库连接来获取通道号
void CTest2Dlg::OnConnectBUTTON() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	NET_DVR_Init();
	//注册设备	
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	//IP控件转换变成CString类型
	unsigned char *pIP;
	CString strIP,temp5,temp,temp9;
	DWORD dwIP;

	m_mIP.GetAddress(dwIP);
	pIP = (unsigned char*)&dwIP;
	strIP.Format("%u.%u.%u.%u",*(pIP+3),*(pIP+2),*(pIP+1),*pIP);
	temp5.Format("%s",strIP);

	lUserID = NET_DVR_Login_V30(temp5.GetBuffer(0), atoi(m_port), m_user.GetBuffer(0),m_pwd.GetBuffer(0), &struDeviceInfo);
	if (lUserID < 0)
	{
		AfxMessageBox("登入失败");
		m_SecList.DeleteAllItems();
		NET_DVR_Cleanup();
		return;
	}
	else
	{
		AfxMessageBox("登录成功");
		temp.Format("JMQ%s",m_num);
		//通过SDK获得设备能力，获取每个IP的通道号
		NET_DVR_MATRIX_ABILITY_V41 struDecoderCapability={0};
		NET_DVR_GetDeviceAbility(lUserID,MATRIXDECODER_ABILITY_V41,NULL,0,(char *)&struDecoderCapability,sizeof(struDecoderCapability));
		int bcnum = struDecoderCapability.struBncInfo.byChanNums;
		temp9.Format("%d",bcnum);
		//写入到文件保存通道个数
		WritePrivateProfileString(temp,"N",temp9,".//config.ini");
		//刷新列表显示数据
		OinitextSecond(m_num);
		return;
	}
}
///右边列表控件刷新数据显示
void CTest2Dlg::OinitextSecond(CString m_num)
{
	UpdateData(FALSE);
	CString temp,temp1,temp2,temp3,temp4,temp5,temp9;
	int m=0,inum,z,k=1;
	m_SecList.DeleteAllItems();

	temp.Format("JMQ%s",m_num);
	//读取每个解码器的通道个数
	inum = GetPrivateProfileInt(temp,"N",0,".//config.ini");

	z = _ttoi(m_total);
	  //通过循环把数据写入
		for (k = 1 ; k <= inum ; k++)
		{	
			temp2.Format("%d",k);
			temp3.Format("%d",m);
			temp4.Format("%s",m_num);
//			m_SecList.InsertItem(m,temp3);		
//			m_SecList.SetItemText(m,1,temp4);
			m_SecList.InsertItem(m,temp2);
			//配置文件考车号的获取
			temp5.Format("BNC%s",temp2);
			GetPrivateProfileString(temp,temp5,"",temp1.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
			m_SecList.SetItemText(m,1,temp1);
		    m++;
		}		
}
///把考车的信息显示到编辑控件当中
void CTest2Dlg::OnClickLISTSecond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_carnum = m_SecList.GetItemText(pNMListView->iItem,1);
	UpdateData(FALSE);
	///获取到通道号来写入考车号到配置文件
	m_passway = m_SecList.GetItemText(pNMListView->iItem,0);
	*pResult = 0;
}
/////增加或者修改配置文件中的考车号
void CTest2Dlg::OnCarAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp9,temp1,temp2,temp3,temp4,temp5,temp6,str;
	int inum,number,compare,k=1;
	temp1.Format("JMQ%s",m_num);
	temp2.Format("BNC%s",m_passway);
	temp3.Format("%s",m_carnum);

	if (m_carnum.IsEmpty())
	{
		AfxMessageBox("错误提示:考车号不能空");
		return ;
	}
	//读取每个解码器的通道个数
	inum = GetPrivateProfileInt(temp1,"N",0,".//config.ini");
	number = inum;
	if (number == 0)
	{
		AfxMessageBox("测试连接失败，无法添加考车号");
		m_carnum=_T("");
		return;
	}

	compare=_ttoi(m_passway);
	if ( compare > number)
	{
		AfxMessageBox("所选择的通道号,超过通道号的个数，请重新选择通道号");
		return;
	}
	if (temp2 == "BNC")
	{
		AfxMessageBox("请选择通道号");
		return;
	}
	//判断在文件中考车号是否已经存在过了
	for (k ;k <= inum ; k++)
	{
		temp5.Format("BNC%d",k);
		GetPrivateProfileString(temp1,temp5,"",temp6.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
		temp6.ReleaseBuffer();
		if (temp3 == temp6)
		{
			str.Format("%s考车号已经存在,请重输入考车号",temp3);
			AfxMessageBox(str);
			GetDlgItem(IDC_EDIT_Carnum)->SetWindowText("");
			return;
		}
	}
	temp9.Format("是否要修改或添加:\n通道号为:%s 考车号:%s\n",m_passway,m_carnum);
	if (MessageBox(temp9,"提示",MB_OKCANCEL) == IDOK)
	{	//修改或者增加
		WritePrivateProfileString(temp1,temp2,temp3,".//config.ini");
	}

	OinitextSecond(m_num);

}
//////删除考车号
void CTest2Dlg::OnCarDeleteBUTTON() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp1,temp2,temp,temp5;
	int num,compare,number;

	temp5.Format("JMQ%s",m_num);
	//读取每个解码器的通道个数
	if (m_carnum.IsEmpty())
	{
		AfxMessageBox("错误提示:考车号不能空");
		return ;
	}
	num = GetPrivateProfileInt(temp5,"N",0,".//config.ini");
	if (num == 0)
	{
		AfxMessageBox("测试连接失败，无法添加考车号");
		m_carnum = _T("");
		return;
	}
	number = num;
	compare = _ttoi(m_passway);
	if ( compare > number)
	{
		AfxMessageBox("所选择的通道号,超过通道号的个数，请重新选择通道号");
		return;
	}
	temp2.Format("BNC%s",m_passway);
	if (temp2 == "BNC")
	{
		AfxMessageBox("请选择通道号");
		return;
	}
	temp.Format("是否要删除:\n 通道号为:%s 考车号:%s\n",m_passway,m_carnum);	
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		temp1.Format("JMQ%s",m_num);

		//删除数据到配置文件中
		WritePrivateProfileString(temp1,temp2,NULL,".//config.ini");			
	}
	m_carnum = _T("");
	//刷新界面
	OinitextSecond(m_num);
}
//响应事件刷新界面当鼠标点击列表控件，自动刷新另外一个列表
void CTest2Dlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString temp,temp1;

	m_mIP.SetWindowText(m_JLLlist.GetItemText(pNMListView->iItem,1));

	m_user = m_JLLlist.GetItemText(pNMListView->iItem,2);
	m_pwd = m_JLLlist.GetItemText(pNMListView->iItem,3);
	m_port = m_JLLlist.GetItemText(pNMListView->iItem,4);
	m_num = m_JLLlist.GetItemText(pNMListView->iItem,0);	
	
	temp1.Format("JMQ%s",m_num);
	int inum = GetPrivateProfileInt(temp1,"N",0,".//config.ini");
	if (inum == 0)
	{
		m_SecList.DeleteAllItems();
	}
	else
	{
		OinitextSecond(m_num);
	}			
	m_carnum = _T("");
	UpdateData(FALSE);
	*pResult = 0;
}

/////双击改变列表控件为编辑框输入内容
void CTest2Dlg::OnDblclkLISTSecond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;  
    CRect rc;  
    m_Row = pNMListView->iItem;//获得选中的行  
    m_Col = pNMListView->iSubItem;//获得选中列  	   	   
    if (pNMListView->iSubItem != 0) //如果选择的是子项;  
    {  
	    m_SecList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);//获得子项的RECT；
  	    if (m_Row == -1)
		{
	 	    return;
		}		   
	    m_Secedit.SetParent(&m_SecList);//转换坐标为列表框中的坐标  
	    m_Secedit.MoveWindow(rc);//移动Edit到RECT坐在的位置;  
	    m_Secedit.SetWindowText(m_SecList.GetItemText(m_Row,m_Col));//将该子项中的值放在Edit控件中；  
	    m_Secedit.ShowWindow(SW_SHOW);//显示Edit控件  
	    m_Secedit.SetFocus();//设置Edit焦点  
	    m_Secedit.ShowCaret();//显示光标  
	    m_Secedit.SetSel(-1);//将光标移动到最后 
	}
	///保存一个全局变量是的数据保存不出错 
	NochangeNum=m_num;
    *pResult = 0;
}
///在列表控件中编辑框写入配置文件
void CTest2Dlg::SetListItemText(void)  
{  		
	UpdateData(TRUE);
	CString tem,temp1,temp2,temp6,temp5;
	int inum,k = 1;
	m_Secedit.GetWindowText(tem);    //得到用户输入的新的内容  	
	if (m_Col == 1)
	{			
		temp1.Format("JMQ%s",m_num);
		temp2.Format("BNC%s",m_passway);			
		//读取每个解码器的通道个数
		inum = GetPrivateProfileInt(temp1,"N",0,".//config.ini");
		if (inum == 0)
		{
		//	AfxMessageBox("测试连接失败，无法添加考车号");
			m_Secedit.SetWindowText(_T(""));
			m_Secedit.ShowWindow(SW_HIDE);
			return;
		}

		//使用一个全局变量，保证在写入数据的时候不会发生写入错误
		//////////////////////////////////////////////////////////////////////////
		if (NochangeNum != m_num)
		{
			m_Secedit.SetWindowText(_T(""));
			m_Secedit.ShowWindow(SW_HIDE);
			return;
		}
		//////////////////////////////////////////////////////////////////////////

		//判断在文件中考车号是否已经存在过了
		for (k ; k <= inum ; k++)
		{
			temp5.Format("BNC%d",k);
			GetPrivateProfileString(temp1,temp5,"",temp6.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
			temp6.ReleaseBuffer();
			if (tem == temp6)
			{
				m_Secedit.SetWindowText(_T(""));
				m_Secedit.ShowWindow(SW_HIDE);
				return;
			}
		}

		WritePrivateProfileString(temp1,temp2,tem,".//config.ini");									
		m_SecList.SetItemText(m_Row,m_Col,tem);   //设置编辑框的新内容 
		GetDlgItem(IDC_EDIT_Carnum)->SetWindowText(tem);//设置到编辑框当中同步数据
		m_Secedit.ShowWindow(SW_HIDE);				
	}
	
} 
///鼠标失去焦点事件函数 
void CTest2Dlg::OnKillfocusSecEDIT() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_Carnum)->SetWindowText("");
	//写入到配置文件当中
	SetListItemText();

}
////通道对应配置按钮
void CTest2Dlg::OnBtnpz() 
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
////响应事件通过下拉框把EVEN写到配置文件当中
void CTest2Dlg::OnSelchangeComboOne() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxone.GetCurSel();
	CString strCBText,temp;	
	m_boxone.GetLBText(nIndex,strCBText);
	temp.Format("是否要设置EVEN为:%s",strCBText);	
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		if (_T("不隔行解码,全部都解")==strCBText)
		{
			strCBText=_T("0");
		}
		else
		{
			strCBText=_T("1");
		}
		WritePrivateProfileString("JMQ","EVEN",strCBText,".//config.ini");
	}

}
////响应事件通过下拉框把KSKM写到配置文件当中
void CTest2Dlg::OnSelchangeComboTwo() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxtwo.GetCurSel();
	CString strCBText,temp;	
	m_boxtwo.GetLBText(nIndex,strCBText);
	temp.Format("是否要设置KSKM为:%s",strCBText);	
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		WritePrivateProfileString("CONFIG","KSKM",strCBText,".//config.ini");
	}
}
////响应事件通过下拉框把VIDEOWND写到配置文件当中
void CTest2Dlg::OnSelchangeComboThree() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxthree.GetCurSel();
	CString strCBText,temp;	
	m_boxthree.GetLBText(nIndex,strCBText);
	temp.Format("是否要设置VIDEOWND为:%s",strCBText);	
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		WritePrivateProfileString("CONFIG","VIDEOWND",strCBText,".//config.ini");
	}
}
//响应事件让数据只能被限制在0到3之间
void CTest2Dlg::OnChangeEDITplay1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num1;
	GetDlgItem(IDC_EDIT_play1)->GetWindowText(tmp); //每次输入都获取并且判断 大于提示重置

	num1 = tmp.GetLength();
	if(num1 == 1  || num1 == 0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("写入的数据只能是  0,1,2,3");
			GetDlgItem(IDC_EDIT_play1)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY1，数据只能是0,1,2,3,并且不允许有相同,请重新输入!");
		GetDlgItem(IDC_EDIT_play1)->SetWindowText("");
	}
}
//响应事件让数据只能被限制在0到3之间
void CTest2Dlg::OnChangeEDITplay2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num2;
	GetDlgItem(IDC_EDIT_play2)->GetWindowText(tmp); //每次输入都获取并且判断 大于提示重置
	num2 = tmp.GetLength();
	if (num2 == 1  || num2 == 0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("写入的数据只能是  0,1,2,3");
			GetDlgItem(IDC_EDIT_play2)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY2，数据只能是0,1,2,3,并且不允许有相同,请重新输入!");
		GetDlgItem(IDC_EDIT_play2)->SetWindowText("");
	}

}
//响应事件让数据只能被限制在0到3之间
void CTest2Dlg::OnChangeEDITplay3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num3;
	GetDlgItem(IDC_EDIT_play3)->GetWindowText(tmp); //每次输入都获取并且判断 大于提示重置
	num3 = tmp.GetLength();
	if (num3 == 1 || num3 ==0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("写入的数据只能是  0,1,2,3");
			GetDlgItem(IDC_EDIT_play3)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY3，数据只能是0,1,2,3,并且不允许有相同,请重新输入!");
		GetDlgItem(IDC_EDIT_play3)->SetWindowText("");
	}

}
//响应事件让数据只能被限制在0到3之间
void CTest2Dlg::OnChangeEDITplay4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num4;
	GetDlgItem(IDC_EDIT_play4)->GetWindowText(tmp); //每次输入都获取并且判断 大于提示重置
	num4 = tmp.GetLength();
	if ( num4 == 1 || num4 == 0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("写入的数据只能是  0,1,2,3");
			GetDlgItem(IDC_EDIT_play4)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY4，数据只能是0,1,2,3,并且不允许有相同,请重新输入!");
		GetDlgItem(IDC_EDIT_play4)->SetWindowText("");
	}

}
////配置文件中DISPLAY的保存
void CTest2Dlg::OnSaveplayBUTTON() 
{
	// TODO: Add your control notification handler code here
	CString temp1 = "",temp2 = "",temp3 = "",temp4 = "",temp;
	GetDlgItem(IDC_EDIT_play1)->GetWindowText(temp1);
	GetDlgItem(IDC_EDIT_play2)->GetWindowText(temp2);
	GetDlgItem(IDC_EDIT_play3)->GetWindowText(temp3);
	GetDlgItem(IDC_EDIT_play4)->GetWindowText(temp4);
	if (temp1.IsEmpty() || temp2.IsEmpty() ||temp3.IsEmpty() ||temp4.IsEmpty() )
	{
		AfxMessageBox("DISPLAY数据不能为空");
		return;
	}
	//判断值是否有重复，方法比较LOW,一个个的互相匹配
	//////////////////////////////////////////////////////////////////////////
	if (temp1 == temp2)
	{
		AfxMessageBox("DISPLAY数据值，不允许有相同");
		return;
	}
	if (temp1 == temp3)
	{
		AfxMessageBox("DISPLAY数据值，不允许有相同");
		return;
	}
	if (temp1 == temp4)
	{
		AfxMessageBox("DISPLAY数据值，不允许有相同");
		return;
	}
	if (temp2 == temp3)
	{
		AfxMessageBox("DISPLAY数据值，不允许有相同");
		return;
	}
	if (temp2 == temp4)
	{
		AfxMessageBox("DISPLAY数据值，不允许有相同");
		return;
	}
	if (temp3 == temp4)
	{
		AfxMessageBox("DISPLAY数据值，不允许有相同");
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	///写入配置文件
	temp.Format("是否要配置\n DISPLAY1为:%s\n DISPLAY2为:%s\n DISPLAY3为:%s\n DISPLAY4为:%s",temp1,temp2,temp3,temp4);	
	if (MessageBox(temp,"提示",MB_OKCANCEL) == IDOK)
	{
		WritePrivateProfileString("CONFIG","DISPLAY1",temp1,".//config.ini");
		WritePrivateProfileString("CONFIG","DISPLAY2",temp2,".//config.ini");
		WritePrivateProfileString("CONFIG","DISPLAY3",temp3,".//config.ini");
		WritePrivateProfileString("CONFIG","DISPLAY4",temp4,".//config.ini");
	}
	//////////////////////////////////////////////////////////////////////////
}

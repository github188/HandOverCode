// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "Windows.h"
#include "Mmsystem.h"
#pragma comment(lib,"Winmm.lib")

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
	m_IP = _T("192.168.0.165");
	m_User = _T("admin");
	m_PWD = _T("hk12345678");
	m_Edit4 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_LIST1, m_List1);
	DDX_Text(pDX, IDC_EDIT1, m_IP);
	DDX_Text(pDX, IDC_EDIT2, m_User);
	DDX_Text(pDX, IDC_EDIT3, m_PWD);
	DDX_Text(pDX, IDC_EDIT4, m_Edit4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
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
	itypeSPZD=0;
	m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List1.InsertColumn(0,_T("报警"),LVCFMT_CENTER,320);

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
void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	TRACE("MessageCallback %d",lCommand);
	CTestDlg *pInfo =(CTestDlg *)pUser;
	switch(lCommand) 
	{  
	case COMM_ALARM_V30:
		{
			NET_DVR_ALARMINFO_V30 m_struAlarmInfoV30;
			memcpy(&m_struAlarmInfoV30, pAlarmInfo, sizeof(NET_DVR_ALARMINFO_V30));
			TRACE("COMM_ALARM_V30 itype=%d,%d\n",pInfo->itypeSPZD,pInfo->dwTimeSPZD);
			if (m_struAlarmInfoV30.dwAlarmType==6)
			{
				if (pInfo->itypeSPZD==0)
				{
					pInfo->dwTimeSPZD=timeGetTime();
					pInfo->itypeSPZD++;
				}
				else
				{
					if (timeGetTime()-pInfo->dwTimeSPZD>2000)
					{
						pInfo->itypeSPZD=0;
					}
					else
					{
						pInfo->dwTimeSPZD=timeGetTime();
						pInfo->itypeSPZD++;
					}
					if (pInfo->itypeSPZD>2)
					{
						pInfo->itypeSPZD=0;
						CTime NowTime=CTime::GetCurrentTime();
						CString strTime;
						strTime.Format("[%s]:视频被遮挡!",NowTime.Format("%y-%m-%d %H:%M:%S"));
						int k=pInfo->m_List1.GetItemCount();
						pInfo->m_List1.InsertItem(k,strTime);
						TRACE(strTime+"\n");
						
					}
				}

				//AfxMessageBox("视频被遮挡!");
			}
		}
		break;
	default:
		break;
	}
}

void CTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	NET_DVR_Init();//初始化SDK
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	//从配置文件读配置  这个不用我写了吧
	UpdateData(TRUE);
	lUserID = NET_DVR_Login_V30(m_IP.GetBuffer(0), 8000, m_User.GetBuffer(0),m_PWD.GetBuffer(0), &struDeviceInfo);
	if (lUserID < 0)
	{
		AfxMessageBox("登录设备失败!");
		NET_DVR_Cleanup(); 
		return;
	}	
	//////////////////////////////////////////////////////////////////////////
	//2017
	DWORD dwReturn = 0;
	NET_DVR_PICCFG_V30 struDVRPicCfgV30;
	memset(&struDVRPicCfgV30,0,sizeof(NET_DVR_PICCFG_V30));
	if(!NET_DVR_GetDVRConfig(lUserID,NET_DVR_GET_PICCFG_V30,1,&struDVRPicCfgV30,sizeof(NET_DVR_PICCFG_V30),&dwReturn))
	{
		TRACE("获取设备的配置信息失败!Errid=%d\n",NET_DVR_GetLastError());
		return ;
	}
	struDVRPicCfgV30.struHideAlarm.dwEnableHideAlarm=1;//0-否，1-低灵敏度，2-中灵敏度，3-高灵敏度
	struDVRPicCfgV30.struHideAlarm.wHideAlarmAreaTopLeftX=100;
	struDVRPicCfgV30.struHideAlarm.wHideAlarmAreaTopLeftY=100;
	struDVRPicCfgV30.struHideAlarm.wHideAlarmAreaWidth=450;
	struDVRPicCfgV30.struHideAlarm.wHideAlarmAreaHeight=320;
	struDVRPicCfgV30.struHideAlarm.strHideAlarmHandleType.dwHandleType=12;
	struDVRPicCfgV30.struHideAlarm.strHideAlarmHandleType.byRelAlarmOut[0]=1;
	struDVRPicCfgV30.struHideAlarm.strHideAlarmHandleType.byRelAlarmOut[1]=1;
	if (!NET_DVR_SetDVRConfig(lUserID,NET_DVR_SET_PICCFG_V30,1,&struDVRPicCfgV30,sizeof(NET_DVR_PICCFG_V30)))
	{
		TRACE("设置设备的配置信息失败!Errid=%d\n",NET_DVR_GetLastError());
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//设置报警回调函数
	NET_DVR_SetDVRMessageCallBack_V30(MessageCallback, this);	
	//启用布防	
	//布防参数
	NET_DVR_SETUPALARM_PARAM  struAlarmParam={0};
	struAlarmParam.dwSize=sizeof(struAlarmParam);
	struAlarmParam.byAlarmInfoType=0; 
	lHandle = NET_DVR_SetupAlarmChan_V41(lUserID, & struAlarmParam);
	if (lHandle < 0)
	{
		AfxMessageBox("NET_DVR_SetupAlarmChan_V41 布防参数失败!");
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup(); 
		return;
	}
	m_Edit4.Format("登录成功,启用布防完成!");
	UpdateData(FALSE);
}

void CTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
//	Sleep(5000);
	//撤销布防上传通道
// 	if (!NET_DVR_CloseAlarmChan_V30(lHandle))
// 	{
// 		printf("NET_DVR_CloseAlarmChan_V30 error, %d\n", NET_DVR_GetLastError());
// 		NET_DVR_Logout(lUserID);
// 		NET_DVR_Cleanup(); 
// 		return;
// 	}
// 	
// 	//注销用户
// 	NET_DVR_Logout(lUserID);
// 	//释放SDK资源
// 	NET_DVR_Cleanup();
// 	return;
}


void CTestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if (lUserID< 0)
	{
		AfxMessageBox("请先登录设备");
		return;
	}
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_WND)->GetSafeHwnd();
	ClientInfo.lChannel     = 1;
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;
	
	m_lPlayHandle= NET_DVR_RealPlay_V30(lUserID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle)
	{
		AfxMessageBox("预览失败!");
		return ;
	}
}

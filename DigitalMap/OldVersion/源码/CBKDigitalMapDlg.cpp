// CBKDigitalMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCBKDigitalMapApp theApp;
extern CPoint temPoint;  //当前对话框移动到的位置

//主驾驶视频

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
unsigned long g_ulProcessId;
//副驾驶视频

BOOL CALLBACK EnumWndProcT(HWND hWnd, LPARAM lParam);
unsigned long g_ulProcessIdT;
//车外视频

BOOL CALLBACK EnumWndProcS(HWND hWnd, LPARAM lParam);
unsigned long g_ulProcessIdS;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

UINT ParsePackage_thd(LPVOID p);
UINT SendCurCarMsg_thd(LPVOID p);  //用于发送消息用的 当前在线考车数
UINT ExamCarGPSLocate_thd(LPVOID p); //用于对应的先乘客处理对应的实时考车GPS信息
UINT WakeUpExamCar_thd(LPVOID p);// 创建一个线程专门用来唤醒考车线程
UINT HideExamCar_thd(LPVOID p);// 创建一个线程专门用来隐藏考车 做这个主要是因为如果考车已经上线了，
									//而且中途离线，可能造成属于他的一些包还没有出来完，所以就又显示出来了，所以用线程将他关闭掉

UINT HideOtherCar_thd(LPVOID p);//这一个线程用于如果有考车被跟踪的时候，别的考车就隐藏

UINT UpdateMap_thd(LPVOID p);//专门用一个线程定时的刷新考车和地图

UINT ButtonUp_thd(LPVOID p);//专门用来设置鼠标移动后弹起的标志位

UINT ListenExamCar_thd(LPVOID p);//专门用来监听在线考车数  主要是用于考车离线用的 考车自动上线已经在socket那里处理了
UINT ListenExamCarOnline_thd(LPVOID p);//专门用来监听在线考车数  主要是用于考车上线用的 考车自动上线已经在socket那里处理了

UINT ExamCarHideDown_thd(LPVOID p);//专门用来设置考车下线的时候隐藏考车

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
//zsz
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
// CCBKDigitalMapDlg dialog

CCBKDigitalMapDlg::CCBKDigitalMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBKDigitalMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCBKDigitalMapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBKDigitalMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	int i = 0;
	//{{AFX_DATA_MAP(CCBKDigitalMapDlg)
	DDX_Control(pDX, IDC_STATIC_TOTALCAR, m_TotalCar);
	DDX_Control(pDX, IDC_STATIC_BOOT, m_boot);

	DDX_Control(pDX, IDC_BUTTON0, m_Button[0]);
	DDX_Control(pDX, IDC_BUTTON1, m_Button[1]);
	DDX_Control(pDX, IDC_BUTTON2, m_Button[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Button[3]);
	DDX_Control(pDX, IDC_BUTTON4, m_Button[4]);
	DDX_Control(pDX, IDC_BUTTON5, m_Button[5]);
	DDX_Control(pDX, IDC_BUTTON6, m_Button[6]);
	DDX_Control(pDX, IDC_BUTTON7, m_Button[7]);
	DDX_Control(pDX, IDC_BUTTON8, m_Button[8]);
	DDX_Control(pDX, IDC_BUTTON9, m_Button[9]);

	DDX_Control(pDX, IDC_BUTTON10, m_Button[10]);
	DDX_Control(pDX, IDC_BUTTON11, m_Button[11]);
	DDX_Control(pDX, IDC_BUTTON12, m_Button[12]);
	DDX_Control(pDX, IDC_BUTTON13, m_Button[13]);
	DDX_Control(pDX, IDC_BUTTON14, m_Button[14]);
	DDX_Control(pDX, IDC_BUTTON15, m_Button[15]);
	DDX_Control(pDX, IDC_BUTTON16, m_Button[16]);
	DDX_Control(pDX, IDC_BUTTON17, m_Button[17]);
	DDX_Control(pDX, IDC_BUTTON18, m_Button[18]);
	DDX_Control(pDX, IDC_BUTTON19, m_Button[19]);
	DDX_Control(pDX, IDC_BUTTON20, m_Button[20]);
 
 	DDX_Control(pDX, IDC_BUTTON21, m_Button[21]);
 	DDX_Control(pDX, IDC_BUTTON22, m_Button[22]);
 	DDX_Control(pDX, IDC_BUTTON23, m_Button[23]);
 	DDX_Control(pDX, IDC_BUTTON24, m_Button[24]);
 	DDX_Control(pDX, IDC_BUTTON25, m_Button[25]);
 	DDX_Control(pDX, IDC_BUTTON26, m_Button[26]);
 	DDX_Control(pDX, IDC_BUTTON27, m_Button[27]);
 	DDX_Control(pDX, IDC_BUTTON28, m_Button[28]);
 	DDX_Control(pDX, IDC_BUTTON29, m_Button[29]);
 	DDX_Control(pDX, IDC_BUTTON30, m_Button[30]);
 
 	DDX_Control(pDX, IDC_BUTTON31, m_Button[31]);
 	DDX_Control(pDX, IDC_BUTTON32, m_Button[32]);
 	DDX_Control(pDX, IDC_BUTTON33, m_Button[33]);
 	DDX_Control(pDX, IDC_BUTTON34, m_Button[34]);
 	DDX_Control(pDX, IDC_BUTTON35, m_Button[35]);
 	DDX_Control(pDX, IDC_BUTTON36, m_Button[36]);
 	DDX_Control(pDX, IDC_BUTTON37, m_Button[37]);
 	DDX_Control(pDX, IDC_BUTTON38, m_Button[38]);
 	DDX_Control(pDX, IDC_BUTTON39, m_Button[39]);
 
 	DDX_Control(pDX, IDC_BUTTON40, m_Button[40]);
 	DDX_Control(pDX, IDC_BUTTON41, m_Button[41]);
 	DDX_Control(pDX, IDC_BUTTON42, m_Button[42]);
 	DDX_Control(pDX, IDC_BUTTON43, m_Button[43]);
 	DDX_Control(pDX, IDC_BUTTON44, m_Button[44]);
 	DDX_Control(pDX, IDC_BUTTON45, m_Button[45]);
 	DDX_Control(pDX, IDC_BUTTON46, m_Button[46]);
 	DDX_Control(pDX, IDC_BUTTON47, m_Button[47]);
 	DDX_Control(pDX, IDC_BUTTON48, m_Button[48]);
 	DDX_Control(pDX, IDC_BUTTON49, m_Button[49]);

	DDX_Control(pDX, IDC_BUTTON50, m_Button[50]);
	DDX_Control(pDX, IDC_BUTTON51, m_Button[51]);
	DDX_Control(pDX, IDC_BUTTON52, m_Button[52]);
	DDX_Control(pDX, IDC_BUTTON53, m_Button[53]);
	DDX_Control(pDX, IDC_BUTTON54, m_Button[54]);
	DDX_Control(pDX, IDC_BUTTON55, m_Button[55]);
	DDX_Control(pDX, IDC_BUTTON56, m_Button[56]);
	DDX_Control(pDX, IDC_BUTTON57, m_Button[57]);
	DDX_Control(pDX, IDC_BUTTON58, m_Button[58]);
 	DDX_Control(pDX, IDC_BUTTON59, m_Button[59]);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCBKDigitalMapDlg, CDialog)
	//{{AFX_MSG_MAP(CCBKDigitalMapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_MESSAGE(WM_BTN_MSGZ,OnBtnMsgz)  
	ON_MESSAGE(WM_BTN_MSGZLeft,OnBtnMsgzLeft)
	ON_WM_CTLCOLOR()  //将控件的背景改为透明
	ON_MESSAGE(WM_BTN_MSG_CAR,OnUpdataCurCar)  
	ON_WM_MOUSEWHEEL() //鼠标中间的滑块
	ON_WM_LBUTTONDOWN() //鼠标左键按下
	ON_WM_LBUTTONUP()  // 鼠标左键提起
	ON_WM_MOUSEMOVE() //鼠标移动
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapDlg message handlers

BOOL CCBKDigitalMapDlg::OnInitDialog()
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
	DeleteLogData();
	
	ShowWindow(SW_SHOWMAXIMIZED);// 全屏

	int Pi;
	for(Pi = 0;Pi < MAX_ExamCar;Pi++)
	{
		bMouseOn[Pi] = 0;  //这个是用于判断鼠标有没有在这辆考车上  TRUE为在这考车上 FALSE 没有落在这辆考车上
	}
	
	iISCreate = 0; //0表示还没有创建这个字进程 1表示已经创建
	iISCreateT = 0; //0表示还没有创建这个字进程 1表示已经创建
	iISCreateS = 0; //0表示还没有创建这个字进程 1表示已经创建
	iStateDlgShow = 0;  //这一个标志位是用来对话框伸长和收回去的时候用的  如果为1 说明正在伸长或收回去 为0则是正常状态
	isRunningUpdate = 0;
	iStateLBD = 0;//用来标记鼠标左键按下 拖动时为1 正常状态是为0

	//设置Static字体  
    m_Font.CreatePointFont(200, "Arial", NULL);  
    m_TotalCar.SetFont(&m_Font,true);  
//	m_TotalCar.MoveWindow(100,100,400,200);
	ReadOnlineCarFromDB();//当前考车数
	CString tmpOnLineCar;
	tmpOnLineCar.Format("当前考车数为：%d",pSocket.TotalCar);
    m_TotalCar.SetWindowText(tmpOnLineCar);  

	
	m_bmpMap.m_hObject = LoadImage(NULL,".\\GNSSmap.dat",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	pBitmapDlg.m_hObject = LoadImage(NULL,".\\boxtip2.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (m_bmpMap.m_hObject==NULL)
	{
		AfxMessageBox("加载地图失败!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE;
	}
	
	CDC *pDC = this->GetDC();   //返回CWnd客户区的设备环境
	CRect ClientRect; //客户端区域
	GetClientRect(ClientRect);

	
	////////////     当前考车视频实时预览    /////////////////////////
	
	g_ulProcessId = 0UL;
	g_ulProcessIdT = 0UL;
	g_ulProcessIdS = 0UL;
	


	////////////                 结束        /////////////////////////

	///////////////////////////
	//  IDB_BITMAP1
	pBitmap.m_hObject = LoadImage(NULL,".\\TT.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	m_Cmem.CreateCompatibleDC(pDC); // 创建一个与设备兼容的内存
	m_Cbuf.CreateCompatibleDC(pDC);
	m_CbufDlg.CreateCompatibleDC(pDC);
	pBG.CreateCompatibleBitmap(pDC,ClientRect.Width(),ClientRect.Height()); //该函数创建与指定设备兼容的的位图
	m_Cmem.SelectObject(pBG);
	
	////////////////////////////


	//读取配置文件中的MAPCONFIGSTATE 的状态 是否为1 如果不是则则行另一个程序
	CString State;
	GetPrivateProfileString("MAPCONFIGSTATE","State","",State.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	if (atoi(State) != 1)  //如果不是则则行另一个程序
	{
		//ShellExecute(NULL,"open",".\\CBKTestVideoPlayBack.exe","playz save",NULL,SW_SHOW); 
		ShellExecute(NULL,"open",".\\ReadConfig.exe","",NULL,SW_SHOW); 
		CCBKDigitalMapDlg::OnCancel();
	}

	
	//读取配置文件的坐标点 左上的 和 右下的点
	CString left,right,top,bottom;
	GetPrivateProfileString("Point","Left","",left.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	GetPrivateProfileString("Point","Right","",right.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	GetPrivateProfileString("Point","Top","",top.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	GetPrivateProfileString("Point","Bottom","",bottom.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	MAPRight = (atof)(right);
	MAPLeft = (atof)(left);
	MAPTop = (atof)(top);
	MAPBotom = (atof)(bottom);

	OriginalMAPBotom = MAPBotom;//备份原始GPS值 
	OriginalMAPLeft = MAPLeft;//备份原始GPS值 
	OriginalMAPTop = MAPTop;//备份原始GPS值 
	OriginalMAPRight = MAPRight;//备份原始GPS值 

	x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
	y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度
	CurrentScrHeight = ClientRect.bottom - ClientRect.top;  //当前屏幕高度
	CurrentScrWidth = ClientRect.right - ClientRect.left;  //当前屏幕宽度

	x_Originalpixel = x_pixel;// 原始的x
	y_Originalpixel = y_pixel;// 原始的y

	
	
	BITMAP bitmap; 
	CBitmap	m_bmpTmp;
	m_dcMem.CreateCompatibleDC(pDC);  // CDC	 m_dcMem;   产生与pDC指定设备兼容的设备上下文内存
	m_bmpMap.GetBitmap(&bitmap);  // 本函数用于查看Cbitmap对象的信息。返回的信息存放在bitmap指向的BITMAP结构中
	y_BitWidth = bitmap.bmHeight; //获取图片的高度
	x_BitWidth = bitmap.bmWidth;  //获取图盘的宽度
	y_OriginalBit = bitmap.bmHeight; //获取图片的高度  保存原来图片的大小
	x_OriginalBit = bitmap.bmWidth; //获取图片的宽度  保存原来图片的大小
	x_BitWidth = ClientRect.Width();
	y_BitWidth = ClientRect.Height();
	m_bmpTmp.CreateCompatibleBitmap(pDC,bitmap.bmWidth,bitmap.bmHeight);  //初始化一个与pDC指定的设备上下文兼容的位图
	m_dcMem.SelectObject(m_bmpTmp);  // 应用只能将位图选入内存设备上下文中
	this->ReleaseDC(pDC);  // 释放设备环境，以供其它应用程序使用
	m_dcMem.SelectObject(m_bmpMap);

	//计算当前的一个像素代表多少经纬度
	x_pixel = ((MAPRight-MAPLeft) / x_OriginalBit);
	y_pixel = ((MAPTop-MAPBotom) / y_OriginalBit);
	MAPRight = MAPLeft + x_pixel * ClientRect.Width();
	MAPBotom = MAPTop - y_pixel * ClientRect.Height();

	//初始化50辆科目三的考车  初始化转开为都没有开始考试
	int i = 0;
	for (i = 0;i < MAX_ExamCar; i ++)
	{
		ExamCarState[i] = FALSE;
	}

	//移动提示框--启动程序提示
	m_boot.SetFont(&m_Font,true);  
	m_boot.MoveWindow(CurrentScrWidth / 2 - 150,CurrentScrHeight / 2 - 25,300,50);
	CString bootText;
	bootText.Format("程序启动中，请稍等...");
	m_boot.SetWindowText(bootText);
	///////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////


	//初始化 考车当前GPS点的信息
	memset(on_ExamCar,0,sizeof(ExamCar_ST) * MAX_ExamCar);

	for (i = 0;i < MAX_ExamCar;i++)
	{
		m_Button[i].LoadBitmap(IDB_BMP_BTNBK);//加载按钮图片
		m_Button[i].MoveWindow(-20,-20,20,20);
		m_Button[i].ShowWindow(SW_HIDE);
		m_Button[i].examCarNumber = 0; //将考车号传进去  已经把中这条语句放到上面的ReadOnlineCarFromDB函数里面
	}
	for(i = 1;i <= pSocket.TotalCar;i++)
	{
		m_Button[i].examCarNumber = SockToCar1.find(i)->second;
		m_Button[i].ThreadNumber = i;
	}

	iNowZoomRunning = 0;//正在放大缩小刷新 1 正在刷新 0 没有在刷新
	ZoomLevel = 20;// 放大等级设置
	ZoomLevel1 = 0;
	x_StartBit = 0;//开始重绘在图片的起始坐标x轴
	y_StartBit = 0;//开始重绘在图片的起始坐标y轴
	PixState = 0;  //原图标志 如果已经缩小到原图状态 则用0表示 如果放大 则用1 表示

	LButtonDown = FALSE; //是用来拖动画布的 当为TRUE时，鼠标按下可以开始拖动画布 FALSE时鼠标左键释放不能再拖动画布
	HadWakeUp = 0;//表示线程已经唤醒了  0 表示考车线程已经被唤醒了 1 代表线程需要唤醒
	iCountFlushMap = 0;//用来统计地图刷新的次数 当为0的时候才可以刷新按钮 
	iCountFlushExamCar = 0;//用来统计考车刷新的次数 当为0的时候才可以刷新地图框

	iContrl = 1;

//	m_TotalCar.MoveWindow(10,10,200,60);
//	CString title;
//	title.Format("考车数量为：0");
//	m_TotalCar.SetCaption(title);
//	m_TotalCar.SetBkMode(TRANSPARENT);// 背景透明


	SetTimer(GPS_Dlg,100,NULL);
	
	/*
	for (i = 0;i < MAX_ExamCar;i++)
	{
		dlgha[i].Create(IDD_DIALOG1,this);
		dlgha[i].ShowWindow(SW_HIDE);
	}
	*/


	//创建在线考车的对话框 
	ExamCarInfo.Create(IDD_DIALOG_ONLINECARINFO,this);
	ExamCarInfo.ShowWindow(SW_SHOW);
	ExamCarInfo.MoveWindow(700,-637,280,640);
	

	// 开线程 处理各个主机发来的信息
//	CWinThread *tmpCwinThread;
	AfxBeginThread(&ParsePackage_thd, this);

//	AfxBeginThread(&SendCurCarMsg_thd, this);  //刷仙当前的在线考车数  现在不用了 因为一开始就读取了当前在线考车数
	AfxBeginThread(&WakeUpExamCar_thd, this); // 专门用来唤醒考车线程
	AfxBeginThread(&HideExamCar_thd, this); // 创建一个线程专门用来隐藏考车 做这个主要是因为如果考车已经上线了，
											//而且中途离线，可能造成属于他的一些包还没有出来完，所以就又显示出来了，所以用线程将他关闭掉

	
	AfxBeginThread(&HideOtherCar_thd, this); //用于有考车被跟踪时，别的考车隐藏
	AfxBeginThread(&UpdateMap_thd, this); //专门用一个线程来刷新地图和考车

	AfxBeginThread(&ListenExamCar_thd, this);
	AfxBeginThread(&ListenExamCarOnline_thd, this);
	 
	
//	SuspendThread(PP); //挂起线程
//	ResumeThread(PP);  //唤醒线程

/********************************************************
下面程序是创建考车线程
***********************************************************/

/*
	ExamCarInfoToThread_ST pExamCarInfoToThread_ST;
	pExamCarInfoToThread_ST = (ExamCarInfoToThread_ST)malloc(sizeof(struct ExamCarInfoToThread));
	

	for (i = 0;i < MAX_ExamCar;i++)
	{
		memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
		pExamCarInfoToThread_ST->pMapDlg = this;
		pExamCarInfoToThread_ST->iExamCarNumber = i;
		tmpCwinThread = AfxBeginThread(&ExamCarGPSLocate_thd, pExamCarInfoToThread_ST);
		ThreadHandle[i] = tmpCwinThread->m_hThread;
		if (i == MAX_ExamCar -1)
		{
			memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
			pExamCarInfoToThread_ST->pMapDlg = this;
			pExamCarInfoToThread_ST->iExamCarNumber = 0; //创建的第一个线程 具体为什么要这么做我也不知道 
		}
		Sleep(500);
		SuspendThread(ThreadHandle[i]);  //挂起线程
	}
	
	free(pExamCarInfoToThread_ST);

*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCBKDigitalMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCBKDigitalMapDlg::OnPaint() 
{
	CPaintDC dc(this);
	m_TotalCar.MoveWindow(10,10,240,50);

	SetStretchBltMode(dc.m_hDC,STRETCH_HALFTONE);

	CFont pFont;
	pFont.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                 // lpszFacename

//	pFont.CreatePointFont(100, "Arial", NULL);  
	

	if (IsIconic())
	{
		 // device context for painting

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
//		CPaintDC dc(this); 
		CRect rect; 
//		m_dcMem.SelectObject(m_bmpMap);

		BITMAP bitmap; 
		m_bmpMap.GetBitmap(&bitmap);  // 本函数用于查看Cbitmap对象的信息。返回的信息存放在bitmap指向的BITMAP结构中
		GetClientRect(&rect); 
	//	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
		// 1、要拷贝的位图的位置不能是直接定位 0,0,bitmap.bmWidth,bitmap.bmHeight  
		//    应该通过参数传进去
		// 2、绘制的图片的宽度也不能直接传进去，应该通过参数直接传进去

		/////////////////////////////////////////////////
		/*
		m_Cbuf.SelectObject(pBG1);
		m_Cmem.BitBlt(0,0,1000,800,&m_Cbuf,0,0,SRCCOPY);
		m_Cbuf.SelectObject(pBitmap);
	
		m_Cmem.BitBlt(m_x,m_y,26,26,&m_Cbuf,26,0,SRCAND);  // 进行与运算 
		m_Cmem.BitBlt(m_x,m_y,26,26,&m_Cbuf,0,0,SRCPAINT);//进行或运算
		*/
	//	m_Cbuf.SelectObject(pBG1);

		SetStretchBltMode(m_Cmem.m_hDC,STRETCH_HALFTONE);//该函数可以设置指定设备环境中的位图拉伸模式
		m_Cmem.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,x_StartBit,y_StartBit,x_BitWidth,y_BitWidth,SRCCOPY); 
		m_Cbuf.SelectObject(pBitmap);

	
//zzz
 		int Pi = 0;
		if(iStateLBD == 0)
		{
			
			for(Pi = 0; Pi < MAX_ExamCar;Pi++)
			{
				// int((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel),
				// int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel) - 10)
				if(m_Button[Pi].CurCar_Xpoint > 0 && m_Button[Pi].CurCar_Ypoint > 0 )
				{
					CRect RC1(0,0,20,20);
					CString szCarNumber;
					szCarNumber.Format("%d",m_Button[Pi].examCarNumber);
					
					
					if(ExamCarInfo.ExamCarDlg.CurTrackExamCar == Pi)
					{
						RC1.SetRect(20,0,40,20);
// 						m_Cbuf.SelectObject(pBitmap);
						m_Cbuf.DrawText("   ",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						m_Cbuf.DrawText(szCarNumber,RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						//m_Cbuf.DrawText("abc",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,20,SRCAND);  // 进行与运算 
						m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,20,0,SRCPAINT);//进行或运算
						if (m_Button[Pi].CurCar_EID !=1000)
						{
							//playvoice here
							m_Button[Pi].CurCar_EID=1000;
						}
					}
					else
					{	
						
// 						m_Cbuf.SelectObject(pBitmap);
						m_Cbuf.DrawText("    ",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						m_Cbuf.DrawText(szCarNumber,RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						//m_Cbuf.DrawText("dd",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						if (m_Button[Pi].CurCar_ZT ==1)//状态=1
						{							
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,20,SRCAND);  // 进行与运算 
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,0,SRCPAINT);//进行或运算
						}
						else
						{							
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,20,SRCAND);  // 进行与运算 
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,40,0,SRCPAINT);//进行或运算
						}
						//////////////////////////////////////////////////////////////////////////zzzzz

					}

				}
			}
		}
		/////////////////////////////////////////////////

		//////////////////////////////////////////////////
		m_Cbuf.SelectObject(pBitmapDlg);
		CBitmap bmpBackground; 
		bmpBackground.LoadBitmap(IDB_BMPPHOBK); 
		BITMAP bitmap2; 
		bmpBackground.GetBitmap(&bitmap2);
		CBitmap *pbmpOld=m_CbufDlg.SelectObject(&bmpBackground);
		CRect rt(0,0,bitmap2.bmWidth,bitmap2.bmHeight);
	//	ReadPhotoFromDB(&m_CbufDlg,"520100713081",&rt);
	
		CFont* def_font = m_Cbuf.SelectObject(&pFont);
		
	//	m_Cbuf.BitBlt(240,15,bitmap.bmWidth,bitmap.bmHeight,&m_CbufDlg,0,0,SRCCOPY); //这个是可以的
	//	m_Cbuf.BitBlt(0,0,350,150,&m_CbufDlg,0,0,SRCCOPY);  //这个是可以的



		for(Pi = 1; Pi < MAX_ExamCar;Pi++)
		{
			// int((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel),
			// int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel) - 10)
			if(  m_Button[Pi].CurCar_Xpoint > 0 && m_Button[Pi].CurCar_Ypoint > 0 && (m_Button[Pi].m_bHover == 1 || bMouseOn[Pi] == 1))
			{
				//////  获取消息框的一些信息  ////
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSGZ,m_Button[Pi].examCarNumber,m_Button[Pi].ThreadNumber);
				//////                        ////
				m_CbufDlg.SelectObject(&bmpBackground);

				CRect RC(100,10,200,35);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].CSCAR,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				RC.SetRect(100,45,200,70);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].szExamineeName,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				RC.SetRect(100,80,200,105);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].strzkzm,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				RC.SetRect(100,115,200,140);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].strCarModel,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );

				ReadPhotoFromDB(&m_CbufDlg,dlgha[m_Button[Pi].ThreadNumber].strzkzm,&rt); //这个准考证要怎么来
				m_Cbuf.BitBlt(240,12,bitmap.bmWidth,bitmap.bmHeight,&m_CbufDlg,0,0,SRCCOPY);
			//	m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+20,m_Button[Pi].CurCar_Ypoint+20,350,150,&m_Cbuf,0,0,SRCCOPY);
				//////////////    接下来要判断是哪个区域    ///////////////////
				//接下来要判断4个块的区域 不同的区域加载不同的背景图片
				CRect ClientRect; //客户端区域
				GetClientRect(ClientRect);
				if (m_Button[Pi].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
				{
					//对话框的箭头打向左下角
					
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+10,m_Button[Pi].CurCar_Ypoint-160,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				else if (m_Button[Pi].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
				{	//对话框的箭头打向左上角
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+10,m_Button[Pi].CurCar_Ypoint+10,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				else if (m_Button[Pi].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
				{	//对话框的箭头打向右下角
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+20,m_Button[Pi].CurCar_Ypoint+10,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				else if (m_Button[Pi].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
				{	//对话框的箭头打向右上角
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+10,m_Button[Pi].CurCar_Ypoint-160,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				////////////////////////////////////////////////////
			}
		}



		m_Cbuf.SelectObject(def_font);
		pFont.DeleteObject();
		//////////////////////////////////////////////////

		
	//	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,x_StartBit,y_StartBit,x_BitWidth,y_BitWidth,SRCCOPY); 

		dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_Cmem,0,0,SRCCOPY);

		
		iNowZoomRunning = 0;//没有放大或缩小 这句话本来是写在这里的 现在改写在OnPaint函数尾部 说明刷新完了
		HadWakeUp = 1;//表示线程已经唤醒了  0 表示考车线程已经被唤醒了 1 代表线程需要唤醒
		if (iCountFlushMap > 0)
		{
			iCountFlushMap--;//用来统计地图刷新的次数 当为0的时候才可以刷新按钮 
		}
		
//		WakeUp_Thread();  //把这个唤醒线程的工作在OnPaint里面的尾部实现
// 		CString ff;
// 		ff.Format("%d %d %d %d",x_StartBit,y_StartBit,x_BitWidth,y_BitWidth);
// 		WriteLog(ff);
	
	//	return TRUE;

		//防止GDI泄露
		bmpBackground.DeleteObject();  //防止GDI泄露
		pbmpOld->DeleteObject();
		CDialog::OnPaint();
	}
	
}



// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCBKDigitalMapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCBKDigitalMapDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnEraseBkgnd(pDC);
}

/**********************************8
函数说明：写错误信息进入配置文件
************************************/
void CCBKDigitalMapDlg::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

/**********************************8
函数说明：将车所对应的GPS坐标添加到对应的文件
参数说明：pExamCar_ST 是要写入的考车信息的包
************************************/
void CCBKDigitalMapDlg::WriteGPSPoint(ExamCar_ST *pExamCar_ST)
{
	/*
	ExamCar_ST *ExamCar;
	ExamCar = (ExamCar_ST *) malloc (sizeof(ExamCar_ST));
	memset(ExamCar,0,sizeof(ExamCar_ST));
	memcpy(ExamCar,pExamCar_ST,sizeof(ExamCar_ST));

	if (ExamCarState[ExamCar->ExamCarNumber] == FALSE) //说明是开始后第一次开始写入GPS点
	{
		CTime curTime =CTime::GetCurrentTime();
		CString Data=curTime.Format("开车%d %Y%m%d%H%M%S",ExamCar->ExamCarNumber);
		FILE *fp=fopen(Data,"a+");

		fprintf(fp,"[%s]%s.\n",Data,szERR);
		fclose(fp);
	}
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
//	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
	*/
}

UINT ParsePackage_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	Package_ST *pPackage_ST = NULL;
	pPackage_ST = (Package_ST *) malloc (sizeof(Package_ST));
	memset(pPackage_ST,0,sizeof(Package_ST));

// 	CString ff1;
// 	ff1.Format("pExamCar_ST = %p, pPackage_ST = %p",pExamCar_ST,pPackage_ST);
// 	pCCBKDigitalMapDlg->WriteLog(ff1);


//	BOOL bExamCarState[MAX_ExamCar];  本来是在这里的现在要把它放到类里面去
	CString OO;
	CString ert;
	int i = 0;
	for(i = 0;i < MAX_ExamCar;i++)
	{
		pCCBKDigitalMapDlg->bExamCarState[i] = FALSE;
	}
	int iCount_Package = 0;
	int iTmpj = 0;
	int iTmpc = 0;
	int iTmpDefault = 0;
	CString R1;
//	while (1)  
	while (pCCBKDigitalMapDlg->iContrl)
	{
		while (!pCCBKDigitalMapDlg->pSocket.pPackage_ST.empty() && pCCBKDigitalMapDlg->iContrl)
		{
		//	pCCBKDigitalMapDlg->WriteLog("解包 进来  0");
			memset(pPackage_ST,0,sizeof(Package_ST));
			pCCBKDigitalMapDlg->pMutexForPG.Lock();
		//	while(pCCBKDigitalMapDlg->pSocket.dequeOK == FALSE)
		//		;
		//	Sleep(10);
		//	pCCBKDigitalMapDlg->WriteLog("解包 进来");
		//	pCCBKDigitalMapDlg->pSocket.dequeOK = FALSE;
			
	//		Package_ST *pPackage_ST11 = NULL;
	//		pPackage_ST11 = (Package_ST *)pCCBKDigitalMapDlg->pSocket.pPackage_ST.front();  //取出先存进去的
			memcpy(pPackage_ST,pCCBKDigitalMapDlg->pSocket.pPackage_ST.front(),sizeof(Package_ST));
			Sleep(2);
/*
			if (pPackage_ST->pExamCar_ST.ExamCarNumber>38 || pPackage_ST->pExamCar_ST.ExamCarNumber <1)
			{
				Sleep(5);
				Sleep(5);
			}
*/
			pCCBKDigitalMapDlg->pSocket.pPackage_ST.pop_front();  //删除第一个元素
			Sleep(2);
		//	pCCBKDigitalMapDlg->pSocket.dequeOK = TRUE;
			iCount_Package ++;
			pCCBKDigitalMapDlg->pMutexForPG.Unlock();
		//	pCCBKDigitalMapDlg->WriteLog("解包 出去");
		//	CString CarNumber;
			switch(pPackage_ST->pPackageHead_ST.PackageType)
			{
			case 1: 
				//为发回考车的信息 包括考车号和GPS坐标
				// 提取GPS的点，同时将点发送到对应的考车，更新此时的GPS坐标 同时将该点写入对应的文件
		//		memcpy(pExamCar_ST,pPackage_ST+8,sizeof(ExamCar_ST));
		//		pExamCar_ST = (ExamCar_ST *)(pPackage_ST + sizeof(PackageHead_ST));
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].ExamCarNumber = pPackage_ST->pExamCar_ST.ExamCarNumber;
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].x_Gps = pPackage_ST->pExamCar_ST.x_Gps;
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].y_Gps = pPackage_ST->pExamCar_ST.y_Gps;
				//////////////////////////////////////////////////////////////////////////
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].zt = pPackage_ST->pExamCar_ST.zt;
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].ErrorID = pPackage_ST->pExamCar_ST.ErrorID;
				//////////////////////////////////////////////////////////////////////////
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].ThreadNumber = pPackage_ST->pExamCar_ST.ThreadNumber;
				//计算此时GPS位于屏幕的坐标		
//zzz

				iTmpj++;

				// ExamCarState
				if (pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ThreadNumber] == FALSE)
				{
					pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ThreadNumber].ExamCarState = 1;//考车在线
			//		pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ThreadNumber].ShowWindow(SW_SHOW);
					// 开启定时器 用于GPS考车点的移动
					//		pCCBKDigitalMapDlg->SetTimer(GPS_CarTimer,10,NULL); 把这个改成唤醒线程
				//	ResumeThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ExamCarNumber]);
					while(ResumeThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ThreadNumber]));
					pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ThreadNumber] = TRUE;
				}
			//	pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ExamCarState = 1;//考车在线
			//	pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ShowWindow(SW_SHOW);
				// 开启定时器 用于GPS考车点的移动
		//		pCCBKDigitalMapDlg->SetTimer(GPS_CarTimer,10,NULL); 把这个改成唤醒线程
			//	ResumeThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ExamCarNumber]);
				break;
			case 10:
				//代表关闭考车
				iTmpc++;
				if (pPackage_ST->pExamCar_ST.ExamCarNumber > 0 && pPackage_ST->pExamCar_ST.ExamCarNumber < MAX_ExamCar)
				{
					pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ExamCarState = 0;//考车离线
					pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ShowWindow(SW_HIDE);
				//	SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ExamCarNumber]);//挂起线程
					while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ThreadNumber]));
					pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ExamCarNumber] = FALSE;
					CString ff;
					ff.Format("%d 考车关闭",pPackage_ST->pExamCar_ST.ExamCarNumber);
					pCCBKDigitalMapDlg->WriteLog(ff);
				}
				break;
			default:
				iTmpDefault++;
				break;
			}
		}
		Sleep(100);
	}
	Sleep(5000);

//	free(pPackage_ST);  //为什么这个释放不了

	int xxx = 0;
 	if (!pCCBKDigitalMapDlg->pSocket.pPackage_ST.empty())
 	{
// 		memcpy(pPackage_ST,pCCBKDigitalMapDlg->pSocket.pPackage_ST.front(),sizeof(Package_ST));
// 		Sleep(5);
// 		R1.Format("解包数：%d  %d  %d  %d   包数为：%d  car  %d  %d %lf  %lf",iCount_Package,iTmpj,iTmpc,iTmpDefault,\
// 			(int) pCCBKDigitalMapDlg->pSocket.pPackage_ST.size(),pPackage_ST->pPackageHead_ST.PackageType,pPackage_ST->pExamCar_ST.ExamCarNumber,\
// 			pPackage_ST->pExamCar_ST.x_Gps,pPackage_ST->pExamCar_ST.y_Gps);
// 			pCCBKDigitalMapDlg->WriteLog(R1);
		xxx = 100;
 	}
	R1.Format("解包数：%d  %d  %d  %d   包数为：%d   %d",iCount_Package,iTmpj,iTmpc,iTmpDefault,(int) pCCBKDigitalMapDlg->pSocket.pPackage_ST.size(),xxx);
	pCCBKDigitalMapDlg->WriteLog(R1);
	
	pCCBKDigitalMapDlg->WriteLog("主 ParsePackage_thd");
	return 0;
}

/*
函数说明：是用来统一在线考车数的，
如果有当前的在线考车数有变化时，会触发一个消息
  现在不用了 ，因为一开始就读取了在线考车数  不会变化
*/

UINT SendCurCarMsg_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{
		Sleep(2000);		
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSG_CAR,pCCBKDigitalMapDlg->pSocket.TotalCar,0);
		/*CurCar = pCCBKDigitalMapDlg->pSocket.TotalCar;*/
	}
	pCCBKDigitalMapDlg->WriteLog("主 SendCurCarMsg_thd");
	Sleep(2000);
	return 0;
}





//专门用来唤醒考车线程
UINT WakeUpExamCar_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
 	while (pCCBKDigitalMapDlg->iContrl)
 	{   // HadWakeUp;//表示线程已经唤醒了  0 表示考车线程已经被唤醒了 1 代表线程需要唤醒
		if (0 == pCCBKDigitalMapDlg->iNowZoomRunning && /*pCCBKDigitalMapDlg->iCountFlushMap == 0 &&*/ pCCBKDigitalMapDlg->HadWakeUp == 1)
 		{
 			Sleep(150);
 
 			pCCBKDigitalMapDlg->WakeUp_Thread();
 			pCCBKDigitalMapDlg->HadWakeUp = 0;
 		}
 		else
 		{
 			Sleep(100);
 		}
 	}
	pCCBKDigitalMapDlg->WriteLog("主 WakeUpExamCar_thd");
	return 0;
}
 

/*
 实时定位在线考车的GPS点 如果对应的考车不在线就挂起 如果对应的考车在线就唤醒
*/
UINT ExamCarGPSLocate_thd(LPVOID p)
{
//	memset(pExamCarInfoToThread_ST,0,sizeof(ExamCarInfoToThread_ST));
	 //  (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = ((ExamCarInfoToThread_ST)p)->pMapDlg;
//	pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	int iExamCarNumber = ((ExamCarInfoToThread_ST)p)->iExamCarNumber;
	int tmpX = 0;
	int tmpY = 0;
	int preX = 0;
	int preY = 0;
	Sleep(1000);
	//在这里已经有对应的考车号和主对话框指针
	CString car;
	CString szContent;
	car.Format("%d",iExamCarNumber);

	

	while (pCCBKDigitalMapDlg->iContrl)
	{		
		
 //		if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)  //说明当前没有被跟踪的考车
 //		{
 		//	if (pCCBKDigitalMapDlg->m_Button[iExamCarNumber].ExamCarState == 1) //说明考车在线
 		//	{
			if (pCCBKDigitalMapDlg->iNowZoomRunning == 1)
			{
				
				Sleep(50);
				continue;
			}
			else if (pCCBKDigitalMapDlg->iNowZoomRunning == 0 && pCCBKDigitalMapDlg->isRunningUpdate == 0)//正在放大或缩小)  和考车追踪时桌面没有刷新
			{
				int iResult = 0;
				pCCBKDigitalMapDlg->Mutex_Self[iExamCarNumber].Lock();// 用于自身用的，主要用于考车离线
				iResult = pCCBKDigitalMapDlg->pMutex.Lock();
				//zzz
 				pCCBKDigitalMapDlg->m_Button[iExamCarNumber].Cur_xPointRelyOnParent = (pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
 				pCCBKDigitalMapDlg->m_Button[iExamCarNumber].Cur_yPointRelyOnParent = pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel);
 				pCCBKDigitalMapDlg->iCountFlushExamCar++;//用来统计考车刷新的次数 当为0的时候才可以刷新地图
				if (pCCBKDigitalMapDlg->m_Button[iExamCarNumber].isCircle == 1)//说明在中心
 				{
 					pCCBKDigitalMapDlg->m_Button[iExamCarNumber].MoveWindow(pCCBKDigitalMapDlg->CurrentScrWidth / 2, pCCBKDigitalMapDlg->CurrentScrHeight / 2,20,20);
				}
 				else	
				{
					if (pCCBKDigitalMapDlg->iNowZoomRunning == 1)
					{
						Sleep(50);
						continue;
					}
					
					/*/////////////////////////////    这地方本来是有的         //////////////////////////////
 					while (pCCBKDigitalMapDlg->isRunningUpdate);
					//////////////////////////////                  /////////////////////////////*/
					
 					pCCBKDigitalMapDlg->isRunningUpdate = 1;
 			//		Sleep(100);
					


					tmpX = (pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
					tmpY = pCCBKDigitalMapDlg->CurrentScrHeight - (pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel;
	
					if (pCCBKDigitalMapDlg->m_Button[iExamCarNumber].ExamCarState != 0)
					{
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Xpoint = int((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel);
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Ypoint = int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel) - 10);

						//zzzz fuzi1
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_ZT =pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].zt;
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_EID =pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].ErrorID;
						//////////////////////////////////////////////////////////////////////////
 						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].MoveWindow(pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Xpoint,pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Ypoint,20,20);
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].Invalidate(FALSE);
					}

						preX = tmpX;
						preY = tmpY;
			//		}
					
 			//		Sleep(100);
 					pCCBKDigitalMapDlg->isRunningUpdate = 0; 
 				}
				
				iResult = pCCBKDigitalMapDlg->pMutex.Unlock();
				pCCBKDigitalMapDlg->Mutex_Self[iExamCarNumber].Unlock();// 用于自身用的，主要用于考车离线
				
				szContent.Format("考车下线 x = %lf,y = %lf",pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps,pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps);
				pCCBKDigitalMapDlg->WriteLog(szContent,car+".clog");
				Sleep(50);
			}
				
 		//	}
 //		}
 			
 	Sleep(100);		
	}
	Sleep(5000);
	pCCBKDigitalMapDlg->WriteLog("主 ExamCarGPSLocate_thd");
	return 0;
}


UINT HideExamCar_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;

	int i = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{
	//	for (i = 1;i < pCCBKDigitalMapDlg->pSocket.TotalCar;i++)
		for (i = 1;i < MAX_ExamCar;i++)
		{
			if (pCCBKDigitalMapDlg->m_Button[i].ExamCarState == 0)//考车离线
			{
				pCCBKDigitalMapDlg->m_Button[i].MoveWindow(-20,-20,20,20);
				pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
			//	while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[i]));  //这个移到了SCOKET 考车离开程序里
				Sleep(2000);  //这里延时2s的主要原因是用来从在线到下线有些考车信息的包还没有全部处理掉，这里起到等待的作用
				pCCBKDigitalMapDlg->bExamCarState[i] = FALSE;
			}			
		}
		Sleep(2000);
	}
	Sleep(4000);

	pCCBKDigitalMapDlg->WriteLog("主 HideExamCar_thd");
	return 0;
}


/*//////////////////////////////
有考车被跟踪的时候别的考车就隐藏
//////////////////////////////*/
UINT HideOtherCar_thd(LPVOID p)
{
	
	Sleep(10000);
	int tmpCurTrackCar = -1;//用来临时存放被跟踪的考车
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
	int i = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{
		if(tmpCurTrackCar != pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar) //说明当前被追踪的考车发生变化
		{
			if(pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1) //说明没有考车被跟踪
			{
				
				tmpCurTrackCar = -1;
				//让当前在线的考车正常显示出来
				for(i = 0;i < MAX_ExamCar;i++)
				{  
					pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
				}
			}
			else
			{
				tmpCurTrackCar = pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar;
				//只显示被追踪的考车
				for(i = 0;i < MAX_ExamCar;i++)
				{
					pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
				}
				pCCBKDigitalMapDlg->m_Button[tmpCurTrackCar].ShowWindow(SW_HIDE);
			}
		}
		else
		{
			Sleep(500);
		}


		
	}
	pCCBKDigitalMapDlg->WriteLog("主 HideOtherCar_thd");
	Sleep(2000);
	return 0;
}


/*//////////////////////////////
用于定期刷新在线考车和地图
//////////////////////////////*/
UINT UpdateMap_thd(LPVOID p)
{
	int tmpCurTrackCar = -1;//用来临时存放被跟踪的考车
	Sleep(10000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
	int i = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{	
		
		pCCBKDigitalMapDlg->HasPlayInvalite();
		Sleep(500);
	}
	pCCBKDigitalMapDlg->WriteLog("主 UpdateMap_thd");
	Sleep(2000);
	return 0;
}


/*
用于监听在线考车数有没有下线
ListenExamCar_thd(LPVOID p);//专门用来监听在线考车数  主要是用于考车离线用的 考车自动上线已经在socket那里处理了
*/
UINT ListenExamCar_thd(LPVOID p)
{
	int tmpTotalCar = 0;//用来临时存放被跟踪的考车
	Sleep(10000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;

	while (pCCBKDigitalMapDlg->iContrl)
	{	

		pCCBKDigitalMapDlg->ReadOnlineCarFromDB1();

		
		Sleep(500);
	}

	pCCBKDigitalMapDlg->WriteLog("主 ListenExamCar_thd");
	Sleep(2000);
	return 0;
}


/*
用于监听在线考车数有没有下线
ListenExamCarOnline_thd(LPVOID p);//专门用来监听在线考车数  主要是用于考车上线用的 考车自动上线已经在socket那里处理了
*/
UINT ListenExamCarOnline_thd(LPVOID p)
{
	int tmpTotalCar = 0;//用来临时存放被跟踪的考车
	Sleep(10000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;

	while (pCCBKDigitalMapDlg->iContrl)
	{	
		pCCBKDigitalMapDlg->ReadOnlineCarFromDB2();
		
		Sleep(1000);
	}

	pCCBKDigitalMapDlg->WriteLog("主 ListenExamCarOnline_thd");
	Sleep(2000);
	return 0;
}


UINT ButtonUp_thd(LPVOID p)
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	Sleep(100);
	pCCBKDigitalMapDlg->iStateLBD = 0;
	return 0;
}


/*
专门用来设置考车下线的时候隐藏考车
*/
UINT ExamCarHideDown_thd(LPVOID p)
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	CButtonz *pButton = (CButtonz *)p;
	Sleep(2000);

	pButton->MoveWindow(-20,-20,20,20);
//	pButton->examCarNumber = 0;
	
	pButton->CurCar_Xpoint = 0;
	pButton->CurCar_Ypoint = 0;
	pButton->CurCar_ZT = 0;
	pButton->CurCar_EID=1000;
	
	pButton->ExamCarState = 0;
	
	CString tmpOnLineCar;
	tmpOnLineCar.Format("当前考车数为：%d",pCCBKDigitalMapDlg->pSocket.TotalCar);
	pCCBKDigitalMapDlg->m_TotalCar.SetWindowText(tmpOnLineCar);  

//	while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ThreadNumber]));
//	pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ExamCarNumber] = FALSE;


	pCCBKDigitalMapDlg->WriteLog("主 ExamCarHideDown_thd");
	return 0;
}

void CCBKDigitalMapDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	int i = 0;
	
	switch(nIDEvent)
	{
	case GPS_CarTimer:
		{
			if (ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)
			{
			
				for (i = 1;i < MAX_ExamCar;i++)
				{
					if (ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)
					{
						if (m_Button[i].ExamCarState == 1) //说明考车在线
						{//zzz
							m_Button[i].Cur_xPointRelyOnParent = (on_ExamCar[i].x_Gps - MAPLeft) / x_pixel;
							m_Button[i].Cur_yPointRelyOnParent = CurrentScrHeight - ((on_ExamCar[i].y_Gps - MAPBotom) / y_pixel);
							if (m_Button[i].isCircle == 1)//说明在中心
							{
								m_Button[i].MoveWindow(this->CurrentScrWidth / 2, this->CurrentScrHeight / 2,20,20);
							}
							else
							{
								while (isRunningUpdate);
								isRunningUpdate = 1;
							//	Sleep(50);
								m_Button[i].MoveWindow(int((on_ExamCar[i].x_Gps - MAPLeft) / x_pixel),int(CurrentScrHeight - ((on_ExamCar[i].y_Gps - MAPBotom) / y_pixel)),20,20);
							//	Sleep(50);
								isRunningUpdate = 0;
							}
						}
					}
					else
					{
						break;
					}
				}
					
			}
		//	m_Button[12].MoveWindow((on_ExamCar[12].x_Gps - MAPLeft) / x_pixel,CurrentScrHeight - ((on_ExamCar[12].y_Gps - MAPBotom) / y_pixel),20,20);
		}
		break;
	case GPS_Dlg:
		KillTimer(GPS_Dlg);
		CreateDlgha();//创建按钮也就是考车的消息框
		pSocket.iC_Dlg_F = 1;
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}



LRESULT CCBKDigitalMapDlg::OnBtnMsgz(WPARAM wParam, LPARAM lParam)
{
	CString str;
	CString Card;
	CString temp;
	UINT ExamCarNumber = wParam;
	temp.Format("%d",wParam);
	Card.Format("考车");
	Card += temp;
	dlgha[lParam].ReadExamInfoFromDB(NULL,Card,NULL);
	str.Format("考车%d",wParam);
/*	str += dlgha.szExamineeName;*/
	dlgha[lParam].CSCAR = str;


	
/*
	if (wParam > 0 && wParam < MAX_ExamCar)
	{

	
		//接下来要判断4个块的区域 不同的区域加载不同的背景图片
		CRect ClientRect; //客户端区域
		GetClientRect(ClientRect);
		if (m_Button[lParam].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
		{
			//对话框的箭头打向左下角
			
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent + 20,m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
		else if (m_Button[lParam].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
		{	//对话框的箭头打向左上角
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent + 20,m_Button[lParam].Cur_yPointRelyOnParent + 20 ,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
		else if (m_Button[lParam].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
		{	//对话框的箭头打向右下角
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent - SubDlgForInfoWidth,m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
		else if (m_Button[lParam].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
		{	//对话框的箭头打向右上角
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent - SubDlgForInfoWidth,m_Button[lParam].Cur_yPointRelyOnParent + 20,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
	}

*/
	
	return TRUE;
}

LRESULT CCBKDigitalMapDlg::OnBtnMsgzLeft(WPARAM wParam, LPARAM lParam)
{
	CString str;
	UINT ExamCarNumber = wParam;
	if (wParam > 0 && wParam < MAX_ExamCar)
	{
		str.Format("考车%d",wParam);
		dlgha[ExamCarNumber].CSCAR = str;
		dlgha[ExamCarNumber].MoveWindow(-500,-300,350,150);
		dlgha[ExamCarNumber].ShowWindow(SW_HIDE);
	}

	return TRUE;
}

HBRUSH CCBKDigitalMapDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOTALCAR)
	{
		pDC->SetTextColor(RGB(255,0,255));  //设置字体颜色
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_BOOT)
	{
		pDC->SetTextColor(RGB(255,0,255));  //设置字体颜色
	}
	if   (nCtlColor   ==   CTLCOLOR_STATIC)        //设置控件为透明
	{     
		pDC->SetBkMode(TRANSPARENT);     
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;

}




LRESULT CCBKDigitalMapDlg::OnUpdataCurCar(WPARAM wParam, LPARAM lParam)
{
	CString str;	
	str.Format("当前考车数为：%d",wParam);
	m_TotalCar.SetWindowText(str);
	CRect pCRect(10,10,240,50);
	InvalidateRect(pCRect,false); //强制更新区域内的界面 
//	Invalidate();//强制刷新这个对话框区域
	return TRUE;
}




//鼠标中间滚轮的消息
//参数说明：zDelta 如果是整数 代表 向前滚动 如果是负数 说明是负数 代表向后滚动  
// pt 代表当前鼠标的坐标值
BOOL CCBKDigitalMapDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	short iDelta = zDelta; //确定滚动方向
	x_CurScr = pt.x; // 当前鼠标在屏幕上的坐标
	y_CurScr = pt.y;
	
	CRect ClientRect; //客户端区域
	GetClientRect(ClientRect);





	if (iDelta > 0) //说明是要进行放大
	{
		int x = 0; //ClientRect.Width(); //屏幕的宽度  这一个用于放大的
		int y = 0;//ClientRect.Height();//屏幕的高度   这一个用于放大的
		DOUBLE x_CurLatitude = 0;//当前纬度
		DOUBLE y_CurLong = 0;// 当前经度
		if (ZoomLevel >= 24)
		{
			return TRUE;
		}
		x = (ClientRect.Width() / 1.2);
		y = (ClientRect.Height() / 1.2);
		// 1、要判断是否处于4个脚的地方还是处于4个边的地方 还是在中间 9个区域
		// 2、确定此时在屏幕中的巨型
		// 3、算出此时左上脚和右下角的经度纬度 同时算出其实在屏幕中1个像素所占的经度纬度 
		//    更新 MAPBotom MAPTop  MAPLeft  MAPRight  x_pixel  y_pixel
		// 4、对应到原来的画布位置
		// 5 、自定义一个消息 刷新屏幕
		if (x_CurScr <= (x / 2) && y_CurScr <= (y / 2)) //说明落在左上角的区域
		{  //  x_StartBit,y_StartBit,x_BitWidth,y_BitWidth
			// x_StartBit,y_StartBit,x_BitWidth,y_BitWidth
			// 还差一个问题  x_StartBit  y_StartBit 的处理  已解决

			MAPLeft = MAPLeft;
			MAPTop = MAPTop;
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			MAPRight = x_pixel * x + MAPLeft;  
			MAPBotom = MAPTop - y_pixel * y;
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度
			
		}
		else if (x_CurScr <= (x / 2) && ((y_CurScr + (y / 2)) >= ClientRect.bottom )) //说明落在左下角的区域
		{	
			DOUBLE X_GPS = (MAPRight - MAPLeft) * 0.1;  //当前屏幕GPS值要增大的  经度差值
			DOUBLE Y_GPS = (MAPTop - MAPBotom) * 0.1;  //当前屏幕GPS值要增大的  纬度差值
			MAPLeft = MAPLeft;
			MAPBotom = MAPBotom;
			MAPRight = x_pixel * x + MAPLeft;  
			MAPTop = MAPBotom + y_pixel * y;

			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度
		}
		else if (x_CurScr <= (x / 2) && ((y_CurScr + (y / 2)) < ClientRect.bottom ))  //说明是在左边的中间区域
		{
			DOUBLE X_GPS = (MAPRight - MAPLeft) * 0.1;  //当前屏幕GPS值要增大的  经度差值
			DOUBLE Y_GPS = (MAPTop - MAPBotom) * 0.1;  //当前屏幕GPS值要增大的  纬度差值
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			MAPLeft = MAPLeft;	
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );

			MAPTop = y_CurLong + y_pixel * (iY / 2);
			MAPBotom = y_CurLong - y_pixel * (iY / 2);
			MAPRight = MAPLeft + x_pixel * iX;

			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度

		}
		else if (((x_CurScr + (x / 2)) < ClientRect.right) && y_CurScr <= (y / 2)) //说明在上边的中间区域
		{
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			MAPTop = MAPTop;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );
			MAPLeft = x_CurLatitude - x_pixel * (iX / 2);
			MAPRight = x_CurLatitude + x_pixel * (iX / 2);
			MAPBotom = MAPTop - y_pixel * iY;
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度

		}
		else if (((x_CurScr + (x / 2)) >= ClientRect.right) && y_CurScr <= (y / 2))  //说明在右上角
		{
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			MAPRight = MAPRight;
			MAPTop = MAPTop;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );
			
			MAPBotom = MAPTop - y_pixel * iY;
			MAPLeft = MAPRight - x_pixel * iX;
			
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度

		}
		else if (((x_CurScr + (x / 2)) >= ClientRect.right) && ((y_CurScr + (y / 2)) < ClientRect.bottom )) //说明在右边的中间区域
		{
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			MAPRight = MAPRight;	
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );
			
			MAPTop = y_CurLong + y_pixel * (iY / 2);
			MAPBotom = y_CurLong - y_pixel * (iY / 2);
			MAPLeft = MAPRight - x_pixel * iX;
			
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度

		}
		else if (((x_CurScr + (x / 2)) >= ClientRect.right) && ((y_CurScr + (y / 2)) >= ClientRect.bottom ))//说明在右下脚
		{
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			MAPRight = MAPRight;
			MAPBotom = MAPBotom;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );
			
			MAPTop = MAPBotom + y_pixel * iY;
			MAPLeft = MAPRight - x_pixel * iX;
			
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度
		}
		else if (((x_CurScr + (x / 2)) < ClientRect.right) && ((y_CurScr + (y / 2)) >= ClientRect.bottom )) //说明在下边的中间区域
		{
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			MAPBotom = MAPBotom;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );
			MAPLeft = x_CurLatitude - x_pixel * (iX / 2);
			MAPRight = x_CurLatitude + x_pixel * (iX / 2);
			MAPTop = MAPBotom + y_pixel * iY;
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度

		}
		else //在中间区域
		{
			DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
			DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
			iY = (ClientRect.Height() / 1.2 );
			MAPLeft = x_CurLatitude - x_pixel * (iX / 2);
			MAPRight = x_CurLatitude + x_pixel * (iX / 2);
			MAPTop = y_CurLong + y_pixel * (iY / 2);
			MAPBotom = y_CurLong - y_pixel * (iY / 2);
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // 获得电脑一个x坐标像素代表多少经度
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // 获得电脑一个y坐标像素代表多少纬度

		}

		switch(ZoomLevel)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
			{
				if (ZoomLevel == 0)
				{
					ZoomLevel = ZoomLevel1;
				}
				ZoomLevel = ZoomLevel + 1;

				


				PixState = 1;  //已经放大
				Suspend_Thread();
				iNowZoomRunning = 1;//正在放大或缩小
				iCountFlushMap ++; //地图的刷新次数加一
				while (iCountFlushExamCar)
				{
					Sleep(200);
					iCountFlushExamCar = 0;
				}
			//	Sleep(100);
			//	Invalidate(FALSE);
				HasPlayInvalite();
//				while (iNowZoomRunning);
	//			WakeUp_Thread();  把这个唤醒线程的工作在OnPaint里面的尾部实现
				
			}

			break;
		case 24:  //不能再进行放大了 
			break;
		default:
			break;
		}
	}
	else  //说明要进行缩小
	{
		// 缩小的思路 
		// 先算出该点的经度 纬度
		// 1、先将全屏的图片缩小为上一次放大之前的图片大小  即把当前的屏幕 除以 1.2 同时重新计算 x_pixel 和y_pixel
		// 2、根据图片的左边经纬度往图片的左边加上 屏幕的宽度 减去 放大前的宽度 再除以2 则是一边要缩小后显示的宽度 同时算出经度
		// 3、如果此时的经度小于等于原始的经度，说明已经到了这张图的原始图了就原图显示否则不是原始图，
		//  如果有就说明 已经缩小到原图了，没有就以该点为中心进行缩放
		int x = 0; //ClientRect.Width(); //屏幕的宽度  这一个用于缩小
		int y = 0;//ClientRect.Height();//屏幕的高度   这一个用于缩小的
		DOUBLE iX = 0; //用于放大之后看下小数点是不是大于0
		DOUBLE iY = 0; //用于放大之后看下小数点是不是大于0
		if (ZoomLevel <= 0)
		{
			return TRUE;
		}

		////////    下面的这些变量是用于添加在原图的基础上再缩小的 多出来的一些临时变量  /////////
		
		DOUBLE itmpMAPLeft = 0;
		DOUBLE itmpMAPRight = 0;
		DOUBLE itmpMAPBotom = 0;
		DOUBLE itmpMAPTop = 0;

		DOUBLE itmpx_pixel = 0;
		DOUBLE itmpy_pixel = 0;
		
		itmpMAPLeft = MAPLeft;
		itmpMAPRight = MAPRight;
		itmpMAPBotom = MAPBotom;
		itmpMAPTop = MAPTop;

		itmpx_pixel = x_pixel;
		itmpy_pixel = y_pixel;


		/////////////////
		iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 代表这个图片的宽度一半
		iY = (ClientRect.Height() / 1.2 );
		x = iX;
		y = iY;
		int x_Offset = 0;
		int y_Offset = 0;
		x_Offset = (ClientRect.Width() - (ClientRect.Width() / 1.2 )) / 2;  //纬度的偏移量
		y_Offset = (ClientRect.Height() - (ClientRect.Height() / 1.2)) /2;  //经度的偏移量

		// 算出当前的经度 纬度
		DOUBLE x_CurLatitude = 0;//当前纬度
		DOUBLE y_CurLong = 0;// 当前经度
		x_CurLatitude = MAPLeft + x_pixel * pt.x;
		y_CurLong = MAPTop - y_pixel * pt.y;

		x_pixel = (DOUBLE)((MAPRight-MAPLeft) / iX); // 获得电脑一个x坐标像素代表多少经度 重新获取像素
		y_pixel = (DOUBLE)((MAPTop-MAPBotom) / iY); // 获得电脑一个y坐标像素代表多少纬度

		MAPRight = x_CurLatitude + x_pixel *  (iX/2);
		MAPBotom = y_CurLong - y_pixel * (iY/2);
		MAPLeft = x_CurLatitude - x_pixel * (iX/2);
		MAPTop = y_CurLong + y_pixel * (iY/2);
		DOUBLE MAPRight1 = MAPRight + x_pixel * ((ClientRect.Width() - iX) / 2);
		DOUBLE MAPBotom1 = MAPBotom - y_pixel * ((ClientRect.Width() - iY) /2) ;
		DOUBLE MAPLeft1 = MAPLeft - x_pixel * ((ClientRect.Width() - iX) / 2);
		DOUBLE MAPTop1 = MAPTop + y_pixel * ((ClientRect.Width() - iY) /2) ;


		if (MAPLeft1 > OriginalMAPLeft || MAPTop1 < OriginalMAPTop || MAPRight1 < OriginalMAPRight || MAPBotom1 > OriginalMAPBotom)  //还没有到原图的大小
		{
			if (MAPLeft < OriginalMAPLeft)  //说明在最左边
			{
				MAPLeft = OriginalMAPLeft;
				
				if (MAPTop > OriginalMAPTop)  //说明在左上角
				{
					
					MAPTop = OriginalMAPTop;
					MAPBotom = MAPTop - y_pixel * iY - y_pixel * y_Offset * 2;
				}
				else if (MAPBotom < OriginalMAPBotom)  //说明在左下角
				{
					MAPBotom = OriginalMAPBotom;
					MAPTop = MAPBotom + y_pixel * iY + y_Offset * y_pixel * 2;
				}
				else
				{
					//说明在左边中间
					MAPBotom = y_CurLong - y_pixel * (iY/2) - y_pixel * y_Offset;
					MAPTop = y_CurLong + y_pixel * (iY/2) + y_pixel * y_Offset;
					

					if (MAPBotom < OriginalMAPBotom && MAPTop > OriginalMAPTop)
					{
						MAPLeft = itmpMAPLeft;
						MAPRight = itmpMAPRight;
						MAPBotom = itmpMAPBotom;
						MAPTop = itmpMAPTop;
						
						x_pixel = itmpx_pixel;
						y_pixel = itmpy_pixel;
						if(ZoomLevel != 0)
						{
							ZoomLevel1 = ZoomLevel;
						}
						ZoomLevel = 0;
						
						return 1;
					}
					else
					{
						if (MAPBotom < OriginalMAPBotom)
						{
							DOUBLE itmp = OriginalMAPBotom - MAPBotom + MAPTop;
							if (itmp > OriginalMAPTop) //也不能再缩小了
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPBotom = OriginalMAPBotom;
								MAPTop = itmp;
							}
						}
						else
						{
							if (MAPTop > OriginalMAPTop)
							{
								DOUBLE itmp = MAPBotom - (MAPTop - OriginalMAPTop);
								if (itmp < OriginalMAPBotom) //也不能再缩小了
								{
									MAPLeft = itmpMAPLeft;
									MAPRight = itmpMAPRight;
									MAPBotom = itmpMAPBotom;
									MAPTop = itmpMAPTop;
									
									x_pixel = itmpx_pixel;
									y_pixel = itmpy_pixel;
									if(ZoomLevel != 0)
									{
										ZoomLevel1 = ZoomLevel;
									}
									ZoomLevel = 0;
									
									return 1;
								}
								else
								{
									MAPTop = OriginalMAPTop;
									MAPBotom = itmp;
								}
							}
							
						}
					}


				}
				MAPRight = MAPLeft + x_pixel * iX + x_pixel * x_Offset * 2;

				if ((MAPRight > OriginalMAPRight) || (MAPBotom < OriginalMAPBotom) || (MAPTop > OriginalMAPTop))
				{


					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					
					return 1;
				}
			}
			else if (MAPRight > OriginalMAPRight) //说明在最右边
			{
				MAPRight = OriginalMAPRight;

				/*
				if ((MAPRight - x_pixel * iX - x_Offset * x_pixel * 2) < OriginalMAPLeft ) //说明不能再缩小了，退出
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					return 1;
				}
				*/
					
				
				if (MAPTop > OriginalMAPTop)  //说明在右上角
				{
					MAPTop = OriginalMAPTop;
					MAPBotom = MAPTop - y_pixel * iY - y_pixel * y_Offset * 2;
				}
				else if (MAPBotom < OriginalMAPBotom)  //说明在右下角
				{
					MAPBotom = OriginalMAPBotom;
					MAPTop = MAPBotom + y_pixel * iY + y_Offset * y_pixel * 2;
				}
				else
				{
					//说明在右边中间
					MAPBotom = y_CurLong - y_pixel * (iY/2) - y_pixel * y_Offset;
					MAPTop = y_CurLong + y_pixel * (iY/2) + y_pixel * y_Offset;

					if (MAPBotom < OriginalMAPBotom && MAPTop > OriginalMAPTop)
					{
						MAPLeft = itmpMAPLeft;
						MAPRight = itmpMAPRight;
						MAPBotom = itmpMAPBotom;
						MAPTop = itmpMAPTop;
						
						x_pixel = itmpx_pixel;
						y_pixel = itmpy_pixel;
						if(ZoomLevel != 0)
						{
							ZoomLevel1 = ZoomLevel;
						}
						ZoomLevel = 0;
						
						return 1;
					}
					else
					{
						if (MAPBotom < OriginalMAPBotom)
						{
							DOUBLE itmp = OriginalMAPBotom - MAPBotom + MAPTop;
							if (itmp > OriginalMAPTop) //也不能再缩小了
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPBotom = OriginalMAPBotom;
								MAPTop = itmp;
							}
						}
						else
						{
							if (MAPTop > OriginalMAPTop)
							{
								DOUBLE itmp = MAPBotom - (MAPTop - OriginalMAPTop);
								if (itmp < OriginalMAPBotom) //也不能再缩小了
								{
									MAPLeft = itmpMAPLeft;
									MAPRight = itmpMAPRight;
									MAPBotom = itmpMAPBotom;
									MAPTop = itmpMAPTop;
									
									x_pixel = itmpx_pixel;
									y_pixel = itmpy_pixel;
									if(ZoomLevel != 0)
									{
										ZoomLevel1 = ZoomLevel;
									}
									ZoomLevel = 0;
									
									return 1;
								}
								else
								{
									MAPTop = OriginalMAPTop;
									MAPBotom = itmp;
								}
							}
							
						}
					}
				}
				MAPLeft = MAPRight - x_pixel * iX - x_Offset * x_pixel * 2;	

				if ((MAPLeft < OriginalMAPLeft) || (MAPBotom < OriginalMAPBotom) || (MAPTop > OriginalMAPTop))
				{
				

					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					
					return 1;
				}
			}
			else if (MAPTop > OriginalMAPTop) //说明在上边的中间 因为左上 和 右上已经排除了
			{
				MAPTop = OriginalMAPTop;
				
				MAPBotom = MAPTop - y_pixel * iY - y_pixel * y_Offset * 2;
				MAPLeft = x_CurLatitude - x_pixel * (iX/2) - x_pixel * x_Offset;
				MAPRight = x_CurLatitude + x_pixel *  (iX/2) + x_pixel * x_Offset;

				if (MAPBotom < OriginalMAPBotom)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}

				if (MAPLeft < OriginalMAPLeft && MAPRight > OriginalMAPRight) //没什么好说的，不能再缩小了
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
					
				}
				else
				{
					if (MAPLeft < OriginalMAPLeft)
					{
						DOUBLE itmp = OriginalMAPLeft - MAPLeft + MAPRight;
						if (itmp > OriginalMAPRight) //也不能再缩小了
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;

							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPLeft = OriginalMAPLeft;
							MAPRight = itmp;
						}
					}
					else
					{
						if (MAPRight > OriginalMAPRight)
						{
							DOUBLE itmp = MAPLeft - (MAPRight - OriginalMAPRight);
							if (itmp < OriginalMAPLeft) //也不能再缩小了
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPRight = OriginalMAPRight;
								MAPLeft = itmp;
							}
						}
						
					}
				}

				
			}
			else if (MAPBotom < OriginalMAPBotom)
			{
				MAPBotom = OriginalMAPBotom;			
				MAPTop = MAPBotom + y_pixel * iY + y_Offset * y_pixel * 2;
				MAPLeft = x_CurLatitude - x_pixel * (iX/2) - x_pixel * x_Offset;
				MAPRight = x_CurLatitude + x_pixel *  (iX/2) + x_pixel * x_Offset;

				if (MAPTop > OriginalMAPTop)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}


				if (MAPLeft < OriginalMAPLeft && MAPRight > OriginalMAPRight)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}
				else
				{
					if (MAPLeft < OriginalMAPLeft)
					{
						DOUBLE itmp = OriginalMAPLeft - MAPLeft + MAPRight;
						if (itmp > OriginalMAPRight) //也不能再缩小了
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;
							
							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPLeft = OriginalMAPLeft;
							MAPRight = itmp;
						}
					}
					else
					{
						if (MAPRight > OriginalMAPRight)
						{
							DOUBLE itmp = MAPLeft - (MAPRight - OriginalMAPRight);
							if (itmp < OriginalMAPLeft) //也不能再缩小了
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPRight = OriginalMAPRight;
								MAPLeft = itmp;
							}
						}
						
					}
				}
			}
			else
			{
				MAPLeft = x_CurLatitude - x_pixel * (iX/2) - x_pixel * x_Offset;
				MAPRight = x_CurLatitude + x_pixel *  (iX/2) + x_pixel * x_Offset;
				MAPBotom = y_CurLong - y_pixel * (iY/2) - y_pixel * y_Offset;
				MAPTop = y_CurLong + y_pixel * (iY/2) + y_pixel * y_Offset;
					
				if (MAPLeft < OriginalMAPLeft && MAPRight > OriginalMAPRight)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}
				else
				{
					if (MAPLeft < OriginalMAPLeft)
					{
						DOUBLE itmp = OriginalMAPLeft - MAPLeft + MAPRight;
						if (itmp > OriginalMAPRight) //也不能再缩小了
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;
							
							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPLeft = OriginalMAPLeft;
							MAPRight = itmp;
						}
					}
					else
					{
						if (MAPRight > OriginalMAPRight)
						{
							DOUBLE itmp = MAPLeft - (MAPRight - OriginalMAPRight);
							if (itmp < OriginalMAPLeft) //也不能再缩小了
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPRight = OriginalMAPRight;
								MAPLeft = itmp;
							}
						}
						
					}
				}



				if (MAPBotom < OriginalMAPBotom && MAPTop > OriginalMAPTop)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}
				else
				{
					if (MAPBotom < OriginalMAPBotom)
					{
						DOUBLE itmp = OriginalMAPBotom - MAPBotom + MAPTop;
						if (itmp > OriginalMAPTop) //也不能再缩小了
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;
							
							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPBotom = OriginalMAPBotom;
							MAPTop = itmp;
						}
					}
					else
					{
						if (MAPTop > OriginalMAPTop)
						{
							DOUBLE itmp = MAPBotom - (MAPTop - OriginalMAPTop);
							if (itmp < OriginalMAPBotom) //也不能再缩小了
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPTop = OriginalMAPTop;
								MAPBotom = itmp;
							}
						}
						
					}
				}


/*				if (MAPLeft <= OriginalMAPLeft || MAPTop >= OriginalMAPTop || MAPRight >= OriginalMAPRight || MAPBotom <= OriginalMAPBotom)
				{

					//////////////////////////////////////////////////////
					//////////////////////////////////////////////////////

					WriteLog("bbbb","bbbb");
					//说明缩小后回到原图大小
					x_StartBit = 0;
					y_StartBit = 0;
					MAPBotom = OriginalMAPBotom;//备份原始GPS值 
					MAPLeft = OriginalMAPLeft;//备份原始GPS值 
					MAPTop = OriginalMAPTop;//备份原始GPS值 
					MAPRight = OriginalMAPRight;//备份原始GPS值 
					x_pixel = x_Originalpixel;// 原始的x
					y_pixel = y_Originalpixel;// 原始的y
					y_BitWidth = y_OriginalBit; //获取图片的高度  保存原来图片的大小
					x_BitWidth = x_OriginalBit; //获取图片的宽度  保存原来图片的大小
					PixState = 0;//回到原图
					ZoomLevel = 0;
// 					CRect ClientRect; //客户端区域
// 					GetClientRect(ClientRect);
// 					InvalidateRect(&ClientRect,FALSE);
					Suspend_Thread();
					iNowZoomRunning = 1;//正在放大或缩小
					iCountFlushMap ++; //地图的刷新次数加一
					while (iCountFlushExamCar)
					{
						Sleep(200);
						iCountFlushExamCar = 0;
					}
					Sleep(100);
					Invalidate(FALSE);
			//		while (iNowZoomRunning);
			//		WakeUp_Thread();
					
				//	Sleep(50);
				//	MessageBox("123");
				//	Sleep(100);
					iNowZoomRunning = 0;//没有放大或缩小
					return CDialog::OnMouseWheel(nFlags, zDelta, pt);
					
				}
*/
			}

			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);

		}
		else//说明已经到了原图的大小了
		{
			return 1;
			//说明已经缩小到最小了
			x_StartBit = 0;
			y_StartBit = 0;
			MAPBotom = OriginalMAPBotom;//备份原始GPS值 
			MAPLeft = OriginalMAPLeft;//备份原始GPS值 
			MAPTop = OriginalMAPTop;//备份原始GPS值 
			MAPRight = OriginalMAPRight;//备份原始GPS值 
			x_pixel = x_Originalpixel;// 原始的x
			y_pixel = y_Originalpixel;// 原始的y
			y_BitWidth = y_OriginalBit; //获取图片的高度  保存原来图片的大小
			x_BitWidth = x_OriginalBit; //获取图片的宽度  保存原来图片的大小
			PixState = 0;//回到原图
			WriteLog("在缩小里面回到了最小","bbbb.clog");
		}

		switch(ZoomLevel)
		{
		case 0:  //不能再进行缩小了
			break;
		
			/*
			//强制给它回到原图
			{
				x_StartBit = 0;
				y_StartBit = 0;
				MAPBotom = OriginalMAPBotom;//备份原始GPS值 
				MAPLeft = OriginalMAPLeft;//备份原始GPS值 
				MAPTop = OriginalMAPTop;//备份原始GPS值 
				MAPRight = OriginalMAPRight;//备份原始GPS值 
				x_pixel = x_Originalpixel;// 原始的x
				y_pixel = y_Originalpixel;// 原始的y
				y_BitWidth = y_OriginalBit; //获取图片的高度  保存原来图片的大小
				x_BitWidth = x_OriginalBit; //获取图片的宽度  保存原来图片的大小
				PixState = 0;//回到原图
				if (PixState == 0  && ZoomLevel == 0)
				{
					break;
				}
				ZoomLevel = ZoomLevel - 1;
				Suspend_Thread();
				iNowZoomRunning = 1;//正在放大或缩小
				iCountFlushMap ++; //地图的刷新次数加一
				while (iCountFlushExamCar)
				{
					Sleep(200);
					iCountFlushExamCar = 0;
				}
				Sleep(100);
				Invalidate(FALSE);
		//		while (iNowZoomRunning);
		//		WakeUp_Thread();
				
			}
			
			break;
*/
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
			{
				if (PixState == 0  && ZoomLevel == 0)
				{
					break;
				}
				ZoomLevel = ZoomLevel - 1;

				
// 				CRect ClientRect; //客户端区域
// 				GetClientRect(ClientRect);
// 				InvalidateRect(&ClientRect,FALSE);
				Suspend_Thread();
				iNowZoomRunning = 1;//正在放大或缩小
				iCountFlushMap ++; //地图的刷新次数加一
				while (iCountFlushExamCar)
				{
					Sleep(200);
					iCountFlushExamCar = 0;
				}
			//	Sleep(100);
			//	Invalidate(FALSE);
				HasPlayInvalite();
			//	while (iNowZoomRunning);
			//	WakeUp_Thread();
				
			}
			break;
		default:
			break;
		}
	}
//	Sleep(50);
//	Sleep(100);
	iNowZoomRunning = 0;//没有放大或缩小
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CCBKDigitalMapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags == MK_LBUTTON )
	{
		//说明是鼠标的左键按下
		LButtonDown = TRUE;
		x_CurLbuttonDown = point.x;
		y_CurLbuttonDown = point.y;
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CCBKDigitalMapDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//说明是鼠标的左键提起
	LButtonDown = FALSE;
	
	CDialog::OnLButtonUp(nFlags, point);

	//创建一个线程用来设置 iStateLBD 标志位  iStateLBD = 0;//用来标记鼠标左键按下 拖动时为1 正常状态是为0
	AfxBeginThread(&ButtonUp_thd, this); //专门用一个线程来刷新地图和考车  

	
}

void CCBKDigitalMapDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	DOUBLE x_Latitude1 = 0;  //纬度的偏移量
	DOUBLE y_Long1 = 0;  //经度的偏移量
	if (LButtonDown && ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)//鼠标按下 开始拖动画布  并且此时没有考车被实时跟踪
	{
		iStateLBD = 1;//用来标记鼠标左键按下 拖动时为1 正常状态是为0
		if (x_CurLbuttonDown != point.x || y_CurLbuttonDown != point.y)
		{
			// 1、判断鼠标是左移 右移 上移 下移
			
			if (x_CurLbuttonDown != point.x)
			{
				if (x_CurLbuttonDown - point.x > 0)// 说明是左移
				{
					//算出此时的纬度偏移量
					x_Latitude1 = (x_CurLbuttonDown - point.x) * x_pixel;
					//看下右边有没有移到原图的右边
					if (MAPRight + x_Latitude1 <= OriginalMAPRight) //说明还没有超出原图
					{
						MAPRight = MAPRight + x_Latitude1;
						MAPLeft = MAPLeft + x_Latitude1;
					}
					else
					{
						//超出了原图的右边  最多只能移到原图的右边
						DOUBLE x_Latitude2 = 0;//这一个是用来看下到底偏移了多少
						x_Latitude2 = OriginalMAPRight - MAPRight;
						MAPRight = OriginalMAPRight;
						MAPLeft = MAPLeft + x_Latitude2;
					}
				}
				else
				{
					//说明是右移
					//算出此时的纬度偏移量
					x_Latitude1 = (point.x - x_CurLbuttonDown) * x_pixel;
					//看下左边有没有移到原图的左边
					if (MAPLeft - x_Latitude1 >= OriginalMAPLeft) //说明还没有超出原图
					{
						MAPLeft = MAPLeft - x_Latitude1;
						MAPRight = MAPRight - x_Latitude1;
					}
					else
					{
						//超出了原图的左边  最多只能移到原图的左边
						DOUBLE x_Latitude2 = 0;//这一个是用来看下到底偏移了多少
						x_Latitude2 = MAPLeft - OriginalMAPLeft;
						MAPLeft = OriginalMAPLeft;
						MAPRight = MAPRight - x_Latitude2;
					}
				}
				 
			}

			//处理上移下移的
			if (y_CurLbuttonDown != point.y)
			{
				if (y_CurLbuttonDown - point.y > 0)// 说明是上移
				{
					//算出此时的经度偏移量
					y_Long1 = (y_CurLbuttonDown - point.y) * y_pixel;
					//看下下边有没有移到原图的下边
					if (MAPBotom - y_Long1 >= OriginalMAPBotom) //说明还没有超出原图
					{
						MAPBotom = MAPBotom - y_Long1;
						MAPTop = MAPTop - y_Long1;
					}
					else
					{
						//超出了原图的下边  最多只能移到原图的下边
						DOUBLE y_Long2 = 0;//这一个是用来看下到底偏移了多少
						y_Long2 = MAPBotom - OriginalMAPBotom;
						MAPBotom = OriginalMAPBotom;
						MAPTop = MAPTop - y_Long2;
					}
				}
				else
				{
					//说明是下移
					//算出此时的纬度偏移量
					y_Long1 = (point.y  - y_CurLbuttonDown) * y_pixel;
					//看下上边有没有移到原图的上边
					if (MAPTop + y_Long1 <= OriginalMAPTop) //说明还没有超出原图
					{
						MAPTop = MAPTop + y_Long1;
						MAPBotom = MAPBotom + y_Long1;
					}
					else
					{
						//超出了原图的上边  最多只能移到原图的上边
						DOUBLE y_Long2 = 0;//这一个是用来看下到底偏移了多少
						y_Long2 = OriginalMAPTop - MAPTop;
						MAPTop = OriginalMAPTop;
						MAPBotom = MAPBotom + y_Long2;
					}
				}
				
			}
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);

			Suspend_Thread();
			iNowZoomRunning = 1;//正在放大或缩小
			iCountFlushMap ++; //地图的刷新次数加一
			while (iCountFlushExamCar)
			{
				Sleep(200);
				iCountFlushExamCar = 0;
			}
		//	Sleep(2);
		//	Invalidate(FALSE);
			HasPlayInvalite();
			//Sleep(20);
		//	iNowZoomRunning = 0;//没有放大或缩小 这句话本来是写在这里的 现在改写在OnPaint函数尾部
			x_CurLbuttonDown = point.x;
			y_CurLbuttonDown = point.y;
			
		}
	}

	// 判断鼠标有没有落在这个区域
	if (0 == ExamCarInfo.MouseIsThisRect)
	{
// 		CString temp;
// 		temp.Format("x = %d y = %d",point.x,point.y);
// 		WriteLog(temp);
		//说明还没有落在这个区域
		if (point.x >= 700 && point.y <= 15 && point.x <= 980)
		{
			ExamCarInfo.ShowDlgState = 0; //伸长出来
			ExamCarInfo.MouseIsThisRect = 1;
		}
		
	}
	else if(1 == ExamCarInfo.MouseIsThisRect)
	{
		//落在这个区域
		if (point.x < 700 || point.y > 640 || point.x > 980)
		{
			//说明已经离开了那个区域
			ExamCarInfo.ShowDlgState = 1; //收缩出来
			ExamCarInfo.MouseIsThisRect = 0;
		}
	}

	/////////////////////
	// 下面这一段代码是用来判断此时鼠标有没有落在考车上面  
	//  m_Button[Pi].CurCar_Xpoint > 0 && m_Button[Pi].CurCar_Ypoint > 0
	int i = 0;
	for(i = 1;i < MAX_ExamCar;i++)
	{
		if(point.x >= m_Button[i].CurCar_Xpoint && point.x <= m_Button[i].CurCar_Xpoint + 20 &&  point.y <= m_Button[i].CurCar_Ypoint + 20  && point.y >= m_Button[i].CurCar_Ypoint)
		{
			// 调用按钮的那个函数  将那个标志位置为TRUE
//			int((on_ExamCar[Pi].x_Gps - MAPLeft) / x_pixel),int(CurrentScrHeight - ((on_ExamCar[Pi].y_Gps - MAPBotom) / y_pixel) - 10)
			bMouseOn[i] = 1;  //这个是用于判断鼠标有没有在这辆考车上  TRUE为在这考车上 FALSE 没有落在这辆考车上
		}
		else
		{
			bMouseOn[i] = 0;  //这个是用于判断鼠标有没有在这辆考车上  TRUE为在这考车上 FALSE 没有落在这辆考车上
		}
	}
	//  如果有就调用对应的提示框
	/////////////////////

	CDialog::OnMouseMove(nFlags, point);
}

void CCBKDigitalMapDlg::CreateDlgha()
{
	int i;
	CWinThread *tmpCwinThread;
	//创建考车线程
	ExamCarInfoToThread_ST pExamCarInfoToThread_ST;
	pExamCarInfoToThread_ST = (ExamCarInfoToThread_ST)malloc(sizeof(struct ExamCarInfoToThread));
	
	for (i = 0;i < MAX_ExamCar+1;i++)
	{
		memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
		pExamCarInfoToThread_ST->pMapDlg = this;
		pExamCarInfoToThread_ST->iExamCarNumber = i;
		tmpCwinThread = AfxBeginThread(&ExamCarGPSLocate_thd, pExamCarInfoToThread_ST);
		ThreadHandle[i] = tmpCwinThread->m_hThread;
		if (i == MAX_ExamCar )
		{
			memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
			pExamCarInfoToThread_ST->pMapDlg = this;
			pExamCarInfoToThread_ST->iExamCarNumber = 0; //创建的第一个线程 具体为什么要这么做我也不知道 
		}
		Sleep(100);
		SuspendThread(ThreadHandle[i]);  //挂起线程
	}
	/*
	Sleep(500);
	for (i = 0;i <= MAX_ExamCar;i++)
	{
		SuspendThread(ThreadHandle[i]);  //挂起线程
	}
	*/
	free(pExamCarInfoToThread_ST);

	//创建各考车对应的提示框
	for (i = 0;i < MAX_ExamCar;i++)
	{
		dlgha[i].Create(IDD_DIALOG1,this);
		dlgha[i].ShowWindow(SW_HIDE);
	}
//	m_boot.MoveWindow(-200, -200,100,50); (CurrentScrWidth / 2 - 150,CurrentScrHeight / 2 - 25,300,50);
	m_boot.SetWindowText("");
	CRect pCRect(CurrentScrWidth / 2 - 150,CurrentScrHeight / 2 - 25,CurrentScrWidth / 2 + 150,CurrentScrHeight / 2 + 25);
	InvalidateRect(&pCRect);
}

void CCBKDigitalMapDlg::Suspend_Thread() //挂起线程
{
	int i = 0;
	for (i = 1;i < pSocket.TotalCar;i++)
	{
		if (m_Button[i].ExamCarState == 1) //考车在线
		{
		//	SuspendThread(ThreadHandle[i]);
			while(!SuspendThread(ThreadHandle[i]));
		//	m_Button[i].ShowWindow(SW_HIDE) ;   这种效果不好 放大或缩小的时候 按钮处会出现白色区域
			m_Button[i].MoveWindow(-20,-20,20,20);//
		}
	}
}

void CCBKDigitalMapDlg::WakeUp_Thread()  //唤醒线程
{
	int i = 0;
	int iCount = 0;
//	int iResult = 0;
//	iResult = pMutex.Unlock();// 用来处理线程有时候只有一个线程操作不知道为什么
	for (i = 1;i < pSocket.TotalCar;i++)
	{
		if (m_Button[i].ExamCarState == 1) //考车在线
		{
			int xx = 0;
			while (xx = ResumeThread(ThreadHandle[i])); //如果计数还没有到0 就继续唤醒
		//	ResumeThread(ThreadHandle[i]);
		}
	}
}

void CCBKDigitalMapDlg::OnDestroy() 
{
//	int m;
	CString E;
	/*
	for (m = 0;m < MAX_ExamCar;m++)
	{
		E.Format("EXAMCAR = %d x = %lf,y = %lf",on_ExamCar[m].ExamCarNumber, on_ExamCar[m].x_Gps,on_ExamCar[m].y_Gps);
		WriteLog(E);
	}
	*/

	iContrl = 0;

	/////////发送一个消息让播放视频程序结束////////
	if(iISCreate == 1)
	{
		HWND hWnd = GetServerMainWnd();
		if(hWnd == NULL)
		{    
	//		return;
		}
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->SendMessage(WM_TEXTMESSAGE, 0, 0);  //这个是用于关闭子进程程序
	}

	if(iISCreateT == 1)
	{
		HWND hWnd = GetServerMainWndT();
		if(hWnd == NULL)
		{    
	//		return;
		}
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->SendMessage(WM_TEXTMESSAGET, 0, 0);  //这个是用于关闭子进程程序
	}

	if(iISCreateS == 1)
	{
		HWND hWnd = GetServerMainWndS();
		if(hWnd == NULL)
		{    
	//		return;
		}
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->SendMessage(WM_TEXTMESSAGES, 0, 0);  //这个是用于关闭子进程程序
	}
	/////////////////
	Sleep(10000);
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


//用来捕获按ESC键的
BOOL CCBKDigitalMapDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int iChoose = 0;
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//MessageBox(_T("回车键按下"));
			return TRUE; //屏蔽Enter键
		case VK_ESCAPE:
			//MessageBox(_T("ESC键按下"));
			iChoose = MessageBox("确定要退出程序么?","退出提示",MB_YESNO);
			if (iChoose == IDYES)
			{
				//return TRUE;
				//::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
				//this->SendMessage(WM_CLOSE);
				CDialog::OnCancel();
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




void CCBKDigitalMapDlg::WriteLog(CString szERR, CString car)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen(car,"a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}


BOOL CCBKDigitalMapDlg::ReadOnlineCarFromDB()
{
	
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString c_ExamCarNumber;
	CString c_TmpCarNumber;
	int iLength;
	//////////       以下代码用来初始化map容器           ///////////
	int iValue = 1;
	for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
	{
		SockToCar1[iValue] = -1;
	}
	
	//////////        初始化map容器结束          ///////////
	try
	{
		//	BOOL sInit=theApp.OnInitSQL();
		//	if (sInit)
		//	{
		CString strSQL;	
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("SELECT * FROM SysCfg WHERE 状态 = '1'");
		
		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
			while(!pSet->adoEOF)
			{
				
				

				vat =pSet->GetCollect("项目");  //里面存的是考车号
				if (vat.vt != VT_NULL)
				{
					c_ExamCarNumber = (LPCSTR)_bstr_t(vat);
					c_TmpCarNumber = c_ExamCarNumber.Left(4);
					if (c_TmpCarNumber == "考车")
					{
						// 截取考车后的所有数据j
						pSocket.TotalCar++;
						iLength = c_ExamCarNumber.GetLength();
						c_TmpCarNumber = c_ExamCarNumber.Right(iLength - 4);
						for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
						{
							if (SockToCar1.find(iValue)->second == -1)
						//	if(m_Button[iValue].examCarNumber == 0)
							{
								SockToCar1[iValue] = _ttoi(c_TmpCarNumber);
							//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
								break;
							}
							
						}
					}
				}
				
				
				//	pSet->MoveNext();
				
				pSet->MoveNext();
				
			}
			pSet->Close();
			//释放
			pSet.Release();
			//	}
	//		Sleep(50);
			
			
		}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}
















BOOL CCBKDigitalMapDlg::ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
   
	CString temp;
	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;
			CString strSQL;	
	//		strSQL.Format("SELECT * FROM StudentPhoto WHERE 准考证明编号 = '%s' and 状态 = '3'",sCard);
			strSQL.Format("SELECT * FROM StudentPhoto WHERE 准考证明编号 = '%s'",sCard);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			if(pRecordset->RecordCount != 1)
			{
				return FALSE;
			}
			
			char* pbuf = NULL;
			
			long lDataSize = pRecordset->GetFields()->GetItem("照片")->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{
				_variant_t varBLOB;
				varBLOB = pRecordset->GetFields()->GetItem("照片")->GetChunk(lDataSize);
				m_pBMPBuffer = new char[lDataSize+1];
				
				pRecordset->Close();
				pRecordset.Release();
				
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					
					SafeArrayUnaccessData(varBLOB.parray);
					
					//******************************************************//
					IStream* pStm;
					LONGLONG cb = lDataSize;
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
					LPVOID pvData;
					if (hGlobal != NULL)
					{
						pvData = GlobalLock(hGlobal);
						memcpy(pvData, m_pBMPBuffer, cb);
						GlobalUnlock(hGlobal);
						CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
					}
					else
					{
						//AfxMessageBox("读取照片信息错误");
						return FALSE;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
					}
					else
					{
						return FALSE;
					}
					pStm->Release ();
					pImg->Release();
					GlobalFree(hGlobal); 
					if(m_pBMPBuffer)
					{
						delete m_pBMPBuffer;
						m_pBMPBuffer = NULL;
					}
					
				}
				pbuf = NULL;
			}
			else
			{
				pRecordset->Close();
				pRecordset.Release();
			}
	//	}

	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		return FALSE;
	}
	
	return TRUE;
}



/*
这个函数的功能是用来如果有视频播放的话刷新的区域要除去视频那一部分
*/
BOOL CCBKDigitalMapDlg::HasPlayInvalite()
{
	if(iStateDlgShow == 0)
	{
		if(iStateDlgShow == 1) // 说明这个时候对话框正在show出来还是收回去
		{
			CRect pRect(0,0,700,CurrentScrHeight);
			InvalidateRect(&pRect,FALSE);
			pRect.SetRect(700,temPoint.y,980,CurrentScrHeight);
			InvalidateRect(&pRect,FALSE);
			pRect.SetRect(980,0,CurrentScrWidth,CurrentScrHeight);
			InvalidateRect(&pRect,FALSE);
		}
		else  // 说明这个时候没有对话框正在show
		{ 
			Invalidate(FALSE);
		}
		
	}
	return TRUE;
}


BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
{
//	TCHAR szWndText[MAX_PATH];
	unsigned long dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if(dwProcId == g_ulProcessId)
	{
		if(NULL == ::GetParent(hWnd))
		{
			*(HWND*)lParam = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}


BOOL CALLBACK EnumWndProcT(HWND hWnd, LPARAM lParam)
{
//	TCHAR szWndText[MAX_PATH];
	unsigned long dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if(dwProcId == g_ulProcessIdT)
	{
		if(NULL == ::GetParent(hWnd))
		{
			*(HWND*)lParam = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CALLBACK EnumWndProcS(HWND hWnd, LPARAM lParam)
{
//	TCHAR szWndText[MAX_PATH];
	unsigned long dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if(dwProcId == g_ulProcessIdS)
	{
		if(NULL == ::GetParent(hWnd))
		{
			*(HWND*)lParam = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCBKDigitalMapDlg::OnButtonopen() 
{
	// TODO: Add your control notification handler code here
	

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//得到ShowPlay.exe绝对路径
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) 
		return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcess = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessId = pi.dwProcessId;

		iISCreate = 1; //说明子进程已经创建
		return TRUE;
	}
	else
	{
		WriteLog("创建主驾进程失败");
		return FALSE;
	//	AfxMessageBox("创建进程失败");
	}

		
}

HWND CCBKDigitalMapDlg::GetServerMainWnd()
{
	HWND hWnd;
	if(0 == EnumWindows((WNDENUMPROC)EnumWndProc, (LPARAM)&hWnd))
	{
		return hWnd;
	}
	return NULL;
}

HWND CCBKDigitalMapDlg::GetServerMainWndT()
{
	HWND hWnd;
	if(0 == EnumWindows((WNDENUMPROC)EnumWndProcT, (LPARAM)&hWnd))
	{
		return hWnd;
	}
	return NULL;
}

HWND CCBKDigitalMapDlg::GetServerMainWndS()
{
	HWND hWnd;
	if(0 == EnumWindows((WNDENUMPROC)EnumWndProcS, (LPARAM)&hWnd))
	{
		return hWnd;
	}
	return NULL;
}

void CCBKDigitalMapDlg::SendMessageToVideoDlg(UINT ExamCar,UINT VideoAdd)
{
	HWND hWnd;
	do
	{
		hWnd = GetServerMainWnd();
		Sleep(100);
		WriteLog("一直循环");
	}
	while(hWnd == NULL);//说明还没有找到这个句柄
  
WriteLog("成功后2");
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGE, ExamCar, VideoAdd);  //
}

void CCBKDigitalMapDlg::SendMesageToVideoDlgClose()
{
	HWND hWnd = GetServerMainWnd();
	if(hWnd == NULL)
	{    
//		return;
	}
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGE, 0, 3);  //这个是用于关闭子进程程序
}

BOOL CCBKDigitalMapDlg::CreateVideoDlg()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//得到ShowPlay.exe绝对路径
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	WriteLog(strFilePath);
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcess = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessId = pi.dwProcessId;

		iISCreate = 1; //说明子进程已经创建
	}
	else
	{
		WriteLog("创建主驾进程失败");
		return FALSE;
	//	AfxMessageBox("创建进程失败");
	}
	return TRUE;
}


BOOL CCBKDigitalMapDlg::CreateVideoDlgT()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//得到ShowPlay.exe绝对路径
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	WriteLog(strFilePath);
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessT = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdT = pi.dwProcessId;

		iISCreateT = 1; //说明子进程已经创建
//		GetDlgItem(IDC_BUTTON_SHOWPLAY)->SetWindowText("隐藏");
	}
	else
	{
		WriteLog("创建副驾进程失败");
		return FALSE;
	//	AfxMessageBox("创建进程失败");
	}
	return TRUE;
}



BOOL CCBKDigitalMapDlg::CreateVideoDlgS()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//得到ShowPlay.exe绝对路径
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	WriteLog(strFilePath);
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessS = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdS = pi.dwProcessId;

		iISCreateS = 1; //说明子进程已经创建
//		GetDlgItem(IDC_BUTTON_SHOWPLAY)->SetWindowText("隐藏");
	}
	else
	{
		WriteLog("创建车外进程失败");
		return FALSE;
	//	AfxMessageBox("创建进程失败");
	}
	return TRUE;
}

BOOL CCBKDigitalMapDlg::OnButtonopenT()
{
	// TODO: Add your control notification handler code here
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//得到ShowPlay.exe绝对路径
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) 
		return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessT = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdT = pi.dwProcessId;

		iISCreateT = 1; //说明子进程已经创建
		return TRUE;
	}
	else
	{
		WriteLog("创建副驾进程失败");
	//	AfxMessageBox("创建进程失败");
		return FALSE;
	}

	
}

BOOL CCBKDigitalMapDlg::OnButtonopenS()
{

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//得到ShowPlay.exe绝对路径
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) 
		return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessS = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdS = pi.dwProcessId;

		iISCreateS = 1; //说明子进程已经创建
		return TRUE;
	}
	else
	{
		WriteLog("创建车外进程失败");
	//	AfxMessageBox("创建进程失败");
		return FALSE;
	}

}

void CCBKDigitalMapDlg::SendMessageToVideoDlgT(UINT ExamCar, UINT VideoAdd)
{
	HWND hWnd;
	do
	{
		hWnd = GetServerMainWndT();
		Sleep(100);
		WriteLog("一直循环");
	}
	while(hWnd == NULL);//说明还没有找到这个句柄
  
	WriteLog("成功后2");
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGET, ExamCar, VideoAdd);  //
}

void CCBKDigitalMapDlg::SendMessageToVideoDlgS(UINT ExamCar, UINT VideoAdd)
{
	HWND hWnd;
	do
	{
		hWnd = GetServerMainWndS();
		Sleep(100);
		WriteLog("一直循环");
	}
	while(hWnd == NULL);//说明还没有找到这个句柄
  
	WriteLog("成功后2");
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGES, ExamCar, VideoAdd);  //
}

void CCBKDigitalMapDlg::SendMesageToVideoDlgCloseT()
{
	HWND hWnd = GetServerMainWndT();
	if(hWnd == NULL)
	{    
//		return;
	}
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGET, 0, 3);  //这个是用于关闭子进程程序
}

void CCBKDigitalMapDlg::SendMesageToVideoDlgCloseS()
{
	HWND hWnd = GetServerMainWndS();
	if(hWnd == NULL)
	{    
//		return;
	}
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGES, 0, 3);  //这个是用于关闭子进程程序
}



BOOL CCBKDigitalMapDlg::ReadOnlineCarFromDB1()
{
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString c_ExamCarNumber;
	CString c_TmpCarNumber;
	int iLength;
	/*/////////       以下代码用来初始化map容器           ///////////
	int iValue = 1;
	for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
	{
		SockToCar1[iValue] = -1;
	}
	
	//////////        初始化map容器结束          //////////*/
	try
	{
		//	BOOL sInit=theApp.OnInitSQL();
		//	if (sInit)
		//	{
		CString strSQL;	
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("SELECT * FROM SysCfg WHERE 状态 != '1'");
		
		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
			while(!pSet->adoEOF)
			{
				
				

				vat =pSet->GetCollect("项目");  //里面存的是考车号
				if (vat.vt != VT_NULL)
				{
					c_ExamCarNumber = (LPCSTR)_bstr_t(vat);
					c_TmpCarNumber = c_ExamCarNumber.Left(4);
					if (c_TmpCarNumber == "考车")
					{
						// 截取考车后的所有数据j
				//		pSocket.TotalCar++;
						iLength = c_ExamCarNumber.GetLength();
						c_TmpCarNumber = c_ExamCarNumber.Right(iLength - 4);
						int iValue = 0;
						for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
						{
							if (SockToCar1.find(iValue)->second == _ttoi(c_TmpCarNumber))  //说明有找到 也就是说这辆考车下线了
						//	if(m_Button[iValue].examCarNumber == 0)
							{
								int tmpCar = SockToCar1[iValue];
								SockToCar1[iValue] = -1;
								pSocket.TotalCar--;
								int iValue1 = 0;
								for(iValue1 = 1; iValue1 < MAX_ExamCar;iValue1 ++)
								{
									if(m_Button[iValue1].examCarNumber == tmpCar)
									{
										m_Button[iValue1].MoveWindow(-20,-20,20,20);
										//这个要改成创建线程来延时后改变标志位
										AfxBeginThread(&ExamCarHideDown_thd, &m_Button[iValue1]);

										/*
										m_Button[iValue1].examCarNumber = 0;
			
										m_Button[iValue1].CurCar_Xpoint = 0;
										m_Button[iValue1].CurCar_Ypoint = 0;

										
										m_Button[iValue1].ExamCarState == 0;

										CString tmpOnLineCar;
										tmpOnLineCar.Format("当前考车数为：%d",pSocket.TotalCar);
										m_TotalCar.SetWindowText(tmpOnLineCar);
										*/ 
										break;
									}
								}
								
							//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
								break;
								
							}
							
						}
					}
				}
				
				
				//	pSet->MoveNext();
				
				pSet->MoveNext();
				
			}
			pSet->Close();
			//释放
			pSet.Release();
			//	}
	//		Sleep(50);
			
			
		}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}

//DEL void CCBKDigitalMapDlg::ChangeTrackVideoPlayT(HTREEITEM pVCurItem)
//DEL {
//DEL 
//DEL }

void CCBKDigitalMapDlg::DeleteLogData()
{
	CFileFind fFind;
	//判断文件MP4是否存在
	//删除MP4
	CString tmpFileName;
	tmpFileName.Format("接收数据.txt");
	if(fFind.FindFile(tmpFileName)!=0)
	{
		CFile::Remove(tmpFileName);
	}
	tmpFileName.Empty();
	tmpFileName.Format("VideoERRLog");
	if(fFind.FindFile(tmpFileName)!=0)
	{
		CFile::Remove(tmpFileName);
	}
	tmpFileName.Empty();
	tmpFileName.Format("datalog");
	if(fFind.FindFile(tmpFileName)!=0)
	{
		CFile::Remove(tmpFileName);
	}
	BOOL found = fFind.FindFile("*.clog");	
	//删除非当日数据库
	while(found)
	{
		found = fFind.FindNextFile();
		tmpFileName = fFind.GetFileName();		
		CFile::Remove(tmpFileName);
	}

}

BOOL CCBKDigitalMapDlg::ReadOnlineCarFromDB2()
{
	BOOL bflag = FALSE;//这个标志位是用来判断当次搜索的时候有没有找到 true为没有找到 需要添加 false为有找到不需要添加
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString c_ExamCarNumber;
	CString c_TmpCarNumber;
	int iLength;
	/*/////////       以下代码用来初始化map容器           ///////////
	int iValue = 1;
	for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
	{
		SockToCar1[iValue] = -1;
	}
	
	//////////        初始化map容器结束          //////////*/
	try
	{
		//	BOOL sInit=theApp.OnInitSQL();
		//	if (sInit)
		//	{
		CString strSQL;	
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("SELECT * FROM SysCfg WHERE 状态 = '1'");
		
		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
			while(!pSet->adoEOF)
			{
				
				

				vat =pSet->GetCollect("项目");  //里面存的是考车号
				if (vat.vt != VT_NULL)
				{
					c_ExamCarNumber = (LPCSTR)_bstr_t(vat);
					c_TmpCarNumber = c_ExamCarNumber.Left(4);
					if (c_TmpCarNumber == "考车")
					{
						// 截取考车后的所有数据j
				//		pSocket.TotalCar++;
						iLength = c_ExamCarNumber.GetLength();
						c_TmpCarNumber = c_ExamCarNumber.Right(iLength - 4);
						int iValue = 0;

						bflag = TRUE;
						for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
						{
							if (SockToCar1.find(iValue)->second == _ttoi(c_TmpCarNumber))  //说明有找到 也就是说这辆考车之前就已经上线了
						//	if(m_Button[iValue].examCarNumber == 0)
							{
								bflag = FALSE;
								break;
							}
						}

						if(bflag)  //说明需要添加
						{
							for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
							{
								if (SockToCar1.find(iValue)->second == -1)
									//	if(m_Button[iValue].examCarNumber == 0)
								{
									SockToCar1[iValue] = _ttoi(c_TmpCarNumber);
									//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
									break;
								}
								
							}
								
							pSocket.TotalCar++;
							int iValue1 = 0;
							for(iValue1 = 1; iValue1 < MAX_ExamCar;iValue1 ++)
							{
								if(m_Button[iValue1].examCarNumber == 0)
								{
									m_Button[iValue1].examCarNumber = _ttoi(c_TmpCarNumber);
		
									m_Button[iValue1].CurCar_Xpoint = 0;
									m_Button[iValue1].CurCar_Ypoint = 0;
									m_Button[iValue1].ExamCarState = 1;


									CString tmpOnLineCar;
									tmpOnLineCar.Format("当前考车数为：%d",pSocket.TotalCar);
									m_TotalCar.SetWindowText(tmpOnLineCar);  

									break;
								}
							}														
						}						
					}
				}
				
				
				//	pSet->MoveNext();
				
				pSet->MoveNext();
				
			}
			pSet->Close();
			//释放
			pSet.Release();
			//	}
	//		Sleep(50);
			
			
		}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}

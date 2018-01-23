// CDMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CDMap.h"
#include "CDMapDlg.h"

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
// CCDMapDlg dialog

CCDMapDlg::CCDMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCDMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCDMapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCDMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDMapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCDMapDlg, CDialog)
	//{{AFX_MSG_MAP(CCDMapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDM_EXITS, OnExits)
	ON_COMMAND(IDM_SHOWALL, OnShowall)
	ON_MESSAGE(UM_SOCKETZ,OnSocketZ)//TCP 257
	ON_COMMAND(IDM_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDMapDlg message handlers
LRESULT CCDMapDlg::OnSocketZ(WPARAM wParam,LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam); //消息类别 
	int iErrorCode = WSAGETSELECTERROR(lParam);//错误代码 
	SOCKET CurSock = (SOCKET)wParam; //目标socket 
	CString recvTemp;
	switch(iEvent) 
	{ 
	case FD_ACCEPT:{
		m_tcps.OnFDAccept();
		return 0;}		
	case FD_READ:{
		recvTemp=m_tcps.RecvFromSock(CurSock);
		OnGnssData(recvTemp,CurSock);
		return 0;}
	case FD_CLOSE:{		
		OnSocketClose(CurSock);
		closesocket(CurSock);
		logz.WriteLog(dwlogID,"TCP 客户端退出! Port:%d",CurSock);
		return 0;}
	default: 
		return 0;
	}
	return 0;
}

BOOL CCDMapDlg::OnFindMAP(int ikch)
{
	MAP_INT_SOCKET::iterator iter;	
	iter = m_CarMapN.find(ikch);	
	if(iter == m_CarMapN.end())
	{
		return FALSE;
	}
	return TRUE;
}

UINT CCDMapDlg::InvaliDateMap_thd(LPVOID p)
{
	int tmpCurTrackCar = -1;//用来临时存放被跟踪的考车
	Sleep(1000);
	CCDMapDlg *pInfo = (CCDMapDlg *)p;
	int CurCar = 0;
	int i = 0;
	while (1)
	{			
		pInfo->Invalidate(FALSE);
		Sleep(1000);
	}
	return 0;
}

BOOL CCDMapDlg::OnInitDialog()
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
//	hbrDlg=CreateSolidBrush(RGB(60,60,60));
	dwlogID=logz.Register("CDMap.log");
	ShowWindow(SW_SHOWMAXIMIZED);// 全屏
	if (!InitMap())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	if (!m_tcps.InitSockS(m_hWnd,9800,UM_SOCKETZ,dwlogID))//接收考车发送TCP数据
	{
		CDialog::OnCancel();
		return FALSE;
	}

	OnShowall();
	
	AfxBeginThread(&InvaliDateMap_thd, this);
	logz.WriteLog(dwlogID,"数字地图 V2.0 版本!Build 20180122");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCDMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCDMapDlg::OnPaint() 
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		//CPaintDC dc(this); // device context for painting

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
		Graphics grap(m_dcMem.GetSafeHdc());
		Color blueColor(255,30,30,30);
		grap.Clear(blueColor);
		grap.SetInterpolationMode(InterpolationModeNearestNeighbor);
		grap.DrawImage(m_pImageMap,Rect(0,0, ClientRect.Width(),ClientRect.Height()),(int)m_ImageL,(int)m_ImageT,(int)m_ImageW,(int)m_ImageH,UnitPixel);
	
		m_imgRc.SetRect(m_ImageL,m_ImageT,m_ImageW+m_ImageL,m_ImageH+m_ImageT);
		MAP_INT_SOCKET::iterator iter;
		for (iter=m_CarMapN.begin();iter!=m_CarMapN.end();iter++)
		{		
			if (carimg[iter->first].IsPtinScreen(m_imgRc,m_ZoomLevel, m_ZoomLevelY))
			{
				carimg[iter->first].DrawCar(&grap);
			}
		}
		dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&m_dcMem,0,0,SRCCOPY);
// 		CString temp,temp1,temp2;
// 		temp.Format("m_ImageL=%.1f,m_ImageT=%.1f,m_ImageW=%.1f,m_ImageH=%.1f,m_ZoomLevel=%f",m_ImageL,m_ImageT,m_ImageW,m_ImageH,m_ZoomLevel);
//  		dc.TextOut(10,10,temp);
// 		temp1.Format("left=%f,top=%f,right=%f,bottom=%f",m_DestL,m_DestT,m_DestW,m_DestH);
// 		dc.TextOut(10,30,temp1);
// 		temp2.Format("m_ImgLOld=%.1f,m_ImgTOld=%.1f,m_ZoomLevelOld=%f",m_ImgLOld,m_ImgTOld,m_ZoomLevelOld);
// 		dc.TextOut(10,60,temp2);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCDMapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCDMapDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CMenu myPmenu,* pmypMenu;
	myPmenu.LoadMenu(IDR_MENU1);
	pmypMenu = myPmenu.GetSubMenu(0);
	pmypMenu->TrackPopupMenu (TPM_LEFTALIGN, point.x, point.y, this);

	CDialog::OnRButtonDown(nFlags, point);
}

bool CCDMapDlg::InitMap()
{
	//初始化地图配置
	m_isLBDown=false;
	CDC *pDC = this->GetDC();
	GetClientRect(&ClientRect); 	
	m_bmpMap.CreateCompatibleBitmap(pDC,ClientRect.Width(),ClientRect.Height());
	if (m_bmpMap.m_hObject==NULL)
	{
		AfxMessageBox("创建 地图 设备兼容的位图失败!");
		return false;
	}
	m_dcMem.CreateCompatibleDC(pDC);
	m_dcMem.SelectObject(m_bmpMap);
	this->ReleaseDC(pDC);

	m_pImageMap=NULL;
	m_pImageMap = Gdiplus::Image::FromFile(L"MAPN.skin");
	
	m_imageSize.cx=m_pImageMap->GetWidth();
	m_imageSize.cy=m_pImageMap->GetHeight();
	if (m_imageSize.cx==0 || m_imageSize.cy==0)
	{
		AfxMessageBox("加载 地图 失败!");
		return false;
	}
// 	float tempLevelx,tempLevely;
// 	tempLevelx=(float)m_imageSize.cx/ClientRect.Width();
// 	tempLevely=(float)m_imageSize.cy/ClientRect.Height();
// 	if (tempLevelx > tempLevely)
// 	{
// 		m_ShowAllL=0;
// 		m_ShowAllT=0;
// 		m_ShowAllW=ClientRect.Width();
// 		m_ShowAllH=ClientRect.Height();
// 		m_ZoomLevel=tempLevely;
// 	}
// 	else
// 	{
// 		m_ShowAllL=0;
// 		m_ShowAllT=0;
// 		m_ShowAllW=ClientRect.Width();
// 		m_ShowAllH=ClientRect.Height();
// 		m_ZoomLevel=tempLevelx;
// 	}
// 	m_DestL=m_ShowAllL;
// 	m_DestT=m_ShowAllT;
// 	m_DestW=m_ShowAllW;
// 	m_DestH=m_ShowAllH;


	m_MinZoomLevel=float((float)m_imageSize.cx / (float)ClientRect.Width());
	m_MinZoomLevelHeight = float((float)m_imageSize.cy / (float)ClientRect.Height());
	m_ZoomLevel=m_MinZoomLevel;
	m_ZoomLevelY=m_MinZoomLevel;
 	m_ImageL=0;
 	m_ImageT=0;
	m_ImageW=m_imageSize.cx;
 	m_ImageH=m_imageSize.cy;
	//m_ImageW=ClientRect.Width()*m_ZoomLevel;
 	//m_ImageH=ClientRect.Height()*m_ZoomLevel;
	
	//////////////////////////////////////////////////////////////////////////
	CString MapCfg,keyXname,keyYname,temp;
	MapCfg.Format(".\\MAP.cfg");
	int ixc,iyc;
	ixc=GetPrivateProfileInt("MAPCONFIG","XC",0,MapCfg);
	iyc=GetPrivateProfileInt("MAPCONFIG","YC",0,MapCfg);
	if (ixc==1)
	{
		keyXname.Format("MINX");
	}
	else
	{
		keyXname.Format("MAXX");
	}
	if (iyc==1)
	{
		keyYname.Format("MINY");
	}
	else
	{
		keyYname.Format("MAXY");
	}
	if(GetPrivateProfileString("MAPCONFIG",keyXname,"",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
	{
		temp.ReleaseBuffer();
		m_Mapx=atof(temp);
	}
	else
	{
		logz.WriteLog(dwlogID,"MAP.cfg Get %s failed! ",keyXname);
	}
	if(GetPrivateProfileString("MAPCONFIG",keyYname,"",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
	{
		temp.ReleaseBuffer();
		m_Mapy=atof(temp);
	}
	else
	{
		logz.WriteLog(dwlogID,"MAP.cfg Get MINY failed! ");
	}
	
	if(GetPrivateProfileString("MAPCONFIG","ZoomIn","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
	{
		temp.ReleaseBuffer();
		m_bs=atof(temp);
	}
	else
	{
		logz.WriteLog(dwlogID,"MAP.cfg Get ZoomIn failed! ");
	}
	//////////////////////////////////////////////////////////////////////////
	return true;
}

void CCDMapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_LBx=point.x;
	m_LBy=point.y;
	m_isLBDown=true;
	CDialog::OnLButtonDown(nFlags, point);
}

void CCDMapDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_isLBDown)//左键按下
	{
		m_ImageL +=(m_LBx-point.x)*m_ZoomLevel;
		m_ImageT +=(m_LBy-point.y)*m_ZoomLevelY;		
		m_LBx=point.x;
		m_LBy=point.y;
		AssertImageLT();
		Invalidate(FALSE);
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CCDMapDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_isLBDown=false;
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CCDMapDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	short iDelta = zDelta; //确定滚动方向
	m_ZoomLevelOld =m_ZoomLevel;//
	m_ZoomLevelOldY = m_ZoomLevelY;
	m_ImgLOld =m_ImageL;
	m_ImgTOld =m_ImageT;
	//////////////////////////////////////////////////////////////////////////
	if (iDelta <0) //缩小
	{
		if (m_ZoomLevel >=m_MinZoomLevel)//
		{
			m_ZoomLevel =m_MinZoomLevel;
		}
		else
		{
			m_ZoomLevel+=0.1f;
		}	
		
		if (m_ZoomLevelY >=m_MinZoomLevelHeight)//
		{
			m_ZoomLevelY =m_MinZoomLevelHeight;
		}
		else
		{
			m_ZoomLevelY+=0.1f;
		}
	}
	else //放大
	{
		if (m_ZoomLevel <=0.4f)//
		{
			m_ZoomLevel =0.4f;
		}
		else
		{
			m_ZoomLevel-=0.1f;
		}

		if (m_ZoomLevelY <=0.4f)//
		{
			m_ZoomLevelY =0.4f;
		}
		else
		{
			m_ZoomLevelY-=0.1f;
		}
	}
	m_ImageL =m_ImgLOld + pt.x*(m_ZoomLevelOld-m_ZoomLevel);
	m_ImageT =m_ImgTOld + pt.y*(m_ZoomLevelOldY-m_ZoomLevelY);
	m_ImageW =ClientRect.Width()*m_ZoomLevel;
	m_ImageH =ClientRect.Height()*m_ZoomLevelY;
	AssertImageLT();
	Invalidate(FALSE);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CCDMapDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_isLBDown)
	{
		SetCursor(LoadCursor(NULL,IDC_SIZEALL));
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CCDMapDlg::AssertImageLT()
{
	if (m_ImageL <0)
	{
		m_ImageL=0;
	}
	if (m_ImageT <0)
	{
		m_ImageT=0;
	}
	if (m_ImageL > m_imageSize.cx -m_ImageW )
	{
		m_ImageL =m_imageSize.cx -m_ImageW;
	}
	if (m_ImageT > m_imageSize.cy -m_ImageH )
	{
		m_ImageT =m_imageSize.cy -m_ImageH;
	}
}

BOOL CCDMapDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CCDMapDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
// 	if ( nCtlColor==CTLCOLOR_DLG )
// 	{
// //		return hbrDlg;
// 	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CCDMapDlg::OnExits() 
{
	// TODO: Add your command handler code here
	if (MessageBox("是否退出程序!","提示",MB_OKCANCEL) == IDOK)
	{
		this->SendMessage(WM_CLOSE);
	}
}

void CCDMapDlg::OnShowall() 
{
	// TODO: Add your command handler code here
	//全图显示
	m_DestL=m_ShowAllL;
	m_DestT=m_ShowAllT;
	m_DestW=m_ShowAllW;
	m_DestH=m_ShowAllH;

	m_ZoomLevel = m_MinZoomLevel;
	m_ZoomLevelY = m_MinZoomLevelHeight;

	m_ImageL=0;
	m_ImageT=0;
	m_ImageW=m_imageSize.cx;
 	m_ImageH=m_imageSize.cy;
	//m_ImageW =ClientRect.Width()*m_ZoomLevel;
	//m_ImageH =ClientRect.Height()*m_ZoomLevel;

	AssertImageLT();
	Invalidate(FALSE);
}

void CCDMapDlg::OnSocketClose(SOCKET CSock)
{
	MAP_INT_SOCKET::iterator iter;
	for (iter=m_CarMapN.begin();iter!=m_CarMapN.end();)
	{
		if (iter->second==CSock)
		{
			logz.WriteLog(dwlogID,"删除 MAP 数据!考车[%d]下线! Port:%d",iter->first,CSock);
			iter=m_CarMapN.erase(iter);			
		}
		else
		{
			iter++;
		}				
	}
}

void CCDMapDlg::OnGnssData(CString recvtemp,SOCKET csocket)
{
	//logz.WriteLog(dwlogID,"收到考车数据,%s", recvtemp);
	
	UINT uCmd=atoi(recvtemp.Left(2));
	recvtemp =recvtemp.Mid(2);
	int CarNum;	
	switch(uCmd)
	{
	case 11:
		{
			int pos=recvtemp.Find(",");
			if (pos >=0)
			{
				float gpsx,gpsy,gpsr;
				CarNum=atoi(recvtemp.Left(pos));
				recvtemp =recvtemp.Mid(pos+1);
				pos=recvtemp.Find(",");
				gpsx =atof(recvtemp.Left(pos));
				recvtemp =recvtemp.Mid(pos+1);
				pos=recvtemp.Find(",");
				gpsy =atof(recvtemp.Left(pos));
				recvtemp =recvtemp.Mid(pos+1);
				gpsr =atof (recvtemp);
				if (OnFindMAP(CarNum)==FALSE)
				{
					m_CarMapN[CarNum]=csocket;
					carimg[CarNum].InitCar(CarNum);
					logz.WriteLog(dwlogID,"新的考车[%d],Port:%d",CarNum,csocket);
				}				
				carimg[CarNum].isOnLine=true;
 				//int tempx=abs(((gpsx-m_Mapx)*m_bs))+176;
 				//int tempy=abs(((gpsy-m_Mapy)*m_bs))+144;
				int tempx=abs(((gpsx-m_Mapx)*m_bs));
 				int tempy=abs(((gpsy-m_Mapy)*m_bs));

				//int tempx=abs(((gpsy-m_Mapx)*m_bs))+176;
				//int tempy=abs(((gpsx-m_Mapy)*m_bs))+144;
				carimg[CarNum].SetXY(tempx,tempy);
				logz.WriteLog(dwlogID,"x=%d,y=%d,,,%d,gpsx=%f,gpsy=%f,m_Mapx=%lf",tempx,tempy,CarNum,gpsx,gpsy,m_Mapx);
//				logz.WriteLog(dwlogID,"x=%d,y=%d,,,%d,gpsx=%f,gpsy=%f,m_Mapx=%lf",tempx,tempy,CarNum,gpsx,gpsy,m_Mapx);
			}
			break;
		}
	default :
		break;
	}
}

void CCDMapDlg::OnTest() 
{
	// TODO: Add your command handler code here
	MAP_INT_SOCKET::iterator iter;
	for (iter=m_CarMapN.begin();iter!=m_CarMapN.end();iter++)
	{		
		TRACE("%d,%d \n",iter->first,iter->second);
	}
	TRACE("End\n");
}

// CUDPServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CUDPServer.h"
#include "CUDPServerDlg.h"

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
// CCUDPServerDlg dialog

CCUDPServerDlg::CCUDPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCUDPServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCUDPServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCUDPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCUDPServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCUDPServerDlg, CDialog)
	//{{AFX_MSG_MAP(CCUDPServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_SOCKUDPZR,OnSocketUDPZR)
	ON_MESSAGE(UM_SOCKUDPZS,OnSocketUDPZS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// CCUDPServerDlg message handlers
//9701
void CCUDPServerDlg::OnSocketUDPZR(WPARAM wParam,LPARAM lParam)//UDP
{
	if ((LOWORD(lParam) & FD_READ)==FD_READ) 
	{
		char *recvbuf=NULL;
		m_Udpzr.RecvFromSock(&recvbuf);
		CString temp;
		temp.Format("%s",recvbuf);
		TRACE("收到车载数据:%s \n",temp);
		//logz.WriteLog(dwlogID,"收到车载数据[%s]",temp);
		//////////////////////////////////////////////////////////////////////////
		FILE *fp=fopen(".\\Data.text","a+");
		fprintf(fp,"%s\n",temp);
		fclose(fp);
		//////////////////////////////////////////////////////////////////////////
		WaitForSingleObject(rListhEvent,50);
		m_rstrList.AddTail(temp);
		SetEvent(rListhEvent);
	}
}

void CCUDPServerDlg::OnSocketUDPZS(WPARAM wParam,LPARAM lParam)//UDP
{
	if ((LOWORD(lParam) & FD_READ)==FD_READ) 
	{
		SOCKETLISTZ tempSock;
		m_Udpzs.RecvFromSockStru(tempSock);
		//TRACE("OnSocketUDPZS,%s,%d\n",tempSock.kchpsu.,tempSock.kchpsu.index);
		m_SockArray.AddTail(tempSock);
	}
}

BOOL CCUDPServerDlg::OnInitDialog()
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
	dwlogID=logz.Register("UDPServer主程序.log");

	m_Udpzr.InitSockU(m_hWnd,9701,UM_SOCKUDPZR);
	m_Udpzs.InitSockU(m_hWnd,9702,UM_SOCKUDPZS);
	
	rListhEvent=NULL;
	sListhEvent=NULL;
	rListhEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	SetEvent(rListhEvent);
	sListhEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	SetEvent(sListhEvent);

	AfxBeginThread(RecvThread,(LPVOID)this);
	AfxBeginThread(SendThread,(LPVOID)this);

	logz.WriteLog(dwlogID,"程序开启.Build 20170704 V1.0.1");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCUDPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCUDPServerDlg::OnPaint() 
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
HCURSOR CCUDPServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

UINT CCUDPServerDlg::RecvThread(LPVOID pParam)
{
	CCUDPServerDlg *pInfo=(CCUDPServerDlg*)pParam;
	BOOL bIsListEmpty;

	while (1)
	{
		WaitForSingleObject(pInfo->rListhEvent,50);
		bIsListEmpty=pInfo->m_rstrList.IsEmpty();
		SetEvent(pInfo->rListhEvent);
		if(!bIsListEmpty)
		{
			TRACE("UpGnssMap \n");
			pInfo->UpGnssMap();
		}
		else
		{
			Sleep(500);
			//continue;	
		}		
	}
	return 0;
}

UINT CCUDPServerDlg::SendThread(LPVOID pParam)
{
	CCUDPServerDlg *pInfo=(CCUDPServerDlg*)pParam;
	BOOL bIsListEmpty;
	SOCKETLISTZ struHead;
	UDPz sendUdp;
	sendUdp.InitSockUC();
	while(1)
	{
		WaitForSingleObject(pInfo->sListhEvent,50);
		bIsListEmpty=pInfo->m_SockArray.IsEmpty();
		SetEvent(pInfo->sListhEvent);
		if(!bIsListEmpty)
		{
			WaitForSingleObject(pInfo->sListhEvent,20);
			struHead=pInfo->m_SockArray.GetHead();
			pInfo->m_SockArray.RemoveHead();
			SetEvent(pInfo->sListhEvent);
			CString strkchp;
			strkchp.Format("%s",struHead.kchpsu.cKchp);
			TRACE("struHead=[%s],socket=%d   %d\n",strkchp,struHead.socketclient,struHead.addrToclient.sin_port);
			MAP_STR_RBODYMSG::iterator iter;	
			iter = pInfo->m_rBodyMap.find(strkchp);	
			if(iter !=  pInfo->m_rBodyMap.end())
			{
				struRBody tempMSG;
				tempMSG=iter->second;
				tempMSG.sKchp.index=struHead.kchpsu.index;
				sendUdp.SendPack((char *)&tempMSG,sizeof(struRBody),struHead);
			}
		}
		else
		{
			Sleep(500);
			continue;
		}
	}
	return 0;
}
//解析
//11,闽A00000,66666.55,55555.66,55.0,555.66,6666.05,#
//strHead.Format("11,闽A00000,1.11,22.22,33.33,444.4,55.55,#");
//#BEGIN,闽A001,2721529.170000,10685456.830000,0.000000,0,0,13.200000,2.200000,0,17-05-12 22:17:11,END&
//20170512 2219 改
//#BEGIN,3,52012,闽A0001,0,2011111,5555555,90,10:10:10,0,268435457,速度,里程,250,2,2,4,经度,纬度,60,0,0,0,航向角,0,0,00001111111111111123,40100,0,0,0,,END&
//20170522 1428 改
void CCUDPServerDlg::UpGnssMap()
{
	CString strHead;
	WaitForSingleObject(rListhEvent,20);
	strHead=m_rstrList.RemoveHead();
	SetEvent(rListhEvent);
	try
	{	
		//TRACE("Replace()=%d\n",strHead.Replace(",",","));
		if(strHead.Replace(",",",")==32)
		{
			CStringArray te;
			DoHaoCString(strHead,te,',');
			//////////////////////////////////////////////////////////////////////////
//			TRACE("te.GetSize()=%d\n",te.GetSize());
// 			for (int i=0;i<te.GetSize();i++)
// 			{
// 				TRACE("%d=%s\n",i,te.GetAt(i));
// 			}
			//////////////////////////////////////////////////////////////////////////
			if(te.GetSize()==33)
			{
				TRACE("%s,%s,%s\n",te.GetAt(3),te.GetAt(2),te.GetAt(3));			
				struRBody temprBody;
				CString tempIndex;
				tempIndex.Format("%s",te.GetAt(3));//车牌			
				strncpy(temprBody.sKchp.cKchp,tempIndex,10);
				temprBody.sGnss.gnssX=atof(te.GetAt(17));
				temprBody.sGnss.gnssY=atof(te.GetAt(18));
				temprBody.sGnss.gnssR=atof(te.GetAt(23));
				temprBody.sGnss.gnssSD=atof(te.GetAt(11));//速度
				temprBody.sGnss.gnssLC=atof(te.GetAt(12));//里程
				strncpy(temprBody.sStu.bStuSFZ,te.GetAt(5),20);
				TRACE("[%s],[%s]\n",te.GetAt(5),temprBody.sStu.bStuSFZ);
				MAP_STR_RBODYMSG::iterator iter;	
				iter = m_rBodyMap.find(tempIndex);	
				if(iter == m_rBodyMap.end())
				{
					m_rBodyMap.insert(pair<CString,struRBody>(tempIndex,temprBody));
				}
				else
				{
					m_rBodyMap[tempIndex]=temprBody;
				}
			}
		}
		else
		{
			logz.WriteLog(dwlogID,"收到的数据分割符[,]个数不正确[%s]",strHead);
		}
	}
	catch (...)
	{
		logz.WriteLog(dwlogID,"UpGnssMap catch...[%s]",strHead);
	}
}

void CCUDPServerDlg::DoHaoCString(CString source,  CStringArray& dest, char division)
{
	dest.RemoveAll();
	int nCount=0;
	for(int i=0;i< source.GetLength();i++)
	{
		if(source.GetAt(i)== division)
		{
			nCount=i;
			dest.Add(source.Left(i)); //去掉右边
			int nIndex=dest.GetSize()-1;
			for(int j=0;j <nIndex;j++)
			{
				dest[nIndex] = dest[nIndex].Right(dest[nIndex].GetLength()-dest[j].GetLength()-1); //去掉左边
			}
		}
	}
	//处理最后一个分隔符division右边的剩余部分。
	dest.Add(source);
	int nIndex2=dest.GetSize()-1;
	for(int j=0;j <nIndex2;j++)
	{
		dest[nIndex2] = dest[nIndex2].Right(dest[nIndex2].GetLength()-dest[j].GetLength()-1); //去掉左边
	}
}

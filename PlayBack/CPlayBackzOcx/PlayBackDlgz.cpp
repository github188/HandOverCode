// PlayBackDlgz.cpp : implementation file
//

#include "stdafx.h"
#include "CPlayBackzOcx.h"
#include "PlayBackDlgz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayBackDlgz dialog


CPlayBackDlgz::CPlayBackDlgz(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayBackDlgz::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayBackDlgz)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPlayBackDlgz::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayBackDlgz)
	DDX_Control(pDX, IDC_SLIDER_Play, m_sliderPlay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayBackDlgz, CDialog)
	//{{AFX_MSG_MAP(CPlayBackDlgz)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_FAST, OnBtnFast)
	ON_BN_CLICKED(IDC_BTN_SLOW, OnBtnSlow)
	ON_BN_CLICKED(IDC_BTN_SOUND, OnBtnSound)
	ON_BN_CLICKED(IDC_BTN_PIC, OnBtnPic)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_Play, OnReleasedcaptureSLIDERPlay)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayBackDlgz message handlers

int CPlayBackDlgz::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 初始化
    NET_DVR_Init();
	m_lPlayBackHandle=-1;
	m_lUserID=-1;
	m_IsPause=FALSE;
	m_bChangeSpeed = FALSE;
	m_bSound=FALSE;
	return 0;
}

BOOL CPlayBackDlgz::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	hbr1=CreateSolidBrush(RGB(60,60,60));
	CString tempPath,strPath;
	tempPath.Format("D:\\Temp");
	DWORD dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	strPath.Format("%s\\%s",tempPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	logfilenamez.Format("%s\\CPlayBackByTimez.log",strPath);
	WriteLogz("InitInstance Build 20171212 0950");
//	MoveUIz();
	SetTimer(1,100,NULL);
	//////////////////////////////////////////////////////////////////////////
// 	fnDVR_Login_V30("192.168.0.92","admin","hk12345678",8000,33);
// 	SetPlayBackTime("2017-12-08 08:00:00","2017-12-08 08:01:20");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayBackDlgz::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		KillTimer(1);
		MoveUIz();
		break;
	case PLAYBYTIME_TIMER:
		{
			DWORD PlayPos;
			NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYGETPOS,0,&PlayPos);
			if(PlayPos>=0 && PlayPos<100)
			{
				m_iNowPos+=1;
				m_sliderPlay.SetPos(m_iNowPos);
			}
			else
			{
				KillTimer(PLAYBYTIME_TIMER);
			}
			break;
		}
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CPlayBackDlgz::OnBtnPlay() 
{
	// TODO: Add your control notification handler code here
	if (m_lUserID==-1)
	{
		AfxMessageBox("设备未登录!无法播放!");
		return ;
	}
	if (m_lPlayBackHandle==-1)//没有播放
	{
		//播放窗口句柄
		HWND hPlayWnd = GetDlgItem(IDC_STATIC_PWND)->GetSafeHwnd();
		m_lPlayBackHandle = NET_DVR_PlayBackByTime(m_lUserID,m_dwChannel,&struStartTime,&struStopTime,hPlayWnd);
		if(-1 == m_lPlayBackHandle)
		{
			WriteLogz("m_lUserID=%d,m_dwChannel=%d,EID=%d",m_lUserID,m_dwChannel,NET_DVR_GetLastError());
			MessageBox("回放失败");
			return;
		}
		
		if(!NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYSTART,0,NULL))
		{
			MessageBox("回放控制失败");
			return;
		}
		m_iNowPos=0;
		SetTimer(PLAYBYTIME_TIMER,1000,NULL);
		SetPlayState(PLAY_NORMAL);
	}
	else
	{
		if(FALSE == m_IsPause) //正在播放，暂停
		{
			if(m_bChangeSpeed)
			{
				NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYNORMAL,0,NULL);
				m_bChangeSpeed = FALSE;				
			}
			NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYPAUSE,0,NULL);
			m_IsPause = TRUE;
			SetPlayState(PLAY_PAUSE);
			KillTimer(PLAYBYTIME_TIMER);
		}
		else                   //暂停，重新播放
		{
			if(m_bChangeSpeed)
			{
                NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYNORMAL,0,NULL);
				m_bChangeSpeed = FALSE;				
			}
            NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYRESTART,0,NULL);
			m_IsPause = FALSE;
			SetPlayState(PLAY_NORMAL);
			SetTimer(PLAYBYTIME_TIMER,1000,NULL);
		}
	}
}

void CPlayBackDlgz::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	if(m_lPlayBackHandle >= 0)
	{
		//停止播放
        NET_DVR_StopPlayBack(m_lPlayBackHandle);
		m_lPlayBackHandle = -1;		
		m_IsPause = FALSE;
		m_bChangeSpeed = FALSE;
		m_sliderPlay.SetPos(0);
        SetPlayState(PLAY_STOP);
	}
}

void CPlayBackDlgz::OnBtnFast() 
{
	// TODO: Add your control notification handler code here
	if(m_lPlayBackHandle >= 0 && !m_IsPause)
	{
		NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYFAST,0,NULL);
		m_bChangeSpeed = TRUE;
	}
}

void CPlayBackDlgz::OnBtnSlow() 
{
	// TODO: Add your control notification handler code here
	if(m_lPlayBackHandle >= 0 && !m_IsPause)
	{
		NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYSLOW,0,NULL);
		m_bChangeSpeed = TRUE;
	}
}

void CPlayBackDlgz::OnBtnSound() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton = NULL; 
	if(!m_bSound)
	{
		if(NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYSTARTAUDIO,0,NULL))
		{
			pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
			pButton->SetIcon(m_hSoundStartIcon);
			// 			GetDlgItem(IDC_BUTTON_REMOTE_SOUND)->EnableWindow(TRUE);
			m_bSound=TRUE;
		}
	}
	else
	{
		if(NET_DVR_PlayBackControl(m_lPlayBackHandle,NET_DVR_PLAYSTOPAUDIO,0,NULL))
		{
			pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
			pButton->SetIcon(m_hSoundStopIcon);
			// 			GetDlgItem(IDC_BUTTON_REMOTE_SOUND)->EnableWindow(TRUE);
			m_bSound=FALSE;
		}
	}
}

void CPlayBackDlgz::OnBtnPic() 
{
	// TODO: Add your control notification handler code here
	
}


void CPlayBackDlgz::SetPlayState(int iState)
{
	CButton *pButton;
    switch(iState)
	{
	case PLAY_STOP:
		pButton = (CButton *)GetDlgItem(IDC_BTN_PLAY);
		pButton->SetIcon(m_hPlayEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_STOP);
		pButton->SetIcon(m_hStopDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_FAST);
		pButton->SetIcon(m_hFastDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_SLOW);
		pButton->SetIcon(m_hSlowDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
		pButton->SetIcon(m_hSoundStopIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_PIC);
		pButton->SetIcon(m_hCaptureIcon);
		pButton->EnableWindow(FALSE);
		break;
	case PLAY_NORMAL:
		pButton = (CButton *)GetDlgItem(IDC_BTN_PLAY);
		pButton->SetIcon(m_hPauseEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_STOP);
		pButton->SetIcon(m_hStopEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_FAST);
		pButton->SetIcon(m_hFastEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_SLOW);
		pButton->SetIcon(m_hSlowEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
		pButton->SetIcon(m_hSoundStopIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BTN_PIC);
		pButton->SetIcon(m_hCaptureIcon);
		pButton->EnableWindow(TRUE);
		break;
	case PLAY_PAUSE:
		pButton = (CButton *)GetDlgItem(IDC_BTN_PLAY);
		pButton->SetIcon(m_hPlayEnableIcon);
		pButton->EnableWindow(TRUE);
		break;
	case PLAY_FAST:
		break;
	case PLAY_SLOW:
		break;
	default:
		break;
	}
}

void CPlayBackDlgz::OnReleasedcaptureSLIDERPlay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_lPlayBackHandle >= 0)
	{
		int pos = m_sliderPlay.GetPos();
		m_sliderPlay.SetPos(pos);
		m_iNowPos=pos;
		UpdateData(FALSE);
		//pos 转成秒 然后加上开始时间
		CTimeSpan TsPos(pos);
		CTime TempTime=m_StartTime+TsPos;
		
		NET_DVR_TIME struTime;
		struTime.dwYear = TempTime.GetYear();
		struTime.dwMonth = TempTime.GetMonth();
		struTime.dwDay =TempTime.GetDay();
		struTime.dwHour = TempTime.GetHour();
		struTime.dwMinute = TempTime.GetMinute();
		struTime.dwSecond = TempTime.GetSecond();
		
		if (NET_DVR_PlayBackControl_V40(m_lPlayBackHandle, NET_DVR_PLAYSETTIME, &struTime, sizeof(struTime), NULL, 0))
		{
			WriteLogz("NET_DVR_PlayBackControl_V40 NET_DVR_PLAYSETTIME faild! %d",NET_DVR_GetLastError());
		}
	}
	*pResult = 0;
}

BOOL CPlayBackDlgz::SetPlayBackTime(LPCTSTR strSTime,LPCTSTR strETime)
{
	if (StrToDVRTime(struStartTime,strSTime)==FALSE)
	{
		return FALSE;
	}
	if (StrToDVRTime(struStopTime,strETime)==FALSE)
	{
		return FALSE;
	}
	
	CTime cTime1(struStartTime.dwYear,struStartTime.dwMonth,struStartTime.dwDay,struStartTime.dwHour,struStartTime.dwMinute,struStartTime.dwSecond);
	m_StartTime=cTime1;

	CTime cTime2(struStopTime.dwYear,struStopTime.dwMonth,struStopTime.dwDay,struStopTime.dwHour,struStopTime.dwMinute,struStopTime.dwSecond);
	m_StopTime=cTime2;

	CTimeSpan tmSpan =m_StopTime - m_StartTime;
	
	m_dwTotalTime = tmSpan.GetTotalSeconds();
	m_sliderPlay.SetRange(0,m_dwTotalTime);
	WriteLogz("m_dwTotalTime=%u,%d",m_dwTotalTime,m_dwTotalTime);
	return TRUE;
}

BOOL CPlayBackDlgz::StrToDVRTime(NET_DVR_TIME &rTime, LPCTSTR lTime)
{
	CString temp;
	temp.Format("%s",lTime);	
	if ( 19 != temp.GetLength() )
	{
		return FALSE;
	}
	rTime.dwYear   = atoi(temp.Mid(0,4));
	rTime.dwMonth  = atoi(temp.Mid(5,2));
	rTime.dwDay    = atoi(temp.Mid(8,2));
	rTime.dwHour   = atoi(temp.Mid(11,2));
	rTime.dwMinute = atoi(temp.Mid(14,2));
	rTime.dwSecond = atoi(temp.Mid(17,2));
	return TRUE;
}

void CPlayBackDlgz::WriteLogz(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	CString Data;
	SYSTEMTIME st;
	GetLocalTime(&st);
	Data.Format("%02d:%02d:%02d.%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	FILE *fp=fopen(logfilenamez,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

void CPlayBackDlgz::MoveUIz()
{
	CRect ClientRC;
	GetClientRect(&ClientRC);
	ClientRC.OffsetRect(PLAYWNDL,PLAYWNDT);
	GetDlgItem(IDC_STATIC_PWND)->MoveWindow(ClientRC.left,ClientRC.top,ClientRC.right-6,ClientRC.bottom-80);
	GetDlgItem(IDC_SLIDER_Play)->MoveWindow(ClientRC.left,ClientRC.bottom-76,ClientRC.right-10,30);

	CRect tempRc;
	int iLeftSpan;
	iLeftSpan=50;
	tempRc.SetRect(40,ClientRC.bottom-40,80,ClientRC.bottom-10);
	GetDlgItem(IDC_BTN_PLAY)->MoveWindow(tempRc.left,tempRc.top,tempRc.Width(),tempRc.Height());
	GetDlgItem(IDC_BTN_STOP)->MoveWindow(tempRc.left+iLeftSpan*1,tempRc.top,tempRc.Width(),tempRc.Height());
	GetDlgItem(IDC_BTN_FAST)->MoveWindow(tempRc.left+iLeftSpan*2,tempRc.top,tempRc.Width(),tempRc.Height());
	GetDlgItem(IDC_BTN_SLOW)->MoveWindow(tempRc.left+iLeftSpan*3,tempRc.top,tempRc.Width(),tempRc.Height());
	GetDlgItem(IDC_BTN_PIC)->MoveWindow(tempRc.left+iLeftSpan*4,tempRc.top,tempRc.Width(),tempRc.Height());
	GetDlgItem(IDC_BTN_SOUND)->MoveWindow(tempRc.left+iLeftSpan*5,tempRc.top,tempRc.Width(),tempRc.Height());


	// 初始化按钮
	m_hPlayEnableIcon =			AfxGetApp()->LoadIcon(IDI_PLAY_ENABLE);
	m_hPlayDisableIcon =		AfxGetApp()->LoadIcon(IDI_PLAY_DISABLE);
	m_hPauseEnableIcon =		AfxGetApp()->LoadIcon(IDI_PAUSE_ENABLE);
	m_hPauseDisableIcon =		AfxGetApp()->LoadIcon(IDI_PAUSE_DISABLE);
	m_hStopEnableIcon =			AfxGetApp()->LoadIcon(IDI_STOP_ENABLE);
	m_hStopDisableIcon =		AfxGetApp()->LoadIcon(IDI_STOP_DISABLE);
	m_hFastDisableIcon =		AfxGetApp()->LoadIcon(IDI_FASTFORWARD_DISABLE);
	m_hFastEnableIcon =			AfxGetApp()->LoadIcon(IDI_FASTFORWARD_ENABLE);
	m_hSlowDisableIcon=			AfxGetApp()->LoadIcon(IDI_FASTBACKWARD_DISABLE);
	m_hSlowEnableIcon =			AfxGetApp()->LoadIcon(IDI_FASTBACKWARD_ENABLE);
	m_hSoundStartIcon=			AfxGetApp()->LoadIcon(IDI_SOUND_ENABLE);
	m_hSoundStopIcon=			AfxGetApp()->LoadIcon(IDI_SOUND_DISABLE);
	m_hCaptureIcon	=			AfxGetApp()->LoadIcon(IDI_PIC);
	CButton *pButton;
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_PLAY);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_STOP);
	pButton->SetIcon(m_hStopDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_FAST);
	pButton->SetIcon(m_hFastDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_SLOW);
	pButton->SetIcon(m_hSlowDisableIcon);
	pButton->EnableWindow(FALSE);

	pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
	pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(FALSE);

	pButton = (CButton *)GetDlgItem(IDC_BTN_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(FALSE);
}

BOOL CPlayBackDlgz::fnDVR_Login_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel)
{
 	if (m_lUserID!=-1)
	{
		//注销用户
		NET_DVR_Logout(m_lUserID);
		m_lUserID=-1;
	}	
	//登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0;					//同步登录方式
    strcpy(struLoginInfo.sDeviceAddress,strDvrIP); //设备IP地址
    struLoginInfo.wPort = (WORD)nPort;							//设备服务端口
    strcpy(struLoginInfo.sUserName,strUseName);			//设备登录用户名
    strcpy(struLoginInfo.sPassword,strPwd);		//设备登录密码
	
    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
	
    m_lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (m_lUserID < 0)
    {
		WriteLogz("登录设备失败!IP=%s,Port=%d ,EID=%d",strDvrIP,nPort,NET_DVR_GetLastError());
		return FALSE;
    }
	m_dwChannel=nChannel;
	return TRUE;
}

HBRUSH CPlayBackDlgz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG || pWnd->GetDlgCtrlID()==IDC_SLIDER_Play || pWnd->GetDlgCtrlID()==IDC_STATIC_PWND)
	{
		return hbr1;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPlayBackDlgz::OnStartPlay()
{
	OnBtnPlay();
}

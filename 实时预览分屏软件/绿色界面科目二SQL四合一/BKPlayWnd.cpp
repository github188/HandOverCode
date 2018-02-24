// BKPlayWnd.cpp: implementation of the CBKPlayWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "play.h"
#include "BKPlayWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBKPlayWnd::CBKPlayWnd()
{
	ISPlaywndd	=FALSE;
	m_bIsRecording = FALSE;
	dwlogid=logz.Register("CBKPlayWnd.log");
}

CBKPlayWnd::~CBKPlayWnd()
{
	DvrLoginout();
}

BOOL CBKPlayWnd::DvrInitLogin(HWND hWnd, char *sDVRIP, char *sUserName, char *sPassword, WORD wDVRPort)
{
	NET_DVR_Init();// 初始化
// 	//设置连接时间与重连时间
// 	NET_DVR_SetConnectTime(2000, 1);
 	NET_DVR_SetReconnect(10000, true);
	
	m_lUserID = NET_DVR_Login_V30(sDVRIP,wDVRPort,sUserName,sPassword, &struDeviceInfo);
	m_zhwnd =hWnd;
	if (  m_lUserID == -1 )
	{
		GetLastErrorz();
		NET_DVR_Cleanup();
		return FALSE;
	}
// 	//设置异常消息回调函数
// 	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
	logz.WriteLog(dwlogid,"登录成功!%s,%d",sDVRIP,wDVRPort);
	return TRUE;
}
BOOL CBKPlayWnd::DvrLoginout()
{
	//关闭预览
	if (ISPlaywndd)
	{
		if(m_bIsRecording)  //正在录像，先停止
		{
			DvrStopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle);
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		ISPlaywndd	=FALSE;
	}
	return TRUE;
}
BOOL CBKPlayWnd::DvrRealPlay(LONG lChannel)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = m_zhwnd;
	ClientInfo.lChannel     = lChannel;
	ClientInfo.lLinkMode    = 0;//0x80000000;//子码流 TCP
    ClientInfo.sMultiCastIP = NULL;

	m_lPlayHandle= NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle)
	{
		GetLastErrorz();
		logz.WriteLog(dwlogid,"NET_DVR_RealPlay_V30 faild!NET_DVR_GetLastError=[%d],lChannel=%d",NET_DVR_GetLastError(),lChannel);
		return FALSE;
	}
	return TRUE;
}

void CBKPlayWnd::Drawztext(CString zstr)
{
	HDC hdc=GetDC(m_zhwnd);
	CRect rc;
	GetClientRect(m_zhwnd,&rc);
	DrawText(hdc,zstr,strlen(zstr),&rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CBKPlayWnd::GetLastErrorz()
{
	switch (NET_DVR_GetLastError())
	{
	case NET_DVR_PASSWORD_ERROR://1
		Drawztext("用户名密码错误,请查看配置文件先!");
		break;
	case NET_DVR_NOINIT://3
		Drawztext("SDK未初始化!");
		break;
	case NET_DVR_OVER_MAXLINK://5
		Drawztext("连接到设备的用户个数超过最大!");
		break;
	case NET_DVR_NETWORK_FAIL_CONNECT://7
		Drawztext("连接设备失败.设备不在线或网络原因引起的连接超时!");
		break;
	case NET_DVR_NETWORK_SEND_ERROR://8
		Drawztext("向设备发送失败!");
		break;
	case NET_DVR_MAX_USERNUM://52
		Drawztext("登录设备的用户数达到最大!");
		break;
	case NET_DVR_BINDSOCKET_ERROR://72
		Drawztext("绑定套接字失败!");
		break;
	case NET_DVR_SOCKETCLOSE_ERROR://73
		Drawztext("socket连接中断,此错误通常是由于连接中断或目的地不可达!");
		break;
	case NET_DVR_IPCHAN_NOTALIVE://83
		Drawztext("预览时外接IP通道不在线!");
		break;
	case NET_DVR_RTSP_SDK_ERROR://84
		Drawztext("加载标准协议通讯库StreamTransClient失败!");
		break;
	case NET_DVR_CONVERT_SDK_ERROR://85
		Drawztext("加载转封装库失败!");
		break;
	default:
		Drawztext("实时预览播放出错!");
		break;
	}
}

BOOL CBKPlayWnd::DvrStartRecord(CString str)
{
	if(m_lPlayHandle == -1)
	{
		return FALSE;
	}
	if(m_bIsRecording)
	{
		DvrStopRecord();
	}
	char RecName[256] = {0};
	sprintf(RecName,"%s.mp4",str);
	if(!NET_DVR_SaveRealData(m_lPlayHandle,RecName))
	{
		return FALSE;
	}
	m_bIsRecording = TRUE;
	return TRUE;
}

BOOL CBKPlayWnd::DvrStopRecord()
{
	if(!NET_DVR_StopSaveRealData(m_lPlayHandle))
	{
		return FALSE;
	}
	m_bIsRecording = FALSE;
	return TRUE;
}
//打开视频1声音
BOOL CBKPlayWnd::OpenVoice()
{
	if (!m_VoiceState)
	{
		if ( !NET_DVR_OpenSound( m_lPlayHandle ) )
		{
			m_VoiceState = FALSE;
			CString str;
			str.Format("打开声音失败!!![ M_Handle=%ld,Errid=%d ]",m_lPlayHandle,NET_DVR_GetLastError());
			AfxMessageBox(str);		
		}
		else
		{
			m_VoiceState = TRUE;			
			return TRUE;
		}
	}
	else
	{
		if( !NET_DVR_CloseSound() )
		{			
			CString str;
			str.Format("关闭声音失败!!![ M_Handle=%ld,Errid=%d ]",m_lPlayHandle,NET_DVR_GetLastError());
			if (NET_DVR_GetLastError() == 51)
			{
				m_VoiceState = FALSE;
			}
			else
			{
				AfxMessageBox(str);
			}
		}
		else
		{
			m_VoiceState = FALSE;
			return FALSE;
		}
	}
	return FALSE;
}

//关闭声音
BOOL CBKPlayWnd::CloseVoice()
{
	m_VoiceState = FALSE;
	return FALSE;
}

//语音对讲
BOOL CBKPlayWnd::VoiceIntercom()
{
	if (m_bVoiceCom)
	{
		if ( !NET_DVR_StopVoiceCom(m_lVocieComHandle) )
		{			
			CString str;
			str.Format("停止对讲失败!!![ UserID=%ld,StartDTalkChan=%ld,m_lChannel=%ld,Voice_Handle=%ld,Errid=%d ]",m_lUserID,m_byStartDTalkChan,m_lChannel,m_lVocieComHandle,NET_DVR_GetLastError());
			AfxMessageBox(str);
		}
		m_bVoiceCom=false;	
		return FALSE;
	}
	else
	{	
		m_lVocieComHandle = NET_DVR_StartVoiceCom_V30(m_lUserID,m_byStartDTalkChan+m_lChannel,1,NULL,NULL);
		if ( m_lVocieComHandle == -1 )
		{
			CString str;
			str.Format("打开对讲失败!!![ UserID=%ld,StartDTalkChan=%ld,m_lChannel=%ld,Voice_Handle=%ld,Errid=%d ]",m_lUserID,m_byStartDTalkChan,m_lChannel,m_lVocieComHandle,NET_DVR_GetLastError());
			AfxMessageBox(str);			
			return FALSE;
		}
		else
		{		
			m_bVoiceCom=true;
			return TRUE;
		}			
	}
	return FALSE;
}

//关闭语音标志位
BOOL CBKPlayWnd::CloseVoiceCom()
{
	m_bVoiceCom = FALSE;
	return FALSE;
}

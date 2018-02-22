// HCNETSDKz.cpp: implementation of the CHCNETSDKz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chikvisionz.h"
#include "HCNETSDKz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCNETSDKz::CHCNETSDKz()
{
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
	logfilenamez.Format("%s\\CHCNETSDKz.log",strPath);

	if (!NET_DVR_Init())// 初始化
	{
		WriteLogz("NET_DVR_Init初始化海康SDK失败!Eid=%d",NET_DVR_GetLastError());
 	}
	TerminalIP[0] ='\0';
	RasUser[0]	  ='\0';
	RasPassword[0]='\0';
	TerminalPort  =8000;
	dwChannel     =-1;
	m_lUserID     =-1;
	lPlayHandle   =-1;
	lPlayBackHandle=-1;

	m_TlUserID=-1;
	m_lVoiceHandle=-1;
}

CHCNETSDKz::~CHCNETSDKz()
{
	NET_DVR_Cleanup();
}

void CHCNETSDKz::WriteLogz(LPCTSTR pstrFormat, ...)
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

BOOL CHCNETSDKz::fnDVR_Login_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel)
{
	if (strcmp(TerminalIP,strDvrIP)==0)
	{	//IP相等
		if (TerminalPort==nPort)
		{	//端口相等
			//同一设备不用重复登录
			if (m_lUserID>=0)
			{
				dwChannel=nChannel;		//通道保存
				return TRUE;
			}
		}
	}	
	if (m_lUserID>=0)
	{
		fnStopPlay();//先关闭播放	
		NET_DVR_Logout(m_lUserID);//退出登录
		m_lUserID=-1;
	}
	strncpy(TerminalIP,strDvrIP,16);
	strncpy(RasUser,strUseName,16);
	strncpy(RasPassword,strPwd,16);
	TerminalPort=(WORD)nPort;
	dwChannel=nChannel;	
	
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//存放设备参数的结构体
	m_lUserID = NET_DVR_Login_V30((LPSTR)strDvrIP,(WORD)nPort,(LPSTR)strUseName,(LPSTR)strPwd, &struDeviceInfo);
	if (m_lUserID < 0)
	{
		WriteLogz("登录设备失败!IP=%s,Port=%d ,EID=%d",strDvrIP,nPort,NET_DVR_GetLastError());
		return FALSE;
	}
	return TRUE;
}

void CHCNETSDKz::fnStopPlay()
{
	if (lPlayHandle !=-1)
	{
		//先关闭播放
		if(!NET_DVR_StopRealPlay(lPlayHandle))
		{
			//失败了
		}
		lPlayHandle=-1;
	}
	//////////////////////////////////////////////////////////////////////////
	if (lPlayBackHandle !=-1)
	{
		//先关闭回放
		if(!NET_DVR_StopPlayBack(lPlayBackHandle))
		{
			//失败了
		}
		lPlayBackHandle=-1;
	}
	//////////////////////////////////////////////////////////////////////////
//	return TRUE;
}




BOOL CHCNETSDKz::fnRealPlay()
{
	fnStopPlay();//先关闭播放	
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = m_hwndz;
	ClientInfo.lChannel     = dwChannel;
	ClientInfo.lLinkMode    = 0;//0x80000000;//子码流 TCP
    ClientInfo.sMultiCastIP = NULL;
	
	lPlayHandle= NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == lPlayHandle)
	{
		CString logstr;
		logstr.Format("开启实时预览失败!Eid=%d",NET_DVR_GetLastError());
		::PostMessage(m_hwndz,m_wMSG,WPARAM(9),LPARAM(new CString(logstr)));
		WriteLogz(logstr);
		return FALSE;
	}
	return TRUE;
}

void CHCNETSDKz::InitHwnd(HWND hwndz, int wMSG)
{
	m_hwndz=hwndz;
	m_wMSG =wMSG;
	//::PostMessage(m_hwndz,m_wMSG,WPARAM(m_itf),LPARAM(new CString(logstr)));
}

BOOL CHCNETSDKz::fnStartVoiceCom_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel)	//开启对讲
{	
	fnStopVoiceCom();//先停止对讲
	//登录前端 然后 对讲
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//存放设备参数的结构体
	m_TlUserID = NET_DVR_Login_V30((LPSTR)strDvrIP,(WORD)nPort,(LPSTR)strUseName,(LPSTR)strPwd, &struDeviceInfo);
	if (m_TlUserID < 0)
	{
		WriteLogz("登录对讲设备失败!IP=%s,Port=%d ,EID=%d",strDvrIP,nPort,NET_DVR_GetLastError());
		return FALSE;
	}
	//struDeviceInfo.byStartDTalkChan;//对讲起始通道
	DWORD dwVoiceChanNo = 0;
	if (nChannel >32 )
	{//struDeviceInfo.byAudioChanNum
		dwVoiceChanNo=struDeviceInfo.byStartDTalkChan+nChannel-33;
	}
	else
	{
		dwVoiceChanNo=struDeviceInfo.byStartDTalkChan+nChannel;
	}
	m_lVoiceHandle= NET_DVR_StartVoiceCom_V30(m_TlUserID,dwVoiceChanNo,1,NULL,NULL);
	if (m_lVoiceHandle ==-1)
	{
		WriteLogz("180131 13:41打开对讲失败!EID=%d,byStartDTalkChan=%d,byAudioChanNum=%d,dwVoiceChanNo=%d",NET_DVR_GetLastError(),struDeviceInfo.byStartDTalkChan,struDeviceInfo.byAudioChanNum,dwVoiceChanNo);
		return FALSE;
	}
	return TRUE;
}

void CHCNETSDKz::fnStopVoiceCom()		//停止对讲
{
	if (m_lVoiceHandle!=-1)
	{
		if (!NET_DVR_StopVoiceCom(m_lVoiceHandle))
		{
			WriteLogz("停止对讲失败!%d",NET_DVR_GetLastError());
		}
		m_lVoiceHandle=-1;
	}
	if (m_TlUserID !=-1)
	{
		NET_DVR_Logout_V30(m_TlUserID);
		m_TlUserID=-1;
	}
}

BOOL CHCNETSDKz::fnOpenSoundShare()
{
	if (lPlayHandle !=-1)
	{
		if(!NET_DVR_SetAudioMode(2))
		{
			WriteLogz("设备声音共享NET_DVR_SetAudioMode faild! Errid=%d",NET_DVR_GetLastError());
		}
		if(!NET_DVR_OpenSoundShare(lPlayHandle))
		{
			WriteLogz("打开声音失败! Errid=%d",NET_DVR_GetLastError());
			return FALSE;
		}
		m_bPlaySound=true;
		return TRUE;
	}
	WriteLogz("lPlayHandle ==-1 打开声音失败! Errid=%d",NET_DVR_GetLastError());
	return FALSE;
}

BOOL CHCNETSDKz::fnCloseSoundShare()
{
	if (lPlayHandle !=-1)
	{
		if (m_bPlaySound)//声音被打开
		{
			if(!NET_DVR_CloseSoundShare(lPlayHandle))
			{
				WriteLogz("关闭声音失败! Errid=%d",NET_DVR_GetLastError());
			}
			else
			{
				m_bPlaySound=false;
			}
		}
	}
	return FALSE;
}

BOOL CHCNETSDKz::fnPBByTime(LPCTSTR strSTime,LPCTSTR strETime,int nWndID)
{
	NET_DVR_TIME struStartTime, struStopTime;
	if (StrToDVRTime(struStartTime,strSTime)==FALSE)
	{
		return FALSE;
	}
	if (StrToDVRTime(struStopTime,strETime)==FALSE)
	{
		return FALSE;
	}
	fnStopPlay();
	//////////////////////////////////////////////////////////////////////////
	lPlayBackHandle = NET_DVR_PlayBackByTime(m_lUserID,dwChannel,&struStartTime,&struStopTime,m_hwndz);
	if(-1 == lPlayBackHandle)
	{
		CString temp;
		temp.Format("按时间回放失败!Eid=%d",NET_DVR_GetLastError());
		AfxMessageBox(temp);
		WriteLogz(temp);
		return FALSE;
	}
	if(!NET_DVR_PlayBackControl(lPlayBackHandle,NET_DVR_PLAYSTART,0,NULL))
	{
		WriteLogz("NET_DVR_PlayBackControl faild! Errid=%d",NET_DVR_GetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CHCNETSDKz::StrToDVRTime(NET_DVR_TIME &rTime, LPCTSTR lTime)
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
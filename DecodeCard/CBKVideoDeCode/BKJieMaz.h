// BKJieMaz.h: interface for the CBKJieMaz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BKJIEMAZ_H__A6D43A05_B185_465B_BF25_93BA1752F9B5__INCLUDED_)
#define AFX_BKJIEMAZ_H__A6D43A05_B185_465B_BF25_93BA1752F9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////////
#include "DataType.h"
#include "HikVisionSdk.h"
#pragma comment(lib,"DS40xxSDK.lib")

#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")
//////////////////////////////////////////////////////////////////////////
#define MAX_DISCHANNEL 16
#define MAX_CHANNELS 100
#define MAX_OSD_BYTES 88
//#define CP_GB2312		936

typedef struct STRU_DECODE_CHAN 
{
	BOOL		isCard;//是否是车载视频
	BOOL		bDecPlay;//是否在解码播放
	HANDLE		hChannelHandle;//解码通道句柄
	int m_Num;
	BOOL	bRealPlay;//
	LONG	lPlayHandle;//
	STRU_DECODE_CHAN()
	{
		m_Num=0;
		bDecPlay=FALSE;
		bRealPlay=FALSE;
	}
	DISPLAY_PARA dsPara;//视频显示参数 2014-06-12
}DECODE_CHAN;

typedef struct tagUSER_CHANNEL_INFO
{
	LONG lDvrUserID;						//DVR用户ID，是NET_DVR_Login的返回值
	LONG lRealPlay;							//播放句柄，是NET_DVR_RealPlay的返回值
	LONG lChannel; 							//通道号：1-32为模拟通道，33-64为IP通道
	LONG lLinkMode;							//最高位(31)为0表示主码流，为1表示子码流，0－30位表示码流连接方式：0-TCP,1-UDP,2-MCAST,3-RTP,4-音视频分开

}USER_CHANNEL_INFO, *PUSER_CHANNEL_INFO;

typedef struct tagUSER_SERVER_INFO
{
	LONG lDvrUserID;						//DVR用户ID，是NET_DVR_Login的返回值
	CHAR sDvrIP[16];						//DVR的IP地址
	CHAR sDvrUserName[16];					//DVR用户名
	CHAR sDvrPassword[16];					//DVR的密码
	DWORD dwDvrPort;						//DVR端口号
	BYTE byChanNum;				    //模拟通道个数
    BYTE byStartChan;			        //起始通道号,例如DVS-1,DVR - 1
}USER_SERVER_INFO;

class CBKJieMaz  
{
public:
	CBKJieMaz();
	virtual ~CBKJieMaz();
public:
	UINT OsdSD[MAX_DISCHANNEL][91];//速度
	UINT OsdSC[MAX_DISCHANNEL][91];//时长
	BOOL DvrSetDisPOSD(UINT disChannel,CString mStrs,UINT line);//显示通道叠加字符
	BOOL TransMultiByteToWideByte(UINT *pFormat, UINT *pFormatOut);
	//////////////////////////////////////////////////////////////////////////
	void DvrPlayStopXM(UINT Channel);
	void F_DVR_Logoutz();
	BOOL DvrPlayChannel(LONG lChannel,CString dIP,UINT jiechannel,BOOL cz=FALSE);
	DWORD F_Login(LPTSTR lpIP, DWORD dwPort, LPTSTR lpUserName, LPTSTR lpUserPassword);
	void SetDispChanLogo(UINT nChannel,BOOL m_bDisLogoEnable=TRUE);
	BOOL SetDisRGN();
	void ChannelClosez();
	BOOL InitDsp(HWND hParent);
	UINT		g_nTotalDecChannels/* = 0*/;			 //解码通道数
	UINT		g_nTotalDspCnt/* = 0*/;
	UINT		g_nTotalDispChannels/* = 0*/;			 //显示通道数
	UINT		g_nDevCount/* = 0*/; //登录设备个数
};

#endif // !defined(AFX_BKJIEMAZ_H__A6D43A05_B185_465B_BF25_93BA1752F9B5__INCLUDED_)

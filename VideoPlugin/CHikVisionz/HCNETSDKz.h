// HCNETSDKz.h: interface for the CHCNETSDKz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCNETSDKZ_H__7C726155_1FD7_46EA_86C9_FC29F05BF7E6__INCLUDED_)
#define AFX_HCNETSDKZ_H__7C726155_1FD7_46EA_86C9_FC29F05BF7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")

/*
typedef struct _terminalpz
{ 

	int  imllx;				//码流类型 0 主码流 1 子码流

	LONG lPlayBackHandle;   //回放句柄
	_terminalpz()
	{
		TerminalIP[0] ='\0';
		RasUser[0]	  ='\0';
		RasPassword[0]='\0';
		TerminalPort  =8000;
		dwChannel     =-1;
		imllx=1;
		m_lUserID     =-1;
		lPlayHandle   =-1;
		lPlayBackHandle =-1;
	}
}STerminalPZ;*/

class CHCNETSDKz  
{
public:
	BOOL fnPBByTime(LPCTSTR strSTime,LPCTSTR strETime,int nWndID);
	BOOL fnOpenSoundShare();
	BOOL fnCloseSoundShare();

	void InitHwnd(HWND hwndz,int wMSG);//初始化
	BOOL fnDVR_Login_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel);
	BOOL fnRealPlay();
	void fnStopPlay();
	//////////////////////////////////////////////////////////////////////////
	BOOL fnStartVoiceCom_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel);	//开启对讲
	void fnStopVoiceCom();		//停止对讲
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CHCNETSDKz();
	virtual ~CHCNETSDKz();
private:
	BOOL StrToDVRTime(NET_DVR_TIME &rTime, LPCTSTR lTime);
	//////////////////////////////////////////////////////////////////////////
	HWND m_hwndz;
	int  m_wMSG;
	//////////////////////////////////////////////////////////////////////////
	CString logfilenamez;
	void WriteLogz(LPCTSTR pstrFormat, ...);
	//////////////////////////////////////////////////////////////////////////
	LONG m_lUserID;			//登录ID
	LONG lPlayHandle;		//播放句柄
	LONG lPlayBackHandle;   //回放句柄
	char TerminalIP[16];	//设备IP
	char RasUser[16];		//用户名
	char RasPassword[16];	//密码
	WORD TerminalPort;		//端口号
	WORD dwChannel;			//通道号

	LONG m_TlUserID;        //对讲登录ID
	LONG m_lVoiceHandle;    //对讲返回句柄

	bool m_bPlaySound;
};

#endif // !defined(AFX_HCNETSDKZ_H__7C726155_1FD7_46EA_86C9_FC29F05BF7E6__INCLUDED_)

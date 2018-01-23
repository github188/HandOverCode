// VideoInterFace.h: interface for the VideoInterFace class.
//
//////////////////////////////////////////////////////////////////////
#include "HCNetSDK.h"  //SDK的头文件  主要定义NET_DVR_DEVICEINFO_V30这个变量

#if !defined(AFX_VIDEOINTERFACE_H__46728AF6_F0B8_4370_91EC_4C41250F717C__INCLUDED_)
#define AFX_VIDEOINTERFACE_H__46728AF6_F0B8_4370_91EC_4C41250F717C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



typedef void(CALLBACK *fRealDataCallBack_V30)(
											  LONG      lRealHandle,
											  DWORD     dwDataType,
											  BYTE      *pBuffer,
											  DWORD     dwBufSize,
											  void      *pUser
											  );




class VideoInterFace : public CDialog 
{
public:
	int CloseSound(DWORD dwMode);
	int OpenSound(DWORD dwMode, WORD wVolume);
	int InitNET_DVR_AUDIOSTREAMDATA();
	void Drawztext(CString zstr);  //用来显示异常代码
	BOOL stopvideotape();
	int stopplay();
	BOOL startvideotape();  //开始录像
	BOOL NET_DVR_LOGIN(char *DVRIP,WORD Port,char *Name,char *Password,HWND pHwnd);
	int play(HWND pHwnd,int iChannel);
	LONG lUserId;
	char Dev_cfg[256];
	VideoInterFace();
	virtual ~VideoInterFace();

private:
	void GetLastErrorz();  // 捕获异常码

	int m_iCurChanIndex; //当前通道号
	BOOL VideoRunning;	//true 是正在浏览视频 false 是没有在播放视频
	HWND pHwnd;
	char * Password;
	char * UserName;
	WORD Port;
	char * DVRIP;
	LONG m_lPlayHandle_record;  //是NET_DVR_StopRealPlay等函数的句柄参数
	BOOL isRecording; //正在录制视频
	NET_DVR_DEVICEINFO_V30 struLocalDeviceInfo;
};






#endif // !defined(AFX_VIDEOINTERFACE_H__46728AF6_F0B8_4370_91EC_4C41250F717C__INCLUDED_)


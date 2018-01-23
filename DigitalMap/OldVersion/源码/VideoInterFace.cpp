// VideoInterFace.cpp: implementation of the VideoInterFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "VideoInterFace.h"
#include "HCNetSDK.h"
#pragma comment(lib,"DsSdk.lib")
#pragma comment(lib,"GdiPlus.lib")
#pragma comment(lib,"HCNetSDK.lib")
#pragma comment(lib,"PlayCtrl.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



struct RIFF_HEADER//文件类型
{
	char szRiffID[4]; // 'R','I','F','F'
	DWORD dwRiffSize;//DWORD最大可容纳4GB的文件大小=FILE_LENGTH-8(即减去此数据结构大小）
	char szRiffFormat[4]; // 'W','A','V','E'
};
struct WAVE_FORMAT
{
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
};
struct FMT_BLOCK//格式
{
	char szFmtID[4]; // 'f','m','t',' ' 
	DWORD dwFmtSize;//////////////一般情况下为16，如有附加信息为18
	WAVE_FORMAT wavFormat;
};
struct FACT_BLOCK//可先项 一般可不用
{
	char szFactID[4]; // 'f','a','c','t'
	DWORD dwFactSize;
};
struct DATA_BLOCK//数据头
{
	char szDataID[4]; // 'd','a','t','a'
	DWORD dwDataSize;//数据长度
};


FILE* file = NULL;	
RIFF_HEADER riff;
WAVE_FORMAT wform;
FMT_BLOCK fmt;
FACT_BLOCK fact;
DATA_BLOCK data;
BYTE buff[1024*256];//声音数据

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VideoInterFace::VideoInterFace()
{
	m_lPlayHandle_record = -1;  //是NET_DVR_StopRealPlay等函数的句柄参数
	m_iCurChanIndex = 0;		//选中的通道号
	VideoRunning = FALSE;		//是否正在浏览视频
	isRecording = FALSE;		//是否正在录制视频
}

VideoInterFace::~VideoInterFace()
{

}


/*************************************************
函数描述:登陆设备
参数说明
DVRIP IP地址
Port  端口号
Name  用户名 
Password 密码
pHwnd  句柄
返回值 true 登陆成功
		false 登陆失败
**************************************************/
BOOL VideoInterFace::NET_DVR_LOGIN(char *DVRIP, WORD pPort, char *pName, char *Password, HWND pHwnd)
{
	this->DVRIP = new char[strlen(DVRIP) + 1];
	strcpy(this->DVRIP,DVRIP);
	this->UserName = new char[strlen(pName) + 1];
	strcpy(this->UserName,pName);
	this->Password = new char[strlen(Password) + 1];
	strcpy(this->Password,Password);
	this->Port = pPort;
	this->pHwnd = pHwnd;

	//初始化
	NET_DVR_Init();
	//设置连接时间与重连时间 
	NET_DVR_SetConnectTime(2000, 1); 
	NET_DVR_SetReconnect(10000, true); 
	
	//连接设备
	char *pAddress = "192.168.0.173";
	WORD Port =8000 ;
	char Name[20] = {"admin"};
	char password[20] = {"12345"};
//	NET_DVR_DEVICEINFO_V30 struLocalDeviceInfo;
	memset(&struLocalDeviceInfo,0,sizeof(struLocalDeviceInfo));
	lUserId = NET_DVR_Login_V30(DVRIP,(WORD)Port,Name,password,&struLocalDeviceInfo);
	
	if (lUserId == NET_DVR_NOERROR) //连接成功
	{
		
		//	SetDlgItemText(IDC_BUTTON_Login,"123");
		MessageBox("连接成功");
		return true;
	}
	else
	{
		GetLastErrorz();
//		CString err;
//		err.Format("错误代码：%d",lUserId);
//		MessageBox(err);
		return false;
	}

	
}



/*************************************************
函数说明 实时预览的回调函数

*************************************************/
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD 
dwBufSize,void* dwUser) 
{ 
	BOOL bRet = FALSE;
	int i = 0;
	HWND hWnd = NULL;
	CString tmp;
	tmp.Format("datatype = %d, dwbufsize = %d", dwDataType, dwBufSize);
	OutputDebugString(tmp);
	
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD:

/*
		if (PLAYBYNAME == dwUser)
		{
			PlayM4_GetPort(&g_pMainDlg->m_lPlayPort);
			hWnd = g_pMainDlg->m_FileWnd;
		}
		
		bRet = PlayM4_SetOverlayMode(g_pMainDlg->m_lPlayPort, FALSE, COLORREF(0));//not play on overplay
		bRet = PlayM4_SetStreamOpenMode(g_pMainDlg->m_lPlayPort, STREAME_FILE);
		
		if (dwBufSize > 0)
		{
			bRet = PlayM4_OpenStream(g_pMainDlg->m_lPlayPort, pBuffer, dwBufSize, 600*1000);//SOURCE_BUF_MIN50*1000
			if (bRet)
			{
				PlayM4_Play(g_pMainDlg->m_lPlayPort, hWnd);
			}
		}
*/
		break;
	case NET_DVR_AUDIOSTREAMDATA:

		if (dwBufSize > 0 )
		{
			fwrite((BYTE *)&buff,sizeof(BYTE),dwBufSize,file);//写音频数据
		}
/*
		if (dwBufSize > 0 && g_pMainDlg->m_lPlayPort >=0)
		{
			PlayM4_InputVideoData(g_pMainDlg->m_lPlayPort, pBuffer, dwBufSize);
		}
*/
		break;
/*
	case NET_DVR_STREAMDATA:
		if (dwBufSize > 0 && g_pMainDlg->m_lPlayPort >=0)
		{
			for (i=0; i<1000; i++)
			{
				bRet = PlayM4_InputData(g_pMainDlg->m_lPlayPort, pBuffer, dwBufSize);
				if (!bRet)
				{
					Sleep(1);
				}
				else
				{
					break;
				}
			}
		}
*/
		break;
	}
	return;
} 








/*
for(int i=0;i<256;i++)//写入随机数据（声音）
{
	BYTE k=rand()%256;
	for(int j=0;j<1024;j++)
		buff[i*1024+j]=k;
}




fwrite((BYTE *)&buff,sizeof(BYTE),1024*256,file);//写音频数据
fclose(file);

 */

/*采集音频流初始化*/
int VideoInterFace::InitNET_DVR_AUDIOSTREAMDATA()
{
//	fmt.wavFormat.wFormatTag=WAVE_FORMAT_PCM;//1
	fmt.wavFormat.wFormatTag=1;//1
	fmt.wavFormat.dwAvgBytesPerSec=11025;
	fmt.wavFormat.dwSamplesPerSec=11025;
	fmt.wavFormat.wBitsPerSample=8;
	fmt.wavFormat.wBlockAlign=1;
	fmt.wavFormat.wChannels=1;
	
	sprintf(fmt.szFmtID,"%s","fmt ");
	fmt.dwFmtSize=16;//一般情况下为16，如有附加信息为18
	sprintf(data.szDataID,"%s","data");
	data.dwDataSize=1024*256;
	riff.dwRiffSize=1024*256+sizeof(FMT_BLOCK)+sizeof(DATA_BLOCK);
	strncpy(riff.szRiffFormat,"WAVE",4);
	sprintf(riff.szRiffID,"%s","RIFF");
	file=fopen("wavefile.wav","wb");
	fwrite(&riff,sizeof(RIFF_HEADER),1,file);//写RIFF_HEADER 
	fwrite(&fmt,sizeof(FMT_BLOCK),1,file);//写FMT_BLOCK
	fwrite(&data,sizeof(DATA_BLOCK),1,file);//写数据头 DATA_BLOCK
	return 1;
}


/*************************************************
函数说明：播放视频
参数说明 
pHwnd  句柄 用来播放视频的窗口句柄
pButtonhwnd  按钮的指针
返回值  1 登陆成功
		0 登陆失败
**************************************************/
int VideoInterFace::play(HWND pHwnd,int iChannel)
{
	NET_DVR_CLIENTINFO clientnfo ={0};
	HWND pWnd = pHwnd; //  GetDlgItem(IDC_STATIC_PLAY)->m_hWnd; //获得窗口的句柄
	clientnfo.hPlayWnd = pWnd; //获得窗口的句柄
	clientnfo.lChannel = iChannel;              //预览通道号 
	clientnfo.lLinkMode = 0;              /*最高位(31)为 0表示主码流，为1 表示子码流。0～30位表示连接
						 方式：0－TCP 方式；1－UDP 方式；2－多播方式；3－RTP 方式; 4- RTP over RTSP，5- RTSP over HTTP */ 
	clientnfo.sMultiCastIP = NULL;      //多播地址，需要多播预览时配置 


	m_lPlayHandle_record = NET_DVR_RealPlay_V30(lUserId,&clientnfo,g_RealDataCallBack_V30,NULL,TRUE);
	if(-1 == m_lPlayHandle_record)
	{
		GetLastErrorz();
// 		DWORD err=NET_DVR_GetLastError();  //获取错误代码
// 		CString m_csErr;
// 		m_csErr.Format("播放出错，错误代码%d",err);
// 		MessageBox(m_csErr);
		NET_DVR_Cleanup();
		return 0;
	}
	VideoRunning = true;
	return 1;

}

/*************************************************
函数说明：停止播放视频
参数说明 

返回值  1 成功
		0 失败
**************************************************/
int VideoInterFace::stopplay()
{
	if(m_lPlayHandle_record != -1)
	{
		if(VideoRunning == TRUE)  //正在录像，先停止
		{
			// 停止录像StopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle_record);
		m_lPlayHandle_record = -1;
		VideoRunning = false;
	//	GetDlgItem(IDC_STATIC_PLAY)->Invalidate();  这个函数的作用？ 指定是否要擦除更新区域内的背景
		return 1;
	}
	else
	{
		return 0;
	}
}



/*************************************************
函数说明：开始录制视频
参数说明
返回值 true 开始录制
		false 录制失败
**************************************************/
BOOL VideoInterFace::startvideotape()
{
	char RecName[256] = {0};	
	CTime CurTime = CTime::GetCurrentTime();;
	sprintf(RecName,"%04d%02d%02d%02d%02d%02d_ch.mp4",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());

	 if(!NET_DVR_SaveRealData(m_lPlayHandle_record,RecName))
	 {
		 GetLastErrorz();
		 MessageBox("启动录像失败");
		 return false;
	 }
	 return true;
}



/*************************************
函数说明：停止录像

*************************************/
BOOL VideoInterFace::stopvideotape()
{
	if(!NET_DVR_StopSaveRealData(m_lPlayHandle_record))
	{
		GetLastErrorz();
		MessageBox("停止录像失败");
		return false;
	}
	isRecording = false;
	return true;
}



/*********************************
函数说明：用来显示异常代码
参数说明  zstr 要显示的内容
*********************************/
void VideoInterFace::Drawztext(CString zstr)
{
	MessageBox(zstr);
// 	HDC hdc=GetDC();
// 	CRect rc;
// 	GetClientRect(pHwnd,&rc);
// 	DrawText(hdc,zstr,strlen(zstr),&rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

/*********************************
函数说明：捕获异常码
参数说明  
*********************************/
void VideoInterFace::GetLastErrorz()
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
	}
}






// 		NET_DVR_TIME struStartTime, struStopTime; 
// 		struStartTime.dwYear      = 2013; 
// 		struStartTime.dwMonth    = 7; 
// 		struStartTime.dwDay        = 15; 
// 		struStartTime.dwHour      = 10; 
// 		struStartTime.dwMinute = 7; 
// 		struStartTime.dwSecond =0; 
// 		struStopTime.dwYear        = 2013; 
// 		struStopTime.dwMonth      = 7; 
// 		struStopTime.dwDay          = 15; 
// 		struStopTime.dwHour        = 10; 
// 		struStopTime.dwMinute    = 15; 
// 		struStopTime.dwSecond    = 0; 
// 
// 
// 		
// 		//按时间下载 
// 		 int hPlayback; 
// 		 hPlayback = NET_DVR_GetFileByTime(lUserId, 1, &struStartTime, &struStopTime, "./test.mp4"); 
// 		 if(hPlayback < 0) 
// 		 { 
// 		   printf("NET_DVR_GetFileByTime fail,last error %d\n",NET_DVR_GetLastError()); 
// 		   NET_DVR_Logout(lUserId); 
// 		   NET_DVR_Cleanup();   
// 		   return false; 
// 		 } 
// 
// 
// 		 //开始下载 
// 		 if(!NET_DVR_PlayBackControl(hPlayback, NET_DVR_PLAYSTART, 0, NULL)) 
// 		 { 
// 			 printf("play back control failed [%d]\n",NET_DVR_GetLastError()); 
// 			 NET_DVR_Logout(lUserId); 
// 			 NET_DVR_Cleanup(); 
// 			 return true; 
// 		 }  
// 
// 
// 		 int nPos = 0; 
// 		 for(nPos = 0; nPos < 100&&nPos>=0; nPos = NET_DVR_GetDownloadPos(hPlayback)) 
// 		 { 
// 			 Sleep(5000);    //millisecond 
// 		 } 
// 		 if(!NET_DVR_StopGetFile(hPlayback)) 
// 		 { 
// 			 printf("failed to stop get file [%d]\n",NET_DVR_GetLastError()); 
// 			 NET_DVR_Logout(lUserId); 
// 			 NET_DVR_Cleanup(); 
// 			 return false; 
// 		 } 
// 		 if(nPos<0||nPos>100) 
// 		 { 
// 			 printf("download err [%d]\n",NET_DVR_GetLastError()); 
// 			 NET_DVR_Logout(lUserId); 
// 			 NET_DVR_Cleanup(); 
// 			 return true; 
// 		  } 
//     
// 	   
// 	   
// 	  
// 	   
// 		//启动预览 并设置回调数据流
// 		//	LONG IrealPlayHandle;
// 		HWND pHwnd = this->m_hWnd;  //获取窗口句柄
// 		NET_DVR_CLIENTINFO clientnfo ={0};
// 		
// 		clientnfo.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd; //获得窗口的句柄
// 		//		ClientInfo->hPlayWnd = pHwnd;              //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空 
// 		clientnfo.lChannel = 1;              //预览通道号 
// 		 clientnfo.lLinkMode = 0;              /*最高位(31)为 0表示主码流，为1 表示子码流。0～30位表示连接
// 		 方式：0－TCP 方式；1－UDP 方式；2－多播方式；3－RTP 方式; 4- RTP over RTSP，5- RTSP over HTTP */ 
// 		 clientnfo.sMultiCastIP = NULL;      //多播地址，需要多播预览时配置 
// 		 
// 		 //		BOOL bPreviewBlock = false;              //请求码流过程是否阻塞，0：否，1：是 
// 		 //		IrealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, NULL, NULL, 0);
// 		 
// 		 m_lPlayHandle_record = NET_DVR_RealPlay_V30(lUserId,&clientnfo,NULL,NULL,TRUE);
// 		 if(-1 == m_lPlayHandle_record)
// 		 {
// 			 DWORD err=NET_DVR_GetLastError();
// 			 CString m_csErr;
// 			 m_csErr.Format("播放出错，错误代码%d",err);
// 			 MessageBox(m_csErr);
// 			 NET_DVR_Cleanup();
// 			 return false;
// 		 }
// 
// 	 char RecName[256] = {0};
// 	CTime CurTime = CTime::GetCurrentTime();;
// 	sprintf(RecName,"%04d%02d%02d%02d%02d%02d_ch.mp4",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
// 		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());
// 
// 	if(!NET_DVR_SaveRealData(m_lPlayHandle_record,RecName))
// 	 {
// 		 MessageBox("启动录像失败");
// 		 return true;
// 	 }
// 	STARTRECORD = false;
// //	 GetDlgItem(IDC_BUTTON_LOGIN1)->SetWindowText("停止录像");
// 		 
// 	}
// 	else if (lUserId < 0)
// 	{
// 		NET_DVR_Cleanup();
// 		return;
// 	}




/*********************************
函数说明：开启预览声音
参数说明：dwMode 选择独占声卡 （1） 或共享声卡（2） 
wVolume  音量设置 取值范围[0,0xffff] 
返回值 成功返回1 失败返回0
*********************************/
int VideoInterFace::OpenSound(DWORD dwMode, WORD wVolume)
{
	//设置声音播放模式。
	BOOL Result ;
	Result = NET_DVR_SetAudioMode(dwMode);
	if (Result == 1) //成功
	{
		switch(dwMode)
		{
		case 1:
			//	独占声卡模式下开启声音
			Result = NET_DVR_OpenSound(m_lPlayHandle_record);
			if (Result == 0)
			{
				return 0;
			}
			break;
		case 2:
			//	共享声卡模式下开启声音。
			
			Result = NET_DVR_OpenSoundShare(m_lPlayHandle_record);
			if (Result == 0)
			{
				return 0;
			}
			break;
		default:
			break;
		}
		//	调节播放音量。
		NET_DVR_Volume(m_lPlayHandle_record,wVolume);
	}
	
	return 1;
}

/*********************************
函数说明：关闭预览声音
参数说明：dwMode 选择独占声卡 （1） 或共享声卡（2） 
返回值 成功返回1 失败返回0
*********************************/
int VideoInterFace::CloseSound(DWORD dwMode)
{
	BOOL Result;
	switch(dwMode)
	{
	case 1:
		//独占声卡模式下关闭声音。		
		Result = NET_DVR_CloseSound();
		if (Result == 0)
		{
			return 0;
		}
		break;
	case 2:
		//共享声卡模式下关闭声音。	
		Result = NET_DVR_CloseSoundShare(m_lPlayHandle_record);
		break;
	default:
		break;
	}
	return 1;
}

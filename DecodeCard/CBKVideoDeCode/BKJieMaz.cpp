// BKJieMaz.cpp: implementation of the CBKJieMaz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BKJieMaz.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_SERVER_NUM			32		//最多添加32台设备
STRU_DECODE_CHAN struDecodeChan[MAX_CHANNELS];				//解码通道相关参数
USER_SERVER_INFO userServerInfo[MAX_SERVER_NUM]; 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CBKJieMaz::CBKJieMaz()
{
	g_nTotalDecChannels = 0;
	g_nTotalDspCnt=0;
	g_nTotalDispChannels=0;
	g_nDevCount = 0;

}

CBKJieMaz::~CBKJieMaz()
{
	F_DVR_Logoutz();
	ChannelClosez();
	HW_ReleaseDirectDraw();
	HW_ReleaseDecDevice();//释放解码卡
}

BOOL CBKJieMaz::InitDsp(HWND hParent)
{
	int decRet=0;
	if ((decRet = HW_InitDecDevice((long*)(&g_nTotalDecChannels))) != 0)//初始化解码卡
	{
		AfxMessageBox("初始化解码卡失败!");
		return FALSE;
	}
	g_nTotalDspCnt = GetDspCount();//获取系统中 DSP的个数
	g_nTotalDecChannels = GetDecodeChannelCount(); //解码通道数
	g_nTotalDispChannels = GetDisplayChannelCount(); //获取系统中显示通道的个数
	//////////////////////////////////////////////////////////////////////////
	CString temp;
	temp.Format("%d",g_nTotalDispChannels);
	::WritePrivateProfileString("CONFIG","displaychannel",temp,".\\search.cfg");//
	UINT BoardCounti=GetBoardCount();
	temp.Format("板卡张数=%d,DSP的个数=%d,解码通道数=%d,显示通道的个数=%d",BoardCounti,g_nTotalDspCnt,g_nTotalDecChannels,g_nTotalDispChannels);
	WriteLog(temp);
	//////////////////////////////////////////////////////////////////////////
	HW_InitDirectDraw(hParent, RGB(10,255,10));//初始化 DirectDraw 
	UINT i;
	for (i = 0; i < g_nTotalDecChannels; i++)
	{
		if (HWERR_SUCCESS != HW_ChannelOpen(i, &struDecodeChan[i].hChannelHandle))//打开解码通道
		{
			struDecodeChan[i].hChannelHandle = NULL; 
			temp.Format("HW_ChannelOpen [%d] failed!",i);
			WriteLog(temp);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	NET_DVR_Init();// SDK初始化
	VERSION_INFO verInfo; 
	GetSDKVersion(&verInfo); 
	CString verStr; 
	verStr.Format("Dsp Ver:%04x-%08x,Driver Ver:%04x-%08x,Sdk Ver:%04x-%08x", 
		verInfo.DspVersion, verInfo.DspBuildNum, verInfo.DriverVersion, verInfo.DriverBuildNum, 
		verInfo.SDKVersion, verInfo.SDKBuildNum); 
	WriteLog(verStr);
	//////////////////////////////////////////////////////////////////////////
	for (int di=0;di<MAX_DISCHANNEL;di++)
	{
		OsdSD[di][0]=112;
		OsdSD[di][1]=548;
		OsdSD[di][2]=_T('\0');
		OsdSC[di][0]=112;
		OsdSC[di][1]=524;
		OsdSC[di][2]=_T('\0');
	}

	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

void CBKJieMaz::ChannelClosez()
{
	UINT i,ret;
	for (i = 0; i < g_nTotalDecChannels; i ++)
	{		
		if (struDecodeChan[i].bDecPlay)
		{
			ret = HW_Stop(struDecodeChan[i].hChannelHandle);
			if (ret != 0)
			{
			}
			else
			{
				struDecodeChan[i].bDecPlay =FALSE;
			}
			
			ret = HW_CloseStream(struDecodeChan[i].hChannelHandle);
			if (ret != 0)
			{

			}
		}
		if ((ret = HW_ChannelClose(struDecodeChan[i].hChannelHandle)) != HWERR_SUCCESS)
		{
		}		
	}
}

BOOL CBKJieMaz::SetDisRGN()
{
	int dw=704;
	int dh=576;
	UINT i=0;
	int ret=0;
	REGION_PARAM param[MAX_DISPLAY_REGION];
	memset(param, 0, sizeof(param)); 
	for(i = 0; i < MAX_DISPLAY_REGION; i++)
	{
		param[i].color = RGB(10, 10, 10); 
		param[i].param = 0; 
	}
	for(i = 0; i < 4; i++)//四画面分割
	{
		param[i].left = ((i%2)*(dw/2))&~7; 
		param[i].top = ((i/2)*(dh/2))&~3; 
		param[i].width = (dw/2)&~7; 
		param[i].height = (dh/2)&~3; 
	}
	CString temp;
	for (i=0;i<g_nTotalDispChannels;i++)//所有的显示通道四分割
	{
		ret = SetDisplayRegion(i, 4, param, 0);
		if (ERR_NOT_SUPPORT==ret)
		{
			WriteLog("DSP 资源不足，无法划分窗口");
		}
		if (ERR_INVALID_DEVICE==ret)
		{
			WriteLog("nDisplayChannel 溢出");
		}
		ret=SetDecoderVideoExtOutput(i*4,0,TRUE,i,0,NULL);//车载
		if (ret !=0)
		{
			temp.Format("显示通道:%d",i);
			WriteLog("SetDecoderVideoExtOutput:车载视频输出设置失败!"+temp);
		}
		ret=SetDecoderVideoExtOutput(i*4+1,0,TRUE,i,1,NULL);//项目
		if (ret !=0)
		{
			temp.Format("显示通道:%d",i);
			WriteLog("SetDecoderVideoExtOutput:项目视频输出设置失败!"+temp);
		}
	}
	return TRUE;
}

void CBKJieMaz::SetDispChanLogo(UINT nChannel,BOOL m_bDisLogoEnable)
{
#ifdef Ver_6
	int ret; 
	CString csLog; 
	int nDispChannel;
	if (nChannel >=5403)
	{
		nDispChannel =nChannel -5403;
		nChannel=5403;
	}
	else
	{
		nDispChannel=nChannel;
	}

	//设置LOGO显示
	if(m_bDisLogoEnable)
	{
		csLog.Format("SetDispChanLogo: nChannel=%d.dat,nDispChannel=%d",nChannel,nDispChannel);
		WriteLog(csLog);
		int w, h; 
		unsigned char *bitsBuffer = (unsigned char *)malloc(704 * 576 * 2); 		
		if(bitsBuffer == NULL)
		{
			WriteLog("SetLogo向系统申请分配704*576*2个字节的内存空间出错!");
			return; 
		}	
		CString temp;
		temp.Format(".\\%d.dat",nChannel);
		if ((ret = LoadYUVFromBmpFile(temp.GetBuffer(0), bitsBuffer, 704 * 576 * 2,  &w, &h)) != HWERR_SUCCESS)
		{
			WriteLog("LoadYUVFromBmpFile failed! "+temp);
		}
		ret = SetDisplayLogo(nDispChannel, 0,288, w, h, (UCHAR *)bitsBuffer); //
		if (ret !=0)
		{
			WriteLog("SetDisplayLogo failed!");
		}
		//ret=SetDisplayLogoDisplayMode(nDispChannel,-1, 0, 0);
		ret=SetDisplayLogoDisplayMode(nDispChannel,RGB(0,0,0), 0, 0);
		if(ret != 0)
		{
			WriteLog("SetDisplayLogoDisplayMode failed!");
		}
		free(bitsBuffer); 
	}
	else
	{		
		ret = StopDisplayLogo(nDispChannel); 		
		if(ret != 0)
		{
			WriteLog("StopDisplayLogo failed!");
		}
	}
#endif	
}

void CALLBACK RealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	int ret = 0;
	DWORD dwRet = 0; 
	DWORD dwReadedLen = 0; 
	CString str; 
	int times = 0; 
	DWORD dwWinIndex = *((unsigned long *)pUser); 
	PBYTE pBufferTemp = NULL; 
	DWORD dwBufSizeTemp = 0; 
	CString csVideoInfo;
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD://coming the stream header, open stream]		
		if (struDecodeChan[dwWinIndex].bDecPlay)
		{
			ret = HW_Stop(struDecodeChan[dwWinIndex].hChannelHandle);
			if (ret != 0)
			{
				str.Format("RealDataCallBack->HW_Stop failed,WinIndex[%02d],ret=%x",dwWinIndex,ret);
				WriteLog(str);
			}
			else
			{
				struDecodeChan[dwWinIndex].bDecPlay =FALSE;
			}
	
			ret = HW_CloseStream(struDecodeChan[dwWinIndex].hChannelHandle);
			if (ret != 0)
			{
				str.Format("RealDataCallBack->HW_CloseStream failed,WinIndex[%02d],ret=%x",dwWinIndex,ret);
				WriteLog(str);
			}
		}
		ret = HW_SetStreamOpenMode(struDecodeChan[dwWinIndex].hChannelHandle,5);
		if (ret != 0)
		{
			str.Format("RealDataCallBack->HW_SetStreamOpenMode failed,WinIndex[%02d],ret=%x",dwWinIndex,ret);
			WriteLog(str);
		}
		ret=HW_OpenStream(struDecodeChan[dwWinIndex].hChannelHandle,pBuffer,dwBufSize);
		if (ret != 0)
		{
			str.Format("RealDataCallBack->HW_OpenStream failed,WinIndex[%02d],ret=%x",dwWinIndex,ret);
			WriteLog(str);
		}		
		ret=HW_Play(struDecodeChan[dwWinIndex].hChannelHandle);
		if (ret != 0)
		{
			str.Format("RealDataCallBack->HW_Play failed,WinIndex[%02d],ret=%x",dwWinIndex,ret);
			WriteLog(str);
		}
		else
		{
			struDecodeChan[dwWinIndex].bDecPlay =TRUE;
		}
		
		break; 
	case EXCEPTION_PREVIEW:
		break; 
	case NET_DVR_STREAMDATA:
	case NET_DVR_STD_VIDEODATA:	
	case NET_DVR_AUDIOSTREAMDATA:
	case NET_DVR_STD_AUDIODATA:
		if (dwBufSize > 0)
		{
			while((dwReadedLen < dwBufSize)&&(times++ < 10))
			{
				pBufferTemp = pBuffer+dwReadedLen; 
				dwBufSizeTemp = dwBufSize - dwReadedLen; 
				dwRet = HW_InputData(struDecodeChan[dwWinIndex].hChannelHandle, pBufferTemp, dwBufSizeTemp); 
				dwReadedLen += dwRet; 
			}
		}
		
		break; 
	default:
		break; 
	}
	
	return; 
}
void CALLBACK g_fStdDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	RealDataCallBack(lRealHandle, dwDataType, pBuffer, dwBufSize, &dwUser); 	
}

DWORD CBKJieMaz::F_Login(LPTSTR lpIP, DWORD dwPort, LPTSTR lpUserName, LPTSTR lpUserPassword)
{
	NET_DVR_DEVICEINFO_V30 struDeviceInfoV30;
	LONG lUserID = NET_DVR_Login_V30(lpIP,(WORD)dwPort,lpUserName,lpUserPassword, &struDeviceInfoV30);
	if(lUserID < 0)
	{
		CString csLog;
		csLog.Format("NET_DVR_Login_V30 failed!,IP=%s,Port=%d,User=%s,Password=%s,Eid=%d",lpIP,dwPort,lpUserName,lpUserPassword,NET_DVR_GetLastError());
		WriteLog(csLog);
		return FALSE;
	}
	else
	{
		userServerInfo[g_nDevCount].lDvrUserID=lUserID;
		userServerInfo[g_nDevCount].dwDvrPort = dwPort;
		sprintf(userServerInfo[g_nDevCount].sDvrIP, "%s", lpIP);
		sprintf(userServerInfo[g_nDevCount].sDvrUserName, "%s", lpUserName);
		sprintf(userServerInfo[g_nDevCount].sDvrPassword, "%s", lpUserPassword);
		userServerInfo[g_nDevCount].byChanNum =struDeviceInfoV30.byChanNum;
		userServerInfo[g_nDevCount].byStartChan =struDeviceInfoV30.byStartChan;
		g_nDevCount+=1;
		CString csLog;
		csLog.Format("NET_DVR_Login_V30 IP=[%s],LOGIN SUCCEED!",lpIP);
		WriteLog(csLog);
	}
	return TRUE;
}

BOOL CBKJieMaz::DvrPlayChannel(LONG lChannel,CString dIP,UINT jiechannel,BOOL cz)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = NULL;
	ClientInfo.lChannel     = lChannel;
	ClientInfo.lLinkMode    = 0x80000000;//0x0;
    ClientInfo.sMultiCastIP = NULL;
	//////////////////////////////////////////////////////////////////////////
	int m_nWndNum=0;//通道序号
	if (jiechannel > g_nTotalDispChannels)
	{
		WriteLog("数据异常,jiechannel大于解码显示通道数!");
		return FALSE;
	}
	if(!cz)
	{
		m_nWndNum=jiechannel*4;
	}
	else
	{
		m_nWndNum=jiechannel*4+1;
	}
	int dvri=0;
	for (int i=0;i<(int)g_nDevCount;i++)
	{
		if(strcmp(userServerInfo[i].sDvrIP,dIP)==0)
		{
			dvri=i;
			i=MAX_SERVER_NUM+100;
		}
	}
	CString cslog;
	cslog.Format("DvrPlayChannel lChannel=%d,IP=%s,DecodeChannel=%d",lChannel,dIP,m_nWndNum);
	WriteLog(cslog);
	BOOL bRet = FALSE;
	if (TRUE==struDecodeChan[m_nWndNum].bRealPlay)
	{
		NET_DVR_StopRealPlay(struDecodeChan[m_nWndNum].lPlayHandle); 
		struDecodeChan[m_nWndNum].bRealPlay=FALSE;
		struDecodeChan[m_nWndNum].lPlayHandle =-1;
	}	
	LONG m_lPlayHandle =-1;
	struDecodeChan[m_nWndNum].m_Num=m_nWndNum;//新增
	m_lPlayHandle = NET_DVR_RealPlay_V30(userServerInfo[dvri].lDvrUserID, &ClientInfo, NULL,&struDecodeChan[m_nWndNum].m_Num, FALSE); 
	if (m_lPlayHandle >= 0)
	{
		bRet = NET_DVR_SetStandardDataCallBack(m_lPlayHandle, g_fStdDataCallBack,struDecodeChan[m_nWndNum].m_Num); 
	}
	if (bRet ==FALSE)
	{
		if (m_lPlayHandle >= 0)
		{
			NET_DVR_StopRealPlay(m_lPlayHandle); 
			m_lPlayHandle =  - 1; 
		}		
		m_lPlayHandle = NET_DVR_RealPlay_V30(userServerInfo[dvri].lDvrUserID, &ClientInfo,RealDataCallBack, &struDecodeChan[m_nWndNum].m_Num, TRUE); 
		if (m_lPlayHandle < 0)
		{
			cslog.Format("NET_DVR_RealPlay_V30 failed!,ErrorID=%d",NET_DVR_GetLastError());
			WriteLog(cslog);
		}
	}
	if (m_lPlayHandle !=-1)
	{
		struDecodeChan[m_nWndNum].bRealPlay=TRUE;
		struDecodeChan[m_nWndNum].lPlayHandle =m_lPlayHandle;
	}
	return TRUE;
}

void CBKJieMaz::F_DVR_Logoutz()
{
	int i;
	for (i=0;i<(int )g_nTotalDispChannels;i++)
	{
		if (TRUE==struDecodeChan[i].bRealPlay)
		{
			NET_DVR_StopRealPlay(struDecodeChan[i].lPlayHandle); 
			struDecodeChan[i].bRealPlay=FALSE;
			struDecodeChan[i].lPlayHandle =-1;
		}
	}
	for(i=0;i<(int)g_nDevCount;i++)
	{
		if(userServerInfo[i].lDvrUserID>=0)
		{
			if(!NET_DVR_Logout_V30(userServerInfo[i].lDvrUserID))
			{
				
			}
			userServerInfo[i].lDvrUserID = -1;
		}
	}
}

void CBKJieMaz::DvrPlayStopXM(UINT Channel)
{
	if (Channel > g_nTotalDispChannels)//大于显示通道
	{
		return ;
	}
	int m_nWndNum;//通道序号
	m_nWndNum=Channel*4+1;//+g_nTotalDispChannels;	
	
	if (TRUE==struDecodeChan[m_nWndNum].bRealPlay)
	{
		NET_DVR_StopRealPlay(struDecodeChan[m_nWndNum].lPlayHandle); 
		struDecodeChan[m_nWndNum].bRealPlay=FALSE;
		struDecodeChan[m_nWndNum].lPlayHandle =-1;
		CString csLog;
		csLog.Format("DvrPlayStopXM:%d",m_nWndNum);
		WriteLog(csLog);
	}
}

BOOL CBKJieMaz::DvrSetDisPOSD(UINT disChannel,CString mStrs,UINT line)
{
	if (disChannel > g_nTotalDispChannels)//大于显示通道
	{
		return FALSE;
	}
	UINT *tempOsdUnionANSI[2];
	int k,i;
	CString mStr;
	if(line ==0)
	{
		mStr.Format("%s",mStrs);
		for (k = 2, i = 0; i < mStr.GetLength(); )
		{				
			if(i >= MAX_OSD_BYTES)
			{
				break;
			}
			if((mStr.GetAt(i) & 0xff00) == 0)    //单字节字符
			{
				OsdSC[disChannel][k++] = mStr.GetAt(i);
				i += 1;
			}
			else									  //双字节
			{
				OsdSC[disChannel][k++] = ((mStr.GetAt(i) & 0xff) << 8) | (mStr.GetAt(i + 1) & 0xff);
				i += 2;
			}	
		}
		OsdSC[disChannel][k] = _T('\0');
	}
	else
	{
		mStr.Format("%s",mStrs);
		for (k = 2, i = 0; i < mStr.GetLength(); )
		{				
			if(i >= MAX_OSD_BYTES)
			{
				break;
			}
			if((mStr.GetAt(i) & 0xff00) == 0)    //单字节字符
			{
				OsdSD[disChannel][k++] = mStr.GetAt(i);
				i += 1;
			}
			else									  //双字节
			{
				OsdSD[disChannel][k++] = ((mStr.GetAt(i) & 0xff) << 8) | (mStr.GetAt(i + 1) & 0xff);
				i += 2;
			}	
		}
		OsdSD[disChannel][k] = _T('\0');
	}
	
	tempOsdUnionANSI[0] = OsdSD[disChannel];
	tempOsdUnionANSI[1] = OsdSC[disChannel];
	UINT *unicoderFormat[2] = {(new UINT[91]), (new UINT[91])};
	TransMultiByteToWideByte(tempOsdUnionANSI[0], unicoderFormat[0]);
	TransMultiByteToWideByte(tempOsdUnionANSI[1], unicoderFormat[1]);
	int param[2];
	param[0]=0; 
	param[1]=0;
	int ret=-1;
//	ret = SetDisplayOsdDisplayMode(0, TRUE, 255, 0,-1,RGB(40,45,50), 0, 1,  param, 2, unicoderFormat);
	ret = SetDisplayOsdDisplayMode(disChannel, TRUE, 255, 0,-1,-1, 0, 1,  param, 2, unicoderFormat);
	if (ret !=0)
	{
		WriteLog("SetDisplayOsdDisplayMode faild!");
	}
	delete unicoderFormat[0];
	delete unicoderFormat[1];
	return TRUE;
}

BOOL CBKJieMaz::TransMultiByteToWideByte(UINT *pFormat, UINT *pFormatOut) 
{ 
#ifdef Ver_6
	UINT* pFormatTemp; 
	UINT* pFormatOutTemp; 
	int i; 
	int k; 
	char multiByteStr[2]; 
	wchar_t  wideCharStr[1]; 
	BOOL bHaveNull = FALSE; 
	//将多字节字符转换成宽字节字符 
	for (i = 0, k = 0; i < 360; k++) 
	{                 
		pFormatTemp = pFormat+k+2; 
		pFormatOutTemp = pFormatOut+k+2; 
		
		if (*pFormatTemp >= _OSD_YEAR4_EX && *pFormatTemp <= _OSD_APM_EX) 
		{ 
			//如果是标签字符，则赋一个没有字符对应的多字节字符编码值z 
			*pFormatOutTemp = *pFormatTemp; 
			i += 2; 
		} 
		else if (0xFF00 & *pFormatTemp) 
		{ 
			multiByteStr[0] = (*pFormatTemp>>8) & 0xFF; 
			multiByteStr[1] = *pFormatTemp & 0xFF; 
			if (MultiByteToWideChar(936, MB_PRECOMPOSED, multiByteStr, 2, (wchar_t*)wideCharStr, 1) == 0) 
			{
				char tempChar[3]; 
				memcpy(tempChar, multiByteStr, 2); 
				tempChar[2] = '\0'; 
				TRACE("MultiByteToWideChar failed, multiByteStr is %s\n", tempChar); 
				//return FALSE; 
				wideCharStr[0] = 0x20; 
			} 
			*pFormatOutTemp = wideCharStr[0]; 
			i += 2; 
		} 
		else 
		{ 
			*pFormatOutTemp = *pFormatTemp; 
			i++; 
		} 
		
		if (*pFormatTemp == '\0') 
		{ 
			bHaveNull = TRUE; 
			break; 
		} 
	} 
	if (!bHaveNull) 
	{ 
		*(pFormatTemp++) = 0; 
	} 	
	*pFormatOut = *pFormat; 
	*(pFormatOut+1) = *(pFormat+1); 
	
#endif	
	return TRUE; 
}
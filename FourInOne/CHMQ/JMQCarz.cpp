// JMQCarz.cpp: implementation of the CJMQCarz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JMQCarz.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJMQCarz::CJMQCarz()
{
	m_iKCH	=-1;
	m_lUserID	=-1;

}

CJMQCarz::~CJMQCarz()
{
// 	if (m_TPic!=NULL)
// 	{
// 		delete m_TPic;
// 	}
// 	if (m_FPic!=NULL)
// 	{
// 		delete m_FPic;
// 	}
}

void CJMQCarz::InitCar(int iKch, LONG lUserID, BYTE (&deChan)[4],DWORD sid,DWORD fid,CString mPath)
{
	m_iKCH=iKch;
	m_lUserID=lUserID;
	m_deChannl[0]=deChan[0];
	m_deChannl[1]=deChan[1];
	m_deChannl[2]=deChan[2];
	m_deChannl[3]=deChan[3];
	//////////////////////////////////////////////////////////////////////////
	slogid=sid;
	flogid=fid;
	logz.WriteLog(slogid,"初始化考车%d,解码通道=%d,%d,%d,%d",iKch,m_deChannl[0],m_deChannl[1],m_deChannl[2],m_deChannl[3]);
	//////////////////////////////////////////////////////////////////////////
}

bool CJMQCarz::StartDynamic(STerminalPZ tpz, int iwnd)
{
	NET_DVR_PU_STREAM_CFG_V41 dt;
	ZeroMemory(&dt, sizeof(dt));
	dt.dwSize = sizeof(NET_DVR_PU_STREAM_CFG_V41);
	dt.byStreamMode = 1;//通过IP或域名取流
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byChanType =0;//通道类型，0-普通通道,1-零通道,2-流ID,3-本地输入源，4-虚拟屏服务器通道
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byChannel =0;//该参数无效，通道号见dwChannel 
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byTransProtocol = 0;//传输协议类型0-TCP，1-UDP	
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byFactoryType=0;/*前端设备厂家类型,通过接口获取*/
	
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byTransMode =tpz.imllx;//传输码流模式 0－主码流 1－子码流    
	sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byAddress, "%s", tpz.TerminalIP);//设备域名
	sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.sUserName, "%s", tpz.RasUser);//登陆帐号
	sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.sPassword, "%s", tpz.RasPassword);//密码	
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.wDVRPort = tpz.TerminalPort;//端口号
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.dwChannel = tpz.dwChannel;//通道号
	
	if (strlen(tpz.mediaIP)!=0)//流媒体IP不为空
	{
		dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.byValid=1;//启用流媒体
		dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.wDevPort=554;//端口
		dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.byTransmitType=0;//TCP
		sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.byAddress, "%s", tpz.mediaIP);//流媒体IP
	}
	if (!NET_DVR_MatrixStartDynamic_V41(m_lUserID,m_deChannl[iwnd], &dt))//解码通道 0-15
	{		
		logz.WriteLog(flogid,"动态解码失败! NET_DVR_MatrixStartDynamic_V41 Eid=%d,ecode:[%s],考车%d",NET_DVR_GetLastError(),tpz.TermainlBH,m_iKCH);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}
	logz.WriteLog(slogid,"开启动态解码!考车%d,ecode=%s,子窗口=%d,mediaIP=%s,码流%d(0主码流,1子码流),%s_%d",\
		m_iKCH,tpz.TermainlBH,iwnd,tpz.mediaIP,tpz.imllx,tpz.TerminalIP,tpz.dwChannel);
	return true;
}

bool CJMQCarz::StartPassive(int iwnd,LONG &lph)
{
	lph=-1;
	NET_DVR_MATRIX_PASSIVEMODE m_PassiveMode;//被动解码
	m_PassiveMode.wTransProtol=0;//wTransProtol 	传输协议：0-TCP，1-UDP，2-MCAST 
	m_PassiveMode.wPassivePort = 8000;//TCP或者UDP端口，TCP时端口默认是8000，不同的解码通道UDP端口号需设置为不同的值
	m_PassiveMode.byStreamType =2;//数据类型: 1-实时流, 2-文件流
	LONG lPassiveHandle=NET_DVR_MatrixStartPassiveDecode(m_lUserID,m_deChannl[iwnd],&m_PassiveMode);
	if (lPassiveHandle < 0)
	{	
		logz.WriteLog(flogid,"启动被动解码失败!NET_DVR_MatrixStartPassiveDecode! errid=%d,解码通道=%d,子窗口=%d,考车%d",NET_DVR_GetLastError(),m_deChannl[iwnd],iwnd,m_iKCH);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}
	lph=lPassiveHandle;
	logz.WriteLog(slogid,"开启被动解码!考车%d,通道号:%d,子窗口:%d,lPassiveHandle:%ld",m_iKCH,m_deChannl[iwnd],iwnd,lPassiveHandle);	
	return true;
}
//NET_DVR_MatrixStopDynamic 停止动态解码
void CJMQCarz::StopDynamic(int iwnd)
{
	if(NET_DVR_MatrixStopDynamic(m_lUserID, m_deChannl[iwnd]))
	{
		logz.WriteLog(slogid,"停止动态解码成功!考车%d,子窗口:%d",m_iKCH,m_deChannl[iwnd]);
	}
	else
	{
		logz.WriteLog(flogid,"停止动态解码失败!考车%d,子窗口:%d,errorid=%d",m_iKCH,m_deChannl[iwnd],NET_DVR_GetLastError());
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
	}
}

bool CJMQCarz::StopPassive(int iwnd)
{
	return true;
}

void CJMQCarz::On17C51(CString zkzm, int idrcs, int ikscs)
{

}

void CJMQCarz::On17C52(CString zkzm, int ic, CString msgz)
{

}

void CJMQCarz::On17C53(CString timestr, CString msgz, int ikcfs)
{

}

void CJMQCarz::On17C55(int ic, CString msgz)
{

}

void CJMQCarz::On17C56(int itype, int ikscj)
{

}

void CJMQCarz::OnGnssData(LPVOID msgz)
{

}

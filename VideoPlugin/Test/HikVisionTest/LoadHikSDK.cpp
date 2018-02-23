// LoadHikSDK.cpp: implementation of the CLoadHikSDK class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "HikVisionTest.h"
#include "LoadHikSDK.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadHikSDK::CLoadHikSDK()
{
	m_Handlez=NULL;
	CString tempPath,strPath;
	strPath.Format("D:\\Temp");
	DWORD dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	tempPath.Format("%s\\%s",strPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//今天日期文件夹
	}
	m_LogFileName.Format("%s\\CLoadHikSDK.log",tempPath);
}

CLoadHikSDK::~CLoadHikSDK()
{
/*	if (m_Handlez!=NULL)		//释放DLL
	{
		::FreeLibrary(m_Handlez);
		m_Handlez=NULL;
	}*/
}

void CLoadHikSDK::WriteLogz(LPCTSTR pstrFormat, ... )
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format("[%02d:%02d:%02d.%03d]",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	
	FILE *fp=fopen(m_LogFileName,"a+");
	fprintf(fp,"%s:%s#\n",strTime,logstr);
	fclose(fp);	
}

bool CLoadHikSDK::InitLoadDll(CString dllpath)
{
	//SetCurrentDirectory("D:\\bkzyVideoOcx\\hikVision");
	//m_Handlez =LoadLibrary(TEXT(dllpath)); 
	m_Handlez =LoadLibraryEx(dllpath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH); 
	if (m_Handlez==NULL)
	{
		WriteLogz("动态加载 CHikVisionz.dll 失败!%s",dllpath);
		return false;
	}
	dllfnInitWnd = (ZfnInitWnd) GetProcAddress(m_Handlez,TEXT("fnInitWnd"));
	dllfnDVRLogin = (ZfnDVRLogin) GetProcAddress(m_Handlez,TEXT("fnDVRLogin"));
	dllfnSetState = (ZfnSetState) GetProcAddress(m_Handlez,TEXT("fnSetState"));
	dllfnPlayBackbyTime= (ZfnPBbyTime) GetProcAddress(m_Handlez,TEXT("fnPlayBackByTime"));
	if (dllfnInitWnd==NULL || dllfnDVRLogin==NULL || dllfnSetState==NULL || dllfnPlayBackbyTime==NULL)
	{
		WriteLogz("动态加载 CHikVisionz.dll 接口失败! InitLoadDll %s",dllpath);
		return false;
	}
	WriteLogz("InitLoadDll succ! %s",dllpath);
	return true;
}

void CLoadHikSDK::SDKInitWnd(HWND hwndz, int wMSG, int nWndID)
{
	try
	{
		dllfnInitWnd(hwndz,wMSG,nWndID);
	}
	catch (...)
	{
		WriteLogz("fnInitWnd catch! %d",nWndID);
	}
}

BOOL CLoadHikSDK::SDKDVRLogin(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel, int nWndID)
{
	try
	{
		return dllfnDVRLogin(strDvrIP,strUseName,strPwd,nPort,nChannel,nWndID,1);
	}
	catch (...)
	{
		WriteLogz("SDKDVRLogin catch! %d",nWndID);
	}
	return FALSE;
}

BOOL CLoadHikSDK::SDKRealPlay(int nWndID)
{
	try
	{
		return dllfnSetState(nWndID,1);
	}
	catch (...)
	{
		WriteLogz("SDKRealPlay catch! %d",nWndID);
	}
	return FALSE;
}

void CLoadHikSDK::SDKStopPlay(int nWndID)
{
	try
	{
		dllfnSetState(nWndID,2);
	}
	catch (...)
	{
		WriteLogz("SDKStopPlay catch! %d",nWndID);
	}
}

BOOL CLoadHikSDK::SDKStartVoiceCom_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel, int nWndID)
{
	try
	{
		return dllfnDVRLogin(strDvrIP,strUseName,strPwd,nPort,nChannel,nWndID,2);
	}
	catch (...)
	{
		WriteLogz("SDKStartVoiceCom_V30 catch! %d",nWndID);
	}
	return FALSE;
}

void CLoadHikSDK::SDKStopVoiceCom(int nWndID)
{
	try
	{
		dllfnSetState(nWndID,3);
	}
	catch (...)
	{
		WriteLogz("SDKStopVoiceCom catch! %d",nWndID);
	}
}

BOOL CLoadHikSDK::SDKPlayBackbyTime(LPCTSTR strSTime, LPCTSTR strETime,int nWndID)
{
	try
	{
		return dllfnPlayBackbyTime(strSTime,strETime,nWndID);
	}
	catch (...)
	{
		WriteLogz("SDKPlayBack catch! %d (%s,%s)",nWndID,strSTime,strETime);
	}
	return FALSE;
}

BOOL CLoadHikSDK::SDKOpenSoundShare(int nWndID)
{
	try
	{
		return dllfnSetState(nWndID,4);
	}
	catch (...)
	{
		WriteLogz("SDKOpenSoundShare catch! %d",nWndID);
	}
	return FALSE;
}

BOOL CLoadHikSDK::SDKCloseSoundShare(int nWndID)
{
	try
	{
		return dllfnSetState(nWndID,5);
	}
	catch (...)
	{
		WriteLogz("SDKCloseSoundShare catch! %d",nWndID);
	}
	return FALSE;
}

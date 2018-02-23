// LoadPlatSDK.cpp: implementation of the CLoadPlatSDK class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadPlatSDK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadPlatSDK::CLoadPlatSDK()
{
	dllPlayVideoPlat=NULL;
	dllStopVideoPlat=NULL;
	dllSetStreamCamer=NULL;
	dllSetCallBack=NULL;
	dllPLATHIN		= NULL;

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
	m_LogFileName.Format("%s\\CLoadPlatSDK.log",tempPath);
}

CLoadPlatSDK::~CLoadPlatSDK()
{
	
}

void CLoadPlatSDK::WriteLogz(LPCTSTR pstrFormat, ... )
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

BOOL CLoadPlatSDK::InitLoadDll()
{

	//////////////////////////////////////////////////////////////////////////
	dllPLATHIN =LoadLibraryEx(TEXT("D:\\bkzyVideoOcx\\IVMS8600\\CBKPlatSDKz.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH); 	
	if (dllPLATHIN == NULL) 
	{ 
		AfxMessageBox("加载 8600数据 失败!"); 
		return FALSE; 
	}
	dllPlayVideoPlat = (DPlayVideoPlat) GetProcAddress(dllPLATHIN,TEXT("PlayVideoPlat"));
	dllStopVideoPlat = (DStopVideoPlat) GetProcAddress(dllPLATHIN,TEXT("StopVideoPlat"));
	dllSetStreamCamer = (DSetStreamCamer) GetProcAddress(dllPLATHIN,TEXT("SetStreamCamer"));
	dllSetCallBack =(DSetCallBack) GetProcAddress(dllPLATHIN,TEXT("SetCallBack"));
	if (dllPlayVideoPlat==NULL || dllStopVideoPlat==NULL || dllSetStreamCamer==NULL || dllSetCallBack==NULL)
	{			
		AfxMessageBox("加载 8600接口 失败!"); 
		return FALSE;
	}
	return TRUE;
}

BOOL CLoadPlatSDK::PLATPlayVideo(int iwndid,long lWnd)
{
	try
	{
		if (dllPlayVideoPlat !=NULL)
		{
			return dllPlayVideoPlat(iwndid,lWnd);
		}		
	}
	catch (...)
	{
		WriteLogz("PLATPlayVideo faild...iWndid=%d",iwndid);
	}
	return FALSE;
}

void CLoadPlatSDK::PLATStopVideo(int iwndid)
{
	try
	{
		if (dllStopVideoPlat !=NULL)
		{
			dllStopVideoPlat(iwndid);
		}		
	}
	catch (...)
	{
		WriteLogz("PLATStopVideo faild...");
	}
}

void CLoadPlatSDK::PLATSetStreamCamer(LPCTSTR csCameraID,int iWnd)
{
	try
	{
		if (dllSetStreamCamer !=NULL)
		{
			dllSetStreamCamer(csCameraID,iWnd);
		}
		else
		{
			if (m_pMessageProc!=NULL)
			{
				m_pMessageProc(iWnd,PLAT_URLFAILD,0,m_dwMessageUser);
			}
		}
	}
	catch (...)
	{
		WriteLogz("PLATSetStreamCamer faild...");
	}
}

void CLoadPlatSDK::PLATSetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser)
{
	try
	{
		m_pMessageProc=cbMessageProc;
		m_dwMessageUser=dwMessageUser;
		if (dllSetCallBack !=NULL)
		{
			dllSetCallBack(cbMessageProc,dwMessageUser);
		}		
	}
	catch (...)
	{
		WriteLogz("SetCallBack faild...");
	}	
}

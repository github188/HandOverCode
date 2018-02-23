// Plat_SDKz.cpp: implementation of the CPlat_SDKz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cbkplatsdkz.h"
#include "Plat_SDKz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlat_SDKz::CPlat_SDKz()
{
	m_pMessageProc = NULL;
	hThreadEvent=NULL;
	m_iLoginID=-1;

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
	logfilenamez.Format("%s\\CBKPlatSDKz.log",strPath);
}

CPlat_SDKz::~CPlat_SDKz()
{

}

DWORD CPlat_SDKz::ThreadMethod()
{
	try
	{
		InitPlat();		
		hThreadEvent =CreateEvent(NULL,FALSE,FALSE,NULL);
		SetEvent(hThreadEvent);
		
		//////////////////////////////////////////////////////////////////////////
		while(1)
		{
			WaitForSingleObject(hThreadEvent,INFINITE);
			if (m_iLoginID ==-1)
			{
				m_iLoginID=Plat_LoginCMS("172.16.1.102","admin","bkzy@123","6600");		//µÇÂ¼CMS·þÎñ				
			}
			if (m_iLoginID ==-1)
			{
				WriteLogz("Plat_LoginCMS faild!Plat_GetLastError=%d",Plat_GetLastError());
				for (int  i=0;i<WND_MAXNUM;i++)
				{
					if (bGetUrl[i]==true)
					{
						if (m_pMessageProc!=NULL)
						{
							m_pMessageProc(i,PLAT_URLFAILD,0,m_dwMessageUser);
						}
						bGetUrl[i]=false;
					}
				}
			}
			else
			{
				for (int  i=0;i<WND_MAXNUM;i++)
				{
					if (bGetUrl[i]==true)
					{
						GetRealStreamCamerUrl(i);
						bGetUrl[i]=false;
					}
				}
			}

		}
		//////////////////////////////////////////////////////////////////////////
		FreePlat();
	}
	catch (...)
	{
		//WriteLogz("~CPlatSDKz() catch");
	}
	return 0;
}

void CPlat_SDKz::InitPlat()
{
	Plat_Init();
	m_iLoginID=-1;
	for (int i=0;i<WND_MAXNUM;i++)
	{
		m_PlayHStream[i]=-1;
		bGetUrl[i]=false;
	}
}

void CPlat_SDKz::FreePlat()
{
	if (m_iLoginID!=-1)
	{
		Plat_LogoutCMS(m_iLoginID);
	}
	Plat_Free();
}

void CPlat_SDKz::GetRealStreamCamerUrl(int iwnd)
{
	try
	{
		RealStreamURL[iwnd]=Plat_QueryRealStreamURL(m_csCameraID[iwnd],m_iLoginID);
		if (RealStreamURL[iwnd].IsEmpty())
		{
			WriteLogz("Plat_QueryRealStreamURL faild!Eid=%d,",Plat_GetLastError(),m_csCameraID[iwnd]);
			if (m_pMessageProc!=NULL)
			{
				m_pMessageProc(iwnd,PLAT_URLFAILD,0,m_dwMessageUser);
			}
		}
		else
		{			
			if (m_pMessageProc!=NULL)
			{
				m_pMessageProc(iwnd,PLAT_URLOK,0,m_dwMessageUser);
			}
		}
	}
	catch (...)
	{
		if (m_pMessageProc!=NULL)
		{
			m_pMessageProc(iwnd,PLAT_URLFAILD,0,m_dwMessageUser);
		}
	}
}

BOOL CPlat_SDKz::PlayVideo(int iWindID,long lWnd)
{
	try
	{
		StopPlay(iWindID);
		m_PlayHStream[iWindID]=Plat_PlayVideo(RealStreamURL[iWindID],lWnd,m_iLoginID);
		if (m_PlayHStream[iWindID]==-1)
		{
			WriteLogz("fn PlayVideo(%d,%ld) faild! LastError=%d",iWindID,lWnd,Plat_GetLastError());
			return FALSE;
		}
	}
	catch (...)
	{
		WriteLogz("fn PlayVideo() faild! catch..."); 
		return FALSE;
	}
	return TRUE;
}

void CPlat_SDKz::StopPlay(int iWindID)
{
	try
	{		
		if (m_PlayHStream[iWindID]!=-1)
		{	
			Plat_StopVideo(m_PlayHStream[iWindID]);
			m_PlayHStream[iWindID]=-1;
		}
	}
	catch (...)
	{
		WriteLogz("fn StopPlay() faild! catch..."); 
	}
}

void CPlat_SDKz::SetStreamCamer(LPCTSTR csCameraID, int iWnd)
{
	m_csCameraID[iWnd].Format("%s",csCameraID);
	bGetUrl[iWnd]=true;
	SetEvent(hThreadEvent);
}

void CPlat_SDKz::SetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser)
{
	m_pMessageProc = cbMessageProc;
	m_dwMessageUser = dwMessageUser;
}

void CPlat_SDKz::WriteLogz(LPCTSTR pstrFormat, ...)
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
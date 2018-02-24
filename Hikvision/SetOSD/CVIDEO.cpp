// CVIDEO.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CVIDEO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCVIDEOApp

BEGIN_MESSAGE_MAP(CCVIDEOApp, CWinApp)
	//{{AFX_MSG_MAP(CCVIDEOApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCVIDEOApp construction

CCVIDEOApp::CCVIDEOApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCVIDEOApp object

CCVIDEOApp theApp;

BOOL CCVIDEOApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempPath,strPath,ePath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	ePath.Format("%s",tempPath.Left(tempPath.ReverseFind('\\')));//当前目录

	strPath.Format("%s\\log",ePath);
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
	logfilename.Format("%s\\字符叠加日志.log",tempPath);
	
	
	Videocfg.Format("%s\\CVideo.cfg",ePath);
	//////////////////////////////////////////////////////////////////////////
	//车载视频服务器IP地址、用户名、密码
	CString temp;
	::GetPrivateProfileString("VIDEO", "IP", NULL, temp.GetBuffer(24), 24, Videocfg);
	temp.ReleaseBuffer();
	if (temp.IsEmpty())//如果配置为空
	{
		Videocfg.Format("%s\\config.dat",ePath);
		TRACE("Empty!");		
		::GetPrivateProfileString("VIDEO", "IP", NULL, temp.GetBuffer(24), 24, Videocfg);
		sIPaddr.Format("%s",temp);//车载DVR地址  
		
		::GetPrivateProfileString("VIDEO", "USER", NULL, temp.GetBuffer(24), 24, Videocfg);
		sUser.Format("%s",temp);  //车载DVR用户名
		
		::GetPrivateProfileString("VIDEO", "PWD", NULL, temp.GetBuffer(24), 24, Videocfg);
		sPwd.Format("%s",temp);  //车载DVR密码 
		
		::GetPrivateProfileString("VIDEO", "TD", NULL, temp.GetBuffer(24), 24, Videocfg);
		sTD.Format("%s",temp); //车载DVR通道

		Videocfg.Format("%s\\CVideo.cfg",ePath);
		::WritePrivateProfileString("VIDEO","IP",sIPaddr,Videocfg);//车载DVR地址
		::WritePrivateProfileString("VIDEO","USER",sUser,Videocfg);//车载DVR用户名
		::WritePrivateProfileString("VIDEO","PWD",sPwd,Videocfg);//车载DVR密码
		::WritePrivateProfileString("VIDEO","TD",sTD,Videocfg);//车载DVR通道		
	}
	else
	{
		::GetPrivateProfileString("VIDEO", "IP", NULL, temp.GetBuffer(24), 24, Videocfg);
		sIPaddr.Format("%s",temp);//车载DVR地址  
		
		::GetPrivateProfileString("VIDEO", "USER", NULL, temp.GetBuffer(24), 24, Videocfg);
		sUser.Format("%s",temp);  //车载DVR用户名
		
		::GetPrivateProfileString("VIDEO", "PWD", NULL, temp.GetBuffer(24), 24, Videocfg);
		sPwd.Format("%s",temp);  //车载DVR密码 
		
		::GetPrivateProfileString("VIDEO", "TD", NULL, temp.GetBuffer(24), 24, Videocfg);
		sTD.Format("%s",temp); //车载DVR通道
	}
	m_iChanShowNum=atoi(sTD);
	//////////////////////////////////////////////////////////////////////////
	NET_DVR_Init();
	exGet=FALSE;
	memset(&m_struShowString, 0, sizeof(m_struShowString));
	WriteLog("InitInstance CVIDEO.dll Build 20171028 1937");	
	
 	CString tempX,tempY;
// 	for (int i = 0 ; i < 8 ; i++)
// 	{
// 		temp.Format("L%d",i);	
// 		strX[i]=GetPrivateProfileInt("strX",temp,0,Videocfg);
// 		strY[i]=GetPrivateProfileInt("strY",temp,0,Videocfg);
// 		if (strX[i]==0)
// 		{
// 			strX[i]=60;
// 			tempX=_T("60");
// 			::WritePrivateProfileString("strX",temp,tempX,Videocfg);
// 		}
// 		if (strY[i]==0)
// 		{
// 			strY[i]=30*i+60;
// 			tempY.Format("%d",strY[i]);
// 			::WritePrivateProfileString("strY",temp,tempY,Videocfg);
// 		}
// 	}
	OnLoginHK();
	GetDVRConfig();
	UINT tempcardnum;
	tempcardnum=GetPrivateProfileInt("ICODE","Icode",99,".\\config.dat");
	tempX.Format("考车%d ",tempcardnum+1);
	OnShowString(tempX,0);
	return CWinApp::InitInstance();
}

int CCVIDEOApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	WriteLog("ExitInstance CVIDEO DLL!");
//	NET_DVR_Cleanup();
	return CWinApp::ExitInstance();
}

void CCVIDEOApp::WriteLog(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	CTime curTime =CTime::GetCurrentTime();
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	TRACE(logstr);
	va_end(avlist);
	FILE *fp=fopen(logfilename,"a+");
	fprintf(fp,"[%s]:%s#\n",curTime.Format(_T("%H:%M:%S")),logstr);
	fclose(fp);
}

BOOL CCVIDEOApp::OnLoginHK()
{
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//存放设备参数的结构体
	m_lLoginID=-1;
	m_lLoginID = NET_DVR_Login_V30((LPSTR)(LPCTSTR)sIPaddr,8000,(LPSTR)(LPCTSTR)sUser,(LPSTR)(LPCTSTR)sPwd, &struDeviceInfo);
	if (m_lLoginID < 0)
	{
		WriteLog("登录视频服务器失败!Eid=%d,IP=%s,%s,%s",NET_DVR_GetLastError(),sIPaddr,sUser,sPwd);
		return FALSE;
	}
	WriteLog("登录成功!%s",sIPaddr);
	return TRUE;
}

BOOL CCVIDEOApp::OnShowString(CString str, UINT uLine)
{
//	DWORD dwReturned;	
	if (uLine > 8)
	{
		WriteLog("字符行只能设置 0到7 ,共8行,不能是==》%d",uLine);
		return FALSE;
	}	
	WriteLog("测试 %s",str);
	BOOL m_bChkShow;
	m_bChkShow = TRUE;	
	m_struShowString.struStringInfo[uLine].wShowString = (WORD)m_bChkShow;
	memcpy(m_struShowString.struStringInfo[uLine].sString, str, 44);
	m_struShowString.struStringInfo[uLine].wStringSize = (WORD)strlen(str);
// 	m_struShowString.struStringInfo[uLine].wShowStringTopLeftX = (WORD)strX[uLine];
// 	m_struShowString.struStringInfo[uLine].wShowStringTopLeftY = (WORD)strY[uLine];	
// 	if (uLine ==7)
// 	{	
		if (!NET_DVR_SetDVRConfig(m_lLoginID, NET_DVR_SET_SHOWSTRING_V30,m_iChanShowNum, &m_struShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
		{
			WriteLog("设置字符参数失败!ErrorID=%d",NET_DVR_GetLastError());
			OnLoginHK();
			GetDVRConfig();
			return FALSE;
		}
		lSendTime=timeGetTime();
//	}
	return TRUE;
}


extern "C" __declspec( dllexport ) BOOL HK_SHOWSTRING(CString data,UINT uline)
{
// 	if (timeGetTime()-theApp.lSendTime<900)
// 	{
// 		return 0;
// 	}
	return theApp.OnShowString(data,uline);
}

extern "C" __declspec( dllexport ) BOOL HK_SHOWSTRINGEX(CString data,UINT uline,int uTD)
{
	// 	if (timeGetTime()-theApp.lSendTime<900)
	// 	{
	// 		return 0;
	// 	}
	return theApp.OnShowStringEx(data,uline,uTD);
}

BOOL CCVIDEOApp::GetDVRConfig()
{
	DWORD dwReturned;
	if (!NET_DVR_GetDVRConfig(m_lLoginID, NET_DVR_GET_SHOWSTRING_V30, m_iChanShowNum, &m_struShowString, sizeof(NET_DVR_SHOWSTRING_V30), &dwReturned))
	{
		WriteLog("NET_DVR_GetDVRConfig Faild! Errid=%d",NET_DVR_GetLastError());
		return FALSE;
	}

	CString tempX,tempY,temp;
	for (int i = 0 ; i < 8 ; i++)
	{
		temp.Format("L%d",i);	
		strX[i]=GetPrivateProfileInt("strX",temp,0,Videocfg);
		strY[i]=GetPrivateProfileInt("strY",temp,0,Videocfg);
		if (strX[i]==0)
		{
			tempX.Format("%d",m_struShowString.struStringInfo[i].wShowStringTopLeftX);
			::WritePrivateProfileString("strX",temp,tempX,Videocfg);
		}
		if (strY[i]==0)
		{
			tempY.Format("%d",m_struShowString.struStringInfo[i].wShowStringTopLeftY);
			::WritePrivateProfileString("strY",temp,tempY,Videocfg);
		}
	}
	return TRUE;
}

BOOL CCVIDEOApp::OnShowStringEx(CString str, UINT uLine,int iTD)
{
	//	DWORD dwReturned;	
	if (exGet==FALSE)
	{
		memset(&m_struShowStringEX, 0, sizeof(m_struShowStringEX));
		DWORD dwReturned;
		if (!NET_DVR_GetDVRConfig(m_lLoginID, NET_DVR_GET_SHOWSTRING_V30, iTD, &m_struShowStringEX, sizeof(NET_DVR_SHOWSTRING_V30), &dwReturned))
		{
			WriteLog("NET_DVR_GetDVRConfig Faild! Errid=%d,Ex iTD=%d",NET_DVR_GetLastError(),iTD);
			return FALSE;
		}
		exGet=TRUE;
	}
	if (uLine > 8)
	{
		WriteLog("字符行只能设置 0到7 ,共8行,不能是==》%d",uLine);
		return FALSE;
	}	
	WriteLog("OnShowStringEx %s,uLine=%d,iTD=%d",str,uLine,iTD);
	BOOL m_bChkShow;
	m_bChkShow = TRUE;	
	m_struShowStringEX.struStringInfo[uLine].wShowString = (WORD)m_bChkShow;
	memcpy(m_struShowStringEX.struStringInfo[uLine].sString, str, 44);
	m_struShowStringEX.struStringInfo[uLine].wStringSize = (WORD)strlen(str);

	if (!NET_DVR_SetDVRConfig(m_lLoginID, NET_DVR_SET_SHOWSTRING_V30,iTD, &m_struShowStringEX, sizeof(NET_DVR_SHOWSTRING_V30)))
	{
		WriteLog("OnShowStringEx设置字符参数失败!ErrorID=%d EX",NET_DVR_GetLastError());
		OnLoginHK();
		exGet=FALSE;
		return FALSE;
	}
//	lSendTime=timeGetTime();

	return TRUE;
}

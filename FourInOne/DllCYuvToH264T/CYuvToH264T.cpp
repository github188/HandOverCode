// CYuvToH264T.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CYuvToH264T.h"

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
// CCYuvToH264TApp
#define EXPORTAPIZ extern "C" __declspec(dllexport) 

BEGIN_MESSAGE_MAP(CCYuvToH264TApp, CWinApp)
	//{{AFX_MSG_MAP(CCYuvToH264TApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCYuvToH264TApp construction

CCYuvToH264TApp::CCYuvToH264TApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCYuvToH264TApp object

CCYuvToH264TApp theApp;

BOOL CCYuvToH264TApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempPath,tempt;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	exePath.Format("%s",tempPath.Left(tempPath.ReverseFind('\\')));//当前目录
	tempPath.Format("%s\\log",exePath);//当前目录下的
	DWORD dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	NowDataPath.Format("%s\\%s",tempPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(NowDataPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(NowDataPath,NULL);//
	}
	tempPath.Format("%s\\StuData",exePath);//当前目录下的
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	NowDataPath.Format("%s\\%s",tempPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(NowDataPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(NowDataPath,NULL);//
	}
	CoInitialize(NULL);//ADO com In
	tempPath.Format("%s\\config.ini",exePath);
	uIsqlororacle=GetPrivateProfileInt("CONFIG","SQLORACLE",1,tempPath);//SQL 还是 Oracle
	uLoadMap=GetPrivateProfileInt("CONFIG","LOADMAP",0,tempPath);//加载地图
	uKSKM=GetPrivateProfileInt("CONFIG","KSKM",2,tempPath);//考试科目
	UINT uSleepTime;
	uSleepTime=GetPrivateProfileInt("CONFIG","DituRRate",2000,tempPath);//地图刷新率
	if (uSleepTime<200)
	{
		dwSleepTime=300;
	}
	else
	{
		dwSleepTime=uSleepTime;
	}
	return CWinApp::InitInstance();
}


int CCYuvToH264TApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();//ADO com Un
	return CWinApp::ExitInstance();
}

DWORD CALLBACK  MessageProcFunc(LONG  lPassiveHandle,char *pSendBuf,DWORD dwBufSize)
{
	if(!NET_DVR_MatrixSendData(lPassiveHandle,pSendBuf,dwBufSize))
	{
		TRACE("NET_DVR_MatrixSendData faild %d \n",NET_DVR_GetLastError());
		return NET_DVR_GetLastError();
	}
	return 5403;
}

EXPORTAPIZ void TFInit(int ikch,int wMSG,HWND hwndz)
{
	theApp.m_TPic[ikch].ThirdPicInit(ikch,theApp.exePath,wMSG,hwndz);
	theApp.m_FPic[ikch].FourthPicInit(ikch,theApp.exePath,wMSG,hwndz);
	theApp.m_FPic[ikch].SetNowDataPath(theApp.NowDataPath,theApp.uKSKM);
	if (theApp.uLoadMap!=0)
	{
		theApp.m_FPic[ikch].LoadMapCfg(theApp.exePath);
	}
}
EXPORTAPIZ void TFPassiveHandle(LONG lPassHandle,int ikch,int itf)
{
	if (itf ==3)
	{
		theApp.m_TPic[ikch].SetCBMatrixSendData(MessageProcFunc,lPassHandle);
		theApp.m_TPic[ikch].SetSQLOracle(theApp.uIsqlororacle);
		theApp.m_TPic[ikch].Start();
	}
	else
	{
		theApp.m_FPic[ikch].SetCBMatrixSendData(MessageProcFunc,lPassHandle);
		theApp.m_FPic[ikch].SetSleepTime(theApp.dwSleepTime);
		theApp.m_FPic[ikch].Start();
	}
}

EXPORTAPIZ void TF17C51(int ikch,CString zkzm,int ikscs,int idrcs)
{
	theApp.m_TPic[ikch].On17C51(zkzm,ikscs,idrcs);
	theApp.m_FPic[ikch].On17C51(zkzm,ikscs,idrcs);
}

EXPORTAPIZ void TF17C52(int ikch,CString zkzm, int ic, CString msgz)
{
	theApp.m_TPic[ikch].On17C52(zkzm);
	theApp.m_FPic[ikch].On17C52(zkzm,ic,msgz);
}

EXPORTAPIZ void TF17C53(int ikch,CString timestr, CString msgz, int ikcfs)
{
	theApp.m_FPic[ikch].On17C53(timestr,msgz,ikcfs);
}

EXPORTAPIZ void TF17C55(int ikch,int ic, CString msgz)
{
	theApp.m_FPic[ikch].On17C55(ic,msgz);
}

EXPORTAPIZ void TF17C56(int ikch,int itype, int ikscj)
{
	theApp.m_TPic[ikch].On17C56(itype,ikscj);
	theApp.m_FPic[ikch].On17C56(itype,ikscj);
}

EXPORTAPIZ void TF17C54(int ikch,LPVOID msgz)
{
	theApp.m_FPic[ikch].OnGnssData(msgz);
}
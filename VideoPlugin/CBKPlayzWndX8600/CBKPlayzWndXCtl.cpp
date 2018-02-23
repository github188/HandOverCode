// CBKPlayzWndXCtl.cpp : Implementation of the CCBKPlayzWndXCtrl ActiveX Control class.

#include "stdafx.h"
#include "CBKPlayzWndX.h"
#include "CBKPlayzWndXCtl.h"
#include "CBKPlayzWndXPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_THREADDLLMSG		WM_APP + 101

IMPLEMENT_DYNCREATE(CCBKPlayzWndXCtrl, COleControl)

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCBKPlayzWndXCtrl, COleControl)
	//{{AFX_MSG_MAP(CCBKPlayzWndXCtrl)
	ON_WM_CREATE()
	ON_MESSAGE(WM_THREADDLLMSG,OnThreadDLLMsg)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCBKPlayzWndXCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCBKPlayzWndXCtrl)
	DISP_FUNCTION(CCBKPlayzWndXCtrl, "StopPlay", StopPlay, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCBKPlayzWndXCtrl, "SetMultiWnd", SetMultiWnd, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CCBKPlayzWndXCtrl, "SetRealStreamID", SetRealStreamID, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCBKPlayzWndXCtrl, "ShowRgppDlg", ShowRgppDlg, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCBKPlayzWndXCtrl, "SetDeviceInfo", SetDeviceInfo, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CCBKPlayzWndXCtrl, "SetPlayBackByTime", SetPlayBackByTime, VT_BOOL, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCBKPlayzWndXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCBKPlayzWndXCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCBKPlayzWndXCtrl)
	EVENT_CUSTOM("RenGongPingPan", FireRenGongPingPan, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("ClickRgppMenu", FireClickRgppMenu, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("RClickMenu", FireRClickMenu, VTS_BSTR  VTS_I2  VTS_I2)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCBKPlayzWndXCtrl, 1)
	PROPPAGEID(CCBKPlayzWndXPropPage::guid)
END_PROPPAGEIDS(CCBKPlayzWndXCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCBKPlayzWndXCtrl, "CBKPLAYZWNDX.CBKPlayzWndXCtrl.1",
	0xd3b921a4, 0xe5d7, 0x48f4, 0x80, 0x83, 0xcf, 0xea, 0x6e, 0x2b, 0x64, 0x74)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCBKPlayzWndXCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCBKPlayzWndX =
		{ 0x3027a138, 0xdf57, 0x420c, { 0x83, 0xae, 0x5b, 0xfd, 0x94, 0x4e, 0xf4, 0x22 } };
const IID BASED_CODE IID_DCBKPlayzWndXEvents =
		{ 0xb53f2586, 0x611f, 0x4444, { 0xb7, 0xc3, 0x11, 0, 0x5d, 0xc, 0x57, 0xa8 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCBKPlayzWndXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCBKPlayzWndXCtrl, IDS_CBKPLAYZWNDX, _dwCBKPlayzWndXOleMisc)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//Interface map for IObjectSafety

BEGIN_INTERFACE_MAP(CCBKPlayzWndXCtrl, COleControl )
INTERFACE_PART(CCBKPlayzWndXCtrl, IID_IObjectSafety, ObjSafe)    
END_INTERFACE_MAP()    

///////////////////////////////////////////////////////////////////////////
//IObjectSafety member functions

//Delegate AddRef, Release, QueryInterface

ULONG FAR EXPORT CCBKPlayzWndXCtrl::XObjSafe::AddRef()
{
	METHOD_PROLOGUE(CCBKPlayzWndXCtrl, ObjSafe)
		return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CCBKPlayzWndXCtrl::XObjSafe::Release()
{
	METHOD_PROLOGUE(CCBKPlayzWndXCtrl, ObjSafe)
		return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CCBKPlayzWndXCtrl::XObjSafe::QueryInterface(
													  REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CCBKPlayzWndXCtrl, ObjSafe)
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

const DWORD dwSupportedBits =
INTERFACESAFE_FOR_UNTRUSTED_CALLER |
INTERFACESAFE_FOR_UNTRUSTED_DATA;
const DWORD dwNotSupportedBits = ~ dwSupportedBits;

///////////////////////////////////////////////////////////////////////////
// CStopLiteCtrl::XObjSafe::GetInterfaceSafetyOptions
// Allows container to query what interfaces are safe for what. We’re
// optimizing significantly by ignoring which interface the caller is
// asking for.
HRESULT STDMETHODCALLTYPE
CCBKPlayzWndXCtrl::XObjSafe::GetInterfaceSafetyOptions(
											  REFIID riid,
											  DWORD __RPC_FAR *pdwSupportedOptions,
											  DWORD __RPC_FAR *pdwEnabledOptions)
{
	METHOD_PROLOGUE(CCBKPlayzWndXCtrl, ObjSafe)
		
		HRESULT retval = ResultFromScode(S_OK);
	
	// does interface exist?
	IUnknown FAR* punkInterface;
	retval = pThis->ExternalQueryInterface(&riid,
		(void * *)&punkInterface);
	if (retval != E_NOINTERFACE) { // interface exists
		punkInterface->Release(); // release it–just checking!
	}
	
	// we support both kinds of safety and have always both set,
	// regardless of interface
	*pdwSupportedOptions = *pdwEnabledOptions = dwSupportedBits;
	
	return retval; // E_NOINTERFACE if QI failed
}

///////////////////////////////////////////////////////////////////////////
// CStopLiteCtrl::XObjSafe::SetInterfaceSafetyOptions
// Since we’re always safe, this is a no-brainer–but we do check to make
// sure the interface requested exists and that the options we’re asked to
// set exist and are set on (we don’t support unsafe mode).
HRESULT STDMETHODCALLTYPE
CCBKPlayzWndXCtrl::XObjSafe::SetInterfaceSafetyOptions(
											  REFIID riid,
											  DWORD dwOptionSetMask,
											  DWORD dwEnabledOptions)
{
	METHOD_PROLOGUE(CCBKPlayzWndXCtrl, ObjSafe)
		
		// does interface exist?
		IUnknown FAR* punkInterface;
	pThis->ExternalQueryInterface(&riid, (void * *)&punkInterface);
	if (punkInterface) { // interface exists
		punkInterface->Release(); // release it–just checking!
	}
	else { // interface doesn’t exist
		return ResultFromScode(E_NOINTERFACE);
	}
	
	// can’t set bits we don’t support
	if (dwOptionSetMask & dwNotSupportedBits) {
		return ResultFromScode(E_FAIL);
	}
	
	// can’t set bits we do support to zero
	dwEnabledOptions &= dwSupportedBits;
	// (we already know there are no extra bits in mask )
	if ((dwOptionSetMask & dwEnabledOptions) !=
		dwOptionSetMask) {
		return ResultFromScode(E_FAIL);
	}       
	
	// don’t need to change anything since we’re always safe
	return ResultFromScode(S_OK);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::CCBKPlayzWndXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCBKPlayzWndXCtrl

BOOL CCBKPlayzWndXCtrl::CCBKPlayzWndXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CBKPLAYZWNDX,
			IDB_CBKPLAYZWNDX,
			afxRegApartmentThreading,
			_dwCBKPlayzWndXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::CCBKPlayzWndXCtrl - Constructor

CCBKPlayzWndXCtrl::CCBKPlayzWndXCtrl()
{
	InitializeIIDs(&IID_DCBKPlayzWndX, &IID_DCBKPlayzWndXEvents);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::~CCBKPlayzWndXCtrl - Destructor

CCBKPlayzWndXCtrl::~CCBKPlayzWndXCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::OnDraw - Drawing function

void CCBKPlayzWndXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
// 	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
// 	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::DoPropExchange - Persistence support

void CCBKPlayzWndXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::OnResetState - Reset control to default state

void CCBKPlayzWndXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl::AboutBox - Display an "About" box to the user

void CCBKPlayzWndXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CBKPLAYZWNDX);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXCtrl message handlers
//自定义消息回调 
void CALLBACK  MessageProcFunc(UINT nWndID,WPARAM wParam,LPARAM lParam,DWORD dwUser)
{
	if(dwUser != 0)
	{
		CCBKPlayzWndXCtrl *dlg = (CCBKPlayzWndXCtrl *)dwUser;
		dlg->MessageProc(nWndID,wParam,lParam);
	}	
}

void CCBKPlayzWndXCtrl::MessageProc(UINT nWndID,WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case VIDEO_MENU_RENGONGPP://人工评判
		{			
			FireClickRgppMenu(m_strKchp[nWndID],nWndID);
		}
		break;
	case VIDEO_MENU_VoiceCom:
		{
			if (lParam==0)//打开对讲
			{
				FireRClickMenu(m_strKchp[nWndID],nWndID,3);
			}
			else//停止对讲
			{
				m_LoadHikSdk.SDKStopVoiceCom(nWndID);
				m_screenPannel.SetPlayWndState(BSDKVoiceCom,false);
			}
		}
		break;
	case VIDEO_MENU_PlaySound:
		{			
			int tempPs=m_screenPannel.GetPlayWndState();			
			if (tempPs & BPlatRealPlayz)
			{
				//流媒体取流预览
				m_LoadPlatSdk.PLATStopVideo(nWndID);
				m_screenPannel.SetPlayWndState(BPlatRealPlayz,false);

				FireRClickMenu(m_strKchp[nWndID],nWndID,1);
				//如果是通过流ID取流。先切换成 直连取流再打开声音
			}
			//打开声音
			if (lParam==0)
			{
				m_LoadHikSdk.SDKOpenSoundShare(nWndID);
				m_screenPannel.SetPlayWndState(BSDKPlaySound);
			}
			else
			{
				m_LoadHikSdk.SDKCloseSoundShare(nWndID);
				m_screenPannel.SetPlayWndState(BSDKPlaySound,false);
			}
		}
		break;
	case VIDEO_MENU_STOPPlay:
		StopPlay();
		break;
	case PLAT_URLOK:
		{	
			if(m_LoadPlatSdk.PLATPlayVideo(nWndID,(long)m_screenPannel.GetPage(nWndID)->m_hWnd)==TRUE)
			{
				m_screenPannel.SetPlayWndState(BPlatRealPlayz);
			}
		}
		break;
	case  PLAT_URLFAILD:
		{
			AfxMessageBox("流ID取流失败!");	
			PostMessage(WM_THREADDLLMSG,(WPARAM)nWndID,0);
			
			//FireRClickMenu(m_strKchp[nWndID],nWndID,3);	
 			m_screenPannel.SetPlayWndThiS("[%s]-->直连设备取流.请稍后!",m_strKchp[nWndID]);		
		}
		break;
	default:
		break;
	}
	
}

//////////////////////////////////////////////////////////////////////////

int CCBKPlayzWndXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//////////////////////////////////////////////////////////////////////////
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
	logfilenamez.Format("%s\\BKPlayzWndX.log",strPath);
 
	m_screenPannel.Create(
		NULL,
		NULL,
		WS_CHILD|WS_VISIBLE, 
		CRect(0,0,0,0), 
		this, 
		1988);
	GetClientRect(&m_screenRect);
	m_screenPannel.MoveWindow(m_screenRect);
	m_screenPannel.SetCallBack(MessageProcFunc,(DWORD)this);
	if(true==m_LoadHikSdk.InitLoadDll("D:\\bkzyVideoOcx\\hikVision\\CHikVisionz.dll"))
	{
		for (int i=0;i<WND_MAXNUM;i++)
		{
			m_LoadHikSdk.SDKInitWnd(((CWnd *)(m_screenPannel.GetPage(i)))->m_hWnd,0,i);
		}
	}	
	m_screenPannel.SetShowPlayWin(SPLIT1);	
	//////////////////////////////////////////////////////////////////////////
	//动态加载 8600平台接口
	m_LoadPlatSdk.InitLoadDll();
	m_LoadPlatSdk.PLATSetCallBack(MessageProcFunc,(DWORD)this);
	//////////////////////////////////////////////////////////////////////////
	return 0;
}

// BOOL CCBKPlayzWndXCtrl::StartPlay() 
// {
// 	// TODO: Add your dispatch handler code here
// /*	int iWindID=m_screenPannel.GetCurWindId();
// 	int iPlayState= m_screenPannel.GetPlayWndState();
// 	if (iPlayState & BWaitPlatRealPlayz)
// 	{
// 		m_screenPannel.SetPlayWndState(BWaitPlatRealPlayz,false);
// //		if(m_PlatSDKz.PlayVideo(iWindID,(long)m_screenPannel.GetPage(iWindID)->m_hWnd)==TRUE)
// //		{
// 			m_screenPannel.SetPlayWndState(BPlatRealPlayz);
// 			return TRUE;
// //		}
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	if (iPlayState & BWaitHCSDKPlayz)
// 	{		
// 		m_screenPannel.SetPlayWndThiS("[%s]-->直连设备取流!",m_strKchp[iWindID]);
// 		m_screenPannel.SetPlayWndState(BWaitHCSDKPlayz,false);
// 		//////////////////////////////////////////////////////////////////////////
// 		if (m_HCNetSDKz.PlayVideo(m_screenPannel.GetPage(iWindID)->m_hWnd,iWindID)==TRUE)
// 		{
// 			m_screenPannel.SetPlayWndState(BHCSDKPlayz);
// 			return TRUE;
// 		}
// 	}*/
// 
// }

BOOL CCBKPlayzWndXCtrl::StopPlay() 
{
	// TODO: Add your dispatch handler code here
	int tempPs=m_screenPannel.GetPlayWndState();
	int iWindId=m_screenPannel.GetCurWindId();

	if (tempPs & BPlatRealPlayz)
	{
		//流媒体取流预览
		m_LoadPlatSdk.PLATStopVideo(iWindId);
		m_screenPannel.SetPlayWndState(BPlatRealPlayz,false);
	}
	if (tempPs & BSDKRealPlayz)
	{
		if (tempPs & BSDKVoiceCom)
		{
			m_LoadHikSdk.SDKStopVoiceCom(iWindId);
			m_screenPannel.SetPlayWndState(BSDKVoiceCom,false);
		}
		//SDK直连取流预览
		m_LoadHikSdk.SDKStopPlay(iWindId);
		m_screenPannel.SetPlayWndState(BSDKRealPlayz,false);
	}
	if (tempPs & BSDKPlayBackz)
	{
		//SDK视频回放
//		m_HCNetSDKz.StopPlay(iWindId);
		m_screenPannel.SetPlayWndState(BSDKPlayBackz,false);
	}
 	m_screenPannel.SetPlayWndState(BNoPlayZ);
	m_screenPannel.SetPlayWndThiS("");
	m_screenPannel.GetPage(iWindId)->Invalidate(TRUE);
	return TRUE;
}

void CCBKPlayzWndXCtrl::SetMultiWnd(short nSplit) 
{
	// TODO: Add your dispatch handler code here
	m_screenPannel.SwitchMultiWnd(nSplit);
}

// BOOL CCBKPlayzWndXCtrl::SetPlayBackByTime(LPCTSTR strSTime, LPCTSTR strETime) 
// {
// 	// TODO: Add your dispatch handler code here
// /*	NET_DVR_TIME struStartTime, struStopTime;
// 	if (StrToDVRTime(struStartTime,strSTime)==FALSE)
// 	{
// 		return FALSE;
// 	}
// 	if (StrToDVRTime(struStopTime,strETime)==FALSE)
// 	{
// 		return FALSE;
// 	}
// 	int m_CurWindId;			//当前窗口ID
// 	m_CurWindId=m_screenPannel.GetCurWindId();//获取当前窗口ID
// 	StopPlay();//先关闭播放	
// 	//////////////////////////////////////////////////////////////////////////
// 	m_Video_PZ[m_CurWindId].lPlayBackHandle = NET_DVR_PlayBackByTime(m_Video_PZ[m_CurWindId].m_lUserID, m_Video_PZ[m_CurWindId].dwChannel,\
// 		&struStartTime,&struStopTime, ((CWnd *)(m_screenPannel.GetPage(m_CurWindId)))->m_hWnd);
// 	if(-1 == m_Video_PZ[m_CurWindId].lPlayBackHandle)
// 	{
// 		CString temp;
// 		temp.Format("按时间回放失败!Eid=%d",NET_DVR_GetLastError());
// 		MessageBox(temp,"提示");
// 		return FALSE;
// 	}*/
// 	return TRUE;
// }

// BOOL CCBKPlayzWndXCtrl::VPlayBackControl(long dwControlCode) 
// {
// 	// TODO: Add your dispatch handler code here
// /*	int m_CurWindId;			//当前窗口ID
// 	m_CurWindId=m_screenPannel.GetCurWindId();//获取当前窗口ID
// 	if (m_Video_PZ[m_CurWindId].lPlayBackHandle==-1)
// 	{
// 		MessageBox("按时间回放失败,先设置回放时间","提示");
// 		return FALSE;
// 	}
// 	//NET_DVR_PLAYSTART
// 	if (!NET_DVR_PlayBackControl(m_Video_PZ[m_CurWindId].lPlayBackHandle, dwControlCode, 0, NULL))
// 	{
// 		return FALSE;
// 	}*/
// 	return TRUE;
// }
/*
BOOL CCBKPlayzWndXCtrl::StrToDVRTime(NET_DVR_TIME &rTime, LPCTSTR lTime)
{
	CString temp;
	temp.Format("%s",lTime);	
	if ( 19 != temp.GetLength() )
	{
		return FALSE;
	}
	rTime.dwYear   = atoi(temp.Mid(0,4));
	rTime.dwMonth  = atoi(temp.Mid(5,2));
	rTime.dwDay    = atoi(temp.Mid(8,2));
	rTime.dwHour   = atoi(temp.Mid(11,2));
	rTime.dwMinute = atoi(temp.Mid(14,2));
	rTime.dwSecond = atoi(temp.Mid(17,2));
	return TRUE;
}
*/

BOOL CCBKPlayzWndXCtrl::SetRealStreamID(LPCTSTR strKchp, LPCTSTR csCameraID) 
{
	// TODO: Add your dispatch handler code here
	StopPlay();
	int CurWindId=m_screenPannel.GetCurWindId();	//获取当前窗口ID
	m_strKchp[CurWindId].Format("%s",strKchp);		//保存考车号牌
	m_screenPannel.SetPlayWndThiS("[%s]->流媒体转发平台取流![%d]",strKchp,CurWindId);	
	Invalidate();
	m_LoadPlatSdk.PLATSetStreamCamer(csCameraID,CurWindId);
	return TRUE;
}

void CCBKPlayzWndXCtrl::ShowRgppDlg(LPCTSTR strKchp, LPCTSTR strStuName, LPCTSTR strStuZkzm, LPCTSTR strReturn) 
{
	// TODO: Add your dispatch handler code here
	try
	{
		//AfxMessageBox("加载 人工评判 DLL !"); 
		//////////////////////////////////////////////////////////////////////////
		//人工评判 DLL
		typedef int (* ShowRGPPDlg)(LPTSTR,LPTSTR,LPTSTR);
		ShowRGPPDlg dllRgppdlg;
		HINSTANCE dllHRGPP;
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		dllHRGPP = LoadLibraryEx(TEXT("D:\\bkzyVideoOcx\\CKM3RGPP.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH); 
		if (dllHRGPP == NULL) 
		{ 
			AfxMessageBox("加载 人工评判 DLL 失败!"); 
			WriteLogz("加载 D:\\bkzyVideoOcx\\CKM3RGPP.dll 失败!"); 
			return ; 
		}	
		dllRgppdlg = (ShowRGPPDlg) GetProcAddress(dllHRGPP,TEXT("ShowRGPPDlg"));
		if (dllRgppdlg!=NULL)
		{			
			int iRgppCode=0;
			iRgppCode=dllRgppdlg((char *)strKchp,(char *)strStuName,(char *)strStuZkzm);
			if (iRgppCode!=0)
			{				
				WriteLogz("人工评判:%d,%s,%s,%s",iRgppCode,strKchp,strStuName,strStuZkzm);
				FireRenGongPingPan(strReturn,iRgppCode);
			}
		}
		FreeLibrary(dllHRGPP);
	}
	catch (...)
	{
		WriteLogz("Exception of ShowRgppDlg(%s,%s,%s)",strKchp,strStuName,strStuZkzm);
	}
}

// BOOL CCBKPlayzWndXCtrl::SetPlayBackInfo(LPCTSTR strDvrIP, LPCTSTR strUserName, LPCTSTR strPwd, long nPort, short nChannel) 
// {
// 	// TODO: Add your dispatch handler code here
// // 	if (m_HCNetSDKz.fnDVR_Login_V30(strDvrIP,strUserName,strPwd,nPort,nChannel,m_screenPannel.GetCurWindId())==FALSE)
// // 	{
// // 		return FALSE;
// // 	}
// 	m_screenPannel.SetPlayWndState(BWaitSDKPlayBackz);
// 	return TRUE;
// }


void CCBKPlayzWndXCtrl::SetDeviceInfo(LPCTSTR strIP, LPCTSTR strUser, LPCTSTR strPwd, short nPort, short nChannel, short nType) 
{
	// TODO: Add your dispatch handler code here
	int nWndID=m_screenPannel.GetCurWindId();
	if (nType ==1)
	{
		//预览
		if (m_LoadHikSdk.SDKDVRLogin(strIP,strUser,strPwd,nPort,nChannel,nWndID)==TRUE)
		{
			if (TRUE==m_LoadHikSdk.SDKRealPlay(nWndID))
			{
				m_screenPannel.SetPlayWndState(BSDKRealPlayz);
			}
		}
	}
	else if (nType ==2)
	{
		//回放
		if (m_LoadHikSdk.SDKDVRLogin(strIP,strUser,strPwd,nPort,nChannel,nWndID)==TRUE)
		{
			m_screenPannel.SetPlayWndState(BWSDKPlayBackz);
		}

	}
	else if (nType ==3)
	{
		//对讲
		if (m_LoadHikSdk.SDKStartVoiceCom_V30(strIP,strUser,strPwd,nPort,nChannel,nWndID)==TRUE)
		{
			m_screenPannel.SetPlayWndState(BSDKVoiceCom);
		}
	}
	else
	{

	}
}

BOOL CCBKPlayzWndXCtrl::SetPlayBackByTime(LPCTSTR strSTime, LPCTSTR strETime) 
{
	// TODO: Add your dispatch handler code here

	int tempPs=m_screenPannel.GetPlayWndState();
	int iWindId=m_screenPannel.GetCurWindId();
	
	if (tempPs & BWSDKPlayBackz)
	{
		m_screenPannel.SetPlayWndState(BWSDKPlayBackz,false);
		if (m_LoadHikSdk.SDKPlayBackbyTime(strSTime,strETime,iWindId)==TRUE)
		{
			m_screenPannel.SetPlayWndState(BSDKPlayBackz);
			return TRUE;
		}		
	}
	return FALSE;
}

void CCBKPlayzWndXCtrl::OnThreadDLLMsg(WPARAM wParam,LPARAM lParam)
{
	try
	{		
		int nWndID;
		nWndID=(int )wParam;
		FireRClickMenu(m_strKchp[nWndID],nWndID,1);
	}
	catch (...)
	{
		WriteLogz("OnThreadDLLMsg catch ...faild!");
	}
}
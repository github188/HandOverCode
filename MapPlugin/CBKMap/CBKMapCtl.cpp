// CBKMapCtl.cpp : Implementation of the CCBKMapCtrl ActiveX Control class.

#include "stdafx.h"
#include "CBKMap.h"
#include "CBKMapCtl.h"
#include "CBKMapPpg.h"
#include <algorithm>
#include "WideCharString.h"

#define   COMPILE_MULTIMON_STUBS   
#include   <multimon.h>   
#pragma   warning(default:4706) 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCBKMapCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCBKMapCtrl, COleControl)
	//{{AFX_MSG_MAP(CCBKMapCtrl)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_MESSAGE(WM_TOOLDLG_MSG,OnToolzDlgMsg)
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(VIDEO_MENU_BASE, VIDEO_MENU_END, OnVideoMenu)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCBKMapCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCBKMapCtrl)
	DISP_FUNCTION(CCBKMapCtrl, "LoadMapFile", LoadMapFile, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCBKMapCtrl, "PlayBackMap", PlayBackMap, VT_EMPTY, VTS_BSTR VTS_I2 VTS_I2)
	DISP_FUNCTION(CCBKMapCtrl, "SetTongJi", SetTongJi, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_R4)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCBKMapCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCBKMapCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCBKMapCtrl)
	EVENT_CUSTOM("GetTongJi", FireGetTongJi, VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCBKMapCtrl, 1)
	PROPPAGEID(CCBKMapPropPage::guid)
END_PROPPAGEIDS(CCBKMapCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCBKMapCtrl, "CBKMAP.CBKMapCtrl.1",
	0xd8809dc2, 0x92bb, 0x4f79, 0x9e, 0xa3, 0x58, 0xd9, 0xa8, 0xef, 0x9, 0xe2)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCBKMapCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCBKMap =
		{ 0x930256fd, 0x76f6, 0x4afb, { 0x8d, 0x35, 0x84, 0xce, 0x38, 0xea, 0x22, 0xf9 } };
const IID BASED_CODE IID_DCBKMapEvents =
		{ 0x61211648, 0xb7b2, 0x4804, { 0x8e, 0xfb, 0x2c, 0x90, 0xd7, 0x5f, 0x82, 0xd6 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCBKMapOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCBKMapCtrl, IDS_CBKMAP, _dwCBKMapOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::CCBKMapCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCBKMapCtrl
///////////////////////////////////////////////////////////////////////////
//Interface map for IObjectSafety

BEGIN_INTERFACE_MAP(CCBKMapCtrl, COleControl )
INTERFACE_PART(CCBKMapCtrl, IID_IObjectSafety, ObjSafe)    
END_INTERFACE_MAP()    

///////////////////////////////////////////////////////////////////////////
//IObjectSafety member functions

//Delegate AddRef, Release, QueryInterface

ULONG FAR EXPORT CCBKMapCtrl::XObjSafe::AddRef()
{
	METHOD_PROLOGUE(CCBKMapCtrl, ObjSafe)
		return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CCBKMapCtrl::XObjSafe::Release()
{
	METHOD_PROLOGUE(CCBKMapCtrl, ObjSafe)
		return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CCBKMapCtrl::XObjSafe::QueryInterface(
													  REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CCBKMapCtrl, ObjSafe)
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
CCBKMapCtrl::XObjSafe::GetInterfaceSafetyOptions(
											  REFIID riid,
											  DWORD __RPC_FAR *pdwSupportedOptions,
											  DWORD __RPC_FAR *pdwEnabledOptions)
{
	METHOD_PROLOGUE(CCBKMapCtrl, ObjSafe)
		
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
CCBKMapCtrl::XObjSafe::SetInterfaceSafetyOptions(
											  REFIID riid,
											  DWORD dwOptionSetMask,
											  DWORD dwEnabledOptions)
{
	METHOD_PROLOGUE(CCBKMapCtrl, ObjSafe)
		
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL CCBKMapCtrl::CCBKMapCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_CBKMAP,
			IDB_CBKMAP,
			afxRegApartmentThreading,
			_dwCBKMapOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::CCBKMapCtrl - Constructor

CCBKMapCtrl::CCBKMapCtrl()
{
	InitializeIIDs(&IID_DCBKMap, &IID_DCBKMapEvents);

	// TODO: Initialize your control's instance data here.
	m_pImageMap=NULL;
	m_TitleBar=NULL;
	m_TSKchp=NULL;
	m_ImgpromptBox=NULL;
	m_ImgTongJi=NULL;
	m_ImgPlayBackBk=NULL;
	m_ImgPBTemp=NULL;
	m_ImgXMP=NULL;
	m_bFullScreen=FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::~CCBKMapCtrl - Destructor

CCBKMapCtrl::~CCBKMapCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::OnDraw - Drawing function
void CCBKMapCtrl::OnToolzDlgMsg(WPARAM wParam,LPARAM lParam)
{
	EMCARCTRL tempID=EMCARCTRL(wParam);
	switch (tempID)
	{
	case MCARMSG:
		break;
	case MXMZTCHANGE:
		{
			ISTRMSGZ *pp=(ISTRMSGZ *)lParam;
			//项目状态
			OnDrawPBBK(pp->strMsg);
			delete pp;
		}
		break;
	case MGNSSMSG:
		{
			GNSSMSGz *pp=(GNSSMSGz *)lParam;
			//TRACE("%d,%f,%f,%f \n",pp->uIndex,pp->cx,pp->cy,pp->cr);
			m_CarImg[pp->uIndex].SetGnssMsg(*pp);			
			if (m_uMapMode==2)
			{
				m_ImageL=(pp->cx-m_ImageW/2);
				m_ImageT=(pp->cy-m_ImageH/2);
				if (m_ImageL <0)
				{
					m_ImageL=0;
				}
				if (m_ImageT <0)
				{
					m_ImageT=0;
				}
				if (m_ImageL > m_imageSize.cx -m_ImageW )
				{
					m_ImageL =m_imageSize.cx -m_ImageW;
				}
				if (m_ImageT > m_imageSize.cy -m_ImageH )
				{
					m_ImageT =m_imageSize.cy -m_ImageH;					
				}
			}			
			delete pp;
		}
		break;
	case MSHOWDLG:
		{
			CRect rect;
			GetClientRect(&rect);			
			if ((int)lParam==0)
			{
				m_ToolzDlg.MoveWindow(0,rect.Height()-460,260,460);
			}
			else
			{
				m_ToolzDlg.MoveWindow(-210,rect.Height()-50,260,460);
			}
		}
		break;
	case MINITCAR:
		{
			ISTRMSGZ *kchp=(ISTRMSGZ *)lParam;
			TRACE("index=%d,cKchp=%s \n",kchp->uIndex,kchp->strMsg);
			int ikch=kchp->uIndex;
			m_iCarMap[ikch]=ikch;
			m_CarImg[ikch].InitCar(kchp->strMsg,m_TSKchp);
			m_CarImg[ikch].SetXYRotate(-10,-10,0);
			m_CarImg[ikch].isOnLine=true;
			m_iCarNum=m_iCarMap.size();
			delete kchp;
		}
		break;
	case MTIPBOXSHOW:
		{
			int ikch=(int )lParam;
			m_CarImg[ikch].SetDrawimg(2);
		}
		break;
	case MTIPBOXHIDE:
		{
			int ikch=(int )lParam;
			m_CarImg[ikch].SetDrawimg(1);
		}
		break;
	case MZKZMBHCHANGE:
		{
			ISTRMSGZ *kchp=(ISTRMSGZ *)lParam;
			m_CarImg[kchp->uIndex].SetStuZkzm(kchp->strMsg);
			delete kchp;
		}
		break;
	default:
		break;
	}
}

void CCBKMapCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	Graphics grap(m_dcMem.GetSafeHdc());
	//Color blueColor(255,30,30,30);
	Color blueColor(255,10,30,40);
	grap.Clear(blueColor);
	grap.SetInterpolationMode(InterpolationModeNearestNeighbor);
	if (m_isLoading)
	{
		grap.DrawImage(m_pImageMap,Rect(0,0, ClientRect.Width(),ClientRect.Height()),(int)m_ImageL,(int)m_ImageT,(int)m_ImageW,(int)m_ImageH,UnitPixel);
		
		DrawPrompt(&grap);
		if (m_uMapMode!=2)
		{
			DrawTongJi(&grap);			
			//////////////////////////////////////////////////////////////////////////
			grap.DrawImage(m_TitleBar,Rect(0,0, ClientRect.Width(),36),0,0,50,36,UnitPixel);
			Font myFont(L"宋体",14);
			SolidBrush blackBrush(Color::White);
			RectF destRect(0.0f,0.0f,ClientRect.Width(),36.0f);
			StringFormat format;
			format.SetAlignment(StringAlignmentCenter);//水平居左StringAlignmentNear //StringAlignmentCenter);//默认都不影响垂直 
			format.SetLineAlignment(StringAlignmentCenter);// 和SetAlignment对应，为竖直的设置
			
			CWideCharString WideCharString(m_TitleText, strlen(m_TitleText));
			grap.DrawString(WideCharString,wcslen(WideCharString),&myFont,destRect,&format,&blackBrush);
		}
		else
		{
			if (ClientRect.Width()>1280)
			{
				DrawPlayBack(&grap);
			}
			
		}
		//////////////////////////////////////////////////////////////////////////
		m_imgRc.SetRect(m_ImageL,m_ImageT,m_ImageW+m_ImageL,m_ImageH+m_ImageT);
		MAP_INT_INT::iterator iter;
		for (iter=m_iCarMap.begin();iter!=m_iCarMap.end();iter++)
		{		
			if (m_CarImg[iter->first].IsPtinScreen(m_imgRc,m_ZoomLevel))
			{
				m_CarImg[iter->first].DrawCar(&grap);
			}
		}
	}
	else
	{
		grap.DrawImage(m_pImageMap,Rect(m_initPicLT.cx,m_initPicLT.cy,m_imageSize.cx,m_imageSize.cy),0,0,m_imageSize.cx,m_imageSize.cy,UnitPixel);
	}
	pdc->BitBlt(0,0,rcBounds.Width(),rcBounds.Height(),&m_dcMem,0,0,SRCCOPY);
// 	CString temp;
// 	temp.Format("%d,%d,%d,%d,%.4f,%.4f",(int)m_ImageL,(int)m_ImageT,(int)m_ImageW,(int)m_ImageH,m_ZoomLevel,m_MinZoomLevel);
// 	pdc->TextOut(10,80,temp);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::DoPropExchange - Persistence support

void CCBKMapCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::OnResetState - Reset control to default state

void CCBKMapCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl::AboutBox - Display an "About" box to the user
void CCBKMapCtrl::OnVideoMenu(UINT nID)
{
	switch(nID)
	{
	case VIDEO_MENU_FULLSCREEN://全屏
		SetFullScreen(!m_bFullScreen);
		break;
	}
}

int GetCurrentScreenMonitorRect(HWND hWnd,RECT& ScreenRect)
{
	int nRet = 0;
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mInfo;
	mInfo.cbSize = sizeof(MONITORINFO);
	if(GetMonitorInfo(hMonitor,&mInfo) == TRUE)
	{
		ScreenRect = mInfo.rcMonitor;
	}
	else
	{
		//取主窗口的信息
		HMONITOR hMonitor2 = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO mInfo2;
		mInfo2.cbSize = sizeof(MONITORINFO);
		if(GetMonitorInfo(hMonitor2,&mInfo2) == TRUE)
		{
			ScreenRect = mInfo.rcMonitor;
		}
		else
		{
			ScreenRect.bottom = 0;
			ScreenRect.left = 0;
			ScreenRect.right = 0;
			ScreenRect.top = 0;
			nRet  = -1;
		}
	}
	return nRet;
}

void CCBKMapCtrl::SetFullScreen(BOOL bFlag)
{
	if(bFlag==m_bFullScreen) return;
	
	if( bFlag )
	{
		//隐藏任务栏
		pWndTask = FindWindow( _T("HHTaskBar"), NULL );
		if ( pWndTask )
		{
			pWndTask->ShowWindow(SW_HIDE);
		}
		
		//全屏
		m_hWndParent=::GetParent(m_hWnd);
		::GetWindowRect(m_hWnd, &rect);
		
		::SetParent(m_hWnd, ::GetDesktopWindow());
		int width = ::GetSystemMetrics(SM_CXSCREEN);
		int height = ::GetSystemMetrics(SM_CYSCREEN);
		::MoveWindow(m_hWnd, 0, 0, width, height, 0);
		m_bFullScreen = true;

		pWndTask = FindWindow( _T("HHTaskBar"), NULL );
		if ( pWndTask )
		{
			pWndTask->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		//恢复任务栏
		if ( pWndTask )
		{
			pWndTask->ShowWindow(SW_SHOW);
		}
		
		//退出全屏
		::SetParent(m_hWnd,m_hWndParent);   
		::MoveWindow(m_hWnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
		m_bFullScreen = false;
	}

	GetClientRect(&ClientRect);
	m_ImageW =ClientRect.Width()*m_ZoomLevel;
	m_ImageH =ClientRect.Height()*m_ZoomLevel;
	m_MinZoomLevel=(float)m_imageSize.cx /ClientRect.Width();
	m_ToolzDlg.MoveWindow(-210,ClientRect.Height()-50,260,460);

	m_promptBoxSize.cx =(ClientRect.Width()/2)-120;
	m_promptBoxSize.cy =(ClientRect.Height()/2)-40;
	m_bFullScreen=bFlag;
	Invalidate();
}

void CCBKMapCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CBKMAP);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl message handlers

int CCBKMapCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	try
	{	
		//////////////////////////////////////////////////////////////////////////
		m_uMapMode=1;
		m_dwTongJiTime=timeGetTime();

		dwLogid=logz.Register("CBKMapCtrl.log");

		CoInitialize(NULL);//ADO in
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
				
		CDC *pDC= this->GetDC();
		GetClientRect(&ClientRect); 
		int nFullWidth =	GetSystemMetrics(SM_CXSCREEN); 
		int nFullHeight =	GetSystemMetrics(SM_CYSCREEN);
		m_bmpMap.CreateCompatibleBitmap(pDC,nFullWidth,nFullHeight);

		if (m_bmpMap.m_hObject==NULL)
		{
			MessageBox("创建 地图 设备兼容的位图失败!");
			logz.WriteLog(dwLogid,"创建 地图 设备兼容的位图失败!,nFullWidth=%d,nFullHeight=%d",nFullWidth,nFullHeight);
			return false;
		}
		m_dcMem.CreateCompatibleDC(pDC);
		m_dcMem.SelectObject(m_bmpMap);
		this->ReleaseDC(pDC);
		
		m_ImageL=0;
		m_ImageT=0;
		m_ZoomLevel=1.0f;
		m_isLBDown=false;
		m_isLoading=false;
		m_ImageW=ClientRect.Width()*m_ZoomLevel;
		m_ImageH=ClientRect.Height()*m_ZoomLevel;
		if (LoadImageFromIDR(IDR_LOADIMG,m_pImageMap)==TRUE)
		{
			m_imageSize.cx=m_pImageMap->GetWidth();
			m_imageSize.cy=m_pImageMap->GetHeight();
			m_initPicLT.cx=(ClientRect.Width()/2)-(m_imageSize.cx/2);
			m_initPicLT.cy=(ClientRect.Height()/2)-(m_imageSize.cy/2);
		}
		//////////////////////////////////////////////////////////////////////////
		LoadImageFromIDR(IDR_TITLEBAR,m_TitleBar);

		LoadImageFromIDR(IDR_TSKCHP,m_TSKchp);

		LoadImageFromIDR(IDR_PROMPTBOX,m_ImgpromptBox);

		LoadImageFromIDR(IDR_TONGJIBOX,m_ImgTongJi);

		LoadImageFromIDR(IDR_PLAYBACKBK,m_ImgPlayBackBk);
		
		LoadImageFromIDR(IDR_XMP,m_ImgXMP);

		m_ImgPBTemp=m_ImgPlayBackBk->Clone();
		//////////////////////////////////////////////////////////////////////////		
		m_promptBoxSize.cx =(ClientRect.Width()/2)-120;
		m_promptBoxSize.cy =(ClientRect.Height()/2)-40;
		
		CRect rect;
		GetClientRect(&rect);
		m_ToolzDlg.Create(CToolzDlg::IDD,this);
		m_ToolzDlg.MoveWindow(-210,rect.Height()-50,260,460);
		
		//	m_ToolzDlg.MoveWindow(rect.Width()-260,rect.Height()-460,260,460);
		m_ToolzDlg.ShowWindow(SW_HIDE);
		m_bDrawPromptBox=false;
		m_fhgr=0.0f;
		m_iykrs=0;
		m_iwkrs=0;
		m_izxkc=0;
	}
	catch (...)
	{
		AfxMessageBox("Create ...");
	}
	return 0;
}

void CCBKMapCtrl::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GdiplusShutdown(gdiplusToken);
	CoUninitialize();//ADO un
	CWnd::OnClose();
}

BOOL CCBKMapCtrl::LoadImageFromIDR(UINT nID, Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),"ZSZ");
	if (!hRsrc)
		return FALSE;
	
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	
	// load from stream
	pImg=Gdiplus::Image::FromStream(pstm);
	
	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}

void CCBKMapCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_LBx=point.x;
	m_LBy=point.y;
	m_isLBDown=true;
	COleControl::OnLButtonDown(nFlags, point);
}

void CCBKMapCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_isLBDown)//左键按下
	{
		m_ImageL +=(m_LBx-point.x)*m_ZoomLevel;
		m_ImageT +=(m_LBy-point.y)*m_ZoomLevel;		
		m_LBx=point.x;
		m_LBy=point.y;
		AssertImageLT();
		Invalidate(FALSE);
	}
	COleControl::OnMouseMove(nFlags, point);
}

void CCBKMapCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_isLBDown=false;
	COleControl::OnLButtonUp(nFlags, point);
}

BOOL CCBKMapCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	//ScreenToClient((LPPOINT)&rc); 
	ClientToScreen(&rc);

	short iDelta = zDelta; //确定滚动方向
	m_ZoomLevelOld =m_ZoomLevel;//
	m_ImgLOld =m_ImageL;
	m_ImgTOld =m_ImageT;
	//////////////////////////////////////////////////////////////////////////
	if (iDelta <0) //缩小
	{
		if (m_ZoomLevel >=m_MinZoomLevel)//
		{
			m_ZoomLevel =m_MinZoomLevel;
			SetpromptText("已缩小到最小%.0f%%",(1/m_ZoomLevel)*100);
		}
		else
		{
			m_ZoomLevel+=0.1f;
			SetpromptText("%.2f%%",(1/m_ZoomLevel)*100);
		}		
	}
	else //放大
	{
		if (m_ZoomLevel <=0.4f)//
		{
			m_ZoomLevel =0.4f;
			SetpromptText("已放大到最大%.0f%%",(1/m_ZoomLevel)*100);
		}
		else
		{
			m_ZoomLevel-=0.1f;
			SetpromptText("%.2f%%",(1/m_ZoomLevel)*100);
		}
	}
	
	m_ImageL =m_ImgLOld + (pt.x-rc.left)*(m_ZoomLevelOld-m_ZoomLevel);
	m_ImageT =m_ImgTOld + (pt.y-rc.top)*(m_ZoomLevelOld-m_ZoomLevel);
	m_ImageW =ClientRect.Width()*m_ZoomLevel;
	m_ImageH =ClientRect.Height()*m_ZoomLevel;
	AssertImageLT();
	Invalidate(FALSE);
	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}


void CCBKMapCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	OnNowGPSData();
	switch (nIDEvent)
	{
	case 1:
		Invalidate(FALSE);
		break;
	case 2:
//		m_ToolzDlg.GetTongJiMsg();
		break;
	}
	
	COleControl::OnTimer(nIDEvent);
}

void CCBKMapCtrl::AssertImageLT()
{
	if (m_ImageL <0)
	{
		m_ImageL=0;
		SetpromptText("已到最左边");
	}
	if (m_ImageT <0)
	{
		m_ImageT=0;
		SetpromptText("已到最上边");
	}
	if (m_ImageL > m_imageSize.cx -m_ImageW )
	{
		m_ImageL =m_imageSize.cx -m_ImageW;
		SetpromptText("已到最右边");
	}
	if (m_ImageT > m_imageSize.cy -m_ImageH )
	{
		m_ImageT =m_imageSize.cy -m_ImageH;
		SetpromptText("已到最下边");
	}
}

void CCBKMapCtrl::DrawPrompt(Graphics *grapz)
{
	if (m_bDrawPromptBox==true)
	{
		if (timeGetTime()-m_dwPromptBoxTime>2000)
		{
			m_bDrawPromptBox=false;
		}
		grapz->DrawImage(m_ImgpromptBox,Rect(m_promptBoxSize.cx,m_promptBoxSize.cy,240,40),0,0,240,40,UnitPixel);
		
		Font myFont(L"宋体",14);
		SolidBrush blackBrush(Color::Yellow);
		RectF destRect(m_promptBoxSize.cx,m_promptBoxSize.cy,240,40);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);//水平居左StringAlignmentNear //StringAlignmentCenter);//默认都不影响垂直 
		format.SetLineAlignment(StringAlignmentCenter);// 和SetAlignment对应，为竖直的设置
		
		CWideCharString WideCharString(m_promptStr, strlen(m_promptStr));
		grapz->DrawString(WideCharString,wcslen(WideCharString),&myFont,destRect,&format,&blackBrush);
	}
}

void CCBKMapCtrl::SetpromptText(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	m_dwPromptBoxTime=timeGetTime();
	m_bDrawPromptBox=true;
	m_promptStr.Format("%s",logstr);
}

void CCBKMapCtrl::DrawTongJi(Graphics *grapz)
{
	if (timeGetTime()-m_dwTongJiTime>15000)//发送事件给Web
	{
		m_dwTongJiTime=timeGetTime();
		FireGetTongJi(m_strKCDM);
	}
	//64;
	int imgLeft,imgTop;
	imgLeft=20;
	imgTop=46;
	grapz->DrawImage(m_ImgTongJi,Rect(imgLeft,imgTop,126,142),0,0,126,142,UnitPixel);

	Font myFont(L"宋体",10);
	SolidBrush blackBrush(Color::Yellow);
	RectF destRect(imgLeft+62,imgTop+8,34,26);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);//水平居左StringAlignmentNear //StringAlignmentCenter);//默认都不影响垂直 
	format.SetLineAlignment(StringAlignmentCenter);// 和SetAlignment对应，为竖直的设置

	DrawStringz(grapz,m_iCarNum,myFont,blackBrush,destRect,format);
//	grapz->DrawRectangle(&Pen(Color::Yellow,1), destRect);
	destRect.Offset(0,26);
	DrawStringz(grapz,m_izxkc,myFont,blackBrush,destRect,format);
//	grapz->DrawRectangle(&Pen(Color::Yellow,1), destRect);
	destRect.Offset(0,26);
	DrawStringz(grapz,m_iykrs,myFont,blackBrush,destRect,format);
//	grapz->DrawRectangle(&Pen(Color::Yellow,1), destRect);
	destRect.Offset(0,26);
	DrawStringz(grapz,m_iwkrs,myFont,blackBrush,destRect,format);
//	grapz->DrawRectangle(&Pen(Color::Yellow,1), destRect);
	destRect.Inflate(8,0);
	destRect.Offset(18,26);
	CString temp;
	temp.Format("%.1f%%",m_fhgr);
	DrawStringz(grapz,temp,myFont,blackBrush,destRect,format);
//	grapz->DrawRectangle(&Pen(Color::Yellow,1), destRect);
// 	CWideCharString WideCharString(m_promptStr, strlen(m_promptStr));
// 	grapz->DrawString(WideCharString,wcslen(WideCharString),&myFont,destRect,&format,&blackBrush);

}

void CCBKMapCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING | m_bFullScreen ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_FULLSCREEN, "全屏显示");

	TrackPopupMenu( 
		menu.m_hMenu, 
		TPM_LEFTALIGN, 
		point.x, 
		point.y,
		0,
		m_hWnd,
		NULL);
}

void CCBKMapCtrl::LoadMapFile(LPCTSTR strkcdm, LPCTSTR strUDate) 
{
	// TODO: Add your dispatch handler code here
	m_iCarNum=0;
	SetpromptText("正在加载地图...");	
	m_strKCDM.Format("%s",strkcdm);
	//////////////////////////////////////////////////////////////////////////
	//检测是否更新地图
	CString m_filename,temp,temp2;
	m_filename.Format("D:\\bkzyMapOcx\\MAP\\%s.map",strkcdm);
	bool bUpMap=true;
	CFileFind mffind;
	if (mffind.FindFile(m_filename)) //地图文件已存在
	{
		bUpMap=false;
	}
	mffind.Close();
	temp2.Format("D:\\bkzyMapOcx\\MAP\\%s.dat",strkcdm);
	GetPrivateProfileString(strkcdm,"UDATE","1986",temp.GetBuffer(MAX_PATH),MAX_PATH,temp2);
	temp.ReleaseBuffer();
	if (temp !=strUDate)	//时间不相等
	{
		bUpMap=true;
	}
	if (bUpMap==true)
	{
		//更新地图
		CString Commandstr;
		Commandstr.Format(" %s",strkcdm);
		PROCESS_INFORMATION  ProcessInfo;     
		STARTUPINFO  StartupInfo;  
		ZeroMemory(&StartupInfo,  sizeof(StartupInfo));   
		StartupInfo.cb  =  sizeof(StartupInfo); 
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_HIDE;		
		if(CreateProcess("D:\\bkzyMapOcx\\CDownMapFDB.exe", (LPSTR)(LPCSTR)Commandstr,NULL,NULL,FALSE,0,NULL, NULL,&StartupInfo,&ProcessInfo))   
		{     
			DWORD dwWait=WaitForSingleObject(ProcessInfo.hProcess,INFINITE);   
			
			CloseHandle(ProcessInfo.hThread);   
			CloseHandle(ProcessInfo.hProcess);			
		}
		//////////////////////////////////////////////////////////////////////////
	}
	m_ToolzDlg.InitKchp(strkcdm);
	//////////////////////////////////////////////////////////////////////////
	unsigned short tcharArr[1024];
	memset(tcharArr,0x0,sizeof(tcharArr));
	swprintf((wchar_t *)tcharArr,L"%s",m_filename.AllocSysString());
	if (m_pImageMap!=NULL)
	{
		delete m_pImageMap;
		m_pImageMap=NULL;
	}
	m_pImageMap = Gdiplus::Image::FromFile(tcharArr);
	
	m_imageSize.cx=0;
	m_imageSize.cy=0;
	m_imageSize.cx=m_pImageMap->GetWidth();
	m_imageSize.cy=m_pImageMap->GetHeight();
	if (m_imageSize.cx==0 || m_imageSize.cy==0)
	{
		temp.Format("加载地图:%s 失败!",strkcdm);
		AfxMessageBox(temp);
		return ;
	}
	m_ZoomLevel=1;
	m_MinZoomLevel=(float)m_imageSize.cx /ClientRect.Width();
	m_ImageW=ClientRect.Width()*m_ZoomLevel;
	m_ImageH=ClientRect.Height()*m_ZoomLevel;
	
	m_ImageL=(m_imageSize.cx-ClientRect.Width())/2;
	m_ImageT=(m_imageSize.cy-ClientRect.Height())/2;
	
	AssertImageLT();
	m_isLoading=true;
	isRunNowGPSData=false;

	temp2.Format("D:\\bkzyMapOcx\\MAP\\%s.dat",strkcdm);
	GetPrivateProfileString(strkcdm,"KCMC","",temp.GetBuffer(MAX_PATH),MAX_PATH,temp2);
	temp.ReleaseBuffer();

	m_TitleText.Format("%s",temp);
	KillTimer(1);
	SetTimer(1,1000,NULL);
	Invalidate();
	m_ToolzDlg.ShowWindow(SW_SHOW);
}

void CCBKMapCtrl::PlayBackMap(LPCTSTR strzkzm, short ikscs, short idrcs) 
{
	// TODO: Add your dispatch handler code here
	m_uMapMode=2;//回放模式
	m_ToolzDlg.SetMode(m_uMapMode);
	//////////////////////////////////////////////////////////////////////////
	if (m_ImgPlayBackBk!=NULL)
	{
		CDC *pDC= this->GetDC();
		m_pbBkbmp.CreateCompatibleBitmap(pDC,m_ImgPlayBackBk->GetWidth(),m_ImgPlayBackBk->GetHeight());
		if (m_pbBkbmp.m_hObject==NULL)
		{
			logz.WriteLog(dwLogid,"创建 m_pbBkbmp 设备兼容的位图失败!");
		}
		m_pbDC.CreateCompatibleDC(pDC);
		m_pbDC.SelectObject(m_pbBkbmp);
		this->ReleaseDC(pDC);
	}

	//下载回放数据
	SetpromptText("正在加载回放数据...");
	CString Commandstr;
	Commandstr.Format(" %s %d %d",strzkzm,ikscs,idrcs);

	PROCESS_INFORMATION  ProcessInfo;     
	STARTUPINFO  StartupInfo;  
	ZeroMemory(&StartupInfo,  sizeof(StartupInfo));   
	StartupInfo.cb  =  sizeof(StartupInfo); 
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;		
	if(CreateProcess("D:\\bkzyMapOcx\\CPlayBackMap.exe", (LPSTR)(LPCSTR)Commandstr,NULL,NULL,FALSE,0,NULL, NULL,&StartupInfo,&ProcessInfo))   
	{     
// 		DWORD dwWait=WaitForSingleObject(ProcessInfo.hProcess,INFINITE);   
// 		
// 		CloseHandle(ProcessInfo.hThread);   
// 		CloseHandle(ProcessInfo.hProcess);			
	}
	//创建UDP服务接收数据
	//回放控制
	OnDrawPBBK("00001111111111112123");
}

void CCBKMapCtrl::SetTongJi(short izxkc, short iykrs, short iwkrs, float fhgr) 
{
	// TODO: Add your dispatch handler code here
	m_izxkc=izxkc;
	m_iykrs=iykrs;
	m_iwkrs=iwkrs;
	m_fhgr =fhgr;
}

void CCBKMapCtrl::DrawPlayBack(Graphics *grapz)
{
	grapz->DrawImage(m_ImgPlayBackBk,Rect(10,10,640,320),0,0,640,320,UnitPixel);
	grapz->DrawImage(m_ImgPBTemp,Rect(10,10,640,320),0,0,640,320,UnitPixel);
// 	Pen blackPen(Color(250, 1, 94,125), 1);
// 	int iCx,iCy;
// 	iCx=ClientRect.Width()/2;
// 	iCy=ClientRect.Height()/2;
// 	grapz->DrawLine(&blackPen,645,330,iCx,iCy);
// 	grapz->DrawLine(&blackPen,650,325,iCx,iCy);
// 	grapz->DrawLine(&blackPen,10,330,iCx,iCy);
// 	grapz->DrawLine(&blackPen,650,10,iCx,iCy);
}

void CCBKMapCtrl::OnSetClientSite() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_pClientSite)//控件删除
	{
		CString Commandstr;
		Commandstr.Format(" 1346708777 1 1");

		PROCESS_INFORMATION  ProcessInfo;     
		STARTUPINFO  StartupInfo;  
		ZeroMemory(&StartupInfo,  sizeof(StartupInfo));   
		StartupInfo.cb  =  sizeof(StartupInfo); 
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_HIDE;		
		CreateProcess("D:\\bkzyMapOcx\\CPlayBackMap.exe", (LPSTR)(LPCSTR)Commandstr,NULL,NULL,FALSE,0,NULL, NULL,&StartupInfo,&ProcessInfo);   
	
	}
	COleControl::OnSetClientSite();
}

void CCBKMapCtrl::OnDrawPBBK(CString str)
{

// 	Rect destRect(ptx*80+XMPLISTX,XMPLISTY,80,100);
// 	graph->DrawImage(ImageExamBK,destRect,ptx*80+XMPLISTX,XMPLISTY,80,100,UnitPixel);//背景
// 	graph->DrawImage(ImageXMBG,destRect,ix*80,itype*100,80,100,UnitPixel);

	Graphics tempGr(m_ImgPBTemp);
	int sLen=str.GetLength();
	str.MakeReverse();
	if (sLen==20)
	{
// 		Graphics Graphz(m_pbDC.GetSafeHdc());
// 		Graphz.DrawImage(m_ImgPlayBackBk,Rect(0,0,640,320));
		if (m_ImgPlayBackBk!=NULL)
		{
			int itype=0;
			char ch;
			int ipx,ipy;
			ipx=48;
			ipy=168;
			for (int i=0;i<16;i++)
			{
				ch=str.GetAt(i);
				if (ch=='2')
				{
					itype=1;
				}
				else if (ch=='3')
				{
					itype=2;
				}
				else
				{
					itype=0;
				}
				Rect destRect((i>7?i-8:i)*72+ipx,(i>7?ipy+72:ipy),72,72);
				tempGr.DrawImage(m_ImgXMP,destRect,itype*72,i*72,72,72,UnitPixel);
				//TRACE("%d,%d,%d,%d \n",destRect.GetLeft(),destRect.GetTop(),i*72,itype*72);
			}
		}		
	}
}

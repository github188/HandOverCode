// CPlayBackzOcxCtl.cpp : Implementation of the CCPlayBackzOcxCtrl ActiveX Control class.

#include "stdafx.h"
#include "CPlayBackzOcx.h"
#include "CPlayBackzOcxCtl.h"
#include "CPlayBackzOcxPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCPlayBackzOcxCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCPlayBackzOcxCtrl, COleControl)
	//{{AFX_MSG_MAP(CCPlayBackzOcxCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCPlayBackzOcxCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCPlayBackzOcxCtrl)
	DISP_FUNCTION(CCPlayBackzOcxCtrl, "LoginDevice", LoginDevice, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2)
	DISP_FUNCTION(CCPlayBackzOcxCtrl, "SetPlayBackByTime", SetPlayBackByTime, VT_BOOL, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCPlayBackzOcxCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCPlayBackzOcxCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCPlayBackzOcxCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCPlayBackzOcxCtrl, 1)
	PROPPAGEID(CCPlayBackzOcxPropPage::guid)
END_PROPPAGEIDS(CCPlayBackzOcxCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCPlayBackzOcxCtrl, "CPLAYBACKZOCX.CPlayBackzOcxCtrl.1",
	0xa166bb8f, 0x89c7, 0x473f, 0x9b, 0x15, 0x1c, 0x9e, 0x5e, 0x2f, 0xde, 0x67)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCPlayBackzOcxCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCPlayBackzOcx =
		{ 0xc387f89c, 0x2fe8, 0x42bf, { 0x8e, 0x12, 0xb8, 0x3b, 0xdb, 0x95, 0xff, 0x15 } };
const IID BASED_CODE IID_DCPlayBackzOcxEvents =
		{ 0x9e6ef0d, 0xac99, 0x4d18, { 0x81, 0x87, 0xc5, 0x97, 0x27, 0x19, 0x47, 0x1b } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCPlayBackzOcxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCPlayBackzOcxCtrl, IDS_CPLAYBACKZOCX, _dwCPlayBackzOcxOleMisc)

///////////////////////////////////////////////////////////////////////////
//Interface map for IObjectSafety

BEGIN_INTERFACE_MAP(CCPlayBackzOcxCtrl, COleControl )
INTERFACE_PART(CCPlayBackzOcxCtrl, IID_IObjectSafety, ObjSafe)    
END_INTERFACE_MAP()    

///////////////////////////////////////////////////////////////////////////
//IObjectSafety member functions

//Delegate AddRef, Release, QueryInterface

ULONG FAR EXPORT CCPlayBackzOcxCtrl::XObjSafe::AddRef()
{
	METHOD_PROLOGUE(CCPlayBackzOcxCtrl, ObjSafe)
		return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CCPlayBackzOcxCtrl::XObjSafe::Release()
{
	METHOD_PROLOGUE(CCPlayBackzOcxCtrl, ObjSafe)
		return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CCPlayBackzOcxCtrl::XObjSafe::QueryInterface(
													  REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CCPlayBackzOcxCtrl, ObjSafe)
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

const DWORD dwSupportedBits =
INTERFACESAFE_FOR_UNTRUSTED_CALLER |
INTERFACESAFE_FOR_UNTRUSTED_DATA;
const DWORD dwNotSupportedBits = ~ dwSupportedBits;

///////////////////////////////////////////////////////////////////////////
// CStopLiteCtrl::XObjSafe::GetInterfaceSafetyOptions
// Allows container to query what interfaces are safe for what. We¡¯re
// optimizing significantly by ignoring which interface the caller is
// asking for.
HRESULT STDMETHODCALLTYPE
CCPlayBackzOcxCtrl::XObjSafe::GetInterfaceSafetyOptions(
											  REFIID riid,
											  DWORD __RPC_FAR *pdwSupportedOptions,
											  DWORD __RPC_FAR *pdwEnabledOptions)
{
	METHOD_PROLOGUE(CCPlayBackzOcxCtrl, ObjSafe)
		
		HRESULT retval = ResultFromScode(S_OK);
	
	// does interface exist?
	IUnknown FAR* punkInterface;
	retval = pThis->ExternalQueryInterface(&riid,
		(void * *)&punkInterface);
	if (retval != E_NOINTERFACE) { // interface exists
		punkInterface->Release(); // release it¨Cjust checking!
	}
	
	// we support both kinds of safety and have always both set,
	// regardless of interface
	*pdwSupportedOptions = *pdwEnabledOptions = dwSupportedBits;
	
	return retval; // E_NOINTERFACE if QI failed
}

///////////////////////////////////////////////////////////////////////////
// CStopLiteCtrl::XObjSafe::SetInterfaceSafetyOptions
// Since we¡¯re always safe, this is a no-brainer¨Cbut we do check to make
// sure the interface requested exists and that the options we¡¯re asked to
// set exist and are set on (we don¡¯t support unsafe mode).
HRESULT STDMETHODCALLTYPE
CCPlayBackzOcxCtrl::XObjSafe::SetInterfaceSafetyOptions(
											  REFIID riid,
											  DWORD dwOptionSetMask,
											  DWORD dwEnabledOptions)
{
	METHOD_PROLOGUE(CCPlayBackzOcxCtrl, ObjSafe)
		
		// does interface exist?
		IUnknown FAR* punkInterface;
	pThis->ExternalQueryInterface(&riid, (void * *)&punkInterface);
	if (punkInterface) { // interface exists
		punkInterface->Release(); // release it¨Cjust checking!
	}
	else { // interface doesn¡¯t exist
		return ResultFromScode(E_NOINTERFACE);
	}
	
	// can¡¯t set bits we don¡¯t support
	if (dwOptionSetMask & dwNotSupportedBits) {
		return ResultFromScode(E_FAIL);
	}
	
	// can¡¯t set bits we do support to zero
	dwEnabledOptions &= dwSupportedBits;
	// (we already know there are no extra bits in mask )
	if ((dwOptionSetMask & dwEnabledOptions) !=
		dwOptionSetMask) {
		return ResultFromScode(E_FAIL);
	}       
	
	// don¡¯t need to change anything since we¡¯re always safe
	return ResultFromScode(S_OK);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::CCPlayBackzOcxCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCPlayBackzOcxCtrl

BOOL CCPlayBackzOcxCtrl::CCPlayBackzOcxCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_CPLAYBACKZOCX,
			IDB_CPLAYBACKZOCX,
			afxRegApartmentThreading,
			_dwCPlayBackzOcxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::CCPlayBackzOcxCtrl - Constructor

CCPlayBackzOcxCtrl::CCPlayBackzOcxCtrl()
{
	InitializeIIDs(&IID_DCPlayBackzOcx, &IID_DCPlayBackzOcxEvents);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::~CCPlayBackzOcxCtrl - Destructor

CCPlayBackzOcxCtrl::~CCPlayBackzOcxCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::OnDraw - Drawing function

void CCPlayBackzOcxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
// 	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
// 	pdc->Ellipse(rcBounds);
	if (!pdc)
        return;
	m_DlgPlayBackz.MoveWindow(rcBounds,1);
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::DoPropExchange - Persistence support

void CCPlayBackzOcxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::OnResetState - Reset control to default state

void CCPlayBackzOcxCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl::AboutBox - Display an "About" box to the user

void CCPlayBackzOcxCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CPLAYBACKZOCX);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxCtrl message handlers

int CCPlayBackzOcxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect;
	GetClientRect(&rect);
	m_DlgPlayBackz.Create(CPlayBackDlgz::IDD,this);
	m_DlgPlayBackz.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	m_DlgPlayBackz.ShowWindow(SW_SHOW);
	return 0;
}

BOOL CCPlayBackzOcxCtrl::LoginDevice(LPCTSTR strIP, LPCTSTR strUser, LPCTSTR strPwd, short nPort, short nChannel) 
{
	// TODO: Add your dispatch handler code here
	if (m_DlgPlayBackz.fnDVR_Login_V30(strIP,strUser,strPwd,nPort,nChannel)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCPlayBackzOcxCtrl::SetPlayBackByTime(LPCTSTR strSTime, LPCTSTR strETime) 
{
	// TODO: Add your dispatch handler code here
	if (m_DlgPlayBackz.SetPlayBackTime(strSTime,strETime)==FALSE)
	{
		return FALSE;
	}
//	m_DlgPlayBackz.OnStartPlay();
	return TRUE;
}

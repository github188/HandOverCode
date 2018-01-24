// CPlayBackzOcxPpg.cpp : Implementation of the CCPlayBackzOcxPropPage property page class.

#include "stdafx.h"
#include "CPlayBackzOcx.h"
#include "CPlayBackzOcxPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCPlayBackzOcxPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCPlayBackzOcxPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCPlayBackzOcxPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCPlayBackzOcxPropPage, "CPLAYBACKZOCX.CPlayBackzOcxPropPage.1",
	0x9485b02a, 0x5804, 0x4b16, 0xaa, 0xce, 0x8f, 0x61, 0x6f, 0x6f, 0xc9, 0xc0)


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxPropPage::CCPlayBackzOcxPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCPlayBackzOcxPropPage

BOOL CCPlayBackzOcxPropPage::CCPlayBackzOcxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CPLAYBACKZOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxPropPage::CCPlayBackzOcxPropPage - Constructor

CCPlayBackzOcxPropPage::CCPlayBackzOcxPropPage() :
	COlePropertyPage(IDD, IDS_CPLAYBACKZOCX_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCPlayBackzOcxPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxPropPage::DoDataExchange - Moves data between page and properties

void CCPlayBackzOcxPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCPlayBackzOcxPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxPropPage message handlers

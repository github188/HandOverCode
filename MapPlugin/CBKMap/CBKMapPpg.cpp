// CBKMapPpg.cpp : Implementation of the CCBKMapPropPage property page class.

#include "stdafx.h"
#include "CBKMap.h"
#include "CBKMapPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCBKMapPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCBKMapPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCBKMapPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCBKMapPropPage, "CBKMAP.CBKMapPropPage.1",
	0xc92fe8a5, 0xa9c0, 0x44bd, 0xac, 0x2e, 0xc1, 0x55, 0x66, 0xc1, 0xab, 0xb6)


/////////////////////////////////////////////////////////////////////////////
// CCBKMapPropPage::CCBKMapPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCBKMapPropPage

BOOL CCBKMapPropPage::CCBKMapPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CBKMAP_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapPropPage::CCBKMapPropPage - Constructor

CCBKMapPropPage::CCBKMapPropPage() :
	COlePropertyPage(IDD, IDS_CBKMAP_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCBKMapPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapPropPage::DoDataExchange - Moves data between page and properties

void CCBKMapPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCBKMapPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKMapPropPage message handlers

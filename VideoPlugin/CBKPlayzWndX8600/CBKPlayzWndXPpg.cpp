// CBKPlayzWndXPpg.cpp : Implementation of the CCBKPlayzWndXPropPage property page class.

#include "stdafx.h"
#include "CBKPlayzWndX.h"
#include "CBKPlayzWndXPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCBKPlayzWndXPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCBKPlayzWndXPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCBKPlayzWndXPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCBKPlayzWndXPropPage, "CBKPLAYZWNDX.CBKPlayzWndXPropPage.1",
	0xf748a7ac, 0x1e5c, 0x41b4, 0xaf, 0xa3, 0xe9, 0xf5, 0x14, 0x54, 0xdb, 0xa2)


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXPropPage::CCBKPlayzWndXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCBKPlayzWndXPropPage

BOOL CCBKPlayzWndXPropPage::CCBKPlayzWndXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CBKPLAYZWNDX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXPropPage::CCBKPlayzWndXPropPage - Constructor

CCBKPlayzWndXPropPage::CCBKPlayzWndXPropPage() :
	COlePropertyPage(IDD, IDS_CBKPLAYZWNDX_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCBKPlayzWndXPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXPropPage::DoDataExchange - Moves data between page and properties

void CCBKPlayzWndXPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCBKPlayzWndXPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXPropPage message handlers

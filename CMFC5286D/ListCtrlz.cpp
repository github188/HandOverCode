// ListCtrlz.cpp : implementation file
//

#include "stdafx.h"
#include "CMFC5286D.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlz

CListCtrlz::CListCtrlz()
{
	m_LineColor=RGB(235, 245, 255);
	m_LineColortwo=RGB(252,252,252);
}

CListCtrlz::~CListCtrlz()
{
}


BEGIN_MESSAGE_MAP(CListCtrlz, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlz)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlz message handlers

void CListCtrlz::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	
	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) 
	{ 
		*pResult = CDRF_NOTIFYITEMDRAW; 
	} 
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage) 
	{ 
		if(pLVCD->nmcd.dwItemSpec % 2) pLVCD->clrTextBk =m_LineColortwo;// RGB(250, 250, 250); 
		else pLVCD->clrTextBk = m_LineColor;//RGB(235, 245, 255); 
		
		//¼ÌÐø 
		*pResult = CDRF_NOTIFYSUBITEMDRAW; 
		
		*pResult = 0;
	}
}

void CListCtrlz::SetLineColor(COLORREF linecolor, COLORREF linecolor2)
{
	m_LineColor=linecolor;
	m_LineColortwo=linecolor2;
}

void CListCtrlz::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(GetHeaderCtrl())
		m_HeaderCtrlz.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	CListCtrl::PreSubclassWindow();
}

void CListCtrlz::SetHeadColor(COLORREF bkcolor, COLORREF textcolor, COLORREF linecolor)
{
	m_HeaderCtrlz.m_bkcolor=bkcolor;
	m_HeaderCtrlz.m_textColor=textcolor;
	m_HeaderCtrlz.m_linecolor=linecolor;
}

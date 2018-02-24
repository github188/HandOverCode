// HeaderCtrlz.cpp : implementation file
//

#include "stdafx.h"
#include "CMFC5286D.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlz

CHeaderCtrlz::CHeaderCtrlz()
{
}

CHeaderCtrlz::~CHeaderCtrlz()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlz, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeaderCtrlz)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlz message handlers
void CHeaderCtrlz::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
}

void CHeaderCtrlz::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect,rectItem; 
	GetClientRect(&rect); 
// 	CDC dcMem; 
// 	dcMem.CreateCompatibleDC(&dc); 
	CBrush br;
	br.CreateSolidBrush(m_bkcolor);//m_bkcolor
	dc.FillRect(&rect,&br);//ÓÃÇå³ý»­°å
	CFont *pFont=GetParent()->GetFont();
	dc.SelectObject(pFont);
	dc.SetTextColor(m_textColor);//m_textColor
	dc.SetBkMode(TRANSPARENT);

	CPen pen(0,0,m_linecolor);//m_linecolor
	dc.SelectObject(&pen);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);
	
	CPoint pt(3,3);
	int nItems = GetItemCount();
	for(int i = 0; i <nItems; i++)
	{
		GetItemRect(i, &rectItem);
		TCHAR buf[256];
		HD_ITEM hditem;
		DRAWITEMSTRUCT	DrawItemStruct;
		DrawItemStruct.CtlType		= 80;
		DrawItemStruct.hDC			= dc.GetSafeHdc();
		DrawItemStruct.itemAction	= ODA_DRAWENTIRE; 
		DrawItemStruct.hwndItem 	= GetSafeHwnd(); 
		DrawItemStruct.rcItem	= rectItem;
		DrawItemStruct.itemID	= i;
		DrawItem(&DrawItemStruct);
		
		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( DrawItemStruct.itemID, &hditem );
		dc.RoundRect(rectItem,pt);
		dc.DrawText(buf,&rectItem,DT_CENTER | DT_SINGLELINE | DT_VCENTER );
	}
	// Do not call CHeaderCtrl::OnPaint() for painting messages
}

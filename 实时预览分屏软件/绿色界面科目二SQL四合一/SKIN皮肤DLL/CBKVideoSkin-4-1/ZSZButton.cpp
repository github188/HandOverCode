// ZSZButton.cpp : implementation file
//

#include "stdafx.h"
//#include "ff.h"
#include "ZSZButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZSZButton

CZSZButton::CZSZButton()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;
}

CZSZButton::~CZSZButton()
{
}


BEGIN_MESSAGE_MAP(CZSZButton, CButton)
	//{{AFX_MSG_MAP(CZSZButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZSZButton message handlers

void CZSZButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking =_TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CZSZButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Invalidate(TRUE);
	return 0;
}
LRESULT CZSZButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Invalidate(TRUE);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CZSZButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CZSZButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	//获取保存在DRAWITEMSTRUCT结构中且在绘制按钮时必须使用的设备上下文
	CDC* mydc=CDC::FromHandle(lpDrawItemStruct->hDC);
	//创建兼容的设备上下文
	CDC* pMenDC=new CDC;
	pMenDC->CreateCompatibleDC(mydc);
	
	//保存旧对象
	CBitmap * pOldBitmap;
	pOldBitmap =pMenDC ->SelectObject(&mybitmap);
	
	CPoint point(0,0);
	//判断按钮是否处于选择状态，如果是则绘制选择状态的位图，也就是选中第二个
	if (lpDrawItemStruct->itemState & ODS_SELECTED) //ODS_SELECTED
	{
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCCOPY);
		
	}
	else
	{	//判断鼠标是否离开还是在按钮上面，以便绘制相应的位图
		if (m_bHover)
		{
			//pMenDC->TextOut(10,10,"你好！");
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCCOPY);
			
		}else
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCCOPY);
		}
	}
	//clean up
	pMenDC ->SelectObject(pOldBitmap);
	delete pMenDC;
	
}

BOOL CZSZButton::LoadBitmap(UINT bitmapid)
{
	//载入图片
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid),\
		IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP bitmapbits;
	//获取位图信息并存入Bitmapbits结构中
	mybitmap.GetBitmap(&bitmapbits);
	
	//取位图相应的高度和 1/3宽度
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/3;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

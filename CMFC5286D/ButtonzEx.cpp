// ButtonzEx.cpp : implementation file
//

#include "stdafx.h"
#include "CMFC5286D.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonzEx

CButtonzEx::CButtonzEx()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;
	m_bmpn = 3;
	for (int i=0; i<5 ;i++)
	{
		m_BdrawX[i] = i % m_bmpn;
	}
	m_pToolTip = NULL;
}

CButtonzEx::~CButtonzEx()
{
	if(m_pToolTip)
	{	
		delete m_pToolTip;
	}
}


BEGIN_MESSAGE_MAP(CButtonzEx, CButton)
	//{{AFX_MSG_MAP(CButtonzEx)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonzEx message handlers
void CButtonzEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

BOOL CButtonzEx::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pToolTip != NULL)
	{
		if ((::IsWindow(m_pToolTip->m_hWnd)) && pMsg->message ==WM_MOUSEMOVE )
		{
			m_pToolTip->RelayEvent(pMsg);		
		}
	}
	return CButton::PreTranslateMessage(pMsg);
}

void CButtonzEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =10;
		m_bTracking =_TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CButtonzEx::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Invalidate(TRUE);
	return 0;
}
LRESULT CButtonzEx::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Invalidate(TRUE);
	return 0;
}

void CButtonzEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
	UINT nState = lpDrawItemStruct->itemState;
	
	int nImgIndex = 0;//bmpNormal
	if(m_bHover)
	{
		if(nState & ODS_SELECTED)
		{
			nImgIndex = 3;//bmpDown
		}
		else
		{
			nImgIndex = 1;//bmpHover
		}
	}
	else if(nState & ODS_FOCUS)
	{
		nImgIndex = 2;//bmpFocus
	}
	else if(nState & ODS_DISABLED)
	{
		nImgIndex = 4;//bmpDisable
	}
	mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*m_BdrawX[nImgIndex],0,SRCCOPY);
	pMenDC ->SelectObject(pOldBitmap);
	delete pMenDC;
}
//////////////////////////////////////////////////////////////////////////
void CButtonzEx::SetToolTipText(CString spText, BOOL bActivate)
{
	if (spText.IsEmpty()) return;
	
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		m_pToolTip->Create(this);
	}
	m_tooltext = spText;

	m_pToolTip->AddTool(this, m_tooltext);
	m_pToolTip->SetDelayTime(1000);
	m_pToolTip->Activate(bActivate);
}

void CButtonzEx::SetToolTipText(UINT nId, BOOL bActivate)
{
	m_tooltext.LoadString(nId);
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}
}
//////////////////////////////////////////////////////////////////////////
BOOL CButtonzEx::LoadBitmap(UINT bitmapid,UINT bmpn)
{
	//载入图片
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid),\
		IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP bitmapbits;
	//获取位图信息并存入Bitmapbits结构中
	mybitmap.GetBitmap(&bitmapbits);
	m_bmpn = bmpn;
	//取位图相应的高度和 1/m_bmpn宽度
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/m_bmpn;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

BOOL CButtonzEx::LoadBitmap(LPCTSTR lpszName,UINT bmpn)
{
	//载入图片
	mybitmap.m_hObject=(::LoadImage(::AfxGetInstanceHandle(),lpszName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	if (mybitmap.m_hObject==NULL)
	{
		//TRACE("载入图片失败\n");
		CString temp;
		temp.Format("加载[%s]图片失败",lpszName);
		AfxMessageBox(temp);
		return FALSE;
	}
	BITMAP bitmapbits;
	//获取位图信息并存入Bitmapbits结构中
	mybitmap.GetBitmap(&bitmapbits);
	m_bmpn = bmpn;
	//取位图相应的高度和 1/m_bmpn宽度
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/m_bmpn;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

void CButtonzEx::SetBtnDrawNum(UINT bmpNormal, UINT bmpHover, UINT bmpFocus, UINT bmpDown, UINT bmpDisable)
{
	m_BdrawX[0] = bmpNormal % m_bmpn;
	m_BdrawX[1] = bmpHover % m_bmpn;
	m_BdrawX[2] = bmpFocus % m_bmpn;
	m_BdrawX[3] = bmpDown % m_bmpn;
	m_BdrawX[4] = bmpDisable % m_bmpn;
}

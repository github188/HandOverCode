// StaticzEx.cpp : implementation file
//

#include "stdafx.h"
#include "cbkfourpickm2.h"
#include "StaticzEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticzEx

CStaticzEx::CStaticzEx()
{
	nFormat =DT_LEFT | DT_SINGLELINE | DT_VCENTER ;
	m_isBkcolor=FALSE;
	m_typee=0;
	m_iTypeFont = 0;
}

CStaticzEx::~CStaticzEx()
{
}


BEGIN_MESSAGE_MAP(CStaticzEx, CStatic)
	//{{AFX_MSG_MAP(CStaticzEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticzEx message handlers

void CStaticzEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	if (m_isBkcolor)
	{
//		dc.SetBkColor(m_BackColor);
		CBrush br;
		br.CreateSolidBrush(m_BackColor);
		dc.FillRect(&rect,&br);//用清除画板
	}
	dc.SetBkMode(TRANSPARENT);
	CFont *pFont=GetParent()->GetFont();
	CFont *pOldFont;
	CFont fDIYFont;
	fDIYFont.CreatePointFont(140,_T("微软雅黑"));
	
	if ( m_iTypeFont == 1 )
	{		
		pOldFont = dc.SelectObject(&fDIYFont);
	} 
	else
	{
		pOldFont = dc.SelectObject(pFont);
	}
	dc.SetTextColor(m_TextColor);
	dc.DrawText(m_strCaption,&rect,nFormat);
	dc.SelectObject(pOldFont);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CStaticzEx::SetCaption(CString strCaption, COLORREF TextColor)
{
	m_TextColor =TextColor;
	m_strCaption =strCaption;
}

void CStaticzEx::SetCaption(CString strCaption)
{
	m_strCaption =strCaption;
}

void CStaticzEx::SetTextColor(COLORREF TextColor)
{
	m_TextColor =TextColor;
}

 void CStaticzEx::SetBkColor(COLORREF bkColor,BOOL isbkcolor/* =TRUE */)
{
 	m_BackColor =bkColor;
 	m_isBkcolor=isbkcolor;
}

void CStaticzEx::SetCaptionSW(CString strCaption)
{
	ShowWindow(SW_HIDE);
	m_strCaption = strCaption;
	ShowWindow(SW_SHOW);
}

void CStaticzEx::SetTextnFormat(UINT nfm)
{
	switch (nfm)
	{
	case 1:
		nFormat=DT_LEFT | DT_SINGLELINE | DT_VCENTER ;
		m_iTypeFont = 0;
		break;
	case 2:
		nFormat=DT_CENTER | DT_SINGLELINE | DT_VCENTER ;
		m_iTypeFont = 0;
		break;
	case 3:
		nFormat=DT_RIGHT | DT_SINGLELINE | DT_VCENTER ;
		m_iTypeFont = 0;
		break;
	case 4:
		nFormat=DT_CENTER | DT_SINGLELINE | DT_VCENTER ;
		m_iTypeFont = 1;
		break;
	default :
		break;
	}
}


void CStaticzEx::SetXmlbColor(UINT uType)
{
/*	if (m_typee == 2 && uType ==3)
	{
		uType=2;
	}
	if (m_typee ==0 && uType ==3)
	{
		uType=0;
	}*/
	switch (uType)
	{
	case 0://正常
		m_BackColor=RGB(238,238,238);
		m_TextColor=RGB(51,51,51);
		m_isBkcolor=TRUE;
		break;
	case 1://正在考试
		m_BackColor=RGB(92,166,71);
		m_TextColor=RGB(255,255,255);
		m_isBkcolor=TRUE;
		break;
	case 2://合格
		m_BackColor=RGB(223,223,223);
		m_TextColor=RGB(92,166,71);
		m_isBkcolor=TRUE;
		break;
	case 3://不合格
		m_BackColor=RGB(254,7,0);
		m_TextColor=RGB(255,255,255);
		m_isBkcolor=TRUE;
		break;
	default :
		break;
	}
	m_typee=uType;
	Invalidate();
}

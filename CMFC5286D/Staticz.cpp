// Staticz.cpp : implementation file
//

#include "stdafx.h"
#include "CMFC5286D.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticz

CStaticz::CStaticz()
{
	nFormat =DT_LEFT | DT_SINGLELINE | DT_VCENTER ;
//	m_isBkcolor=FALSE;
}

CStaticz::~CStaticz()
{

}


BEGIN_MESSAGE_MAP(CStaticz, CStatic)
	//{{AFX_MSG_MAP(CStaticz)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticz message handlers

void CStaticz::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
// 	if (m_isBkcolor)
// 	{
// 		dc.SetBkColor(m_BackColor);
// 		CBrush br;
// 		br.CreateSolidBrush(m_BackColor);
// 		dc.FillRect(&rect,&br);//ÓÃÇå³ý»­°å
// 	}
	dc.SetBkMode(TRANSPARENT);
	CFont *pFont=GetParent()->GetFont();
	CFont *pOldFont;
	pOldFont=dc.SelectObject(pFont);
	dc.SetTextColor(m_TextColor);
	dc.DrawText(m_strCaption,&rect,nFormat);
	dc.SelectObject(pOldFont);
	// Do not call CStatic::OnPaint() for painting messages
}

void CStaticz::SetCaption(CString strCaption, COLORREF TextColor)
{
	m_TextColor =TextColor;
	m_strCaption =strCaption;
}

void CStaticz::SetCaption(CString strCaption)
{
	m_strCaption =strCaption;
}

void CStaticz::SetTextColor(COLORREF TextColor)
{
	m_TextColor =TextColor;
}

void CStaticz::SetCaptionSW(CString strCaption)
{
	ShowWindow(SW_HIDE);
	m_strCaption =strCaption;
	ShowWindow(SW_SHOW);
}

void CStaticz::SetTextnFormat(UINT nfm)
{
	switch (nfm)
	{
	case 1:
		nFormat=DT_LEFT | DT_SINGLELINE | DT_VCENTER ;
		break;
	case 2:
		nFormat=DT_CENTER | DT_SINGLELINE | DT_VCENTER ;
		break;
	case 3:
		nFormat=DT_RIGHT | DT_SINGLELINE | DT_VCENTER ;
		break;
	default :
		break;
	}
}


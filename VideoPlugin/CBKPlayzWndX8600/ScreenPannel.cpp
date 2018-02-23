// ScreenPannel.cpp : implementation file
//

#include "stdafx.h"
#include "CBKPlayzWndX.h"
#include "ScreenPannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenPannel

CScreenPannel::CScreenPannel()
{
	m_curScreen=0;
	m_Split=0;

	m_pMessageProc = NULL;
	m_dwMessageUser = 0;
}

CScreenPannel::~CScreenPannel()
{

}


BEGIN_MESSAGE_MAP(CScreenPannel, CWnd)
	//{{AFX_MSG_MAP(CScreenPannel)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreenPannel message handlers

int CScreenPannel::GetCurWindId()
{
	return m_curScreen;	
}

void CScreenPannel::SetCurWindId(int nCuWinID)
{
	m_curScreen = nCuWinID;
}

int CScreenPannel::GetSplit()
{
	return m_Split;
}
void CScreenPannel::SetSplit(int split)
{
	m_Split=split;
}

int CScreenPannel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	for(int i = 0; i < WND_MAXNUM; i++)
	{
		m_wndVideo[i].Create(
			NULL, 
			NULL, 
			WS_VISIBLE | WS_CHILD,
			CRect(0, 0, 0, 0),
			this,
			1986,
			NULL);
		
		m_wndVideo[i].SetWinID(i);		
		AddPage(&m_wndVideo[i]);
	}	
	SetActivePage(&m_wndVideo[0], TRUE);
	SetDrawActivePage(TRUE, RGB(248,5,182), RGB(248,5,182));
	
	return 0;
}

void CScreenPannel::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	for(int i=0; i < WND_MAXNUM; i++)
	{
		m_wndVideo[i].DestroyWindow();
	}
}

BOOL CScreenPannel::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(RGB(60,60,60));
	pDC->FillRect(&rt,&br);
	
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}

void CScreenPannel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateWnd();
	// Do not call CWnd::OnPaint() for painting messages
}

int CScreenPannel::SetShowPlayWin(int nMain)
{
	int nNum = 16;
	int nBegin = 0;
	switch(nMain) 
	{
	case SPLIT1:
		nNum = 1;
		nBegin = 0;
		break;
	case SPLIT4:
		nNum = 4;
		if (m_curScreen >= 12)
		{
			nBegin = 12;
		}
		else if (m_curScreen >= 8)
		{
			nBegin = 8;
		}
		else if (m_curScreen >= 4)
		{
			nBegin = 4;
		}
		else
		{
			nBegin = 0;
		}
		break;
	case SPLIT9:
		nNum = 9;
		if (m_curScreen >= 10)
		{
			nBegin = 7;
		}
		else
		{
			nBegin = 0;
		}
		break;
	case SPLIT16:
		nNum = 16;
		nBegin = 0;
		break;
	default:
		break;
	}
	
	CList<CWnd*, CWnd*> templist;
	
	POSITION pos = m_PageList.GetHeadPosition();
	while(pos != NULL)
	{
		CWnd* pWnd = m_PageList.GetNext(pos);
		if (pWnd)
		{
			templist.AddTail(pWnd);
		}
	}	
	m_PageList.RemoveAll();
	
	for(int i=nBegin; i < (nBegin+nNum); i++)
	{
		AddPage(&m_wndVideo[i], TRUE);
	}
	
	SetActivePage(&m_wndVideo[m_curScreen], TRUE);
	
	pos = templist.GetHeadPosition();
	while(pos != NULL)
	{
		CWnd* pWnd = templist.GetNext(pos);
		if (pWnd)
		{
			pWnd->ShowWindow(SW_HIDE);
		}
	}	
	templist.RemoveAll();
	
	return m_PageList.GetCount();
}

void CScreenPannel::SwitchMultiWnd(int nSplit)
{
	if (nSplit<0 || nSplit >SPLIT16)
	{
		nSplit=0;
	}

	m_Split=nSplit;
	if (SPLIT1 == nSplit)
	{
		SetMultiScreen(FALSE);
	}
	else
	{
		SetMultiScreen(TRUE);
		SetShowPlayWin(nSplit);
	}	
}

void CScreenPannel::SetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser)
{
	m_pMessageProc = cbMessageProc;
	m_dwMessageUser = dwMessageUser;
}


void CScreenPannel::OnVideoMenu(UINT nWndID,WPARAM wParam,LPARAM lParam)
{
	try
	{
		if (m_pMessageProc!=NULL)
		{
			m_pMessageProc(nWndID,wParam,lParam,m_dwMessageUser);
		}
	}
	catch (...)
	{
		
	}
}

int CScreenPannel::GetPlayWndState()
{
	return m_wndVideo[m_curScreen].GetPlayStatez();
}

void CScreenPannel::SetPlayWndState(int playstate,bool bAdd)
{
	m_wndVideo[m_curScreen].SetPlayStatez(playstate,bAdd);
	if (bAdd)
	{		
		if (playstate==BSDKRealPlayz || playstate==BPlatRealPlayz)
		{
			CWnd *pWnd=GetNextPage(GetActivePage());			
			SetActivePage(pWnd);
			m_curScreen=((CPlayWndz*)pWnd)->GetWinID();
	 	}
	}
}

void CScreenPannel::SetPlayWndThiS(LPCTSTR pstrFormat, ... )
{
	CString str;
	va_list avlist;
	va_start(avlist, pstrFormat);
	str.FormatV(pstrFormat, avlist);
	va_end(avlist);
	m_wndVideo[m_curScreen].SetTiShiText(str);
	m_wndVideo[m_curScreen].Invalidate();
	WriteLogz("SetPlayWndThiS:%s",str);
}


// void CScreenPannel::SetVoiceSound(int ivoice, bool btype, UINT uWndID)
// {
// 	m_wndVideo[uWndID].SetVoiceSound(ivoice,btype);
// }

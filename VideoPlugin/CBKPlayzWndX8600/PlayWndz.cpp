// PlayWndz.cpp : implementation file
//

#include "stdafx.h"
#include "CBKPlayzWndX.h"
#include "PlayWndz.h"
#include "ScreenPannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayWndz

CPlayWndz::CPlayWndz()
{
	m_nWndID=0;
	m_nPreSplit = 0;
	m_bType=0;
	m_PlayState=BNoPlayZ;
//	m_bPlaySound=false;
//	m_bVoiceCom=false;
}

CPlayWndz::~CPlayWndz()
{

}


BEGIN_MESSAGE_MAP(CPlayWndz, CWnd)
	//{{AFX_MSG_MAP(CPlayWndz)
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(VIDEO_MENU_BASE, VIDEO_MENU_END, OnVideoMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPlayWndz message handlers

BOOL CPlayWndz::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(VIDEO_BACK_COLOR);
	pDC->FillRect(&rt,&br);
  	if ((m_PlayState & BPlatRealPlayz) || (m_PlayState & BSDKRealPlayz))
	{
		TRACE("字符不叠加 %d\n",m_PlayState);
	}
	else
	{
		if (m_bType==0)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(30,30,30));
			pDC->DrawText("北科舟宇",&rt,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (m_bType==1)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(230,30,30));
			pDC->DrawText(m_TiShiText,&rt,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			
		}
	}
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}

void CPlayWndz::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	pContainer->SetActivePage(this);

	//////////////////////////////////////////////////////////////////////////
	CMenu mPopMenu;
	mPopMenu.CreatePopupMenu();
	mPopMenu.AppendMenu(MF_STRING | (pContainer->GetSplit()==0 ?MF_CHECKED : MF_UNCHECKED),VIDEO_MENU_PIC1," 1画面");
	mPopMenu.AppendMenu(MF_STRING | (pContainer->GetSplit()==1 ?MF_CHECKED : MF_UNCHECKED),VIDEO_MENU_PIC4," 4画面");
	mPopMenu.AppendMenu(MF_STRING | (pContainer->GetSplit()==2 ?MF_CHECKED : MF_UNCHECKED),VIDEO_MENU_PIC9," 9画面");
	mPopMenu.AppendMenu(MF_STRING | (pContainer->GetSplit()==3 ?MF_CHECKED : MF_UNCHECKED),VIDEO_MENU_PIC16,"16画面");
	//////////////////////////////////////////////////////////////////////////
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING | pContainer->GetFullScreen()? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_FULLSCREEN, NAME_MENU_FULLSCREEN);
//	menu.AppendMenu(MF_STRING | pContainer->GetMultiScreen()    ? MF_CHECKED : MF_UNCHECKED,VIDEO_MENU_MULTISCREEN, NAME_MENU_MULTISCREEN);
	menu.AppendMenu(MF_STRING | pContainer->GetAutoAdjustPos() ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_AUTOADJUST, NAME_MENU_AUTOADJUST);
	menu.AppendMenu(MF_POPUP,(UINT)mPopMenu.m_hMenu,"画面布局");
	
	if ((m_PlayState & BPlatRealPlayz) || (m_PlayState & BSDKRealPlayz))
	{	//开始对讲//停止对讲//打开声音//关闭声音
		menu.AppendMenu(MF_SEPARATOR);//分隔符
		menu.AppendMenu(MF_STRING , VIDEO_MENU_VoiceCom, (m_PlayState & BSDKVoiceCom)?"停止对讲":"开始对讲");
		menu.AppendMenu(MF_STRING , VIDEO_MENU_PlaySound, (m_PlayState & BSDKPlaySound)?"关闭声音":"打开声音");
		menu.AppendMenu(MF_STRING , VIDEO_MENU_RENGONGPP, "人工评判");
		menu.AppendMenu(MF_SEPARATOR);//分隔符
		menu.AppendMenu(MF_STRING , VIDEO_MENU_STOPPlay, "关闭播放");
	}
	
	TrackPopupMenu( 
		menu.m_hMenu, 
		TPM_LEFTALIGN, 
		point.x, 
		point.y,
		0,
		m_hWnd,
		NULL);
}

BOOL CPlayWndz::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::DestroyWindow();
}

LRESULT CPlayWndz::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	if(pContainer)
	{
		switch(message)
		{
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{			
				pContainer->SetActivePage(this);
				pContainer->SetCurWindId(m_nWndID);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
 				BOOL bMulti = pContainer->GetMultiScreen();
 				pContainer->SetMultiScreen(!bMulti);
				
				if (TRUE == bMulti)
				{
					m_nPreSplit = pContainer->GetSplit();//窗口数
					pContainer->SetSplit(SPLIT1);
				}
				else
				{
					pContainer->SetSplit(m_nPreSplit);
				}
			}
			break;
		default:
			break;
		}
	}
	return CWnd::DefWindowProc(message, wParam, lParam);
}

void CPlayWndz::OnVideoMenu(UINT nID)
{
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	switch(nID)
	{
	case VIDEO_MENU_FULLSCREEN://全屏
		pContainer->SetFullScreen(!pContainer->GetFullScreen());
		break;
// 	case VIDEO_MENU_MULTISCREEN://多屏
// 		{
// 			BOOL bMulti = pContainer->GetMultiScreen();
// 			pContainer->SetMultiScreen(!bMulti);
// 			
// 			if (TRUE == bMulti)
// 			{
// 				m_nPreSplit = pContainer->GetSplit();
// 				pContainer->SetSplit(SPLIT1);
// 			}
// 			else
// 			{
// 				pContainer->SetSplit(m_nPreSplit);
// 			}			
// 			break;
// 		}		
	case VIDEO_MENU_AUTOADJUST://自动
		pContainer->SetAutoAdjustPos(!pContainer->GetAutoAdjustPos());
		break;
	
	case VIDEO_MENU_VoiceCom:
		pContainer->OnVideoMenu(m_nWndID,nID,(m_PlayState & BSDKVoiceCom));
		break;
	case VIDEO_MENU_PlaySound:
		pContainer->OnVideoMenu(m_nWndID,nID,(m_PlayState & BSDKPlaySound));
		break;
	case VIDEO_MENU_RENGONGPP:
	case VIDEO_MENU_STOPPlay:
		pContainer->OnVideoMenu(m_nWndID,nID,0);
		break;		
	case VIDEO_MENU_PIC1:
		pContainer->SwitchMultiWnd(SPLIT1);
		break;
	case VIDEO_MENU_PIC4:
		pContainer->SwitchMultiWnd(SPLIT4);
		break;
	case VIDEO_MENU_PIC9:
		pContainer->SwitchMultiWnd(SPLIT9);
		break;
	case VIDEO_MENU_PIC16:
		pContainer->SwitchMultiWnd(SPLIT16);
		break;
	default:
		break;
	}
}

void CPlayWndz::SetWinID(int ID)
{
	m_nWndID = ID;
}

int CPlayWndz::GetWinID(void)
{
	return m_nWndID;
}

void CPlayWndz::SetTiShiText(CString str)
{
	if (str.IsEmpty())
	{
		m_bType=2;
	}
	else
	{
		m_bType=1;
		m_TiShiText.Format("%s",str);
	}
}

int CPlayWndz::GetPlayStatez()
{
	return m_PlayState;
}

void CPlayWndz::SetPlayStatez(int playstate,bool isAdd)
{
	if (playstate==BNoPlayZ)
	{
		m_PlayState=BNoPlayZ;
	}
	else
	{
		if (isAdd)
		{
			m_PlayState=m_PlayState | playstate ;
		}
		else
		{
			m_PlayState=m_PlayState & (~playstate) ;
		}
		
	}
}

// void CPlayWndz::SetVoiceSound(int ivoice, bool btype)
// {
// 	if (ivoice==1)
// 	{
// 		m_bVoiceCom=btype;
// 	}
// 	else
// 	{
// 		m_bPlaySound=btype;
// 	}
// }

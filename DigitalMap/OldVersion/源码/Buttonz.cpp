// Buttonz.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "Buttonz.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKDigitalMapApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CButtonz

CButtonz::CButtonz()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;

	m_pToolTip = NULL;
	ExamCarState = 0;
	isCircle = 0;//是否处于中心点 1 为是 0 为不是
	bTrack = FALSE;//默认没有跟踪
	CurCar_Xpoint = 0; //当前的考车坐标经纬度
	CurCar_Ypoint = 0; //当前的考车坐标经纬度
	CurCar_ZT=1;
	CurCar_EID=1000;

}

CButtonz::~CButtonz()
{
	if(m_pToolTip)
	{	
		delete m_pToolTip;
	}
}


BEGIN_MESSAGE_MAP(CButtonz, CButton)
	//{{AFX_MSG_MAP(CButtonz)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSEMOVE, OnMouseMove)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonz message handlers

void CButtonz::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}
//////////////////////////////////////////////////////////////////////////
void CButtonz::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default



	
	if (!m_bTracking)
	{

		CRect    rectBtn;  //按钮的范围
		CPoint    ppoint;
		
		GetWindowRect( rectBtn );
		Cur_xPoint = rectBtn.left;
		Cur_yPoint = rectBtn.top;
	   GetCursorPos( &ppoint );

		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking =_TrackMouseEvent(&tme);


	}
	
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CButtonz::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Sleep(50);
	Invalidate(FALSE);
	return 0;
}

/**********************************************************
bErase参数指定了在处理更新区域的时候是否要擦除更新区域内的背景。
如果bErase为TRUE，则当调用BeginPaint的时候，将擦除背景。
如果bErase为FALSE，则背景保持不变。如果对于更新区域的任何部分bErase为TRUE，
则整个区域的背景都会被擦除，而不仅是给定的部分。 
**********************************************************/
LRESULT CButtonz::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Sleep(50);
	Invalidate(FALSE);
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CButtonz::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	// TODO: Add your code to draw the specified item
	//获取保存在DRAWITEMSTRUCT结构中且在绘制按钮时必须使用的设备上下文
	CDC* mydc=CDC::FromHandle(lpDrawItemStruct->hDC);
	//创建兼容的设备上下文
	CDC* pMenDC=new CDC;
	pMenDC->CreateCompatibleDC(mydc);
	CDC m_dcBuf;
	m_dcBuf.CreateCompatibleDC(mydc);
	//保存旧对象
	CBitmap * pOldBitmap;
	pOldBitmap =pMenDC ->SelectObject(&mybitmap);


	//	CPoint point(0,0);
	//判断按钮是否处于选择状态，如果是则绘制选择状态的位图，也就是选中第二个
	if (lpDrawItemStruct->itemState & ODS_SELECTED) //ODS_SELECTED
	{
		//mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCCOPY);
		// 从源设备上下文拷贝位图到这个当前设备上下文
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,m_ButtonSize.cy,SRCAND);
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCPAINT);
		
	}
	else
	{	
		//判断鼠标是否离开还是在按钮上面，以便绘制相应的位图
		if (m_bHover)
		{
			//mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCCOPY);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,m_ButtonSize.cy,SRCAND);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCPAINT);
			::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSGZ,examCarNumber,ThreadNumber);
			
		}else
		{
			//mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCCOPY);
			if (bTrack)
			{
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,m_ButtonSize.cy,SRCAND);
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCPAINT);
			}else
			{
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,m_ButtonSize.cy,SRCAND);
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCPAINT);
			}

			::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSGZLeft,ThreadNumber,0);
		}
	}
//	pMenDC->SetTextColor(RGB(0,255,0));
	CRect RC;
	GetClientRect(&RC);
	CString CarNumber;
	CarNumber.Format(" %d ",examCarNumber); 

	CFont pFont;
	pFont.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");                 // lpszFacename

//	pFont.CreatePointFont(100, "Arial", NULL);  
	CFont* def_font = pMenDC->SelectObject(&pFont);
	pMenDC->DrawText(CarNumber,RC,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
	pMenDC->SelectObject(def_font);
	pFont.DeleteObject();
	pMenDC ->SelectObject(pOldBitmap);
	

	delete pMenDC;
	this->ReleaseDC(mydc);
	this->ReleaseDC(&m_dcBuf);
	if (pCCBKDigitalMapDlg->iCountFlushExamCar > 0)
	{
		pCCBKDigitalMapDlg->iCountFlushExamCar--;//用来统计考车刷新的次数 当为0的时候才可以刷新地图
	}
}
//////////////////////////////////////////////////////////////////////////
BOOL CButtonz::LoadBitmap(UINT bitmapid)
{
	//载入图片
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid),\
		IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP bitmapbits;
	//获取位图信息并存入Bitmapbits结构中
	mybitmap.GetBitmap(&bitmapbits);
	
	//取位图相应的高度和 1/3宽度
	m_ButtonSize.cy =bitmapbits.bmHeight/2;
	m_ButtonSize.cx =bitmapbits.bmWidth/3;

// 	CRgn rgn;
//     rgn. CreateEllipticRgn(0,0,m_ButtonSize.cx,m_ButtonSize.cy);
//     SetWindowRgn(rgn,TRUE);

	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

BOOL CButtonz::LoadBitmap(LPCTSTR lpszName)
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
	
	//取位图相应的高度和 1/3宽度
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/3;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

BOOL CButtonz::PreTranslateMessage(MSG* pMsg) 
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

void CButtonz::SetToolTipText(CString spText, BOOL bActivate)
{
	if (spText.IsEmpty()) return;
	
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		m_pToolTip->Create(this);
		//m_pToolTip->Activate(TRUE);
	}
	m_tooltext = spText;
// 	if (m_pToolTip->GetToolCount() == 0)
// 	{
// 		CRect rectBtn; 
// 		GetClientRect(rectBtn);
// 		m_pToolTip->AddTool(this, m_tooltext, rectBtn, 1);
// 	}
// 	m_pToolTip->UpdateTipText(m_tooltext, this, 1);
	m_pToolTip->AddTool(this, m_tooltext);
	m_pToolTip->SetDelayTime(1000);
	m_pToolTip->Activate(bActivate);
}

void CButtonz::SetToolTipText(UINT nId, BOOL bActivate)
{
	m_tooltext.LoadString(nId);
	// If string resource is not empty
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}
}


BOOL CButtonz::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

/**********************************8
函数说明：写错误信息进入配置文件
************************************/
void CButtonz::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}


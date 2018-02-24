// BmpDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "LimitTime.h"
#include "resource.h"
#include "BmpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define BKtopSize 44
#define MSGLEFT 20 //左边20
#define MSGTOP	10 
#define MSGHEIGHT 35
/////////////////////////////////////////////////////////////////////////////
// CBmpDlg dialog


CBmpDlg::CBmpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBmpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBmpDlg)
	DDX_Control(pDX, IDOK, m_BIDO);
	DDX_Control(pDX, IDCANCEL, m_BIDC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBmpDlg, CDialog)
	//{{AFX_MSG_MAP(CBmpDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpDlg message handlers

void CBmpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CBmpDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CBmpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BIDC.LoadBitmap(IDB_BMPBTNC);
	m_BIDO.LoadBitmap(IDB_BMPBTNO);
//////////////////////////////////////////////////////////////////////////
	//移动按钮位置
	CRect rect;
	GetDlgItem(IDCANCEL)->GetWindowRect(&rect);  //要获取位置的控件ID
	ScreenToClient(rect); //rect.left 就是X坐标,rect.top 就是Y坐标
	rect.top+=LineHeight-35;
	rect.bottom+=LineHeight-35;
	GetDlgItem(IDCANCEL)->MoveWindow(rect);
	GetDlgItem(IDOK)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.top+=LineHeight-35;
	rect.bottom+=LineHeight-35;
	GetDlgItem(IDOK)->MoveWindow(rect);
//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBmpDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CBmpDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	DoHaoCString(dlgMessage,msg_dest,',');
	lineNum=msg_dest.GetSize();
	LineHeight=(lineNum-1)*35;
	SetWindowPos(NULL,0,0,284,100+LineHeight,SWP_NOMOVE | SWP_NOOWNERZORDER );
	CRgn m_rgn;
	CRect rc;	
	GetClientRect(&rc);
	rc-=rc.TopLeft();
	m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,4,4);
	SetWindowRgn(m_rgn,TRUE);
}

BOOL CBmpDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BMP_MESSAGEBK); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rect.Width(),BKtopSize,&dcMem,0,0,bitmap.bmWidth,BKtopSize,SRCCOPY);
	dc.StretchBlt(0,BKtopSize,rect.Width(),LineHeight,&dcMem,0,BKtopSize,bitmap.bmWidth,1,SRCCOPY);
	dc.StretchBlt(0,BKtopSize+LineHeight,rect.Width(),rect.Height()-BKtopSize-LineHeight,&dcMem,\
		0,BKtopSize+35,bitmap.bmWidth,bitmap.bmHeight-BKtopSize-35,SRCCOPY);
	
	//////////////////////////////////////////////////////////////////////////
	CFont font1,font2,*oldfont;
	
	font1.CreateFont(20, 0, 0, 0, FW_DEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	
	font2.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	//////////////////////////////////////////////////////////////////////////
	dc.SetTextColor(RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);
	//////////////////////////////////////////////////////////////////////////
	//标题
	oldfont=dc.SelectObject(&font1);
	CRect r1(MSGLEFT,MSGTOP,rect.right-MSGLEFT,MSGTOP+MSGHEIGHT);
	dc.DrawText(msg_dest[0],&r1,DT_CENTER);
	oldfont=dc.SelectObject(&font2);
	dc.SetTextColor(RGB(86,86,86));
	int i;
	for (i=1;i<(int)lineNum;i++)
	{
		CRect r2(MSGLEFT,8+BKtopSize+MSGHEIGHT*(i-1),rect.Width()-MSGLEFT,BKtopSize+MSGHEIGHT*i);
		dc.DrawText(msg_dest[i],&r2,DT_CENTER);
		
	}
	font1.DeleteObject();
	font2.DeleteObject();
	oldfont->DeleteObject();
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


//////////////////////////////////////////////////////////////////////////
//参数说明:  
//source： 需要分割的源字符串；
//dest： 存储分割后的各个字符串变量的目标变量；
//division：分割字符
//////////////////////////////////////////////////////////////////////////
void CBmpDlg::DoHaoCString(CString source,  CStringArray& dest, char division)
{
	dest.RemoveAll();
	int nCount=0;
	for(int i=0;i< source.GetLength();i++)
	{
		if(source.GetAt(i)== division)
		{
			nCount=i;
			dest.Add(source.Left(i)); //去掉右边
			int nIndex=dest.GetSize()-1;
			for(int j=0;j <nIndex;j++)
			{
				dest[nIndex] = dest[nIndex].Right(dest[nIndex].GetLength()-dest[j].GetLength()-1); //去掉左边
			}
		}
	}
	//处理最后一个分隔符division右边的剩余部分。
	dest.Add(source);
	int nIndex2=dest.GetSize()-1;
	for(int j=0;j <nIndex2;j++)
	{
		dest[nIndex2] = dest[nIndex2].Right(dest[nIndex2].GetLength()-dest[j].GetLength()-1); //去掉左边
	}
} 

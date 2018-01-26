// YLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autoprintmanager.h"
#include "YLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYLDlg dialog


CYLDlg::CYLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYLDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYLDlg, CDialog)
	//{{AFX_MSG_MAP(CYLDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYLDlg message handlers

void CYLDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,525,750,SWP_NOMOVE | SWP_NOOWNERZORDER );
}

BOOL CYLDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bmpMap.m_hObject = LoadImage(NULL,".\\skin.ui",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (m_bmpMap.m_hObject==NULL)
	{
		AfxMessageBox("¼ÓÔØÆ¤·ôÊ§°Ü!"); 
		//TerminateProcess(GetCurrentProcess(),0); 
		return FALSE;
	}
	CDC *pDC = this->GetDC();
	BITMAP bitmap; 
	CBitmap	m_bmpTmp;
	m_dcMem.CreateCompatibleDC(pDC);
	m_bmpMap.GetBitmap(&bitmap);
	dx=bitmap.bmWidth;
	dy=bitmap.bmHeight;
	m_bmpTmp.CreateCompatibleBitmap(pDC,bitmap.bmWidth,bitmap.bmHeight);
	m_dcMem.SelectObject(m_bmpTmp);
	this->ReleaseDC(pDC);
	m_dcMem.SelectObject(m_bmpMap);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CYLDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
}

BOOL CYLDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect; 
	CPaintDC dc(this);
	GetClientRect(&rect); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,0,0,dx,dy,SRCCOPY); 
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

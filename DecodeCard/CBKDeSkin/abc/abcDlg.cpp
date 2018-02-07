// abcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "abc.h"
#include "abcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbcDlg dialog

CAbcDlg::CAbcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAbcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAbcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAbcDlg, CDialog)
	//{{AFX_MSG_MAP(CAbcDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbcDlg message handlers

BOOL CAbcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	dllHandle = LoadLibrary(TEXT("CBKDeSkin.dll")); 
	if (dllHandle == NULL) 
	{ 
		AfxMessageBox("CBKSkinbmp.dll ß∞‹!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE; 
	}  
	
    zbmpmessagebox = (DrawImage) GetProcAddress(dllHandle,TEXT("DrawImage"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAbcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAbcDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAbcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAbcDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(1,1,1);
}

void CAbcDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CString temp,temp1;
	temp.Format("kch%d",1);
	for (int i=0;i<17;i++)
	{
		temp1.Format("MSG%d",i);
		::WritePrivateProfileString(temp,temp1,temp1,".\\allcard.msg");//…Ë±∏∂Àø⁄
	}
}

void CAbcDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(1,1,0);//51
}

void CAbcDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,302);//52 
}

void CAbcDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(3,1,2);//53      0,1,2
}

void CAbcDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,399);//55
}

void CAbcDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(6,1,1);//56 1,2
}

void CAbcDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(6,1,2);//56 1,2
}

void CAbcDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,402);//52 
}

void CAbcDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,501);//52 
}

void CAbcDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,602);//52 
}

void CAbcDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,702);//52 
}

void CAbcDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,249);//52 
}

void CAbcDlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(2,1,259);//52
}

void CAbcDlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,499);//55
}

void CAbcDlg::OnButton16() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,599);//55
}

void CAbcDlg::OnButton17() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,699);//55
}

void CAbcDlg::OnButton18() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,799);//55
}

void CAbcDlg::OnButton19() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,899);//55
}

void CAbcDlg::OnButton20() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(5,1,999);//55
}

void CAbcDlg::OnButton21() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(3,1,1);//53      0,1,2
}

void CAbcDlg::OnButton22() 
{
	// TODO: Add your control notification handler code here
	zbmpmessagebox(3,1,0);//53      0,1,2
}

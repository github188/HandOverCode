// DlgPlayWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CBKFourPicKM2.h"
#include "DlgPlayWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKFourPicKM2App theApp;

#define XMLB_RGB RGB(157,160,171)
#define TEXT_RGB RGB(187,187,187)
/////////////////////////////////////////////////////////////////////////////
// CDlgPlayWnd dialog


CDlgPlayWnd::CDlgPlayWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlayWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPlayWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StuName=_T("");
	m_StuSFZM=_T("");
	m_StuZKZ=_T("");
	uRecordCount=0;
	uksCS=0;
	uksRs=0;
	//////////////////////////////////////////////////////////////////////////
	nFrames= 0;
	opts[0]=&pCompressOption;
	m_IsTime=TRUE;
	isStartAvi=FALSE;
	//////////////////////////////////////////////////////////////////////////
//	IsPLAYWND2=FALSE;
}


void CDlgPlayWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPlayWnd)
	DDX_Control(pDX, IDC_VoiceIn, m_BNTVoice);
	DDX_Control(pDX, IDC_VoiceClose, m_BNTVoice2);
	DDX_Control(pDX, IDC_BTNJudge, m_BTNJudge);
	DDX_Control(pDX, IDC_BUTTON2, m_BNTSpeak2);
	DDX_Control(pDX, IDC_BUTTON1, m_BNTSpeak);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BTNClose);
	DDX_Control(pDX, IDC_STATIC_KCH, m_SKCH);
	DDX_Control(pDX, IDC_STATIC_TEXT4, m_Stext4);
	DDX_Control(pDX, IDC_STATIC_TEXT3, m_Stext3);
	DDX_Control(pDX, IDC_STATIC_TEXT2, m_Stext2);
	DDX_Control(pDX, IDC_STATIC_TEXT1, m_Stext1);
	DDX_Control(pDX, IDC_STATIC_XM7, m_xmlb7);
	DDX_Control(pDX, IDC_STATIC_XM6, m_xmlb6);
	DDX_Control(pDX, IDC_STATIC_XM5, m_xmlb5);
	DDX_Control(pDX, IDC_STATIC_XM4, m_xmlb4);
	DDX_Control(pDX, IDC_STATIC_XM3, m_xmlb3);
	DDX_Control(pDX, IDC_STATIC_XM2, m_xmlb2);
	DDX_Control(pDX, IDC_STATIC_XM1, m_xmlb1);
	DDX_Control(pDX, IDC_LIST_ERROR, m_ListCtrlError);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlayWnd, CDialog)
	//{{AFX_MSG_MAP(CDlgPlayWnd)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_VoiceIn, OnVoiceIn)
	ON_BN_CLICKED(IDC_VoiceClose, OnVoiceClose)
	ON_BN_CLICKED(IDC_BTNJudge, OnBTNJudge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayWnd message handlers

HBRUSH CDlgPlayWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG)
	{
		// 		pDC->SetBkMode(TRANSPARENT);  
		//		pDC->SetTextColor(RGB(10,255,255));
		HBRUSH   hbr1=CreateSolidBrush(RGB(138,138,138)); 
		return   hbr1; 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgPlayWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,720,450,SWP_NOMOVE | SWP_NOOWNERZORDER );
}

BOOL CDlgPlayWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	logid=logz.Register("CDlgPlayWnd.log");
	logxmid=logz.Register("CXMCode.log");
	//设置各屏项目列表0 为考五项 其它值为 考七项
	uXmlb=GetPrivateProfileInt("UIskin","xmlb",0,".\\config.ini");
	//////////////////////////////////////////////////////////////////////////
	//错误列表是否显示0表示显示扣分为0的项
	ErrListShow=GetPrivateProfileInt("UIskin","ErrListShow",1,".\\config.ini");
	//////////////////////////////////////////////////////////////////////////
	OnMoveandUI();
	if (!OnInitSQLw())
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
	}
	//////////////////////////////////////////////////////////////////////////
	if (uKch ==0)
	{
		return FALSE;
	}
	//统计考车总人数和总次数
	CString temp;
	temp.Format("考车%d",uKch);
	CountExam(temp);
	//打开定时器，分别加载视频和考试信息
	SetTimer(INITWNDTIME,1000,NULL);
	SetTimer(STUZKZTIME,5000,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlayWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rect; 
	GetClientRect(&rect);
	theApp.lpfnDrawBkgnd(dc.GetSafeHdc(),rect,0);
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgPlayWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//MessageBox(_T("回车键按下"));
			//			return TRUE;
		case VK_ESCAPE:
			//MessageBox(_T("ESC键按下"));
			if (theApp.zbmpmessagebox("退出提示,确定要退出程序么?"))
			{
				//return TRUE;
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
			//	GetParent()->SendMessage(WM_CLOSE);
			}
			else
			{
				return TRUE;
			}
			break;
			//return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
void CDlgPlayWnd::OnShowPHO()
{
	CDC *pDC = this->GetDC();
	CDC dcMem; 
	dcMem.CreateCompatibleDC(pDC); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BMPPHOBK); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
	this->ReleaseDC(pDC);
	CRect rt(0,0,bitmap.bmWidth,bitmap.bmHeight);
	ReadPhotoFromDB(&dcMem,m_StuZKZ,&rt);
	//////////////////////////////////////////////////////////////////////////
	HDC hdc;
	hdc=GetDlgItem(IDC_STATIC_PHO)->GetDC()->m_hDC;
	CDC *pDC1= CDC::FromHandle(hdc);
	pDC1->StretchBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	pDC1->DeleteDC();
	//////////////////////////////////////////////////////////////////////////
}
BOOL CDlgPlayWnd::ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
   
	CString temp;
	try
	{
// 		BOOL sInit=theApp.OnInitSQL();
// 		if (sInit)
// 		{
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;
			CString strSQL;	
			strSQL.Format("SELECT * FROM StudentPhoto WHERE 准考证明编号 = '%s'",sCard);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnwnd, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			if(pRecordset->RecordCount != 1)
			{
				return FALSE;
			}
			
			char* pbuf = NULL;
			
			long lDataSize = pRecordset->GetFields()->GetItem("照片")->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{
				_variant_t varBLOB;
				varBLOB = pRecordset->GetFields()->GetItem("照片")->GetChunk(lDataSize);
				m_pBMPBuffer = new char[lDataSize+1];
				
				pRecordset->Close();
				pRecordset.Release();
				
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					
					SafeArrayUnaccessData(varBLOB.parray);
					
					//******************************************************//
					IStream* pStm;
					LONGLONG cb = lDataSize;
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
					LPVOID pvData;
					if (hGlobal != NULL)
					{
						pvData = GlobalLock(hGlobal);
						memcpy(pvData, m_pBMPBuffer, cb);
						GlobalUnlock(hGlobal);
						CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
					}
					else
					{
						//AfxMessageBox("读取照片信息错误");
						return FALSE;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
					}
					else
					{
						return FALSE;
					}
					pStm->Release ();
					pImg->Release();
					GlobalFree(hGlobal); 
					if(m_pBMPBuffer)
					{
						delete m_pBMPBuffer;
						m_pBMPBuffer = NULL;
					}
					
				}
			}
			else
			{
				pRecordset->Close();
				pRecordset.Release();
			}
//		}

	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		return FALSE;
	}
	
	return TRUE;
}

void CDlgPlayWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
// 	
	switch (nIDEvent)
	{
	case INITWNDTIME:
		OnInitTime();
		break;
	case ERRORCODETIME:
		OnErrorcodeTime();
		break;
	case STUZKZTIME:
		OnStuzkzTime();
		break;
	case VIDEOPLAY_TIME:
		OnRecord();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgPlayWnd::OnMoveandUI()
{
	CRect clirc;
	GetClientRect(&clirc);
	CRect rc1,rc2;
	rc1.SetRect(clirc.left+4,clirc.top+4,299,225);
	rc2.SetRect(rc1.right,rc1.top,rc1.right+rc1.Width(),rc1.bottom);
	GetDlgItem(IDC_STATIC_WND1)->MoveWindow(rc1);
	GetDlgItem(IDC_STATIC_WND2)->MoveWindow(rc2);
	GetDlgItem(IDC_STATIC_PHO)->MoveWindow(rc2.right+3,rc1.bottom-156,118,154);
	GetDlgItem(IDC_BTN_EXIT)->MoveWindow(670,0,47,22);
	GetDlgItem(IDC_BUTTON1)->MoveWindow(rc2.right,0,46,36);
	GetDlgItem(IDC_BUTTON2)->MoveWindow(rc2.right,0,46,36);
	GetDlgItem(IDC_VoiceIn)->MoveWindow(rc2.right,37,86,22);
	GetDlgItem(IDC_VoiceClose)->MoveWindow(rc2.right,37,86,22);
	GetDlgItem(IDC_BTNJudge)->MoveWindow(rc2.right+50,37,86,22);
	GetDlgItem(IDC_STATIC_KCH)->MoveWindow(rc2.right+47,15,72,36);
	GetDlgItem(IDC_BTN_EXIT)->MoveWindow(670,0,47,22);
	m_BTNClose.LoadBitmap(IDB_BMPCLOSE);
	m_BNTSpeak.LoadBitmap(IDB_BMPSPEAK1);
	m_BNTSpeak2.LoadBitmap(IDB_BMPSPEAK2);
	m_BNTVoice.LoadBitmap(IDB_BMPVoice2);
	m_BNTVoice2.LoadBitmap(IDB_BMPVoice1);
	m_BTNJudge.LoadBitmap(IDB_BMPJudge);

	CRect rctemp(554,234,160,28);
	int XMh=29;
	GetDlgItem(IDC_STATIC_XM1)->MoveWindow(rctemp.left,rctemp.top,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_XM2)->MoveWindow(rctemp.left,rctemp.top+XMh,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_XM5)->MoveWindow(rctemp.left,rctemp.top+XMh*2,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_XM4)->MoveWindow(rctemp.left,rctemp.top+XMh*3,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_XM3)->MoveWindow(rctemp.left,rctemp.top+XMh*4,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_XM6)->MoveWindow(rctemp.left,rctemp.top+XMh*5,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_XM7)->MoveWindow(rctemp.left,rctemp.top+XMh*6,rctemp.right,rctemp.bottom);
	
	rctemp.SetRect(64,240,160,22);
	//XMh = 28 ;
	GetDlgItem(IDC_STATIC_TEXT1)->MoveWindow(rctemp.left,rctemp.top,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_TEXT2)->MoveWindow(rctemp.left,rctemp.top+XMh,rctemp.right,rctemp.bottom);
	GetDlgItem(IDC_STATIC_TEXT3)->MoveWindow(rctemp.left+256,rctemp.top,rctemp.right+60,rctemp.bottom);
	GetDlgItem(IDC_STATIC_TEXT4)->MoveWindow(rctemp.left+256,rctemp.top+XMh,rctemp.right+60,rctemp.bottom);
	
	GetDlgItem(IDC_LIST_ERROR)->MoveWindow(16,300,518,136);
	m_ListCtrlError.SetExtendedStyle(LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP /*|LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE*/);
	m_ListCtrlError.InsertColumn(0,_T("出错时间"),LVCFMT_LEFT,100,-1);
	m_ListCtrlError.InsertColumn(1,_T("扣分原因"),LVCFMT_LEFT,300,-1);
	m_ListCtrlError.InsertColumn(2,_T("扣分"),LVCFMT_LEFT,60,-1);
	m_ListCtrlError.SetLineColor(RGB(223,223,223),RGB(238,238,238));
	m_ListCtrlError.SetHeadColor(RGB(251,251,251),RGB(63,135,0),RGB(85,185,73));
	m_ListCtrlError.SetTextColor(RGB(51,51,51));
	
	m_xmlb1.SetCaption("☉倒车入库");
	m_xmlb2.SetCaption("☉侧方停车");
	m_xmlb3.SetCaption("☉直角转弯");
	m_xmlb4.SetCaption("☉曲线行驶");
	m_xmlb5.SetCaption("☉定点坡起");
	if (0==uXmlb)
	{
		m_xmlb6.SetCaption("");
		m_xmlb7.SetCaption("");
	}
	else
	{
		m_xmlb6.SetCaption("☉雨雾湿滑");
		m_xmlb7.SetCaption("☉模拟隧道");
	}

	
	m_xmlb1.SetTextColor(RGB(51,51,51));
	m_xmlb2.SetTextColor(RGB(51,51,51));
	m_xmlb3.SetTextColor(RGB(51,51,51));
	m_xmlb4.SetTextColor(RGB(51,51,51));
	m_xmlb5.SetTextColor(RGB(51,51,51));
	m_xmlb6.SetTextColor(RGB(51,51,51));
	m_xmlb7.SetTextColor(RGB(51,51,51));

// 	m_xmlb1.SetXmlbColor(1);
// 	m_xmlb3.SetXmlbColor(2);
// 	m_xmlb7.SetXmlbColor(3);
// 	m_xmlb2.SetXmlbColor(3);

	m_Stext1.SetTextColor(RGB(63,135,0));
	m_Stext3.SetTextColor(RGB(63,135,0));
	m_Stext2.SetTextColor(RGB(254,7,0));
	m_Stext4.SetTextColor(RGB(254,7,0));

	CString temp;
	temp.Format("考车%d",uKch);
	m_SKCH.SetCaption(temp,RGB(238,238,238));
	m_SKCH.SetTextnFormat(4);
}

BOOL CDlgPlayWnd::SearchTDH(CString errorcode)
{
	CString temp,tempk;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("select * from TBKVideo where 编号='%s'",errorcode);
		_RecordsetPtr pSet =m_pConnwnd->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("设备IP");
				if (vat.vt != VT_NULL)
				{
					m_xmsbip = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("用户名");
				if (vat.vt != VT_NULL)
				{
					m_xmsbuser = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("密码");
				if (vat.vt != VT_NULL)
				{
					m_xmsbpwd = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("端口号");
				if (vat.vt != VT_NULL)
				{
					m_xmsbport = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("通道号");
				if (vat.vt != VT_NULL)
				{
					m_xmsbtd = (LPCSTR)_bstr_t(vat);
				}
				logz.WriteLog(logxmid,"SearchTDH errorcode=%s,m_xmsbip=%s,m_xmsbuser=%s,m_xmsbpwd=%s,m_xmsbport=%s,m_xmsbtd=%s",errorcode,m_xmsbip,m_xmsbuser,m_xmsbpwd,m_xmsbport,m_xmsbtd);
				pSet->MoveNext();
			}//while
		}//if
		else
		{
			tempk.Format("错误提示,编号为[%s]信息不全.",errorcode);
			theApp.zbmpmessagebox(tempk.GetBuffer(0));
			m_xmsbip=_T("");
			m_xmsbuser=_T("");
			m_xmsbpwd=_T("");
			m_xmsbport=_T("");
			m_xmsbtd=_T("");
			return FALSE;
		}

	}
	catch (_com_error &comError)
	{
		tempk.Format("错误提示,查询通道号失败!,编号为[%s]信息不全,请先配置错误编号与通道关系",errorcode);
		theApp.zbmpmessagebox(tempk.GetBuffer(0));
		TRACE(comError.Description());
		return FALSE;
	}
	return TRUE;
}

void CDlgPlayWnd::OnInitTime()
{	
//	OnShowPHO();
	KillTimer(INITWNDTIME);
	try
	{		
		CString temp;
		temp.Format("考车%d",uKch);
		SearchTDH(temp);
		HWND hwnd;
		hwnd=GetDlgItem(IDC_STATIC_WND1)->GetSafeHwnd();
		if (playwnd1.DvrInitLogin(hwnd,m_xmsbip.GetBuffer(0),m_xmsbuser.GetBuffer(0),m_xmsbpwd.GetBuffer(0),atoi(m_xmsbport)))
		{
			playwnd1.DvrRealPlay(atoi(m_xmsbtd));
		}
	}
	catch (...)
	{
		TRACE("OnInitTime()\n");
	}
}

void CDlgPlayWnd::OnErrorcodeTime()
{
	KillTimer(ERRORCODETIME);
	CString temp,errorcode,ertime,erkflx,erkcfs;
	int iItem;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("SELECT er.错误编号,CONVERT(varchar(12),出错时间,108) as 出错时间,扣分类型,扣除分数 \
			FROM ErrorRecords er,ErrorData ed where er.错误编号=ed.错误编号 and 准考证明编号='%s' and 考试次数='%d' and 当日次数='%d' \
			order by 出错时间 asc",m_StuZKZ,iKscs,iDrcs);
		_RecordsetPtr pSet;
		pSet.CreateInstance("ADODB.Recordset");
		pSet->CursorLocation = adUseClient;
		pSet->Open((_variant_t)_bstr_t(temp), _variant_t((IDispatch*)m_pConnwnd, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
//		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			pSet->MoveLast();
			int ireco=pSet->RecordCount;
			while( ireco>uRecordCount)
			{
				vat =pSet->GetCollect("错误编号");
				if (vat.vt != VT_NULL)
				{
					errorcode = (LPCSTR)_bstr_t(vat);
				}
				else{errorcode =_T("");}
				vat =pSet->GetCollect("出错时间");
				if (vat.vt != VT_NULL)
				{
					ertime = (LPCSTR)_bstr_t(vat);
				}
				else{ertime =_T("");}
				vat =pSet->GetCollect("扣分类型");
				if (vat.vt != VT_NULL)
				{
					erkflx = (LPCSTR)_bstr_t(vat);
				}
				else{erkflx =_T("");}
				vat =pSet->GetCollect("扣除分数");
				if (vat.vt != VT_NULL)
				{
					erkcfs = (LPCSTR)_bstr_t(vat);
				}
				else{erkcfs =_T("");}

				if (ErrListShow ==0 && strcmp(erkcfs,"0")==0)
				{
					iItem=m_ListCtrlError.GetItemCount();
					m_ListCtrlError.InsertItem(iItem,ertime);
					m_ListCtrlError.SetItemText(iItem,1,erkflx);
					m_ListCtrlError.SetItemText(iItem,2,erkcfs);
				}
				OnSwitchXMLB(errorcode);//刷项目颜色
				if (strcmp(erkcfs,"0")!=0)
				{
					iItem=m_ListCtrlError.GetItemCount();
					m_ListCtrlError.InsertItem(iItem,ertime);
					m_ListCtrlError.SetItemText(iItem,1,erkflx);
					m_ListCtrlError.SetItemText(iItem,2,erkcfs);
					OnResult();//计算分数-刷不合格项目颜色
				}				
				pSet->MovePrevious();
				//pSet->MoveNext();
				uRecordCount++;
			}
		}
	}
	catch (_com_error &comError)
	{
		TRACE(comError.Description());
		logz.WriteLog(logid,"OnErrorcodeTime catch error");
		//return ;
	}
	SetTimer(ERRORCODETIME,2000,NULL);
	//SetTimer(STUZKZTIME,5000,NULL);
}

void CDlgPlayWnd::OnStuzkzTime()
{
	CString temp,stuname,stusfz,stuzkz,skscs,sdrcs;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("select 姓名,准考证明编号,身份证明编号,考试次数,当日次数 from StudentInfo \
			where 登录状态='已登录' AND 状态='3' AND 考车号='考车%d' AND \
			(Select CONVERT(varchar(100), 预考日期, 23)) = (Select CONVERT(varchar(100), GETDATE(), 23)) \
			order by 登录顺序 asc",uKch);
		_RecordsetPtr pSet =m_pConnwnd->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			// 			while(!pSet->adoEOF)
			// 			{
			vat =pSet->GetCollect("姓名");
			if (vat.vt != VT_NULL)
			{
				stuname = (LPCSTR)_bstr_t(vat);
			}else{stuname =_T("");}
			vat =pSet->GetCollect("准考证明编号");
			if (vat.vt != VT_NULL)
			{
				stuzkz = (LPCSTR)_bstr_t(vat);
			}else{stuzkz=_T("");}
			vat =pSet->GetCollect("身份证明编号");
			if (vat.vt != VT_NULL)
			{
				stusfz = (LPCSTR)_bstr_t(vat);
			}else{stusfz=_T("");}
			vat =pSet->GetCollect("考试次数");
			if (vat.vt != VT_NULL)
			{
				skscs = (LPCSTR)_bstr_t(vat);
			}else{skscs=_T("0");}
			vat =pSet->GetCollect("当日次数");
			if (vat.vt != VT_NULL)
			{
				sdrcs = (LPCSTR)_bstr_t(vat);
			}else{sdrcs=_T("0");}
			pSet->MoveNext();
			//			}//while
		}//if
	}
	catch (_com_error &comError)
	{
		TRACE(comError.Description());
		logz.WriteLog(logid,"OnStuzkzTime _com_error %s:%s",comError.ErrorMessage(),(LPCTSTR)comError.Description());
		return ;
	}
	int tempkscs,tempdrcs;
	tempdrcs=atoi(sdrcs);
	tempkscs=atoi(skscs);


	if (stuzkz != m_StuZKZ && stuzkz !="")
	{
		SetTimer(ERRORCODETIME,2000,NULL);
		m_ListCtrlError.DeleteAllItems();
		m_StuName=stuname;
		m_StuSFZM=stusfz;
		m_StuZKZ =stuzkz;
		iKscs=tempkscs;
		iDrcs=tempdrcs;
		OnShowPHO();
		m_Stext1.SetCaptionSW(m_StuName);
		m_Stext3.SetCaptionSW(m_StuSFZM);
		m_Stext2.SetCaptionSW("100");//初始化成绩为100分

		uRecordCount=0;
		m_xmlb1.SetXmlbColor(0);
		m_xmlb2.SetXmlbColor(0);
		m_xmlb3.SetXmlbColor(0);
		m_xmlb4.SetXmlbColor(0);
		m_xmlb5.SetXmlbColor(0);
		m_xmlb6.SetXmlbColor(0);
		m_xmlb7.SetXmlbColor(0);
		
		uksRs+=1;
		uksCS+=1;
		temp.Format("%d/%d",iKscs,iDrcs);
	//	temp.Format("%d/%d",uksRs,uksCS);		
		m_Stext4.SetCaptionSW(temp);
		playwnd2.DvrLoginout();
		OnStartAvi();//开始录像
		logz.WriteLog(logid,"考生姓名:%s,%d,%d",m_StuName,iKscs,iDrcs);
	}

	if (tempdrcs !=0 && tempdrcs !=iDrcs) //当日次数不相等
	{
		m_ListCtrlError.DeleteAllItems();
		m_StuName=stuname;
		m_StuSFZM=stusfz;
		m_StuZKZ =stuzkz;
		iKscs=tempkscs;
		iDrcs=tempdrcs;
		OnShowPHO();
		m_Stext1.SetCaptionSW(m_StuName);
		m_Stext3.SetCaptionSW(m_StuSFZM);
		m_Stext2.SetCaptionSW("100");//初始化成绩为100分

		uRecordCount=0;
		
		m_xmlb1.SetXmlbColor(0);
		m_xmlb2.SetXmlbColor(0);
		m_xmlb3.SetXmlbColor(0);
		m_xmlb4.SetXmlbColor(0);
		m_xmlb5.SetXmlbColor(0);
		m_xmlb6.SetXmlbColor(0);
		m_xmlb7.SetXmlbColor(0);
		if (iDrcs!=1)
		{
			uksCS+=1;

			temp.Format("%d/%d",iKscs,iDrcs);
		//	temp.Format("%d/%d",uksRs,uksCS);
			m_Stext4.SetCaptionSW(temp);
		}
		playwnd2.DvrLoginout();
		OnStartAvi();
	}
	OnStuZT();

}

UINT CDlgPlayWnd::QHThreadFuncc(LPVOID Pparam)
{
	CDlgPlayWnd *pInfo=(CDlgPlayWnd *)Pparam;
	CString errcode;
	errcode.Format("Q%s",pInfo->m_errorCodez);
	CString temp,temp1;
	GetPrivateProfileString(errcode,"Time","5000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp1.Format("%s",temp);
	DWORD sleepd=(DWORD) atol(temp1);
	pInfo->logz.WriteLog(pInfo->logid,"QHThreadFunc sleep=%d",sleepd);
	Sleep(sleepd);
	pInfo->OnQHPlay2();

	GetPrivateProfileString(errcode,"Time2","5000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp1.Format("%s",temp);
	sleepd=(DWORD) atol(temp1);
	pInfo->logz.WriteLog(pInfo->logid,"QHThreadFunc sleep2=%d",sleepd);
	Sleep(sleepd);
	pInfo->OnEPlaywnd2(pInfo->m_errorCodez);
	return 0;
}

void CDlgPlayWnd::OnSwitchXMLB(CString errorcode)
{
	logz.WriteLog(logxmid,"考车%d ,XMCode=%s",uKch,errorcode);
	int temp=atoi(errorcode);
	if ( temp>=201510 && temp <=201700 )
	{
		m_xmlb1.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
	}
	else if ( temp>=204510 && temp <=204700 )
	{
		m_xmlb2.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
	}
	else if ( temp>=203510 && temp <203700 )
	{
		m_xmlb5.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
		//20170414
		CFileFind FindDBFile;
		if (FindDBFile.FindFile("ZZIPChannel.dat"))
		{
			m_errorCodez.Format("%s",errorcode);
			AfxBeginThread(CDlgPlayWnd::QHThreadFuncc,this);
			logz.WriteLog(logid,"多画面切换!定点%s",m_errorCodez);
		}
	}
	else if ( temp>=206510 && temp <=206700 )
	{
		m_xmlb4.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
	}
	else if ( temp>=207510 && temp <=207700 )
	{
		m_xmlb3.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
	}
	else if (214990 ==temp)
	{
		m_xmlb7.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
	}
	else if (214990 ==temp )
	{
		m_xmlb6.SetXmlbColor(1);
		OnEPlaywnd2(errorcode);
	}
	if (201990 ==temp)//结束
	{
		m_xmlb1.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
	if (204990 ==temp)
	{
		m_xmlb2.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
	if (203990 ==temp)
	{
		m_xmlb5.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
	if (206990 ==temp)
	{
		m_xmlb4.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
	if (207990 ==temp)
	{
		m_xmlb3.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
	if (214990 ==temp)
	{
		m_xmlb7.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
	if (215990 ==temp)
	{
		m_xmlb6.SetXmlbColor(2);
		playwnd2.DvrLoginout();
	}
}

void CDlgPlayWnd::OnKsxmE100()
{
	m_xmlb1.SetXmlbColor(3);
	m_xmlb2.SetXmlbColor(3);
	m_xmlb3.SetXmlbColor(3);
	m_xmlb4.SetXmlbColor(3);
	m_xmlb5.SetXmlbColor(3);
	m_xmlb6.SetXmlbColor(3);
	m_xmlb7.SetXmlbColor(3);
}

void CDlgPlayWnd::OnEPlaywnd2(CString errocode)
{
	SearchTDH(errocode);
	HWND hwnd;
	hwnd=GetDlgItem(IDC_STATIC_WND2)->GetSafeHwnd();
	playwnd2.DvrLoginout();
	
	if (playwnd2.DvrInitLogin(hwnd,m_xmsbip.GetBuffer(0),m_xmsbuser.GetBuffer(0),m_xmsbpwd.GetBuffer(0),atoi(m_xmsbport)))
	{
		playwnd2.DvrRealPlay(atoi(m_xmsbtd));
		logz.WriteLog(logxmid,"登录设备成功!%s,%s_%s_%s_%s_%s",errocode,m_xmsbip,m_xmsbuser,m_xmsbpwd,m_xmsbport,m_xmsbtd);
		playwnd2.ISPlaywndd=TRUE;
	}	
	else
	{
		logz.WriteLog(logxmid,"登录失败!设备%s,%s_%s_%s_%s_%s",errocode,m_xmsbip,m_xmsbuser,m_xmsbpwd,m_xmsbport,m_xmsbtd);
	}
}

void CDlgPlayWnd::OnResult()
{
	int Result=100;
	int iItem=m_ListCtrlError.GetItemCount();
	CString temp;
	for (int i=0;i<iItem;i++)
	{
		temp=m_ListCtrlError.GetItemText(i,2);
		Result-=atoi(temp);
	}
	Result=Result<0?0:Result;
	if (Result < 80)//不合格
	{
		OnKsxmE100();
		playwnd2.DvrLoginout();
	}
	temp.Format("%d",Result);
	m_Stext2.SetCaptionSW(temp);
}

BOOL CDlgPlayWnd::OnInitSQLw()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConnwnd.CreateInstance("ADODB.Connection");
	if(m_pConnwnd==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		logz.WriteLog(logid,"初始化连接对象失败");
		return FALSE;
	}
	HRESULT res=-1;
	try
	{
		//建立数据库连接
		CString Sqlstr;
		CString m_strInEdit;
		GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
		m_strInEdit.ReleaseBuffer();
		if (!m_strInEdit.IsEmpty())
		{
			int iLen =m_strInEdit.GetLength();
			
			for(int i=0;i<iLen;i++)
			{
				m_strInEdit.SetAt(i,m_strInEdit[i]-1);
			}
			m_strInEdit.MakeReverse();
		}
		Sqlstr=m_strInEdit;
		res = m_pConnwnd->Open((_bstr_t)Sqlstr,"","",0);		
		if(res==S_OK)//0
		{
			logz.WriteLog(logid,"打开数据库成功!%d",m_WndNum);
		}
		else
		{
			logz.WriteLog(logid,"打开数据库失败!%d",m_WndNum);
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
void CDlgPlayWnd::OnRecord()
{
	if(m_IsTime)
	{
		m_IsTime = FALSE;
		CDC* pDeskDC =  this->GetDC();		//获取桌面画布对象
		CRect rc;
		GetClientRect(&rc);
		CDC  memDC;											//定义一个内存画布
		memDC.CreateCompatibleDC(pDeskDC);					//创建一个兼容的画布
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDeskDC,rc.Width(),rc.Height());	//创建兼容位图
		memDC.SelectObject(&bmp);							//选中位图对象	
		BITMAP bitmap;
		bmp.GetBitmap(&bitmap);
		memDC.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,pDeskDC,0,0,SRCCOPY);
		
		
		DWORD size=bitmap.bmWidthBytes*bitmap.bmHeight;
		
		BYTE* lpData = new BYTE[size];
		
		int panelsize  = 0;									//记录调色板大小
		if(bitmap.bmBitsPixel<16)							//判断是否为真彩色位图
			panelsize =(int) pow(2,bitmap.bmBitsPixel*sizeof(RGBQUAD));
		
		BITMAPINFOHEADER *pBInfo = new BITMAPINFOHEADER;
		pBInfo->biBitCount       = bitmap.bmBitsPixel;
		pBInfo->biClrImportant   = 0;
		pBInfo->biCompression    = 0;
		pBInfo->biHeight         = bitmap.bmHeight;
		pBInfo->biPlanes         = bitmap.bmPlanes;
		pBInfo->biSize           = sizeof(BITMAPINFOHEADER);
		pBInfo->biSizeImage      = bitmap.bmWidthBytes*bitmap.bmHeight;
		pBInfo->biWidth          = bitmap.bmWidth;
		pBInfo->biXPelsPerMeter  = 0;
		pBInfo->biYPelsPerMeter  = 0;
		BITMAPINFO bInfo;
		bInfo.bmiHeader = *pBInfo;
		GetDIBits(memDC.m_hDC,bmp,0,pBInfo->biHeight,lpData,&bInfo,DIB_RGB_COLORS);
		
		if(nFrames == 0)
		{
			CString rate;
			rate="5";
			AVIFileOpen(&pfile,avipath+aviname+".avi",OF_WRITE | OF_CREATE,NULL);
			memset(&strhdr, 0, sizeof(strhdr));
			strhdr.fccType    = streamtypeVIDEO;
			strhdr.fccHandler = 0;
			strhdr.dwScale    = 1;
			strhdr.dwRate     = atoi(rate);
			strhdr.dwSuggestedBufferSize = pBInfo->biSizeImage;
			SetRect(&strhdr.rcFrame,0,0,pBInfo->biWidth,pBInfo->biHeight);
			hr = AVIFileCreateStream(pfile,&ps,&strhdr); 
			
			opts[0]->fccType = streamtypeVIDEO;
			opts[0]->fccHandler = mmioStringToFOURCC("MSVC", 0);
			opts[0]->dwQuality = 8500;//7500
			opts[0]->dwBytesPerSecond = 0;
			opts[0]->dwFlags = AVICOMPRESSF_VALID || AVICOMPRESSF_KEYFRAMES;
			opts[0]->lpFormat = 0;
			opts[0]->cbFormat = 0;
			opts[0]->dwInterleaveEvery = 0;
			
			AVIMakeCompressedStream(&pComStream,ps,&pCompressOption,NULL); 
			AVIStreamSetFormat(pComStream,0,pBInfo,sizeof(BITMAPINFOHEADER));
		}
		
		
		hr = AVIStreamWrite(pComStream,nFrames ,1,(LPBYTE)lpData,
			pBInfo->biSizeImage,AVIIF_KEYFRAME,NULL,NULL);
		nFrames++;
		
		this->ReleaseDC(pDeskDC);
		delete []lpData;
		delete pBInfo ;	
		m_IsTime = TRUE;
	}
}

void CDlgPlayWnd::OnStartAvi()
{
//	return ;
	try
	{	
		if (isStartAvi)
		{
			OnStopAvi();		
		}
		//m_StuZKZ="1";
		// 	m_Stext1.SetCaptionSW("");
		// 	m_Stext3.SetCaptionSW("");
		// 	m_Stext2.SetCaptionSW("");
		// 	m_Stext4.SetCaptionSW("");	
		// 	OnShowPHO();
		//  m_ListCtrlError.DeleteAllItems();
		
		//aviname.Format("%s_%s_%d_%d",m_StuZKZ,m_StuName,iKscs,iDrcs);//准考证号+姓名+考试次数+当日次数
		aviname.Format("%s_%s_%d_%d",m_StuSFZM,m_StuName,iKscs,iDrcs);//身份证号+姓名+考试次数+当日次数
		AVIFileInit();
		SetTimer(VIDEOPLAY_TIME,200,NULL);
		isStartAvi=TRUE;
		CString temp;
		temp.Format("%s%s",avipath,aviname);
		playwnd1.DvrStartRecord(temp);
	}
	catch (...)
	{
		TRACE("OnStartAvi() catch\n");
	}
}

void CDlgPlayWnd::OnStopAvi()
{
//	return ;
	playwnd1.DvrStopRecord();
	isStartAvi=FALSE;
	KillTimer(VIDEOPLAY_TIME);
	AVIStreamClose(pComStream);
	AVIStreamClose(ps);
	if(pfile != NULL)
		AVIFileRelease(pfile);
	AVIFileExit();
	nFrames =0;
	UINT yasuo;
	yasuo=GetPrivateProfileInt("UIskin","yasuo",0,".\\config.ini");
	if ( 0==yasuo )
	{
 		CString temp; 		
		temp.Format("%s%s.avi",avipath,aviname);
 		ShellExecute(NULL,"open",".\\CMP4ToMP3.exe",temp,NULL,SW_HIDE); 
		logz.WriteLog(logid,"调用CMP4ToMP3.exe %s",temp);
	}

}
void CDlgPlayWnd::WriteFF(CString str)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d");
	FILE *fp=fopen(Data,"a+");
	fprintf(fp,"%s\n",str);
	fclose(fp);
}

BOOL CDlgPlayWnd::OnStuZT()
{
	CString temp,stuzt;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("select 状态 from StudentInfo where 准考证明编号='%s'",m_StuZKZ);
		_RecordsetPtr pSet =m_pConnwnd->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			
			vat =pSet->GetCollect("状态");
			if (vat.vt != VT_NULL)
			{
				stuzt = (LPCSTR)_bstr_t(vat);
			}else{stuzt =_T("0");}
			pSet->MoveNext();
			if (strcmp(stuzt,"1")==0 || strcmp(stuzt,"2")==0)
			{
				if (isStartAvi)
				{
					OnStopAvi();
// 					m_Stext1.SetCaptionSW("");
// 					m_Stext3.SetCaptionSW("");
// 					m_Stext2.SetCaptionSW("");
// 					m_Stext4.SetCaptionSW("");
// 					m_StuZKZ="1";
// 					OnShowPHO();
// 					m_ListCtrlError.DeleteAllItems();
				}
				playwnd2.DvrLoginout();
			}
		}//if
	}
	catch (_com_error &comError)
	{
		logz.WriteLog(logid,"OnStuZT Error %s:%s",comError.ErrorMessage(),(LPCTSTR)comError.Description());
		return FALSE;
	}
	return TRUE;
}

void CDlgPlayWnd::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if (theApp.zbmpmessagebox("退出提示,确定要退出程序么?"))
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
	}
}

//统计总考试次数和总人数  - IT.C
void CDlgPlayWnd::CountExam(CString strCarNum)
{
	CString temp;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("select count(*) as 总人数,SUM(当日次数) as 总次数 from StudentInfo where \
		(Select CONVERT(varchar(100), 预考日期, 23)) = (Select CONVERT(varchar(100), GETDATE(), 23))  \
		AND (状态='1'or 状态='2') AND 考车号='%s' ",strCarNum);
		_RecordsetPtr pSet =m_pConnwnd->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			
			vat =pSet->GetCollect("总人数");
			if (vat.vt != VT_NULL)
			{
				uksRs = atoi( (LPCSTR)_bstr_t(vat) );
			}
			else
			{
				uksRs  = 0;
			}

			vat =pSet->GetCollect("总次数");
			if (vat.vt != VT_NULL)
			{
				uksCS = atoi( (LPCSTR)_bstr_t(vat) );
			}
			else
			{
				uksCS  = 0;
			}
			pSet->MoveNext();		
		}
	}
	catch (_com_error &comError)
	{
		logz.WriteLog(logid,"统计总人数和考试次数 Error %s:%s",comError.ErrorMessage(),(LPCTSTR)comError.Description());
		return ;
	}
}

void CDlgPlayWnd::OnButton1() 
{
	// TODO: Add your control notification handler code here
	if (playwnd1.OpenVoice())
	{
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BUTTOND_MSG,m_WndNum,0);
	}
	else
	{	
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);	
	}	
}

void CDlgPlayWnd::OnButton2() 
{
	// TODO: Add your control notification handler code here
	if (playwnd1.OpenVoice())
	{		
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
		
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BUTTOND_MSG,m_WndNum,0);
// 		for (int i=1;i<5;i++)
// 		{
// 			if (PlayWnd[i]->m_WndNum != m_WndNum)
// 			{									
// 				PlayWnd[i]->OnCloseSpeak2();
// 			}			
// 		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);	
	}
}

void CDlgPlayWnd::OnVoiceIn() 
{
	// TODO: Add your control notification handler code here
	if ( playwnd1.VoiceIntercom() )
	{
		GetDlgItem(IDC_VoiceIn)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_VoiceClose)->ShowWindow(SW_HIDE);
// 		for (int i=1;i<5;i++)
// 		{
// 			if (PlayWnd[i]->m_WndNum != m_WndNum)
// 			{
// 				PlayWnd[i]->OnCloseVoiceCom();
// 			}			
// 		}
	} 
	else
	{
		GetDlgItem(IDC_VoiceIn)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VoiceClose)->ShowWindow(SW_SHOW);
	}
}

void CDlgPlayWnd::OnVoiceClose() 
{
	// TODO: Add your control notification handler code here
	if ( playwnd1.VoiceIntercom() )
	{
		GetDlgItem(IDC_VoiceIn)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_VoiceClose)->ShowWindow(SW_HIDE);
// 		for (int i=1;i<5;i++)
// 		{
// 			if (PlayWnd[i]->m_WndNum != m_WndNum)
// 			{
// 				PlayWnd[i]->OnCloseVoiceCom();
// 			}			
// 		}
	} 
	else
	{
		GetDlgItem(IDC_VoiceIn)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VoiceClose)->ShowWindow(SW_SHOW);
	}
}

void CDlgPlayWnd::OnBTNJudge() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgPlayWnd::OnCloseSpeak2()
{
	playwnd1.CloseVoice();
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
}

BOOL CDlgPlayWnd::OnCloseVoiceCom()
{
	playwnd1.CloseVoiceCom();
	GetDlgItem(IDC_VoiceIn)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_VoiceClose)->ShowWindow(SW_SHOW);
	return TRUE;
}

void CDlgPlayWnd::OnQHPlay2()
{
	CString temp,temp1,lpAppName;
	lpAppName.Format("Q%s",m_errorCodez);
	GetPrivateProfileString(lpAppName,"Login","",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp.ReleaseBuffer();
	int pos=temp.Replace(",",",");
	if (pos ==3)
	{
		pos =temp.Find(",");
		m_xmsbip =temp.Left(pos);
		temp1=temp.Mid(pos+1);
		pos =temp1.Find(",");
		m_xmsbuser=temp1.Left(pos);
		temp=temp1.Mid(pos+1);
		pos =temp.Find(",");
		m_xmsbpwd =temp.Left(pos);
		temp1=temp.Mid(pos+1);
		m_xmsbport=temp1;
	}
	else
	{
		m_xmsbip=_T("");
		m_xmsbuser=_T("");
		m_xmsbpwd=_T("");
	}

	GetPrivateProfileString(lpAppName,"Channel","",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp.ReleaseBuffer(MAX_PATH);
	m_xmsbtd.Format("%s",temp);

	logz.WriteLog(logid,"定点切换%s,%s,%s,%s,==%s",m_xmsbip,m_xmsbuser,m_xmsbpwd,m_xmsbtd,m_errorCodez);
	HWND hwnd;
	hwnd=GetDlgItem(IDC_STATIC_WND2)->GetSafeHwnd();
	playwnd2.DvrLoginout();
	if (playwnd2.DvrInitLogin(hwnd,m_xmsbip.GetBuffer(0),m_xmsbuser.GetBuffer(0),m_xmsbpwd.GetBuffer(0),atoi(m_xmsbport)))
	{		
		playwnd2.DvrRealPlay(atoi(m_xmsbtd));
		playwnd2.ISPlaywndd=TRUE;
	}
}

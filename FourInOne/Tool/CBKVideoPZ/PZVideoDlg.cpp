// PZVideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKVideoPZ.h"
#include "PZVideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPZVideoDlg dialog


CPZVideoDlg::CPZVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPZVideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPZVideoDlg)
	m_Path = _T("");
	m_Edit_BZ = _T("");
	m_EditPort = _T("8000");
	m_EditPwd = _T("12345");
	m_EditTdh = _T("");
	m_EditUser = _T("admin");
	//}}AFX_DATA_INIT
}


void CPZVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPZVideoDlg)
	DDX_Control(pDX, IDC_IPADDRESS2, m_MediaIPAddr);
	DDX_Control(pDX, IDC_COMBO_ML, m_ComboBoxML);
	DDX_Control(pDX, IDC_COMBO_SXNUM, m_ComboSXBH);
	DDX_Control(pDX, IDC_COMBO2, m_ComSkin);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BTNClose);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddrCtrl);
	DDX_Control(pDX, IDC_COMBO_BH, m_ComboBoxBH);
	DDX_Control(pDX, IDC_LISTBKVideo, m_ListConVideo);
	DDX_Text(pDX, IDC_EPATH, m_Path);
	DDX_Text(pDX, IDC_EDIT_BZ, m_Edit_BZ);
	DDX_Text(pDX, IDC_EDIT_PROT, m_EditPort);
	DDX_Text(pDX, IDC_EDIT_PWD, m_EditPwd);
	DDX_Text(pDX, IDC_EDIT_TDH, m_EditTdh);
	DDX_Text(pDX, IDC_EDIT_USER, m_EditUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPZVideoDlg, CDialog)
	//{{AFX_MSG_MAP(CPZVideoDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNCHANGE, OnBtnchange)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_BH, OnSelchangeComboBh)
	ON_BN_CLICKED(IDC_BTNSAVE, OnBtnsave)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTBKVideo, OnItemchangedLISTBKVideo)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeComboSkin)
	ON_BN_CLICKED(IDC_BTNSKINSET, OnBtnskinset)
	ON_BN_CLICKED(IDC_BTN_LMTIP, OnBtnLmtip)
	ON_BN_CLICKED(IDC_BTN_TYIPPWD, OnBtnTyippwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPZVideoDlg message handlers


BOOL CPZVideoDlg::OnInitSQLEX()
{
	//////////////////////////////////////////////////////////////////////////
//	AfxOleInit();
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
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
	try
	{
		//建立数据库连接
		CString Sqlstr;
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		zbmpmessagebox(errormessage.GetBuffer(0));
		ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

BOOL CPZVideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!OnInitSQLEX())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	m_ListConVideo.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListConVideo.InsertColumn(0,_T("编号"),LVCFMT_LEFT,60,-1);
	m_ListConVideo.InsertColumn(1,_T("设备IP"),LVCFMT_LEFT,100,-1);
	m_ListConVideo.InsertColumn(2,_T("用户名"),LVCFMT_LEFT,60,-1);
	m_ListConVideo.InsertColumn(3,_T("密码"),LVCFMT_LEFT,-10,-1);
	m_ListConVideo.InsertColumn(4,_T("通道"),LVCFMT_LEFT,40,-1);
	m_ListConVideo.InsertColumn(5,_T("端口"),LVCFMT_LEFT,40,-1);	
	m_ListConVideo.InsertColumn(6,_T("N"),LVCFMT_LEFT,30,-1);
	m_ListConVideo.InsertColumn(7,_T("码流"),LVCFMT_LEFT,40,-1);
	m_ListConVideo.InsertColumn(8,_T("流媒体"),LVCFMT_LEFT,90,-1);
	m_ListConVideo.InsertColumn(9,_T("备注"),LVCFMT_LEFT,140,-1);

	m_ComboSXBH.InsertString(0,"1");
	m_ComboSXBH.InsertString(1,"2");
	m_ComboSXBH.InsertString(2,"3");
	m_ComboSXBH.InsertString(3,"4");
	m_ComboSXBH.InsertString(4,"5");
	m_ComboSXBH.InsertString(5,"6");
	m_ComboSXBH.InsertString(6,"7");
	m_ComboSXBH.InsertString(7,"8");
	m_ComboSXBH.SetCurSel(0);

	m_ComboBoxML.InsertString(0,"主码流"); 
	m_ComboBoxML.InsertString(1,"子码流");
	m_ComboBoxML.SetCurSel(1);

	UpdateList();
	UpdateCombox();
	m_ComboBoxBH.SetCurSel(0);//

	GetDlgItem(IDC_BTN_EXIT)->MoveWindow(663,0,47,22);
	m_BTNClose.LoadBitmap(IDB_BMPCLOSE);

	
	hbr1=CreateSolidBrush(RGB(255,255,255)); 
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPZVideoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CPZVideoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,720,450,SWP_NOMOVE | SWP_NOOWNERZORDER );
	CRgn m_rgn;
	CRect rc;	
	GetClientRect(&rc);
	rc-=rc.TopLeft();
	m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);
	SetWindowRgn(m_rgn,TRUE);
}

void CPZVideoDlg::OnBtnchange() 
{
	// TODO: Add your control notification handler code here
	CString ReturnPach;								//字符串变量
	TCHAR szPath[_MAX_PATH];                       	//保存路径变量
	BROWSEINFO bi;                                  	// BROWSEINFO结构变量
	bi.hwndOwner    = NULL;                          	//HWND句柄
	bi.pidlRoot       = NULL;                          	//默认值为NULL
	bi.lpszTitle       = _T("文件浏览对话框");          	//对话框标题
	bi.pszDisplayName = szPath;                         //选择文件夹路径
	bi.ulFlags        = BIF_RETURNONLYFSDIRS;       //标记
	bi.lpfn           = NULL;                          	//默认值为NULL
	bi.lParam         = NULL;                          	//回调消息
	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);  	//显示文件浏览对话框
	if(pItemIDList)
	{
		if(SHGetPathFromIDList(pItemIDList,szPath)) 		//判断是否获得文件夹路径
			ReturnPach = szPath;					//获得文件夹路径
	}
	else
	{
		ReturnPach = "C:\\"; 							//文件夹路径为空
	}
	if ( ReturnPach.Right(1) != "\\" )
	{
		ReturnPach+="\\";
	}
	m_Path=ReturnPach;				//显示文件夹路径
	UpdateData(FALSE);
//////////////////////////////////////////////////////////////////////////
	ULARGE_INTEGER totalsize;
	ULARGE_INTEGER freesize;
	ULARGE_INTEGER availablesize;
	CString str,temp;
	str = ReturnPach.Left(3);
	::GetDiskFreeSpaceEx(str,&totalsize,&freesize,&availablesize);
	temp.Format("%ldM",totalsize.QuadPart/1024/1024);
	GetDlgItem(IDC_STATIC_SYKJ)->SetWindowText("剩余空间："+temp);
	if(totalsize.QuadPart/1024/1024 < 300)
	{
		MessageBox("剩余空间不足300M，请整理磁盘");
	}
	//////////////////////////////////////////////////////////////////////////
	m_Path.Replace("\\","\\\\");
	WritePrivateProfileString("UIskin","avipath",m_Path,".\\search.cfg");
}

BOOL CPZVideoDlg::UpdateList()
{	
	int k=0;
	CString tempk;
	m_ListConVideo.DeleteAllItems();//清空列表
	try
	{
			VARIANT cnt;
			cnt.vt = VT_INT;
			CString temp;
			temp.Format("SELECT * FROM TBKVideo ORDER BY 编号");
			_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
			_variant_t vat;
			CString strbh,strsbip,strport,struser,strpwd,strtdh,strbz,strnid,strml,strmedia;
			if(pSet != NULL)
			{
				while(!pSet->adoEOF)
				{
					vat =pSet->GetCollect("编号");
					if (vat.vt != VT_NULL)
					{
						strbh = (LPCSTR)_bstr_t(vat);
					}else{strbh=_T("");}
					vat =pSet->GetCollect("设备IP");
					if (vat.vt != VT_NULL)
					{
						strsbip = (LPCSTR)_bstr_t(vat);
					}else{strsbip=_T("");}
					vat =pSet->GetCollect("端口号");
					if (vat.vt != VT_NULL)
					{
						strport = (LPCSTR)_bstr_t(vat);
					}else{strport=_T("");}
					vat =pSet->GetCollect("用户名");
					if (vat.vt != VT_NULL)
					{
						struser = (LPCSTR)_bstr_t(vat);
					}else{struser=_T("");}
					vat =pSet->GetCollect("密码");
					if (vat.vt != VT_NULL)
					{
						strpwd = (LPCSTR)_bstr_t(vat);
					}else{strpwd=_T("");}
					vat =pSet->GetCollect("通道号");
					if (vat.vt != VT_NULL)
					{
						strtdh = (LPCSTR)_bstr_t(vat);
					}else{strtdh=_T("");}
					vat =pSet->GetCollect("备注");
					if (vat.vt != VT_NULL)
					{
						strbz = (LPCSTR)_bstr_t(vat);
					}else{strbz=_T("");}
					vat =pSet->GetCollect("Nid");
					if (vat.vt != VT_NULL)
					{
						strnid = (LPCSTR)_bstr_t(vat);
					}else{strnid=_T("");}
					vat =pSet->GetCollect("MediaIP");
					if (vat.vt != VT_NULL)
					{
						strmedia = (LPCSTR)_bstr_t(vat);
					}else{strmedia=_T("");}
					vat =pSet->GetCollect("TransMode");
					if (vat.vt != VT_NULL)
					{
						strml = (LPCSTR)_bstr_t(vat);
					}else{strml=_T("");}
					//tempk.Format("%d",k+1);
					m_ListConVideo.InsertItem(k,strbh);
					m_ListConVideo.SetItemText(k,1,strsbip);
					m_ListConVideo.SetItemText(k,2,struser);
					m_ListConVideo.SetItemText(k,3,strpwd);
					m_ListConVideo.SetItemText(k,4,strtdh);
					m_ListConVideo.SetItemText(k,5,strport);
					m_ListConVideo.SetItemText(k,6,strnid);
					m_ListConVideo.SetItemText(k,7,strml);
					m_ListConVideo.SetItemText(k,8,strmedia);
					m_ListConVideo.SetItemText(k,9,strbz);
					k++;
					pSet->MoveNext();
				}//while
			}//if
			else
			{				
				TRACE("查询到记录为空\n");
				return FALSE;
			}
	}
	catch (_com_error &comError)
	{
		//TRACE("打开数据库异常：%s!\n",comError.ErrorMessage());
 		tempk.Format("错误提示,更新通道对应列表失败!,错误[%s]",comError.Description());
		zbmpmessagebox(tempk.GetBuffer(0));
	}
	return TRUE;
}

void CPZVideoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CPZVideoDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	CPZVideoDlg::OnCancel();
}

BOOL CPZVideoDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAP_BACK); 
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,720,450,SRCCOPY); 
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CPZVideoDlg::UpdateCombox()
{
	int k=0;
	CString tempk;
	//	m_ComboBoxBH.ResetContent();
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		CString temp;
		temp.Format("select 错误编号,扣分类型 from ErrorData where 扣除分数='0'");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		CString errorcode,kfls;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("错误编号");
				if (vat.vt != VT_NULL)
				{
					errorcode = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("扣分类型");
				if (vat.vt != VT_NULL)
				{
					kfls = (LPCSTR)_bstr_t(vat);
				}
				//tempk.Format("%d",k+1);
				//m_ListConVideo.InsertItem(k,strbh);
				m_ComboBoxBH.InsertString(k,errorcode);
				ComboboxArray.Add(kfls);
				k++;
				pSet->MoveNext();
			}//while
		}//if
		else
		{
			
			TRACE("查询到记录为空\n");
			return FALSE;
		}
		pSet->Close();
		temp.Format("select 项目 from SysCfg where 项目 like '考车%%'");
		pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("项目");
				if (vat.vt != VT_NULL)
				{
					errorcode = (LPCSTR)_bstr_t(vat);
				}
				m_ComboBoxBH.InsertString(k,errorcode);
				ComboboxArray.Add(errorcode);
				k++;
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		//TRACE("打开数据库异常：%s!\n",comError.ErrorMessage());
		tempk.Format("错误提示,更新Combox列表失败!,%s",comError.Description());
		zbmpmessagebox(tempk.GetBuffer(0));
	}
	return TRUE;
}


void CPZVideoDlg::OnSelchangeComboBh() 
{
	// TODO: Add your control notification handler code here
	m_Edit_BZ.Format("%s",ComboboxArray.GetAt(m_ComboBoxBH.GetCurSel()));
	UpdateData(FALSE);
}

void CPZVideoDlg::OnBtnsave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	AddTBKVideo();
	UpdateList();
}

void CPZVideoDlg::AddTBKVideo()
{

	//////////////////////////////////////////////////////////////////////////
	_RecordsetPtr pRecordset;
	_variant_t vtemp;
	CString temp,temp1,m_editIP,m_mediaip;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		int Count=m_ComboBoxBH.GetCurSel();
		int isxbh =m_ComboSXBH.GetCurSel()+1;
		int imllx =m_ComboBoxML.GetCurSel();

		int n=m_ComboBoxBH.GetLBTextLen(Count);
		m_ComboBoxBH.GetLBText(Count,temp.GetBuffer(n));
		//////////////////////////////////////////////////////////////////////////
		BYTE nField0,nField1,nField2,nField3;
		m_IPAddrCtrl.GetAddress(nField0,nField1,nField2,nField3);
		m_editIP.Format("%u.%u.%u.%u",nField0,nField1,nField2,nField3);
		m_MediaIPAddr.GetAddress(nField0,nField1,nField2,nField3);
		m_mediaip.Format("%u.%u.%u.%u",nField0,nField1,nField2,nField3);
		if (m_mediaip=="0.0.0.0")
		{
			m_mediaip=_T("");
		}
		strSQL.Format("select * from TBKVideo where 编号 = '%s' and Nid=%d ", temp,isxbh);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		if(pRecordset->RecordCount == 1)
		{
			temp1.Format("修改提示,数据库中已存在编号[%s],是否要更改信息!",temp);
			if (zbmpmessagebox(temp1.GetBuffer(0)))
			{
				pRecordset->PutCollect("编号",(_variant_t)temp);
				pRecordset->PutCollect("设备IP",(_variant_t)m_editIP);				
				pRecordset->PutCollect("用户名",(_variant_t)m_EditUser);
				pRecordset->PutCollect("密码",(_variant_t)m_EditPwd);
				pRecordset->PutCollect("通道号",(_variant_t)m_EditTdh);
				pRecordset->PutCollect("端口号",(_variant_t)m_EditPort);
				pRecordset->PutCollect("备注",(_variant_t)m_Edit_BZ);
				pRecordset->PutCollect("Nid",_variant_t((long)isxbh));
				pRecordset->PutCollect("TransMode",_variant_t((long)imllx));
				pRecordset->PutCollect("MediaIP",(_variant_t)m_mediaip);
				pRecordset->Update();
			}
		}
		else if(pRecordset->RecordCount == 0)
		{
			temp1.Format("新增提示,是否新增编号[%s]信息!",temp);
			if (zbmpmessagebox(temp1.GetBuffer(0)))
			{
				pRecordset->AddNew();
				pRecordset->PutCollect("编号",(_variant_t)temp);
				pRecordset->PutCollect("设备IP",(_variant_t)m_editIP);				
				pRecordset->PutCollect("用户名",(_variant_t)m_EditUser);
				pRecordset->PutCollect("密码",(_variant_t)m_EditPwd);
				pRecordset->PutCollect("通道号",(_variant_t)m_EditTdh);
				pRecordset->PutCollect("端口号",(_variant_t)m_EditPort);
				pRecordset->PutCollect("备注",(_variant_t)m_Edit_BZ);
				pRecordset->PutCollect("Nid",_variant_t((long)isxbh));
				pRecordset->PutCollect("TransMode",_variant_t((long)imllx));
				pRecordset->PutCollect("MediaIP",(_variant_t)m_mediaip);
				pRecordset->Update();
			}
		}
		else
		{
			zbmpmessagebox("提示,此编号在数据库中存在不止1条记录!");
		}
		
		pRecordset->Close();
		pRecordset.Release();		
	}
	catch (_com_error e) 
	{
		temp1.Format("错误提示,添加通道信息失败!!,%s",e.Description());
		zbmpmessagebox(temp1.GetBuffer(0));
	}	
}

HBRUSH CPZVideoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor ==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(RGB(82,82,82));
		return   hbr1; 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPZVideoDlg::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	CString videobh;
	int Count=m_ComboBoxBH.GetCurSel();
	int n=m_ComboBoxBH.GetLBTextLen(Count);
	m_ComboBoxBH.GetLBText(Count,videobh.GetBuffer(n));	
	int isxbh =m_ComboSXBH.GetCurSel()+1;
	temp.Format("删除提示,是否要删除编号[%s],摄像头编号[%d]",videobh,isxbh);
	if (zbmpmessagebox(temp.GetBuffer(0)))
	{
		try
		{
			_RecordsetPtr pRecordset;
			CString strSQL;	
			_variant_t vTemp;

			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;				
			if (isxbh==0)
			{
				strSQL.Format("select * from TBKVideo where 编号 = '%s' and Nid is null ", videobh);
			}
			else
			{
				strSQL.Format("select * from TBKVideo where 编号 = '%s' and Nid=%d ", videobh,isxbh);
			}
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			if(pRecordset->RecordCount == 0)
			{
				zbmpmessagebox("删除提示,数据库中无该记录!");
				pRecordset->Close();
				pRecordset.Release();
				return;
			}
			else
			{
				pRecordset->Delete(adAffectCurrent);		
				pRecordset->UpdateBatch(adAffectAll);
			}		
			
			pRecordset->Close();
			pRecordset.Release();
			temp.Format("提示,删除编号[%s]成功!,摄像头编号[%d]",videobh,isxbh);
			zbmpmessagebox(temp.GetBuffer(0));
			UpdateList();
		}
		catch (_com_error e) 
		{
			
			temp.Format("错误提示,删除编号失败!,%s",e.Description());
			zbmpmessagebox(temp.GetBuffer(0));
		}
	}//if
}

void CPZVideoDlg::OnItemchangedLISTBKVideo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	CString temp;
	temp=m_ListConVideo.GetItemText(pNMListView->iItem,0);
	int nIndex = 0;
	nIndex=m_ComboBoxBH.FindString(nIndex,temp);
	m_ComboBoxBH.SetCurSel(nIndex);	//编号
 	m_IPAddrCtrl.SetWindowText(m_ListConVideo.GetItemText(pNMListView->iItem,1));//IP
 	m_EditUser=m_ListConVideo.GetItemText(pNMListView->iItem,2);//用户
 	m_EditPwd=m_ListConVideo.GetItemText(pNMListView->iItem,3);//密码
 	m_EditTdh=m_ListConVideo.GetItemText(pNMListView->iItem,4);//通道
	m_EditPort=m_ListConVideo.GetItemText(pNMListView->iItem,5);//端口

	temp=m_ListConVideo.GetItemText(pNMListView->iItem,6);
	m_ComboSXBH.SetCurSel(atoi(temp)-1);
	temp=m_ListConVideo.GetItemText(pNMListView->iItem,7);
	m_ComboBoxML.SetCurSel(atoi(temp));
	m_MediaIPAddr.SetWindowText(m_ListConVideo.GetItemText(pNMListView->iItem,8));//流媒体
 	m_Edit_BZ=m_ListConVideo.GetItemText(pNMListView->iItem,9);//备注



	
	

	UpdateData(FALSE);
	*pResult = 0;
}

void CPZVideoDlg::OnSelchangeComboSkin() 
{
	// TODO: Add your control notification handler code here
	int changecombo=m_ComSkin.GetCurSel();
	changecombo=changecombo%3;
	CRect rect,rc; 
	CDC			m_dcBuf;
	CBitmap		m_bmpTmp;
	CDC *pDC =this->GetDC();
	m_dcBuf.CreateCompatibleDC(pDC);
	m_bmpTmp.CreateCompatibleBitmap(pDC,720,450);
 	m_dcBuf.SelectObject(m_bmpTmp);
	this->ReleaseDC(pDC);

	rect.SetRect(0,0,720,450);

	GetDlgItem(IDC_STATIC_SKIN)->GetClientRect(&rc);
	GetDlgItem(IDC_STATIC_SKIN)->GetDC()->BitBlt(0,0,rc.Width(),rc.Height(),&m_dcBuf,0,0,SRCCOPY);

}

void CPZVideoDlg::OnBtnskinset() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	int changecombo=m_ComSkin.GetCurSel();
	temp.Format("修改提示,是否修改皮肤为[皮肤%d]",changecombo+1);
	if (zbmpmessagebox(temp.GetBuffer(0)))
	{
		changecombo =changecombo %3;
		temp.Format("%d",changecombo);
		WritePrivateProfileString("UIskin","nskin",temp,".\\search.cfg");
	}
}

BOOL CPZVideoDlg::zbmpmessagebox(LPTSTR msg)
{
	CBmpDlg msgdlg;
	msgdlg.dlgMessage=msg;
	int nResponse = msgdlg.DoModal();
	if (nResponse == IDOK)
	{
		return TRUE;
	}
	else if (nResponse == IDCANCEL)
	{
		return FALSE;
	}
	return TRUE;
}

void CPZVideoDlg::OnBtnLmtip() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("update tbkvideo set MediaIP=NULL");
		m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		AfxMessageBox("修改成功!");
		UpdateList();
	}
	catch (_com_error e) 
	{
		temp.Format("修改失败!%s",e.Description());
		AfxMessageBox(temp);				
	}
}

void CPZVideoDlg::OnBtnTyippwd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("update tbkvideo set 用户名='%s',密码='%s' ",m_EditUser,m_EditPwd);
		m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		AfxMessageBox("修改成功!");
		UpdateList();
	}
	catch (_com_error e) 
	{
		temp.Format("修改失败!%s",e.Description());
		AfxMessageBox(temp);				
	}
}

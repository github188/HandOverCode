// DlgStuMsg.cpp : implementation file
//

#include "stdafx.h"
#include "cfourpic.h"
#include "DlgStuMsg.h"
#include "StuEndExam.h"
#include "CmdStoredProcz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStuMsg dialog


CDlgStuMsg::CDlgStuMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStuMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStuMsg)
	m_strZkzm= _T("");
	//}}AFX_DATA_INIT
}


void CDlgStuMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStuMsg)
	DDX_Control(pDX, IDC_BTN_RGPP, m_BntRgpp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStuMsg, CDialog)
	//{{AFX_MSG_MAP(CDlgStuMsg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_SOCKZ,OnSock)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RGPP, OnBtnRgpp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStuMsg message handlers

void CDlgStuMsg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,CliRcMin.x,CliRcMin.y,SWP_NOMOVE | SWP_NOOWNERZORDER );
}
void CDlgStuMsg::SetCliRcSize(int x, int y,int kch)
{
	CliRcMin.x=x;
	CliRcMin.y=y;
	m_ikch=kch;
}

HBRUSH CDlgStuMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	//|| pWnd->GetDlgCtrlID()==IDC_STATIC_TXT1 || pWnd->GetDlgCtrlID()==IDC_STATIC_TXT2 \
//	|| pWnd->GetDlgCtrlID()==IDC_STATIC_TXT3 || pWnd->GetDlgCtrlID()==IDC_STATIC_TXT4 \
//		|| pWnd->GetDlgCtrlID()==IDC_STATIC_TXT5 || pWnd->GetDlgCtrlID()==IDC_STATIC_TXT6
	if (nCtlColor==CTLCOLOR_DLG )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return hbr1;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgStuMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	hbr1=CreateSolidBrush(RGB(17,20,26));//CreateSolidBrush(RGB(30,76,100));
	int txtW,txtH;
	txtW=CliRcMin.x-30;
	txtH=CliRcMin.y/9;
	int i;
	for (i=0;i<8;i++)
	{
		GetDlgItem(IDC_STATIC_TXT1+i)->MoveWindow(15,i*txtH,txtW,txtH);
		m_text[i].SubclassDlgItem(IDC_STATIC_TXT1+i,this);
		m_text[i].SetTextColor(RGB(255,255,255));
	}
	GetDlgItem(IDC_BTN_RGPP)->MoveWindow(15,CliRcMin.y-40, 105, 35);
	m_BntRgpp.LoadBitmap(IDB_BMP_RGPP);
	m_errLine=0;
	CString temp;
	temp.Format("考车 %d         成绩:",m_ikch);
	m_text[0].SetCaptionSW(temp);
	m_text[1].SetCaptionSW("考生姓名:   当日/考试次数:");
	m_text[2].SetCaptionSW("准考证号:");
	m_text[3].SetCaptionSW("档位:     车速:");	
	m_text[4].SetCaptionSW("状态:");
	m_text[5].SetCaptionSW("扣分1:");
	m_text[6].SetCaptionSW("扣分2:");
	m_text[7].SetCaptionSW("扣分3:");

	m_text[4].SetTextColor(RGB(255,188,36));
	m_text[5].SetTextColor(RGB(239,69,61));
	m_text[6].SetTextColor(RGB(239,69,61));
	m_text[7].SetTextColor(RGB(239,69,61));

	UINT uPort=5386+m_ikch;
	if (!mysockz.InitSock(m_hWnd,uPort))
	{
		temp.Format("套接字服务初始化失败!端口%d",uPort);
		m_text[1].SetCaptionSW(temp);
// 		CDialog::OnCancel();
// 		return FALSE;
	}
	if (!OnInitSQL())
	{
		m_text[2].SetCaptionSW("连接数据库失败!");
		//CDialog::OnCancel();
		//return FALSE;
	}
	m_TRun=FALSE;
	SetTimer(1,3000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStuMsg::OnSock(WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
	case FD_READ:
		OnReadSock();
		break;
	}
}
void CDlgStuMsg::OnReadSock()
{
	CString strTemp;
	strTemp=mysockz.RecvFromSock();
	int pos=0;
	int tempkch=0;
	CString temp,temp1,temp2;
	if (strTemp.Replace(",",",")==3)
	{
		pos=strTemp.Find(",");	
		if (pos >0)
		{
			strTemp=strTemp.Mid(pos+1);
			pos=strTemp.Find(",");	
			tempkch=atoi(strTemp.Left(pos));
			if (tempkch ==m_ikch)
			{
				temp=strTemp.Mid(pos+1);
				pos=temp.Find(",");
				temp2=temp.Left(pos);
				temp1=temp.Mid(pos+1);
				strTemp.Format("档位:%s   车速:%s km/h",temp1,temp2);
				m_text[3].SetCaptionSW(strTemp);
				//UpdateData(FALSE);
			}		
		}
	}
	
}

void CDlgStuMsg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	mysockz.CloseSocket();

	CDialog::OnClose();
}

BOOL CDlgStuMsg::OnInitSQL()
{
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
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CString Sqlstr;
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		TRACE(Sqlstr);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
			WriteLog("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			WriteLog("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}


void CDlgStuMsg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (FALSE==m_TRun)
	{
		m_TRun=TRUE;
		OnStu();
		GetErrorRecords();
		m_TRun=FALSE;
	}

	
	CDialog::OnTimer(nIDEvent);
}

void CDlgStuMsg::OnStu()
{
	CString sqltemp;
	try
	{
		CString strdrcs,strkscs,strzkzm;
		CString temp;
		VARIANT cnt;
		cnt.vt = VT_INT;		
		sqltemp.Format("SELECT 准考证明编号,姓名,考试次数,当日次数 FROM StudentInfo WHERE 考车号='考车%d' \
			AND 状态='3' AND (Select CONVERT(varchar(100), 预考日期, 23)) = (Select CONVERT(varchar(100), GETDATE(), 23)) ",m_ikch);

		TRACE(sqltemp+"\n");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		int ecodei=0;
		if(pSet != NULL )
		{
			while(!pSet->adoEOF)
			{
				
				vat =pSet->GetCollect("姓名");
				if (vat.vt != VT_NULL)
				{
					temp.Format("考生姓名:%s",(LPCSTR)_bstr_t(vat));
					if (temp!=m_StuName)
					{
						m_StuName.Format("%s",temp);
						temp.Format("%s 当日/考试次数:%s/%s",m_StuName,strdrcs,strkscs);
						m_text[1].SetCaptionSW(temp);
						//m_text[1].SetCaptionSW(m_StuName);
					}					
				}
				vat =pSet->GetCollect("考试次数");
				if (vat.vt != VT_NULL)
				{
					strkscs=(LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("当日次数");
				if (vat.vt != VT_NULL)
				{
					strdrcs=(LPCSTR)_bstr_t(vat);
					if (strdrcs!=m_drcs)
					{
						m_drcs.Format("%s",strdrcs);
						temp.Format("%s 当日/考试次数:%s/%s",m_StuName,strdrcs,strkscs);
						m_text[1].SetCaptionSW(temp);

						m_text[5].SetCaptionSW("扣分1:");
						m_text[6].SetCaptionSW("扣分2:");
						m_text[7].SetCaptionSW("扣分3:");
						WriteLog("新当日次数%s",m_drcs);
					}
				}
				vat =pSet->GetCollect("准考证明编号");
				if (vat.vt != VT_NULL)
				{
					strzkzm=(LPCSTR)_bstr_t(vat);
					if (strzkzm!=m_strZkzm)
					{					
						m_strZkzm.Format("%s",strzkzm);
						m_text[2].SetCaptionSW("准考证号:"+m_strZkzm);
						m_Ejlbh=0;
						m_text[5].SetCaptionSW("扣分1:");
						m_text[6].SetCaptionSW("扣分2:");
						m_text[7].SetCaptionSW("扣分3:");
						WriteLog("新准考证明编号%s",m_strZkzm);
					}					
				}
				temp.Format("SELECT 100-SUM(扣除分数) AS 成绩 FROM ErrorRecords JOIN ErrorData ON  ErrorRecords.错误编号=ErrorData.错误编号 \
					WHERE ErrorRecords.准考证明编号='%s' and 考试次数='%s' and 当日次数='%s' ",strzkzm,strkscs,strdrcs);
				OnStuCJ(temp);
				pSet->MoveNext();
			}		
			pSet->Close();
			pSet.Release();
		}
	}
	catch (_com_error &comError)
	{
		WriteLog("OnStu Error ! %s,%s",comError.Description(),comError.ErrorMessage());
	}
//	UpdateData(FALSE);
	
}

void CDlgStuMsg::OnStuCJ(CString sqlstr)
{
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		TRACE(sqlstr+"\n");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqlstr,&cnt,adCmdUnknown);
		_variant_t vat;
		int ecodei=0;
		if(pSet != NULL )
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("成绩");
				if (vat.vt != VT_NULL)
				{
//					m_Txt4.Format("实时成绩:%s",(LPCSTR)_bstr_t(vat));
					CString temp,temp1;
					temp1.Format("%s",(LPCSTR)_bstr_t(vat));
					int ikscj=atoi(temp1);
					if (ikscj<0)
					{
						ikscj=0;
					}
					temp.Format("考车 %d         成绩:%d",m_ikch,ikscj);
					if (temp!=m_Strkscj)
					{
						m_Strkscj.Format("%s",temp);
						m_text[0].SetCaptionSW(m_Strkscj);
					}					
				}			
				pSet->MoveNext();
			}
			pSet->Close();
			pSet.Release();			
		}
	}
	catch (_com_error &comError)
	{
		WriteLog("OnStu Error ! %s,%s",comError.Description(),comError.ErrorMessage());
	}
}

BOOL CDlgStuMsg::PreTranslateMessage(MSG* pMsg) 
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
			if (MessageBox("是否退出程序!","提示",MB_OKCANCEL) == IDOK)
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

void CDlgStuMsg::OnBtnRgpp() 
{
	// TODO: Add your control notification handler code here
	if (!m_strZkzm.IsEmpty())
	{
		CStuEndExam  StuEndExamDlg;
		StuEndExamDlg.sEndExamCard=m_strZkzm;
		
		StuEndExamDlg.EndType=0;
		StuEndExamDlg.DoModal();	
	}
	else
	{
		MessageBox("无考试中考生!不能进行人工评判!");
	}
}

void CDlgStuMsg::GetErrorRecords()
{
	if (m_strZkzm.IsEmpty())
	{
		return ;
	}
	CCmdStoredProcz AddTercmd(m_pConn);
	AddTercmd.SetCommandText("pr_ErrorRecords2");
	AddTercmd.SetParameters("@ZKZM_Number",m_strZkzm);
	AddTercmd.SetParameters("@Ejlbh",m_Ejlbh);
	
	int cont=AddTercmd.CmdExecute();
	if (cont ==-1)
	{
		return ;
	}
	int reerrori=AddTercmd.GetErrorCode("@intErrorCode");
	if (reerrori==3)
	{
		CString strKFLX,strTime,strKCFS,strtemp;
		AddTercmd.GetCollect("记录编号",m_Ejlbh);
		AddTercmd.GetCollect("扣分类型",strKFLX);
		AddTercmd.GetCollect("出错时间",strTime);
		AddTercmd.GetCollect("扣除分数",strKCFS);
		AddTercmd.GetCollect("ELINE",m_errLine);
		if (m_errLine>=0 && m_errLine <3 )
		{
			strtemp.Format("%s 扣%s分 %s",strTime,strKCFS,strKFLX);
			m_text[4].SetCaptionSW("状态:"+strKFLX);
			m_text[5+m_errLine].SetCaptionSW(strtemp);
		}		
	}
	else
	{
		CString strKFLX;
		AddTercmd.GetCollect("记录编号",m_Ejlbh);
		AddTercmd.GetCollect("扣分类型",strKFLX);
		m_text[4].SetCaptionSW("状态:"+strKFLX);
	}
	AddTercmd.RecordSetRelease();
	AddTercmd.CmdRelease();
}

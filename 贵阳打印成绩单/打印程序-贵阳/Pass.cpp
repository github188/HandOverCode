// Pass.cpp : implementation file
//

#include "stdafx.h"
#include "autoprintmanager.h"
#include "Pass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPass dialog


CPass::CPass(CWnd* pParent /*=NULL*/)
	: CDialog(CPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPass)
	m_Pass = _T("");
	//}}AFX_DATA_INIT
}


void CPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPass)
	DDX_Text(pDX, IDC_EDIT1, m_Pass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPass, CDialog)
	//{{AFX_MSG_MAP(CPass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPass message handlers

void CPass::UpdatePass()
{
	int k=0;
	CString tempk;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		CString temp;
		temp.Format("SELECT * FROM PassW");
		_RecordsetPtr pSet =m_pConnDB->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("密码");
				if (vat.vt != VT_NULL)
				{
					strpass = (LPCSTR)_bstr_t(vat);
				}
				k++;
				pSet->MoveNext();
			}//while
			pSet->Close();
			pSet.Release();
		}//if
		
	}
	catch (_com_error &comError)
	{
		//TRACE("打开数据库异常：%s!\n",comError.ErrorMessage());
		tempk.Format("错误提示,更新驾校列表失败!,%s",comError.Description());
	}
	UpdateData(TRUE);
}


BOOL CPass::CreatDBCon()
{
	m_pConnDB.CreateInstance("ADODB.Connection");
	if(m_pConnDB==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	try
	{
		//建立数据库连接
		res = m_pConnDB->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=kc.mdb;User ID='admin';Password=;Jet OLEDB:Database Password='ustbzy'","","",adModeUnknown);		
		//res = m_pConnDB->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=kc.mdb ","","",adModeUnknown);		
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
		errormessage.Format("连接mdb数据库失败!\r\n错误信息:%s",e.ErrorMessage());
		AfxMessageBox(errormessage);
		return FALSE;
	}	
	return TRUE;
}




BOOL CPass::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!CreatDBCon())
	{
		CDialog::OnCancel();
	}
	
	UpdatePass();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPass::PreTranslateMessage(MSG* pMsg) 
{
	
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE) 
	{
		return TRUE;
	}
	if (pMsg->wParam == VK_RETURN) 
	{
		UpdateData(TRUE);
		//MessageBox(m_Pass);
		if (strcmp(m_Pass,strpass)!=0)
		{
			CDialog::OnCancel();
		}else
		{
			CDialog::OnOK();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

// ADO.cpp: implementation of the ADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADO.h"
#include <afxdisp.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern 

ADO::ADO()
{

}

ADO::~ADO()
{

}

void ADO::OnInitADOConn(int SQLorOracle)
{

	::CoInitialize(NULL);
	 CString m_strInEdit;
	try
	{
       m_pConnection.CreateInstance("ADODB.Connection");  //创建连接对象实例
	   ////////////////////////////////照片数据库//////////////////////////////////////////
	  
	   GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\ConfigPhoto.ini");
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
	   /*
	   CString strConnect;
		if (SQLorOracle==1)
		{
			strConnect.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",m_IP,m_DataBase,m_Name,m_Pwd);
		}else
		{
			strConnect.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=bkzy)));User ID=sapho;Password=%s",m_IP,m_Pwd);
		}
		*/
	   m_pConnection->Open((_bstr_t)m_strInEdit,"","",adModeUnknown); //打开数据库
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接2ndPhoto照片数据库失败!,错误信息:%s,请先配置照片数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		//AfxMessageBox(m_strInEdit);
		if (SQLorOracle==1)
		{
			ShellExecute(NULL,"open",".\\sqlLink.exe",".\\ConfigPhoto.ini",NULL,SW_SHOW);//调用SQL工具
		}
		else
		{
			ShellExecute(NULL,"open",".\\oracle.exe",".\\ConfigPhoto.ini",NULL,SW_SHOW);//调用Oracle工具
		}
	}


}

_RecordsetPtr&  ADO::OpenRecordset(CString sql)
{
	ASSERT(!sql.IsEmpty());										//SQL语句不能为空
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建记录集对象实例
		m_pRecordset->Open(_bstr_t(sql), m_pConnection.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText); 	//执行SQL得到记录集
	}
	catch(_com_error e)											//捕获可能的异常
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}

void ADO::CloseRecordset()
{ 
	if(m_pRecordset->GetState() == adStateOpen)	//判断当前的记录集状态
		m_pRecordset->Close();					//关闭记录集
}

void ADO::CloseConn()
{
	m_pConnection->Close();									//关闭数据库连接
	::CoUninitialize();										//释放COM环境
}

UINT ADO::GetRecordCount(_RecordsetPtr pRecordset)
{
	int nCount = 0;											//声明保存记录数的变量
	try{
		pRecordset->MoveFirst();								//将记录集指针移动到第一条记录
	}
	catch(...)												//捕捉可能出现的错误
	{
		return 0;											//产生错误时返回0
	}
	if(pRecordset->adoEOF)									//判断记录集中是否没有记录
		return 0;											//无记录时返回0
	while (!pRecordset->adoEOF)								//当记录集指针没有指向最后时
	{
		pRecordset->MoveNext();								//将记录集指针移动到下一条记录
		nCount = nCount + 1;								//记录个数的变量加1
	}
	pRecordset->MoveFirst();									//将记录集指针移动到第一条记录
	return nCount;											//返回记录数
}



CString ADO::GetStrFromRecordset(char * str)
{
	 		  _variant_t var;
			  CString strPosInfo;
		      var= m_pRecordset->GetCollect(str);          //可能为空
			  if(var.vt != VT_NULL)   //为NULL
               strPosInfo = (char*)(_bstr_t)var;
	 	      else
 			  strPosInfo = "";

			  return strPosInfo;
}

int ADO::GetIntFromRecordset(char *str)
{
	_variant_t var;
 	int  iPosInfo=0;
	var= m_pRecordset->GetCollect(str);          //可能为空
	if(var.vt != VT_NULL)   //为NULL
		iPosInfo =atoi((char*)(_bstr_t)var);         //
	else 
		iPosInfo =0;

  
	
	return iPosInfo;
}

CString ADO::GetDateTime(char * str,char * sDateFormat)
{
        if(sDateFormat==NULL)
		{
			sDateFormat="%Y-%m-%d";
		}
	    CString strTimeGet;  
        _variant_t varTimeGet;  
        COleDateTime oleTimeGet;    
      
        varTimeGet=m_pRecordset->GetCollect(str);  
        if (varTimeGet.vt!=VT_NULL)  
        {  

            oleTimeGet=(COleDateTime)varTimeGet;  
            strTimeGet=oleTimeGet.Format(sDateFormat);
           // AfxMessageBox(strTimeGet);  
        }  
		else
        {
			strTimeGet="";
		}
	
		
		return strTimeGet;

}

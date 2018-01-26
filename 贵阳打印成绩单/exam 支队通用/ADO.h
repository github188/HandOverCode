// ADO.h: interface for the ADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADO_H__E0F8432E_F797_4979_8FB0_EAC60CA6C7F9__INCLUDED_)
#define AFX_ADO_H__E0F8432E_F797_4979_8FB0_EAC60CA6C7F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ADO  
{
public:

    _ConnectionPtr m_pConnection;					//连接对象指针
	_RecordsetPtr  m_pRecordset;					//记录集对象指针

public:
    CString GetDateTime(char * str,char * sDateFormat=NULL);
	int GetIntFromRecordset(char * str);
	CString GetStrFromRecordset(char * str );
	ADO();
	virtual ~ADO();
	void OnInitADOConn(CString m_IP,CString m_DataBase,CString m_Name,CString m_Pwd,int SQLorOracle);							//连接数据库
	_RecordsetPtr&  OpenRecordset(CString sql);		//打开记录集
	void CloseRecordset();							//关闭记录集
	void CloseConn();								//关闭数据库连接
	UINT GetRecordCount(_RecordsetPtr pRecordset);	//获得记录数	
};

#endif // !defined(AFX_ADO_H__E0F8432E_F797_4979_8FB0_EAC60CA6C7F9__INCLUDED_)

// CmdStoredProcz.h: interface for the CCmdStoredProcz class.zsz 
//存储过程
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSTOREDPROCZ_H__D6CBDD5A_8B98_4B38_B02E_380AB5406218__INCLUDED_)
#define AFX_CMDSTOREDPROCZ_H__D6CBDD5A_8B98_4B38_B02E_380AB5406218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCmdStoredProcz  
{
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
public:
	BOOL GetCollect(LPCSTR strFieldName,CString &strValue);//取值(字段名,CString)
	BOOL GetCollect(LPCSTR strFieldName,double &Value);//取值(字段名,double)
	BOOL GetCollect(LPCSTR strFieldName,float &Value);//取值(字段名,float)
	BOOL GetCollect(LPCSTR strFieldName,long &Value);//取值(字段名,long)
	BOOL GetCollect(LPCSTR strFieldName,int &Value);//取值(字段名,int)
	BOOL pRsetISBOF();//指针是否在在记录集头
	BOOL pRsetISEOF();//指针是否在在记录集尾
	//////////////////////////////////////////////////////////////////////////
	BOOL RecordSetRelease();//释放
	BOOL CmdRelease();//释放
	//////////////////////////////////////////////////////////////////////////
	int GetErrorCode(LPCTSTR strErrorcode);//获取返回值
	int GetErrorCode(int index);//获取返回值	
	//////////////////////////////////////////////////////////////////////////
	BOOL SetCommandText(LPCTSTR lpstrCommand);//设置存储过程名称
	BOOL SetParameters(LPCTSTR lpstr,CString strValue);//设置参数
	BOOL SetParameters(LPCTSTR lpstr, int Valuei);
	long CmdExecute();//执行
	BOOL ExecuteCmd();
	//////////////////////////////////////////////////////////////////////////
	CCmdStoredProcz();
	CCmdStoredProcz(_ConnectionPtr pAdoConnection);
	virtual ~CCmdStoredProcz();
private:
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCommand;
};

#endif // !defined(AFX_CMDSTOREDPROCZ_H__D6CBDD5A_8B98_4B38_B02E_380AB5406218__INCLUDED_)

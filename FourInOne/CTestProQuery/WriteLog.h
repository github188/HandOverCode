// WriteLog.h: interface for the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITELOG_H__8747780C_BE83_4604_B063_0B167C95230E__INCLUDED_)
#define AFX_WRITELOG_H__8747780C_BE83_4604_B063_0B167C95230E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define SAVEDAYS 5

class AFX_EXT_CLASS CWriteLog  
{
public:
	void UnRegister(DWORD MoudleID);
	DWORD Register(LPCTSTR ModuleNameToReg);
	void WriteLog(DWORD MoudleID,LPCTSTR pstrFormat, ...);
	CWriteLog();
	virtual ~CWriteLog();
private:
	CString logfilepath;
	static DWORD m_dwFatalErrThrdID;
	static CPtrList m_lstpThreads;
	void DeleteDirectory(CString strDir);
};

#endif // !defined(AFX_WRITELOG_H__8747780C_BE83_4604_B063_0B167C95230E__INCLUDED_)

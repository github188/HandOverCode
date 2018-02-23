// WideCharString.h: interface for the CWideCharString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIDECHARSTRING_H__08AF6AD2_0188_4F7D_8297_C252A433B1AA__INCLUDED_)
#define AFX_WIDECHARSTRING_H__08AF6AD2_0188_4F7D_8297_C252A433B1AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWideCharString : public CObject  
{
public:
	CWideCharString();
	CWideCharString(const CWideCharString& wcString);
	//len=-1 代表取整串字符的长度，若len大于整串字符的长度，则忽略len
	CWideCharString(LPCWSTR widestring, int len=-1);
	CWideCharString(const char* lpszString, int len=-1);
	virtual ~CWideCharString();
	operator LPCWSTR() const {	return m_pBuffer; }
	
	int GetLength() const { return m_nBufferLen; }
	
	CWideCharString operator + (const CWideCharString& wcString);
	CWideCharString& operator += (const CWideCharString& wcString);
	WCHAR	operator[](int index) const;
	WCHAR&	operator[](int index);
	
	CWideCharString& operator = (const CWideCharString& wcString);
	CWideCharString& operator = (const char* lpszString);
	BOOL operator == (const CWideCharString& wcString);
	BOOL operator != (const CWideCharString& wcString);
	CWideCharString Mid(int begin,int len=-1);
	
private:
	WCHAR*	m_pBuffer;
	int		m_nBufferLen;
};

#endif // !defined(AFX_WIDECHARSTRING_H__08AF6AD2_0188_4F7D_8297_C252A433B1AA__INCLUDED_)

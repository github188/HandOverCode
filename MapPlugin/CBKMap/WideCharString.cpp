// WideCharString.cpp: implementation of the CWideCharString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WideCharString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWideCharString::CWideCharString()
{
	m_nBufferLen=0;
	m_pBuffer=new WCHAR [m_nBufferLen+1];
	m_pBuffer[0]=0;
}

CWideCharString::CWideCharString(LPCWSTR widestring, int len)
{
	if(widestring)
		m_nBufferLen=wcslen(widestring);
	else
		m_nBufferLen=0;
	if(len<m_nBufferLen && len>=0){
		m_nBufferLen=len;
	}

	m_pBuffer=new WCHAR [m_nBufferLen+1];
	if(widestring)
		wcsncpy(m_pBuffer, widestring,m_nBufferLen);
	m_pBuffer[m_nBufferLen]=0;
}

CWideCharString::CWideCharString(const CWideCharString& wcString)
{
	m_nBufferLen=wcString.m_nBufferLen;
	m_pBuffer=new WCHAR [m_nBufferLen+1];
	wcscpy(m_pBuffer,wcString.m_pBuffer);
}

CWideCharString::CWideCharString(const char* lpszString, int len)
{
	if(!lpszString){
		m_nBufferLen=0;
		m_pBuffer=new WCHAR [1];
		m_pBuffer[0]=0;
		return;
	}
	int srclen=strlen(lpszString);

	if(len < srclen && len >=0){
		srclen=len;
	}

	m_nBufferLen=srclen;
	m_pBuffer=new WCHAR[m_nBufferLen+1];
	m_nBufferLen=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,lpszString,srclen,m_pBuffer,m_nBufferLen+1);
	m_pBuffer[m_nBufferLen]=0;
}

CWideCharString::~CWideCharString()
{
	if(m_pBuffer)
		delete[] m_pBuffer;
}

CWideCharString& CWideCharString::operator=(const CWideCharString& wcString)
{
	if(this == &wcString)
		return *this;

	if(m_pBuffer)
		delete[] m_pBuffer;
	m_nBufferLen=wcString.m_nBufferLen;
	m_pBuffer=new WCHAR [m_nBufferLen+1];
	wcscpy(m_pBuffer,wcString.m_pBuffer);
	return *this;
}

CWideCharString& CWideCharString::operator =(const char* lpszString)
{
	if(lpszString==NULL){
		m_nBufferLen=0;
		if(!m_pBuffer){
			m_pBuffer=new WCHAR [m_nBufferLen+1];
		}
		m_pBuffer[0]=0;
		return *this;
	}
	
	int srclen=strlen(lpszString);
	m_nBufferLen=srclen;
	if(m_pBuffer)
		delete[] m_pBuffer;
	m_pBuffer=new WCHAR[m_nBufferLen+1];
	m_nBufferLen=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,lpszString,srclen,m_pBuffer,m_nBufferLen+1);
	m_pBuffer[m_nBufferLen]=0;
	return *this;	

}

CWideCharString CWideCharString::Mid(int begin,int len)
{
	if (len<0) len=GetLength();
	if(begin+len>m_nBufferLen)
		len=m_nBufferLen-begin;

	return CWideCharString(m_pBuffer+begin,len);
}
BOOL CWideCharString::operator == (const CWideCharString& wcString)
{
	return (wcscmp(m_pBuffer,wcString.m_pBuffer)==0);
}
BOOL CWideCharString::operator != (const CWideCharString& wcString)
{
	return (wcscmp(m_pBuffer,wcString.m_pBuffer)!=0);
}
WCHAR CWideCharString::operator[] (int index) const
{
	if(index<0 || index>=m_nBufferLen)
		return 0;
	return m_pBuffer[index];
}

WCHAR& CWideCharString::operator[] (int index)
{
	if(index<0 || index>=m_nBufferLen){
		static WCHAR w=0;
		return w;
	}
	return m_pBuffer[index];
}

CWideCharString CWideCharString::operator + (const CWideCharString& wcString)
{
	int totalLen=m_nBufferLen+wcString.m_nBufferLen;
	WCHAR* pBuf=new WCHAR[totalLen+1];

	wcscpy(pBuf,m_pBuffer);
	wcscat(pBuf+m_nBufferLen,wcString.m_pBuffer);

	CWideCharString ret(pBuf);
	delete[] pBuf;

	return ret;
}

CWideCharString& CWideCharString::operator += (const CWideCharString& wcString)
{
	int totalLen=m_nBufferLen+wcString.m_nBufferLen;
	WCHAR* pTempBuf=new WCHAR[totalLen+1];

	wcscpy(pTempBuf,m_pBuffer);
	wcscat(pTempBuf+m_nBufferLen,wcString.m_pBuffer);

	delete[] m_pBuffer;
	m_pBuffer=pTempBuf;
	m_nBufferLen=totalLen;

	return *this;
}
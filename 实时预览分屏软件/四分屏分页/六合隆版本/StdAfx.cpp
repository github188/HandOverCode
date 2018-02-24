// stdafx.cpp : source file that includes just the standard includes
//	CFourPic.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CString logFileName;
void WriteLog(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	CTime curTime =CTime::GetCurrentTime();
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	TRACE(logstr);
	va_end(avlist);
	FILE *fp=fopen(logFileName,"a+");
	fprintf(fp,"[%s]:%s#\n",curTime.Format(_T("%H:%M:%S")),logstr);
	fclose(fp);
}


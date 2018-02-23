// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CString logfilenamez;
void WriteLogz(LPCTSTR pstrFormat, ... )
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format("[%02d:%02d:%02d.%03d]",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	
	FILE *fp=fopen(logfilenamez,"a+");
	fprintf(fp,"%s:%s#\n",strTime,logstr);
	fclose(fp);	
}
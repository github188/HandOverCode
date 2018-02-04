// stdafx.cpp : source file that includes just the standard includes
//	CBKVideoDeCode.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "CBKVideoDeCode.h"
extern CCBKVideoDeCodeApp theApp;
void WriteLog(CString logstr)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%H:%M:%S");
	FILE *fp=fopen(theApp.logFileName,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}
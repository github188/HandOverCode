// stdafx.cpp : source file that includes just the standard includes
//	BKPrintSheet.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



// 判断目录是否存在
BOOL FolderExists(CString s)
{
	DWORD attr; 
	attr = GetFileAttributes(s); 
	return (attr != (DWORD)(-1) ) && ( attr & FILE_ATTRIBUTE_DIRECTORY); 
}


// 创建目录
BOOL CreateDir(CString P)
{
	int len=P.GetLength();
	if ( len <2 ) 
	{
		return FALSE; 
	}
	if('\\'==P[len-1])
	{
		P=P.Left(len-1);
		len=P.GetLength();
	}
	if ( len <=0 ) 
	{
		return FALSE;
	}
	if (len <=3) 
	{
		if (FolderExists(P))
		{
			return TRUE;
		}
		else 
		{
			return FALSE; 
		}
	}
	if (FolderExists(P))
	{
		return TRUE;
	}
	CString Parent;
	Parent=P.Left(P.ReverseFind('\\') );
	if(Parent.GetLength()<=0)
	{
		return FALSE; 
	}
	BOOL Ret=CreateDir(Parent); 
	if(Ret) 
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength=sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor=NULL;
		sa.bInheritHandle=0;
		Ret=(CreateDirectory(P,&sa)==TRUE);
		return Ret;
	}
	else
	{
		return FALSE;
	}
}

void WriteLogPPTXT(CString xmname,CString zkzmBH,CString sstr)   //写入日志文件
{
	CString cFpath=_T("\\log");
	TCHAR FPath[128];
	::GetCurrentDirectory(MAX_PATH, FPath);
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	CString Data1=curTime.Format("\\%Y-%m-%d");
	CString Data2=curTime.Format("%H");
	CString ZHName;
	ZHName.Format("\\日志_%s_%s_%s.txt",xmname,zkzmBH,Data2);
	cFpath =cFpath+Data1+"\\Signal-log";
	strcat(FPath,cFpath);
	CreateDir(FPath);
	cFpath=FPath+ZHName;
	FILE *fp=fopen(cFpath,"a+");
	fprintf(fp,"[%s].%s\n",Data,sstr);
	fclose(fp);	
}
// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "WideCharString.h"
void DrawStringz(Graphics *graphics, const TCHAR *str, const Font &font, const Brush &brush, const RectF &rect, StringFormat &fmt)
{
	CWideCharString string(str, strlen(str));
	graphics->DrawString(string, wcslen(string), &font,rect, &fmt, &brush);
}
void DrawStringz(Graphics *graphics,int istr, const Font &font, const Brush &brush, const RectF &rect, StringFormat &fmt)
{
	CString str;
	str.Format("%d",istr);
	CWideCharString string(str, strlen(str));
	graphics->DrawString(string, wcslen(string), &font,rect, &fmt, &brush);
}
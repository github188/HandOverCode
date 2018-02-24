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

CString VariantToStr(_variant_t var)
{
	CString str;
	
	switch (var.vt)
	{
		/*空*/
	case VT_NULL:
		str = _T("");
		break;
	case VT_EMPTY:
		str = _T("");
		break;
		/*数字*/
	case VT_UI1:
		str.Format("%d", var.iVal);
		break;
	case VT_I2:
		str.Format("%d", var.iVal);
		break;
	case VT_I4:
		str.Format("%d", var.lVal);
		break;
	case VT_R8:
		str.Format("%f", var.dblVal);
		break;
	case VT_R4:
		str.Format("%f", var.fltVal);
		break;
		/*文本*/
	case VT_BSTR:
		str.Format("%s", (char *)_bstr_t(var.bstrVal));
		break;
		/*日期/时间*/
	case VT_DATE:
		{
			COleDateTime time_value;
			time_value = var.date;
			str = time_value.Format("%Y年%m月%d日");
		}
		break;
		/*货币*/
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			str = cy.Format();
		}
		break;
		/*BOOL*/
	case VT_BOOL:
		str = var.boolVal? "男" : "女"; 
		break;
		
	default:
		str = _T("");
		break;
	}
	
	return str;

}
BOOL ChangeToVariant(long value, _variant_t *var, DataTypeEnum type)
{
	switch(type)
	{
	case adBigInt:
		var->vt = VT_I4;
		var->lVal = value;
		break;
	default:
	    break;
	}
	return TRUE;


}

BOOL ChangeToVariant(int value, _variant_t *var, DataTypeEnum type)
{
	switch (type)
	{
	case adInteger:
		var->vt = VT_I2;
		var->iVal = value;
		break;
		
	case adBoolean:
		var->vt = VT_BOOL;
		var->boolVal = !value;
		break;
		
	default:
		break;
	}
	
	return TRUE;
}

BOOL ChangeToVariant(COleDateTime time, _variant_t *var, DataTypeEnum type)
{
	switch (type)
	{
	case adDBDate:
		var->vt = VT_DATE;
		var->date = time;
		break;
		
	default:
		break;
	}
	
	return TRUE;

}

BOOL ChangeToVariant(CString str, _variant_t *var, DataTypeEnum type)
{
	switch (type)
	{
	case adBSTR:
		*var = (_variant_t)(str);
		break;
		
	case adInteger:
		var->vt = VT_I4;
		var->lVal = atoi(str);
		break;
		
	default:
		break;
	}
	
	return TRUE;
}
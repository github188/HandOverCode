// BKPrintSheet.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BKPrintSheet.h"
#include "ADO.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CBKPrintSheetApp
#define DllExport  __declspec( dllexport )

BEGIN_MESSAGE_MAP(CBKPrintSheetApp, CWinApp)
	//{{AFX_MSG_MAP(CBKPrintSheetApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBKPrintSheetApp construction
// #define LINEHEIGHT 110 //行高
// #define LINEOFNAMERIGHT 380 //姓名右边竖线小格子

CBKPrintSheetApp::CBKPrintSheetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBKPrintSheetApp object

CBKPrintSheetApp theApp;





int CBKPrintSheetApp::loadconfig()
{
	//解密
	TCHAR FPath[128];
	::GetCurrentDirectory(MAX_PATH, FPath);
	strcat(FPath, "\\cfg.ini");
	CFile ConfigFile;
	if(!ConfigFile.Open(FPath, CFile::modeCreate||CFile::modeRead, NULL))
	{
		//AfxMessageBox("找不到配置文件!");
		TRACE("找不到配置文件!");
		return -1;
	}
	ConfigFile.Close();
	//CString sTemp,s,str;
	int izt;
	izt=GetPrivateProfileInt("config","KGprint",0,FPath);
	return izt;
	//::GetPrivateProfileString("config", "KGprint", 0, sTemp.GetBuffer(50), 50, FPath); 
//sTemp.ReleaseBuffer();

}



int CBKPrintSheetApp::Serialnum(CString strYKRQ,CString strExamNo,CString strZT,CString strDLR,CString strBkcx)
{
	
	int iCount=1;
	try
	{
		_variant_t vat;
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		//导入资料
		CString strText,szSQL;
		if (strZT==_T("1"))
		{
			szSQL.Format("Set ARITHABORT ON;select dbo.charDecode(准考证明编号) as 准考证明编号 from examrecordindetail where 考试日期=dbo.dateEncode('%s') \
				and (考试成绩-right(dbo.charDecode(准考证明编号),2)-45)>=%d and 驾校名称='%s' and 考试车型 ='%s'order by \
				考试时间",strYKRQ,kmms,strDLR,strBkcx);
		}else if (strZT==_T("2"))
		{
			szSQL.Format("Set ARITHABORT ON;select dbo.charDecode(准考证明编号) as 准考证明编号 from examrecordindetail where 考试日期=dbo.dateEncode('%s') and \
				((考试成绩-right(dbo.charDecode(准考证明编号),2)-45)<%d) and 当日次数='2' and 驾校名称='%s' and 考试车型='%s' \
				order by  考试时间 ",strYKRQ,kmms,strDLR,strBkcx);
		}
		pRecordset->Open(_variant_t(szSQL), _variant_t((IDispatch*)theApp.m_pConnection, true),
			adOpenDynamic, adLockOptimistic, adCmdText);//打开到数据库连接的记录
		int len=pRecordset->RecordCount;
		if(pRecordset != NULL)
		{
			while(!pRecordset->adoEOF)
			{
				vat =pRecordset->GetCollect("准考证明编号");
				if (vat.vt != VT_NULL)
				{
					strText = (LPCSTR)_bstr_t(vat);
					if (strcmp(strExamNo,strText)!=0)
						iCount++;
					else
						break;
					
				}
				pRecordset->MoveNext();
			}
		}
		if (iCount>len)
		{
			iCount=0;
		}
		pRecordset->Close();
		pRecordset.Release();
	}
	catch (_com_error e)
	{
		CString sErrInfo;
		sErrInfo.Format("加载考生%s序号失败!未考试",strExamNo);
		AfxMessageBox(sErrInfo);
		return -1;
	}
	return iCount;
}








extern "C" DllExport BOOL PrintSheet(LPTSTR lpExamNo, int ikscs,LPTSTR strSQLIP,LPTSTR strUser,LPTSTR strPassword) 
{
	theApp.m_IPAddress.Format("%s",strSQLIP);
	theApp.m_PassWord.Format("%s",strPassword);
	if(theApp.DoPreparePrintDC() == FALSE) return FALSE;
	CString szTitle;
	CString strExamNo;
	strExamNo.Format("%s",lpExamNo);
	szTitle.Format("场考成绩单--准考证明编号:%s",strExamNo);
	DOCINFO di;	
	::ZeroMemory(&di, sizeof(DOCINFO)); 	
	di.cbSize = sizeof(DOCINFO); 
	di.lpszDocName = szTitle;
	if (theApp.m_DC.StartDoc(&di)<0)
	{	
		AfxMessageBox(_T("连接打印机失败!"), MB_ICONSTOP);
		return FALSE;
	}
	if(theApp.m_DC.StartPage() < 0)//开始页
	{
		AfxMessageBox( _T("打印失败!"), MB_ICONSTOP);
		theApp.m_DC.AbortDoc();
		return FALSE;
	}
	CoInitialize(NULL);
	if (!theApp.CreateDataCon(strSQLIP,strUser,strPassword))
	{
		theApp.m_DC.EndPage(); 
		theApp.m_DC.EndDoc();
		theApp.DoClearPrintDC();
		return FALSE;
	}
	theApp.ZPHQ=GetPrivateProfileInt("Switch","ZPHQ",0,".\\cfg.ini");
	if (theApp.ZPHQ==0)
	{
		::WritePrivateProfileString("Switch","ZPHQ ","2",".\\cfg.ini");  //抓拍照片  1.存储过程  2.字段获取
		theApp.ZPHQ=2;
	}
	theApp.SQLorOracle=GetPrivateProfileInt("CONFIG","SQLorOracle",0,".\\cfg.ini");
	if (theApp.SQLorOracle==0)
	{
		::WritePrivateProfileString("CONFIG","SQLorOracle ","1",".\\cfg.ini");
		theApp.SQLorOracle=1;
	}
	//////////////////////////////////////////////////////////////////////////
	CString strName,strZkzm,strSfzm,strBkcx,strJX,strYWLX,strKsrq,strYycs,strksy1,strdrcs,strYKRQ,strZT,strDLR;
	strYycs.Format("%d",ikscs);
	CString strTime1,strTime2,strCJ1,strCJ2,szSQL,strTimeStop1,strTimeStop2;
	try
	{
		_variant_t vat;
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		//导入资料
		szSQL.Format("Set ARITHABORT ON;select top 1 sc.驾校编号,dbo.charDecode(ex.姓名) as 姓名,dbo.charDecode(ex.准考证明编号) as 准考证明编号,dbo.charDecode(ex.身份证明编号) as 身份证明编号,ex.考试车型,ex.驾校名称,ex.考试原因, \
			ex.考试成绩-right(dbo.charDecode(ex.准考证明编号),2)-45 as 成绩,dbo.dateDecode(ex.考试日期),ex.考试次数,ex.当日次数,ex.考试员1   \
			from examrecordindetail ex,SchoolInfo sc where  准考证明编号=dbo.charEncode('%s') and sc.驾校名称=ex.驾校名称 and 考试次数='%d' order by 考试时间 desc",strExamNo,ikscs);
		//AfxMessageBox(szSQL);
		TRACE(szSQL);
		pRecordset->Open(_variant_t(szSQL), _variant_t((IDispatch*)theApp.m_pConnection, true),
			adOpenDynamic, adLockOptimistic, adCmdText);//打开到数据库连接的记录
		if (0==pRecordset->RecordCount )
		{
			AfxMessageBox("打印内容为空!");
			/*终止打印作业*/
			theApp.m_DC.EndPage(); 
			theApp.m_DC.EndDoc();
			/*清除打印场景*/ 
			theApp.DoClearPrintDC();
			return FALSE;
		}
		if(pRecordset != NULL)
		{
			while(!pRecordset->adoEOF)
			{
				vat =pRecordset->GetCollect("姓名");
				if (vat.vt != VT_NULL){
					strName = (LPCSTR)_bstr_t(vat);
				}else{strName=_T("");}
				vat =pRecordset->GetCollect("准考证明编号");
				if (vat.vt != VT_NULL){
					strZkzm = (LPCSTR)_bstr_t(vat);
				}else{strZkzm=_T("");}
				vat =pRecordset->GetCollect("身份证明编号");
				if (vat.vt != VT_NULL){
					strSfzm = (LPCSTR)_bstr_t(vat);
				}else{strSfzm=_T("");}
				vat =pRecordset->GetCollect("考试车型");
				if (vat.vt != VT_NULL){
					strBkcx = (LPCSTR)_bstr_t(vat);
				}else{strBkcx=_T("");}
				vat =pRecordset->GetCollect("驾校名称");
				if (vat.vt != VT_NULL){
					strJX = (LPCSTR)_bstr_t(vat);
				}else{strJX=_T("");}
				vat =pRecordset->GetCollect("驾校编号");
				if (vat.vt != VT_NULL){
					strDLR = (LPCSTR)_bstr_t(vat);
				}else{strDLR=_T("");}
				vat =pRecordset->GetCollect("考试原因");
				if (vat.vt != VT_NULL){
					strYWLX = (LPCSTR)_bstr_t(vat);
				}else{strYWLX=_T("");}
				vat =pRecordset->GetCollect("考试员1");
				if (vat.vt != VT_NULL){
					strksy1 = (LPCSTR)_bstr_t(vat);
				}else{strksy1=_T("");}
				vat =pRecordset->GetCollect("当日次数");
				if (vat.vt != VT_NULL){
					strdrcs = (LPCSTR)_bstr_t(vat);
				}else{strdrcs=_T("");}
				vat = pRecordset->GetCollect("成绩");
				if ( vat.vt != VT_NULL )
				{
					strZT = (LPCSTR)_bstr_t(vat);
					if (atoi(strZT)>=theApp.kmms)
					{
						strZT="1";
					}else
						strZT="2";

				}else{strZT=_T("0");}

				vat =pRecordset->GetCollect("考试日期");
				if (vat.vt != VT_NULL){
					COleDateTime date_value;
					date_value = vat.date;
					strKsrq =date_value.Format("%Y年%m月%d日");
					strYKRQ=date_value.Format("%Y-%m-%d");
				}else{strKsrq=_T("");}
				
				if (strYWLX==_T("A"))
				{
					strYWLX="初领";
				}
				else if (strYWLX==_T("B"))
				{
					strYWLX="增驾";
				}else if (strYWLX==_T("F"))
				{
					strYWLX="满分学习";
				}else if (strYWLX==_T("D"))
				{
					strYWLX="补考";
				}
				pRecordset->MoveNext();
			}
		}
		pRecordset->Close();
		szSQL.Format("Set ARITHABORT ON;select dbo.dateDecode(开始时间) as 开始时间,考试成绩-RIGHT(dbo.charDecode(准考证明编号),2)-45 as 成绩,当日次数,dbo.dateDecode(考试时间) as 考试时间 from \
			ExamRecord where 准考证明编号=dbo.charEncode('%s') and 考试次数='%d' ",strExamNo,ikscs);
		pRecordset->Open(_variant_t(szSQL), _variant_t((IDispatch*)theApp.m_pConnection, true),
			adOpenDynamic, adLockOptimistic, adCmdText);//打开到数据库连接的记录
		CString tempdrcs;
		if(pRecordset != NULL)
		{
			while(!pRecordset->adoEOF)
			{
				vat =pRecordset->GetCollect("当日次数");
				if (vat.vt != VT_NULL)
				{
					tempdrcs = (LPCSTR)_bstr_t(vat);
					if (strcmp(tempdrcs,"1")==0)
					{
						vat =pRecordset->GetCollect("开始时间");
						COleDateTime date_value;
						date_value = vat.date;
						strTime1 =date_value.Format("%H:%M:%S");
						vat =pRecordset->GetCollect("考试时间");
						if (vat.vt != VT_NULL){
							COleDateTime date_value1;
							date_value1 = vat.date;
							strTimeStop1=date_value1.Format("%H:%M:%S");
						}
						VARIANT intCol =pRecordset->GetCollect("成绩");
						if (intCol.intVal >=theApp.kmms)
						{
							strCJ1.Format("%d (合格)",intCol.intVal);
						}
						else
						{
							strCJ1.Format("%d (不合格)",intCol.intVal);
						}	
					}
					if (strcmp(tempdrcs,"2")==0)
					{
						vat =pRecordset->GetCollect("开始时间");
						COleDateTime date_value;
						date_value = vat.date;
						strTime2 =date_value.Format("%H:%M:%S");
						vat =pRecordset->GetCollect("考试时间");
						COleDateTime date_value1;
						date_value1 = vat.date;
						strTimeStop2 =date_value1.Format("%H:%M:%S");

						VARIANT intCol =pRecordset->GetCollect("成绩");
						if (intCol.intVal >=theApp.kmms)
						{
							strCJ2.Format("%d (合格)   ",intCol.intVal);
						}
						else
						{
							strCJ2.Format("%d (不合格)  ",intCol.intVal);
						}

					}
				}
				pRecordset->MoveNext();
			}
		}
		pRecordset->Close();
		pRecordset.Release();
		
	}
	catch (_com_error e)
	{
		CString sErrInfo;
		sErrInfo.Format("加载打印数据失败!\n%s %s考试数据不完整!",strName,strExamNo);
		WriteLogPPTXT("打印照片编号","日志",sErrInfo);
		/*终止打印作业*/
		theApp.m_DC.EndPage(); 
		theApp.m_DC.EndDoc();
		/*清除打印场景*/ 
		theApp.DoClearPrintDC();
		return FALSE;
	}
	//GetKFX(CStringArray &dest,CString zkzm,int kscs,int drcs);
	int idrcs=_ttoi(strdrcs);
	if (!theApp.CreateDataCon(strSQLIP,strUser,strPassword))
	{
		theApp.m_DC.EndPage(); 
		theApp.m_DC.EndDoc();
		theApp.DoClearPrintDC();
		return FALSE;
	}	
	int iCount=theApp.Serialnum(strYKRQ,strExamNo,strZT,strJX,strBkcx);
	if (iCount==-1)
	{
		/*终止打印作业*/
		theApp.m_DC.EndPage(); 
		theApp.m_DC.EndDoc();
		/*清除打印场景*/ 
		theApp.DoClearPrintDC();
		return FALSE;
	}
	int   nHorRes   =   GetDeviceCaps(theApp.m_DC, HORZRES); 
	int   nVerRes   =   GetDeviceCaps(theApp.m_DC, VERTRES);
	CRect DCRect(140,80,nHorRes-100,nVerRes-80);//最大的
	theApp.DCRectT=DCRect;
	theApp.LineHeight = DCRect.Height()/26;
	CPen pen,* oldpen;
	pen.CreatePen(PS_SOLID,4,RGB(0,0,0));
	oldpen = theApp.m_DC.SelectObject(&pen);
	//////////////////////////////////////////////////////////////////////////
	CFont font1,font2,font3,*oldfont;
	font1.CreateFont(-MulDiv(16,theApp.m_DC.GetDeviceCaps(LOGPIXELSY),72), 0, 0, 0, 800, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "黑体");
	font2.CreateFont(-MulDiv(12,theApp.m_DC.GetDeviceCaps(LOGPIXELSY),72), 0, 0, 0, 400, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
				
	//font2 加粗
	font3.CreateFont(-MulDiv(12,theApp.m_DC.GetDeviceCaps(LOGPIXELSY),72), 0, 0, 0, 560, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	//////////////////////////////////////////////////////////////////////////
	oldfont = theApp.m_DC.SelectObject(&font1);//大字体
	CString Cstrtemp;
	CRect TitleRect(DCRect.left,DCRect.top,DCRect.right,DCRect.top+theApp.LineHeight*2);
	//theApp.m_DC.Rectangle(TitleRect);	
	if (theApp.kmms==80)
	{
		Cstrtemp = _T("科目二考试成绩单");
	}else
		Cstrtemp = _T("科目三道路驾驶技能考试成绩单");
	theApp.m_DC.DrawText(Cstrtemp,&TitleRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	oldfont = theApp.m_DC.SelectObject(&font2);
 	Cstrtemp.Format("第%d张-%s-%s",iCount,strBkcx,strJX);
	WriteLogPPTXT("打印照片编号","日志",Cstrtemp);
	CRect SerialRect(DCRect.left,DCRect.top,DCRect.right/3,DCRect.top+theApp.LineHeight*3);  //60,60,380,150
	theApp.m_DC.DrawText(Cstrtemp,&SerialRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
 	oldfont = theApp.m_DC.SelectObject(&font2);


	DCRect.top=TitleRect.bottom;
	CRect QRrc;
	QRrc.SetRect(TitleRect.right-TitleRect.Height(),TitleRect.top,TitleRect.right,TitleRect.bottom);
	//theApp.m_DC.Rectangle(QRrc);
	theApp.m_DC.Rectangle(DCRect);
	for (int i=4;i<23;i++)
	{
		if ((i>8&&i<12) || (i>12&&i<16) || (i>18&&i<21))
		{
			continue;
		}
		theApp.DrawXLine(i);
	}
	CRect r1,r2,r3,r4,r5,r6,rline;
	r1.SetRect(DCRect.left,DCRect.top,DCRect.right*0.15,DCRect.top+theApp.LineHeight);
	r2.SetRect(r1.right+10,r1.top,(DCRect.right+10)*0.38,r1.bottom);
	r3.SetRect(r2.right,r1.top,DCRect.right*0.53,r1.bottom);
	r4.SetRect(r3.right,r1.top,DCRect.right*0.7,r1.bottom);
	r5.SetRect(r4.right,r1.top,DCRect.right*0.85,r1.bottom);
	r6.SetRect(r5.right,r1.top,DCRect.right,r1.bottom);
	rline.SetRect(DCRect.left,DCRect.top,DCRect.right,DCRect.top+theApp.LineHeight);
	//////////////////////////////////////////////////////////////////////////
	CRect photo1,photo2;
	photo1.SetRect(r5.left,r5.top+theApp.LineHeight-2,r5.right,r5.bottom+theApp.LineHeight*3);
	photo2.SetRect(photo1.right,photo1.top,photo1.right+photo1.Width()-2,photo1.bottom);
	theApp.m_DC.Rectangle(photo1);
	theApp.m_DC.Rectangle(photo2);
	theApp.ReadGYPhotoFromDB(&theApp.m_DC, strExamNo,&photo1,&photo2);
	//////////////////////////////////////////////////////////////////////////
	CRect XMPhoto1,XMPhoto2;
	XMPhoto1.SetRect(DCRect.left+10,DCRect.top+theApp.LineHeight*10,DCRect.right-10,DCRect.top+theApp.LineHeight*14);
	XMPhoto2.SetRect(DCRect.left+10,DCRect.top+theApp.LineHeight*20,DCRect.right-10,DCRect.top+theApp.LineHeight*24);
	if (theApp.ZPHQ==1)
	{
		theApp.ReadXMPhotoFromDB(&theApp.m_DC,strExamNo,1,ikscs,&XMPhoto1);  //存储过程
	}else
		theApp.ReadXMPhotoFromSQL(&theApp.m_DC,&XMPhoto1,strExamNo,ikscs,1);  //通过字段获取
	//theApp.ReadXMPhotoFromDB(&theApp.m_DC,strExamNo,1,ikscs,&XMPhoto1);
	if (2==idrcs)
	{
	//	theApp.ReadXMPhotoFromDB(&theApp.m_DC,strExamNo,2,ikscs,&XMPhoto2);
		if (theApp.ZPHQ==1)
		{
			theApp.ReadXMPhotoFromDB(&theApp.m_DC,strExamNo,2,ikscs,&XMPhoto2);
		}else
			theApp.ReadXMPhotoFromSQL(&theApp.m_DC,&XMPhoto2,strExamNo,ikscs,2);
	}
	
	//////////////////////////////////////////////////////////////////////////
	CRect kfxrc;
	kfxrc.SetRect(r1.right+100,DCRect.top+theApp.LineHeight*6,DCRect.right,DCRect.top+theApp.LineHeight*7);
	CStringArray cakfx1,cakfx2;
	theApp.GetKFX(cakfx1,strExamNo,ikscs,1);
	for (int ci=0;ci<cakfx1.GetSize();ci++)
	{
		switch (ci)
		{
		case 0:
			theApp.SetDcText(cakfx1.GetAt(0),kfxrc,0,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 1:
			theApp.SetDcText(cakfx1.GetAt(1),kfxrc,1,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 2:
			theApp.SetDcText(cakfx1.GetAt(2),kfxrc,2,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		}
	}
	if (idrcs ==2)
	{
		theApp.GetKFX(cakfx2,strExamNo,ikscs,2);
		for (int ci=0;ci<cakfx2.GetSize();ci++)
		{
			switch (ci)
			{
			case 0:
				theApp.SetDcText(cakfx2.GetAt(0),kfxrc,10,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				break;
			case 1:
				theApp.SetDcText(cakfx2.GetAt(1),kfxrc,11,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				break;
			case 2:
				theApp.SetDcText(cakfx2.GetAt(2),kfxrc,12,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				break;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////

	theApp.DrawXLine(3);
	theApp.DrawYLine(r1.right,2,6);
	theApp.DrawYLine(r1.right,7,12);
	theApp.DrawYLine(r1.right,17,22);	
	theApp.DrawYLine(r2.right,2,6);
	theApp.DrawYLine(r3.right,2,6);
	theApp.DrawYLine(r4.right,2,6);
	theApp.DrawYLine(r5.right,2,6);
	oldfont = theApp.m_DC.SelectObject(&font3);
	theApp.SetDcText("姓    名",r1,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("准考证号",r1,1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("身份证号",r1,2,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("驾校简称",r1,3,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考试时间",r1,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("扣分项",r1,7,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考试员签名",r1,9,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	theApp.SetDcText("考试时间",r1,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("扣分项",r1,17,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考试员签名",r1,19,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考试日期",r3,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("报考车型",r3,1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("业务类型",r3,2,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("预约次数",r3,3,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//theApp.SetDcText("2014年03月28日",r4,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("报名照片",r5,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("门禁照片",r6,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	if (theApp.kmms==80)
	{
		theApp.SetDcText("科目二考试",rline,4,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}else
		theApp.SetDcText("科目三道路驾驶技能考试",rline,4,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (theApp.kmms==80)
	{
		theApp.SetDcText("科目二补考",rline,14,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	} 
	else
	{
		theApp.SetDcText("科目三道路驾驶技能补考",rline,14,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	CRect r7,r8,r9;
	r7.SetRect(r1.right,r1.top,rline.Width()/2,r1.bottom);
	r8.SetRect(r7.right,r7.top,r7.right+r1.Width(),r7.bottom);
	r9.SetRect(r8.right,r8.top,rline.right,r8.bottom);
	// 	theApp.m_DC.Rectangle(r7);
	// 	theApp.m_DC.Rectangle(r8);
	// 	theApp.m_DC.Rectangle(r9);
	theApp.DrawYLine(r7.right,7,8);
	theApp.DrawYLine(r8.right,7,8);
	theApp.DrawYLine(r7.right,11,12);
	theApp.DrawYLine(r8.right,11,12);
	theApp.DrawYLine(r7.right,17,18);
	theApp.DrawYLine(r8.right,17,18);
	theApp.DrawYLine(r7.right,21,22);
	theApp.DrawYLine(r8.right,21,22);

	theApp.SetDcText("考试成绩",r8,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考生签名",r8,9,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考试成绩",r8,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("考生签名",r8,19,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	oldfont = theApp.m_DC.SelectObject(&font2);
	//////////////////////////////////////////////////////////////////////////
	//ReadQZPhoto(CDC *pDC, CString sCard, CString sSir, int iDRCS,CRect *rc1, CRect *rc2, CRect *rc3,CRect *rc4)
	CRect qzr1,qzr2,qzr3,qzr4;
	qzr1.SetRect(r7.left+200,r7.top-10+theApp.LineHeight*9,r7.left+200+(r7.Height()*2),r7.top+theApp.LineHeight*10);
	qzr3.SetRect(qzr1.left,qzr1.top+theApp.LineHeight*10,qzr1.right,qzr1.top+theApp.LineHeight*11+10);
	qzr2.SetRect(r9.left+200,qzr1.top-10,r9.left+240+(r9.Height()/3*4),qzr1.bottom);
	qzr4.SetRect(qzr2.left,qzr2.top+theApp.LineHeight*10,qzr2.right,qzr2.top+theApp.LineHeight*11);
	theApp.ReadQZPhoto(&theApp.m_DC,strExamNo,strksy1,idrcs,&qzr1,&qzr2,&qzr3,&qzr4);
//////////////////////////////////////////////////////////////////////////
	theApp.DrawXLine(11);
	theApp.DrawXLine(21);
	CString strtemp;
	theApp.SetDcText(" "+strName,r2,0,DT_LEFT | DT_VCENTER | DT_SINGLELINE);//姓名
 	theApp.SetDcText(" "+strZkzm,r2,1,DT_LEFT | DT_VCENTER | DT_SINGLELINE);//准考证明
 	theApp.SetDcText(" "+strSfzm,r2,2,DT_LEFT | DT_VCENTER | DT_SINGLELINE);//身份证明
	strtemp.Format("%s-%s-%s",strName,strZkzm,strSfzm);
	WriteLogPPTXT("打印照片编号","日志",strtemp);
 	theApp.SetDcText(" "+strJX,r2,3,DT_LEFT | DT_VCENTER | DT_SINGLELINE);//驾校
	theApp.SetDcText(strKsrq,r4,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//考试日期
	theApp.SetDcText(strBkcx,r4,1,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//报考车型
	theApp.SetDcText(strYWLX,r4,2,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//业务类型
	theApp.SetDcText(strYycs,r4,3,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//预约次数
	theApp.SetDcText(strCJ1,r9,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//考试成绩1
	theApp.SetDcText(strCJ2,r9,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//考试成绩2
	CString strTimeZH;
	if (strTime1!=_T(""))
	{
		strTimeZH.Format("%s-%s",strTime1,strTimeStop1);
		theApp.SetDcText(strTimeZH,r7,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//考试时间1
	}
	if (strTime2!=_T(""))
	{
		strTimeZH.Format("%s-%s",strTime2,strTimeStop2);
		theApp.SetDcText(strTimeZH,r7,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//考试时间2
	}
	/*
	//////////////////////////////////////////////////////////////////////////
	CString twostr;
	if (idrcs ==1)
	{
		twostr.Format("姓名:%s\n身份证:%s\n一次成绩:%s",strName,strSfzm,strCJ1);
	}
	else
	{
		twostr.Format("姓名:%s\n身份证:%s\n二次成绩:%s",strName,strSfzm,strCJ2);
	}
	theApp.TwoDimQR(&theApp.m_DC,twostr,&QRrc);
	*/

	///////////////////条形码/////////////////
	///////////////////条形码/////////////////
	CFont fonttm;
	fonttm.CreateFont(120, 0, 0, 0, 200, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "IDAutomationHC39M");
	oldfont = theApp.m_DC.SelectObject(&fonttm);
	CString sfzmbh;
	sfzmbh.Format("*%s*",strSfzm);	
	theApp.m_DC.TextOut(DCRect.left*15.4,DCRect.top-160,sfzmbh);
	fonttm.DeleteObject();
	///////////////////条形码/////////////////
	pen.DeleteObject();
	oldpen->DeleteObject();
	font1.DeleteObject();
	font2.DeleteObject();
	font3.DeleteObject();
	oldfont->DeleteObject();
	/*终止打印作业*/
	theApp.m_DC.EndPage(); 
	theApp.m_DC.EndDoc();
	/*清除打印场景*/ 
	//theApp.m_DC.DeleteDC();
	theApp.DoClearPrintDC();
	CoUninitialize();
	return TRUE;
}


CString CBKPrintSheetApp::KM3Read(int iPHOTO)
{
	
	CString strtemp;
	switch (iPHOTO)
	{
	case 40100:
		strtemp=_T("SCZB"); //上车准备
		break;
	case 40200:
		strtemp=_T("QB");  //起步
		break;
	case 40300:
		strtemp=_T("ZX");  //直线
		break;
	case 40400:
		strtemp=_T("JJD");  //加减档
		break;
	case 40500:
		strtemp=_T("BGCD");  //变更车道
		break;
	case 40600:
		strtemp=_T("KBTC");  //靠边停车
		break;
	case 40700:
		strtemp=_T("LKZX");  //直行通过路口
		break;
	case 40800:
		strtemp=_T("LKZZ");  //路口左转
		break;
	case 40900:
		strtemp=_T("LKYZ");  //路口右转
		break;
	case 41000:
		strtemp=_T("RXHD");  //人行横道
		break;
	case 41100:
		strtemp=_T("TGXX");   //学校
		break;
	case 41200:
		strtemp=_T("TGGGCZ");  //公共汽车
		break;
	case 41300:
		strtemp=_T("HC");  //会车
		break;
	case 41400:
		strtemp=_T("CC");  //超车
		break;
	case 41500:
		strtemp=_T("DT");  //掉头
		break;
	case 41700:
		strtemp=_T("BGCD");  //夜间小车
		break;
	case 41600:
		strtemp=_T("BGCD");  //夜间大车
		break;
	}
	return strtemp;
}

BOOL CBKPrintSheetApp::ReadProject(CStringArray &strProject,CString sExamNo,int iKscs,int iDrcs)
{
	ADO m_AdoProject;
	m_AdoProject.OnInitADOConn(m_IPAddress,sVersion,"sa",m_PassWord,theApp.SQLorOracle);
	CString strsql,strtemp;
	strsql.Format("Set ARITHABORT ON;select 公安网代码 from errorrecordindetail where right(公安网代码,2)='00' and right(错误编号,3)!='990' \
		and 准考证明编号=dbo.charEncode('%s') and 考试次数='%d' and 当日次数='%d'",sExamNo,iKscs,iDrcs);
	//AfxMessageBox(strsql);
	m_AdoProject.m_pRecordset=m_AdoProject.OpenRecordset(strsql);
	int len=m_AdoProject.GetRecordCount(m_AdoProject.m_pRecordset);
	int icount=0;
	if (kmms==80)
		strProject.Add("p10001");
	if (len!=0)
	{
		while(!m_AdoProject.m_pRecordset->adoEOF)								//当记录集指针没有指向最后时
		{
			if (icount>=2)
				break;
			if (kmms==80)
				strsql.Format("p%s",m_AdoProject.GetStrFromRecordset("公安网代码"));
			else
			{
				strtemp.Format("%s",m_AdoProject.GetStrFromRecordset("公安网代码"));
				strsql=KM3Read(atoi(strtemp));
			}
			if (strsql!=_T(""))
				icount+=1;
			strProject.Add(strsql);
			m_AdoProject.m_pRecordset->MoveNext();								//将记录集指针移动到下一条记录								//记录个数的变量加1
		}
	}
	if (kmms==80)
		strProject.Add("p10086");
	else
		strProject.Add("ENDBUTTON");
	m_AdoProject.CloseRecordset();
	m_AdoProject.CloseConn();
	return TRUE;
}




BOOL CBKPrintSheetApp::ReadXMPhotoFromSQL(CDC *pDC,CRect *rc,CString sExamNo,int iKscs, int Printcs)
{
   	CStringArray sItem11;/*={"QB","SCZB","ZX","BGCD","LKZZ","LKYZ","LKZX","JJD","KBTC","DT","HC","CC","RXHD","TGXX","TGGGCZ","JSQY","ENDBUTTON"}*/

	ReadProject(sItem11,sExamNo,iKscs,Printcs);
	int lenarr=sItem11.GetSize();
	CString strtemparr;
	for (int i=0;i<lenarr;i++)
	{
		strtemparr+="-"+sItem11[i];
	}
	WriteLogPPTXT("打印照片编号","日志",strtemparr);
	int iShowPhoto=0;
	CString ItemPicture[20];
	int iHavePhoto=0;
	ADO m_AdoPhoto;
	m_AdoPhoto.OnInitADOConn(m_IPAddress,sVersionphoto,"sa",m_PassWord,theApp.SQLorOracle);
	CString temp;
	try
	{
		CString strSQL;
		strSQL.Format("Set ARITHABORT ON;SELECT * FROM XMPhoto WHERE (准考证明编号 = dbo.charEncode('%s') AND (考试次数 = %d) AND (当日次数 =%d)",sExamNo,iKscs,Printcs);
		
        m_AdoPhoto.OpenRecordset(strSQL);
        if(m_AdoPhoto.GetRecordCount(m_AdoPhoto.m_pRecordset)==0)
		{
			return FALSE;
		}
		int len=sItem11.GetSize();
		for(int z=0;z<len;z++)
		{
			long lDataSize = m_AdoPhoto.m_pRecordset->GetFields()->GetItem(sItem11[z].GetBuffer(0))->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{
				ItemPicture[iHavePhoto]=sItem11[z];
				iHavePhoto++;

			}
		
		}

		if(iHavePhoto<3)
		{
           if(iHavePhoto==1)
		   {
              ItemPicture[1]=ItemPicture[0];
              ItemPicture[2]=ItemPicture[0];
			  iHavePhoto=3;
		
		   }
		   else if(iHavePhoto==2)
		   {
			  ItemPicture[2]=ItemPicture[0];
              iHavePhoto=3;
			  
		   }
		}
		for(z=0;z<iHavePhoto;z++)
		{	
		    long lDataSize = m_AdoPhoto.m_pRecordset->GetFields()->GetItem(ItemPicture[z].GetBuffer(0))->ActualSize; //数据库中图像数据长度
			ItemPicture[z].ReleaseBuffer();
			if (lDataSize > 0)
			{
				char* pbuf = NULL;
				_variant_t varBLOB;
		//		varBLOB.vt = VT_I4;
				LPVOID m_pBMPBuffer;
//				varBLOB = m_AdoPhoto.m_pRecordset->GetFields()->GetItem("p10001")->GetChunk(lDataSize);
				varBLOB = m_AdoPhoto.m_pRecordset->GetFields()->GetItem("p20200")->GetChunk(lDataSize);
  			    varBLOB =m_AdoPhoto.m_pRecordset->GetFields()->GetItem(ItemPicture[z].GetBuffer(0))->GetChunk(lDataSize);
				ItemPicture[z].ReleaseBuffer();
	
				
				m_pBMPBuffer = new char[lDataSize+1];
				if (varBLOB.vt ==(VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					SafeArrayUnaccessData(varBLOB.parray);
					//******************************************************//
					IStream* pStm;
					long cb = lDataSize;
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
					LPVOID pvData;
					if (hGlobal != NULL)
					{
						pvData = GlobalLock(hGlobal);
						memcpy(pvData, m_pBMPBuffer, cb);
						GlobalUnlock(hGlobal);
						CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
					}
					else
					{
						pStm->Release ();
						GlobalFree(hGlobal); 
						if(m_pBMPBuffer)
						{
							delete m_pBMPBuffer;
							m_pBMPBuffer = NULL;
						}
						continue;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						iShowPhoto++;
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						CRect rcClient;
						rcClient.SetRect(1400,-350,1410,-565);
						pImg->Render(*pDC,rc->left+rc->Width()*(iShowPhoto-1)/3,rc->top,rc->Width()/3,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
						if(iShowPhoto>=3)
						{
							break;
						}

					}
					else
					{
						pStm->Release ();
						pImg->Release();
						GlobalFree(hGlobal); 
						if(m_pBMPBuffer)
						{
							delete m_pBMPBuffer;
							m_pBMPBuffer = NULL;
						}
						continue;
					}
					pStm->Release ();
					pImg->Release();
					GlobalFree(hGlobal); 
					if(m_pBMPBuffer)
					{
						delete m_pBMPBuffer;
						m_pBMPBuffer = NULL;
					}
					
				}
				if(m_pBMPBuffer)
				{
					delete m_pBMPBuffer;
					m_pBMPBuffer = NULL;
				}
			}else
			{
				CString strerror;
				strerror.Format("获取复制照片字段%s失败!",ItemPicture[z]);
				WriteLogPPTXT("打印照片编号","日志",strerror);
			}
		}
		m_AdoPhoto.CloseRecordset();
		m_AdoPhoto.CloseConn();
		
	}
	catch (_com_error e) 
	{
		CString strErr;
		strErr.Format("读取项目照片数据库失败!");
		WriteLogPPTXT("打印照片编号","日志",strErr);
		//AfxMessageBox(strErr);
		//AfxMessageBox(e.Description());
		return FALSE;
	}


	return TRUE;
}





BOOL CBKPrintSheetApp::DoPreparePrintDC()
{
	CPrintDialog dlgPrint(FALSE);
	
	if(!dlgPrint.GetDefaults()) return FALSE; 
	
	CString PrinterName = dlgPrint.GetDeviceName();
	HDC hdc1 = dlgPrint.GetPrinterDC();	
	LPDEVMODE pDM = dlgPrint.GetDevMode(); 
	
	if(pDM == NULL) return FALSE; 
	
	::GlobalUnlock(pDM); 
	
	if(!m_DC.Attach(hdc1)) return FALSE; 
	
	/*设置打印标志*/ 
	m_DC.m_bPrinting = TRUE; 
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 创建数据库连接
//////////////////////////////////////////////////////////////////////////
BOOL CBKPrintSheetApp::CreateDataCon(CString strSQLIP,CString strUser,CString strPasword)
{
	HRESULT hr;		//记录操作结果
	HRESULT hr1;
	CString strSQL;
	
	GetPrivateProfileString("Switch","data","",sVersion.GetBuffer(100),100,"./cfg.ini");
	sVersion.ReleaseBuffer();
	if (sVersion==_T(""))
	{
		::WritePrivateProfileString("Switch","data","2ndDrivingTestSystem","./cfg.ini"); 
		sVersion=_T("2ndDrivingTestSystem");
		sVersionphoto=_T("2ndPhoto");
		kmms=80;
	}else if (sVersion==_T("2ndDrivingTestSystem"))
	{
		sVersionphoto=_T("2ndPhoto");
		kmms=80;
	}else if (sVersion==_T("3rdDrivingTestSystem"))
	{
		sVersionphoto=_T("3rdPhoto");
		kmms=90;
	}
	try
	{		
		hr = m_pConnection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			strSQL.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",strSQLIP,sVersion,strUser,strPasword);
			m_pConnection->Open((_bstr_t)strSQL,"","",adModeUnknown); 
		}
		hr1=m_pConn.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr1))
		{
			strSQL.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",strSQLIP,sVersionphoto,strUser,strPasword);
			m_pConn->Open((_bstr_t)strSQL,"","",adModeUnknown);
		}
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("链接SQL数据库错误!\n%s",(LPSTR)e.Description());
		AfxMessageBox(strErr);
		return FALSE;
	}	
	return TRUE;	
}

void CBKPrintSheetApp::SetDcText(LPCSTR str,CRect rect,int line,UINT uFormat)
{
	CRect re;
	re=rect;
	re.top+=LineHeight*line;
	re.bottom+=LineHeight*line;

	m_DC.DrawText(str,&re,uFormat);
	
}

void CBKPrintSheetApp::DrawXLine(int line)
{	
	m_DC.MoveTo(DCRectT.left+2,DCRectT.top +line*LineHeight);
	m_DC.LineTo(DCRectT.right-2,DCRectT.top + line*LineHeight);
}

void CBKPrintSheetApp::DrawYLine(int x1, int sLine, int eLine)
{

	m_DC.MoveTo(x1,DCRectT.top +sLine*LineHeight);
	m_DC.LineTo(x1,DCRectT.top + eLine*LineHeight);
}
void CBKPrintSheetApp::GetKFX(CStringArray &dest,CString zkzm,int kscs,int drcs)
{
	dest.RemoveAll();	
	_RecordsetPtr pRecordset;
	_variant_t vat;
	CString strCtime,gawdm,kflx,kcfs,xm,temp;
	int ierrxm=0;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	/* */
		strSQL.Format("Set ARITHABORT ON;select CONVERT(varchar(30),dbo.dateDecode(er.出错时间), 24) as 出错时间,ed.公安网代码,dbo.charDecode(ed.扣分类型) as 扣分类型,dbo.intDecode(ed.扣除分数) as 扣除分数 \
		from ErrorRecords er,ErrorData ed where 准考证明编号=dbo.charEncode('%s') and er.错误编号=ed.错误编号 and er.考试次数='%d' \
		and 当日次数='%d' order by 出错时间 asc ",zkzm,kscs,drcs);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnection, true),adOpenDynamic, adLockOptimistic, adCmdText);
		
		if(pRecordset != NULL)
		{
			while(!pRecordset->adoEOF)
			{
				vat =pRecordset->GetCollect("扣除分数");
				if (vat.vt != VT_NULL)
				{
					kcfs = (LPCSTR)_bstr_t(vat);
				}else{kcfs=_T("");}
				if (strcmp(kcfs,"0")!=0)
				{
					if (ierrxm ==0)
					{
						xm= kflx;
					}
					vat =pRecordset->GetCollect("出错时间");
					if (vat.vt != VT_NULL)
					{
						strCtime = (LPCSTR)_bstr_t(vat);
					}else{strCtime=_T("");}
					vat =pRecordset->GetCollect("公安网代码");
					if (vat.vt != VT_NULL)
					{
						gawdm = (LPCSTR)_bstr_t(vat);
					}else{gawdm=_T("");}
					vat =pRecordset->GetCollect("扣分类型");
					if (vat.vt != VT_NULL)
					{
						kflx = (LPCSTR)_bstr_t(vat);
					}else{kflx=_T("");}
					if (gawdm =="10210" || gawdm =="10101" || gawdm =="10205" || gawdm =="10103")
					{
						xm =_T("");
					}
					if (xm.Left(4)=="进入")
					{
						xm =xm.Mid(4);
					}
					if (xm.Right(4)=="考试")
					{
						xm =xm.Left(xm.GetLength()-4);
					}
					temp.Format("%s %s %s 扣%s分\n",strCtime,gawdm,kflx,kcfs);
					TRACE(temp);
					dest.Add(temp);
					ierrxm =1;
				}
				else
				{
					vat =pRecordset->GetCollect("扣分类型");
					if (vat.vt != VT_NULL)
					{
						kflx = (LPCSTR)_bstr_t(vat);
					}else{kflx=_T("");}
					ierrxm =0;
				}				
				pRecordset->MoveNext(); 
			}
		}		
		pRecordset->Close();
		pRecordset.Release();			
	}
	catch (_com_error e) 
	{
		temp.Format("读数据库出错!!,%s",e.Description());
	}
	
}
/*******************************************************************************
函数: ReadGYPhotoFromDB
功能: 获取数据库的照片(右上角的一寸照片+右上角的登录抓拍照片)
作者: 黄康
日期: 2012-09-12
参数: 
		CDC *pDC              绘图对象
        CString sCard         考生的准考证明编号
		CRect *rc1            一寸照片的打印方位
		CRect *rc2			  门禁照片的打印方位
*******************************************************************************/
BOOL CBKPrintSheetApp::ReadGYPhotoFromDB(CDC *pDC, CString sCard, CRect *rc1, CRect *rc2)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
   
	CString temp;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		strSQL.Format("Set ARITHABORT ON;SELECT 照片,门禁照片 FROM StudentPhoto WHERE 准考证明编号 = dbo.charEncode('%s')",sCard);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConnection, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
     
		if(pRecordset->RecordCount != 1)
		{
			return FALSE;
		}

		char* pbuf = NULL;
		long lDataSize;
		
		//公安网照片
		lDataSize = pRecordset->GetFields()->GetItem("照片")->ActualSize; //数据库中图像数据长度
		if (lDataSize > 0)
		{	
			varBLOB = pRecordset->GetFields()->GetItem("照片")->GetChunk(lDataSize);
			m_pBMPBuffer = new char[lDataSize+1];
			
			if (varBLOB.vt == (VT_ARRAY|VT_UI1))
			{
				SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
				memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer

				SafeArrayUnaccessData(varBLOB.parray);

				//******************************************************//
				IStream* pStm;
				LONGLONG cb = lDataSize;
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
				LPVOID pvData;
				if (hGlobal != NULL)
				{
					pvData = GlobalLock(hGlobal);
					memcpy(pvData, m_pBMPBuffer, cb);
					GlobalUnlock(hGlobal);
					CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
				}
				else
				{
					WriteLogPPTXT("打印照片编号","日志","读取报名照片信息错误");
					return FALSE;
				}
				
				IPicture *pImg = NULL;    
				if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
				{
					OLE_XSIZE_HIMETRIC hmWidth;
					OLE_YSIZE_HIMETRIC hmHeight;
					
					pImg->get_Width(&hmWidth);
					pImg->get_Height(&hmHeight);
					
					pImg->Render(*pDC,rc1->left,rc1->top,rc1->right-rc1->left,rc1->bottom-rc1->top,0,hmHeight,hmWidth,-hmHeight,NULL);
				}
				else
				{
					AfxMessageBox("Error Loading Picture From Stream!");
					return FALSE;
				}

				pStm->Release ();
				pImg->Release();
				GlobalFree(hGlobal); 
				if(m_pBMPBuffer)
				{
					delete m_pBMPBuffer;
					m_pBMPBuffer = NULL;
				}
			}
		}	

		////////*************门禁照片**************////////////
		lDataSize = pRecordset->GetFields()->GetItem("门禁照片")->ActualSize; //数据库中图像数据长度
		if (lDataSize > 0)
		{	
			varBLOB = pRecordset->GetFields()->GetItem("门禁照片")->GetChunk(lDataSize);
			m_pBMPBuffer = new char[lDataSize+1];
			
			if (varBLOB.vt == (VT_ARRAY|VT_UI1))
			{
				SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
				memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer

				SafeArrayUnaccessData(varBLOB.parray);

				//******************************************************//
				IStream* pStm;
				LONGLONG cb = lDataSize;
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
				LPVOID pvData;
				if (hGlobal != NULL)
				{
					pvData = GlobalLock(hGlobal);
					memcpy(pvData, m_pBMPBuffer, cb);
					GlobalUnlock(hGlobal);
					CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
				}
				else
				{
					WriteLogPPTXT("打印照片编号","日志","读取门禁照片信息错误");
					return FALSE;
				}
				
				IPicture *pImg = NULL;    
				if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
				{
					OLE_XSIZE_HIMETRIC hmWidth;
					OLE_YSIZE_HIMETRIC hmHeight;
					
					pImg->get_Width(&hmWidth);
					pImg->get_Height(&hmHeight);
					
					pImg->Render(*pDC,rc2->left,rc2->top,rc2->right-rc2->left,rc2->bottom-rc2->top,0,hmHeight,hmWidth,-hmHeight,NULL);
				}
				else
				{
					AfxMessageBox("Error Loading Picture From Stream!");
					return FALSE;
				}

				pStm->Release ();
				pImg->Release();
				GlobalFree(hGlobal); 
				if(m_pBMPBuffer)
				{
					delete m_pBMPBuffer;
					m_pBMPBuffer = NULL;
				}
			}
		}
	
		pRecordset->Close();
		pRecordset.Release();
	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		WriteLogPPTXT("打印照片编号","日志","读取一寸照片信息错误catch");
		return FALSE;
	}
	return TRUE;
}

BOOL CBKPrintSheetApp::ReadXMPhotoFromDB(CDC *pDC, CString sZkzm, int iDrcs, int iKscs, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr m_pRecordset;
	_variant_t varBLOB;
	try
	{
// 		CString temp;
// 		temp.Format("zkzm%s,idrcs=%d,ikscs=%d",sZkzm,iDrcs,iKscs);
// 		AfxMessageBox(temp);
		if (m_pConn == NULL) return FALSE;
		m_pConn->CursorLocation =adUseClient;		
		m_pRecordset.CreateInstance("ADODB.Recordset");
		
		// 初始化命令对象
		_CommandPtr m_pCommand;
		m_pCommand.CreateInstance("ADODB.Command");	
		m_pCommand->ActiveConnection = m_pConn;
		m_pCommand->CommandType = adCmdStoredProc;
		m_pCommand->CommandText = _bstr_t(_T("XMPhotoCount"));
		
		//创建参数
		m_pCommand->Parameters->Refresh(); 
		m_pCommand->Parameters->Item[_variant_t(_bstr_t("@Zkzmbh"))]->Value =_variant_t(sZkzm);
		m_pCommand->Parameters->Item[_variant_t(_bstr_t("@kscs"))]->Value =_variant_t((long)iKscs);
		m_pCommand->Parameters->Item[_variant_t(_bstr_t("@drcs"))]->Value =_variant_t((long)iDrcs);
		//执行存储过程
		m_pRecordset =  m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
		int phonum=0;
		if(m_pRecordset != NULL && (!m_pRecordset->adoEOF))
		{
			m_pRecordset->MoveFirst();
			while(!m_pRecordset->adoEOF)
			{
				char* pbuf = NULL;
				long lDataSize = m_pRecordset->GetFields()->GetItem("照片")->ActualSize; //数据库中图像数据长度
				if (lDataSize > 0)
				{
					varBLOB = m_pRecordset->GetFields()->GetItem("照片")->GetChunk(lDataSize);
					m_pBMPBuffer = new char[lDataSize+1];
					if (varBLOB.vt == (VT_ARRAY|VT_UI1))
					{
						SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
						memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
						SafeArrayUnaccessData(varBLOB.parray);
						
						//******************************************************//
						IStream* pStm;
						LONGLONG cb = lDataSize;
						HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
						LPVOID pvData;
						if (hGlobal != NULL)
						{
							pvData = GlobalLock(hGlobal);
							memcpy(pvData, m_pBMPBuffer, cb);
							GlobalUnlock(hGlobal);
							CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
						}
						else
						{
							return FALSE;
						}
						
						IPicture *pImg = NULL;    
						if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
						{
							OLE_XSIZE_HIMETRIC hmWidth;
							OLE_YSIZE_HIMETRIC hmHeight;
							
							pImg->get_Width(&hmWidth);
							pImg->get_Height(&hmHeight);
							
							pImg->Render(*pDC,rc->left+(phonum*(rc->Width()/3)),rc->top,rc->Width()/3,rc->Height(),0,hmHeight,hmWidth,-hmHeight,NULL);
							phonum+=1;
						}
						else
						{
							//AfxMessageBox("打印照片失败!");
							return FALSE;
						}
						pStm->Release ();
						pImg->Release();
						GlobalFree(hGlobal); 
						if(m_pBMPBuffer)
						{
							delete m_pBMPBuffer;
							m_pBMPBuffer = NULL;
						}
					}//
				}//lDataSize > 0
				m_pRecordset->MoveNext();
				if (m_pRecordset->adoEOF && phonum!=3)//小于三张用前一张
				{
					m_pRecordset->MovePrevious();
				}
			}//while
		}//if
		m_pCommand.Release();
		m_pCommand.Detach();
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("调用存储过程XMPhotoCount失败!\r\n原因:%s",e.ErrorMessage());
		//AfxMessageBox(log);
	}
	return TRUE;
}

/************************************************************************/
/* 考官签名照、考生签名照                                               */
/************************************************************************/
BOOL CBKPrintSheetApp::ReadQZPhoto(CDC *pDC, CString sCard, CString sSir, int iDRCS,CRect *rc1, CRect *rc2, CRect *rc3,CRect *rc4)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
	
	

	try
	{

		if(m_pConnection ==NULL) return FALSE;
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;
		strSQL.Format("select 签名照片 from sirinfo where 考试员姓名 = '%s' and 签名照片 is not null ",sSir);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnection, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);

		char* pbuf = NULL;
		long lDataSize;
		if (pRecordset->RecordCount == 1)
		{			
			////////*************考官签名照**************////////////
			lDataSize = pRecordset->GetFields()->GetItem("签名照片")->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{	
				varBLOB = pRecordset->GetFields()->GetItem("签名照片")->GetChunk(lDataSize);
				m_pBMPBuffer = new char[lDataSize+1];
				
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					SafeArrayUnaccessData(varBLOB.parray);				
					
					IStream* pStm;
					LONGLONG cb = lDataSize;
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
					LPVOID pvData;
					if (hGlobal != NULL)
					{
						pvData = GlobalLock(hGlobal);
						memcpy(pvData, m_pBMPBuffer, cb);
						GlobalUnlock(hGlobal);
						CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
					}
					else
					{
						WriteLogPPTXT("打印照片编号","日志","读取考试员签名照片信息错误");
						return FALSE;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						
						int Bprint=theApp.loadconfig();
						if (Bprint==0)
						{
							
							pImg->Render(*pDC,rc1->left,rc1->top,(rc1->right - rc1->left),rc1->bottom - rc1->top,0,hmHeight,hmWidth,-hmHeight,NULL);
							
							if (iDRCS == 2)
								pImg->Render(*pDC,rc3->left,rc3->top,(rc3->right - rc3->left),rc3->bottom - rc3->top,0,hmHeight,hmWidth,-hmHeight,NULL);
					
						}
					}
					else
					{
						AfxMessageBox("Error Loading Picture From Stream!");
						return FALSE;
					}
					pStm->Release ();
					pImg->Release();
					GlobalFree(hGlobal); 
					if(m_pBMPBuffer)
					{
						delete m_pBMPBuffer;
						m_pBMPBuffer = NULL;
					}
				}
			}
		}
	

		pRecordset->Close();
		strSQL.Format("Set ARITHABORT ON;SELECT 签名照片 FROM StudentPhoto WHERE 准考证明编号 = dbo.charEncode('%s') and 签名照片 is not null",sCard);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnection, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		if(pRecordset->RecordCount == 1)
		{
			////////*************考生签名照片**************////////////
			lDataSize = pRecordset->GetFields()->GetItem("签名照片")->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{	
				varBLOB = pRecordset->GetFields()->GetItem("签名照片")->GetChunk(lDataSize);
				m_pBMPBuffer = new char[lDataSize+1];
				
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					SafeArrayUnaccessData(varBLOB.parray);				
					
					IStream* pStm;
					LONGLONG cb = lDataSize;
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
					LPVOID pvData;
					if (hGlobal != NULL)
					{
						pvData = GlobalLock(hGlobal);
						memcpy(pvData, m_pBMPBuffer, cb);
						GlobalUnlock(hGlobal);
						CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
					}
					else
					{
						WriteLogPPTXT("打印照片编号","日志","读取学员签名照片信息错误");
						return FALSE;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						pImg->Render(*pDC,rc2->left,rc2->top, rc2->right - rc2->left,rc2->bottom - rc2->top,0,hmHeight,hmWidth,-hmHeight,NULL);
						if (iDRCS == 2)
							pImg->Render(*pDC,rc4->left,rc4->top,(rc4->right - rc4->left),rc4->bottom - rc4->top,0,hmHeight,hmWidth,-hmHeight,NULL);
						
					}
					else
					{
						//AfxMessageBox("Error Loading Picture From Stream!");
						return FALSE;
					}
					pStm->Release ();
					pImg->Release();
					GlobalFree(hGlobal); 
					if(m_pBMPBuffer)
					{
						delete m_pBMPBuffer;
						m_pBMPBuffer = NULL;
					}
				}
			}
		}
		pRecordset->Close();
		pRecordset.Release();
	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		CString strErr;
		WriteLogPPTXT("打印照片编号","日志","读取签名照片信息错误catch");
		AfxMessageBox(strErr);
 		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 释放连接
//////////////////////////////////////////////////////////////////////////
void CBKPrintSheetApp::DoClearPrintDC()
{
	if (m_pConn->GetState() == adStateOpen)
	{
		m_pConn->Close();
	}
	if (m_pConn)
	{
		m_pConn.Release();
		m_pConn = NULL;
	}
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	m_DC.DeleteDC();
}


void CBKPrintSheetApp::TwoDimQR(CDC *pDC ,CString strEncodeData, CRect *rc)
{
	CQR_Encode* pQR_Encode = new CQR_Encode;
	int nLevel = 1;
	int nVersion = 0;
	BOOL bAutoExtent = 1;
	//	BOOL bAutoExtent = 0;
	int nMaskingNo = -1;
	
	int i , j;
	
	CDC* pSymbleDC;
	CBitmap* pSymbleBitmap;
	CBitmap* pOldBitmap;
	int nSymbleSize;
	
	BOOL bDataEncoded = pQR_Encode->EncodeData(nLevel, nVersion, bAutoExtent, nMaskingNo, strEncodeData);
	
	if (bDataEncoded)
	{	
		nSymbleSize = pQR_Encode->m_nSymbleSize + (QR_MARGIN * 2);
		
		pSymbleBitmap = new CBitmap;
		pSymbleBitmap->CreateBitmap(nSymbleSize, nSymbleSize, 1, 1, NULL);
		
		pSymbleDC = new CDC;
		pSymbleDC->CreateCompatibleDC(NULL);
		
		pOldBitmap = pSymbleDC->SelectObject(pSymbleBitmap);
		
		pSymbleDC->PatBlt(0, 0, nSymbleSize, nSymbleSize, WHITENESS);
		
		for (i = 0; i < pQR_Encode->m_nSymbleSize; ++i)
		{
			for (j = 0; j < pQR_Encode->m_nSymbleSize; ++j)
			{
				if (pQR_Encode->m_byModuleData[i][j])
				{
					pSymbleDC->SetPixel(i + QR_MARGIN, j + QR_MARGIN, RGB(0, 0, 0)); 
				}
			}
		}
		
		pDC->StretchBlt(rc->left, rc->top, rc->Width(), rc->Height(), pSymbleDC, 0, 0, nSymbleSize, nSymbleSize, SRCCOPY);
		
		pSymbleDC->SelectObject(pOldBitmap);
		
		delete pSymbleBitmap;
		delete pSymbleDC;
	}
	delete pQR_Encode;
}



// BKPrintSheet.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BKPrintSheet.h"
#include "ADO.h"

#include <string>
using namespace std; 
#include <windows.h> 
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

HBITMAP CBKPrintSheetApp::CopyDCToBitmap(HDC hScrDC, LPRECT lpRect)   
{   
	HDC  hMemDC;               
	//   屏幕和内存设备描述表   
	HBITMAP hBitmap,hOldBitmap;         
	//   位图句柄   
	int  nX, nY, nX2, nY2;               
	//   选定区域坐标   
	int  nWidth, nHeight;               
	//   位图宽度和高度   
	
	//   确保选定区域不为空矩形   
	if   (IsRectEmpty(lpRect))   
		return   NULL;   
	
	//   获得选定区域坐标   
	nX   = lpRect->left;   
	nY   = lpRect->top;   
	nX2  = lpRect->right;   
	nY2  = lpRect->bottom;   
	nWidth  = nX2 - nX;   
	nHeight = nY2 - nY;   
	//   为屏幕设备描述表创建兼容的内存设备描述表   
	hMemDC  = CreateCompatibleDC(hScrDC);   
	//   创建一个与屏幕设备描述表兼容的位图,函数需要与CreateCompatibleDC配合使用   
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);   
	//   把新位图选到内存设备描述表中   
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);   
	//   把屏幕设备描述表拷贝到内存设备描述表中
	//   StretchBlt函数从源矩形中复制一个位图到目标矩形，必要时按目前目标设备设置的模式进行图像的拉伸或压缩。
	StretchBlt(hMemDC,0,0,nWidth,nHeight,  hScrDC,nX,nY,nWidth,nHeight,SRCCOPY);  // SRCCOPY
	//BitBlt(hMemDC,   0,   0,   nWidth,   nHeight,hScrDC,   nX,   nY,   SRCCOPY);   
	//   得到屏幕位图的句柄     
	
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);   
	//   清除     
	
	//DeleteDC(hScrDC);   
	DeleteDC(hMemDC);   
	DeleteObject(hOldBitmap);   
	//   返回位图句柄   
	return   hBitmap;   
}   

BOOL CBKPrintSheetApp::SaveBmp(HBITMAP hBitmap, CString FileName)   
{   
	if (FileName.IsEmpty())
	{
		FileName=_T("BKVideoBMPBack.bmp");
	}
	HDC   hDC;   
	//当前分辨率下每象素所占字节数   
	int   iBits;   
	//位图中每象素所占字节数   
	WORD  wBitCount;   
	//定义调色板大小，位图中像素字节大小，位图文件大小，写入文件字节数
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;     
	//位图属性结构     
	BITMAP   Bitmap;       
	//位图文件头结构   
	BITMAPFILEHEADER   bmfHdr;       
	//位图信息头结构     
	BITMAPINFOHEADER   bi;       
	//指向位图信息头结构       
	LPBITMAPINFOHEADER lpbi;       
	//定义文件，分配内存句柄，调色板句柄     
	HANDLE  fh, hDib, hPal, hOldPal=NULL;     
    
	//计算位图文件每个像素所占字节数     
	hDC   = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);     
	DeleteDC(hDC);     
	if (iBits <= 1)wBitCount = 1;     
	else if (iBits <= 4) wBitCount = 4;     
	else if (iBits <= 8) wBitCount = 8;     
	else wBitCount = 24;     
    
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);   
	bi.biSize = sizeof(BITMAPINFOHEADER);   
	bi.biWidth = Bitmap.bmWidth;   
	bi.biHeight = Bitmap.bmHeight;   
	bi.biPlanes = 1;   
	bi.biBitCount = wBitCount;   
	bi.biCompression = BI_RGB;   
	bi.biSizeImage = 0;   
	bi.biXPelsPerMeter = 0;   
	bi.biYPelsPerMeter = 0;   
	bi.biClrImportant = 0;   
	bi.biClrUsed = 0;   
    
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;   
//    dwBmBitsSize =(Bitmap.bmWidth * Bitmap.bmHeight * wBitCount) /8;//zsz
	//   为位图内容分配内存     
	hDib  = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     
	lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;     
    
	//   处理调色板       
	hPal  = GetStockObject(DEFAULT_PALETTE);     
	if   (hPal)     
	{     
		hDC = ::GetDC(NULL);     
		//hDC   =   m_pDc->GetSafeHdc();   
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	//   获取该调色板下新的像素值     
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
		+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     
    
	//   恢复调色板       
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}    
    
	//   创建位图文件       
	fh = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE;
    
	//   设置位图文件头     
	bmfHdr.bfType = 0x4D42;   //   ;BM;
	dwDIBSize   = 940856;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//   写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//   写入位图文件其余内容
	dwDIBSize=dwDIBSize-14;
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//   清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
    
	return TRUE;
}

extern "C" DllExport BOOL PrintSheetEx(LPTSTR lpExamNo, int ikscs,LPTSTR strSQLIP,LPTSTR strUser,LPTSTR strPassword) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);	
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP1));	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
//////////////////////////////////////////////////////////////////////////
	CString strExamNo;
	strExamNo.Format("%s",lpExamNo);	
	CoInitialize(NULL);
	if (!theApp.CreateDataCon(strSQLIP,strUser,strPassword))
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	CString strName,strZkzm,strSfzm,strBkcx,strJX,strYWLX,strKsrq,strYycs,strksy1,strdrcs,strYKRQ;
	strYycs.Format("%d",ikscs);
	int iCount=1;
	CString strTime1,strTime2,strCJ1,strCJ2,szSQL,strTimeKS,strTimeJS;
	try
	{
		_variant_t vat;
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		//导入资料
		szSQL.Format("select st.姓名,st.准考证明编号,st.身份证明编号,st.考试车型,sc.驾校名称,st.考试原因,st.考试日期,st.考试次数,st.当日次数, \
			st.考试员1 from studentinfo st,SchoolInfo sc where 准考证明编号='%s' and st.代理人=sc.驾校编号", strExamNo);
		TRACE(szSQL);
		pRecordset->Open(_variant_t(szSQL), _variant_t((IDispatch*)theApp.m_pConnection, true),
			adOpenDynamic, adLockOptimistic, adCmdText);//打开到数据库连接的记录
		if (0==pRecordset->RecordCount )
		{
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
		if (theApp.SQLorOracle==1)
		{
			szSQL.Format("select 考试时间,开始时间,考试成绩-RIGHT(准考证明编号,2)-45 as 成绩,当日次数 from \
			ExamRecord where 准考证明编号='%s' and 考试次数='%d' ",strExamNo,ikscs);
		} 
		else
		{
			szSQL.Format("select 考试时间,开始时间,考试成绩-substr(准考证明编号,-2,2)-45 as 成绩,当日次数 from \
			ExamRecord where 准考证明编号='%s' and 考试次数='%d' ",strExamNo,ikscs);
		}
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
						COleDateTime date_value1;
						date_value1 = vat.date;
						strTimeKS=date_value1.Format("%H:%M:%S");
						vat =pRecordset->GetCollect("考试时间");
						COleDateTime date_value2;
						date_value2 = vat.date;
						strTimeJS =date_value2.Format("%H:%M:%S");
						if (strTimeKS!=_T(""))
							strTime1=strTimeKS+"-"+strTimeJS;
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
						COleDateTime date_value1;
						date_value1 = vat.date;
						strTimeKS =date_value1.Format("%H:%M:%S");
						vat =pRecordset->GetCollect("考试时间");
						COleDateTime date_value2;
						date_value2 = vat.date;
						strTimeJS =date_value2.Format("%H:%M:%S");
						if (strTimeKS!=_T(""))
							strTime2=strTimeKS+"-"+strTimeJS;
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
		/*
		CString strBDzmkm;
		szSQL.Format("select 准考证明编号 from studentinfo where 预考日期='%s' order by  考试日期 desc",strYKRQ);
		pRecordset->Open(_variant_t(szSQL), _variant_t((IDispatch*)theApp.m_pConnection, true),
			adOpenDynamic, adLockOptimistic, adCmdText);//
		AfxMessageBox(szSQL);
		if(pRecordset != NULL)
		{
			while(!pRecordset->adoEOF)
			{
				vat =pRecordset->GetCollect("准考证明编号");
				if (vat.vt != VT_NULL)
				{
					 strBDzmkm= (LPCSTR)_bstr_t(vat);
					 if (strcmp(strBDzmkm,strZkzm)!=0)
					 {
						continue;
						iCount++;

					 }
					 else
						break;
				}
				pRecordset->MoveNext();
			}
		}
		pRecordset->Close();
		*/
		pRecordset.Release();
	}
	catch (_com_error e)
	{
		CString sErrInfo;
		sErrInfo.Format("加载打印数据失败!\n%s %s考试数据不完整!",strName,strExamNo);
		AfxMessageBox(sErrInfo);
		AfxMessageBox(e.Description());
		return FALSE;
	}

	//GetKFX(CStringArray &dest,CString zkzm,int kscs,int drcs);
	int idrcs=_ttoi(strdrcs);
// 	strName=_T("张三四");
// 	strZkzm=_T("520100863085");
// 	strSfzm=_T("42222317909314907");
// 	strBkcx=_T("C1");
// 	strJX=_T("贵阳时发驾校朝秦暮");
// 	strYWLX=_T("初领");
// 	strKsrq=_T("2014年03月28日");
// 	strYycs=_T("1");
// 	strTime1=_T("2014年04月01日 09:09:09");
// 	strTime2=_T("2014年04月01日 09:09:09");
// 	strCJ1=_T("100 (合格)");
// 	strCJ2=_T("90 (不合格)");
	//////////////////////////////////////////////////////////////////////////
	//theApp.m_DC.SetMapMode(MM_LOMETRIC);
	int   nHorRes   =466; //  GetDeviceCaps(theApp.m_DC, HORZRES); 
	int   nVerRes   =672;//   GetDeviceCaps(theApp.m_DC, VERTRES);
	CRect DCRect(14,8,nHorRes-10,nVerRes-8);//最大的
	theApp.DCRectT=DCRect;
	theApp.LineHeight = DCRect.Height()/26;
	//theApp.m_DC.Rectangle(DCRect);
	//////////////////////////////////////////////////////////////////////////
	//
	CPen pen,* oldpen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	oldpen = ImageDC.SelectObject(&pen);
	//////////////////////////////////////////////////////////////////////////
	CFont font1,font2,font3,*oldfont;
	font1.CreateFont(18, 0, 0, 0, 80, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "黑体");
	font2.CreateFont(10, 0, 0, 0, 40, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
				
	//font2 加粗
	font3.CreateFont(10, 0, 0, 0, 56, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	//////////////////////////////////////////////////////////////////////////
	oldfont = ImageDC.SelectObject(&font1);//大字体
	CString Cstrtemp;
	CRect TitleRect(DCRect.left,DCRect.top,DCRect.right,DCRect.top+theApp.LineHeight*2);
	//theApp.m_DC.Rectangle(TitleRect);	
	if (theApp.kmms==80)
	{
		Cstrtemp = _T("科目二考试成绩单");
	}else
		Cstrtemp = _T("科目三道路驾驶技能考试成绩单");
	ImageDC.DrawText(Cstrtemp,&TitleRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	oldfont = ImageDC.SelectObject(&font2);

	DCRect.top=TitleRect.bottom;
	CRect QRrc;
	QRrc.SetRect(TitleRect.right-TitleRect.Height(),TitleRect.top,TitleRect.right,TitleRect.bottom);
	//theApp.m_DC.Rectangle(QRrc);
	ImageDC.Rectangle(DCRect);
	for (int i=4;i<23;i++)
	{
		if ((i>8&&i<12) || (i>12&&i<16) || (i>18&&i<21))
		{
			continue;
		}
		theApp.DrawXLine(i,&ImageDC);
	}
	CRect r1,r2,r3,r4,r5,r6,rline;
	r1.SetRect(DCRect.left,DCRect.top,DCRect.right*0.15,DCRect.top+theApp.LineHeight);
	r2.SetRect(r1.right,r1.top,DCRect.right*0.38,r1.bottom);
	r3.SetRect(r2.right,r1.top,DCRect.right*0.53,r1.bottom);
	r4.SetRect(r3.right,r1.top,DCRect.right*0.7,r1.bottom);
	r5.SetRect(r4.right,r1.top,DCRect.right*0.85,r1.bottom);
	r6.SetRect(r5.right,r1.top,DCRect.right,r1.bottom);
	rline.SetRect(DCRect.left,DCRect.top,DCRect.right,DCRect.top+theApp.LineHeight);
	//////////////////////////////////////////////////////////////////////////
	CRect photo1,photo2;
	photo1.SetRect(r5.left,r5.top+theApp.LineHeight-2,r5.right,r5.bottom+theApp.LineHeight*3);
	photo2.SetRect(photo1.right,photo1.top,photo1.right+photo1.Width()-2,photo1.bottom);
	ImageDC.Rectangle(photo1);
	ImageDC.Rectangle(photo2);
	theApp.ReadGYPhotoFromDB(&ImageDC, strExamNo,&photo1,&photo2);
	//////////////////////////////////////////////////////////////////////////
	CRect XMPhoto1,XMPhoto2;
	XMPhoto1.SetRect(DCRect.left,DCRect.top+theApp.LineHeight*10,DCRect.right,DCRect.top+theApp.LineHeight*14);
	XMPhoto2.SetRect(DCRect.left,DCRect.top+theApp.LineHeight*20,DCRect.right,DCRect.top+theApp.LineHeight*24);
	// 	theApp.m_DC.Rectangle(XMPhoto1);
	// 	theApp.m_DC.Rectangle(XMPhoto2);
	theApp.ReadXMPhotoFromDB(&ImageDC,strExamNo,1,ikscs,&XMPhoto1);
	if (2==idrcs)
	{
		theApp.ReadXMPhotoFromDB(&ImageDC,strExamNo,2,ikscs,&XMPhoto2);
	}
	
	//////////////////////////////////////////////////////////////////////////
	CRect kfxrc;
	kfxrc.SetRect(r1.right+10,DCRect.top+theApp.LineHeight*6,DCRect.right,DCRect.top+theApp.LineHeight*7);
	CStringArray cakfx1,cakfx2;
	
	theApp.GetKFX(cakfx1,strExamNo,ikscs,1);
	for (int ci=0;ci<cakfx1.GetSize();ci++)
	{
		switch (ci)
		{
		case 0:
			theApp.SetDcText(cakfx1.GetAt(0),kfxrc,0,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);
			break;
		case 1:
			theApp.SetDcText(cakfx1.GetAt(1),kfxrc,1,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);
			break;
		case 2:
			theApp.SetDcText(cakfx1.GetAt(2),kfxrc,2,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);
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
				theApp.SetDcText(cakfx2.GetAt(0),kfxrc,10,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);
				break;
			case 1:
				theApp.SetDcText(cakfx2.GetAt(1),kfxrc,11,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);
				break;
			case 2:
				theApp.SetDcText(cakfx2.GetAt(2),kfxrc,12,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);
				break;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////

	theApp.DrawXLine(3,&ImageDC);
	theApp.DrawYLine(r1.right,2,6,&ImageDC);
	theApp.DrawYLine(r1.right,7,12,&ImageDC);
	theApp.DrawYLine(r1.right,17,22,&ImageDC);	
	theApp.DrawYLine(r2.right,2,6,&ImageDC);
	theApp.DrawYLine(r3.right,2,6,&ImageDC);
	theApp.DrawYLine(r4.right,2,6,&ImageDC);
	theApp.DrawYLine(r5.right,2,6,&ImageDC);
	oldfont = ImageDC.SelectObject(&font3);
	theApp.SetDcText("姓    名",r1,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("准考证号",r1,1,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("身份证号",r1,2,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("驾校简称",r1,3,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("考试时间",r1,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("扣分项",r1,7,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("考试员签名",r1,9,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	
	theApp.SetDcText("考试时间",r1,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("扣分项",r1,17,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("考试员签名",r1,19,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);

	theApp.SetDcText("考试日期",r3,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("报考车型",r3,1,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("业务类型",r3,2,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("预约次数",r3,3,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);

	//theApp.SetDcText("2014年03月28日",r4,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	theApp.SetDcText("报名照片",r5,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("门禁照片",r6,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	if (theApp.kmms==80)
	{
		theApp.SetDcText("科目二考试",rline,4,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	}else
		theApp.SetDcText("科目三道路驾驶技能考试",rline,4,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	
	if (theApp.kmms==80)
	{
		theApp.SetDcText("科目二补考",rline,14,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	} 
	else
	{
		theApp.SetDcText("科目三道路驾驶技能补考",rline,14,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	}
	//theApp.SetDcText("科目二考试",rline,4,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	//theApp.SetDcText("科目二补考",rline,14,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	CRect r7,r8,r9;
	r7.SetRect(r1.right,r1.top,rline.Width()/2,r1.bottom);
	r8.SetRect(r7.right,r7.top,r7.right+r1.Width(),r7.bottom);
	r9.SetRect(r8.right,r8.top,rline.right,r8.bottom);
	// 	theApp.m_DC.Rectangle(r7);
	// 	theApp.m_DC.Rectangle(r8);
	// 	theApp.m_DC.Rectangle(r9);
	theApp.DrawYLine(r7.right,7,8,&ImageDC);
	theApp.DrawYLine(r8.right,7,8,&ImageDC);
	theApp.DrawYLine(r7.right,11,12,&ImageDC);
	theApp.DrawYLine(r8.right,11,12,&ImageDC);
	theApp.DrawYLine(r7.right,17,18,&ImageDC);
	theApp.DrawYLine(r8.right,17,18,&ImageDC);
	theApp.DrawYLine(r7.right,21,22,&ImageDC);
	theApp.DrawYLine(r8.right,21,22,&ImageDC);

	theApp.SetDcText("考试成绩",r8,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("考生签名",r8,9,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("考试成绩",r8,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	theApp.SetDcText("考生签名",r8,19,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);
	oldfont = ImageDC.SelectObject(&font2);
	//////////////////////////////////////////////////////////////////////////
	//ReadQZPhoto(CDC *pDC, CString sCard, CString sSir, int iDRCS,CRect *rc1, CRect *rc2, CRect *rc3,CRect *rc4)
	CRect qzr1,qzr2,qzr3,qzr4;
	qzr1.SetRect(r7.left+20,r7.top-10+theApp.LineHeight*9,r7.left+20+(r7.Height()*2),r7.top+theApp.LineHeight*10);
	qzr3.SetRect(qzr1.left,qzr1.top+theApp.LineHeight*10,qzr1.right,qzr1.top+theApp.LineHeight*11+10);
	qzr2.SetRect(r9.left+20,qzr1.top-10,r9.left+24+(r9.Height()/3*4),qzr1.bottom);
	qzr4.SetRect(qzr2.left,qzr2.top+theApp.LineHeight*10+20,qzr2.right,qzr2.top+theApp.LineHeight*11+20);
	theApp.ReadQZPhoto(&ImageDC,strExamNo,strksy1,idrcs,&qzr1,&qzr2,&qzr3,&qzr4);
//////////////////////////////////////////////////////////////////////////
	
	theApp.DrawXLine(11,&ImageDC);
	theApp.DrawXLine(21,&ImageDC);
	
	theApp.SetDcText(" "+strName,r2,0,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);//姓名
 	theApp.SetDcText(" "+strZkzm,r2,1,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);//准考证明
 	theApp.SetDcText(" "+strSfzm,r2,2,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);//身份证明
 	theApp.SetDcText(" "+strJX,r2,3,DT_LEFT | DT_VCENTER | DT_SINGLELINE,&ImageDC);//驾校
	theApp.SetDcText(strKsrq,r4,0,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//考试日期
	theApp.SetDcText(strBkcx,r4,1,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//报考车型
	theApp.SetDcText(strYWLX,r4,2,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//业务类型
	theApp.SetDcText(strYycs,r4,3,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//预约次数

	theApp.SetDcText(strCJ1,r9,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//考试成绩1
	theApp.SetDcText(strCJ2,r9,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//考试成绩2
	theApp.SetDcText(strTime1,r7,5,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//考试时间1
	theApp.SetDcText(strTime2,r7,15,DT_CENTER | DT_VCENTER | DT_SINGLELINE,&ImageDC);//考试时间2

	//////////////////////////////////////////////////////////////////////////
// 	CString twostr;
// 	if (idrcs ==1)
// 	{
// 		twostr.Format("姓名:%s\n身份证:%s\n一次成绩:%s",strName,strSfzm,strCJ1);
// 	}
// 	else
// 	{
// 		twostr.Format("姓名:%s\n身份证:%s\n二次成绩:%s",strName,strSfzm,strCJ2);
// 	}
// 	theApp.TwoDimQR(&ImageDC,twostr,&QRrc);
	///////////////////条形码/////////////////
	///////////////////条形码/////////////////
	CFont fonttm;
	fonttm.CreateFont(120, 0, 0, 0, 200, 0, 0, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "IDAutomationHC39M");
	oldfont = ImageDC.SelectObject(&fonttm);
	CString sfzmbh;
	sfzmbh.Format("*%s*",strSfzm);	
	ImageDC.TextOut(DCRect.left*15.4,DCRect.top-160,sfzmbh);
	fonttm.DeleteObject();
	////////////////////////////////////
	pen.DeleteObject();
	oldpen->DeleteObject();
	font1.DeleteObject();
	font2.DeleteObject();
	font3.DeleteObject();
	oldfont->DeleteObject();

	ImageDC.TextOut(10,10,"预览");
	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,466,672);
	HBITMAP hBmp;
	hBmp=theApp.CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("skin.ui");
	theApp.SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();	
	ImageDC.DeleteDC();
	//////////////////////////////////////////////////////////////////////////
	theApp.DoClearPrintDC();
	CoUninitialize();
	return TRUE;

}


//////////////////////////////////////////////////////////////////////////
// 创建数据库连接
//////////////////////////////////////////////////////////////////////////
BOOL CBKPrintSheetApp::CreateDataCon(CString strSQLIP,CString strUser,CString strPasword)
{
	SQLorOracle=GetPrivateProfileInt("CONFIG","SQLorOracle",1,".\\cfg.ini");
	HRESULT hr;		//记录操作结果
	HRESULT hr1;
	CString strSQL,m_strInEdit;
	int sqlerror=0;
	CString sVersion,sVersionphoto;
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
		////////////////////////////////信息数据库//////////////////////////////////////////
		/*
		CString m_strInEdit;
		GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
		m_strInEdit.ReleaseBuffer();
		if (!m_strInEdit.IsEmpty())
		{
			int iLen =m_strInEdit.GetLength();
			
			for(int i=0;i<iLen;i++)
			{
				m_strInEdit.SetAt(i,m_strInEdit[i]-1);
			}
			m_strInEdit.MakeReverse();
			sqlerror=1;
		}
		*/
		if(SUCCEEDED(hr))
		{
			
			if (theApp.SQLorOracle==1)
			{
				m_strInEdit.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",strSQLIP,sVersion,strUser,strPasword);
			} 
			else
			{
				m_strInEdit.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)\
				(HOST=%s)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=BKZY)));User ID=sa;Password=%s",strSQLIP,strPasword);
			}
			
			m_pConnection->Open((_bstr_t)m_strInEdit,"","",adModeUnknown); 
		}
		hr1=m_pConn.CreateInstance("ADODB.Connection");
		////////////////////////////////信息数据库//////////////////////////////////////////
/*
		GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\ConfigPhoto.ini");
		m_strInEdit.ReleaseBuffer();
		if (!m_strInEdit.IsEmpty())
		{
			int iLen =m_strInEdit.GetLength();
			
			for(int i=0;i<iLen;i++)
			{
				m_strInEdit.SetAt(i,m_strInEdit[i]-1);
			}
			m_strInEdit.MakeReverse();
			sqlerror=2;
		}
		*/
		if(SUCCEEDED(hr1))
		{
			
			if (theApp.SQLorOracle==1)
			{
				m_strInEdit.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",strSQLIP,sVersionphoto,strUser,strPasword);
			} 
			else
			{
				m_strInEdit.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)\
				(HOST=%s)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=BKZY)));User ID=sapho;Password=%s",strSQLIP,strPasword);
			}
			
			m_pConn->Open((_bstr_t)m_strInEdit,"","",adModeUnknown);
		}
	}
	catch (_com_error e)
	{
		if (sqlerror==1)
		{
			CString errormessage;
			errormessage.Format("数据库提示,连接2ndDrivingTestSystem数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
			AfxMessageBox(errormessage);
//			AfxMessageBox(m_strInEdit);
			if (SQLorOracle==1)
			{
				ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW);//调用SQL工具
			}
			else
			{
				ShellExecute(NULL,"open",".\\oracle.exe","config.ini",NULL,SW_SHOW);//调用Oracle工具
			}
		}	
		else
		{
			CString errormessage;
			errormessage.Format("数据库提示,连接2ndPhoto照片数据库失败!,错误信息:%s,请先配置照片数据库连接",e.ErrorMessage());
			AfxMessageBox(errormessage);
//			AfxMessageBox(m_strInEdit);
			if (SQLorOracle==1)
			{
				ShellExecute(NULL,"open",".\\sqlLink.exe",".\\ConfigPhoto.ini",NULL,SW_SHOW);//调用SQL工具
			}
			else
			{
				ShellExecute(NULL,"open",".\\oracle.exe",".\\ConfigPhoto.ini",NULL,SW_SHOW);//调用Oracle工具
			}
			return FALSE;
		}

		return FALSE;
	}	
	return TRUE;	
}

void CBKPrintSheetApp::SetDcText(LPCSTR str,CRect rect,int line,UINT uFormat,CDC *pDC)
{
	CRect re;
	re=rect;
	re.top+=LineHeight*line;
	re.bottom+=LineHeight*line;
	pDC->DrawText(str,&re,uFormat);
	
}

void CBKPrintSheetApp::DrawXLine(int line,CDC *pDC)
{	
	pDC->MoveTo(DCRectT.left+2,DCRectT.top +line*LineHeight);
	pDC->LineTo(DCRectT.right-2,DCRectT.top + line*LineHeight);
}

void CBKPrintSheetApp::DrawYLine(int x1, int sLine, int eLine,CDC *pDC)
{

	pDC->MoveTo(x1,DCRectT.top +sLine*LineHeight);
	pDC->LineTo(x1,DCRectT.top + eLine*LineHeight);
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
		if (theApp.SQLorOracle==1)
		{
			strSQL.Format("select CONVERT(varchar(30),er.出错时间, 24) as 出错时间,ed.公安网代码,ed.扣分类型,ed.扣除分数 \
			  from ErrorRecords er,ErrorData ed where 准考证明编号='%s' and er.错误编号=ed.错误编号 and er.考试次数='%d' \
			and 当日次数='%d' order by 出错时间 asc ",zkzm,kscs,drcs);
		} 
		else
		{
			strSQL.Format("select to_char(er.出错时间,'HH12:MI:SS') as 出错时间,ed.公安网代码,ed.扣分类型,ed.扣除分数 \
			 from ErrorRecords er,ErrorData ed where 准考证明编号='%s' and er.错误编号=ed.错误编号 and er.考试次数='%d' \
			and 当日次数='%d' order by 出错时间 asc ",zkzm,kscs,drcs);
		}
		
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
					temp.Format("%s %s %s %s 扣%s分\n",strCtime,gawdm,xm,kflx,kcfs);
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
		strSQL.Format("SELECT 照片,门禁照片 FROM StudentPhoto WHERE 准考证明编号 = '%s'",sCard);
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
					AfxMessageBox("读取照片信息错误");
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
					AfxMessageBox("读取照片信息错误");
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
		CString strErr;
		strErr.Format("读取照片数据库失败!");
		AfxMessageBox(strErr);
 		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}




BOOL CBKPrintSheetApp::ReadXMPhotoFromDB(CDC *pDC, CString sExamNo, int Printcs, int iKscs, CRect *rc)
{
   	CStringArray sItem11;/*={"QB","SCZB","ZX","BGCD","LKZZ","LKYZ","LKZX","JJD","KBTC","DT","HC","CC","RXHD","TGXX","TGGGCZ","JSQY","ENDBUTTON"}*/
	sItem11.Add("QB");
	sItem11.Add("SCZB");
	sItem11.Add("ZX");
	sItem11.Add("BGCD");
	sItem11.Add("LKZZ");
	sItem11.Add("LKYZ");
	sItem11.Add("LKZX");
	sItem11.Add("JJD");//桩考  1	
	sItem11.Add("KBTC");//连续障碍  1	
	sItem11.Add("DT");//单边桥  1
	sItem11.Add("HC");//限宽门  1
	sItem11.Add("CC");//起伏路 1
	sItem11.Add("RXHD");//雨雾天  1
	sItem11.Add("TGXX");//湿滑路  1
	sItem11.Add("TGGGCZ");// 山区急转 1
	sItem11.Add("JSQY");//窄路掉头   1
	sItem11.Add("ENDBUTTON");//隧道  1
	/*
	sItem11.Add("p10001");
	sItem11.Add("p20500");
	sItem11.Add("p20300");
	sItem11.Add("p20700");
	sItem11.Add("p20400");
	sItem11.Add("p20600");
	sItem11.Add("p20100");
	sItem11.Add("p20200");//桩考  1	
	sItem11.Add("p20900");//连续障碍  1	
	sItem11.Add("p20500");//单边桥  1
	sItem11.Add("p20800");//限宽门  1
	sItem11.Add("p21000");//起伏路 1
	sItem11.Add("p21500");//雨雾天  1
	sItem11.Add("p21600");//湿滑路  1
	sItem11.Add("p21300");// 山区急转 1
	sItem11.Add("p21100");//窄路掉头   1
	sItem11.Add("p21400");//隧道  1
	sItem11.Add("p21200");//高速公路 1
	sItem11.Add("p21700");//紧急处置 1
	sItem11.Add("p10086");
*/
	_variant_t varBLOB;	
	int iShowPhoto=0;
	CString ItemPicture[20];
	int iHavePhoto=0;
	ADO m_AdoPhoto;
	m_AdoPhoto.OnInitADOConn(theApp.SQLorOracle);
	LPVOID m_pBMPBuffer;
	CString temp;
	try
	{
		
		CString strSQL;
		long lDataSize;
		strSQL.Format("SELECT * FROM XMPhoto WHERE (准考证明编号 = '%s') AND (考试次数 = %d) AND (当日次数 =%d)",sExamNo,iKscs,Printcs);
        m_AdoPhoto.OpenRecordset(strSQL);
        if(m_AdoPhoto.GetRecordCount(m_AdoPhoto.m_pRecordset)==0)
		{
			temp.Format("准考证明编号=%s考试次数=%d当日次数 =%d",sExamNo,iKscs,Printcs);
			return FALSE;
		}
		int len=sItem11.GetSize();
		for(int z=0;z<len;z++)
		{
			lDataSize = m_AdoPhoto.m_pRecordset->GetFields()->GetItem(sItem11[z].GetBuffer(MAX_PATH))->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{
			
				ItemPicture[iHavePhoto]=sItem11[z];
				iHavePhoto++;

			}
		}
		temp.Format("改学员存在共%d张",iHavePhoto);


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
			  ItemPicture[2]=ItemPicture[1];
              iHavePhoto=3;

		   }
		}
		char* pbuf = NULL;
	    variant_t varBLOB;
		for(z=0;z<iHavePhoto;z++)
		{	
		    lDataSize = m_AdoPhoto.m_pRecordset->GetFields()->GetItem(ItemPicture[z].GetBuffer(0))->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{
				varBLOB =m_AdoPhoto.m_pRecordset->GetFields()->GetItem(ItemPicture[z].GetBuffer(0))->GetChunk(lDataSize);
				m_pBMPBuffer = new char[lDataSize+1];
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
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
					//	AfxMessageBox("读取照片信息错误");
					//	return FALSE;
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
					//	pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
						pImg->Render(*pDC,rc->left+rc->Width()*(iShowPhoto-1)/3,rc->top,rc->Width()/3,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
						temp.Format("照片尺寸:上 %d 下 %d 左 %d 右 %d ",rc->left+rc->Width()*(iShowPhoto-1)/3,rc->top,rc->Width()/3,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight);
						if(iShowPhoto>=3)
						{
							break;
						}

					}
					else
					{
						//AfxMessageBox("打印照片失败!");
						//return FALSE;
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
			}else
			{
				CString strerror;
				strerror.Format("获取复制照片字段%s失败!",ItemPicture[z]);
				AfxMessageBox(strerror);
			}
		}

		
		m_AdoPhoto.CloseRecordset();
		m_AdoPhoto.CloseConn();
		
		
	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		CString strErr;
		strErr.Format("读取项目照片数据库失败!");
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}


/*
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
							//AfxMessageBox("读取照片信息错误");
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
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("调用存储过程XMPhotoCount失败!\r\n原因:%s",e.ErrorMessage());
		//AfxMessageBox(log);
	}
	return TRUE;
}
*/

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
						AfxMessageBox("读取照片信息错误");
						return FALSE;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						
						pImg->Render(*pDC,rc1->left,rc1->top,(rc1->right - rc1->left),rc1->bottom - rc1->top,0,hmHeight,hmWidth,-hmHeight,NULL);
						
						if (iDRCS == 2)
							pImg->Render(*pDC,rc3->left,rc3->top,(rc3->right - rc3->left),rc3->bottom - rc3->top,0,hmHeight,hmWidth,-hmHeight,NULL);
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
		strSQL.Format("SELECT 签名照片 FROM StudentPhoto WHERE 准考证明编号 = '%s' and 签名照片 is not null",sCard);
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
						//AfxMessageBox("读取照片信息错误");
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
		strErr.Format("读取照片数据库失败!");
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

BOOL CBKPrintSheetApp::InitApplication() 
{
	// TODO: Add your specialized code here and/or call the base class
//	isPrintOK=TRUE;
	return CWinApp::InitApplication();
}

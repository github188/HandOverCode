// CBKDeSkin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKDeSkin.h"

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
// CCBKDeSkinApp

BEGIN_MESSAGE_MAP(CCBKDeSkinApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKDeSkinApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKDeSkinApp construction

CCBKDeSkinApp::CCBKDeSkinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKDeSkinApp object

CCBKDeSkinApp theApp;
//	theApp.DrawImage17C51(isave);
//	theApp.DrawImage17C52(isave,4);
// 	theApp.DrawImage17C53(isave,0);
//  theApp.DrawImage17C55(isave,7);
//  theApp.DrawImage17C56(isave,2);
//	theApp.DrawImageBK();
//savetype 1 开始 2项目开始 3扣分  5项目结束 6结束
//ntype
extern "C" __declspec(dllexport) BOOL DrawImage(UINT savetype,UINT isave,UINT ntype)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	theApp.DrawImageBK();
// 	return FALSE;
	switch (savetype)
	{
	case 1://开始考试
		theApp.DrawImage17C51(isave);
		break;
	case 2://项目开始
		theApp.DrawImage17C52(isave,ntype);
		break;
	case 3://扣分
		theApp.DrawImage17C53(isave,ntype);
		break;
	case 5://项目结束
		theApp.DrawImage17C55(isave,ntype);
		break;
	case 6://结束考试
		theApp.DrawImage17C56(isave,ntype);
		break;
	default :
		break;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
HBITMAP CCBKDeSkinApp::CopyDCToBitmap(HDC hScrDC, LPRECT lpRect)   
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

BOOL CCBKDeSkinApp::SaveBmp(HBITMAP hBitmap, CString FileName)   
{   
	if (FileName.IsEmpty())
	{
		FileName=_T("CCBKDeSkinApp.bmp");
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
	dwDIBSize   = 608312;//540728 256;
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

void CCBKDeSkinApp::DrawImage17C51(UINT isave)
{
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_BACK));	
	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
	//////////////////////////////////////////////////////////////////////////
	CDC m_dcBuf;
	m_dcBuf.CreateCompatibleDC(NULL);
	CString temppho,tempphomj;
	temppho.Format("%d.pho",isave);
	CBitmap m_bmpPho;//照片
	m_bmpPho.m_hObject = LoadImage(NULL,temppho.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (m_bmpPho.m_hObject!=NULL)
	{
		m_dcBuf.SelectObject(&m_bmpPho);
		ImageDC.BitBlt(12,40,118,146,&m_dcBuf,0,0,SRCCOPY);
	}
	tempphomj.Format("%d.phomj",isave);
	m_bmpPho.m_hObject = LoadImage(NULL,tempphomj.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (m_bmpPho.m_hObject!=NULL)
	{
		m_dcBuf.SelectObject(&m_bmpPho);
		ImageDC.BitBlt(496,30,118,146,&m_dcBuf,0,0,SRCCOPY);//6
		//ImageDC.StretchBlt(512,30,100,128,&m_dcBuf,0,0,118,146,SRCCOPY);
	}
	m_bmpPho.DeleteObject();
	m_dcBuf.DeleteDC();
	CFileFind FindDBFile;
	if (FindDBFile.FindFile(temppho))
	{
		CFile::Remove(temppho);
	}
	if (FindDBFile.FindFile(tempphomj))
	{
		CFile::Remove(tempphomj);
	}
	//////////////////////////////////////////////////////////////////////////
	CFont fontz;
	fontz.CreateFont(-MulDiv(16,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_DEMIBOLD,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	ImageDC.SelectObject(&fontz);
	ImageDC.SetBkMode(TRANSPARENT);
	ImageDC.SetTextColor(RGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////
	CRect textrc[12];
	textrc[0].SetRect(185,36,285,60);
	textrc[1].SetRect(185,62,285,86);
	textrc[2].SetRect(225,88,450,112);
	textrc[3].SetRect(225,114,450,138);
	textrc[4].SetRect(225,140,610,164);	
	//textrc[5].SetRect(335,4,450,28);//考试员
	textrc[5].SetRect(335,4,700,28);//考试员 20160803
	textrc[6].SetRect(335,36,450,60);
	textrc[7].SetRect(6,4,280,28);
	textrc[8].SetRect(335,62,450,86);//成绩	
	textrc[9].SetRect(225,166,610,190);//当前状态
	textrc[10].SetRect(98,202,230,226);//开始时间
	//////////////////////////////////////////////////////////////////////////
	CString Appname,keyName;
	Appname.Format("kch%d",isave);
	CString strtext;
	for (int i=0;i<10;i++)
	{
//  		ImageDC.Rectangle(textrc[i]);
//  		ImageDC.SetTextColor(RGB(255,0,0));
		keyName.Format("MSG%d",i);
		GetPrivateProfileString(Appname,keyName,"",strtext.GetBuffer(MAX_PATH),MAX_PATH,".\\allcard.msg");
		strtext.ReleaseBuffer();
		ImageDC.DrawText(strtext,&textrc[i],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	}
	CTime curTime =CTime::GetCurrentTime();
	CString Time = curTime.Format("%H:%M:%S");//时钟
	ImageDC.DrawText(Time,&textrc[10],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	//////////////////////////////////////////////////////////////////////////
	CString xmstr[7];
	xmstr[0].LoadString(IDS_STRDCRK);//倒车入库
	xmstr[1].LoadString(IDS_STRCFTC);//侧方停车
	xmstr[2].LoadString(IDS_STRDDTC);//坡道定点停车和起步
	xmstr[3].LoadString(IDS_STRQXXS);//曲线行驶
	xmstr[4].LoadString(IDS_STRZJZY);//直角转弯
	xmstr[5].LoadString(IDS_STRMNSD);//模拟隧道行驶
	xmstr[6].LoadString(IDS_STRYWT);//模拟雨（雾）天行驶
	int xmi;
	CRect xmstrrc;
	CFont fontz1;
	fontz1.CreateFont(-MulDiv(14,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_NORMAL,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	ImageDC.SelectObject(&fontz1);

	for(int k=0;k<7;k++)
	{
		xmi=xmlinei[k];
		xmstrrc.SetRect(616,xmi*35,702,xmi*35+35);
		ImageDC.DrawText(xmstr[k],&xmstrrc,DT_CENTER | DT_SINGLELINE | DT_VCENTER );
		
	}
	//////////////////////////////////////////////////////////////////////////
	fontz.DeleteObject();
	fontz1.DeleteObject();
	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,704,288);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("%d.dat",isave);
	SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();
	ImageDC.DeleteDC();
}

void CCBKDeSkinApp::DrawImage17C52(UINT isave,UINT xmcode)
{
	UINT xmi;
	CString xmstr;
	if (xmcode >201509 && xmcode <201700)
	{
		xmi =xmlinei[0];
		xmstr.LoadString(IDS_STRDCRK);//倒车入库
	}
	else if (xmcode >204509 && xmcode <204700)
	{
		xmi =xmlinei[1];
		xmstr.LoadString(IDS_STRCFTC);//侧方停车
	}
	else if (xmcode >203509 && xmcode <203700)
	{
		xmi =xmlinei[2];
		xmstr.LoadString(IDS_STRDDTC);//坡道定点停车和起步
	}
	else if (xmcode >206509 && xmcode <206700)
	{
		xmi =xmlinei[3];
		xmstr.LoadString(IDS_STRQXXS);//曲线行驶
	}
	else if (xmcode >207509 && xmcode <207700)
	{
		xmi =xmlinei[4];
		xmstr.LoadString(IDS_STRZJZY);//直角转弯
	}
	else if (xmcode == 214510)
	{
		xmi =xmlinei[5];
		xmstr.LoadString(IDS_STRMNSD);//模拟隧道行驶
	}
	else if (xmcode==215510 || xmcode==216510)
	{
		xmi =xmlinei[6];
		xmstr.LoadString(IDS_STRYWT);//模拟雨（雾）天行驶
	}

	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	CString temp;
	temp.Format("%d.dat",isave);
	CBitmap bmpBackground; 
	bmpBackground.m_hObject = LoadImage(NULL,temp.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (bmpBackground.m_hObject==NULL)
	{
		bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_BACK));	
	}	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
	//////////////////////////////////////////////////////////////////////////
	CDC m_dcBuf;
	m_dcBuf.CreateCompatibleDC(NULL);	
	CBitmap m_bmptemp;//
	m_bmptemp.LoadBitmap(MAKEINTRESOURCE(IDB_BMPGREEN));		
	m_dcBuf.SelectObject(&m_bmptemp);
	ImageDC.BitBlt(614,xmi*35,88,35,&m_dcBuf,0,0,SRCCOPY);
	m_bmptemp.DeleteObject();
	m_dcBuf.DeleteDC();
	//////////////////////////////////////////////////////////////////////////
	CRect textrc;//当前项目
	textrc.SetRect(225,166,610,190);
	ImageDC.SetBkColor(RGB(4,88,122));
	CBrush br;
	br.CreateSolidBrush(RGB(4,88,122));
	ImageDC.FillRect(&textrc,&br);//用清除画板
	br.DeleteObject();

	CString Appname;
	Appname.Format("kch%d",isave);
	CString strtext;
	GetPrivateProfileString(Appname,"MSG9","",strtext.GetBuffer(MAX_PATH),MAX_PATH,".\\allcard.msg");
	strtext.ReleaseBuffer();	
	::WritePrivateProfileString(Appname,"MSG9","空闲",".\\allcard.msg");
	//////////////////////////////////////////////////////////////////////////
 	CFont fontz;
	fontz.CreateFont(-MulDiv(14,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_DEMIBOLD,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
 	ImageDC.SelectObject(&fontz);
 	ImageDC.SetBkMode(TRANSPARENT);
 	ImageDC.SetTextColor(RGB(255,255,0));
// 	//////////////////////////////////////////////////////////////////////////
	ImageDC.DrawText(strtext,&textrc,DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	
	CFont fontz1;
	fontz1.CreateFont(-MulDiv(12,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_NORMAL,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	ImageDC.SelectObject(&fontz1);
	textrc.SetRect(616,xmi*35,702,xmi*35+35);
	ImageDC.DrawText(xmstr,&textrc,DT_CENTER | DT_SINGLELINE | DT_VCENTER );
 	//////////////////////////////////////////////////////////////////////////
 	fontz.DeleteObject();
	fontz1.DeleteObject();
 	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,704,288);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("%d.dat",isave);
	SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();
	
	ImageDC.DeleteDC();
}

void CCBKDeSkinApp::DrawImage17C53(UINT isave,UINT eline)
{
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	CString temp;
	temp.Format("%d.dat",isave);
	CBitmap bmpBackground; 
	bmpBackground.m_hObject = LoadImage(NULL,temp.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (bmpBackground.m_hObject==NULL)
	{
		bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_BACK));	
	}	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
	//////////////////////////////////////////////////////////////////////////
	CFont fontz;
	fontz.CreateFont(-MulDiv(12,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_NORMAL,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	ImageDC.SelectObject(&fontz);
	ImageDC.SetBkMode(TRANSPARENT);
	ImageDC.SetTextColor(RGB(255,255,255));
	int k =eline %4;
	CRect recttt[3];
	recttt[0].SetRect(240,223+k*21,320,243+k*21);
	recttt[1].SetRect(321,223+k*21,561,243+k*21);
	recttt[2].SetRect(562,223+k*21,612,243+k*21);
	CString Appname,keyName;
	CString strtext;
	Appname.Format("kch%d",isave);
	for (int i=0;i<3;i++)
	{
		keyName.Format("MSG%d",i+13);
		GetPrivateProfileString(Appname,keyName,"",strtext.GetBuffer(MAX_PATH),MAX_PATH,".\\allcard.msg");
		strtext.ReleaseBuffer();
		ImageDC.DrawText(strtext,&recttt[i],DT_CENTER | DT_SINGLELINE | DT_VCENTER );
	}
	////////////////////////////////////////////////////////////////////////////
	CRect textrc;
	textrc.SetRect(335,62,450,86);//成绩
	ImageDC.SetBkColor(RGB(4,88,122));
	CBrush br;
	br.CreateSolidBrush(RGB(4,88,122));
	ImageDC.FillRect(&textrc,&br);//用清除画板
	br.DeleteObject();
	
	GetPrivateProfileString(Appname,"MSG8","",strtext.GetBuffer(MAX_PATH),MAX_PATH,".\\allcard.msg");
	strtext.ReleaseBuffer();

//	int tempi;
// 	tempi=atoi(strtext);
// 	if (tempi<80)
// 	{
// 		ImageDC.SetTextColor(RGB(255,40,20));
// 	}
	CFont fontz1;
	fontz1.CreateFont(-MulDiv(14,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_DEMIBOLD,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("宋体"));
	ImageDC.SelectObject(&fontz);
	ImageDC.DrawText(strtext,&textrc,DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	//////////////////////////////////////////////////////////////////////////
	fontz.DeleteObject();
	fontz1.DeleteObject();
	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,704,288);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("%d.dat",isave);
	SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();	
	ImageDC.DeleteDC();
}

void CCBKDeSkinApp::DrawImage17C55(UINT isave,UINT xmcode)
{
	UINT xmi;
	CString xmstr;
	if (xmcode==201990)
	{
		xmi =xmlinei[0];
		xmstr.LoadString(IDS_STRDCRK);//倒车入库
	}
	else if (xmcode==204990)
	{
		xmi =xmlinei[1];
		xmstr.LoadString(IDS_STRCFTC);//侧方停车
	}
	else if (xmcode==203990)
	{
		xmi =xmlinei[2];
		xmstr.LoadString(IDS_STRDDTC);//坡道定点停车和起步
	}
	else if (xmcode ==206990)
	{
		xmi =xmlinei[3];
		xmstr.LoadString(IDS_STRQXXS);//曲线行驶
	}
	else if (xmcode==207990)
	{
		xmi =xmlinei[4];
		xmstr.LoadString(IDS_STRZJZY);//直角转弯
	}
	else if (xmcode == 214990)
	{
		xmi =xmlinei[5];
		xmstr.LoadString(IDS_STRMNSD);//模拟隧道行驶
	}
	else if ( xmcode ==216990 || xmcode ==215990)
	{
		xmi =xmlinei[6];
		xmstr.LoadString(IDS_STRYWT);//模拟雨（雾）天行驶
	}
	else
	{

	}
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	CString temp;
	temp.Format("%d.dat",isave);
	CBitmap bmpBackground; 
	bmpBackground.m_hObject = LoadImage(NULL,temp.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (bmpBackground.m_hObject==NULL)
	{
		bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_BACK));	
	}	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
	//////////////////////////////////////////////////////////////////////////
	CDC m_dcBuf;
	m_dcBuf.CreateCompatibleDC(NULL);	
	CBitmap m_bmptemp;//
	m_bmptemp.LoadBitmap(MAKEINTRESOURCE(IDB_BMPBLUE));		
	m_dcBuf.SelectObject(&m_bmptemp);
	ImageDC.BitBlt(614,xmi*35,88,35,&m_dcBuf,0,0,SRCCOPY);
	m_bmptemp.DeleteObject();
	m_dcBuf.DeleteDC();
	//////////////////////////////////////////////////////////////////////////
	CRect textrc;
	textrc.SetRect(225,166,610,190);
	ImageDC.SetBkColor(RGB(4,88,122));
	CBrush br;
	br.CreateSolidBrush(RGB(4,88,122));
	ImageDC.FillRect(&textrc,&br);//用清除画板
	br.DeleteObject();
	
	CString Appname;
	Appname.Format("kch%d",isave);
	CString strtext;
	GetPrivateProfileString(Appname,"MSG9","",strtext.GetBuffer(MAX_PATH),MAX_PATH,".\\allcard.msg");
	strtext.ReleaseBuffer();
	::WritePrivateProfileString(Appname,"MSG9","空闲",".\\allcard.msg");
	//////////////////////////////////////////////////////////////////////////
	CFont fontz;
	fontz.CreateFont(-MulDiv(12,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_NORMAL,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	ImageDC.SelectObject(&fontz);
	ImageDC.SetBkMode(TRANSPARENT);
	ImageDC.SetTextColor(RGB(255,255,255));
	// 	//////////////////////////////////////////////////////////////////////////
	ImageDC.DrawText(strtext,&textrc,DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	textrc.SetRect(616,xmi*35,702,xmi*35+35);
	ImageDC.DrawText(xmstr,&textrc,DT_CENTER | DT_SINGLELINE | DT_VCENTER );
	//////////////////////////////////////////////////////////////////////////
	fontz.DeleteObject();
	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,704,288);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("%d.dat",isave);
	SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();
	
	ImageDC.DeleteDC();
}
void CCBKDeSkinApp::DrawImage17C56(UINT isave,UINT zt)
{
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	CString temp;
	temp.Format("%d.dat",isave);
	CBitmap bmpBackground; 
	bmpBackground.m_hObject = LoadImage(NULL,temp.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (bmpBackground.m_hObject==NULL)
	{
		bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_BACK));	
	}	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
	//////////////////////////////////////////////////////////////////////////
	CRect textrc;
	textrc.SetRect(225,166,610,190);
	ImageDC.SetBkColor(RGB(4,88,122));
	CBrush br;
	br.CreateSolidBrush(RGB(4,88,122));
	ImageDC.FillRect(&textrc,&br);//用清除画板
	br.DeleteObject();
	//////////////////////////////////////////////////////////////////////////
	CDC m_dcBuf;
	m_dcBuf.CreateCompatibleDC(NULL);	
	CBitmap m_bmpMask,m_bmpTemp;//
	if (zt ==1)
	{
		m_bmpMask.LoadBitmap(MAKEINTRESOURCE(IDB_BMP11));
		m_bmpTemp.LoadBitmap(MAKEINTRESOURCE(IDB_BMP1));
	}
	if (zt ==2)
	{
		m_bmpMask.LoadBitmap(MAKEINTRESOURCE(IDB_BMP22));
		m_bmpTemp.LoadBitmap(MAKEINTRESOURCE(IDB_BMP2));
	}			
	m_dcBuf.SelectObject(&m_bmpMask);
	ImageDC.BitBlt(120,60,430,160,&m_dcBuf,0,0,SRCAND);
			
	m_dcBuf.SelectObject(&m_bmpTemp);
	ImageDC.BitBlt(120,60,430,160,&m_dcBuf,0,0,SRCPAINT);

	m_bmpMask.DeleteObject();
	m_bmpTemp.DeleteObject();
	m_dcBuf.DeleteDC();

	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,704,288);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("%d.dat",isave);
	SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();
	
	ImageDC.DeleteDC();
}

void CCBKDeSkinApp::DrawImageBK()
{
	UINT bmpi;
	bmpi=100;
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_BACK));	
	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground);
	//////////////////////////////////////////////////////////////////////////
	CFont fontz;
// 	fontz.CreateFont(-MulDiv(12,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
// 		FW_NORMAL,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
// 		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	fontz.CreateFont(-MulDiv(14,ImageDC.GetDeviceCaps(LOGPIXELSY),72),0,0,0,\
		FW_DEMIBOLD,0,0,0,GB2312_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,\
		DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	ImageDC.SelectObject(&fontz);
	ImageDC.SetBkMode(TRANSPARENT);
	ImageDC.SetTextColor(RGB(255,255,255));
	//////////////////////////////////////////////////////////////////////////
	CRect textrc[13];
	textrc[0].SetRect(138,4+32,285,28+32);
	textrc[1].SetRect(138,30+32,285,54+32);
	textrc[2].SetRect(138,56+32,450,80+32);
	textrc[3].SetRect(138,82+32,450,106+32);
	textrc[4].SetRect(138,108+32,610,132+32);	
//	textrc[5].SetRect(288,4+32,450,28+32);//考试员	
//	textrc[6].SetRect(10,196+32,230,220+32);
//	textrc[7].SetRect(10,170+32,230,194+32);
//	textrc[8].SetRect(10,222+32,230,246+32);//成绩	
//	textrc[10].SetRect(288,30+32,450,54+32);//开始时间

	textrc[5].SetRect(268,4,490,28);//考试员
	textrc[6].SetRect(288,4+32,450,28+32);//次数
	textrc[7].SetRect(6,4,280,28);
	textrc[8].SetRect(288,30+32,450,54+32);//成绩
	textrc[9].SetRect(138,134+32,610,158+32);//当前状态
	textrc[10].SetRect(10,170+32,230,194+32);//开始时间
	textrc[11].SetRect(10,196+32,230,220+32);//考试时长
	textrc[12].SetRect(10,222+32,230,246+32);//行驶速度
	//ImageDC.Rectangle(textrc[9]);
	//ImageDC.SetTextCharacterExtra(1);//设置字符间距
	//////////////////////////////////////////////////////////////////////////
	CString Appname,keyName;
	Appname.Format("kch%d",bmpi);
	CString strtext;
	for (int i=0;i<13;i++)
	{
		keyName.Format("MSG%d",i);
		GetPrivateProfileString(Appname,keyName,"",strtext.GetBuffer(MAX_PATH),MAX_PATH,".\\allcard.msg");
		strtext.ReleaseBuffer();
// 		ImageDC.Rectangle(textrc[i]);
// 		ImageDC.SetTextColor(RGB(255,0,0));
		ImageDC.DrawText(strtext,&textrc[i],DT_LEFT | DT_SINGLELINE | DT_VCENTER );		
	}	
	//////////////////////////////////////////////////////////////////////////
	fontz.DeleteObject();
	//////////////////////////////////////////////////////////////////////////
	CRect DcRect;
	DcRect.SetRect(0,0,704,288);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(ImageDC.GetSafeHdc(),DcRect);	
	CString filename;
	filename.Format("CBKVideoBKmask.bmp");
	SaveBmp(hBmp,filename);	
	DeleteObject(hBmp);
	ImageDC.SelectObject(pbmpOld);
	bmpBackground.DeleteObject();	
	ImageDC.DeleteDC();
}

BOOL CCBKDeSkinApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString temp;
	int xmpz=0;
	for (int i=0;i<7;i++)
	{
		temp.Format("%d",i+1);
		xmlinei[i]=GetPrivateProfileInt("KSXMLB",temp,i,".\\CardChannel.cfg");
		xmpz+=xmlinei[i];
	}
	if (xmpz !=28)
	{
		for (int i=0;i<7;i++)
		{
			temp.Format("%d",i+1);
			::WritePrivateProfileString("KSXMLB",temp,temp,".\\CardChannel.cfg");
			xmlinei[i]=i;
		}		
	}
	return CWinApp::InitInstance();
}

// CBKVideoSkin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKVideoSkin.h"
#include "BmpDlg.h"

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
// CCBKVideoSkinApp

BEGIN_MESSAGE_MAP(CCBKVideoSkinApp, CWinApp)
	//{{AFX_MSG_MAP(CCBKVideoSkinApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoSkinApp construction

CCBKVideoSkinApp::CCBKVideoSkinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKVideoSkinApp object

CCBKVideoSkinApp theApp;
UINT nResource[1]={IDB_BMPUI1};
extern "C" __declspec(dllexport) BOOL DrawImage(HDC hDC, CRect &rc,UINT nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT tempn;
	tempn =0;
	CDC *pDC= CDC::FromHandle(hDC);
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(pDC);
	
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(MAKEINTRESOURCE(nResource[tempn]));
	
	CBitmap *pbmpOld=ImageDC.SelectObject(&bmpBackground); 
	
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	
	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&ImageDC,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	
	ImageDC.SelectObject(pbmpOld);
	ImageDC.DeleteDC();
	return TRUE;
}
extern "C" BOOL PASCAL EXPORT MessageBoxbmp2(LPTSTR msg)
{
	return	TRUE;
}
extern "C" __declspec(dllexport) BOOL MessageBoxbmp(LPTSTR msg) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CBmpDlg msgdlg;
	msgdlg.dlgMessage=msg;
	
	if (msgdlg.dlgMessage=="")
	{
		msgdlg.dlgMessage.LoadString(IDS_STRINGOD);
	}
	int nResponse = msgdlg.DoModal();
	if (nResponse == IDOK)
	{
		return TRUE;
	}
	else if (nResponse == IDCANCEL)
	{
		return FALSE;
	}
	return TRUE;
}

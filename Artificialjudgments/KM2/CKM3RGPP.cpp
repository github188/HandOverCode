// CKM3RGPP.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CKM3RGPP.h"
#include "RgppKm3.h"

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
// CCKM3RGPPApp

BEGIN_MESSAGE_MAP(CCKM3RGPPApp, CWinApp)
	//{{AFX_MSG_MAP(CCKM3RGPPApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCKM3RGPPApp construction
#define DLL_EXPORT extern "C" _declspec(dllexport)

CCKM3RGPPApp::CCKM3RGPPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCKM3RGPPApp object

CCKM3RGPPApp theApp;

BOOL CCKM3RGPPApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CFileFind FindDBFile;
	if (!FindDBFile.FindFile("ErrorData.dat"))
	{
		//AfxMessageBox("1");
		//没有找到文件
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ERRORDATA), "ZSZ");
		HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);
		LPVOID lpExe = LockResource(hGlobal);
		CFile file;
		if(file.Open("ErrorData.dat", CFile::modeCreate | CFile::modeWrite))
			file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));
		file.Close();
		
		::UnlockResource(hGlobal);
		::FreeResource(hGlobal);
	}
	return CWinApp::InitInstance();
}

// DLL_EXPORT int ShowRGPPDlg(LPTSTR strCar,LPTSTR strName,LPCTSTR strNum) 
// {	
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	CRgppKm3 rgppdlg;
// 	rgppdlg.SetMsg(strCar,strName,strNum);
// 	int nResponse = rgppdlg.DoModal();
// 	if (nResponse>10000 && nResponse <50000)
// 	{
// 		return nResponse;
// 	}
// 	return 0;
// }

DLL_EXPORT int ShowRGPPDlg(LPTSTR strCar,LPTSTR strName,LPCTSTR strNum,int xmcode) 
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRgppKm3 rgppdlg;
	rgppdlg.SetXMCodez(strCar,strName,strNum,xmcode);
	int nResponse = rgppdlg.DoModal();
	if (nResponse>10000 && nResponse <50000)
	{
		return nResponse;
	}
	return 0;
}

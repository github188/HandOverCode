// BKPrintSheet.h : main header file for the BKPRINTSHEET DLL
//

#if !defined(AFX_BKPRINTSHEET_H__6EB3C996_7150_42CF_BC94_68F0F84E87E4__INCLUDED_)
#define AFX_BKPRINTSHEET_H__6EB3C996_7150_42CF_BC94_68F0F84E87E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "QR_Encode.h"
/////////////////////////////////////////////////////////////////////////////
// CBKPrintSheetApp
// See BKPrintSheet.cpp for the implementation of this class
//

class CBKPrintSheetApp : public CWinApp
{
public:
	CBKPrintSheetApp();
	int ZPHQ,kmms;
	int Serialnum(CString strYKRQ,CString strExamNo,CString strZT,CString strDLR,CString strBkcx);
	int loadconfig();
	CString m_IPAddress,m_PassWord;
	CString sVersion,sVersionphoto;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBKPrintSheetApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBKPrintSheetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CDC m_DC;
	_ConnectionPtr m_pConnection;
	_ConnectionPtr m_pConn;
public:
	void DoClearPrintDC();
	void DrawYLine(int x1,int sLine,int eLine);
	CRect DCRectT;
	void DrawXLine(int line);
	int LineHeight,SQLorOracle;
	void SetDcText(LPCSTR str,CRect rect,int line,UINT uFormat);
	BOOL DoPreparePrintDC();
	BOOL CreateDataCon(CString strSQLIP,CString strUser,CString strPasword);
	void GetKFX(CStringArray &dest,CString zkzm,int kscs,int drcs);
	BOOL ReadXMPhotoFromDB(CDC *pDC, CString sZkzm, int iDrcs, int iKscs, CRect *rc);
	BOOL ReadXMPhotoFromSQL(CDC *pDC,CRect *rc,CString sExamNo,int iKscs, int Printcs);
	BOOL ReadGYPhotoFromDB(CDC *pDC, CString sCard, CRect *rc1, CRect *rc2);
	BOOL ReadQZPhoto(CDC *pDC, CString sCard, CString sSir, int iDRCS,CRect *rc1, CRect *rc2, CRect *rc3,CRect *rc4);
	void TwoDimQR(CDC *pDC ,CString strEncodeData, CRect *rc);
	BOOL ReadProject(CStringArray &strProject,CString sExamNo,int iKscs,int iDrcs);
	CString KM3Read(int StrPHOTO);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BKPRINTSHEET_H__6EB3C996_7150_42CF_BC94_68F0F84E87E4__INCLUDED_)

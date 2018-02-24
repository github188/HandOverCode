// CatchPhotoHK.h : main header file for the CATCHPHOTOHK DLL
//

#if !defined(AFX_CATCHPHOTOHK_H__FF4AFC9A_E97A_4146_AF7B_05FA63C50B4C__INCLUDED_)
#define AFX_CATCHPHOTOHK_H__FF4AFC9A_E97A_4146_AF7B_05FA63C50B4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <map>
using namespace std;

#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
/////////////////////////////////////////////////////////////////////////////
// CCatchPhotoHKApp
// See CatchPhotoHK.cpp for the implementation of this class
//
#define SAVEDAYS 50 //LOG保存天数

class HKLogin
{
public:
	HKLogin()
	{
		m_IPaddrL=_T("");
		m_Port=8000;
		m_User=_T("");
		m_Password=_T("");
		lUserID=-1;
		pStruWorkStateV40=NULL;
		bisDoLogin=false;
	}
public:
	CString m_IPaddrL;//IP
	int m_Port;//端口号
	CString m_User;//用户名
	CString m_Password;//密码
	LONG lUserID;//设备ID
	NET_DVR_WORKSTATE_V40 *pStruWorkStateV40; //获取工作状态
	DWORD DGetWorkStateTime; //设备获取工作状态时间
	bool bisDoLogin; //是否正在登录
};

class MYKCH
{ 
public:
	MYKCH()
	{
		m_KCH=_T("");
		m_Channel=-1;
		m_IPAddr=_T("");//IP
	}
public:
	CString m_KCH;//考车号
	int m_Channel;//通道号
	CString m_IPAddr;//IP
};

typedef map<CString,MYKCH> MAP_STR_MYKCH;
typedef map<CString,HKLogin> MAP_STR_HKLOGIN;

// class CHKIPFindz 
// {
// public:
// 	CHKIPFindz(const CString str)  
// 	{
// 		m_sIPAddr.Format("%s",str);
// 	}
// 	bool operator()(HKLogin& dValue)  
// 	{  
// 		if (dValue.m_IPaddrL.Compare(m_sIPAddr)==0)
// 		{
// 			return true;
// 		}
// 		else
// 		{
// 			return false;
// 		}  
// 	}  
// private:
// 	CString m_sIPAddr;
// };
// 
// class CKCHFindz
// {
// public:
// 	CKCHFindz(const CString str,UINT uIndex)  
// 	{
// 		m_sKch.Format("%s_%d",str,uIndex);
// 	}
// 	bool operator()(MYKCH& dValue)  
// 	{  
// 		if (dValue.GetKch().Compare(m_sKch)==0)
// 		{
// 			return true;
// 		}
// 		else
// 		{
// 			return false;
// 		}  
// 	}  
// private:
// 	CString m_sKch;
// };

class CCatchPhotoHKApp : public CWinApp
{
public:
	UINT uSqlOrOracle;
	LONG GetSignalStatic(MYKCH myKch);
	UINT GetNVRWorkState(LONG lUserID,int Channel);
	CString OnGetLastErrorData(int ierrorid);
	LONG OnHKLogin(char *sDVRIP, char *sUserName, char *sPassword, WORD wDVRPort);
	BOOL OnCaptureJPEGPic(CString strKch,UINT index, CString fileName);
	BOOL CatchPhotoEx(CString strkch, CString strdrcs, CString strkscs, CString strzkzmbh,CString strECode);
	CString LastError;
	CString Decode(CString str);
	void DeleteDirectory(CString strDir);
	void AddHKLogin(const HKLogin &login,int Nid);
	CString m_SQLIPAddress,m_SQLPassword;
	BOOL TransferPhoto(CString strkch,CString strzkzmbh,CString strkscs,CString strdrcs,int ixmcode);
	void AddKch(const MYKCH &kch);
	BOOL CaptureJPEGPic(LONG lUserID,int Channel,CString Filename);
	BOOL F_Login(HKLogin login);
	BOOL CatchPhoto(CString strkch,CString strdrcs,CString strkscs,CString strzkzmbh,CString strECode);

	CCatchPhotoHKApp();
	CString logFile,strPath;
	void WriteLog(LPCTSTR pstrFormat, ... );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchPhotoHKApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCatchPhotoHKApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
// 	vector<HKLogin> m_HKLogin;	
// 	vector<MYKCH> m_myKch;

	MAP_STR_MYKCH m_myKch;
	MAP_STR_HKLOGIN m_HKLogin;
//	static HANDLE hMutexM;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHPHOTOHK_H__FF4AFC9A_E97A_4146_AF7B_05FA63C50B4C__INCLUDED_)

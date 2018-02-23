// LoadPlatSDK.h: interface for the CLoadPlatSDK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADPLATSDK_H__2662E6E7_799F_4FCA_A7D9_1C3AE2FE81E3__INCLUDED_)
#define AFX_LOADPLATSDK_H__2662E6E7_799F_4FCA_A7D9_1C3AE2FE81E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef void (CALLBACK *CBMessageProcFunc)(UINT nWndID,WPARAM wParam,LPARAM lParam,DWORD dwUser);
#define PLAT_URLOK		WM_USER + 1905
#define PLAT_URLFAILD	WM_USER + 1906

class CLoadPlatSDK  
{
public:
	BOOL PLATPlayVideo(int iwndid,long lWnd);
	void PLATStopVideo(int iwndid);
	void PLATSetStreamCamer(LPCTSTR csCameraID,int iWnd);
	void PLATSetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser);	
	BOOL InitLoadDll();
	//////////////////////////////////////////////////////////////////////////
	CLoadPlatSDK();
	virtual ~CLoadPlatSDK();
private:
	CBMessageProcFunc	m_pMessageProc;
	DWORD				m_dwMessageUser;
	//////////////////////////////////////////////////////////////////////////
	CString m_LogFileName;
	void WriteLogz(LPCTSTR pstrFormat, ... );
	//////////////////////////////////////////////////////////////////////////
	typedef BOOL (* DPlayVideoPlat)(int ,long );
	typedef void (* DStopVideoPlat)(int );	
	typedef void (* DSetStreamCamer)(LPCTSTR ,int );
	typedef void (* DSetCallBack)(CBMessageProcFunc, DWORD);
	HINSTANCE dllPLATHIN;
	DPlayVideoPlat dllPlayVideoPlat;
	DStopVideoPlat dllStopVideoPlat;
	DSetStreamCamer dllSetStreamCamer;
	DSetCallBack dllSetCallBack;
};

#endif // !defined(AFX_LOADPLATSDK_H__2662E6E7_799F_4FCA_A7D9_1C3AE2FE81E3__INCLUDED_)

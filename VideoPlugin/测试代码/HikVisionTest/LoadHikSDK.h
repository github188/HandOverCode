// LoadHikSDK.h: interface for the CLoadHikSDK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADHIKSDK_H__8EE1266A_C81C_4422_A3B3_8F1C8315C8B7__INCLUDED_)
#define AFX_LOADHIKSDK_H__8EE1266A_C81C_4422_A3B3_8F1C8315C8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoadHikSDK  
{
public:	
	BOOL SDKCloseSoundShare(int nWndID);
	BOOL SDKOpenSoundShare(int nWndID);
	void SDKInitWnd(HWND hwndz, int wMSG, int nWndID);
	BOOL SDKDVRLogin(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel, int nWndID);
	BOOL SDKPlayBackbyTime(LPCTSTR strSTime, LPCTSTR strETime,int nWndID);
	BOOL SDKRealPlay(int nWndID);
	void SDKStopPlay(int nWndID);
	BOOL SDKStartVoiceCom_V30(LPCTSTR strDvrIP, LPCTSTR strUseName, LPCTSTR strPwd, long nPort, short nChannel, int nWndID);
	void SDKStopVoiceCom(int nWndID);
	bool InitLoadDll(CString dllpath);
	CLoadHikSDK();
	virtual ~CLoadHikSDK();
private:
	void WriteLogz(LPCTSTR pstrFormat, ... );
	CString m_LogFileName;
	//////////////////////////////////////////////////////////////////////////
	HINSTANCE m_Handlez;
	typedef void (* ZfnInitWnd)(HWND /*hwndz*/, int /*wMSG*/, int /*nWndID*/);
	typedef BOOL (* ZfnDVRLogin)(LPCTSTR /*strDvrIP*/, LPCTSTR /*strUseName*/, LPCTSTR /*strPwd*/, long /*nPort*/, short /*nChannel*/, int /*nWndID*/,int /*itype*/);
	typedef BOOL (* ZfnSetState)(int /*nWndID*/,int /*itype*/);
	ZfnInitWnd dllfnInitWnd;
	ZfnDVRLogin dllfnDVRLogin;
	ZfnSetState dllfnSetState;

	typedef BOOL (*ZfnPBbyTime)(LPCTSTR,LPCTSTR,int);
	ZfnPBbyTime dllfnPlayBackbyTime;
// 	typedef void (* ZfnInitWnd)(HWND /*hwndz*/, int /*wMSG*/, int /*nWndID*/);
// 	typedef BOOL (* ZfnDVRLogin)(LPCTSTR /*strDvrIP*/, LPCTSTR /*strUseName*/, LPCTSTR /*strPwd*/, long /*nPort*/, short /*nChannel*/, int /*nWndID*/);
// 	typedef BOOL (* ZfnRealPlay)(int /*nWndID*/);
// 	typedef void (* ZfnStopPlay)(int /*nWndID*/);
// 	ZfnInitWnd dllfnInitWnd;
// 	ZfnDVRLogin dllfnDVRLogin;
// 	ZfnRealPlay dllfnRealPlay;
// 	ZfnStopPlay dllfnStopPlay;
// 
// 	ZfnDVRLogin dllfnVoiceLogin;
// 	ZfnStopPlay dllfnStopVoice;
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_LOADHIKSDK_H__8EE1266A_C81C_4422_A3B3_8F1C8315C8B7__INCLUDED_)

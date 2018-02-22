// Plat_SDKz.h: interface for the CPlat_SDKz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAT_SDKZ_H__B3656629_F2B3_400E_AA65_5C3D37611C0B__INCLUDED_)
#define AFX_PLAT_SDKZ_H__B3656629_F2B3_400E_AA65_5C3D37611C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WND_MAXNUM 16

#define PLAT_URLOK		WM_USER + 1905
#define PLAT_URLFAILD	WM_USER + 1906

class CPlat_SDKz  :public CThreadz
{
public:
	CBMessageProcFunc	m_pMessageProc;
	DWORD				m_dwMessageUser;
	void SetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser);

	void SetStreamCamer(LPCTSTR csCameraID,int iWnd);
	BOOL PlayVideo(int iWindID,long lWnd);	//播放
	void StopPlay(int iWindID);				//停止播放
	CPlat_SDKz();
	virtual ~CPlat_SDKz();
protected:
	DWORD ThreadMethod();
private:	
	void WriteLogz(LPCTSTR pstrFormat, ...);
	CString logfilenamez;

	void GetRealStreamCamerUrl(int iwnd);
	bool bGetUrl[WND_MAXNUM];
	void FreePlat();
	void InitPlat();
	int m_iLoginID;
	int m_PlayHStream[WND_MAXNUM];
	CString RealStreamURL[WND_MAXNUM];
	CString m_csCameraID[WND_MAXNUM];

	HANDLE hThreadEvent;	//线程句柄
};

#endif // !defined(AFX_PLAT_SDKZ_H__B3656629_F2B3_400E_AA65_5C3D37611C0B__INCLUDED_)

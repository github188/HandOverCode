// Threadz.h: interface for the CThreadz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_)
#define AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//基类 

#include <math.h>
#include <vfw.h>
#include <mmsystem.h>
#pragma comment (lib,"vfw32.lib")
#pragma comment (lib,"winmm.lib")
typedef DWORD (CALLBACK *OnNET_DVR_MatrixSendData)(LONG  lPassiveHandle,char *pSendBuf,DWORD dwBufSize);
/*
DWORD CALLBACK  MessageProcFunc(LONG  lPassiveHandle,char *pSendBuf,DWORD dwBufSize)
{
	if(!NET_DVR_MatrixSendData(lPassiveHandle,pSendBuf,dwBufSize))
	{
		TRACE("NET_DVR_MatrixSendData faild %d \n",NET_DVR_GetLastError());
		return NET_DVR_GetLastError();
	}
	return 5403;
}
*/
class AFX_CLASS_EXPORT CAVIThreadz  
{
public:

	CDC m_ImageDC;
	int m_iCarNum;
	void SetCBMatrixSendData(OnNET_DVR_MatrixSendData onMatrixSendData,LONG  lPassiveHandle);
	void WriteLog(LPCTSTR pstrFormat, ...);	//写日志
	bool InitDC(int iCarNum,int itf,CString ePath,int wMSG,HWND hwndz);
	bool IsMSendDataT();				//转码并发送数据	
	void SavePngFile();					//保存成PNG文件 测试使用
	//////////////////////////////////////////////////////////////////////////
	DWORD Start(DWORD dwCreationFlags = 0);
	DWORD Stop (WORD timeout);			//等待一段时间
	DWORD Stop (bool bForceKill = false);
	DWORD Resume();						//恢复线程
	DWORD Suspend();					//挂起线程
	int GetPriority();					//获取线程优先级
	BOOL SetPriority(int priority);		//设置线程优先级
	CAVIThreadz();
	virtual ~CAVIThreadz();
protected:
	virtual DWORD ThreadMethod() = 0;
private:
	static DWORD WINAPI EntryPoint(LPVOID pArg);//多态性,调用子类的实际工作函数
	HANDLE	m_hThread;
	DWORD	m_dwTID;                    //Thread ID线程ID
	LPVOID	m_pParent;                  //this pointer of the parent CThread object
	DWORD	m_dwExitCode;               //Exit Code of the thread 线程退出码   
protected:
    LPTHREAD_START_ROUTINE    m_pThreadFunction;    //工作线程指针
    BOOL	m_runthread;                //线程是否继续运行的标志
	//////////////////////////////////////////////////////////////////////////
private:
	int m_itf;
	HWND m_hwndz;
	int m_wMSG;
	LONG m_lPassiveHandle;
	OnNET_DVR_MatrixSendData m_NET_DVR_MatrixSendData;

	bool MatrixSendData();
	bool WriteAVIFile();
	bool Transcode();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	CString exePath,m_avifnpath;//程序目录			
	CString m_avifn,m_yuvfn;	//文件名 avi 和 yuv	
	//////////////////////////////////////////////////////////////////////////
	CBitmap bmpBackground;
	BITMAPINFOHEADER *pBInfo;
	BITMAPINFO bInfo;
	BYTE lpData[405504];
	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps; 
	PAVISTREAM pComStream;
	int nFrames;// = 0; 
	HRESULT hr; 
	AVICOMPRESSOPTIONS pCompressOption;   
	AVICOMPRESSOPTIONS FAR * opts[1];// = {&pCompressOption};
	static HANDLE hAVIMutexM;//avi互斥量
};

#endif // !defined(AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_)

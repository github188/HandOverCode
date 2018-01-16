// Threadz.cpp: implementation of the CThreadz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AVIThreadz.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HANDLE CAVIThreadz::hAVIMutexM=CreateMutex(NULL,FALSE,NULL);//创建互斥对象

CAVIThreadz::CAVIThreadz()
{
	m_pThreadFunction = CAVIThreadz::EntryPoint;
	m_runthread = FALSE;
	m_NET_DVR_MatrixSendData=NULL;

	nFrames= 0;
	opts[0]=&pCompressOption;
	m_iCarNum=0;
}

CAVIThreadz::~CAVIThreadz()
{
	if (bmpBackground.m_hObject!=NULL)
	{
		bmpBackground.DeleteObject();
	}
	if ( m_hThread )
		Stop(true);  
}

DWORD CAVIThreadz::Start(DWORD dwCreationFlags)
{
	m_runthread = true;
	m_hThread = CreateThread(NULL, 0, m_pThreadFunction, this, 0, &dwCreationFlags);
	m_dwExitCode = (DWORD)-1;	
	return GetLastError();
}

DWORD CAVIThreadz::Stop(bool bForceKill)
{
	if ( m_hThread )
	{
		
		if (m_runthread == TRUE)
			m_runthread = FALSE;        //first, try to stop the thread nice
		
		GetExitCodeThread(m_hThread, &m_dwExitCode);
		
		if ( m_dwExitCode == STILL_ACTIVE && bForceKill )
		{
			//强制杀死线程
			TerminateThread(m_hThread, DWORD(-1));
			m_hThread = NULL;
		}
	}	
	return m_dwExitCode;
}

DWORD CAVIThreadz::Stop(WORD timeout)
{
	Stop(false);
	WaitForSingleObject(m_hThread, timeout);//等待一段时间
	return Stop(true);
}

DWORD CAVIThreadz::Suspend()
{
	//挂起线程
	return SuspendThread(m_hThread);
}

DWORD CAVIThreadz::Resume()
{
	//恢复线程
	return ResumeThread(m_hThread);
}

BOOL CAVIThreadz::SetPriority(int priority)
{
	//设置线程优先级
	return SetThreadPriority(m_hThread, priority);
}

int CAVIThreadz::GetPriority()
{
	//获取线程优先级
	return GetThreadPriority(m_hThread);
}
// 
DWORD CAVIThreadz::EntryPoint(LPVOID pArg)
{
	CAVIThreadz *pParent = reinterpret_cast<CAVIThreadz*>(pArg);
	pParent->ThreadMethod();//多态性，调用子类的实际工作函数
	return 0;
}

//////////////////////////////////////////////////////////////////////////
bool CAVIThreadz::InitDC(int iCarNum,int itf,CString ePath,int wMSG,HWND hwndz)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_wMSG=wMSG;
	m_hwndz=hwndz;
	m_itf=itf;

	if(m_ImageDC.CreateCompatibleDC(NULL)==NULL)
	{
		return false;
	}
	bmpBackground.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAPBK));	
	if (bmpBackground.m_hObject==NULL)
	{	
		WriteLog("bmpBackGround is NULL %d",iCarNum);
		return false;
	}
	m_ImageDC.SelectObject(bmpBackground);
	pBInfo = new BITMAPINFOHEADER;
	pBInfo->biBitCount       = 32;
	pBInfo->biClrImportant   = 0;
	pBInfo->biCompression    = 0;
	pBInfo->biHeight         = 288;
	pBInfo->biPlanes         = 1;
	pBInfo->biSize           = sizeof(BITMAPINFOHEADER);
	pBInfo->biSizeImage      = 405504;
	pBInfo->biWidth          = 352;
	pBInfo->biXPelsPerMeter  = 0;
	pBInfo->biYPelsPerMeter  = 0;
	
	bInfo.bmiHeader = *pBInfo;
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType    = streamtypeVIDEO;
	strhdr.fccHandler = 0;
	strhdr.dwScale    = 1;
	strhdr.dwRate     = 3;//关键帧数 1秒3帧
	strhdr.dwSuggestedBufferSize = pBInfo->biSizeImage;
	SetRect(&strhdr.rcFrame,0,0,pBInfo->biWidth,pBInfo->biHeight);
	opts[0]->fccType = streamtypeVIDEO;
	opts[0]->fccHandler = mmioStringToFOURCC("MSVC", 0);
	opts[0]->dwQuality = 10000;
	opts[0]->dwBytesPerSecond = 0;
	opts[0]->dwFlags = AVICOMPRESSF_VALID || AVICOMPRESSF_KEYFRAMES;
	opts[0]->lpFormat = 0;
	opts[0]->cbFormat = 0;
	opts[0]->dwInterleaveEvery = 0;
	//////////////////////////////////////////////////////////////////////////
	m_iCarNum=iCarNum;
	exePath.Format("%s",ePath);
	m_avifnpath.Format("%s\\log\\z%d_%d",ePath,m_iCarNum,itf);
	
	m_avifn.Format("%s.avi",m_avifnpath);
	m_yuvfn.Format("%s.yuv",m_avifnpath);
	return true;
}

void CAVIThreadz::WriteLog(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	CTime curTime =CTime::GetCurrentTime();
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	::PostMessage(m_hwndz,m_wMSG,WPARAM(m_itf),LPARAM(new CString(logstr)));
	// 	FILE *fp=fopen(logfilename,"a+");
	// 	fprintf(fp,"[%s]:%s#\n",curTime.Format(_T("%d %H:%M:%S")),logstr);
	// 	fclose(fp);
}

bool CAVIThreadz::IsMSendDataT()
{
	int iTryType=0;
	try
	{
		if (WriteAVIFile()==false)//写AVI文件不成功
		{
			return false;
		}
		iTryType=1;
		if (Transcode()==false)//转码不成功
		{
			return false;
		}
		iTryType=2;
		if (MatrixSendData()==false)//发送到解码器不成功
		{
			return false;
		}	
	}
	catch (CMemoryException )
	{
		WriteLog("IsMSendDataT CMemoryException CarNum=%d itryTpye=%d",m_iCarNum,iTryType);
		return false;
	}
	catch (CFileException )
	{
		WriteLog("IsMSendDataT CFileException CarNum=%d itryTpye=%d",m_iCarNum,iTryType);
		return false;
	}
	catch (CException )
	{
		WriteLog("IsMSendDataT CException CarNum=%d itryTpye=%d",m_iCarNum,iTryType);
		return false;
	}
	catch (... )
	{
		WriteLog("IsMSendDataT... CarNum=%d itryTpye=%d",m_iCarNum,iTryType);
		return false;
	}
	return true;
}

bool CAVIThreadz::WriteAVIFile()
{
	bool breturn=true;
	GetDIBits(m_ImageDC.m_hDC,bmpBackground,0,288,&lpData,&bInfo,DIB_RGB_COLORS);
	
	nFrames =0;	
	CFileFind FFYUV;
	if (FFYUV.FindFile(m_yuvfn))
	{
		CFile::Remove(m_yuvfn);
	}
	FFYUV.Close();
	
	DWORD dtemp=timeGetTime();
	m_avifn.Format("%s_%d.avi",m_avifnpath,dtemp);
	m_yuvfn.Format("%s_%d.yuv",m_avifnpath,dtemp);
	//	WaitForSingleObject(hAVIMutexM,INFINITE);//互斥量
	WaitForSingleObject(hAVIMutexM,3000);//互斥量
	AVIFileInit();		
	if(AVIFileOpen(&pfile,m_avifn,OF_WRITE | OF_CREATE,NULL)==0)
	{		
		hr = AVIFileCreateStream(pfile,&ps,&strhdr); 		
		AVIMakeCompressedStream(&pComStream,ps,&pCompressOption,NULL); 
		AVIStreamSetFormat(pComStream,0,pBInfo,sizeof(BITMAPINFOHEADER));
		while (nFrames <= 2 )
		{
			hr = AVIStreamWrite(pComStream,nFrames ,1,(LPBYTE)lpData,
				pBInfo->biSizeImage,AVIIF_KEYFRAME,NULL,NULL);
			nFrames++;		
		}
	}
	else
	{
		WriteLog("AVIFileOpen is faild! %d",m_iCarNum);
		breturn=false;
	}
	//////////////////////////////////////////////////////////////////////////
	if (pComStream)
	{
		AVIStreamClose(pComStream);
	}
	if (ps)
	{
		AVIStreamClose(ps);
	}
	if(pfile != NULL)
		AVIFileRelease(pfile);
	nFrames =0;
	AVIFileExit();
	ReleaseMutex(hAVIMutexM);//互斥量
	return breturn;
}

bool CAVIThreadz::Transcode()
{
	CFileFind FFAvi;
	if (!FFAvi.FindFile(m_avifn))
	{
		WriteLog("FindFile is faild!%d,%s",m_iCarNum,m_avifn);
		return false;
	}
	FFAvi.Close();
	CString Commandstr,mencoderp;
	Commandstr.Format(" -ovc x264 -x264encopts bitrate=256 -vf scale=352:288 \"%s\" -O \"%s\"",m_avifn,m_yuvfn);
	mencoderp.Format("%s\\mencoder.exe",exePath);
	PROCESS_INFORMATION  ProcessInfo;     
	STARTUPINFO  StartupInfo;  
	ZeroMemory(&StartupInfo,  sizeof(StartupInfo));   
	StartupInfo.cb  =  sizeof(StartupInfo); 
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;		
//	StartupInfo.wShowWindow = SW_SHOW;		
	if(CreateProcess(mencoderp, (LPSTR)(LPCSTR)Commandstr,NULL,NULL,FALSE,0,NULL, NULL,&StartupInfo,&ProcessInfo))   
	{     
		DWORD dwWait=WaitForSingleObject(ProcessInfo.hProcess,5403);   
		if (dwWait==WAIT_TIMEOUT || dwWait==WAIT_FAILED)
		{
			TerminateProcess(ProcessInfo.hProcess,0);//强制关闭  
			CloseHandle(ProcessInfo.hThread);   
			CloseHandle(ProcessInfo.hProcess);	
			WriteLog("WaitForSingleObject %s",Commandstr);
			WriteLog("WaitForSingleObject WAIT_TIMEOUT OR WAIT_FAILED,%d",m_iCarNum);
			Sleep(10);
			return false;
		}  
		CloseHandle(ProcessInfo.hThread);   
		CloseHandle(ProcessInfo.hProcess);
		//压缩完成
		CFile::Remove(m_avifn);
		Sleep(1);
	}
	else
	{
		WriteLog("CreateProcess mencoder.exe faild!,%d %s",m_iCarNum,Commandstr);
		return false;
	}
	return true;
}

bool CAVIThreadz::MatrixSendData()
{
	if (m_NET_DVR_MatrixSendData==NULL)
	{
		return false;
	}
	bool breturn=false;
	HANDLE hFileHandle;
	hFileHandle = INVALID_HANDLE_VALUE;
	hFileHandle = CreateFile(m_yuvfn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileHandle == INVALID_HANDLE_VALUE)
	{
		WriteLog("CreateFile is INVALID_HANDLE_VALUE,GetLastError=%d,%d",GetLastError(),m_iCarNum);
		Sleep(500);
		return false;
	}
	DWORD dwFileSize = GetFileSize(hFileHandle, NULL);
	DWORD dwRealSize = 0;
	char *tmpFile = new char[32 * 1024];
	DWORD tmpSize = 32 * 1024;
	BOOL bReadFlag = TRUE;
	while (1)
	{
		bReadFlag = ReadFile(hFileHandle, tmpFile, tmpSize, &dwRealSize, NULL);
		if (!bReadFlag)
		{
			WriteLog("ReadFile faild!%d",m_iCarNum);
			breturn=false;
			break;
		}
		if (dwRealSize > 0)
		{
			DWORD dwErrid=m_NET_DVR_MatrixSendData(m_lPassiveHandle,tmpFile, tmpSize);
			if (dwErrid!=5403)
			{
				WriteLog("NET_DVR_MatrixSendData faild!考车号=%d,Errid=%d",m_iCarNum,dwErrid);
				Sleep(1000);
				breturn=false;
				break;
			}		
			Sleep(300);
		}
		else
		{
			breturn=true;
			break;
		}
	}
	CloseHandle(hFileHandle);
	hFileHandle = INVALID_HANDLE_VALUE;
	if (NULL != tmpFile)
	{
		delete []tmpFile;
		tmpFile = NULL;
	}
	return breturn;
}

void CAVIThreadz::SetCBMatrixSendData(OnNET_DVR_MatrixSendData onMatrixSendData,LONG  lPassiveHandle)
{
	m_NET_DVR_MatrixSendData=onMatrixSendData;
	m_lPassiveHandle=lPassiveHandle;
}

void CAVIThreadz::SavePngFile()
{
	Bitmap mbitmap(HBITMAP(bmpBackground),NULL);//从CBitmap中得到HBitmap		
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	unsigned short tcharArr[100];
	CString logstr;
	logstr.Format("%s\\log\\%d_%d.png",exePath,m_iCarNum,timeGetTime());
	memset(tcharArr,0x0,sizeof(tcharArr));
	swprintf((wchar_t *)tcharArr,L"%s",logstr.AllocSysString());
	mbitmap.Save(tcharArr,&pngClsid);//保存
}

int CAVIThreadz::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num= 0;
	UINT size= 0;	
	ImageCodecInfo* pImageCodecInfo= NULL;	
	GetImageEncodersSize(&num, &size);
	if(size== 0)
	{
		return -1;
	}
	pImageCodecInfo= (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo== NULL)
	{
		return -1;
	}	
	GetImageEncoders(num, size, pImageCodecInfo);	
	for(UINT j=0; j< num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format)== 0)
		{
			*pClsid= pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}	
	free(pImageCodecInfo);
	return -1;
}
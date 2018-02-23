// TMCP_SDK_INTERFACE_D.h: interface for the CTMCP_SDK_INTERFACE_D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMCP_SDK_INTERFACE_D_H__FA69E0B9_2428_47BF_B636_FDF8A587A11E__INCLUDED_)
#define AFX_TMCP_SDK_INTERFACE_D_H__FA69E0B9_2428_47BF_B636_FDF8A587A11E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum TMCP_STREAM_LINK
{
    TCP = 1,			
		UDP
};

enum TMCP_STREAM_TYPE
{
    DEFAULT = 0,                //按照平台上配的             
		MAIN ,
		SUB  
};

int Plat_Init();
int Plat_LogoutCMS(int iUserhandle);
int Plat_Free();//{return 1;}
int Plat_LoginCMS(const char * cscmsURL, 
				  const char * csUserName, 
				  const char * csPSW, 
				  const char * csPort,
				  int iLoginType = 0);
int Plat_StopVideo(int  hStream);//

const char * Plat_QueryRealStreamURL(const char* csCameraID, int iUserHandle, TMCP_STREAM_TYPE streamType = DEFAULT, TMCP_STREAM_LINK streamLink = TCP);

int Plat_PlayVideo(const char* URL, 
					long hWnd,
					int iUserHandle, 
					void(__stdcall *fStreamCallback)(int handle,const char* data,int size,void *pUser) = NULL, 
        void *pUser = NULL);
int Plat_GetLastError();

#endif // !defined(AFX_TMCP_SDK_INTERFACE_D_H__FA69E0B9_2428_47BF_B636_FDF8A587A11E__INCLUDED_)

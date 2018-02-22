// TMCP_SDK_INTERFACE_D.cpp: implementation of the CTMCP_SDK_INTERFACE_D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMCP_SDK_INTERFACE_D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int Plat_Init(){return -1;}
int Plat_LogoutCMS(int iUserhandle){return -1;}
int Plat_Free(){return -1;}

int Plat_LoginCMS(const char * cscmsURL, const char * csUserName, const char * csPSW, const char * csPort, int iLoginType /* = 0 */)
{
	return -1;
}
int Plat_StopVideo(int  hStream){return 1;}

const char * Plat_QueryRealStreamURL(const char* csCameraID, int iUserHandle, TMCP_STREAM_TYPE streamType /* = DEFAULT */, TMCP_STREAM_LINK streamLink /* = TCP */)
{
	return "";
}

int Plat_GetLastError()
{
	return -1;
}

int Plat_PlayVideo(const char* URL, long hWnd, int iUserHandle, void(__stdcall *fStreamCallback)(int handle,const char* data,int size,void *pUser) /* = NULL */, void *pUser /* = NULL */)
{
	return -1;
}
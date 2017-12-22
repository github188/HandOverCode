// JMQManager.h: interface for the CJMQManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JMQMANAGER_H__552C3098_4290_4455_975D_A65E29E08B9E__INCLUDED_)
#define AFX_JMQMANAGER_H__552C3098_4290_4455_975D_A65E29E08B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JMQCarz.h"
#include <map>
#include <list>
using namespace std;

typedef map<int,CJMQCarz> MAP_INT_JMQCar;
typedef map<CString,STerminalPZ> MAP_STR_VIDEOPZ;
typedef map<CString,ERRORDATA> MAP_STR_ERRORDATA;
//typedef list<JGPTLISTSTRU> LIST_JGPT;

typedef struct _XMCODE
{
	int xmCode;
	int ikch;
}XMcode;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CJMQManager  
{
public:	
	int iQhKCH[100];
	void GetKCHArray(CStringArray& kcharr);//获取所有初始化完成的考车号
	void OnJGPTData(CString strmsg);
	void OnJ17C51(int ikch, CString zkzmbh);	
	void OnJ17C52(int ikch, CString zkzmbh,CString smsg);
	void OnJ17C53(int ikch, CString smsg,CString timestr);
	void OnJ17C55(int ikch, CString zkzmbh,CString smsg);
	void OnJ17C56(int ikch, CString zkzmbh,CString smsg);

	void OnQH17C52(int ikch,int ibh);
	//////////////////////////////////////////////////////////////////////////
	//数据库
	_ConnectionPtr m_pConn;//数据库
	BOOL OnInitSQL();//初始化数据库连接
	BOOL SetTerminalPZ();
	BOOL SetErrorData();
	BOOL GetTerminalPZ(STerminalPZ &tpz,CString str);
	//////////////////////////////////////////////////////////////////////////
	void TermailDEInit();//解码器初始化	
	void RunMap();//初始化
	//////////////////////////////////////////////////////////////////////////
	BOOL OnFindCar(int ikch);//考车是否初始化完成
public:
	BOOL LoadTFDll();
	void ReRunData(int ikch,int itype,CString zkzm,CString msg);
	void GetCS(CString str,int &ikscs,int &idrcs);
	void DelMSGList(int ikch,int itype);
	void AddMSGList(int ikch,int itype,CString zkzmbh,CString msgstr,int iruntime=5);
	HANDLE ListhEvent;
	void RunListMsgThread();
	static UINT ListMSGThread(LPVOID pParam);
//	LIST_JGPT m_ListJgpt;
	void OnGnssData(char *recvbuf,int ikch);
	CString GetErrorKFLX(CString str);
	CJMQManager();
	virtual ~CJMQManager();
private:	
	void CutOutString(CString source,  CStringArray& dest, char division);//字符串截取
	int GetErrorKCFS(CString str);//获取扣除分数
	bool OnCheckChan(UINT kch,int bncn);//通道检测及初始化
	bool OnDS63Init(int id);//设备初始化
private:
	UINT m_DisPlayShowi[5];
	MAP_INT_JMQCar m_JMQCar;//考车列表
	MAP_STR_ERRORDATA m_Errordata;//扣分类型
	MAP_STR_VIDEOPZ m_VideoPZ;//摄像头配置
	//////////////////////////////////////////////////////////////////////////
	NET_DVR_MATRIX_ABILITY_V41 m_Ability;//设备能力集
	LONG m_lUserID;//登录ID
	//////////////////////////////////////////////////////////////////////////
	CString configfile;//配置文件
	CString ModulePath;//当前目录
	//日志
	DWORD AppLogID,dwSLogID,dwFLogID;//日志ID
	CWriteLog logz;//日志类
	UINT uKSKM;//考试科目三
	UINT uWNDTWO;//画面二
	
	UINT uMergeVideo;
private:
	//////////////////////////////////////////////////////////////////////////
	typedef void (* TFInit)(int /*ikch*/,int /*wMSG*/,HWND /*hwndz*/);
	typedef void (* TFPassH)(LONG /*lPassHandle*/,int /*ikch*/,int /*itf*/);
	typedef void (* TF17C51)(int /*ikch*/,CString /*zkzm*/,int /*ikscs*/,int /*idrcs*/);
	typedef void (* TF17C52)(int /*ikch*/,CString /*zkzm*/, int /*ic*/, CString /*msgz*/);
	typedef void (* TF17C53)(int /*ikch*/,CString /*timestr*/, CString /*msgz*/, int /*ikcfs*/);
	typedef void (* TF17C54)(int /*ikch*/,LPVOID /*msgz*/);
	typedef void (* TF17C55)(int /*ikch*/,int /*ic*/, CString /*msgz*/);
	typedef void (* TF17C56)(int /*ikch*/,int /*itype*/, int /*ikscj*/);
	
	TFInit dllTFInit;
	TFPassH dTFPassH;
	TF17C51 dTF17C51;
	TF17C52 dTF17C52;
	TF17C53 dTF17C53;
	TF17C54 dTF17C54;
	TF17C55 dTF17C55;
	TF17C56 dTF17C56;
	HINSTANCE dllYuvToH264TH;
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_JMQMANAGER_H__552C3098_4290_4455_975D_A65E29E08B9E__INCLUDED_)

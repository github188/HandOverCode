// SOCKET.h: interface for the CSOCKET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKET_H__2A559AC2_EEE2_4A35_9BC1_5F25569D2267__INCLUDED_)
#define AFX_SOCKET_H__2A559AC2_EEE2_4A35_9BC1_5F25569D2267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock.h"
#pragma comment(lib,"wsock32")
#include <string>
#include <deque>
#include <map>
#include <iostream>


#define MAX_ExamCar 60

using namespace std;

struct PackageHead_st
{
	UINT PackageType; //包的类型 1 代表考车信息的包
};
typedef struct PackageHead_st PackageHead_ST;

struct ExamCar_st
{
	UINT ExamCarNumber;//考车号
	UINT ThreadNumber;//线程号
	DOUBLE x_Gps;  // 发送过来的GPS点 的纬度 X坐标
	DOUBLE y_Gps;  // 发送过来的GPS点 的经度 y坐标
	UINT zt;//状态
	UINT ErrorID;//错误编号
};
typedef struct ExamCar_st ExamCar_ST;

struct Package_st
{
	PackageHead_ST pPackageHead_ST;
	ExamCar_ST pExamCar_ST;
};
typedef struct Package_st Package_ST;

class CSOCKET  
{
public:
	BOOL dequeOK;  //用于队列的压入和取出 TRUE 允许 FALSE 不允许
	void InitSocket();
	SOCKET m_hSocket;
	void WriteLog(CString szERR);
//	void WriteLog(CString szERR,CString dataFile);
	CSOCKET();
	virtual ~CSOCKET();
	deque<Package_ST *> pPackage_ST;
	map<int,UINT> SockToCar;
	
	UINT TotalCar;  //当前考车在线数量
	int iC_Dlg_F;//创建对话框完成
	


};



#endif // !defined(AFX_SOCKET_H__2A559AC2_EEE2_4A35_9BC1_5F25569D2267__INCLUDED_)

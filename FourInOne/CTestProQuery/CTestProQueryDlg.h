// CTestProQueryDlg.h : header file
//

#if !defined(AFX_CTESTPROQUERYDLG_H__2FBC39CA_BAB8_424B_91D3_0FEAEAF399E8__INCLUDED_)
#define AFX_CTESTPROQUERYDLG_H__2FBC39CA_BAB8_424B_91D3_0FEAEAF399E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#define UM_SOCKETZ		WM_USER +1986
#define UM_SOCKUDPZ		WM_USER +1987

#include "TCPSockz.h"
#include "JmqUDP.h"
#pragma comment(lib,"CUdpForJMQ.lib")

typedef enum PACKTYPE{ 
	SOCKZREEOR,
	GNSSDATA,
	JGPTDATA,
	JMQStart,
	QueryStart,
	AllCarNum,
	M17C51,
	M17C52,
	M17C53,
	M17C54,
	M17C55,
	M17C56
};

typedef struct _terminalpz
{ 
	char TermainlBH[10];	//编号
	char TerminalIP[16];	//设备IP
	char RasUser[16];		//用户名
	char RasPassword[16];	//密码
	WORD TerminalPort;		//端口号
	WORD dwChannel;			//通道号
	char mediaIP[16];		//流媒体IP
	int  imllx;				//码流类型 0 主码流 1 子码流
	_terminalpz()
	{
		TermainlBH[0] ='\0';
		TerminalIP[0] ='\0';
		RasUser[0]	  ='\0';
		RasPassword[0]='\0';
		TerminalPort  =8000;
		dwChannel     =-1;
		mediaIP[0]='\0';
		imllx=1;
	}
}STerminalPZ;

typedef struct _errordata
{
	char errorlx[26];
	int ikcfs;
}ERRORDATA;

typedef struct _StuDentData
{
	int	errorid;
	CString m_KCH;//考车号
	CString m_Zkzmbh;//准考证明编号	.
	CString m_xmcode;
	_StuDentData()
	{
		errorid=0;
		m_KCH=_T("");
		m_Zkzmbh=_T("");
		m_xmcode=_T("");
	}
}STUDENTDATA;

typedef struct _SQLDate
{
	int	iKch;	
	char m_Zkzmbh[20];//准考证明编号	
	char m_Msg[20];
	_SQLDate()
	{
		iKch=0;
		m_Zkzmbh[0]='\0';
		m_Msg[0]='\0';
	}
}SQLDATE;

#include <map>
using namespace std;
typedef map<CString,STerminalPZ> MAP_STR_VIDEOPZ;
typedef map<CString,ERRORDATA> MAP_STR_ERRORDATA;
typedef map<int,STUDENTDATA> MAP_INT_STUDENTDATA;
/////////////////////////////////////////////////////////////////////////////
// CCTestProQueryDlg dialog

class CCTestProQueryDlg : public CDialog
{
// Construction
public:
	CCTestProQueryDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CCTestProQueryDlg)
	enum { IDD = IDD_CTESTPROQUERY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCTestProQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCTestProQueryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSocketZ(WPARAM wParam,LPARAM lParam);//TCP
	afx_msg void OnSocketUDPZ(WPARAM wParam,LPARAM lParam);//UDP
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool bThreadisRun;
	HANDLE hExitThread;
	void CutOutString(CString source,  CStringArray& dest, char division);//字符串截取
	BOOL OnFindCar(int ikch);
	void OnCarToJGPTData(CString strmsg);
	UINT uSQLORTCP;
	bool isprErrorN;
	CWriteLog logz;
	DWORD dwlogID;
	CTCPSockz m_tcps;//TCP
	CJmqUDP jmqudp;

	CString configfile;//配置文件
	CString ModulePath;//当前目录

	_ConnectionPtr m_pConn;//数据库
	BOOL OnInitSQL();//初始化数据库连接

	MAP_INT_STUDENTDATA m_CarIntMap;

	BOOL OnprErrorN();
	BOOL OnCardTestJMQ();
	static UINT QueryDataThread(LPVOID pParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTESTPROQUERYDLG_H__2FBC39CA_BAB8_424B_91D3_0FEAEAF399E8__INCLUDED_)

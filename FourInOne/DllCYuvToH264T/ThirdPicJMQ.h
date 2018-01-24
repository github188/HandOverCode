// ThirdPicJMQ.h: interface for the CThirdPicJMQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THIRDPICJMQ_H__8F5907CA_486F_4FD0_893F_8F97564C9226__INCLUDED_)
#define AFX_THIRDPICJMQ_H__8F5907CA_486F_4FD0_893F_8F97564C9226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThirdPicJMQ  : public CAVIThreadz
{
public:
	int iRunNum;
	void ThirdPicInit(int ikch, CString path,int wMSG,HWND hwndz);//初始化
	void SetSQLOracle(UINT itype);				//设置数据库连接SQL 1还是Oracle 0
	void SetSleepTime(DWORD dwTime);
	void On17C51(CString zkzmbh,int ikscs,int idrcs);	//准考证明编号,考试次数,当日次数
	void On17C52(CString zkzmbh);						//准考证明编号
	void On17C56(int izt,int kscj);						//考试状态1合格2不合格,考试成绩
public:
	CThirdPicJMQ();
	virtual ~CThirdPicJMQ();
protected:
	DWORD ThreadMethod();//线程	
private:
	Image *ImgBK;			//背景
	Image *ImgHGBHG;		//合格不合格图片
	DWORD m_SleepTime;		//线程运行间隔时间
	HANDLE hThreadEvent;	//线程句柄
	bool threadRun;			//线程是否运行
	CString m_zkzmbh;		//准考证明编号  17C51
	int m_idrcs,m_ikscs;	//考试次数 当日次数 17C51
	CString textstr[9];	
	int m_iMtype;
	int m_izt;
	
	bool m_bEncrypt;	//数据库字段是否有做加密

private:
	void DrawHbHg(Graphics *graphics,int x,int y);//17C56时调用 画 合格与不合格  	
	//////////////////////////////////////////////////////////////////////////
	CString m_Connstr;			//数据库连接字串
	CString sqlExstr;			//查询语句
	_ConnectionPtr m_pConn;
	int CreateAdoConn();
	void RealseAdoConn();
	bool GetStuMSG();
	int GetDrcs();
	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard,CRect rc);		//证件照片
	BOOL ReadMJPhotoFromDB(CDC *pDC, CString sCard,CRect rc); 	//门禁照片
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_THIRDPICJMQ_H__8F5907CA_486F_4FD0_893F_8F97564C9226__INCLUDED_)

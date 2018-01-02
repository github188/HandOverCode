// FourthPicJMQ.h: interface for the CFourthPicJMQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURTHPICJMQ_H__7970E164_2015_4B34_9E91_763BC138A42E__INCLUDED_)
#define AFX_FOURTHPICJMQ_H__7970E164_2015_4B34_9E91_763BC138A42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
class CFourthPicJMQ  : public CAVIThreadz
{
public:	
	UINT uDituPy;
	int iRunNum;//多运行几次
	//////////////////////////////////////////////////////////////////////////
	void FourthPicInit(int ikch, CString path,int wMSG,HWND hwndz);//初始化
	void LoadMapCfg(CString path);		//加载地图文件 没加载默认是项目牌版本
	void SetSleepTime(DWORD dwTime);	//默认1000毫秒
	//////////////////////////////////////////////////////////////////////////
	void On17C51(CString zkzmbh,int ikscs,int idrcs);		//准考证明编号,考试次数,当日次数
	void On17C52(CString zkzmbh,int icode,CString smsg);	//准考证明编号,项目编号,信息
	void On17C53(CString strTime,CString smsg,int ikcfs);	//扣分时间,扣分原因,扣除分数
	void On17C55(int icode,CString smsg);					//项目编号,信息
	void On17C56(int itype,int kscj);						//考试状态1合格2不合格,考试成绩
	void OnGnssData(LPVOID msgz);							//经纬度速度里程等结构体
	//////////////////////////////////////////////////////////////////////////	
public:
	void SetNowDataPath(CString path,UINT uKSKM);
	CFourthPicJMQ();
	virtual ~CFourthPicJMQ();
protected:
	DWORD ThreadMethod();//线程
private:
	UINT m_uKSKM;
	CString stuDataName,m_NowDataPath;
	void WriteStuData(LPCTSTR pstrFormat, ...);//保存回放数据
	//////////////////////////////////////////////////////////////////////////
	bool m_DrawMap;				//是否画地图
	void DrawXMListTM();		//项目列表线程
	void DrawMapTM();			//动态地图线程
	//////////////////////////////////////////////////////////////////////////
	int m_type;
	int m_i52type,m_i55type;
	int GetMType(int &type);
	void SetMtype(int &type,int idrxmp);
	void DrawXMList(Graphics *graphics, Image *img);
	//////////////////////////////////////////////////////////////////////////
	bool m_DrawCar;				//是否显示考车
	POINT m_mayWH;				//地图长宽
	double m_Mapx,m_Mapy,m_bs;	//经纬度 和倍数
	int m_imapx,m_imapy;		//经度 纬度 平面坐标点
	//////////////////////////////////////////////////////////////////////////
	int ikscj;					//考试成绩	
	int iDrXMP;					//画项目牌
	int m_ErrorLine;			//错误行
	CString m_ErrorMSG[3][3];	//9个错误信息
	CString nowztstr;			//当前状态
	CTime m_StartTime;			//考试开始时间	
	CString m_kscssj;
	GNSSMSG m_GnssMsg;			//GNSS数据
	int GetXMPi(int icode);
	void DrawTime(Graphics *graphics,int x,int y);//考试时长
	//////////////////////////////////////////////////////////////////////////
	Image *ImgMark;			//遮罩层
	Image *ImgMap;			//地图文件
	Image *ImgTime;			//时间
	Image *ImgXmp;			//项目牌
	Image *ImgCAR;			//考车
	//////////////////////////////////////////////////////////////////////////
	DWORD m_SleepTime;		//线程运行间隔时间
	HANDLE hThreadEvent;	//线程句柄
	bool threadRun;			//线程是否运行

	//huangqiwei
	CString m_sExePath;	//程序路径
	CString m_sMapPath;	//底图路径
	bool m_bNineMaps;		//是否采用九宫格模式
	bool m_bBigCar;	//大车
	Image *ImgSignal;			//信号
	Image *ImgXMList;		//地图模式下也要展示项目牌
	Image *ImgXMListMark;		//地图模式下也要展示项目牌,这张图是项目背景
	bool m_bDrawSignal;	//是否绘制信号
	bool m_bSignalAqd;	//安全带信号
	bool m_bSignalJs;	//脚刹信号
	bool m_bSignalSs;	//手刹信号
	bool m_bSignalFs;	//副刹信号
	bool m_bSignalLb;	//喇叭信号
	bool m_bSignalZzx;	//左转向信号
	bool m_bSignalYzx;	//右转向信号
	bool m_bSignalYgd;	//远光灯信号
	bool m_bSignalJgd;	//近光灯信号
	bool m_bSignalXh;	//熄火信号
	bool m_bSignalKgm;	//开关门信号
	bool m_bSignalLh;	//离合信号
	bool m_bSignalDw;	//档位信号
	void DrawSignal(Graphics *graphics);//实时信号
	void DrawNineMaps(Graphics *graphics, int carX, int carY);//画九宫格地图
};

#endif // !defined(AFX_FOURTHPICJMQ_H__7970E164_2015_4B34_9E91_763BC138A42E__INCLUDED_)

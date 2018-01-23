// CBKDigitalMapDlg.h : header file
//

#if !defined(AFX_CBKDIGITALMAPDLG_H__4B6522BF_C2F2_4FC0_9F70_6AC7D4C06270__INCLUDED_)
#define AFX_CBKDIGITALMAPDLG_H__4B6522BF_C2F2_4FC0_9F70_6AC7D4C06270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buttonz.h"
#include "DD.h"
#include "ExamCarInfoDlg.h"
//#include <afxmt.h>


/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapDlg dialog


#define GPS_CarTimer 1
#define GPS_Dlg 2
#define SubDlgForInfoWidth 350
#define SubDlgForInfoHeight 150


class CCBKDigitalMapDlg : public CDialog
{
// Construction
public:
	void DeleteLogData();  //删除之前的记录文件
	BOOL ReadOnlineCarFromDB2();
	BOOL ReadOnlineCarFromDB1();
	void SendMesageToVideoDlgCloseS();
	void SendMesageToVideoDlgCloseT();
	void SendMessageToVideoDlgS(UINT ExamCar,UINT VideoAdd);
	void SendMessageToVideoDlgT(UINT ExamCar,UINT VideoAdd);
	BOOL OnButtonopen(); 
	BOOL OnButtonopenS();
	BOOL OnButtonopenT();
	BOOL CreateVideoDlg();
	BOOL CreateVideoDlgT();
	BOOL CreateVideoDlgS();
	void SendMesageToVideoDlgClose();
	void SendMessageToVideoDlg(UINT ExamCar,UINT VideoAdd);  // VideoAdd是考车上的哪路视频 1 是主驾驶 2 是副驾驶 3 是车外
	HWND GetServerMainWnd();
	HWND GetServerMainWndT();
	HWND GetServerMainWndS();
	int iISCreate; //0表示还没有创建这个字进程 1表示已经创建
	int iISCreateT; //0表示还没有创建这个字进程 1表示已经创建
	int iISCreateS; //0表示还没有创建这个字进程 1表示已经创建
	HANDLE m_hProcess;  //用于子进程的 也就是是打开视频的进程
	HANDLE m_hProcessT;  //用于子进程的 也就是是打开视频的进程
	HANDLE m_hProcessS;  //用于子进程的 也就是是打开视频的进程
	BOOL HasPlayInvalite();  //这个函数的功能是用来如果有视频播放的话刷新的区域要除去视频那一部分
	int iStateDlgShow;  //这一个标志位是用来对话框伸长和收回去的时候用的  如果为1 说明正在伸长或收回去 为0则是正常状态
	int bMouseOn[MAX_ExamCar];  //这个是用于判断鼠标有没有在这辆考车上  TRUE为在这考车上 FALSE 没有落在这辆考车上
	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc);
	CRect rt;
	int iStateLBD;//用来标记鼠标左键按下 拖动时为1 正常状态是为0
	CDC m_Cbuf;
	CDC m_Cmem;
	CDC m_CbufDlg;
	CBitmap pBitmap;
	CBitmap pBitmapDlg;
	CBitmap pBG;
//	CBitmap pBG1;
	///////////////////
	BOOL ReadOnlineCarFromDB();//获取在线考车数
	void WriteLog(CString szERR,CString car);
	CCriticalSection  pMutexForPG;//用于保证解包和接收包互斥
	int iContrl;//用来控制当程序结束的时候释放线程 初始化为 1 结束的时候为0 在主程序销毁清零
	CCriticalSection  pMutex; //互斥锁 用于考车GPS点显示的互斥
	CMutex Mutex_Self[MAX_ExamCar];//用于考车自己互斥用，主要是用于考车下线用
	int iCountFlushMap;//用来统计地图刷新的次数 当为0的时候才可以刷新按钮 
	int iCountFlushExamCar;//用来统计考车刷新的次数 当为0的时候才可以刷新地图
	int HadWakeUp;//表示线程已经唤醒了  0 表示考车线程已经被唤醒了 1 代表线程需要唤醒
	void WakeUp_Thread();  //唤醒线程
	void Suspend_Thread(); //挂起线程
	int iNowZoomRunning;//正在放大缩小的刷新  1 正在刷新 0 没有在刷新
	HANDLE ThreadHandle[MAX_ExamCar+1];
	void CreateDlgha();
	int isRunningUpdate;  //正在刷新按钮 1正在刷新 0 没有在刷新
	//CExamCarInfoDlg 所要用的成员变量已经函数
	CExamCarInfoDlg ExamCarInfo;
    // 结束
	int y_CurLbuttonDown;  //当前鼠标按下的x坐标  用于移动画布的
	int x_CurLbuttonDown;  //当前鼠标按下的y坐标  用于移动画布的
	BOOL LButtonDown;  // 是用来拖动画布的 当为TRUE时，鼠标按下可以开始拖动画布 FALSE时鼠标左键释放不能再拖动画布
	int PixState;  //原图标志 如果已经缩小到原图状态 则用0表示 如果放大 则用1 表示
	int y_OriginalBit;  //原始图片的高度
	int x_OriginalBit; //原始图片的宽度
	int y_BitWidth;  //应该要加载的图片的高度
	int x_BitWidth;	//应该要加载的图片的宽度
	int y_StartBit; //开始重绘在图片的起始坐标y轴 
	int x_StartBit;//开始重绘在图片的起始坐标x轴
	//放大缩小比例选择 1.2
	int ZoomLevel;  //放大缩小等级  为 0 时可以开始放大 当到达4时，就不能再放大了  每放大一次或缩小一次 这个值就加一或减一
	// ZoomLevel为-20 的时候说明已经是最小了不能再缩小了
	int ZoomLevel1; //这个变量用来存储最小时，此时的缩小级别
	int y_CurScr;  //当触发鼠标中间滑块是，当前鼠标的x坐标值
	int x_CurScr;  //当触发鼠标中间滑块是，当前鼠标的y坐标值
//	int iNowTrack;//正在放大缩小的刷新  1 正在刷新 0 没有在刷新
	
//	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CDD dlgha[MAX_ExamCar];
	void WriteGPSPoint(ExamCar_ST *pExamCar_ST);
	DOUBLE MAPBotom; //地图的底部GPS值
	DOUBLE MAPTop;  //地图的顶部GPS值
	DOUBLE MAPLeft;  //地图的左边GPS值
	DOUBLE MAPRight;  //地图的右边GPS值

	DOUBLE OriginalMAPBotom; //地图的底部GPS值 原始的完整图片的GPS值
	DOUBLE OriginalMAPTop;  //地图的顶部GPS值
	DOUBLE OriginalMAPLeft;  //地图的左边GPS值
	DOUBLE OriginalMAPRight;  //地图的右边GPS值
	void WriteLog(CString szERR);
	CCBKDigitalMapDlg(CWnd* pParent = NULL);	// standard constructor
	DOUBLE x_pixel;  //对应屏幕一个像素代表多少纬度
	DOUBLE y_pixel;  //对应屏幕一个像素代表多少经度
	DOUBLE x_Originalpixel;  //对应屏幕一个像素代表多少纬度  原始的x
	DOUBLE y_Originalpixel;  //对应屏幕一个像素代表多少经度  原始的y
	ExamCar_ST on_ExamCar[MAX_ExamCar]; //当前考车GPS的最新信息

	UINT CurrentScrHeight;  //当前屏幕高度
	UINT CurrentScrWidth;  //当前屏幕宽度
	CSOCKET pSocket;//创建Socket对象
	BOOL ExamCarState[MAX_ExamCar]; //考车状态 TRUE 表示开始考试 FALSE 表示考试结束 这个主要用于存放各个考车GPS点 定义最多50辆考车

	BOOL bExamCarState[MAX_ExamCar];  //本来是在解包里面的局部变量

	map<int,UINT> SockToCar1; //用来存放上线的考车号对应的在线程中的位置
	// Dialog Data
	//{{AFX_DATA(CCBKDigitalMapDlg)
	enum { IDD = IDD_CBKDIGITALMAP_DIALOG };
	CStatic	m_TotalCar;
	CStatic	m_boot;
	CButtonz	m_Button[MAX_ExamCar];
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKDigitalMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CFont m_Font;  

	// Generated message map functions
	//{{AFX_MSG(CCBKDigitalMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnBtnMsgz(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnBtnMsgzLeft(WPARAM wParam,LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  //将控件的背景改为透明

	afx_msg LRESULT OnUpdataCurCar(WPARAM wParam,LPARAM lParam); //发送消息用于更新当前考车数
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); //用于鼠标中间的滑块 滚轮

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
//	afx_msg void OnButtonopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bmpMap;
	CDC	 m_dcMem;
};


//在这里构造一个结构体 成员为 CCBKDigitalMapDlg 的指针以及 对应考车号
struct ExamCarInfoToThread
{
	CCBKDigitalMapDlg *pMapDlg; //主对话框的指针
	int iExamCarNumber; //考车号
};
typedef struct ExamCarInfoToThread * ExamCarInfoToThread_ST;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKDIGITALMAPDLG_H__4B6522BF_C2F2_4FC0_9F70_6AC7D4C06270__INCLUDED_)

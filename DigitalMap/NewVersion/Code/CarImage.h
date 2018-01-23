// CarImage.h: interface for the CCarImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_)
#define AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCarImage  
{
public:	
	CCarImage();
	virtual ~CCarImage();
public:	
	void SetXY(int gx,int gy);
	void SetCarNum(int inum);
	void SetStuZkzm(CString zkzm);
	void SetDrawimg(int drawi);
	void SetEllipsePenColor(UINT iset);//设置圆形颜色
	bool isOnLine;//考车在线
	BOOL IsPtinScreen(CRect src,float fzl, float fzlY);//是否在屏幕区域内
	void SetXYRotate(double cx,double cy,double cr);//设置XY及旋转角度
	void DrawCar(Graphics* graphics);//画车
	void InitCar(int cnum);//初始化考车
	int GetCarNum();//获取考车号
private:
	int idrawimg;
	float m_CRotate;//旋转角度
	int m_CarNum;//考车号
//	Image *m_pImageCar;//考车图
//	CSize m_ImgSize;//图像大小
	PointF m_CenterPt,m_mapPT;//中心
	RectF m_picRCF;//m_CenterPt为中心的矩形 与缩放级别相关
	Pen *EllipsePen;
	//////////////////////////////////////////////////////////////////////////
	unsigned short tcharArr[10];//考车号
	unsigned short tcharArrzkzm[50];//准考证明编号
	StringFormat format;
};

#endif // !defined(AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_)

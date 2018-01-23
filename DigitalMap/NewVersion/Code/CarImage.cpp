// CarImage.cpp: implementation of the CCarImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCarImage::CCarImage()
{
	m_CRotate=0.0f;
	m_CenterPt.X=0;
	m_CenterPt.Y=0;
	m_mapPT.X=0;
	m_mapPT.Y=0;
	isOnLine=false;
	m_CarNum =-1;
	idrawimg=0;
}

CCarImage::~CCarImage()
{
//	delete m_pImageCar;
}

void CCarImage::InitCar(int cnum)
{
	m_CarNum =cnum;
//	m_pImageCar =Image::FromFile(L"car.png");
//	m_ImgSize.cx=m_pImageCar->GetWidth();
//	m_ImgSize.cy=m_pImageCar->GetHeight();
 	memset(tcharArr,0x0,sizeof(tcharArr));
 	swprintf((wchar_t *)tcharArr,L"%d",m_CarNum);
	//////////////////////////////////////////////////////////////////////////		
	format.SetAlignment(StringAlignmentCenter);//默认都不影响垂直 
	format.SetLineAlignment(StringAlignmentCenter);// 和SetAlignment对应，为竖直的设置
	Pen tempPen(Color(255,40,176,255),4);//, 255));
	EllipsePen =tempPen.Clone();
}

void CCarImage::DrawCar(Graphics *graphics)
{	
// 	if (idrawimg ==1 || idrawimg ==2 )//画车边框
// 	{
// 		graphics->TranslateTransform(m_CenterPt.X,m_CenterPt.Y);
// 		graphics->RotateTransform(m_CRotate);
// 		//恢复绘图平面在水平和垂直方向的平移
// 		graphics->TranslateTransform(-m_CenterPt.X, -m_CenterPt.Y);
// 		//绘制图片
// 		graphics->DrawImage(m_pImageCar,m_picRCF);
// 		graphics->ResetTransform();
// 	}
// 	if (idrawimg ==2 || idrawimg ==3)
// 	{
// 		Image image(L"tipbox.png");
// 
// 		graphics->DrawImage(&image,m_CenterPt.X,m_CenterPt.Y);
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	if (idrawimg >=0 )
// 	{
		Font myFont(L"Arial",14);		
		SolidBrush blackBrush(Color(255,238,7,0));
		RectF temprf(m_CenterPt.X-10,m_CenterPt.Y-10,20,20);
		graphics->DrawEllipse(EllipsePen, temprf);
		//	graphics->FillEllipse(&blackBrush, temprf);
		graphics->DrawString(
			tcharArr,
			wcslen(tcharArr),
			&myFont,
			m_picRCF,
			&format,
		&blackBrush);
//	}
}

void CCarImage::SetXYRotate(double cx,double cy,double cr)
{
	m_mapPT.X =(float)cx;
	m_mapPT.Y =(float)cy;
	m_CRotate =(float)cr;
//	isOnLine =true;
}

BOOL CCarImage::IsPtinScreen(CRect src, float fzl, float fzlY)
{	
	CPoint temppt;
	temppt.x =(int)m_mapPT.X;
	temppt.y =(int)m_mapPT.Y;
	if (!src.PtInRect(temppt))
	{//不在屏幕区域内
		return FALSE;
	}

	m_CenterPt.X=(m_mapPT.X-src.left) / fzl;
	m_CenterPt.Y=(m_mapPT.Y-src.top) / fzlY;
	m_picRCF.X=m_CenterPt.X-20;
	m_picRCF.Y=m_CenterPt.Y-20;
	m_picRCF.Width =40;
	m_picRCF.Height =40;
	return TRUE;
}
int CCarImage::GetCarNum()
{
	return m_CarNum;
}

void CCarImage::SetEllipsePenColor(UINT iset)
{
	switch (iset)
	{
	case 1:
		EllipsePen->SetColor(Color(255,240,150,9));
		break;
	case 2:
		EllipsePen->SetColor(Color(255,255,0,151));
		break;
	case 3:
		EllipsePen->SetColor(Color(255,51,153,51));
		break;
	default:
		break;
	}
}

void CCarImage::SetDrawimg(int drawi)
{
	idrawimg =drawi;
}

void CCarImage::SetStuZkzm(CString zkzm)
{
	if (!zkzm.IsEmpty())
	{
		memset(tcharArrzkzm,0x0,sizeof(tcharArrzkzm));
 		swprintf((wchar_t *)tcharArrzkzm,L"%s.png",zkzm);	
	}	
}

void CCarImage::SetCarNum(int inum)
{
	m_CarNum=inum;
	memset(tcharArr,0x0,sizeof(tcharArr));
 	swprintf((wchar_t *)tcharArr,L"%d",m_CarNum);
}

void CCarImage::SetXY(int gx, int gy)
{
	m_mapPT.X =(float)gx;
	m_mapPT.Y =(float)gy;
}

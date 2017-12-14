// FourthPicJMQ.cpp: implementation of the CFourthPicJMQ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "CYuvToH264T.h"
#include "FourthPicJMQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//extern CCYuvToH264TApp theApp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFourthPicJMQ::CFourthPicJMQ()
{
	hThreadEvent=NULL;
	m_SleepTime=2000;
	m_imapx=0;
	m_imapy=0;
	m_ErrorLine=0;
	m_DrawCar=false;//不画画车模型
	m_DrawMap=false;//画地图
	ikscj=100;
	iDrXMP=0;
	m_i52type=0;
	m_i55type=0;
	iRunNum=5;
	m_uKSKM=2;//默认科目二

	m_bDrawSignal = false;
	m_bNineMaps = false;
	m_bBigCar = false;
}

CFourthPicJMQ::~CFourthPicJMQ()
{

}

DWORD CFourthPicJMQ::ThreadMethod()
{
	DWORD times=m_iCarNum*(rand()%10);
	Sleep(times);
	if (m_DrawMap==true)
	{
		//调用动态地图版本
		DrawMapTM();
	}
	else
	{
		//调用项目列表版本
		DrawXMListTM();
	}
	return 0;
}

void CFourthPicJMQ::WriteStuData(LPCTSTR pstrFormat, ...)
{
	if (threadRun ==true)
	{		
		CString logstr;
		CTime curTime =CTime::GetCurrentTime();
		va_list avlist;
		va_start(avlist, pstrFormat);
		logstr.FormatV(pstrFormat, avlist);
		TRACE(logstr+"\n");
		va_end(avlist);
		FILE *fp=fopen(stuDataName,"a+");
		fprintf(fp,"[%s]:%d*%s#\n",curTime.Format(_T("%Y-%m-%d %H:%M:%S")),m_iCarNum,logstr);
		fclose(fp);
	}
}

void CFourthPicJMQ::FourthPicInit(int ikch, CString path,int wMSG,HWND hwndz)
{
	InitDC(ikch,4,path,wMSG,hwndz);

	CString temp;
	unsigned short StcharArr[256];

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\mark.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgMark=Image::FromFile(StcharArr);



	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\time.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgTime=Image::FromFile(StcharArr);//时间

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\xmp.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgXmp=Image::FromFile(StcharArr);

	//huangqiwei
	CString configfile;
	configfile.Format("%s\\config.ini",path);	
	UINT uBigCar;
	uBigCar=GetPrivateProfileInt("CONFIG","BigCar",0,configfile);
	if (1 == uBigCar)
	{
		m_bBigCar = true;
	}

	m_type =m_type | MTYPE1;
	iRunNum=5;
	//////////////////////////////////////////////////////////////////////////
}

void CFourthPicJMQ::DrawNineMaps(Graphics *graphics, int carX, int carY)
{
	try
	{
	CString configfile;
	UINT uMaxX = 0;
	UINT uMinX = 0;
	UINT uMaxY = 0;
	UINT uMinY = 0;
	UINT uZoomIn = 0;
	UINT uSplitWidth = 0;
	configfile.Format("%s\\HS_CONF_MAP.ini",m_sExePath);	
	uMaxX=GetPrivateProfileInt("CONFIG","MAXX",0,configfile);
	uMinX=GetPrivateProfileInt("CONFIG","MINX",0,configfile);
	uMaxY=GetPrivateProfileInt("CONFIG","MAXY",0,configfile);
	uMinY=GetPrivateProfileInt("CONFIG","MINY",0,configfile);
	uZoomIn=GetPrivateProfileInt("CONFIG","ZOOMIN",0,configfile);
	uSplitWidth=GetPrivateProfileInt("CONFIG","SPLITWIDTH",0,configfile);
	//WriteLog("carX=%d,carY=%d,splitWidth=%d,maxx=%d",carX,carY,uSplitWidth,uMaxX);
	if (0==uMaxX || 0 == uMinX || 0 == uMaxY || 0 == uMinY || 0 ==uZoomIn || 0 == uSplitWidth)
	{
		WriteLog("错误：从HS_CONF_MAP.ini配置文件中得到的坐标范围存在异常，请检查配置文件");
		return;
	}

	int rowNum = carY/ uSplitWidth;
	int columnNum = carX/ uSplitWidth;
	//WriteLog("rowNum=%d,columnNum=%d",rowNum,columnNum);
	if (0 == rowNum && 0 == columnNum)
	{
		//WriteLog("错误：从HS_CONF_MAP.ini配置文件中得到的坐标范围存在异常，请检查配置文件.carX=%d, carY=%d",carX, carY);
		//return;
		rowNum = 1;
		columnNum = 1;
		carX += uSplitWidth;
		carY += uSplitWidth;
	}

	//加载考车坐标周围的9张图片
	CString temp;
	unsigned short StcharArr[256];
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum-1, columnNum-1);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgLeftTop = Image::FromFile(StcharArr);

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum-1, columnNum);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgTop = Image::FromFile(StcharArr);

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum-1, columnNum+1);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgRightTop = Image::FromFile(StcharArr);

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum, columnNum-1);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgLeftMiddle = Image::FromFile(StcharArr);
	
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum, columnNum);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgMiddle = Image::FromFile(StcharArr);
	
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum, columnNum+1);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgRightMiddle = Image::FromFile(StcharArr);

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum+1, columnNum-1);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgLeftBottom = Image::FromFile(StcharArr);
	
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum+1, columnNum);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgBottom = Image::FromFile(StcharArr);
	
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\%d_%d.png",m_sMapPath, rowNum+1, columnNum+1);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	Image *imgRightBottom = Image::FromFile(StcharArr);

	//生成一张临时的大图
	Bitmap bm(3*uSplitWidth, 3*uSplitWidth);
	Graphics *gNineMaps;
	gNineMaps = Graphics::FromImage(&bm);
	gNineMaps->DrawImage(imgLeftTop, Rect(0, 0, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgTop, Rect(uSplitWidth, 0, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgRightTop, Rect(2 * uSplitWidth, 0, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgLeftMiddle, Rect(0, uSplitWidth, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgMiddle, Rect(uSplitWidth, uSplitWidth, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgRightMiddle, Rect(2 * uSplitWidth, uSplitWidth, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgLeftBottom, Rect(0, 2 * uSplitWidth, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgBottom, Rect(uSplitWidth, 2 * uSplitWidth, uSplitWidth, uSplitWidth));
	gNineMaps->DrawImage(imgRightBottom, Rect(2 * uSplitWidth, 2 * uSplitWidth, uSplitWidth, uSplitWidth));

	//绘制实时地图
	int relativeX = carX % uSplitWidth + uSplitWidth;
	int relativeY = carY % uSplitWidth + uSplitWidth;
	//WriteLog("relativex=%d,relativey=%d",relativeX,relativeY);
	graphics->DrawImage(&bm, Rect(0,0,352,288), (relativeX-176), (relativeY-144), 352, 288, UnitPixel);
	
	delete imgLeftTop;
	delete imgTop;
	delete imgRightTop;
	delete imgLeftMiddle;
	delete imgMiddle;
	delete imgRightMiddle;
	delete imgLeftBottom;
	delete imgBottom;
	delete imgRightBottom;
	delete gNineMaps;
	}
	catch (...)
	{
		WriteLog("错误：DrawNineMaps 捕获取异常");
		return;
	}
	
}

void CFourthPicJMQ::DrawSignal(Graphics *graphics)
{
	int splitDest = 25;
	int splitSource = 40;
	int y = 72;
	//安全带
	if (1 == m_GnssMsg.aqd)
	{
		graphics->DrawImage(ImgSignal, Rect(0 * splitDest, y, splitDest, splitDest), 0, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(0 * splitDest, y, splitDest, splitDest), 0, 0, splitSource, splitSource, UnitPixel);
	}

	//脚刹
	if (1 == m_GnssMsg.js)
	{
		graphics->DrawImage(ImgSignal, Rect(1 * splitDest, y, splitDest, splitDest), 12 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(1 * splitDest, y, splitDest, splitDest), 12 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//手刹
	if (1 == m_GnssMsg.ss)
	{
		graphics->DrawImage(ImgSignal, Rect(2 * splitDest, y, splitDest, splitDest), 9 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(2 * splitDest, y, splitDest, splitDest), 9 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//副刹
	if (1 == m_GnssMsg.fs)
	{
		graphics->DrawImage(ImgSignal, Rect(3 * splitDest, y, splitDest, splitDest), 13 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(3 * splitDest, y, splitDest, splitDest), 13 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//喇叭
	if (1 == m_GnssMsg.lb)
	{
		graphics->DrawImage(ImgSignal, Rect(4 * splitDest, y, splitDest, splitDest), 7 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(4 * splitDest, y, splitDest, splitDest), 7 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//左转向
	if (1 == m_GnssMsg.zzx)
	{
		graphics->DrawImage(ImgSignal, Rect(5 * splitDest, y, splitDest, splitDest), 1 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(5 * splitDest, y, splitDest, splitDest), 1 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//右转向
	if (1 == m_GnssMsg.yzx)
	{
		graphics->DrawImage(ImgSignal, Rect(6 * splitDest, y, splitDest, splitDest), 2 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(6 * splitDest, y, splitDest, splitDest), 2 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//远光灯
	if (1 == m_GnssMsg.ygd)
	{
		graphics->DrawImage(ImgSignal, Rect(7 * splitDest, y, splitDest, splitDest), 4 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(7 * splitDest, y, splitDest, splitDest), 4 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//近光灯
	if (1 == m_GnssMsg.jgd)
	{
		graphics->DrawImage(ImgSignal, Rect(8 * splitDest, y, splitDest, splitDest), 3 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(8 * splitDest, y, splitDest, splitDest), 3 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

// 	//警示灯
// 	if (1 == m_GnssMsg.jsd)
// 	{
// 		graphics->DrawImage(ImgSignal, Rect(9 * splitDest, y, splitDest, splitDest), 14 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
// 	}
// 	else
// 	{
// 		graphics->DrawImage(ImgSignal, Rect(9 * splitDest, y, splitDest, splitDest), 14 * splitSource, 0, splitSource, splitSource, UnitPixel);
// 	}

	//熄火
	if (1 == m_GnssMsg.xh)
	{
		graphics->DrawImage(ImgSignal, Rect(9 * splitDest, y, splitDest, splitDest), 10 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(9 * splitDest, y, splitDest, splitDest), 10 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//开关门
	if (1 == m_GnssMsg.kgm)
	{
		graphics->DrawImage(ImgSignal, Rect(10 * splitDest, y, splitDest, splitDest), 6 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(10 * splitDest, y, splitDest, splitDest), 6 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//离合
	if (1 == m_GnssMsg.lh)
	{
		graphics->DrawImage(ImgSignal, Rect(11 * splitDest, y, splitDest, splitDest), 17 * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
	else
	{
		graphics->DrawImage(ImgSignal, Rect(11 * splitDest, y, splitDest, splitDest), 17 * splitSource, 0, splitSource, splitSource, UnitPixel);
	}

	//档位
	if (m_GnssMsg.dw >= 0 && m_GnssMsg.dw <= 5)
	{
		int nIndex = m_GnssMsg.dw + 18;
		graphics->DrawImage(ImgSignal, Rect(12 * splitDest, y, splitDest, splitDest), nIndex * splitSource, splitSource, splitSource, splitSource, UnitPixel);
	}
}

void CFourthPicJMQ::DrawTime(Graphics *graphics,int x,int y)
{
	CTimeSpan ctempSpan;
	ctempSpan = CTime::GetCurrentTime() - m_StartTime;
	//H 考试时长
	graphics->DrawImage(ImgTime,Rect(x+10*0,y,10,16), 10*(ctempSpan.GetHours()/10), 0,10,16,UnitPixel); 
	graphics->DrawImage(ImgTime,Rect(x+10*1,y,10,16), 10*(ctempSpan.GetHours()%10), 0,10,16,UnitPixel); 
	//:
	graphics->DrawImage(ImgTime,Rect(x+10*2,y,10,16), 100, 0,10,16,UnitPixel); 
	//M
	graphics->DrawImage(ImgTime,Rect(x+10*3,y,10,16), 10*(ctempSpan.GetMinutes()/10), 0,10,16,UnitPixel);
	graphics->DrawImage(ImgTime,Rect(x+10*4,y,10,16), 10*(ctempSpan.GetMinutes()%10), 0,10,16,UnitPixel); 
	//:
	graphics->DrawImage(ImgTime,Rect(x+10*5,y,10,16), 100, 0,10,16,UnitPixel); 
	//S
	graphics->DrawImage(ImgTime,Rect(x+10*6,y,10,16), 10*(ctempSpan.GetSeconds()/10), 0,10,16,UnitPixel); 
	graphics->DrawImage(ImgTime,Rect(x+10*7,y,10,16), 10*(ctempSpan.GetSeconds()%10), 0,10,16,UnitPixel); 
}

int CFourthPicJMQ::GetXMPi(int icode)
{
	int itype=0;

	if (m_uKSKM==2)//科目二
	{   //201510
		icode=icode/1000;
		
		if (!m_bBigCar)
		{
			if (201==icode)
			{
				itype=1;
			}
			else if (204==icode)
			{
				itype=2;
			}
			else if (203==icode)
			{
				itype=3;
			}
			else if (206==icode)
			{
				itype=4;
			}
			else if (207==icode)
			{
				itype=5;
			}
			else if (214==icode)
			{
				itype=7;
			}
			else if (215==icode)
			{
				itype=6;
			}
			else if (216==icode)
			{
				itype=6;
			}
			else
			{
				itype=0;
			}
		}
		else
		{
			if (202 == icode)	//桩考
			{
				itype = 1;
			}
			else if (204 == icode)	//侧方
			{
				itype = 2;
			}
			else if (203 == icode)	//定点
			{
				itype = 3;
			}
			else if (205 == icode)	//单边桥
			{
				itype = 4;
			}
			else if (209 == icode)	//连续障碍
			{
				itype = 5;
			}
			else if (208 == icode)	//限宽门
			{
				itype = 6;
			}
			else if (207 == icode)	//直角
			{
				itype = 7;
			}
			else if (206 == icode)	//曲线
			{
				itype = 8;
			}
			else if (210 == icode)	//起伏
			{
				itype = 9;
			}
			else if (211 == icode)	//掉头
			{
				itype = 10;
			}
			else if (213 == icode)	//急弯
			{
				itype = 11;
			}
			else if (216 == icode)	//湿滑
			{
				itype = 12;
			}
			else if (215 == icode)	//雨雾
			{
				itype = 13;
			}
			else if (214 == icode)	//隧道
			{
				itype = 14;
			}
			else if (212 == icode)	//高速
			{
				itype = 15;
			}
			else if (217 == icode || 218 == icode)	//紧急情况
			{
				itype = 16;
			}
			else
			{
				itype = 0;
			}
		}
		
	}
	else
	{
		if (icode==201)
		{
			itype=1;
		}
		else if (icode==202)
		{
			itype=2;
		}
		else if (icode==203)
		{
			itype=3;
		}
		else if (icode==204)
		{
			itype=5;
		}
		else if (icode==205)
		{
			itype=7;
		}
		else if (icode==206)
		{
			itype=10;
		}
		else if (icode==207)
		{
			itype=11;
		}
		else if (icode==208)
		{
			itype=12;
		}
		else if (icode==209)
		{
			itype=13;
		}
		else if (icode==210)
		{
			itype=14;
		}
		else if (icode==211)
		{
			itype=6;
		}
		else if (icode==212)
		{
			itype=15;
		}
		else if (icode==213)
		{
			itype=16;
		}
		else if (icode==214)
		{
			itype=8;
		}
		else if (icode==215)
		{
			itype=9;
		}
		else if (icode==216)
		{
			itype=7;
		}
		else if (icode==217)
		{
			itype=4;
		}
		else
		{
			itype=0;
		}
	}
	return itype;
}

void CFourthPicJMQ::OnGnssData(LPVOID msgz)
{
	if (threadRun==true)
	{
		memcpy((char *)&m_GnssMsg,msgz,sizeof(GNSSMSG));
		WriteStuData("%lf,%lf,%f,%f,%f",m_GnssMsg.gnssX,m_GnssMsg.gnssY,m_GnssMsg.gnssSD,m_GnssMsg.gnssLC,m_GnssMsg.gnssR);
		WriteStuData(
			"aqd=%d,js=%d,ss=%d,fs=%d,lh=%d,lb=%d,zzx=%d,yzx=%d,ygd=%d,jgd=%d,jsd=%d,xh=%d,kgm=%d,dw=%d",
			m_GnssMsg.aqd, m_GnssMsg.js, m_GnssMsg.ss, m_GnssMsg.fs, m_GnssMsg.lh, m_GnssMsg.lb, m_GnssMsg.zzx,
			m_GnssMsg.yzx, m_GnssMsg.ygd, m_GnssMsg.jgd, m_GnssMsg.jsd, m_GnssMsg.xh, m_GnssMsg.kgm, m_GnssMsg.dw);
		if (m_DrawMap==true)
		{		
			//计算 X Y
			int tempx,tempy;
			if (uDituPy==1)
			{
				tempx=abs((int)((m_GnssMsg.gnssX-m_Mapx)*m_bs))-176;
				tempy=abs((int)((m_GnssMsg.gnssY-m_Mapy)*m_bs))-144;
				WriteLog("qw2 gnssy=%f,tempy=%d,maxy=%lf", m_GnssMsg.gnssY, tempy,m_Mapy);
			}
			else
			{
				tempx=abs((int)((m_GnssMsg.gnssX-m_Mapx)*m_bs));//-176;
				tempy=abs((int)((m_GnssMsg.gnssY-m_Mapy)*m_bs));//-144;
			}
			if (tempx <0 || tempx >m_mayWH.x)
			{
				WriteLog("m_GnssMsg.gnssX=%lf,m_Mapx=%lf,m_bs=%lf",m_GnssMsg.gnssX,m_Mapx,m_bs);
			}
			else
			{
				
				m_imapx=tempx;
			}
			if (tempy <0 || tempy >m_mayWH.y)
			{
				WriteLog("m_GnssMsg.gnssY=%lf,m_Mapy=%lf,m_bs=%lf",m_GnssMsg.gnssY,m_Mapy,m_bs);
			}
			else
			{
				m_imapy=tempy;
			}

			//WriteLog("+176x=%d,y=%d",m_imapx,m_imapy);
		}
	}	
}

void CFourthPicJMQ::On17C51(CString zkzmbh,int ikscs,int idrcs)
{
	//	m_type =m_type | MTYPE1;
	stuDataName.Format("%s\\%s_%d_%d.data",m_NowDataPath,zkzmbh,ikscs,idrcs);
	threadRun=true;
	m_StartTime=CTime::GetCurrentTime();
	m_kscssj.Format("开始时间:%s",m_StartTime.Format("%H:%M:%S"));
	m_ErrorLine=0;
	for (int i=0;i<3;i++)
	{
		m_ErrorMSG[i][0]=_T("");
		m_ErrorMSG[i][1]=_T("");
		m_ErrorMSG[i][2]=_T("");
	}
	nowztstr.Format("开始考试...");
	m_i52type=0;
	m_i55type=0;
	ikscj=100;
	m_type =m_type | MTYPE1;
	SetEvent(hThreadEvent);
	WriteStuData("XMD*1*%s*%d*%d",zkzmbh,ikscs,idrcs);
}

void CFourthPicJMQ::On17C52(CString zkzmbh,int icode,CString smsg)
{
	//	m_type =m_type | MTYPE2;

	if (threadRun==false)
	{
		On17C51(zkzmbh,0,0);
	}
	//当前状态
	iDrXMP=GetXMPi(icode);
	SetMtype(m_i52type,iDrXMP);
	nowztstr.Format("%s",smsg);
	WriteStuData("XMD*2*%s*%d*%s",zkzmbh,icode,smsg);
}

void CFourthPicJMQ::On17C53(CString strTime,CString smsg,int ikcfs)
{
	//	m_type =m_type | MTYPE3;	
	if (m_ErrorLine>2)
	{
		m_ErrorLine=2;
	}
	m_ErrorMSG[m_ErrorLine][0].Format("%s",strTime);
	m_ErrorMSG[m_ErrorLine][1].Format("%s",smsg);
	m_ErrorMSG[m_ErrorLine][2].Format("扣%d分",ikcfs);
//	nowztstr.Format("%s",smsg);
	ikscj-=ikcfs;
	if (ikscj<0)
	{
		ikscj=0;
	}
	m_ErrorLine+=1;
	WriteStuData("XMD*3*%s*%d*%s",strTime,ikcfs,smsg);

}

void CFourthPicJMQ::On17C55(int icode,CString smsg)
{
	//	m_type =m_type | MTYPE5;
	nowztstr.Format("%s",smsg);	
	iDrXMP=GetXMPi(icode);
	SetMtype(m_i55type,iDrXMP);
	iDrXMP=0;
	WriteStuData("XMD*5**%d*%s",icode,smsg);
}

void CFourthPicJMQ::On17C56(int itype,int kscj)
{
	//	m_type =m_type | MTYPE6;
	ikscj=kscj;//考试成绩
	if (itype==1)
	{
		nowztstr.Format("考试合格!!!^_^                  ");
	}
	else
	{
		nowztstr.Format("考试不合格!!!                   ");
	}
	WriteStuData("XMD*6**%d*%d",itype,kscj);
	threadRun=false;
	iRunNum=20;
}

void CFourthPicJMQ::SetSleepTime(DWORD dwTime)
{
	m_SleepTime=dwTime;
	WriteLog("20171030 1042设置地图刷新率=(%d),1000表示1秒",m_SleepTime);
}

void CFourthPicJMQ::LoadMapCfg(CString path)
{
	m_sExePath = path;
	m_sMapPath = path + "\\map";
	
	CString temp;
	unsigned short StcharArr[256];
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\MAPN.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgMap=Image::FromFile(StcharArr);


	//////////////////////////////////////////////////////////////////////////

	

	
	
	//////////////////////////////////////////////////////////////////////////
	CString strCarN,configfile;
	configfile.Format("%s\\config.ini",path);	
	UINT uDrawCar;
	uDrawCar=GetPrivateProfileInt("CONFIG","DrawCar",0,configfile);
	if (uDrawCar==1)
	{
		strCarN.Format("%d",m_iCarNum);
		int carNo=GetPrivateProfileInt("CARSKIN",strCarN,0,configfile);
		memset(StcharArr,0x0,sizeof(StcharArr));
		temp.Format("%s\\Car%d.skin",path,carNo);
		WriteLog("加载config.ini CARSKIN[%s]",temp);
		swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
		ImgCAR=Image::FromFile(StcharArr);
		m_DrawCar=true;//画车模型
	}
	uDituPy=GetPrivateProfileInt("CONFIG","DITUPY",1,configfile);//地图偏移

	//huangqiwei
	UINT uDrawSignal;
	uDrawSignal = GetPrivateProfileInt("CONFIG","DrawSignal",0,configfile); 
	if (1 == uDrawSignal)
	{
		memset(StcharArr, 0x0, sizeof(StcharArr));
		temp.Format("%s\\signal.png", path);
		swprintf((wchar_t *)StcharArr, L"%s", temp.AllocSysString());
		ImgSignal = Image::FromFile(StcharArr);
		m_bDrawSignal = true;
	}
	
	UINT uNineMaps;
	uNineMaps = GetPrivateProfileInt("CONFIG","NineMaps",0,configfile); 
	if (1 == uNineMaps)
	{
		m_bNineMaps = true;
	}

	m_DrawMap=true;
	if (m_bNineMaps)
	{
		CString MapCfg,keyXname,keyYname;
		MapCfg.Format("%s\\HS_CONF_MAP.ini",path);
		int ixc,iyc;
		ixc=GetPrivateProfileInt("CONFIG","XC",0,MapCfg);
		iyc=GetPrivateProfileInt("CONFIG","YC",0,MapCfg);
		
		float maxX = 0.0;
		float minX = 0.0;
		float maxY = 0.0;
		float minY = 0.0;
		if(GetPrivateProfileString("CONFIG","MAXX","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			maxX=atof(temp);
		}
		else
		{
			WriteLog("HS_CONF_MAP.ini Get MAXX failed! %d",m_iCarNum);
		}
		if(GetPrivateProfileString("CONFIG","MINX","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			minX=atof(temp);
		}
		else
		{
			WriteLog("HS_CONF_MAP.ini Get MINX failed! %d",m_iCarNum);
		}
		if(GetPrivateProfileString("CONFIG","MAXY","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			maxY=atof(temp);
		}
		else
		{
			WriteLog("HS_CONF_MAP.ini Get MAXY failed! %d",m_iCarNum);
		}
		if(GetPrivateProfileString("CONFIG","MINY","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			minY=atof(temp);
		}
		else
		{
			WriteLog("HS_CONF_MAP.ini Get MINY failed! %d",m_iCarNum);
		}

		if (ixc==1)
		{
			m_Mapx = minX;
		}
		else
		{
			m_Mapx = maxX;
		}
		if (iyc==1)
		{
			m_Mapy = minY;
		}
		else
		{
			m_Mapy = maxY;
		}
		
		if(GetPrivateProfileString("CONFIG","ZoomIn","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			m_bs=atof(temp);
		}
		else
		{
			WriteLog("HS_CONF_MAP.ini Get ZoomIn failed! %d",m_iCarNum);
		}

		m_mayWH.x = (maxX - minX) * m_bs;
		m_mayWH.y = (maxY - minY) * m_bs;
	}
	else
	{
		m_mayWH.x=ImgMap->GetWidth();
		m_mayWH.y=ImgMap->GetHeight();
		
		CString MapCfg,keyXname,keyYname;
		MapCfg.Format("%s\\MAP.cfg",path);
		int ixc,iyc;
		ixc=GetPrivateProfileInt("MAPCONFIG","XC",0,MapCfg);
		iyc=GetPrivateProfileInt("MAPCONFIG","YC",0,MapCfg);
		if (ixc==1)
		{
			keyXname.Format("MINX");
		}
		else
		{
			keyXname.Format("MAXX");
		}
		if (iyc==1)
		{
			keyYname.Format("MINY");
		}
		else
		{
			keyYname.Format("MAXY");
		}
		if(GetPrivateProfileString("MAPCONFIG",keyXname,"",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			m_Mapx=atof(temp);
		}
		else
		{
			WriteLog("MAP.cfg Get %s failed! %d",keyXname,m_iCarNum);
		}
		if(GetPrivateProfileString("MAPCONFIG",keyYname,"",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			m_Mapy=atof(temp);
		}
		else
		{
			WriteLog("MAP.cfg Get MINY failed! %d",m_iCarNum);
		}
		
		if(GetPrivateProfileString("MAPCONFIG","ZoomIn","",temp.GetBuffer(MAX_PATH),MAX_PATH,MapCfg))
		{
			temp.ReleaseBuffer();
			m_bs=atof(temp);
		}
		else
		{
			WriteLog("MAP.cfg Get ZoomIn failed! %d",m_iCarNum);
		}
	}

	WriteLog("20171030 1042 LoadMapCfg OK!uDituPy=%d",uDituPy);
}

void CFourthPicJMQ::DrawMapTM()
{
	DWORD Rtime,Stime;//线程计时
	hThreadEvent =CreateEvent(NULL,FALSE,FALSE,NULL);
	SetEvent(hThreadEvent);
	WriteLog("FourthPic 线程开启,科目%d,动态地图版本20171011 1520!ThreadRun %d",m_uKSKM,m_iCarNum);
	Graphics grdc(m_ImageDC.GetSafeHdc());
	threadRun=false;
	
	m_StartTime=CTime::GetCurrentTime();
	CRect textrc[4];
	textrc[0].SetRect(0,0,348,30);
	textrc[1].SetRect(0,236,98,262);//速度
	textrc[2].SetRect(0,262,98,288);//里程
	textrc[3].SetRect(264,236,350,262);//成绩
	CRect errorRect[3];
	errorRect[0].SetRect(6,216,346,236);
	errorRect[1].SetRect(6,196,346,216);
	errorRect[2].SetRect(6,176,346,196);
	CString strtext;
	CFont fontz;//
	fontz.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");
	CFont fontzerror;//
	fontzerror.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体");
	
	m_ImageDC.SelectObject(&fontz);
	m_ImageDC.SetBkMode(TRANSPARENT);//透明
	m_ImageDC.SetTextColor(RGB(255,255,255));
	bool runerror=false;
	while (1)
	{
		WaitForSingleObject(hThreadEvent,INFINITE);
		Rtime=timeGetTime();
		//////////////////////////////////////////////////////////////////////////

		//WriteLog("qw m_imapx=%d,m_imapy=%d", m_imapx,m_imapy);

		//绘制地图
		if (m_bNineMaps)
		{
			DrawNineMaps(&grdc, m_imapx + 176, m_imapy + 144);
		}
		else
		{
			grdc.DrawImage(ImgMap,Rect(0,0,352,288),m_imapx,m_imapy,352,288,UnitPixel);//地图
		}
		
		if (m_DrawCar==true)//是否绘制考车
		{
			grdc.TranslateTransform(176,144);
			grdc.RotateTransform(m_GnssMsg.gnssR);//角度
			grdc.TranslateTransform(-176,-144);
			//绘制图片
			grdc.DrawImage(ImgCAR,Rect(0,0,352,288));//画车模型
			grdc.ResetTransform();
		}
		grdc.DrawImage(ImgMark,Rect(0,0,352,288));//遮罩
		
		strtext.Format("%s[%d]",nowztstr,m_iCarNum);
		m_ImageDC.DrawText(strtext,&textrc[0],DT_RIGHT | DT_SINGLELINE | DT_VCENTER );
		strtext.Format("%4.1fkm/h",m_GnssMsg.gnssSD);//117.92);//
		m_ImageDC.DrawText(strtext,&textrc[1],DT_RIGHT | DT_SINGLELINE | DT_VCENTER );
		strtext.Format("%6.1fm",m_GnssMsg.gnssLC);//5000.26);//
		m_ImageDC.DrawText(strtext,&textrc[2],DT_RIGHT | DT_SINGLELINE | DT_VCENTER );
		strtext.Format("成绩:%d",ikscj);		
		m_ImageDC.DrawText(strtext,&textrc[3],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		if (m_ErrorLine >0 && m_ErrorLine<=3)
		{
			m_ImageDC.SelectObject(&fontzerror);
			m_ImageDC.SetTextColor(RGB(255,0,0));
			for(int i=0;i<m_ErrorLine;i++)
			{
				strtext.Format("[%d] %s %s %s",i+1,m_ErrorMSG[i][0],m_ErrorMSG[i][1],m_ErrorMSG[i][2]);
				m_ImageDC.DrawText(strtext,&errorRect[i],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
			}
			m_ImageDC.SetTextColor(RGB(255,255,255));
			m_ImageDC.SelectObject(&fontz);
		}
		if (m_uKSKM==3)
		{
			if (iDrXMP>0 && iDrXMP<17)
			{	
				grdc.DrawImage(ImgXmp,Rect(0,0,72,72),0,iDrXMP*72,72,72,UnitPixel); 	
	 		}
		}
		DrawTime(&grdc,266,268);//考试时长

		//huangqiwei temp
		if (m_bDrawSignal)
		{
			DrawSignal(&grdc);
		}


#ifdef SAVE_PNGFILE
		SavePngFile();
		runerror=true;	
#else
		runerror=IsMSendDataT();
#endif		
		//////////////////////////////////////////////////////////////////////////
		if (runerror==false && threadRun==false)
		{
			TRACE("runerror==false \n");
			Sleep(100);
			SetEvent(hThreadEvent);
		}
		if (threadRun==true)
		{
			SetEvent(hThreadEvent);
		}
		else
		{
			if (iRunNum>0)
			{
				iRunNum-=1;
				SetEvent(hThreadEvent);
			}
		}
		Stime =timeGetTime()-Rtime;
		Stime =Stime>m_SleepTime?1:m_SleepTime-Stime;
		Sleep(Stime);
	}
	fontzerror.DeleteObject();
	fontz.DeleteObject();
	WriteLog("线程退出!动态地图版本.ThreadRun %d",m_iCarNum);
}

void CFourthPicJMQ::DrawXMListTM()
{
	DWORD Rtime,Stime;//线程计时
	hThreadEvent =CreateEvent(NULL,FALSE,FALSE,NULL);
 	SetEvent(hThreadEvent);
	WriteLog("FourthPic 线程开启!科目%d项目列表版本20171010 1019.ThreadRun %d",m_uKSKM,m_iCarNum);
	Graphics grdc(m_ImageDC.GetSafeHdc());
	threadRun=false;

	m_StartTime=CTime::GetCurrentTime();
	CRect textrc[4];
	textrc[0].SetRect(4,0,348,36);//状态
	textrc[3].SetRect(4,36,263,66);//成绩
	textrc[1].SetRect(4,66,263,88);//速度
	textrc[2].SetRect(4,88,263,114);//里程	

	CRect errorRect[6];
	errorRect[0].SetRect(2,117,264,145);
	errorRect[3].SetRect(2,145,264,173);
	errorRect[1].SetRect(2,174,264,202);
	errorRect[4].SetRect(2,202,264,230);
	errorRect[2].SetRect(2,231,264,259);
	errorRect[5].SetRect(2,259,264,287);
	CString strtext;
	CFont fontz,fontzerr;//
	fontz.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");

	fontzerr.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, DEFAULT_PITCH | FF_SWISS, "宋体");

	m_ImageDC.SelectObject(&fontz);
	m_ImageDC.SetBkMode(TRANSPARENT);//透明
	m_ImageDC.SetTextColor(RGB(255,255,255));
	bool runerror=false;
	while (1)
	{
		WaitForSingleObject(hThreadEvent,INFINITE);
		Rtime=timeGetTime();
		//////////////////////////////////////////////////////////////////////////
		if (m_type & MTYPE1)
		{
			m_type=m_type & (~ MTYPE1);
			grdc.DrawImage(ImgMark,Rect(0,0,352,288));//遮罩
			if (m_uKSKM==3 || m_bBigCar)	//大车的项目牌与科目三类似 
			{
				grdc.DrawImage(ImgXmp,Rect(264,0,88,288),0,0,88,288,UnitPixel);//项目牌
			}
			else
			{
				grdc.DrawImage(ImgXmp,Rect(264,36,88,252),0,0,88,252,UnitPixel);//项目牌
			}			
		}
		grdc.DrawImage(ImgMark,Rect(0,0,263,288),0,0,263,288,UnitPixel);
		//grdc.DrawImage(ImgMark,Rect(0,0,352,30),0,0,352,30,UnitPixel);
		strtext.Format("%s(%d)",nowztstr,m_iCarNum);
		m_ImageDC.DrawText(strtext,&textrc[0],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		strtext.Format("速度:%4.1fkm/h",m_GnssMsg.gnssSD);
		m_ImageDC.DrawText(strtext,&textrc[1],DT_LEFT | DT_SINGLELINE | DT_VCENTER );		
		
		m_ImageDC.DrawText(m_kscssj,&textrc[2],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		strtext.Format("时长:         成绩:%d",ikscj);		
		m_ImageDC.DrawText(strtext,&textrc[3],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		if (m_ErrorLine >0 && m_ErrorLine<=3)
		{
			//m_ImageDC.SetTextColor(RGB(255,204,0));
			m_ImageDC.SetTextColor(RGB(0,0,0));
			m_ImageDC.SelectObject(&fontzerr);
			for(int i=0;i<m_ErrorLine;i++)
			{
				strtext.Format("%s %s",m_ErrorMSG[i][0],m_ErrorMSG[i][2]);
				m_ImageDC.DrawText(strtext,&errorRect[i],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
				strtext.Format("%s",m_ErrorMSG[i][1]);
				m_ImageDC.DrawText(strtext,&errorRect[i+3],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
			}
			m_ImageDC.SelectObject(&fontz);
			m_ImageDC.SetTextColor(RGB(255,255,255));
		}
		
		DrawXMList(&grdc);
		DrawTime(&grdc,56,44);//考试时长
		//////////////////////////////////////////////////////////////////////////
#ifdef SAVE_PNGFILE
		SavePngFile();
		runerror=true;	
#else
		runerror=IsMSendDataT();
#endif		
		//////////////////////////////////////////////////////////////////////////
		if (runerror==false && threadRun==false)
		{
			TRACE("runerror==false \n");
			Sleep(100);
			SetEvent(hThreadEvent);
		}
		if (threadRun==true)
		{
			SetEvent(hThreadEvent);
		}
		else
		{
			if (iRunNum>0)
			{
				iRunNum-=1;
				SetEvent(hThreadEvent);
			}
		}
		Stime =timeGetTime()-Rtime;
		Stime =Stime>m_SleepTime?1:m_SleepTime-Stime;
		Sleep(Stime);
	}
	fontz.DeleteObject();
	WriteLog("线程退出!项目列表版本,ThreadRun %d",m_iCarNum);
}

void CFourthPicJMQ::DrawXMList(Graphics *graphics)
{
	int tempi=-1;
	tempi=GetMType(m_i52type);
	while (tempi!=0)
	{
		if (m_uKSKM==3 || m_bBigCar)
		{
			tempi-=1;
			
			if (tempi%2==0)
			{
				graphics->DrawImage(ImgXmp,Rect(264,(tempi/2)*36,44,36),88,(tempi/2)*36,44,36,UnitPixel);
			}
			else
			{
				graphics->DrawImage(ImgXmp,Rect(308,(tempi/2)*36,44,36),132,(tempi/2)*36,44,36,UnitPixel);
			}
			
		}
		else
		{
			graphics->DrawImage(ImgXmp,Rect(264,tempi*36,88,36),88,(tempi-1)*36,88,36,UnitPixel);
		}
		tempi=GetMType(m_i52type);
	}
	tempi=GetMType(m_i55type);
	while (tempi!=0)
	{
		if (m_uKSKM==3 || m_bBigCar)
		{
			tempi-=1;
			
			if (tempi%2==0)
			{
				graphics->DrawImage(ImgXmp,Rect(264,(tempi/2)*36,44,36),176,(tempi/2)*36,44,36,UnitPixel);
			}
			else
			{
				graphics->DrawImage(ImgXmp,Rect(308,(tempi/2)*36,44,36),220,(tempi/2)*36,44,36,UnitPixel);
			}
		}
		else
		{
			graphics->DrawImage(ImgXmp,Rect(264,tempi*36,88,36),176,(tempi-1)*36,88,36,UnitPixel);
		}
		tempi=GetMType(m_i55type);
	}
}

void CFourthPicJMQ::SetMtype(int &type, int idrxmp)
{
	switch (idrxmp)
	{
	case 1:
		type =type | MTYPE1;
		break;
	case 2:
		type =type | MTYPE2;
		break;
	case 3:
		type =type | MTYPE3;
		break;
	case 4:
		type =type | MTYPE4;
		break;
	case 5:
		type =type | MTYPE5;
		break;
	case 6:
		type =type | MTYPE6;
		break;
	case 7:
		type =type | MTYPE7;
		break;
	case 8:
		type =type | MTYPE8;
		break;
	case 9:
		type =type | MTYPE9;
		break;
	case 10:
		type =type | MTYPE10;
		break;
	case 11:
		type =type | MTYPE11;
		break;
	case 12:
		type =type | MTYPE12;
		break;
	case 13:
		type =type | MTYPE13;
		break;
	case 14:
		type =type | MTYPE14;
		break;
	case 15:
		type =type | MTYPE15;
		break;
	case 16:
		type =type | MTYPE16;
		break;
	}
}

int CFourthPicJMQ::GetMType(int &type)
{
	if (type & MTYPE1)
	{
		type=type & (~ MTYPE1);
		return 1;
	}
	if (type & MTYPE2)
	{
		type=type & (~ MTYPE2);
		return 2;
	}
	if (type & MTYPE3)
	{
		type=type & (~ MTYPE3);
		return 3;
	}
	if (type & MTYPE4)
	{
		type=type & (~ MTYPE4);
		return 4;
	}
	if (type & MTYPE5)
	{
		type=type & (~ MTYPE5);
		return 5;
	}
	if (type & MTYPE6)
	{
		type=type & (~ MTYPE6);
		return 6;
	}
	if (type & MTYPE7)
	{
		type=type & (~ MTYPE7);
		return 7;
	}
	if (type & MTYPE8)
	{
		type=type & (~ MTYPE8);
		return 8;
	}
	if (type & MTYPE9)
	{
		type=type & (~ MTYPE9);
		return 9;
	}
	if (type & MTYPE10)
	{
		type=type & (~ MTYPE10);
		return 10;
	}
	if (type & MTYPE11)
	{
		type=type & (~ MTYPE11);
		return 11;
	}

	if (type & MTYPE12)
	{
		type=type & (~ MTYPE12);
		return 12;
	}
	if (type & MTYPE13)
	{
		type=type & (~ MTYPE13);
		return 13;
	}
	if (type & MTYPE14)
	{
		type=type & (~ MTYPE14);
		return 14;
	}
	if (type & MTYPE15)
	{
		type=type & (~ MTYPE15);
		return 15;
	}
	if (type & MTYPE16)
	{
		type=type & (~ MTYPE16);
		return 16;
	}
	return 0;
}

void CFourthPicJMQ::SetNowDataPath(CString path,UINT uKSKM)
{
	m_NowDataPath=path;
	stuDataName.Format("%s\\%d_1.log",m_NowDataPath,m_iCarNum);
	m_uKSKM=uKSKM;
}

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

	m_type =m_type | MTYPE1;
	iRunNum=5;
	//////////////////////////////////////////////////////////////////////////
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
		if (m_DrawMap==true)
		{		
			//计算 X Y
			int tempx,tempy;
			if (uDituPy==1)
			{
				tempx=abs((int)((m_GnssMsg.gnssX-m_Mapx)*m_bs))-176;
				tempy=abs((int)((m_GnssMsg.gnssY-m_Mapy)*m_bs))-144;
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
	CString temp;
	unsigned short StcharArr[256];
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\MAPN.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgMap=Image::FromFile(StcharArr);

	m_mayWH.x=ImgMap->GetWidth();
	m_mayWH.y=ImgMap->GetHeight();
	//////////////////////////////////////////////////////////////////////////
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
	m_DrawMap=true;
	//////////////////////////////////////////////////////////////////////////
	CString strCarN,configfile;
	configfile.Format("%s\\config.ini",path);	
	UINT uDrawCar;
	uDrawCar=GetPrivateProfileInt("CONFIG","DrawCar",0,configfile);
	if (uDrawCar==1)
	{
		strCarN.Format("%d",m_iCarNum);
		ixc=GetPrivateProfileInt("CARSKIN",strCarN,0,configfile);
		memset(StcharArr,0x0,sizeof(StcharArr));
		temp.Format("%s\\Car%d.skin",path,ixc);
		WriteLog("加载config.ini CARSKIN[%s]",temp);
		swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
		ImgCAR=Image::FromFile(StcharArr);
		m_DrawCar=true;//画车模型
	}
	uDituPy=GetPrivateProfileInt("CONFIG","DITUPY",1,configfile);//地图偏移
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
		grdc.DrawImage(ImgMap,Rect(0,0,352,288),m_imapx,m_imapy,352,288,UnitPixel);//地图
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
			if (m_uKSKM==2)
			{
				grdc.DrawImage(ImgXmp,Rect(264,36,88,252),0,0,88,252,UnitPixel);//项目牌
			}
			else
			{
				grdc.DrawImage(ImgXmp,Rect(264,0,88,288),0,0,88,288,UnitPixel);//项目牌
			}			
		}
		grdc.DrawImage(ImgMark,Rect(0,0,263,288),0,0,263,288,UnitPixel);
		grdc.DrawImage(ImgMark,Rect(0,0,352,30),0,0,352,30,UnitPixel);
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
//		tempi-=1;
		if (m_uKSKM==2)
		{
			graphics->DrawImage(ImgXmp,Rect(264,tempi*36,88,36),88,(tempi-1)*36,88,36,UnitPixel);
		}
		else
		{
			if (tempi%2==0)
			{
				graphics->DrawImage(ImgXmp,Rect(264,(tempi/2)*36,44,36),88,(tempi/2)*36,44,36,UnitPixel);
			}
			else
			{
				graphics->DrawImage(ImgXmp,Rect(308,(tempi/2)*36,44,36),132,(tempi/2)*36,44,36,UnitPixel);
			}

		}
		tempi=GetMType(m_i52type);
	}
	tempi=GetMType(m_i55type);
	while (tempi!=0)
	{
//		tempi-=1;
		if (m_uKSKM==2)
		{
			graphics->DrawImage(ImgXmp,Rect(264,tempi*36,88,36),176,(tempi-1)*36,88,36,UnitPixel);
		}
		else
		{
			if (tempi%2==0)
			{
				graphics->DrawImage(ImgXmp,Rect(264,(tempi/2)*36,44,36),176,(tempi/2)*36,44,36,UnitPixel);
			}
			else
			{
				graphics->DrawImage(ImgXmp,Rect(308,(tempi/2)*36,44,36),220,(tempi/2)*36,44,36,UnitPixel);
			}
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

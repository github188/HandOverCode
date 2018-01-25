// JMQManager.cpp: implementation of the CJMQManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJMQ.h"
#include "JMQManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJMQManager::CJMQManager()
{
	AppLogID=logz.Register("合码器主程序.log");
	dwSLogID=logz.Register("HMQSuccess.log");
	dwFLogID=logz.Register("HMQError.log");
	CString tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	ModulePath.Format("%s",tempPath.Left(tempPath.ReverseFind('\\')));
	configfile.Format("%s\\config.ini",ModulePath);//当前目录下
	NET_DVR_Init();//海康SDK初始化
	
	uMergeVideo = 0;
	m_bBigCar = false;
	m_bEncrypt = false;

	UINT uBigCar = GetPrivateProfileInt("CONFIG","BigCar",0,configfile);	//大车科目二合格分数为90
	if (1 == uBigCar)
	{
		m_bBigCar = true;
	}

	UINT uEncrypt = GetPrivateProfileInt("CONFIG","ENCRYPT",0,configfile);
	if (1 == uEncrypt)
	{
		m_bEncrypt = true;
	}
}

CJMQManager::~CJMQManager()
{
	NET_DVR_Cleanup();//释放
	if (dllYuvToH264TH !=NULL)
	{
		FreeLibrary(dllYuvToH264TH);
	}
}
//////////////////////////////////////////////////////////////////////////

void CJMQManager::TermailDEInit()
{	
	LoadTFDll();//动态加载DLL
	DWORD dwVersion = NET_DVR_GetSDKBuildVersion();
	int jmqnum=GetPrivateProfileInt("JMQ","NUM",0,configfile);//读取合码器个数
	int EVEN=GetPrivateProfileInt("JMQ","EVEN",0,configfile);//读取是否隔行合码
	uKSKM=GetPrivateProfileInt("CONFIG","KSKM",2,configfile);//考试科目
	uWNDTWO=GetPrivateProfileInt("CONFIG","WND2",0,configfile);//画面二状态 0
	logz.WriteLog(AppLogID,"读取到%d台合码器!EVEN=[%d],科目%d,画面2=%d,HCNetSDK V%d.%d.%d.%d",jmqnum,EVEN,uKSKM,uWNDTWO,\
		(0xff000000 & dwVersion)>>24,(0x00ff0000 & dwVersion)>>16,(0x0000ff00 & dwVersion)>>8, (0x000000ff & dwVersion));
	CString lpAppName,lpKeyName;
	UINT ukch=0;
	m_DisPlayShowi[4]=GetPrivateProfileInt("CONFIG","VIDEOWND",1,configfile);
	for (int i=0;i<4;i++)
	{
		lpKeyName.Format("DISPLAY%d",i+1);
		m_DisPlayShowi[i]=GetPrivateProfileInt("CONFIG",lpKeyName,i,configfile);
		ukch+=m_DisPlayShowi[i];
	}
	if (ukch!=6)
	{
		for (int i=0;i<4;i++)
		{
			lpKeyName.Format("DISPLAY%d",i+1);
			lpAppName.Format("%d",i);
			::WritePrivateProfileString("CONFIG",lpKeyName,lpAppName,configfile);
			m_DisPlayShowi[i]=i;
		}
	}
	logz.WriteLog(AppLogID,"子窗口配置[%d,%d,%d,%d],音频窗口[%d]",m_DisPlayShowi[0],m_DisPlayShowi[1],m_DisPlayShowi[2],\
		m_DisPlayShowi[3],m_DisPlayShowi[4]);
	for (i=1;i<=jmqnum;i++)
	{
		if (OnDS63Init(i)==true)//设备初始化成功
		{
			lpAppName.Format("JMQ%d",i);
			for (int j=0;j<m_Ability.struDviInfo.byChanNums;j++)//DVI个数循环
			{
				if (EVEN==1 && j%2==1)
				{
					continue;
				}
				lpKeyName.Format("BNC%d",j+1);//从1开始
				ukch=GetPrivateProfileInt(lpAppName,lpKeyName,0,configfile);
				if (0 == ukch)//没有配置
				{
					logz.WriteLog(AppLogID,"合码器[JMQ%d]的BNC通道[%s]处于空闲,可以配置!",i,lpKeyName);
				}
				else
				{
					OnCheckChan(ukch,j);
				}
			}
		}
		else
		{
			logz.WriteLog(AppLogID,"合码器[JMQ%d]未初始化成功!",i);
		}
	}
}

bool CJMQManager::OnDS63Init(int id)
{
	CString temp,temp1;
	temp1.Format("%d",id);
	GetPrivateProfileString("JMQ",temp1,"",temp.GetBuffer(MAX_PATH),MAX_PATH,configfile);
	temp.ReleaseBuffer();
	int pos=temp.Replace(",",",");
	if (pos !=3)
	{
		logz.WriteLog(dwFLogID,"OnDS63Init:读取配置出错%d,[%s]",id,temp);
		return false;
	}
	CString sIPaddr,sUser,sPwd,sPort;
	pos =temp.Find(",");
	sIPaddr =temp.Left(pos);
	temp1=temp.Mid(pos+1);
	pos =temp1.Find(",");
	sUser=temp1.Left(pos);
	temp=temp1.Mid(pos+1);
	pos =temp.Find(",");
	sPwd =temp.Left(pos);
	sPort=temp.Mid(pos+1);
	//////////////////////////////////////////////////////////////////////////
	m_lUserID=-1;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//存放设备参数的结构体
	m_lUserID = NET_DVR_Login_V30(sIPaddr.GetBuffer(0),atoi(sPort),sUser.GetBuffer(0),sPwd.GetBuffer(0), &struDeviceInfo);
	if (m_lUserID < 0)
	{
		logz.WriteLog(dwFLogID,"登录失败! NET_DVR_Login_V30 failed! errid=%d,IP=%s,USER=%s,PWD=%s,PORT=%s",\
		NET_DVR_GetLastError(),sIPaddr,sUser,sPwd,sPort);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	char cTemp[SERIALNO_LEN] = {0};
	ZeroMemory(cTemp, SERIALNO_LEN);
	memcpy(cTemp, struDeviceInfo.sSerialNumber, SERIALNO_LEN);
	memset(&m_Ability, 0, sizeof(NET_DVR_MATRIX_ABILITY_V41));//获取设备能力集
    if (!NET_DVR_GetDeviceAbility(m_lUserID, MATRIXDECODER_ABILITY_V41, NULL, 0, (char*)&m_Ability, sizeof(NET_DVR_MATRIX_ABILITY_V41)))
    {
		logz.WriteLog(dwFLogID,"获取设备能力集失败! JMQ NET_DVR_GetDeviceAbility failed! errid=%d,TID=%d",NET_DVR_GetLastError(),id);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}	
	logz.WriteLog(AppLogID,"合码器=[%d],IP=%s,[%s],DecN=%d,BncN=%d",id,sIPaddr,cTemp,m_Ability.byDecChanNums,m_Ability.struBncInfo.byChanNums);
	//////////////////////////////////////////////////////////////////////////
	//获取重启 设置设备重启  重启后能解决很多事情
	NET_DVR_AUTO_REBOOT_CFG m_struRebootTime;
	memset(&m_struRebootTime, 0, sizeof(m_struRebootTime));
	DWORD dwReturned = 0;
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_AUTO_REBOOT_CFG, 0, &m_struRebootTime, sizeof(m_struRebootTime), &dwReturned))
	{
		logz.WriteLog(dwFLogID,"获取自动重启参数失败!JMQ NET_DVR_GET_AUTO_REBOOT_CFG failed!errid=%d,TID=%d",NET_DVR_GetLastError(),id);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
	}
	else
	{
		if (m_struRebootTime.struRebootTime.byDate!=7)
		{
			m_struRebootTime.dwSize = sizeof(NET_DVR_AUTO_REBOOT_CFG);
			m_struRebootTime.struRebootTime.byDate = 7;
			m_struRebootTime.struRebootTime.byHour = 5;
			m_struRebootTime.struRebootTime.byMinute = 9;
			if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_AUTO_REBOOT_CFG, 0, &m_struRebootTime, sizeof(m_struRebootTime)))
			{
				logz.WriteLog(dwFLogID,"设置自动重启参数失败!JMQ NET_DVR_GET_AUTO_REBOOT_CFG failed!errid=%d,TID=%d",NET_DVR_GetLastError(),id);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool CJMQManager::OnCheckChan(UINT kch,int bncn)//考车号 BNC编号
{
	DWORD dechanisopen;
	BYTE byDecChan[4];//解码通道
	DWORD dwDecChan;
	//解码通道检测
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<4;i++)
	{
		byDecChan[i]=m_Ability.byStartChan+bncn*4 + i;
		dwDecChan=byDecChan[i];
		if (NET_DVR_MatrixGetDecChanEnable(m_lUserID,dwDecChan/*byDecChan[i]*/,&dechanisopen))
		{
			if (0==dechanisopen)//指向DWORD的指针，取出的值0表示关闭 
			{
				if (!NET_DVR_MatrixSetDecChanEnable(m_lUserID,byDecChan[i],1))//打开通道
				{
					logz.WriteLog(dwFLogID,"NET_DVR_MatrixSetDecChanEnable failed!,errid=%d,打开通道%d失败!",NET_DVR_GetLastError(),byDecChan[i]);
					::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
					return false;
				}
			}
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	//显示通道检测
	DWORD dwDispChan;
	NET_DVR_MATRIX_VOUTCFG m_DispChanCfg;
	memset(&m_DispChanCfg, 0, sizeof(NET_DVR_MATRIX_VOUTCFG));	
	dwDispChan=m_Ability.struDviInfo.byStartChan+bncn;//显示通道
	//合码器获取显示通道配置
	if (!NET_DVR_MatrixGetDisplayCfg_V41(m_lUserID, dwDispChan, &m_DispChanCfg))
	{
		logz.WriteLog(dwFLogID,"获取显示通道配置失败! NET_DVR_MatrixGetDisplayCfg_V41 failed! errid=%d,channel=%d",NET_DVR_GetLastError(),dwDispChan);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
    }
	if ((4!=m_DispChanCfg.dwWindowMode) || \
		(m_DispChanCfg.byJoinDecChan[0] != byDecChan[m_DisPlayShowi[0]]) || \
		(m_DispChanCfg.byJoinDecChan[1] != byDecChan[m_DisPlayShowi[1]]) || \
		(m_DispChanCfg.byJoinDecChan[2] != byDecChan[m_DisPlayShowi[2]]) || \
		(m_DispChanCfg.byJoinDecChan[3] != byDecChan[m_DisPlayShowi[3]]) )//显示通道不是四分割输出
	{
		m_DispChanCfg.byJoinDecChan[0]=byDecChan[m_DisPlayShowi[0]];
		m_DispChanCfg.byJoinDecChan[1]=byDecChan[m_DisPlayShowi[1]];
		m_DispChanCfg.byJoinDecChan[2]=byDecChan[m_DisPlayShowi[2]];		
		m_DispChanCfg.byJoinDecChan[3]=byDecChan[m_DisPlayShowi[3]];
		m_DispChanCfg.byAudio=1;//开启音频
		m_DispChanCfg.byAudioWindowIdx= m_DisPlayShowi[4];//音频子窗口 1
		m_DispChanCfg.byVedioFormat =1;//视频制式：1- NTSC，2- PAL
		m_DispChanCfg.dwResolution= 67207228;
		m_DispChanCfg.dwWindowMode= 4;
		m_DispChanCfg.byScale= 0;//真实
		//解码器设置显示通道配置 
		if (!NET_DVR_MatrixSetDisplayCfg_V41(m_lUserID,dwDispChan,&m_DispChanCfg)) 
        {
			if (NET_DVR_GetLastError()==29)
			{
				logz.WriteLog(dwFLogID,"错误29:设备操作失败,请将合码器完全恢复下.设备管理=>恢复默认参数=>完成恢复");
			}
			logz.WriteLog(dwFLogID,"NET_DVR_MatrixSetDisplayCfg_V41 failed! errid=%d,DisPlayChann=%d",NET_DVR_GetLastError(),dwDispChan);
			::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
			return false;		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	m_JMQCar[kch].InitCar(kch,m_lUserID,byDecChan,dwSLogID,dwFLogID,ModulePath);//考车初始化
	if (kch>0 && kch<100)
	{
		dllTFInit(kch,UM_JGPTDATA,AfxGetMainWnd()->m_hWnd);
	}
	return true;
}

BOOL CJMQManager::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		logz.WriteLog(AppLogID,"初始化连接对象失败!CoInitialize NULL");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,configfile);
	m_strInEdit.ReleaseBuffer();
	if (!m_strInEdit.IsEmpty())
	{
		int iLen =m_strInEdit.GetLength();
		
		for(int i=0;i<iLen;i++)
		{
			m_strInEdit.SetAt(i,m_strInEdit[i]-1);
		}
		m_strInEdit.MakeReverse();
	}
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CString Sqlstr;
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
			logz.WriteLog(AppLogID,"成功连接到数据库!");
		}
		else
		{
			TRACE("打开数据库失败!");
			logz.WriteLog(AppLogID,"连接数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		errormessage.Format("%s\\sqlLink.exe",ModulePath);
		ShellExecute(NULL,"open",errormessage,"config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

BOOL CJMQManager::SetTerminalPZ()
{
	CString sqltemp,strNid;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;
		int sqlororacle=GetPrivateProfileInt("CONFIG","SQLORACLE",1,configfile);//SQL 还是 Oracle
		if(sqlororacle==0)
		{
			sqltemp.Format("SELECT 编号,设备IP,用户名,密码,端口号,通道号,TransMode,MediaIP,编号||'_'||Nid as N FROM TBKVideo order by 编号");//
			logz.WriteLog(AppLogID,"Oracle 版本");
		}
		else
		{
			sqltemp.Format("SELECT 编号,设备IP,用户名,密码,端口号,通道号,TransMode,MediaIP,编号+'_'+Nid as N FROM TBKVideo order by 编号");//
			logz.WriteLog(AppLogID,"SQL 版本");
		}

		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("N");
				if (vat.vt != VT_NULL)
				{
					strNid=(LPCSTR)_bstr_t(vat);
					vat =pSet->GetCollect("编号");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].TermainlBH,(LPCSTR)_bstr_t(vat),10);
					}
					vat =pSet->GetCollect("设备IP");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].TerminalIP,(LPCSTR)_bstr_t(vat),16);
					}
					vat =pSet->GetCollect("用户名");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].RasUser,(LPCSTR)_bstr_t(vat),16);
					}
					vat =pSet->GetCollect("密码");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].RasPassword,(LPCSTR)_bstr_t(vat),16);
					}
					vat =pSet->GetCollect("端口号");
					if (vat.vt != VT_NULL)
					{
						m_VideoPZ[strNid].TerminalPort=atoi((LPCSTR)_bstr_t(vat));
					}
					vat =pSet->GetCollect("通道号");
					if (vat.vt != VT_NULL)
					{
						m_VideoPZ[strNid].dwChannel=atoi((LPCSTR)_bstr_t(vat));
					}
					vat =pSet->GetCollect("TransMode");
					if (vat.vt != VT_NULL)
					{
						m_VideoPZ[strNid].imllx=atoi((LPCSTR)_bstr_t(vat));
					}
					vat =pSet->GetCollect("MediaIP");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].mediaIP,(LPCSTR)_bstr_t(vat),16);
					}
				}
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		logz.WriteLog(AppLogID,"查询硬盘录像机通道配置信息失败!FUNC SetTerminalPZ,%s",comError.Description());
		return FALSE;
	}
	logz.WriteLog(AppLogID,"读取数据库配置FUNC SetTerminalPZ 共%d条",m_VideoPZ.size());
	return TRUE;
}

BOOL CJMQManager::SetErrorData()
{
	CString sqltemp,strNid;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;

		if (m_bEncrypt)
		{
			sqltemp.Format("Set ARITHABORT ON;select 错误编号,dbo.charDecode(扣分类型) as 扣分类型,dbo.intDecode(扣除分数) as 扣除分数 from ErrorData");
		}
		else
		{
			sqltemp.Format("select 错误编号,扣分类型,扣除分数 from ErrorData");
		}

		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("错误编号");
				if (vat.vt != VT_NULL)
				{
					strNid=(LPCSTR)_bstr_t(vat);
					vat =pSet->GetCollect("扣分类型");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_Errordata[strNid].errorlx,(LPCSTR)_bstr_t(vat),30);
					}
					vat =pSet->GetCollect("扣除分数");
					if (vat.vt != VT_NULL)
					{
						m_Errordata[strNid].ikcfs=atoi((LPCSTR)_bstr_t(vat));
					}
				}
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		logz.WriteLog(AppLogID,"查询表ErrorData信息失败!FUNC SetErrorData,%s",comError.Description());
		return FALSE;
	}
	logz.WriteLog(AppLogID,"读取数据库配置FUNC SetErrorData 共%d条",m_Errordata.size());
	return TRUE;
}

BOOL CJMQManager::OnFindCar(int ikch)
{
	MAP_INT_JMQCar::iterator iter;	
	iter = m_JMQCar.find(ikch);	
	if(iter == m_JMQCar.end())
	{
		logz.WriteLog(dwFLogID,"OnFindCar not find car [%d]",ikch);
		return FALSE;
	}
	return TRUE;
}

void CJMQManager::RunMap()
{
	MAP_INT_JMQCar::iterator iter;
	CString temp;
	STerminalPZ temppz;
	LONG lph;
	uMergeVideo=GetPrivateProfileInt("CONFIG","MergeVideo",0,configfile);//是否合成视频
	
	for (iter=m_JMQCar.begin();iter!=m_JMQCar.end();iter++)
	{
		Sleep(10);
		temp.Format("考车%d_1",iter->first);	
		if(GetTerminalPZ(temppz,temp)==TRUE)
		{
			iter->second.StartDynamic(temppz,0);//车载视频动态
		}	
		
		if (0 == uMergeVideo)
		{
			temp.Format("考车%d_2",iter->first);	//副驾视频	
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				iter->second.StartDynamic(temppz,1);
			}
		}
		else
		{
			temp.Format("考车%d_10",iter->first);		//叠加后的视频
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				iter->second.StartDynamic(temppz,1);
			}	
		}


// 		temp.Format("考车%d_3",iter->first);	
// 		if(GetTerminalPZ(temppz,temp)==TRUE)
// 		{
// 			iter->second.StartDynamic(temppz,2);//车载视频动态
// 		}	
		if(iter->second.StartPassive(2,lph)==true)
		{
			dTFPassH(lph,iter->first,3);
		}
		if(iter->second.StartPassive(3,lph)==true)
		{
			dTFPassH(lph,iter->first,4);
		}
	}
}

BOOL CJMQManager::GetTerminalPZ(STerminalPZ &tpz,CString str)
{
	MAP_STR_VIDEOPZ::iterator iter;	
	iter = m_VideoPZ.find(str);	
	if(iter == m_VideoPZ.end())
	{
		logz.WriteLog(AppLogID,"警告:[%s]数据未配置,请配置下! GetTerminalPZ faild !",str);
		return FALSE;
	}
	tpz=iter->second;
	return TRUE;
}

void CJMQManager::OnJ17C51(int ikch, CString str)
{
	int ikscs,idrcs;
	GetCS(str,ikscs,idrcs);
//	m_JMQCar[ikch].On17C51(str,ikscs,idrcs);
	dTF17C51(ikch,str,ikscs,idrcs);
	//////////////////////////////////////////////////////////////////////////
	if (uWNDTWO > 1)
	{		
		CString temp;
		STerminalPZ temppz;
		temp.Format("10001_1");//201
		if (0 == uMergeVideo)	//不是采用合成视频的模式, 视频合成场景下，四合一不进行切换
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//第二画面进项目
			}
		}

	}
}

void CJMQManager::OnJGPTData(CString strmsg)
{
	CString strTemp;
	strTemp.Format("%s",strmsg);
	if(strTemp.Replace('*',',')==9)
	{
		int icmd;
		int ikch;
		CStringArray te;
		CutOutString(strTemp,te,',');
		if(te.GetSize()==10)
 		{
			ikch=atoi(te.GetAt(1));
			if (OnFindCar(ikch))
		 	{
				icmd=atoi(te.GetAt(2));
				switch (icmd)
				{
				case 1:
					OnJ17C51(ikch,te.GetAt(6));
					break;
				case 2:
					OnJ17C52(ikch,te.GetAt(6),te.GetAt(5));
					break;
				case 3:
					OnJ17C53(ikch,te.GetAt(5),te.GetAt(7));
					break;
				case 5:
					OnJ17C55(ikch,te.GetAt(6),te.GetAt(5));
					break;
				case 6:
					OnJ17C56(ikch,te.GetAt(6),te.GetAt(5));
					break;
				default:
					break;
				}
				logz.WriteLog(AppLogID,"考车%d:17C5%d,准考证明编号:[%s][%s]",ikch,icmd,te.GetAt(6),te.GetAt(5));
			}else
			{
				logz.WriteLog(AppLogID,"OnJGPTData OnFindCar Error:%s",strTemp);
			}
		}
	}
	else
	{
		logz.WriteLog(AppLogID,"OnJGPTData Error:%s",strTemp);
	}
}

void CJMQManager::OnJ17C56(int ikch,CString zkzmbh,CString smsg)
{
	int kshgcj=80;
	int ikscj=atoi(smsg);
	if(uKSKM==3)//科目三
	{
		kshgcj=90;
	}
	else
	{
		if (m_bBigCar)
		{
			kshgcj = 90;
		}
	}
	if(ikscj>=kshgcj)//考试合格
	{
		//m_JMQCar[ikch].On17C56(1,ikscj);
		dTF17C56(ikch,1,ikscj);
	}
	else//考试不合格
	{
		//m_JMQCar[ikch].On17C56(2,ikscj);
		dTF17C56(ikch,2,ikscj);
	}

	if (uWNDTWO > 1)
	{		
		CString temp;
		STerminalPZ temppz;
		temp.Format("10086_1");//201	
		if (0 == uMergeVideo)	//不是采用合成视频的模式, 视频合成场景下，四合一不进行切换
		{
			if (GetTerminalPZ(temppz,temp)==TRUE)	
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//第二画面进项目
			}

		}
	}
}

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI QHThreadFuncc(LPVOID param)
{
	XMcode * mycode =(XMcode *)param;
	CString errcode;
	int ikch=mycode->ikch;
	errcode.Format("Q%d",mycode->xmCode);
	CString temp,temp1;
	GetPrivateProfileString(errcode,"Time","2000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp1.Format("%s",temp);
	DWORD sleepd=(DWORD) atol(temp1);	
	Sleep(sleepd);
	
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,1,ikch);
// 	if (mycode->xmCode >500 && mycode->xmCode <510)
// 	{
// 		GetPrivateProfileString(errcode,"Time2","2000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
// 		temp1.Format("%s",temp);
// 		DWORD sleepd=(DWORD) atol(temp1);
// 		Sleep(sleepd);
// 		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,1,ikch);
// 	}
// 	if (mycode->xmCode ==249)
// 	{
// 		GetPrivateProfileString(errcode,"Time2","2000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
// 		temp1.Format("%s",temp);
// 		DWORD sleepd=(DWORD) atol(temp1);
// 		Sleep(sleepd);
// 		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,3,ikch);
// 	}
	delete [] mycode;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void CJMQManager::OnQH17C52(int ikch,int ibh)
{	
	CString temp;
	STerminalPZ temppz;
	temp.Format("%d_%d",iQhKCH[ikch],ibh);	
	if (0 == uMergeVideo)	//不是采用合成视频的模式, 视频合成场景下，四合一不进行切换
	{
		if(GetTerminalPZ(temppz,temp)==TRUE)
		{
			m_JMQCar[ikch].StartDynamic(temppz,1);//
		}
		
	}
	logz.WriteLog(AppLogID,"画面切换 OnQH17C52 考车%d,%s",ikch,temp);
}
//////////////////////////////////////////////////////////////////////////
void CJMQManager::OnJ17C52(int ikch, CString zkzmbh,CString smsg)
{
	if (uWNDTWO!=1)
	{
		CString temp;
		STerminalPZ temppz;
		temp.Format("%s_1",smsg);//201	
		if (0 == uMergeVideo)	//不是采用合成视频的模式, 视频合成场景下，四合一不进行切换
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//第二画面进项目
			}
			
		}
		//////////////////////////////////////////////////////////////////////////

		int xmcode=atoi(smsg);
// 		if ( (xmcode>=501 && xmcode <599) || 249==xmcode || 259==xmcode)
// 		{
// 			CFileFind FindDBFile;
// 			if (FindDBFile.FindFile("ZZIPChannel.dat"))
// 			{
// 				iQhKCH[ikch]=xmcode;
// 				XMcode * mycode =new XMcode;
// 				mycode->xmCode=xmcode;
// 				mycode->ikch=ikch;
// 				HANDLE hThread;
// 				hThread= CreateThread(NULL,0,QHThreadFuncc,mycode,0,0);
// 				CloseHandle(hThread);
// 				logz.WriteLog(AppLogID,"多画面切换!定点--湿滑路---隧道:%s",smsg);
// 			}
// 		}
		if (15010==xmcode|| 15020==xmcode || 15030==xmcode)
		{
			logz.WriteLog(AppLogID,"CFileFind 前面...定点:%d",xmcode);
			CFileFind FindDBFile;
			if (FindDBFile.FindFile("ZZIPChannel.dat"))
			{
				if ( 15010==xmcode|| 15020==xmcode || 15030==xmcode)
				{
				iQhKCH[ikch]=xmcode+1;
				XMcode * mycode =new XMcode;
				mycode->xmCode=xmcode;
				mycode->ikch=ikch;
				HANDLE hThread;
				hThread= CreateThread(NULL,0,QHThreadFuncc,mycode,0,0);
				CloseHandle(hThread);
				logz.WriteLog(AppLogID,"定点:%s",smsg);
				}
			}
			return ;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	int icode=atoi(smsg);
	int ite;
	if (icode>300 && icode<400)
	{
		ite=201510;
	}
	else if (icode>400 && icode<500)
	{
		ite=204510;
	}
	else if (icode>500 && icode<600)
	{
		ite=203510;
	}
	else if (icode>600 && icode<700)
	{
		ite=206510;
	}
	else if (icode>700 && icode<800)
	{
		ite=207510;
	}
	else
	{
		ite=icode;
	}
	//dTF17C52(ikch,zkzmbh,atoi(smsg),GetErrorKFLX(smsg));
	dTF17C52(ikch,zkzmbh,ite,GetErrorKFLX(smsg));
}

void CJMQManager::OnJ17C55(int ikch, CString zkzmbh,CString smsg)
{
	CString temp;
	if(uKSKM==3)//科目三
	{		
		STerminalPZ temppz;
		temp.Format("考车%d_2",ikch);	
		if (0 == uMergeVideo)	//不是采用合成视频的模式, 视频合成场景下，四合一不进行切换
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//车载视频动态第二画面车外
			}
			
		}		
		if (atoi(smsg)<700)
		{
			temp.Format("%d",atoi(smsg)+700);
		}
		else
		{
			temp.Format("%d",atoi(smsg));
		}
		//218 -->918
	}
	else
	{
		//201510==>201990
		//m_JMQCar[ikch].StopDynamic(1);
		//temp.Format("%d",atoi(smsg)/1000*1000+990);//302-->399

		temp.Format("%d",atoi(smsg)/1000*1000+990);//
	}

	int icode=atoi(smsg);
	int ite;
	if (icode>300 && icode<400)
	{
		ite=201510;
	}
	else if (icode>400 && icode<500)
	{
		ite=204510;
	}
	else if (icode>500 && icode<600)
	{
		ite=203510;
	}
	else if (icode>600 && icode<700)
	{
		ite=206510;
	}
	else if (icode>700 && icode<800)
	{
		ite=207510;
	}
	else
	{
		ite=icode;
	}

//	dTF17C55(ikch,atoi(smsg),GetErrorKFLX(temp));
	dTF17C55(ikch,ite,GetErrorKFLX(temp));
	if (uWNDTWO > 1)
	{		
		STerminalPZ temppz;
		temp.Format("10086_1");//201	
		if (0 == uMergeVideo)	//不是采用合成视频的模式, 视频合成场景下，四合一不进行切换
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//第二画面进项目
			}
			
		}
	}
}

void CJMQManager::CutOutString(CString source,  CStringArray& dest, char division)
{
	dest.RemoveAll();
	int nCount=0;
	for(int i=0;i< source.GetLength();i++)
	{
		if(source.GetAt(i)== division)
		{
			nCount=i;
			dest.Add(source.Left(i)); //去掉右边
			int nIndex=dest.GetSize()-1;
			for(int j=0;j <nIndex;j++)
			{
				dest[nIndex] = dest[nIndex].Right(dest[nIndex].GetLength()-dest[j].GetLength()-1); //去掉左边
			}
		}
	}
	//处理最后一个分隔符division右边的剩余部分。
	dest.Add(source);
	int nIndex2=dest.GetSize()-1;
	for(int j=0;j <nIndex2;j++)
	{
		dest[nIndex2] = dest[nIndex2].Right(dest[nIndex2].GetLength()-dest[j].GetLength()-1); //去掉左边
	}
}

CString CJMQManager::GetErrorKFLX(CString str)
{
	CString temp=_T("");
	MAP_STR_ERRORDATA::iterator iter;	
	iter = m_Errordata.find(str);	
	if(iter == m_Errordata.end())
	{
		logz.WriteLog(AppLogID,"警告:[%s]数据未配置,请配置下! GetErrorData faild !",str);
		temp.Format("%s...",str);
		return temp;
	}
	temp.Format("%s",iter->second.errorlx);
	return temp;
}

int CJMQManager::GetErrorKCFS(CString str)
{
	int ikcfs=0;
	MAP_STR_ERRORDATA::iterator iter;	
	iter = m_Errordata.find(str);	
	if(iter == m_Errordata.end())
	{
		logz.WriteLog(AppLogID,"警告:[%s]数据未配置,请配置下表ErrorData! GetErrorKFFS faild !",str);
		return ikcfs;
	}
	ikcfs=iter->second.ikcfs;
	return ikcfs;
}

void CJMQManager::OnJ17C53(int ikch, CString str,CString timestr)
{
	int pos=str.Find("@");
	int ecodei;
	if (pos <= 0)
	{
		logz.WriteLog(dwFLogID,"OnJ17C53 find @ 没有找到%d= %s",ikch,str);
		return ;
	}
	ecodei=atoi(str.Left(pos));
	CString temp;
	if(uKSKM==3)//科目三
	{		
		switch(ecodei)
		{
		case 201:
			temp.Format("起步");
			break;
		case 202:
			temp.Format("起步");
			break;
		case 203:
			temp.Format("直线");
			break;
		case 204:
			temp.Format("变更");
			break;
		case 205:
			temp.Format("直行");
			break;
		case 206:
			temp.Format("人行");
			break;
		case 207:
			temp.Format("学校");
			break;
		case 208:
			temp.Format("车站");
			break;
		case 209:
			temp.Format("会车");
			break;
		case 210:
			temp.Format("超车");
			break;
		case 211:
			temp.Format("靠边");
			break;
		case 212:
			temp.Format("掉头");
			break;
		case 213:
			temp.Format("夜间");
			break;
		case 214:
			temp.Format("左转");
			break;
		case 215:
			temp.Format("右转");
			break;
		case 216:
			temp.Format("直行");
			break;	
		case 217:
			temp.Format("加减");
			break;
		default:
			temp.Format("综合");
			break;
		}
	}
	else
	{		
		if (ecodei>201509 && ecodei<201700)
		{
			temp.Format("倒车入库");
		}
		else if (ecodei>204509 && ecodei<204700)
		{
			temp.Format("侧方停车");
		}
		else if (ecodei>203509 && ecodei<203700)
		{
			temp.Format("定点坡起");
		}
		else if (ecodei>206509 && ecodei<206700)
		{
			temp.Format("曲线行驶");
		}
		else if (ecodei>207509 && ecodei<207700)
		{
			temp.Format("直角转弯");
		}
		else if (ecodei==259)
		{
			temp.Format("雨雾湿滑");
		}
		else if (ecodei==249)
		{
			temp.Format("模拟遂道");
		}
		else
		{
			temp.Format("综合评判");
		}
	}
	str=str.Mid(pos+1);
	str.Replace("@","");
	//////////////////////////////////////////////////////////////////////////	
	int ikcfs=GetErrorKCFS(str);
	dTF17C53(ikch,temp,GetErrorKFLX(str),ikcfs);
}

void CJMQManager::OnGnssData(char *recvbuf, int ikch)
{
	if (OnFindCar(ikch))
	{
		//m_JMQCar[ikch].OnGnssData(recvbuf);
		dTF17C54(ikch,recvbuf);
	}
}

UINT CJMQManager::ListMSGThread(LPVOID pParam)
{
	CJMQManager *pInfo=(CJMQManager*)pParam;
// 	LIST_JGPT::iterator iter;
// 	JGPTLISTSTRU templs;
// 	bool isEmpty;
// 	Sleep(1000);
// 	while (1)
// 	{
// 		WaitForSingleObject(pInfo->ListhEvent,1500);
// 		isEmpty=pInfo->m_ListJgpt.empty();
// 		SetEvent(pInfo->ListhEvent);
// 		if (isEmpty)
// 		{
// 			TRACE("m_ListJgpt is Empty\n");
// 			Sleep(1500);
// 			continue;
// 		}
// 		WaitForSingleObject(pInfo->ListhEvent,1500);
// 		for (iter=pInfo->m_ListJgpt.begin();iter!=pInfo->m_ListJgpt.end();)
// 		{
// 			if(iter->irunnum==0)
// 			{
// 				iter->irunnum=5;				
// 				iter->ics--;
// 				if (iter->ics<0)
// 				{
// 					TRACE("删除%d,%d \n",iter->ikch,iter->ics);
// 					iter=pInfo->m_ListJgpt.erase(iter);
// 				}
// 				else
// 				{//重新触发
// 					TRACE("重新触发%d,%d \n",iter->ikch,iter->ics);
// 					pInfo->ReRunData(iter->ikch,iter->itype,iter->zkzmbh,iter->msg);
// 					iter++;
// 				}				
// 			}
// 			else
// 			{
// 				iter->irunnum--;
// 				TRACE("%d,%d,%d,%s \n",iter->ikch,iter->irunnum,iter->itype,iter->msg);
// 				iter++;
// 			}
// 		}
// 		SetEvent(pInfo->ListhEvent);
// 		Sleep(1000);
// 	}	
	return 0;
}

void CJMQManager::RunListMsgThread()
{
// 	ListhEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
// 	SetEvent(ListhEvent);
// 	AfxBeginThread(ListMSGThread,(LPVOID)this);
}

void CJMQManager::AddMSGList(int ikch,int itype,CString zkzmbh,CString msgstr,int iruntime)
{
// 	JGPTLISTSTRU templ;
// 	templ.ikch=ikch;
// 	templ.itype=itype;
// 	templ.ics=3;//做三次
// 	templ.irunnum=iruntime;
// 	templ.zkzmbh.Format("%s",zkzmbh);
// 	templ.msg.Format("%s",msgstr);
// 	
// 	WaitForSingleObject(ListhEvent,1500);
// 	m_ListJgpt.push_back(templ);
// 	SetEvent(ListhEvent);
}

void CJMQManager::DelMSGList(int ikch, int itype)
{
// 	if (OnFindCar(ikch))
// 	{
// 		LIST_JGPT::iterator iter;
// 		WaitForSingleObject(ListhEvent,1500);
// 		for (iter=m_ListJgpt.begin();iter!=m_ListJgpt.end();)
// 		{
// 			if ((iter->ikch==ikch) && (iter->itype==itype))
// 			{
// // 				if (itype==1)
// // 				{
// // //					m_JMQCar[ikch].SetZkzmbh(iter->zkzmbh);
// // 				}
// 				m_ListJgpt.erase(iter++);
// 				//logz.WriteLog(AppLogID,"DelMSGList %d,%d",ikch,itype);
// 			}
// 			else
// 			{
// 				++iter;
// 			}
// 		}
// 		SetEvent(ListhEvent);
// 	}
}

void CJMQManager::GetCS(CString str, int &ikscs, int &idrcs)
{
	CString sqltemp;
	try
	{
		ikscs=0;
		idrcs=0;
		VARIANT cnt;
		cnt.vt = VT_INT;

		if (m_bEncrypt)
		{
			sqltemp.Format("Set ARITHABORT ON;SELECT 考试次数,当日次数 FROM StudentInfo WHERE 准考证明编号=dbo.charEncode('%s') ",str);
		}
		else
		{
			sqltemp.Format("SELECT 考试次数,当日次数 FROM StudentInfo WHERE 准考证明编号='%s' ",str);
		}
		
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			vat =pSet->GetCollect("考试次数");
			if (vat.vt != VT_NULL)
			{
				ikscs=vat.intVal;
			}
			else
			{
				ikscs=0;
			}
			vat =pSet->GetCollect("当日次数");
			if (vat.vt != VT_NULL)
			{
				idrcs=vat.intVal;
			}
			else
			{
				idrcs=0;
			}	
		}//if		
	}
	catch (_com_error e) 
	{
		ikscs=0;
		idrcs=0;
		logz.WriteLog(AppLogID,"FUN GetCS com_error=%s,准考证明编号:%s",e.ErrorMessage(),str);
		try
		{
			if (m_pConn->GetState() == adStateOpen)
			{
				m_pConn->Close();
			}
			if (m_pConn)
			{
				m_pConn.Release();
				m_pConn = NULL;
			}
			OnInitSQL();
		}
		catch (_com_error e) 
		{
			logz.WriteLog(AppLogID,"OnprH264Format Close and Release com_error=%s",e.ErrorMessage());
		}
	}
}

void CJMQManager::ReRunData(int ikch, int itype, CString zkzm, CString msg)
{
	switch (itype)
	{
	case 0:		
		break;
	case 1:
		OnJ17C51(ikch,zkzm);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		OnJ17C56(ikch,zkzm,msg);
		break;
	default:
		break;
	}
	logz.WriteLog(AppLogID,"重新触发信息:考车%d,状态:%d,%s",ikch,itype,zkzm);
}

BOOL CJMQManager::LoadTFDll()
{
	dllYuvToH264TH = LoadLibrary(TEXT("CYuvToH264T.dll")); 
	if (dllYuvToH264TH == NULL) 
	{ 
		AfxMessageBox("加载 CYuvToH264T.dll 失败!"); 
		logz.WriteLog(AppLogID,"加载 CYuvToH264T.dll 失败!");
		return FALSE; 
	} 
	dllTFInit=(TFInit)GetProcAddress(dllYuvToH264TH,TEXT("TFInit"));
	dTFPassH=(TFPassH)GetProcAddress(dllYuvToH264TH,TEXT("TFPassiveHandle"));
	dTF17C51=(TF17C51)GetProcAddress(dllYuvToH264TH,TEXT("TF17C51"));
	dTF17C52=(TF17C52)GetProcAddress(dllYuvToH264TH,TEXT("TF17C52"));
	dTF17C53=(TF17C53)GetProcAddress(dllYuvToH264TH,TEXT("TF17C53"));
	dTF17C54=(TF17C54)GetProcAddress(dllYuvToH264TH,TEXT("TF17C54"));
	dTF17C55=(TF17C55)GetProcAddress(dllYuvToH264TH,TEXT("TF17C55"));
	dTF17C56=(TF17C56)GetProcAddress(dllYuvToH264TH,TEXT("TF17C56"));
	
	if (dllTFInit ==NULL || dTFPassH ==NULL || dTF17C51 ==NULL|| dTF17C52 ==NULL\
		|| dTF17C53 ==NULL|| dTF17C54 ==NULL|| dTF17C55 ==NULL|| dTF17C56 ==NULL)
	{
		logz.WriteLog(AppLogID,"加载 CYuvToH264T.dll 失败!");
		return FALSE;
	}
	return TRUE;
}

void CJMQManager::GetKCHArray(CStringArray &kcharr)
{
	MAP_INT_JMQCar::iterator iter;
	CString temp;
	for (iter=m_JMQCar.begin();iter!=m_JMQCar.end();iter++)
	{		
		temp.Format("%d",iter->first);
		kcharr.Add(temp);
	}
}

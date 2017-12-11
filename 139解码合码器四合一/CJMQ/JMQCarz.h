// JMQCarz.h: interface for the CJMQCarz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JMQCARZ_H__7D783AED_D50E_4CDA_A1C3_6D94A1D0F311__INCLUDED_)
#define AFX_JMQCARZ_H__7D783AED_D50E_4CDA_A1C3_6D94A1D0F311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJMQCarz  
{
public:		
	CJMQCarz();
	virtual ~CJMQCarz();
private:
	int m_iKCH;
	LONG m_lUserID;//设备登录ID
	BYTE m_deChannl[4];//考车所对应的解码通道号
	CWriteLog logz;//日志
	DWORD slogid;//JMQSuccess.log
	DWORD flogid;//JMQError.log
public:
	void OnGnssData(LPVOID msgz);
	void On17C56(int itype,int ikscj);
	void On17C55(int ic,CString msgz);
	void On17C53(CString timestr,CString msgz,int ikcfs);
	void On17C52(CString zkzm,int ic,CString msgz);
	void On17C51(CString zkzm,int idrcs,int ikscs);
	bool StartPassive(int iwnd,LONG &lph);//开启被动解码
	bool StopPassive(int iwnd);//停止被动解码
	bool StartDynamic(STerminalPZ tpz,int iwnd);//开启动态解码(STerminalPZ配置结构,窗口 0 车载 1项目)
	void StopDynamic(int iwnd);//停止动态解码(窗口 0 车载 1项目)
	//////////////////////////////////////////////////////////////////////////
	void InitCar(int iKch, LONG lUserID, BYTE (&deChan) [4],DWORD sid,DWORD fid,CString mPath);//初始化
};

#endif // !defined(AFX_JMQCARZ_H__7D783AED_D50E_4CDA_A1C3_6D94A1D0F311__INCLUDED_)

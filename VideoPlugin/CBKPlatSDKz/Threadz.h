// Threadz.h: interface for the CThreadz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_)
#define AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//线程基类 
class AFX_CLASS_EXPORT CThreadz  
{
public:
	DWORD Start(DWORD dwCreationFlags = 0);
	DWORD Stop (WORD timeout);			//等待一段时间
	DWORD Stop (bool bForceKill = false);
	DWORD Resume();						//恢复线程
	DWORD Suspend();					//挂起线程
	int GetPriority();					//获取线程优先级
	BOOL SetPriority(int priority);		//设置线程优先级
	CThreadz();
	virtual ~CThreadz();
protected:
	virtual DWORD ThreadMethod() = 0;
private:
	static DWORD WINAPI EntryPoint(LPVOID pArg);//多态性,调用子类的实际工作函数
	HANDLE	m_hThread;
	DWORD	m_dwTID;                    //Thread ID线程ID
	LPVOID	m_pParent;                  //this pointer of the parent CThread object
	DWORD	m_dwExitCode;               //Exit Code of the thread 线程退出码   
protected:
    LPTHREAD_START_ROUTINE    m_pThreadFunction;    //工作线程指针
    BOOL	m_runthread;                //线程是否继续运行的标志
};

#endif // !defined(AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_)

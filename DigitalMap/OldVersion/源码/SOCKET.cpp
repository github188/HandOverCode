// SOCKET.cpp: implementation of the CSOCKET class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SOCKET.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"

extern CCBKDigitalMapApp theApp;

#define MAX_BUF_SIZE 4096

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#pragma warning disable:C4786
//#pragma diag_suppress C4786

UINT server_thd(LPVOID p);

CSOCKET::CSOCKET()
{
	dequeOK = TRUE; //默认允许
	TotalCar = 0;  //当前考车数量
	iC_Dlg_F = 0;//考车对话框创建完成
//	InitSocket();
	AfxBeginThread(&server_thd, this);

}

CSOCKET::~CSOCKET()
{

}

/**********************************8
函数说明：写错误信息进入配置文件
************************************/
void CSOCKET::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

// void CSOCKET::WriteLog(CString szERR,CString dataFile)
// {
// 
// 	FILE *fp=fopen(dataFile,"a+");
// 	fprintf(fp,szERR);
// 	fclose(fp);
// }


void CSOCKET::InitSocket()
{
	//
	//初始化Winsock
	WSADATA wsaDATA;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaDATA);
	if(ret != 0)
	{
		CString err;
		err.Format("SOCKET初始化失败");
		WriteLog(err);
	}
	
	SOCKADDR_IN m_addr;
	

	
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons(9800);

	//创建新的套接字，这里是创建的流类型的套接字
	m_hSocket = socket(AF_INET, SOCK_STREAM,0);	

	/////////////////////

	int on = 1000;
	if(setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) < 0)

//	int timeout = 3000; //3s
	
//	int ret1=setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(const char *)&timeout,sizeof(timeout));
	
 //  int ret2=setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(const char *)&timeout,sizeof(timeout));
	/////////////////////


	//绑定一个套接字到本机的地址
	ret = 0;
	int error = 0;
	ret = bind(m_hSocket, (struct sockaddr*)&m_addr, sizeof(SOCKADDR_IN));
	if(ret == SOCKET_ERROR)
	{
		CString err;
		err.Format("SOCKET bind失败");
		WriteLog(err);
	}
	
	//开始一个侦听过程，等待用户的连接
	ret = listen(m_hSocket, 20);
	if(ret == SOCKET_ERROR)
	{
		CString err;
		err.Format("SOCKET listen失败");
		WriteLog(err);
	}


	
}


UINT server_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	map<int,UINT>::iterator it;

	while (!pCCBKDigitalMapDlg->pSocket.iC_Dlg_F)
		Sleep(1000);
	pCCBKDigitalMapDlg->pSocket.InitSocket();

	//非堵塞等待客户的连接
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	CSOCKET *pSocket = (CSOCKET *)p;
	char chData[MAX_BUF_SIZE];
	CFile destFile;
	CString strIni;	
	CString strlon;	
	CString strlat;
	CString strZT,strErrid;
	DOUBLE tplon = 0;
	DOUBLE tplat = 0;

	int nread;
//	char buff[100];
//	Package_ST pPackage_ST;
	Package_ST p_Package_ST[MAX_ExamCar];

	int iCountPackage = 0;
	CString R2;
	
	int Maxfd = 0;
	int fd_Num = 0; //有动作的描述符总数
	SOCKET sock;                                                  //这个用来接受一个连接
	fd_set rfd,copyrfd;                                //描述符集 这个将用来测试有没有一个可用的连接  备份读的集合
	struct timeval timeout;
	int client[FD_SETSIZE];		//用来存放客户端描述符的
	FD_ZERO(&rfd);                                                //总是这样先清空一个描述符集
	FD_ZERO(&copyrfd);
	timeout.tv_sec = 20;                                          //等下select用到这个
	timeout.tv_usec = 0;
	u_long ul = 1;
	ioctlsocket(pSocket->m_hSocket, FIONBIO, &ul);                         //用非阻塞的连接
		
	//现在开始用select
	FD_SET(pSocket->m_hSocket, &copyrfd);                                      //把sock放入要测试的描述符集 就是说把sock放入了rfd里面 这样下一步调用select对rfd进行
	Maxfd = pSocket->m_hSocket;//获取当前描述符的最大值
		
	int i = 0;
	for(i = 0; i < FD_SETSIZE; i++)
	client[i] = -1;
	struct timeval _timeout;
	_timeout.tv_sec = 5;
	_timeout.tv_usec = 0;
	while(pCCBKDigitalMapDlg->iContrl)
	{
		rfd = copyrfd;  //将备份的集合成员给读的集合
//			FD_SET(sock, &rfd);                                   //把sock放入读描述符集
//			FD_SET(sock, &wfd);                                   //把sock放入写描述符集
		fd_Num = select(Maxfd + 1, &rfd, NULL, 0, &_timeout);
		if (fd_Num == 0)
		{
			//各路考车没有动作  继续监听
			pCCBKDigitalMapDlg->WriteLog("继续监听");
			continue;
		}
		if(SOCKET_ERROR != fd_Num)       //测试这两个描述符集,永不超时 其中rfd只用来测试读 wfd只用来测试写
		{//没有错误

				if(FD_ISSET(pSocket->m_hSocket, &rfd))
				{//有一个描述符准备好了

					sock = accept(pSocket->m_hSocket,(struct sockaddr *)&client_addr, &iaddrSize);                           //一个用来测试读 一个用来测试写
					if(sock == SOCKET_ERROR)
					{
						CString err;
						err.Format("SOCKET 连接失败");
						pSocket->WriteLog(err);
					}
			//		pSocket->TotalCar = pSocket->TotalCar + 1;  //当前在线考车数
					for(i = 0; i < MAX_ExamCar; i++)
					{	if(client[i] < 0)
						{
							client[i] = sock;
							
							CString PPP;
							PPP.Format("%d %d 有辆考车连接上了！",i,sock);
							pSocket->WriteLog(PPP);

							break;
						}
					}
					if(sock > Maxfd)
						Maxfd = sock; 
					FD_SET(sock,&copyrfd);
					fd_Num --;
					if (fd_Num <= 0)
					{
						continue;
					}
					
					//		FD_ZERO(&rfd);
					//		FD_ZERO(&wfd);
				}

				//已连接FD产生可读事件
				
				for(i = 0; i < MAX_ExamCar; i++)  // FD_SETSIZE
				{		 
					
					if(FD_ISSET(client[i],&rfd))
					{
						sock = client[i];
						memset(chData,0,(sizeof(chData)/sizeof(char)));	
						
						nread = recv(sock, chData, MAX_BUF_SIZE, 0); // MSG_PEEK   MSG_OOB
						if(nread == -1 || nread == SOCKET_ERROR )
						{
							client[i] = -1; //客户端关闭
							FD_CLR(sock,&copyrfd);
							
							fd_Num --;
							pCCBKDigitalMapDlg->WriteLog("考车下线了");
							
						//	it = pSocket->SockToCar.find(sock);
							

			
							/*

							memset(&pPackage_ST,0,sizeof(Package_ST));
							pPackage_ST.pPackageHead_ST.PackageType = 10;// 代表关闭这个考车
							pPackage_ST.pExamCar_ST.ExamCarNumber = pSocket->SockToCar.find(sock)->second;
							//本来这个不是在这里处理的，后来出现问题了就只能再这里处理了
							//  问题现象是：如果有很多考车在线，然后有考车离线，这辆考车就再那里不动了 在解包那里处理不了，
							//  这个包它没有收到
							pCCBKDigitalMapDlg->m_Button[pPackage_ST.pExamCar_ST.ExamCarNumber].ExamCarState = 0;//考车离线
							pCCBKDigitalMapDlg->Mutex_Self[pPackage_ST.pExamCar_ST.ExamCarNumber].Lock();// 用于自身用的，主要用于考车离线
							while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST.pExamCar_ST.ExamCarNumber]));  //挂起线程
							Sleep(10);
							pCCBKDigitalMapDlg->Mutex_Self[pPackage_ST.pExamCar_ST.ExamCarNumber].Unlock();// 用于自身用的，主要用于考车离线
							pCCBKDigitalMapDlg->WriteLog("接收退出 进来   0");
							pCCBKDigitalMapDlg->pMutexForPG.Lock();
							CString XXX;
							XXX.Format("接收退出 进来 %d",pPackage_ST.pExamCar_ST.ExamCarNumber);
							pCCBKDigitalMapDlg->WriteLog(XXX);
					//		while (pSocket->dequeOK == FALSE)
					//			;
							//	Sleep(10);
					//		pSocket->dequeOK = FALSE;
							pSocket->pPackage_ST.push_back((Package_ST *)&pPackage_ST);
						//	Sleep(2);
					//		pSocket->dequeOK = TRUE;
							pCCBKDigitalMapDlg->pMutexForPG.Unlock();
							pCCBKDigitalMapDlg->WriteLog("接收退出 出去");



							*/
// 							it=mymap.find('b');
// 							mymap.erase (it); 
						//	pSocket->SockToCar.erase (it);  这句话为什么不用？
							if(fd_Num <= 0)
							{
							//	AfxMessageBox("关闭一个客户端");
								break;	
							}

							closesocket(sock);
							
						}
						else
						{
						//	pSocket->WriteLog(chData,"接收数据.txt");

							client[i] = -1; //客户端关闭
							FD_CLR(sock,&copyrfd);
					//		pSocket->TotalCar = pSocket->TotalCar - 1;
							
							destFile.Open(".\\接收数据.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::modeNoTruncate );	
							//	destFile.Write("[GPSInfo]\r\n", 11); // Write it
							destFile.SeekToEnd();
							destFile.Write(chData, nread); // Write it	
							destFile.Close();
							/*
							pCCBKDigitalMapDlg->WriteLog(chData);
							
							CStdioFile file;
							if(file.Open(_T("接收数据.txt"), CFile::modeWrite | CFile::typeText , NULL))		
							{		
								file.SeekToEnd();		
								file.WriteString(chData);		
								file.Close();		
							}
							*/

							destFile.Open(".\\Recv.sh", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);	
						//	destFile.Write("[GPSInfo]\r\n", 11); // Write it
							destFile.Write(chData, nread); // Write it	
							destFile.Close();
							
							for(i = 1; i <= MAX_ExamCar; i++)
							{
								//清空
								strIni = "";	
								strlon = "";	
								strlat = "";
									
							//	WaitForSingleObject(g_RecvFileEvent.m_hObject, 100);
								strIni.Format("CAR%dlon", i);			
								GetPrivateProfileString("GPSInfo",strIni.GetBuffer(MAX_PATH), NULL,strlon.GetBuffer(MAX_PATH),MAX_PATH,".\\Recv.sh");
								strIni.Format("CAR%ddla", i);
								GetPrivateProfileString("GPSInfo",strIni.GetBuffer(MAX_PATH), NULL,strlat.GetBuffer(MAX_PATH),MAX_PATH,".\\Recv.sh");
								strZT.Format("ZT%d",i);
								strErrid.Format("ErrID%d",i);
								UINT zt=GetPrivateProfileInt("GPSInfo",strZT.GetBuffer(MAX_PATH),1,".\\Recv.sh");
								UINT errid=GetPrivateProfileInt("GPSInfo",strErrid.GetBuffer(MAX_PATH),0,".\\Recv.sh");
							//	g_RecvFileEvent.SetEvent();
								//AfxMessageBox(strlon);
								//AfxMessageBox(strlat);
								
								if(strlat == "" || strlon == "")
								{
									TRACE("%d", strlon.GetLength());
								//	str.Format("strlat.IsEmpty   %d", strlat.GetLength());
									//AfxMessageBox(str);
									continue;
								}
							//	pCCBKDigitalMapDlg->WriteLog("qwerty1");
							//	iOnline++;
								tplon = atof(strlon);
								tplat = atof(strlat);

								if(tplon == 0 || tplat == 0)
								{
									continue;
								}


								////////   接下来要判断的是 如果有考车中间加上去的时候就要要让考车有对应的线程  /////////
								//	pCCBKDigitalMapDlg    pSocket

								/*
								if(iS == (pCCBKDigitalMapDlg->pSocket.TotalCar + 1) && pCCBKDigitalMapDlg->m_Button[iS - 1].examCarNumber != i)
								{
											
								//	pCCBKDigitalMapDlg->m_Button[i].examCarNumber = pCCBKDigitalMapDlg->SockToCar1.find(i)->second;
									pCCBKDigitalMapDlg->m_Button[iS].examCarNumber = i;
									pCCBKDigitalMapDlg->m_Button[iS].ThreadNumber = pCCBKDigitalMapDlg->m_Button[iS - 1].ThreadNumber++;
									pCCBKDigitalMapDlg->m_Button[iS].ExamCarState == 1;

									int iValue = 0;
									for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
									{
										if (pCCBKDigitalMapDlg->SockToCar1.find(iValue)->second == -1)
									//	if(m_Button[iValue].examCarNumber == 0)
										{
											pCCBKDigitalMapDlg->SockToCar1[iValue] = i;
										//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
											break;
										}
										
									}
									pCCBKDigitalMapDlg->pSocket.TotalCar++;
									CString tmpOnLineCar;
									tmpOnLineCar.Format("当前考车数为：%d",pCCBKDigitalMapDlg->pSocket.TotalCar);
									pCCBKDigitalMapDlg->m_TotalCar.SetWindowText(tmpOnLineCar);  
								}

								////////////////////////////////////////////////////*/


								memset(p_Package_ST+i,0,sizeof(Package_ST));
								int i_Find = 1;
						
								while (pCCBKDigitalMapDlg->m_Button[i_Find].examCarNumber != i)
								{
									i_Find ++;								
								}
								(p_Package_ST+i)->pPackageHead_ST.PackageType = 1;// 
								(p_Package_ST+i)->pExamCar_ST.ExamCarNumber = i;
								(p_Package_ST+i)->pExamCar_ST.y_Gps = tplat;//
								(p_Package_ST+i)->pExamCar_ST.x_Gps = tplon;//
								(p_Package_ST+i)->pExamCar_ST.zt =zt; //zzz
								(p_Package_ST+i)->pExamCar_ST.ErrorID=errid;//zzz
								(p_Package_ST+i)->pExamCar_ST.ThreadNumber = i_Find; //这个是线程号
								
								
								//  ThreadNumber
	

								//保存数据，数据内容为考车号以及当前GPS的值
								
		//						pSocket->SendString("192.168.0.62",MSG_GPS,temp.GetBuffer(0));
								pCCBKDigitalMapDlg->pMutexForPG.Lock();

								pSocket->pPackage_ST.push_back(p_Package_ST+i);
						//		Sleep(2);
						//		pSocket->dequeOK = TRUE;
								iCountPackage++;
								pCCBKDigitalMapDlg->pMutexForPG.Unlock();
						//		pCCBKDigitalMapDlg->WriteLog("接收 出去");
								
								
							//	pSocket->SockToCar[sock] = pPackage_ST.pExamCar_ST.ExamCarNumber;
								//看下有没有存在

							}
							
							closesocket(sock);

							

							fd_Num --;
							if(fd_Num <= 0)
							{
								break;	
							}
						}
					}
				}
			

		}

	}
	R2.Format("收包数：%d",iCountPackage);
	pCCBKDigitalMapDlg->WriteLog(R2);
	pCCBKDigitalMapDlg->WriteLog("Socket StretchDlg_thd");
	return 0;
}
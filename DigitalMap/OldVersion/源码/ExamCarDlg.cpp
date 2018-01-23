// ExamCarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cbkdigitalmap.h"
#include "ExamCarDlg.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKDigitalMapApp theApp;
extern unsigned long g_ulProcessId;
#define ExamCarN0 10
#define ExamCarN0Sub 1000



UINT SerachOnLineExamCar_thd(LPVOID p);  //用来判断此时的在线考车 对树状列表进行添加或删除操作
UINT RefreshCDDDlg_thd(LPVOID p);  //刷新开启消息框的的对话框
UINT TrackExamCar_thd(LPVOID p);  //追踪考车
/////////////////////////////////////////////////////////////////////////////
// CExamCarDlg dialog


CExamCarDlg::CExamCarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExamCarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExamCarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExamCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExamCarDlg)
	DDX_Control(pDX, IDC_TREE_EXAM, m_exam);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExamCarDlg, CDialog)
	//{{AFX_MSG_MAP(CExamCarDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_EXAM, OnSelchangedTreeChan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExamCarDlg message handlers

BOOL CExamCarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_exam.MoveWindow(1,1,260,388);  
	m_exam.DeleteAllItems();
// 	m_FirstDevItem = NULL;
// 	iExamCarCount = 0;
	CurTrackExamCar = -1;  //当前被跟踪的考车号 默认为-1
	
	InitMytree();  
	//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	int i = 0;
	for (i = 1;i < MAX_ExamCar ;i++)
	{
		ArryExamCarPopbox[i] = 0;//用来判断此时的提示框有没有开启  如果为1 则开启 如果为0 则没有开启
	}
	for (i = 1;i < MAX_ExamCar; i++)
	{
		
		if(((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].ExamCarState) //如果为在线
		{	
			CString tempCar;
			tempCar.Format("考车%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
			m_hDevItem = m_exam.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
			m_exam.SetItemData(m_hDevItem,(ExamCarN0 * i) + 10);
			HTREEITEM ExamCarItem = m_exam.InsertItem("开启追踪",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 1);   //Data对应通道在数组中的索引
			ExamCarItem = m_exam.InsertItem("开启提示框",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 2);   //Data对应通道在数组中的索引

			/////////////
			ExamCarItem = m_exam.InsertItem("开启主驾驶视频",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 3);   //Data对应通道在数组中的索引
			ExamCarItem = m_exam.InsertItem("开启副驾驶视频",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 4);   //Data对应通道在数组中的索引
			ExamCarItem = m_exam.InsertItem("开启车外视频",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 5);   //Data对应通道在数组中的索引
		}
	}
	
	iExamCarVideo = -1; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
	iExamCarVideot = -1; 
	iExamCarVideos = -1; 

	// 开线程 搜索在线的考车数
	AfxBeginThread(&SerachOnLineExamCar_thd, this);
	// 开线程 刷新开启消息框的提示框
	AfxBeginThread(&RefreshCDDDlg_thd, this);  
	//追踪考车
	AfxBeginThread(&TrackExamCar_thd, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CExamCarDlg::OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UpdateData(true);
	CString node_name;
	node_name=m_exam.GetItemText(pNMTreeView->itemNew.hItem);  //获取文本信息

	
	HTREEITEM hSelected = m_exam.GetSelectedItem();
//	BOOL SetItemText( HTREEITEM hItem, LPCTSTR lpszItem );

	
	
	//未选中
	if(NULL == hSelected)
		return;
	DWORD itemData = m_exam.GetItemData(hSelected);
	HTREEITEM hParent = NULL;
	int ExamCarNumber1 = itemData/1000;    // 第几辆考车  
	int ExamCarNumber = 0;    // 第某辆考车的线程号  
	int ExamCarNumberiIndex = itemData%1000;  //第几辆考车的子项
	CCBKDigitalMapDlg *pCBDigitalMaoDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////////    查找考车所对应的线程号  ////////////////////
	int j = 0;
	for (j = 1;j < MAX_ExamCar;j++)
	{
		if (ExamCarNumber1 == pCBDigitalMaoDlg->SockToCar1.find(j)->second)
		{
			//说明找到了 
			ExamCarNumber = j;
			break;
		}
	}
	/////////////////////////////////////////////////////////////
	switch(ExamCarNumber)
	{
	case 1://第一辆考车
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}

					ChangeTrack(hSelected);  
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
				{
					//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
					
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}

				break;
			
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 2:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 3:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 4:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 5:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 6:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 7:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 8:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 9:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
					
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 10:  //第10辆考车
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
					
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 11:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 12:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 13:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 14:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 15:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 16:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 17:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 18:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 19:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 20:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 21:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 22:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 23:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 24:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 25:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 26:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 27:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 28:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 29:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 30:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 31:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 32:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 33:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 34:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 35:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 36:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 37:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 38:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //开启追踪
				if (node_name == "开启追踪")  //说明此时已经开启追踪了
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"取消追踪");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "取消追踪")  //说明此时已经开启追踪了
				{
					m_exam.SetItemText(hSelected,"开启追踪");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//是否处于中心点 1 为是 0 为不是
				}
				break;
			case 2: //开启提示框
				if (node_name == "开启提示框")  //说明此时已经开启了
				{
					m_exam.SetItemText(hSelected,"取消提示框");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "取消提示框")
				{
					m_exam.SetItemText(hSelected,"开启提示框");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //开启主驾驶视频
				{
					if (node_name == "开启主驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"关闭主驾驶视频");
						if(iExamCarVideo == -1)
						{
							//说明只要打开视频
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideo();  //关闭实时预览视频
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭主驾驶视频")
					{
						CloseExamCarVideo();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启主驾驶视频"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //开启副驾驶视频
				{
					if (node_name == "开启副驾驶视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"关闭副驾驶视频");
						if(iExamCarVideot == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoT();  //关闭实时预览视频
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭副驾驶视频")
					{
						CloseExamCarVideoT();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启副驾驶视频"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //开启主驾驶视频
				{
					if (node_name == "开启车外视频")  //说明此时已经开启了   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"关闭车外视频");
						if(iExamCarVideos == -1)
						{
							//说明只要打开视频
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//先关闭视频后打开视频
							CloseExamCarVideoS();  //关闭实时预览视频
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号
					}
					else if (node_name == "关闭车外视频")
					{
						CloseExamCarVideoS();  //关闭实时预览视频
						m_exam.SetItemText(hSelected,"开启车外视频"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	default:
		break;
		
	}
	//////////////////    用来选择被跟踪的考车 使其考车用别的颜色显示      /////////////////////////
	int i = 0;
	for (i = 1;i < MAX_ExamCar;i++)
	{
		pCBDigitalMaoDlg->m_Button[i].bTrack = FALSE;	
	}
	if (CurTrackExamCar != -1)  //说明有考车被跟踪
	{
		pCBDigitalMaoDlg->m_Button[CurTrackExamCar].bTrack = TRUE;
	}
	
	//////////////////         结束       /////////////////////////
	
	hParent = m_exam.GetParentItem(hSelected);
	m_exam.SelectItem(hParent);
//	m_exam.SetItemState(hSelected, NULL, NULL);
	UpdateData(false);
	
	*pResult = 0;
}



BOOL CExamCarDlg::InitMytree()
{
	//节点的图标
	int i=0;
	int i_count=2;
	//载入图标
	HICON icon[4];
	icon[0] = AfxGetApp()->LoadIcon (IDI_ICON1);
	icon[1] = AfxGetApp()->LoadIcon (IDI_ICON2);
	
	//创建图像列表控件
	m_imagelist=new CImageList;
	m_imagelist->Create(16,16,0,7,7);
	m_imagelist->SetBkColor (RGB(255,255,255));
	for(int n=0;n<2;n++)
	{
		m_imagelist->Add(icon[n]); //把图标载入图像列表控件
	}
	m_exam.SetImageList(m_imagelist,TVSIL_NORMAL); //为m_mytree设置一个图像列表，使CtreeCtrl的节点显示不同的图标 
	m_exam.SetBkColor(RGB(160,210,230));//设置m_mytree的背景色
	
	return true;
}

UINT SerachOnLineExamCar_thd(LPVOID p)  //实时刷新在线考车
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CExamCarDlg *pExamCarDlg = (CExamCarDlg *)p;
	
	int i = 0;
	for (i = 0;i < MAX_ExamCar; i++) //初始化考车状态为离线  在线考车为1
	{
		pExamCarDlg->ArryExamCar[i] = FALSE;
	}
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);

	while (pCCBKDigitalMapDlg->iContrl)
	{
		for (i = 1; i < MAX_ExamCar;i++)
		{
			if (pCCBKDigitalMapDlg->m_Button[i].ExamCarState != pExamCarDlg->ArryExamCar[i]) //说明在线考车有刷新
			{
				//判断是考车上线还是下线
				if (FALSE == pExamCarDlg->ArryExamCar[i]) //说明是考车上线
				{
					//向树状列表添加考车
					CString tempCar;
					tempCar.Format("考车%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
					pExamCarDlg->m_hDevItem = pExamCarDlg->m_exam.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(pExamCarDlg->m_hDevItem,(ExamCarN0 * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)));
					HTREEITEM ExamCarItem = pExamCarDlg->m_exam.InsertItem("开启追踪",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 1);   //Data对应通道在数组中的索引
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("开启提示框",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 2);   //Data对应通道在数组中的索引

					/////////
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("开启主驾驶视频",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 3);   //Data对应通道在数组中的索引
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("开启副驾驶视频",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 4);   //Data对应通道在数组中的索引
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("开启车外视频",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 5);   //Data对应通道在数组中的索引
					/////////

					pExamCarDlg->ArryExamCar[i] = TRUE;

				}
				else
				{
					//说明考车下线
					//从树状列表中删除考车
					// 要先找到该条信息的索引
					HTREEITEM pCurItem;
					HTREEITEM temSub;
					CString szItemText;
					CString tmpItemText;
 					szItemText.Format("考车%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
 					pCurItem = pExamCarDlg->m_exam.GetFirstVisibleItem( );
 					tmpItemText = pExamCarDlg->m_exam.GetItemText(pCurItem );
 					while(szItemText != tmpItemText)
 					{
 						pCurItem = pExamCarDlg->m_exam.GetNextSiblingItem(pCurItem);
 						tmpItemText = pExamCarDlg->m_exam.GetItemText(pCurItem );
 					}
 					temSub = pExamCarDlg->m_exam.GetChildItem( pCurItem );
 					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
 					CString szText;
 					szText = pExamCarDlg->m_exam.GetItemText(temSub);
 					if (szText == "取消提示框")
 					{
 						pExamCarDlg->m_exam.SetItemText(temSub,"开启提示框");
 					}
 					pExamCarDlg->ArryExamCarPopbox[i] = 0;
					
// 					pCCBKDigitalMapDlg->m_Button[i].m_bHover = FALSE;
// 					pCCBKDigitalMapDlg->m_Button[i].Invalidate(FALSE);
					pCCBKDigitalMapDlg->dlgha[i].ShowWindow(SW_HIDE);
//					pCCBKDigitalMapDlg->Invalidate(FALSE);  后面注释
/*					Sleep(500);*/

					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
 					szText = pExamCarDlg->m_exam.GetItemText(temSub);
					if (szText == "关闭主驾驶视频")
					{
						pExamCarDlg->m_exam.SetItemText(temSub,"开启主驾驶视频");
						//发送一个消息
						pExamCarDlg->CloseExamCarVideo();  //关闭实时预览视频
						
						pExamCarDlg->iExamCarVideo = -1;
 					}

					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
					szText = pExamCarDlg->m_exam.GetItemText(temSub);
					if (szText == "关闭副驾驶视频")
					{
						pExamCarDlg->m_exam.SetItemText(temSub,"开启副驾驶视频");
						//发送一个消息
						pExamCarDlg->CloseExamCarVideoT();  //关闭实时预览视频
						
						pExamCarDlg->iExamCarVideot = -1;
 					}

					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
					szText = pExamCarDlg->m_exam.GetItemText(temSub);
					if (szText == "关闭车外视频")
					{
						pExamCarDlg->m_exam.SetItemText(temSub,"开启车外视频");
						//发送一个消息
						pExamCarDlg->CloseExamCarVideoS();  //关闭实时预览视频
						
						pExamCarDlg->iExamCarVideos = -1;
 					}


					pExamCarDlg->m_exam.DeleteItem(pCurItem);
					pExamCarDlg->ArryExamCar[i] = FALSE;
					if (pExamCarDlg->CurTrackExamCar == i)
					{
						pExamCarDlg->CurTrackExamCar = -1;
						pCCBKDigitalMapDlg->isRunningUpdate = 0; //如果是要下线的考车正是被跟踪的考车 则要运行这一条程序 停止刷新
					}
					pCCBKDigitalMapDlg->m_Button[i].m_bHover = FALSE;
					pExamCarDlg->ArryExamCarPopbox[i] = 0;
				}
			}
		}
		Sleep(300);
	}
	pCCBKDigitalMapDlg->WriteLog("考车 SerachOnLineExamCar_thd");
	return 0;
}


UINT RefreshCDDDlg_thd(LPVOID p)  //实时刷新在线考车的消息提示框
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CExamCarDlg *pExamCarDlg = (CExamCarDlg *)p;
	BOOL ArryExamCar[MAX_ExamCar];
	int i = 0;
	for (i = 0;i < MAX_ExamCar; i++) //初始化考车状态为离线  在线考车为1
	{
		ArryExamCar[i] = FALSE;
	}
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	
	while (pCCBKDigitalMapDlg->iContrl)
	{
		for (i = 1; i < MAX_ExamCar;i++)
		{
			//这里添加一个问题的解决 —— 就是当放大后移动画布时，如果此时某个考车已经已经开启提示框，然后考车又移出了屏幕之外
			// 则那个对话框还留在那里 解决思路为判断该点有没有在屏幕外，如果有就暂时关闭 提示框 刷新 在开启 再刷新
			if (pExamCarDlg->ArryExamCarPopbox[i])  //代表此时开启提示框
			{
				if (pCCBKDigitalMapDlg->MAPLeft > pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps || pCCBKDigitalMapDlg->MAPRight < pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps || pCCBKDigitalMapDlg->MAPTop < pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps || pCCBKDigitalMapDlg->MAPBotom > pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps )
				{
					//说明此时已经该考车已经在屏幕之外了
// 					pCCBKDigitalMapDlg->m_Button[i].m_bHover = FALSE;
// 					pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
// 					pCCBKDigitalMapDlg->m_Button[i].Invalidate(FALSE);
					pCCBKDigitalMapDlg->dlgha[i].ShowWindow(SW_HIDE);
//					pCCBKDigitalMapDlg->Invalidate(FALSE);
				}

				if (pCCBKDigitalMapDlg->m_Button[i].m_bHover == FALSE)
				{
					pCCBKDigitalMapDlg->m_Button[i].m_bHover = TRUE;
				}
				pCCBKDigitalMapDlg->m_Button[i].Invalidate(FALSE);
			}
			Sleep(20);
		}
	//	Sleep(0);
	}
	pCCBKDigitalMapDlg->WriteLog("考车 RefreshCDDDlg_thd");
	return 0;
}


//追踪考车
UINT TrackExamCar_thd(LPVOID p) 
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CExamCarDlg *pExamCarDlg = (CExamCarDlg *)p;
//	int i;
	DOUBLE x_TOLeft = 0;
	DOUBLE y_ToTop = 0;
	DOUBLE Left_Latitude = 0;//相对的纬度
	DOUBLE Top_Long = 0;//相对的经度
	DOUBLE Right_Latitude = 0;//相对的纬度
	DOUBLE Bottom_Long = 0;//相对的经度
	
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	int tmpExamCar = -1;
	DOUBLE x_Pixel = pCCBKDigitalMapDlg->x_pixel;  //保存此时的地图像素 主要用于如果地图放大或缩小时重新定位考车的坐标
	DOUBLE X_Left = pCCBKDigitalMapDlg->MAPLeft;//存放当前屏幕左边的纬度值 主要用于如果地图如果有拖动则触发重新定位
	DOUBLE X_Right = pCCBKDigitalMapDlg->MAPRight;//存放当前屏幕右边的纬度值 主要用于如果地图如果有拖动则触发重新定位
	DOUBLE Y_Top = pCCBKDigitalMapDlg->MAPTop;//存放当前屏幕上边的纬度值 主要用于如果地图如果有拖动则触发重新定位
	DOUBLE Y_Bottom = pCCBKDigitalMapDlg->MAPBotom;//存放当前屏下边的纬度值 主要用于如果地图如果有拖动则触发重新定位
	int xState = 0;  //用于屏幕拖动时 0 屏幕没有拖动 1 屏幕有拖动
	int xState1 = 0;// 这个是用来标记此时不是手动拖动地图而是自动移动地图的 如果为1 自动移动地图 0是手动移动地图

	DOUBLE OriginalLeft; //存放当前追踪后的屏幕的经度和纬度
	DOUBLE OriginalRight;
	DOUBLE OriginalTop;
	DOUBLE OriginalBottom;
	static CBrush brush(RGB(255,0,0));
	SetClassLong(pCCBKDigitalMapDlg->m_hWnd,GCL_HBRBACKGROUND,(LONG)(HBRUSH)brush);

	while (pCCBKDigitalMapDlg->iContrl)
	{
	//	CString PPP1;
	//	PPP1.Format("%d  followed",pExamCarDlg->CurTrackExamCar);
	//	pCCBKDigitalMapDlg->WriteLog(PPP1);
		if (pExamCarDlg->CurTrackExamCar != -1)  //说明有考车要被跟踪
		{
			/*
			CString PPP;
			PPP.Format("%d  examination of car being followed",pExamCarDlg->CurTrackExamCar);
			pCCBKDigitalMapDlg->WriteLog(PPP);
			*/
			if (tmpExamCar != pExamCarDlg->CurTrackExamCar)
			{
				xState1 = 0;
			}
			//首先判断此时的地图是不是原图 如果是原图就不做处理，不是则在做处理
			if (pCCBKDigitalMapDlg->MAPLeft != pCCBKDigitalMapDlg->OriginalMAPLeft || pCCBKDigitalMapDlg->MAPRight != pCCBKDigitalMapDlg->OriginalMAPRight)
			{
				pCCBKDigitalMapDlg->WriteLog("PPP2");
				if (X_Left != pCCBKDigitalMapDlg->MAPLeft || X_Right != pCCBKDigitalMapDlg->MAPRight || Y_Top != pCCBKDigitalMapDlg->MAPTop || Y_Bottom != pCCBKDigitalMapDlg->MAPBotom)
				{
					if (xState1 == 0)
					{
						X_Left = pCCBKDigitalMapDlg->MAPLeft;//存放当前屏幕左边的纬度值 主要用于如果地图如果有拖动则触发重新定位
						X_Right = pCCBKDigitalMapDlg->MAPRight;//存放当前屏幕右边的纬度值 主要用于如果地图如果有拖动则触发重新定位
						Y_Top = pCCBKDigitalMapDlg->MAPTop;//存放当前屏幕上边的纬度值 主要用于如果地图如果有拖动则触发重新定位
						Y_Bottom = pCCBKDigitalMapDlg->MAPBotom;//存放当前屏下边的纬度值 主要用于如果地图如果有拖动则触发重新定位
						xState = 1;
					}

				}
				//说明地图有放大
				//判断是不是正在追踪的考车 如果不是则重新将该考车尽量往屏幕中间靠 
				if (tmpExamCar != pExamCarDlg->CurTrackExamCar || x_Pixel != pCCBKDigitalMapDlg->x_pixel || xState == 1) //说明不是正在追踪的考车 或者 此时的地图有被放大或缩小过
				{
					xState = 0;
					tmpExamCar = pExamCarDlg->CurTrackExamCar;
					x_Pixel = pCCBKDigitalMapDlg->x_pixel;
					//尽量的将考车往屏幕中心移动
					//算出差值的经度纬度
					Left_Latitude = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].x_Gps - ((pCCBKDigitalMapDlg->CurrentScrWidth) / 2) * pCCBKDigitalMapDlg->x_pixel;  //算出了相对的屏幕左边纬度值
					Top_Long = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].y_Gps + ((pCCBKDigitalMapDlg->CurrentScrHeight) / 2) * pCCBKDigitalMapDlg->y_pixel;
					Right_Latitude = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].x_Gps + ((pCCBKDigitalMapDlg->CurrentScrWidth) / 2) * pCCBKDigitalMapDlg->x_pixel;  //算出了相对的屏幕左边纬度值
					Bottom_Long = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].y_Gps - ((pCCBKDigitalMapDlg->CurrentScrHeight) / 2) * pCCBKDigitalMapDlg->y_pixel;
					if (Left_Latitude >= pCCBKDigitalMapDlg->OriginalMAPLeft && Right_Latitude <= pCCBKDigitalMapDlg->OriginalMAPRight) //还在地图的范围内 x坐标可以在中间显示
					{
						pCCBKDigitalMapDlg->MAPLeft = Left_Latitude;
						pCCBKDigitalMapDlg->MAPRight = Right_Latitude;
						X_Left = pCCBKDigitalMapDlg->MAPLeft;
					}
					else
					{
						if (Left_Latitude >= pCCBKDigitalMapDlg->OriginalMAPLeft) //说明右边已经超出了 从右边确定x的在屏幕的值
						{
							pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->OriginalMAPRight;
							pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPRight - pCCBKDigitalMapDlg->CurrentScrWidth *  pCCBKDigitalMapDlg->x_pixel;
						}
						else //说明是左边已经超出了
						{
							pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->OriginalMAPLeft;
							pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPLeft + pCCBKDigitalMapDlg->CurrentScrWidth * pCCBKDigitalMapDlg->x_pixel;
						}
					}

					pCCBKDigitalMapDlg->WriteLog("PPP3");
					if (Bottom_Long >= pCCBKDigitalMapDlg->OriginalMAPBotom && Top_Long <= pCCBKDigitalMapDlg->OriginalMAPTop) //还在地图的范围内 y坐标可以在中间显示
					{
						pCCBKDigitalMapDlg->MAPBotom = Bottom_Long;
						pCCBKDigitalMapDlg->MAPTop = Top_Long;
					}
					else
					{
						if (Bottom_Long >= pCCBKDigitalMapDlg->OriginalMAPBotom) //说明上边已经超出了 从上边确定y的在屏幕的值
						{
							pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->OriginalMAPTop;
							pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->CurrentScrHeight * pCCBKDigitalMapDlg->y_pixel;
						}
						else //说明是下边已经超出了
						{
							pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->OriginalMAPBotom;
							pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPBotom + pCCBKDigitalMapDlg->CurrentScrHeight * pCCBKDigitalMapDlg->y_pixel;
						}
					}

					pCCBKDigitalMapDlg->x_StartBit = (pCCBKDigitalMapDlg->MAPLeft - pCCBKDigitalMapDlg->OriginalMAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
					pCCBKDigitalMapDlg->y_StartBit = (pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->MAPTop) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
					pCCBKDigitalMapDlg->x_BitWidth = (pCCBKDigitalMapDlg->MAPRight - pCCBKDigitalMapDlg->MAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
					pCCBKDigitalMapDlg->y_BitWidth = (pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->MAPBotom) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
					OriginalLeft = pCCBKDigitalMapDlg->MAPLeft;
					OriginalRight = pCCBKDigitalMapDlg->MAPRight;
					OriginalTop = pCCBKDigitalMapDlg->MAPTop;
					OriginalBottom = pCCBKDigitalMapDlg->MAPBotom;
					/*////////////////////////////////////////////////////////
					while (pCCBKDigitalMapDlg->isRunningUpdate);
					/////////////////////////////////////////////////////////*/
					Sleep(50);
					
					pCCBKDigitalMapDlg->isRunningUpdate = 1;
				//	pCCBKDigitalMapDlg->Suspend_Thread(); //挂起线程
//					Sleep(30);
//					pCCBKDigitalMapDlg->WriteLog("PPP4");
//					pCCBKDigitalMapDlg->Invalidate(FALSE);
//					Sleep(30); 
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->WriteLog("PPP5");

				}
				else
				{
					//说明此时没有对地图放大 和 拉动以及更换考车跟踪
					//看下此时该点在屏幕上的位置  看下有没有超出屏幕的百分之七十
					pCCBKDigitalMapDlg->WriteLog("PPP2A");
					int x_Cur = 0;  //当前在屏幕的坐标
					int y_Cur = 0;
					int iLeft,iRight,iTop,iBottom;
					DOUBLE x_Latitude1; // 纬度要偏移的相对值
					DOUBLE y_Long1; // 经度要偏移的相对值
					iLeft = pCCBKDigitalMapDlg->CurrentScrWidth * 0.5;
					iRight = pCCBKDigitalMapDlg->CurrentScrWidth - pCCBKDigitalMapDlg->CurrentScrWidth * 0.5;
					iTop = pCCBKDigitalMapDlg->CurrentScrHeight * 0.5;
					iBottom = pCCBKDigitalMapDlg->CurrentScrHeight - pCCBKDigitalMapDlg->CurrentScrHeight * 0.5;
					x_Cur = (pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
					y_Cur = (pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].y_Gps) / pCCBKDigitalMapDlg->y_pixel;
					if (x_Cur < iLeft) //说明此时要把画布往右边移动 
					{
						//如果此时的左边已经到了最左边则不处理 不是则往右边移动
						if (pCCBKDigitalMapDlg->MAPLeft > pCCBKDigitalMapDlg->OriginalMAPLeft)
						{
							//可以向右移动  移动的值为 分两种 第一种是移动后移到了地图的最左边 还一种是没有移动到最左边
							x_Latitude1 = (iLeft - x_Cur) * pCCBKDigitalMapDlg->x_pixel;
							if (pCCBKDigitalMapDlg->MAPLeft - x_Latitude1 >= pCCBKDigitalMapDlg->OriginalMAPLeft) //说明可以正常移动
							{
					//			pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//是否处于中心点 1 为是 0 为不是
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPLeft - x_Latitude1;
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPRight - x_Latitude1;
							}
							else //说明移动不了那么多了，因为已经到了最左边
							{
					//			pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
								x_Latitude1 = pCCBKDigitalMapDlg->MAPLeft - pCCBKDigitalMapDlg->OriginalMAPLeft;
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->OriginalMAPLeft;
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPRight - x_Latitude1;
							}
						}
					}
					if (x_Cur > iRight) //说明此时要把画布往左边移动 
					{
						//如果此时的右边已经到了最右边则不处理 不是则往左边移动
						if (pCCBKDigitalMapDlg->MAPRight < pCCBKDigitalMapDlg->OriginalMAPRight)
						{
							//可以向左移动  移动的值为 分两种 第一种是移动后移到了地图的最右边 还一种是没有移动到最左边
							x_Latitude1 = (x_Cur - iRight) * pCCBKDigitalMapDlg->x_pixel;
							if (pCCBKDigitalMapDlg->MAPRight + x_Latitude1 <= pCCBKDigitalMapDlg->OriginalMAPRight) //说明可以正常移动
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//是否处于中心点 1 为是 0 为不是
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPRight + x_Latitude1;
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPLeft + x_Latitude1;
							}
							else //说明移动不了那么多了，因为已经到了最右边
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
								x_Latitude1 = pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->MAPRight;
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->OriginalMAPRight;
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPLeft + x_Latitude1;
							}
						}
					}
					if (y_Cur > iBottom) //说明此时要把画布往上边移动 
					{
						//如果此时的下边已经到了最下边则不处理 不是则往上边移动
						if (pCCBKDigitalMapDlg->MAPBotom > pCCBKDigitalMapDlg->OriginalMAPBotom)
						{
							//可以向上移动  移动的值为 分两种 第一种是移动后移到了地图的最下边 还一种是没有移动到最下边
							y_Long1 = (y_Cur - iBottom) * pCCBKDigitalMapDlg->y_pixel;
							if (pCCBKDigitalMapDlg->MAPBotom - y_Long1 >= pCCBKDigitalMapDlg->OriginalMAPBotom) //说明可以正常移动
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//是否处于中心点 1 为是 0 为不是
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPBotom - y_Long1;
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPTop - y_Long1;
							}
							else //说明移动不了那么多了，因为已经到了最右边
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
								y_Long1 = pCCBKDigitalMapDlg->MAPBotom - pCCBKDigitalMapDlg->OriginalMAPBotom;
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->OriginalMAPBotom;
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPTop - y_Long1;
							}
						}
					}
					if (y_Cur < iTop) //说明此时要把画布往下边移动 
					{
						//如果此时的下边已经到了最上边则不处理 不是则往下边移动
						if (pCCBKDigitalMapDlg->MAPTop < pCCBKDigitalMapDlg->OriginalMAPTop)
						{
							//可以向下移动  移动的值为 分两种 第一种是移动后移到了地图的最上边 还一种是没有移动到最上边
							y_Long1 = (iTop - y_Cur) * pCCBKDigitalMapDlg->y_pixel;
							if (pCCBKDigitalMapDlg->MAPTop + y_Long1 <= pCCBKDigitalMapDlg->OriginalMAPTop) //说明可以正常移动
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//是否处于中心点 1 为是 0 为不是
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPTop + y_Long1;
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPBotom + y_Long1;
							}
							else //说明移动不了那么多了，因为已经到了最右边
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
								y_Long1 = pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->MAPTop;
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->OriginalMAPTop;
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPBotom + y_Long1;
							}
						}
					}
					pCCBKDigitalMapDlg->WriteLog("PPP2B");
					if (OriginalLeft != pCCBKDigitalMapDlg->MAPLeft || OriginalRight != pCCBKDigitalMapDlg->MAPRight || OriginalTop != pCCBKDigitalMapDlg->MAPTop || OriginalBottom != pCCBKDigitalMapDlg->MAPBotom)
					{
						xState1 = 1;
						pCCBKDigitalMapDlg->x_StartBit = (pCCBKDigitalMapDlg->MAPLeft - pCCBKDigitalMapDlg->OriginalMAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
						pCCBKDigitalMapDlg->y_StartBit = (pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->MAPTop) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
						pCCBKDigitalMapDlg->x_BitWidth = (pCCBKDigitalMapDlg->MAPRight - pCCBKDigitalMapDlg->MAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
						pCCBKDigitalMapDlg->y_BitWidth = (pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->MAPBotom) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
					
						pCCBKDigitalMapDlg->WriteLog("PPP6");
						/*/////////////////一下代码本来是有的，由于现场会一直在循环里面出不来 所以就把它去了
						
						while (pCCBKDigitalMapDlg->isRunningUpdate);
						
						////////////////////////////////////////////////*/
						pCCBKDigitalMapDlg->WriteLog("PPP7");

						Sleep(50);
						pCCBKDigitalMapDlg->isRunningUpdate = 1;
					//	pCCBKDigitalMapDlg->Suspend_Thread(); //挂起线程
//						Sleep(30);
//						pCCBKDigitalMapDlg->Invalidate(FALSE);
//						Sleep(50);
						pCCBKDigitalMapDlg->isRunningUpdate = 0;
						OriginalLeft = pCCBKDigitalMapDlg->MAPLeft;
						OriginalRight = pCCBKDigitalMapDlg->MAPRight;
						OriginalTop = pCCBKDigitalMapDlg->MAPTop;
						OriginalBottom = pCCBKDigitalMapDlg->MAPBotom;
						Sleep(20);
				//		Sleep(1000);
						
					}
					if (pCCBKDigitalMapDlg->MAPLeft == pCCBKDigitalMapDlg->OriginalMAPLeft || pCCBKDigitalMapDlg->MAPRight == pCCBKDigitalMapDlg->OriginalMAPRight || pCCBKDigitalMapDlg->MAPBotom == pCCBKDigitalMapDlg->OriginalMAPBotom || pCCBKDigitalMapDlg->MAPTop == pCCBKDigitalMapDlg->OriginalMAPTop)
					{
						pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是
					}
					else
					{
						pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//是否处于中心点 1 为是 0 为不是
					}

					pCCBKDigitalMapDlg->WriteLog("PPP2C");
					if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar != -1)
					{
/*						
						for (i = 1;i < MAX_ExamCar;i++)
						{
							if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar != -1)
							{
								if (pCCBKDigitalMapDlg->m_Button[i].ExamCarState == 1) //说明考车在线
								{
									pCCBKDigitalMapDlg->m_Button[i].Cur_xPointRelyOnParent = (pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
									pCCBKDigitalMapDlg->m_Button[i].Cur_yPointRelyOnParent = pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel);
									if (pCCBKDigitalMapDlg->m_Button[i].isCircle == 1)//说明在中心
									{
										pCCBKDigitalMapDlg->m_Button[i].MoveWindow(pCCBKDigitalMapDlg->CurrentScrWidth / 2, pCCBKDigitalMapDlg->CurrentScrHeight / 2,20,20);
									}
									else
									{

										while (pCCBKDigitalMapDlg->isRunningUpdate);
										pCCBKDigitalMapDlg->isRunningUpdate = 1;
										Sleep(50);
										pCCBKDigitalMapDlg->m_Button[i].MoveWindow(int((pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel),int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel)),20,20);
										Sleep(50);
										pCCBKDigitalMapDlg->isRunningUpdate = 0;

									}
								}
							}
							else
							{
								break;
							}
						}
*/
					//	pCCBKDigitalMapDlg->WakeUp_Thread();  //唤醒线程
						
					}


										
				}
			}
		}
 		else
 		{
 			pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//是否处于中心点 1 为是 0 为不是  考车自由走
			Sleep(100);
 		}
		Sleep(10);
	}
	pCCBKDigitalMapDlg->WriteLog("考车 TrackExamCar_thd");
	return 0;
}

void CExamCarDlg::ChangeTrack(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //当前子项的父项
	HTREEITEM temSub; //子项获取
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //获得第一项
	while (tem != NULL) //如果返回空说明已经到了尾端了  退出
	{
		if (tem != tem1) //此时获取的父项不是选择的那个父项
		{
			//查看此时追踪的状态是开启还是取消
			//获取子项的第一项
			temSub = m_exam.GetChildItem( tem );
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "取消追踪")
			{
				m_exam.SetItemText(temSub,"开启追踪");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}


}
//
BOOL CExamCarDlg::OpenExamCarVideo(UINT ExamCarNo,UINT VideoAdd)
{	
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	if(pCbkdlg->iISCreate == 0)
	{
		//说明还没有创建
			BOOL bRET = FALSE;
			bRET = pCbkdlg->CreateVideoDlg();
			if(bRET == FALSE)
				return FALSE;

			/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
			WriteLog("成功后1");
			pCbkdlg->SendMessageToVideoDlg(ExamCarNo,VideoAdd);
			pCbkdlg->ExamCarInfo.GetDlgItem(IDC_BUTTONMAIN)->SetWindowText("关主驾");
			//////////////////////////////////
	}
	else
	{
		/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
		pCbkdlg->SendMessageToVideoDlg(ExamCarNo,VideoAdd);
		//////////////////////////////////
	}

	return TRUE;
}

BOOL CExamCarDlg::CloseExamCarVideo()    
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
	pCbkdlg->SendMesageToVideoDlgClose();
	//////////////////////////////////

	return TRUE;
}

void CExamCarDlg::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

void CExamCarDlg::ChangeTrackVideoPlay(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //当前子项的父项
	HTREEITEM temSub; //子项获取
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //获得第一项
	while (tem != NULL) //如果返回空说明已经到了尾端了  退出
	{
		if (tem != tem1) //此时获取的父项不是选择的那个父项
		{
			//查看此时追踪的状态是开启还是取消
			//获取子项的第一项
			temSub = m_exam.GetChildItem( tem );
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "关闭主驾驶视频")
			{
				m_exam.SetItemText(temSub,"开启主驾驶视频");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}


}

BOOL CExamCarDlg::OpenExamCarVideoT(UINT ExamCarNo, UINT VideoAdd)
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	if(pCbkdlg->iISCreateT == 0)
	{
		//说明还没有创建
			BOOL bRET = FALSE;
			bRET = pCbkdlg->CreateVideoDlgT();
			if(bRET == FALSE)
				return FALSE;

			/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
			WriteLog("成功后1");
			pCbkdlg->SendMessageToVideoDlgT(ExamCarNo,VideoAdd);
			pCbkdlg->ExamCarInfo.GetDlgItem(IDC_BUTTONCOPILOT)->SetWindowText("关副驾");
			//////////////////////////////////
	}
	else
	{
		/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
		pCbkdlg->SendMessageToVideoDlgT(ExamCarNo,VideoAdd);
		//////////////////////////////////
	}

	return TRUE;
}

BOOL CExamCarDlg::OpenExamCarVideoS(UINT ExamCarNo, UINT VideoAdd)
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	if(pCbkdlg->iISCreateS == 0)
	{
		//说明还没有创建
			BOOL bRET = FALSE;
			bRET = pCbkdlg->CreateVideoDlgS();
			if(bRET == FALSE)
				return FALSE;

			/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
			WriteLog("成功后1");
			pCbkdlg->SendMessageToVideoDlgS(ExamCarNo,VideoAdd);
			pCbkdlg->ExamCarInfo.GetDlgItem(IDC_BUTTONOUTSIDE)->SetWindowText("关车外");
			//////////////////////////////////
	}
	else
	{
		/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
		pCbkdlg->SendMessageToVideoDlgS(ExamCarNo,VideoAdd);
		//////////////////////////////////
	}

	return TRUE;
}

BOOL CExamCarDlg::CloseExamCarVideoT()
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
	pCbkdlg->SendMesageToVideoDlgCloseT();
	//////////////////////////////////

	return TRUE;
}

BOOL CExamCarDlg::CloseExamCarVideoS()
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////调用主对话框 实现的功能是发送消息到考车视频的对话框////////
	pCbkdlg->SendMesageToVideoDlgCloseS();
	//////////////////////////////////

	return TRUE;
}

void CExamCarDlg::ChangeTrackVideoPlayT(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //当前子项的父项
	HTREEITEM temSub; //子项获取
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //获得第一项
	while (tem != NULL) //如果返回空说明已经到了尾端了  退出
	{
		if (tem != tem1) //此时获取的父项不是选择的那个父项
		{
			//查看此时追踪的状态是开启还是取消
			//获取子项的第一项
			temSub = m_exam.GetChildItem( tem );
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "关闭副驾驶视频")
			{
				m_exam.SetItemText(temSub,"开启副驾驶视频");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}
}

void CExamCarDlg::ChangeTrackVideoPlayS(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //当前子项的父项
	HTREEITEM temSub; //子项获取
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //获得第一项
	while (tem != NULL) //如果返回空说明已经到了尾端了  退出
	{
		if (tem != tem1) //此时获取的父项不是选择的那个父项
		{
			//查看此时追踪的状态是开启还是取消
			//获取子项的第一项
			temSub = m_exam.GetChildItem( tem );
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "关闭车外视频")
			{
				m_exam.SetItemText(temSub,"开启车外视频");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}
}

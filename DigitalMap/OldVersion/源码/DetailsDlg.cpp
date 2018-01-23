// DetailsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cbkdigitalmap.h"
#include "DetailsDlg.h"
#include "ExamCarDlg.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKDigitalMapApp theApp;
#define ExamCarN0 10
#define ExamCarN0Sub 1000

char zkzh[100][30];//存放准考证号

UINT UpdateExamCarInfo_thd(LPVOID p);  //刷新在线考车的实时信息

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg dialog


CDetailsDlg::CDetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDlg)
	DDX_Control(pDX, IDC_TREE_DETAILS, m_details);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailsDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg message handlers

BOOL CDetailsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_details.MoveWindow(1,1,260,388);  
	m_details.DeleteAllItems();
	
	InitMytree(); 


	AfxBeginThread(&UpdateExamCarInfo_thd, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDetailsDlg::InitMytree()
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
	m_details.SetImageList(m_imagelist,TVSIL_NORMAL); //为m_mytree设置一个图像列表，使CtreeCtrl的节点显示不同的图标 
	m_details.SetBkColor(RGB(160,210,230));//设置m_mytree的背景色
	
	return true;
}



UINT UpdateExamCarInfo_thd(LPVOID p)  //实时刷新在线考车信息
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CDetailsDlg *pDetailsDlg = (CDetailsDlg *)p;
	//  在这里重新刷新列表的值  这一块到时候可能会用线程来操作
	pDetailsDlg->m_details.DeleteAllItems(); //清空列表值
	// 然后根据在线考车找到考了什么项目 查找数据库
	// BOOL ArryExamCar[MAX_ExamCar]; //在线考车 1 在线 0 不在线
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	int i = 0;
	BOOL OnlineExamCar[MAX_ExamCar];

	memset(zkzh,0,100*30);
	for (i = 0;i < MAX_ExamCar;i++) //初始化在线考车为不在线
	{
		OnlineExamCar[i] = FALSE;
	}
	while (pCCBKDigitalMapDlg->iContrl)
//	while (1)
	{
		for (i = 1;i < MAX_ExamCar;i++)
		{
			if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.ArryExamCar[i] == TRUE)
			{
				// 先判断这辆考车有没有在线
				if (OnlineExamCar[i] == FALSE)
				{
					OnlineExamCar[i] = TRUE;
				}
				//说明考车在线 查找数据库然后将一些考车中的信息显示出来
				CString tempCar;
				int iCar;
			//	tempCar.Format("考车%d",i);
				tempCar.Format("考车%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
				iCar = ((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber;
				pDetailsDlg->ReadExamInfoFromDB(NULL, tempCar, NULL,zkzh[iCar]);
			}
			else
			{
				// 说明考车已经不在线或者从没有上线过 
				// 先判断下这辆考车有没有上线过 如果有就给它下线 同时将树状控件中的内容清空
				if (OnlineExamCar[i] == TRUE) //说明考车有上线过
				{
					// 清除记录 设置为下线
					OnlineExamCar[i] = FALSE;
			//		pDetailsDlg->m_details.DeleteItem(pCurItem);


					//从树状列表中删除考车
					// 要先找到该条信息的索引
 					HTREEITEM pCurItem;
 					HTREEITEM temSub;
 					CString szItemText;
 					CString tmpItemText;
			//		szItemText.Format("考车%d",i);
					szItemText.Format("考车%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
			//		GetChildItem( HTREEITEM hItem );
					pCurItem = pDetailsDlg->m_details.GetChildItem(TVI_ROOT); 
			//		pCurItem = pDetailsDlg->m_details.GetFirstVisibleItem( );
					if (pCurItem != NULL)
					{
						tmpItemText = pDetailsDlg->m_details.GetItemText(pCurItem );
						while(szItemText != tmpItemText)
						{
							pCurItem = pDetailsDlg->m_details.GetNextSiblingItem(pCurItem);
							if (pCurItem == NULL)
							{
								break;
							}
							tmpItemText = pDetailsDlg->m_details.GetItemText(pCurItem );
						}
						if(szItemText == tmpItemText)
						{
							pDetailsDlg->m_details.DeleteItem(pCurItem);
						}
						


					}
					pCurItem = NULL;
					temSub = NULL;
					szItemText = _T("");
 					tmpItemText = _T("");
				}
			}
		}
		Sleep(1000);
	}
	Sleep(5000);
	pCCBKDigitalMapDlg->WriteLog("详 UpdateExamCarInfo_thd");
	return 0;
}



void CDetailsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDetailsDlg::ReadExamInfoFromDB(CDC *pDC, CString sCard, CRect *rc,char *pzkzh)
{
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString szExamineeName;  //姓名
	CString strzkzm;  //准考证明编号
	CString strCarModel;  //考试车型
	CString szPointsType;  //扣分类型
	CString szDeductionPoint;  //扣除分数

	char bufzkzh[30];
	memset(bufzkzh,0,30);
	strcpy(bufzkzh,pzkzh);

	int j = 0;

	//////
	CTime pTime = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format("%d-%d-%d",pTime.GetYear(),pTime.GetMonth(),pTime.GetDay());
	////////

	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			CString strSQL;	
			VARIANT cnt;
			cnt.vt = VT_INT;
			strSQL.Format("SELECT * FROM StudentInfo WHERE 考车号 = '%s' and 状态 = '3' and 预考日期 = '%s'",sCard,szTime);
			// 			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
			// 				adOpenDynamic, adLockOptimistic, adCmdText);
			
			_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
			_variant_t vat;
			if(pSet != NULL && (!pSet->adoEOF))
			{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
				while(!pSet->adoEOF)
				{
					vat =pSet->GetCollect("姓名");
					if (vat.vt != VT_NULL)
					{
						szExamineeName = (LPCSTR)_bstr_t(vat);
					}
					vat =pSet->GetCollect("准考证明编号");
					if (vat.vt != VT_NULL)
					{
						strzkzm = (LPCSTR)_bstr_t(vat);
					}
					vat =pSet->GetCollect("考试车型");
					if (vat.vt != VT_NULL)
					{
						strCarModel = (LPCSTR)_bstr_t(vat);
					}
					
					pSet->MoveNext();
					break;
				}

				/////      以下判断是不是原来的那个考生   ///////
				if(strcmp(strzkzm,bufzkzh)) 
				{
					//说明不是相同的准考证号
					if(!strcmp(bufzkzh,""))
					{
						//说明准考证号为空

					}
					else
					{
						//说明不为空 此时已经换了另一个考生了，要把之前的记录都删除
						HTREEITEM pCurItem2;
						HTREEITEM temSub2;
						CString tmpItemText2;
						CString tempCar2;
						CString szText2;  //找到最后一条子项的内容
						szText2 = _T("");
						tempCar2 = sCard;
						pCurItem2 = m_details.GetChildItem(TVI_ROOT); 
						if (pCurItem2 != NULL)
						{
							tmpItemText2 = m_details.GetItemText(pCurItem2 );
								

							while(tempCar2 != tmpItemText2)
							{
								pCurItem2 = m_details.GetNextSiblingItem(pCurItem2);
								if (pCurItem2 == NULL)
								{

									break;
								}
								tmpItemText2 = m_details.GetItemText(pCurItem2);
							}

							///// 这里要做的是把之前的全部都删除掉 
							temSub2 = m_details.GetChildItem( pCurItem2 );
							while (temSub2 != NULL)
							{
								//这里要考虑到怎么删除
								m_details.DeleteItem( temSub2);
								temSub2 = m_details.GetChildItem( pCurItem2 );

						//		szText2 = m_details.GetItemText(temSub2);
						//		temSub2 = m_details.GetNextSiblingItem( temSub2 );
						//		m_details.DeleteItem( temSub2);
							}
						}
					}
					strcpy(pzkzh,strzkzm);

				}
				/////////////////////////////////////////////////
				


				/////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////
				//处理完要添加的数据之后 就要插入到树状控件对应的子项里面
				
				//说明考车在线 查找数据库然后将一些考车中的信息显示出来
				CString tempCar;
				tempCar = sCard;
				// 先查找这辆考车有没有在树状控件里面 如果有就继续添加子项 没有就插入然后添加子项
				
				HTREEITEM pCurItem;
				HTREEITEM temSub;
				
				CString tmpItemText;
				int i;
				i = atoi(tempCar.Right(1));
				
			//	pCurItem = m_details.GetFirstVisibleItem( );
				// 展开第一个节点    
				// HTREEITEM hChildItem = m_treeCtrlList.GetChildItem(TVI_ROOT); 
				pCurItem = m_details.GetChildItem(TVI_ROOT); 
				if (pCurItem != NULL)
				{
					tmpItemText = m_details.GetItemText(pCurItem );
					    

					while(tempCar != tmpItemText)
					{
						pCurItem = m_details.GetNextSiblingItem(pCurItem);
						if (pCurItem == NULL)
						{
							//添加考车的父项
							m_hDevItem = m_details.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
							m_details.SetItemData(m_hDevItem,(ExamCarN0 * i) + 10);
							break;
						}
						tmpItemText = m_details.GetItemText(pCurItem );
					}
				}
				else
				{
					//添加考车的父项
					m_hDevItem = m_details.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
					m_details.SetItemData(m_hDevItem,(ExamCarN0 * i) + 10);
				}
				
				//判断这个父项里面有没有这个值 如果有就继续查找下一条 如果没有就添加
				// 先找到最后一条 
				CString szText;  //找到最后一条子项的内容
				szText = _T("");
				if (pCurItem != NULL)
				{
					temSub = m_details.GetChildItem( pCurItem );
					
					while (temSub != NULL)
					{
						szText = m_details.GetItemText(temSub);
						temSub = m_details.GetNextSiblingItem( temSub );
						
					}
				}
				

				strSQL.Format("SELECT * FROM ErrorRecordInDetail WHERE 准考证明编号 = '%s' order by 出错时间 asc",strzkzm );
				// 			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
				// 				adOpenDynamic, adLockOptimistic, adCmdText);
				
				pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
			//	_variant_t vat;
				if(pSet != NULL && (!pSet->adoEOF))
				{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
					while(!pSet->adoEOF)
					{
						vat =pSet->GetCollect("扣分类型");
						if (vat.vt != VT_NULL)
						{
							szPointsType = (LPCSTR)_bstr_t(vat);
						}
						vat =pSet->GetCollect("扣除分数");
						if (vat.vt != VT_NULL)
						{
							szDeductionPoint = (LPCSTR)_bstr_t(vat);
						}	
						
						//判断列表有没有这项 没有就往后加入 如果有就不加入
					//	HTREEITEM ExamCarItem;

						if (szPointsType.GetLength() > 20)
						{
							szPointsType = szPointsType.Left(20);
						}

						if (szText == _T(""))
						{
							// 说明是新添加的父项
							if (szText != szPointsType)
							{
								
								HTREEITEM ExamCarItem = m_details.InsertItem(szPointsType,0,1,m_hDevItem,TVI_LAST);
								m_details.SetItemData(ExamCarItem,(ExamCarN0Sub * j) + 1);   //Data对应通道在数组中的索引
								j++;
							}
							pSet->MoveNext();
						}
						else  //说明是已经存在的父项
						{
							if (szText == szPointsType) //还没有找到那个最后插入的记录值
							{
								//已经找到了这条记录值  将这条记录之后的都添加上去

								pSet->MoveNext();
								while(!pSet->adoEOF)
								{
									vat =pSet->GetCollect("扣分类型");
									if (vat.vt != VT_NULL)
									{
										szPointsType = (LPCSTR)_bstr_t(vat);
									}
									vat =pSet->GetCollect("扣除分数");
									if (vat.vt != VT_NULL)
									{
										szDeductionPoint = (LPCSTR)_bstr_t(vat);
									}	
									
									//判断列表有没有这项 没有就往后加入 如果有就不加入
									//	HTREEITEM ExamCarItem;
									
									if (szPointsType.GetLength() > 20)
									{
										szPointsType = szPointsType.Left(20);
									}
									HTREEITEM ExamCarItem = m_details.InsertItem(szPointsType,0,1,m_hDevItem,TVI_LAST);
									m_details.SetItemData(ExamCarItem,(ExamCarN0Sub * j) + 1);   //Data对应通道在数组中的索引
									j++;
									pSet->MoveNext();
								}								
							}
							else
							{
								pSet->MoveNext();
							}
						}

						
	
					//	break;
					}
				}

			}
			
			
	//	}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}

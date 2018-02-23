// ToolzDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cbkmap.h"
#include "ToolzDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolzDlg dialog


CToolzDlg::CToolzDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolzDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolzDlg)
	m_strKch = _T("");
	//}}AFX_DATA_INIT
}


void CToolzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolzDlg)
	DDX_Control(pDX, IDC_BTN_TEST, m_BtnShow);
	DDX_Control(pDX, IDC_LIST_CAR, m_ListCarZ);
	DDX_Text(pDX, IDC_STATIC_KCH, m_strKch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolzDlg, CDialog)
	//{{AFX_MSG_MAP(CToolzDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnTest)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAR, OnDblclkListCar)
	ON_BN_CLICKED(IDC_BTN_ZHUIZONG, OnBtnZhuizong)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CAR, OnItemchangedListCar)
	ON_MESSAGE(UM_SOCKUDPZ,OnSocketUDPZ)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolzDlg message handlers
////20170425 
void CToolzDlg::OnSocketUDPZ(WPARAM wParam,LPARAM lParam)//UDP
{
//	int iEvent = WSAGETSELECTEVENT(lParam); //消息类别 
//	int iErrorCode = WSAGETSELECTERROR(lParam);//错误代码 
	if ((LOWORD(lParam) & FD_READ)==FD_READ) //有数据在读了
	{
		char *recvbuf=NULL;
		if(m_Udpz.RecvFromSock(&recvbuf)==TRUE)
		{
			struRBody rBodyz;
			memcpy(&rBodyz,recvbuf,sizeof(struRBody));
			TRACE("收到的数据:%s,%lf,%lf\n",rBodyz.sKchp.cKchp,rBodyz.sGnss.gnssX,rBodyz.sGnss.gnssY);
			if (strcmp(m_sRBodyMap[rBodyz.sKchp.index].sStu.bStuSFZ,rBodyz.sStu.bStuSFZ))
			{
				//TRACE("[%s]!=[%s]\n",m_sRBodyMap[rBodyz.sKchp.index].sStu.bStuSFZ,rBodyz.sStu.bStuSFZ);
				strcpy(m_sRBodyMap[rBodyz.sKchp.index].sStu.bStuSFZ,rBodyz.sStu.bStuSFZ);
				//身份证编号修改
			}
			if (strcmp(m_sRBodyMap[rBodyz.sKchp.index].sStu.bXMZT,rBodyz.sStu.bXMZT))
			{
				//项目信息修改
				strcpy(m_sRBodyMap[rBodyz.sKchp.index].sStu.bXMZT,rBodyz.sStu.bXMZT);
				ISTRMSGZ *iXmzt =new ISTRMSGZ();
				::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MXMZTCHANGE,(LPARAM)iXmzt);
			}
			GNSSMSGz *GnssMsg =new GNSSMSGz();
			if (m_dXC==0)
			{
				GnssMsg->cx=(m_dMaxX-rBodyz.sGnss.gnssX)*m_dZoomIn+176;
			}
			else
			{
				GnssMsg->cx=(rBodyz.sGnss.gnssX-m_dMinX)*m_dZoomIn+176;
			}
			if (m_dYC==0)
			{
				GnssMsg->cy=(m_dMaxY-rBodyz.sGnss.gnssY)*m_dZoomIn+144;
			}
			else
			{
				GnssMsg->cy=(rBodyz.sGnss.gnssY-m_dMinY)*m_dZoomIn+144;
			}
			GnssMsg->sd=rBodyz.sGnss.gnssSD;
			GnssMsg->lc=rBodyz.sGnss.gnssLC;
			
			GnssMsg->cr=rBodyz.sGnss.gnssR;
			if (m_uPBType==2)//回放模式
			{
				if (m_iPBModeIndex==999)
				{
					
					MAP_INT_sRBODY::iterator iter;
					for(iter=m_sRBodyMap.begin(); iter!=m_sRBodyMap.end(); iter++)
					{						
						if (!strcmp(iter->second.sKchp.cKchp,rBodyz.sKchp.cKchp))
						{
							m_iPBModeIndex=iter->first;
							TRACE("%d-->%s \n",iter->first,iter->second.sKchp.cKchp);
						}
					}
				}
				GnssMsg->uIndex=m_iPBModeIndex;
			}
			else
			{
				GnssMsg->uIndex=rBodyz.sKchp.index;
			}
			
			
			::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MGNSSMSG,(LPARAM)GnssMsg);
		}
	}
}

// void CToolzDlg::UpdateDisplayDlg(int Transparent)
// {
// 	HDC hdcTemp=GetDC()->m_hDC;
// 	m_hdcMemory=CreateCompatibleDC(hdcTemp);
// 	HBITMAP hBitMap=CreateCompatibleBitmap(hdcTemp,m_BakWidth,m_BakHeight);
// 	SelectObject(m_hdcMemory,hBitMap);
// 	if(Transparent<0||Transparent>100)	Transparent=100;
// 	
// 	m_Blend.SourceConstantAlpha=int(Transparent*2.55);//1~255
// 	HDC hdcScreen=::GetDC (m_hWnd);
// 	RECT rct;
// 	GetWindowRect(&rct);
// 	POINT ptWinPos={rct.left,rct.top};
// 	Graphics graph(m_hdcMemory);
// 	
// 	Point points[] = { Point(0, 0),Point(m_BakWidth, 0), Point(0, m_BakHeight)};
// 	
// 	graph.DrawImage(m_pImageBK, points, 3);	
// 	SIZE sizeWindow={m_BakWidth,m_BakHeight};
// 	POINT ptSrc={0,0};
// 	DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
// 	if((dwExStyle&0x80000)!=0x80000)
// 		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000);
// 	
// 	BOOL bRet=FALSE;
// 	bRet= UpdateLayeredWindow( m_hWnd,hdcScreen,&ptWinPos,
// 		&sizeWindow,m_hdcMemory,&ptSrc,0,&m_Blend,2);
// 	graph.ReleaseHDC(m_hdcMemory);
// 	::ReleaseDC(m_hWnd,hdcScreen);
// 	hdcScreen=NULL;
// 	::ReleaseDC(m_hWnd,hdcTemp);
// 	hdcTemp=NULL;
// 	DeleteObject(hBitMap);
// 	DeleteDC(m_hdcMemory);
// 	m_hdcMemory=NULL;
// 	//	return bRet;
// }

BOOL CToolzDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	dwLogid=logz.Register("CToolzDlg.log");
	m_bToolDlgShow=false;
	//m_BtnShow
	m_BtnShow.MoveWindow(210,0,50,50);
	m_BtnShow.LoadBitmap(IDB_BMP_SHOWTOOL,4);
	m_BtnShow.SetBtnDrawNum(0,1,0,2,0);
	//////////////////////////////////////////////////////////////////////////
	m_ListCarZ.MoveWindow(4,50,250,380);
	m_ListCarZ.SetExtendedStyle(LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP /*|LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE*/);
	//m_ListCarZ.SetExtendedStyle(/*LVS_EX_GRIDLINES |*/LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_ListCarZ.InsertColumn(0,_T("N"),LVCFMT_LEFT,30,-1);
	m_ListCarZ.InsertColumn(1,_T("考车号牌"),LVCFMT_LEFT,80,-1);
	m_ListCarZ.InsertColumn(2,_T("显示"),LVCFMT_LEFT,48,-1);
	m_ListCarZ.InsertColumn(3,_T("提示"),LVCFMT_LEFT,48,-1);
	m_ListCarZ.InsertColumn(4,_T("zkzm"),LVCFMT_LEFT,-20,-1);
	m_ListCarZ.SetTextColor(RGB(251,0,251));
	
	if(m_Udpz.InitSockU(m_hWnd,9705,UM_SOCKUDPZ)==FALSE)
	{
		logz.WriteLog(dwLogid,"m_Udpz.InitSockUC() faild!");
	}
// 	if(m_Udpz.InitSockUC()==TRUE)
// 	{
// 		m_Udpz.WSAAsyncSelectz(m_hWnd,UM_SOCKUDPZ);
// 	}
// 	else
// 	{
// 		logz.WriteLog(dwLogid,"m_Udpz.InitSockUC() faild!");
// 	}
	CString tempPath;
	tempPath.Format("D:\\Temp");
	DWORD dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	strPath.Format("%s\\%s",tempPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CToolzDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_uPBType=1;
	m_iPBModeIndex=999;
	LoadImageFromIDR(IDR_TOOLBK,m_pImageBK);
	LoadImageFromIDR(IDR_IMGTipBox,m_pImgTipBox);
	SetWindowPos(NULL,0,0,260,460,SWP_NOMOVE | SWP_NOOWNERZORDER ); 
	return 0;
}

BOOL CToolzDlg::LoadImageFromIDR(UINT nID, Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),"ZSZ");
	if (!hRsrc)
		return FALSE;
	
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	
	// load from stream
	pImg=Gdiplus::Image::FromStream(pstm);
	
	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}

void CToolzDlg::OnDestroy() 
{
	delete m_pImageBK;
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CToolzDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
	Graphics grap(dc.GetSafeHdc());
	grap.DrawImage(m_pImageBK,Rect(0,0,260,460));
}

HBRUSH CToolzDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor=CTLCOLOR_DLG)
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	//	return m_brush;
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CToolzDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CToolzDlg::OnBtnTest() 
{
	// TODO: Add your control notification handler code here
	m_bToolDlgShow=m_bToolDlgShow?false:true;
	if (m_bToolDlgShow)
	{
		::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MSHOWDLG,0);
	}
	else
	{
		::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MSHOWDLG,1);
	}
	
}

void CToolzDlg::OnDblclkListCar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iSubItem==2 || pNMListView->iSubItem==3)
	{		
		if (m_ListCarZ.GetItemText(pNMListView->iItem,pNMListView->iSubItem)=="√")
		{
			m_ListCarZ.SetItemText(pNMListView->iItem,pNMListView->iSubItem,"×");
			if (pNMListView->iSubItem==3)
			{
				::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MTIPBOXHIDE,pNMListView->iItem);
			}
			else
			{

			}
			
		}
		else
		{
			m_ListCarZ.SetItemText(pNMListView->iItem,pNMListView->iSubItem,"√");			
			if (pNMListView->iSubItem==3)
			{
				::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MTIPBOXSHOW,pNMListView->iItem);
			}
			else
			{
				
			}
		}
	}
	*pResult = 0;
}

void CToolzDlg::OnBtnZhuizong() 
{
	// TODO: Add your control notification handler code here

	if (m_strKch.IsEmpty())
	{
		AfxMessageBox("请选择列表中的考车进行追踪!");
		return ;
	}
	DrawTipBoxForz("0000","张三","闽A002",1);
	GNSSMSGz *GnssMsg =new GNSSMSGz();
	GnssMsg->uIndex=1;
	GnssMsg->cx=150.0f;
	GnssMsg->cy=200.0f;
	GnssMsg->cr=45.22f;
	GnssMsg->sd=0;
	GnssMsg->lc=0;
	::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MGNSSMSG,(LPARAM)GnssMsg);
}

void CToolzDlg::OnItemchangedListCar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_strKch=m_ListCarZ.GetItemText(pNMListView->iItem,1);
	UpdateData(FALSE);
	*pResult = 0;
}

void CToolzDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
// 	if (nIDEvent==1)
// 	{
		//GetGnssMsgFromS();
	if(m_uPBType==1)
	{
		GetGnssMsgFromS();
	}
	else
	{
		KillTimer(nIDEvent);
	}
//	}
	CDialog::OnTimer(nIDEvent);
}

void CToolzDlg::GetGnssMsgFromS()
{
	if (isGetGnssFS==false)
	{	
		isGetGnssFS=true;
		MAP_INT_sRBODY::iterator iter;
		for(iter=m_sRBodyMap.begin(); iter!=m_sRBodyMap.end(); iter++)
		{
			//TRACE("[%s][%d]\n",iter->second.sKchp.cKchp,iter->second.sKchp.index);
			m_Udpz.SendPack((char *)&iter->second.sKchp,sizeof(struKCHP));
		}
		isGetGnssFS=false;
	}
}

//void CToolzDlg::GetTongJiMsg()
//{
/*	int izt[4];
	izt[0]=0;
	izt[1]=0;
	izt[2]=0;
	izt[3]=0;
	try
	{
		if (m_pConn==NULL)
		{
			return;
		}

		_RecordsetPtr pRecordsetz;
		pRecordsetz.CreateInstance("ADODB.Recordset");
		pRecordsetz->CursorLocation = adUseClient;
		_variant_t vat;
		CString strSQL;
		int iTempN,itempZt;
		
		strSQL.Format("SELECT 状态,COUNT(*) AS N FROM STUDENTINFO WHERE 预约日期=TRUNC(sysdate) AND KCDM='%s' GROUP BY 状态",m_strkcdm);
		//TRACE("%s\n",strSQL);
		pRecordsetz->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);		
		if(pRecordsetz != NULL)
		{
			while (!pRecordsetz->adoEOF)
			{
				vat =pRecordsetz->GetCollect("N");
				if (vat.vt != VT_NULL)
				{
					iTempN=atoi((LPCSTR)_bstr_t(vat));
				}
				else{ iTempN=0; }
				vat =pRecordsetz->GetCollect("状态");
				
				if (vat.vt != VT_NULL)
				{
					itempZt=atoi((LPCSTR)_bstr_t(vat));
					switch(itempZt)
					{
					case 0://未考
						izt[0]=iTempN;
						break;
					case 1://合格
						izt[1]=iTempN;
						break;
					case 2://不合格
						izt[2]=iTempN;
						break;
					case 3://考试中
						izt[3]=iTempN;
						break;
					default:
						break;
					}
				}
				pRecordsetz->MoveNext();
			}
		}
		pRecordsetz->Close();
		pRecordsetz.Release();
	}
	catch (_com_error &e) 
	{
		CString temp;
		temp.Format("GetTongJiMsg! ErrorMessage:%s",e.ErrorMessage());
		AfxMessageBox(temp);
		return ;
	}
	iykrs=izt[1]+izt[2]+izt[3];
	iwkrs=izt[0];
	int izrs;
	izrs=iykrs+iwkrs;
	if (izrs==0)
	{
		izrs=1;
	}
	fhgr=(FLOAT(izt[1])/izrs)*100;
	*/
//}

void CToolzDlg::DrawTipBoxForz(CString strZkzm,CString stuName,CString strKchp,int uIndex)
{
	Image *tImage=m_pImgTipBox->Clone();
	Graphics imageGraphics(tImage);

	Font myFont(L"宋体",11);
	SolidBrush blackBrush(Color::Yellow);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);//水平居左StringAlignmentNear //StringAlignmentCenter);//默认都不影响垂直 
	format.SetLineAlignment(StringAlignmentCenter);// 和SetAlignment对应，为竖直的设置
	RectF destRect(52,36,142,20);
//	imageGraphics.DrawRectangle(&Pen(Color::Yellow,1), destRect);
	
	DrawStringz(&imageGraphics,stuName,myFont,blackBrush,destRect,format);

	destRect.Offset(0,60);
//	imageGraphics.DrawRectangle(&Pen(Color::Yellow,1), destRect);

	DrawStringz(&imageGraphics,strZkzm,myFont,blackBrush,destRect,format);

	RectF rectKchp(110,0,86,30);
//	imageGraphics.DrawRectangle(&Pen(Color::Yellow,1), rectKchp);

	DrawStringz(&imageGraphics,strKchp,myFont,blackBrush,rectKchp,format);
	//////////////////////////////////////////////////////////////////////////
	CString filename;
	filename.Format("%s\\%s.tbox",strPath,strZkzm);
	unsigned short tcharArr[1024];
	memset(tcharArr,0x0,sizeof(tcharArr));	
	swprintf((wchar_t *)tcharArr,L"%s",filename.AllocSysString());
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	tImage->Save(tcharArr, &pngClsid, NULL);

	//////////////////////////////////////////////////////////////////////////
	ISTRMSGZ *kchpMsg =new ISTRMSGZ();
	kchpMsg->uIndex=uIndex;
	kchpMsg->strMsg.Format("%s",filename);
	::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MZKZMBHCHANGE,(LPARAM)kchpMsg);
}

int CToolzDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num= 0;
	UINT size= 0;
	
	ImageCodecInfo* pImageCodecInfo= NULL;
	
	GetImageEncodersSize(&num, &size);
	if(size== 0)
	{
		return -1;
	}
	pImageCodecInfo= (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo== NULL)
	{
		return -1;
	}
	
	GetImageEncoders(num, size, pImageCodecInfo);
	
	for(UINT j=0; j< num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format)== 0)
		{
			*pClsid= pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}	
	free(pImageCodecInfo);
	return -1;
}

void CToolzDlg::InitKchp(CString strkcdm)
{
	m_sRBodyMap.clear();
	m_ListCarZ.DeleteAllItems();
	CString temp,tempk,strConfile;
	int iCarNum;
	strConfile.Format("D:\\bkzyMapOcx\\MAP\\%s.dat",strkcdm);
	iCarNum=GetPrivateProfileInt(strkcdm,"CarNum",0,strConfile); 

	for (int i=0;i<iCarNum-1;i++)
	{
		tempk.Format("%d",i+1);
		GetPrivateProfileString(strkcdm,tempk,"",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
		temp.ReleaseBuffer();
		m_ListCarZ.InsertItem(i,tempk);
		m_ListCarZ.SetItemText(i,1,temp);
		m_ListCarZ.SetItemText(i,2,"√");
		m_ListCarZ.SetItemText(i,3,"×");		
		memset(&m_sRBodyMap[i],0,sizeof(struRBody));
		m_sRBodyMap[i].sKchp.index=i;
		strcpy(m_sRBodyMap[i].sKchp.cKchp, temp.GetBuffer(temp.GetLength()));
		temp.ReleaseBuffer(temp.GetLength());
		//////////////////////////////////////////////////////////////////////////
		//初始化考车 IMAGE
		ISTRMSGZ *kchpMsg =new ISTRMSGZ();
		kchpMsg->uIndex=i;
		kchpMsg->strMsg.Format("%s",temp);
		::PostMessage(GetParent()->m_hWnd,WM_TOOLDLG_MSG,MINITCAR,(LPARAM)kchpMsg);
		//////////////////////////////////////////////////////////////////////////
		//生成初始化提示框
		tempk.Format("1346708777.%d",i);
		memcpy(m_sRBodyMap[i].sStu.bStuSFZ, tempk.GetBuffer(tempk.GetLength()),tempk.GetLength());

		TRACE("tempk%s=[%s]\n",tempk,m_sRBodyMap[i].sStu.bStuSFZ);
		tempk.ReleaseBuffer(tempk.GetLength());
		DrawTipBoxForz(tempk,"",temp,i);
		//////////////////////////////////////////////////////////////////////////
	}
	//////////////////////////////////////////////////////////////////////////
	//读取配置信息
	GetPrivateProfileString(strkcdm,"MAXX","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dMaxX=atof(temp);
	GetPrivateProfileString(strkcdm,"MAXY","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dMaxY=atof(temp);
	GetPrivateProfileString(strkcdm,"MINX","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dMinX=atof(temp);
	GetPrivateProfileString(strkcdm,"MINY","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dMinY=atof(temp);
	GetPrivateProfileString(strkcdm,"XC","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dXC=atof(temp);
	GetPrivateProfileString(strkcdm,"YC","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dYC=atof(temp);
	GetPrivateProfileString(strkcdm,"ZOOMIN","0",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	m_dZoomIn=atof(temp);
	//
	logz.WriteLog(dwLogid,"m_dMaxX=%lf,m_dMaxY=%lf,m_dMinX=%lf,m_dMinY=%lf,m_dXC=%lf,m_dYC=%lf,m_dZoomIn=%lf",m_dMaxX,m_dMaxY,m_dMinX,m_dMinY,m_dXC,m_dYC,m_dZoomIn);

	GetPrivateProfileString(strkcdm,"UDPSERVER","",temp.GetBuffer(MAX_PATH),MAX_PATH,strConfile);
	temp.ReleaseBuffer();
	if (!temp.IsEmpty())
	{
		m_UdpServerIP.Format("%s",temp);
		m_Udpz.SetSendAddr(m_UdpServerIP,9702);
		KillTimer(2);
		SetTimer(2,1000,NULL);
		isGetGnssFS=false;
	}
}


void CToolzDlg::SetMode(UINT utype)
{
	m_uPBType=utype;
}

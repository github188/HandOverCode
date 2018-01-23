// DD.cpp : implementation file
//

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "DD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCBKDigitalMapApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDD dialog


CDD::CDD(CWnd* pParent /*=NULL*/)
	: CDialog(CDD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDD, CDialog)
	//{{AFX_MSG_MAP(CDD)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()  //将控件的背景改为透明
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDD message handlers

BOOL CDD::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CDD::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDD::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAPSUB); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	int nRight=bitmap.bmWidth;
	int nBtonn=bitmap.bmHeight;
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, 
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

////////防止GDI泄露
	bmpBackground.DeleteObject();  //防止GDI泄露
	pbmpOld->DeleteObject();

//	return TRUE;
	// Do not call CDialog::OnPaint() for painting messages
	GetDlgItem(IDC_STATIC_CARNUMBER1)->SetWindowText(CSCAR);
	GetDlgItem(IDC_STATIC_NAME1)->SetWindowText(szExamineeName);
	GetDlgItem(IDC_STATIC_ZKZH1)->SetWindowText(strzkzm);
	GetDlgItem(IDC_STATIC_EXAMMODEL1)->SetWindowText(strCarModel);
	OnShowPHO();  //显示在照片
	this->ReleaseDC(&dcMem);
}

void CDD::SetupRegion(CDC *pDC, CBitmap &cBitmap, COLORREF TransColor)
{
    CDC memDC;
    //创建与传入DC兼容的临时DC
    memDC.CreateCompatibleDC(pDC);
    
    CBitmap *pOldMemBmp=NULL;
    //将位图选入临时DC
    pOldMemBmp=memDC.SelectObject(&cBitmap);
    
    CRgn wndRgn;
    //创建总的窗体区域，初始region为0
    wndRgn.CreateRectRgn(0,0,0,0);
    
    
    BITMAP bit;   
    cBitmap.GetBitmap (&bit);//取得位图参数，这里要用到位图的长和宽     
    
    int y;
    for(y=0;y<bit.bmHeight  ;y++)//y=0;y<=bit.bmHeight  ;y++)
    {
        CRgn rgnTemp; //保存临时region
        
        int iX = 0;
        do
        {
            //跳过透明色找到下一个非透明色的点.
            while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) == TransColor)
                iX++;
            
            //记住这个起始点
            int iLeftX = iX;
            
            //寻找下个透明色的点
            while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) != TransColor)
                ++iX;
            
            //创建一个包含起点与重点间高为1像素的临时“region”
            rgnTemp.CreateRectRgn(iLeftX, y, iX, y+1);
            
            //合并到主"region".
            wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR);
            
            //删除临时"region",否则下次创建时和出错
            rgnTemp.DeleteObject();
        }while(iX <bit.bmWidth );
        iX = 0;
    }
    
    if(pOldMemBmp)
        memDC.SelectObject(pOldMemBmp);
    
    CWnd * pWnd = pDC->GetWindow();
    pWnd->SetWindowRgn(wndRgn,TRUE);   // 是设置了一个窗口的区域.只有被包含在这个区域内的地方才会被重绘,而不包含在区域内的其他区域系统将不会显示.  
    pWnd->SetForegroundWindow();    
	this->ReleaseDC(&memDC);
	pOldMemBmp->DeleteObject();
	
}


BOOL CDD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_STATIC_CARNUMBER)->MoveWindow(10,10,80,25); //考车号
	GetDlgItem(IDC_STATIC_CARNUMBER1)->MoveWindow(100,10,80,25); //考车号
	GetDlgItem(IDC_STATIC_NAME)->MoveWindow(10,45,80,25); //考生姓名
	GetDlgItem(IDC_STATIC_NAME1)->MoveWindow(100,45,80,25); //考生姓名
	GetDlgItem(IDC_STATIC_ZKZH)->MoveWindow(10,80,80,25); //准考证号
	GetDlgItem(IDC_STATIC_ZKZH1)->MoveWindow(100,80,80,25); //准考证号
	GetDlgItem(IDC_STATIC_EXAMMODEL)->MoveWindow(10,115,80,25); //考试车型
	GetDlgItem(IDC_STATIC_EXAMMODEL1)->MoveWindow(100,115,80,25); //考试车型

	GetDlgItem(IDC_STATIC_PHO)->MoveWindow(220,10,90,110);  //移动照片的静态编辑框位置

//	GetDlgItem(IDC_STATIC_CARNUMBER)->SetWindowText("考车号：");
//	GetDlgItem(IDC_STATIC_NAME)->SetWindowText("考生姓名：");
//	GetDlgItem(IDC_STATIC_ZKZH)->SetWindowText("准考证号：");
//	GetDlgItem(IDC_STATIC_EXAMMODEL)->SetWindowText("考车车型：");

	
	// TODO: Add extra initialization here
//	m_bmp.LoadBitmap(IDB_BITMAPSUB);
	CClientDC dc(this);  // 类CClientDC派生于CDC，在构造时调用了Windows函数GetDC，在析构时调用了ReleaseDC。这意味着和CClientDC对象相关的设备上下文是窗口的客户区。

//    SetupRegion(&dc,m_bmp,RGB(255,0,255));


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)  //将控件的背景改为透明
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if   (nCtlColor   ==   CTLCOLOR_STATIC)        //设置控件为透明
	{     
		pDC->SetBkMode(TRANSPARENT);     
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


BOOL CDD::ReadExamInfoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	CTime pTime = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format("%d-%d-%d",pTime.GetYear(),pTime.GetMonth(),pTime.GetDay());

 //  LPVOID m_pBMPBuffer;
	CString temp;
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
					
				//	pSet->MoveNext();
					pSet->Close();
	  				//释放
	  				pSet.Release();
					
					break;
				}
		//	}
		//	Sleep(50);


			}
			else
			{
				szExamineeName = _T("");
				strzkzm = _T("");
				strCarModel = _T("");
			}
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

BOOL CDD::ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
   
	CString temp;
	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;
			CString strSQL;	
	//		strSQL.Format("SELECT * FROM StudentPhoto WHERE 准考证明编号 = '%s' and 状态 = '3'",sCard);
			strSQL.Format("SELECT * FROM StudentPhoto WHERE 准考证明编号 = '%s'",sCard);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			if(pRecordset->RecordCount != 1)
			{
				return FALSE;
			}
			
			char* pbuf = NULL;
			
			long lDataSize = pRecordset->GetFields()->GetItem("照片")->ActualSize; //数据库中图像数据长度
			if (lDataSize > 0)
			{
				_variant_t varBLOB;
				varBLOB = pRecordset->GetFields()->GetItem("照片")->GetChunk(lDataSize);
				m_pBMPBuffer = new char[lDataSize+1];
				
				pRecordset->Close();
				pRecordset.Release();
				
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBMPBuffer, pbuf, lDataSize);//复制数据到缓冲区m_pBMPBuffer
					
					SafeArrayUnaccessData(varBLOB.parray);
					
					//******************************************************//
					IStream* pStm;
					LONGLONG cb = lDataSize;
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
					LPVOID pvData;
					if (hGlobal != NULL)
					{
						pvData = GlobalLock(hGlobal);
						memcpy(pvData, m_pBMPBuffer, cb);
						GlobalUnlock(hGlobal);
						CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
					}
					else
					{
						//AfxMessageBox("读取照片信息错误");
						return FALSE;
					}
					
					IPicture *pImg = NULL;    
					if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
					{
						OLE_XSIZE_HIMETRIC hmWidth;
						OLE_YSIZE_HIMETRIC hmHeight;
						
						pImg->get_Width(&hmWidth);
						pImg->get_Height(&hmHeight);
						pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
					}
					else
					{
						return FALSE;
					}
					pStm->Release ();
					pImg->Release();
					GlobalFree(hGlobal); 
					if(m_pBMPBuffer)
					{
						delete m_pBMPBuffer;
						m_pBMPBuffer = NULL;
					}
					
				}
				pbuf = NULL;
			}
			else
			{
				pRecordset->Close();
				pRecordset.Release();
			}
	//	}

	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		return FALSE;
	}
	
	return TRUE;

}

void CDD::OnShowPHO()
{
	CDC *pDC = this->GetDC();
	CDC dcMem; 
	dcMem.CreateCompatibleDC(pDC); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BMPPHOBK); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
	this->ReleaseDC(pDC);
	CRect rt(0,0,bitmap.bmWidth,bitmap.bmHeight);
	ReadPhotoFromDB(&dcMem,strzkzm,&rt);
	//////////////////////////////////////////////////////////////////////////
	HDC hdc;
	hdc=GetDlgItem(IDC_STATIC_PHO)->GetDC()->m_hDC;
	CDC *pDC1= CDC::FromHandle(hdc);
	pDC1->StretchBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);

//防止GDI泄露
	bmpBackground.DeleteObject();  //防止GDI泄露
	pbmpOld->DeleteObject();
	this->ReleaseDC(pDC1);

}

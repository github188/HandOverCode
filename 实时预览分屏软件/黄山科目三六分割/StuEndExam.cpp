// StuEndExam.cpp : implementation file
//

#include "stdafx.h"
#include "cfourpic.h"
#include "StuEndExam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStuEndExam dialog
extern CCFourPicApp theApp;

CStuEndExam::CStuEndExam(CWnd* pParent /*=NULL*/)
	: CDialog(CStuEndExam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStuEndExam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStuEndExam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStuEndExam)
	DDX_Control(pDX, IDC_COMBOITEM, m_CBItem);
	DDX_Control(pDX, IDC_COMBOERR, m_CBErrInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStuEndExam, CDialog)
	//{{AFX_MSG_MAP(CStuEndExam)
	ON_CBN_SELCHANGE(IDC_COMBOITEM, OnSelchangeComboitem)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADDERR, OnBtnAdderr)
	ON_BN_CLICKED(IDC_BTN_STUEND, OnBtnStuend)
	ON_BN_CLICKED(IDC_BTNONCANCEL, OnBtnoncancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStuEndExam message handlers


void CStuEndExam::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CStuEndExam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnInitSQL();
	m_CBItem.InsertString(0,"通用评判");
	m_CBItem.InsertString(1,"上车准备");
	m_CBItem.InsertString(2,"起步");
	m_CBItem.InsertString(3,"会车");
	m_CBItem.InsertString(4,"左转弯");
	m_CBItem.InsertString(5,"右转弯");
	m_CBItem.InsertString(6,"直行路口");
	m_CBItem.InsertString(7,"变更车道");
	m_CBItem.InsertString(8,"超车");
    m_CBItem.InsertString(9,"通过人行横道");
	m_CBItem.InsertString(10,"掉头");
	m_CBItem.InsertString(11,"通过公共汽车站");
	m_CBItem.InsertString(12,"通过学校区域");
	m_CBItem.InsertString(13,"直线行驶");
    m_CBItem.InsertString(14,"靠边停车");	
	
    m_CBItem.SetCurSel(0);

	m_CBErrInfo.InsertString(0,"遮挡、关闭车内音视频监控设备(人工评判)");
	m_CBErrInfo.InsertString(1,"不按考试员指令驾驶(人工评判)");
	m_CBErrInfo.InsertString(2,"驾驶汽车双手同时离开转向盘(人工评判)");
	m_CBErrInfo.InsertString(3,"单手控制转向盘时不能平稳控制行驶方向(人工评判)");
	m_CBErrInfo.InsertString(4,"车辆在行驶中低头看挡或连续2次挂挡不进(人工评判)");
	m_CBErrInfo.InsertString(5,"视线离开行驶方向超过2s(人工评判)");
	m_CBErrInfo.InsertString(6,"通过积水路面和遇行人有不减速等不文明驾驶(人工评判)");
	m_CBErrInfo.InsertString(7,"通过人行横道不停车，不主动避让车辆和行人(人工评判)");
	m_CBErrInfo.InsertString(8,"行驶中身体任何部位伸出窗外(人工评判)");
	m_CBErrInfo.InsertString(9,"对可能出现危险的情形未采取减速等安全措施(人工评判)");
	m_CBErrInfo.InsertString(10,"因观察、判断或者操作不当出现危险情况(人工评判)");
	m_CBErrInfo.InsertString(11,"考生未按照预约考试时间参加考试(人工评判)");
	m_CBErrInfo.InsertString(12,"驾驶姿势不正确(人工评判)");
	m_CBErrInfo.InsertString(13,"操纵转向盘手法不合理(人工评判)");
	m_CBErrInfo.InsertString(14,"换挡时发生齿轮撞击(人工评判)");
	m_CBErrInfo.InsertString(15,"遇情况时不会合理使用离合器半联动控制车速(人工评判)");
	m_CBErrInfo.InsertString(16,"制动不平顺(人工评判)");

	m_CBErrInfo.SetCurSel(0);
	
	
	if(EndType==2)
	{
        GetDlgItem(IDC_BTN_ADDERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNONCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STUEND)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_ADDERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNONCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STUEND)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStuEndExam::OnSelchangeComboitem() 
{
	// TODO: Add your control notification handler code here
	//　int nIndex = m_combo.GetCurSel();
	
	
	int nIndex=m_CBItem.GetCurSel();
    
	m_CBErrInfo.ResetContent();
	switch(nIndex)
	{
	case 0: //通用评判
		m_CBErrInfo.InsertString(0,"遮挡、关闭车内音视频监控设备(人工评判)");
		m_CBErrInfo.InsertString(1,"不按考试员指令驾驶(人工评判)");
		m_CBErrInfo.InsertString(2,"驾驶汽车双手同时离开转向盘(人工评判)");
		m_CBErrInfo.InsertString(3,"单手控制转向盘时不能平稳控制行驶方向(人工评判)");
		m_CBErrInfo.InsertString(4,"车辆在行驶中低头看挡或连续2次挂挡不进(人工评判)");
		m_CBErrInfo.InsertString(5,"视线离开行驶方向超过2s(人工评判)");
		m_CBErrInfo.InsertString(6,"通过积水路面和遇行人有不减速等不文明驾驶(人工评判)");
		m_CBErrInfo.InsertString(7,"通过人行横道不停车，不主动避让车辆和行人(人工评判)");
	    m_CBErrInfo.InsertString(8,"行驶中身体任何部位伸出窗外(人工评判)");
		m_CBErrInfo.InsertString(9,"对可能出现危险的情形未采取减速等安全措施(人工评判)");
		m_CBErrInfo.InsertString(10,"因观察、判断或者操作不当出现危险情况(人工评判)");
		m_CBErrInfo.InsertString(11,"考生未按照预约考试时间参加考试(人工评判)");
		m_CBErrInfo.InsertString(12,"驾驶姿势不正确(人工评判)");
		m_CBErrInfo.InsertString(13,"操纵转向盘手法不合理(人工评判)");
		m_CBErrInfo.InsertString(14,"换挡时发生齿轮撞击(人工评判)");
		m_CBErrInfo.InsertString(15,"遇情况时不会合理使用离合器半联动控制车速(人工评判)");
		m_CBErrInfo.InsertString(16,"制动不平顺(人工评判)");	   
		break;

	case 1: //上车准备
		m_CBErrInfo.InsertString(0,"打开车门前不观察后方交通情况(人工评判)");
		break;
	case 2: //起步
		m_CBErrInfo.InsertString(0,"制动气压不足起步(人工评判)");
		m_CBErrInfo.InsertString(1,"起步前未观察后视镜和侧头观察后方交通情况(人工评判)");
		m_CBErrInfo.InsertString(2,"交通情况复杂时起步不能合理使用喇叭(人工评判)");
		m_CBErrInfo.InsertString(3,"起步时车辆发生闯动(人工评判)");
		m_CBErrInfo.InsertString(4,"启动发动机前不检查驾驶座椅、后视镜、仪表(人工评判)");	                            
		
        break;
	case 3: //会车
		m_CBErrInfo.InsertString(0,"会车困难时不让行(人工评判)");
		m_CBErrInfo.InsertString(1,"横向安全间距判断差，紧急转向避让对方来车(人工评判)");

        break;
	case 4: //左转弯
		m_CBErrInfo.InsertString(0,"不按规定减速或停车瞭望(左转人工评判)");
		m_CBErrInfo.InsertString(1,"左、右方和转弯路口时，未观察交通情况(左转人工评判)");

        break;
	case 5: //右转弯
		m_CBErrInfo.InsertString(0,"不按规定减速或停车瞭望(右转人工评判)");
		m_CBErrInfo.InsertString(1,"左、右方和转弯路口时，未观察交通情况(右转人工评判)");
        break;
	case 6: //直行路口
		m_CBErrInfo.InsertString(0,"不按规定减速或停车瞭望(直行人工评判)");
		m_CBErrInfo.InsertString(1,"左、右方和转弯路口时，未观察交通情况(直行人工评判)");
		break;
	case 7: //变更车道
		m_CBErrInfo.InsertString(0,"变更车道未通过后视镜观察后方道路交通情况(人工评判)");
		m_CBErrInfo.InsertString(1,"变更车道时判断车辆安全距离不合理(人工评判)");
        break;
	case 8: //超车
		m_CBErrInfo.InsertString(0,"超车前不通过内外后视镜观察后方和左侧交通(人工评判)");
		m_CBErrInfo.InsertString(1,"超车时机选择不合理影响其他车辆正常行驶(人工评判)");
		m_CBErrInfo.InsertString(2,"超车时，未侧头观察被超越车辆动态(人工评判)");
	    m_CBErrInfo.InsertString(3,"超车时未与被超越车辆保持安全距离(人工评判)");
	    m_CBErrInfo.InsertString(4,"超车后急转向驶回原车道妨碍被超车辆行驶(人工评判)");
		m_CBErrInfo.InsertString(5,"没有中心线和同方向只有一条行车道从右侧超车(评判)");
		m_CBErrInfo.InsertString(6,"当后车发出超车信号，不减速靠右让行(人工评判)");
		
        break;
	case 9: //通过人行横道
		m_CBErrInfo.InsertString(0,"不观察左、右方交通情况(人行横道人工评判)");
		m_CBErrInfo.InsertString(1,"未停车礼让行人(人行横道人工评判)");
	    break;
	case 10: //掉头
		m_CBErrInfo.InsertString(0,"不能正确观察交通情况选择掉头时机(人工评判)");
		m_CBErrInfo.InsertString(1,"掉头地点选择不当(人工评判)");
		m_CBErrInfo.InsertString(2,"掉头前未发出掉头信号(人工评判)");
		m_CBErrInfo.InsertString(2,"掉头时妨碍正常行驶的车辆和行人通行(人工评判)");

	    break;
	case 11: //通过公共汽车站
		m_CBErrInfo.InsertString(0,"不观察左、右方交通情况(公共汽车站人工评判)");
		m_CBErrInfo.InsertString(1,"未停车礼让行人(公共汽车站人工评判)");
	    break;
	case 12: //通过学校区域
		m_CBErrInfo.InsertString(0,"不观察左、右方交通情况(学校区域人工评判)");
		m_CBErrInfo.InsertString(1,"未停车礼让行人(学校区域人工评判)");
	    break;
	case 13: //直线行驶
		m_CBErrInfo.InsertString(0,"不适时通过内、外后视镜观察后方交通情况(人工评判)");
		m_CBErrInfo.InsertString(1,"未及时发现路面障碍物发现障碍物未及时减速(人工评判)");

        break;
	case 14: //靠边停车
		m_CBErrInfo.InsertString(0,"停车前，不通过后视镜观察交通情况(人工评判)");
		m_CBErrInfo.InsertString(1,"考试员发出靠边停车指令未能在规定距离停车(人工评判)");
		m_CBErrInfo.InsertString(1,"左、右方和转弯路口时，未观察交通情况(直线人工评判)");
        break;

	}

}

void CStuEndExam::OnBtnAdderr() 
{
	// TODO: Add your control notification handler code here
	CString sItem="",sErrInfo="",sInfo="";
	m_CBItem.GetWindowText(sItem);
    m_CBErrInfo.GetWindowText(sErrInfo);
    

	if(sItem.IsEmpty() || sErrInfo.IsEmpty())
	{
		MessageBox("请选择扣分项目和扣分项");
		return;
	}

	if (IDOK == MessageBox("将该考生考试终止!\n该考生本次考试不合格\n是否确认?", 
		"请确认...", MB_OKCANCEL|MB_ICONQUESTION))
	{
		ManuExamEndStr(sEndExamCard,sErrInfo);
		//日志记录
		
	}
	else
	{
		return;
	}

	
   MessageBox("人工评判添加完成！");

   SendMessage(WM_CLOSE);


}

void CStuEndExam::ManuExamEndStr(CString sCard,CString sErr)
{
	_RecordsetPtr pRecordset;
	CString strSQL,sStuCard;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vSir1,vSir2,vKSRQ,vRd;
	int iZT,iDrcs,iKscs;
	if (sCard == "准考证号:")
	{
		AfxMessageBox("准考证明编号不能为空");
		return;
	}
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM StudentInfo WHERE 准考证明编号='%s'",sCard);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		

			vZT  = pRecordset->GetCollect("状态");
			vKscs  = pRecordset->GetCollect("考试次数");
			vDrcs  = pRecordset->GetCollect("当日次数");
			vStuCard = pRecordset->GetCollect("准考证明编号");
			vSir1 = pRecordset->GetCollect("考试员1");
			vSir2 = pRecordset->GetCollect("考试员2");
		    vKSRQ = pRecordset->GetCollect("预考日期");
			vRd =   pRecordset->GetCollect("考试路线");
		
		iZT = atoi(VariantToStr(vZT));
        iKscs = atoi(VariantToStr(vKscs));
		iDrcs = atoi(VariantToStr(vDrcs));
		sStuCard=VariantToStr(vStuCard);
		if (iZT != 3)
		{
			AfxMessageBox("该考生已完成考试，无法远程终止!\n请通过人工评判对本次考试追加评判!");
		}
		else
		{  
			if(sErr=="通用评判-考试作弊或违规操作")// 
			{
				ManuExamAllEndAddErr(sStuCard,iKscs,iDrcs,sErr);
				pRecordset->PutCollect("结束考试",_variant_t("2"));
			}
			else
			{ 
				ManuExamEndAddErr(sStuCard,iKscs,iDrcs,sErr);
				pRecordset->PutCollect("结束考试",_variant_t("1"));
			}
			pRecordset->Update();
		}
		
		pRecordset->Close();


		
	
		ModifyRecord(sEndExamCard,"人工评判","",m_stSIR);
		
		
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("读取%s学员信息失败!",sCard);
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}	
	

	
}

void CStuEndExam::ManuExamStuEnd(CString sCard,int inum)//合格结束评判不合格
{
	if (sCard == "准考证号:")
	{
		AfxMessageBox("准考证明编号不能为空");
		return;
	}
	
	_RecordsetPtr pRecordset;
	CString strSQL,sStuCard;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard;
	int iZT,iDrcs,iKscs;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM StudentInfo WHERE 准考证明编号='%s'",sCard);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);

			vZT  = pRecordset->GetCollect("状态");
			vKscs  = pRecordset->GetCollect("考试次数");
			vDrcs  = pRecordset->GetCollect("当日次数");
			vStuCard = pRecordset->GetCollect("准考证明编号");
			vStuSir1 = pRecordset->GetCollect("考试员1");
			vStuSir2 = pRecordset->GetCollect("考试员2");
			vStuKSRQ = pRecordset->GetCollect("预考日期");
			vStuRd =   pRecordset->GetCollect("考试路线");

		iZT = atoi(VariantToStr(vZT));
        iKscs = atoi(VariantToStr(vKscs));
		iDrcs = atoi(VariantToStr(vDrcs));
		sStuCard=VariantToStr(vStuCard);
		if (iZT != 1)
		{
			AfxMessageBox("该考生不为合格!\n无法操作!");
		}
		else
		{  
			
			CString anum;
			anum.Format("%d",inum);
			ManuExamAllEndAddErr(sStuCard,iKscs,iDrcs,anum);
			ManuExamAllEndAddExam(sStuCard,iKscs,iDrcs);
			pRecordset->PutCollect("当日次数", _variant_t("2"));
			pRecordset->Update();
			
			ModifyRecord(sEndExamCard,"人工评判(合格后)","",m_stSIR);
		}
		
		pRecordset->Close();
		
		
		
		
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("读取%s学员信息失败!",sCard);
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}	
	
	
}

void CStuEndExam::ManuExamEndAddErr(CString sStuCard,int iKscs,int iDric,CString sErr) //增加错误项
{
	
	_RecordsetPtr ipRecordset;
	CString istrSQL;
	ipRecordset.CreateInstance("ADODB.Recordset");
	ipRecordset->CursorLocation = adUseClient;
	istrSQL.Format("SELECT * FROM ErrorData WHERE 扣分类型='%s'",sErr);
	ipRecordset->Open((_variant_t)_bstr_t(istrSQL), _variant_t((IDispatch*)m_pConnex, true), adOpenDynamic, adLockOptimistic, adCmdText);
	
	_variant_t num;
	int inum;
	CString iinum;
	num=ipRecordset->GetCollect("错误编号");
	inum = atoi(VariantToStr(num));
	iinum.Format("%d",inum);
	
	
	
	
	CString strSQL;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vDateTime,vErr;
	
	CString nDateTime;
	CString nDate;
	COleDateTime OleTime = COleDateTime::GetCurrentTime();
	nDateTime.Format("%s", OleTime.Format("%Y-%m-%d %H:%M:%S"));
	
	
	_RecordsetPtr pRecordset;
	
	
	
	ChangeToVariant(sStuCard, &vStuCard, adBSTR);
	ChangeToVariant(iKscs, &vKscs, adInteger);
	ChangeToVariant(iDric, &vDrcs, adInteger);
	ChangeToVariant(nDateTime, &vDateTime, adBSTR); //错误的时间
   	ChangeToVariant(sErr, &vErr, adBSTR); //错误的时间
	
	
	
	
    
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM ErrorRecords WHERE (准考证明编号 = '%s') AND (考试次数 = %d) AND (当日次数 = %d)",sStuCard,iKscs,iDric);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		pRecordset->AddNew();
		
		pRecordset->PutCollect("准考证明编号",vStuCard);
		pRecordset->PutCollect("错误编号",_variant_t(iinum));
		pRecordset->PutCollect("出错时间", vDateTime);
		pRecordset->PutCollect("考试次数", vKscs);
		pRecordset->PutCollect("当日次数", vDrcs);
		pRecordset->PutCollect("备注", vErr);
		
		
		pRecordset->Update();
		
		pRecordset->Close();
		
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("读取%s学员信息失败!",sStuCard);
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}			
}

void CStuEndExam::ManuExamAllEndAddErr(CString sStuCard,int iKscs,int iDric,CString sErr) //增加错误项
{
	
	CString strSQL;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vDateTime,vErr;
	
	CString nDateTime;
	CString nDate;
	COleDateTime OleTime = COleDateTime::GetCurrentTime();
	nDateTime.Format("%s", OleTime.Format("%Y-%m-%d %H:%M:%S"));
	
	
	_RecordsetPtr pRecordset;
	
	
	
	ChangeToVariant(sStuCard, &vStuCard, adBSTR);
	ChangeToVariant(iKscs, &vKscs, adInteger);
//	ChangeToVariant(iDric, &vDrcs, adInteger);
	ChangeToVariant(nDateTime, &vDateTime, adBSTR); //错误的时间
   	ChangeToVariant(sErr, &vErr, adBSTR); //错误的时间
	
    
	for(int i=iDric;i<=2;i++)
	{
		if(i>0 && i<3)
		{
			try
			{

		        //////////////////////////////////////////////////////////////////////////
				pRecordset.CreateInstance("ADODB.Recordset");
				pRecordset->CursorLocation = adUseClient;
				strSQL.Format("SELECT * FROM ErrorRecords WHERE (准考证明编号 = '%s') AND (考试次数 = %d) AND (当日次数 = %d)",sStuCard,iKscs,i);
				
				pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
					adOpenDynamic, adLockOptimistic, adCmdText);
				

				pRecordset->AddNew();
				
// 				int iinum;
// 				iinum =atoi(sErr);
				ChangeToVariant(i, &vDrcs, adInteger);
				pRecordset->PutCollect("准考证明编号",vStuCard);
				pRecordset->PutCollect("错误编号",_variant_t(sErr));
				pRecordset->PutCollect("出错时间", vDateTime);
				pRecordset->PutCollect("考试次数", vKscs);
				pRecordset->PutCollect("当日次数", vDrcs);
				pRecordset->PutCollect("备注", vErr);
				
				
				pRecordset->Update();
				
				pRecordset->Close();
                //////////////////////////////////////////////////////////////////////////


				////////////////////////////////////////////////////////////////////////////





				////////////////////////////////////////////////////////////////////////////


				
			}
			catch (_com_error e)
			{
				CString strErr;
				strErr.Format("读取%s学员信息失败!",sStuCard);
				AfxMessageBox(strErr);
				AfxMessageBox(e.Description());
				return ;
			}
		}
	}



	
}


void CStuEndExam::ManuExamAllEndAddExam(CString sStuCard,int iKscs,int iDric) //增加Exam
{

	CString strSQL;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vDateTime,vErr;
	
	CString nDateTime;
	CString nDate;
	COleDateTime OleTime = COleDateTime::GetCurrentTime();
	nDateTime.Format("%s", OleTime.Format("%Y-%m-%d %H:%M:%S"));
	
	
	_RecordsetPtr pRecordset;
	
	
	
	ChangeToVariant(sStuCard, &vStuCard, adBSTR);
	ChangeToVariant(iKscs, &vKscs, adInteger);
	//	ChangeToVariant(iDric, &vDrcs, adInteger);
	ChangeToVariant(nDateTime, &vDateTime, adBSTR); //错误的时间
 
	
    CString sDRCS;

	for(int i=iDric;i<=2;i++)
	{
		if(i>0 && i<3)
		{
			sDRCS.Format("%d",i);
			ChangeToVariant(i, &vDrcs, adInteger);

			try
			{
				pRecordset.CreateInstance("ADODB.Recordset");
	        	pRecordset->CursorLocation = adUseClient;
		
				strSQL.Format("SELECT * FROM ExamRecord WHERE 准考证明编号 = '%s' AND 考试次数 = %d AND 当日次数=%d",sStuCard,iKscs,i);
			
				
				pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
					adOpenDynamic, adLockOptimistic, adCmdText);

				while (!pRecordset->adoEOF)
				{
					pRecordset->Delete(adAffectCurrent);
					pRecordset->MoveNext();
				}
				pRecordset->UpdateBatch(adAffectAll);

				//添加到SQL
				pRecordset->AddNew();
				pRecordset->PutCollect("准考证明编号", vStuCard);
				pRecordset->PutCollect("当日次数", vDrcs);
				pRecordset->PutCollect("考试次数", vKscs);
				pRecordset->PutCollect("考试员1", vStuSir1);
				pRecordset->PutCollect("考试员2", vStuSir2);
				pRecordset->PutCollect("考试路线", vStuRd);
				pRecordset->PutCollect("考试日期", vStuKSRQ);

				
				pRecordset->Update();
				pRecordset->Close();
				


				
			}
			catch (_com_error e)
			{
				AfxMessageBox("考试结果记录不能删除!");
				AfxMessageBox(e.Description());
				AfxMessageBox(e.ErrorMessage());
			}
		}
	}



}



void CStuEndExam::OnBtnStuend() 
{
	// TODO: Add your control notification handler code here
	CString sErrInfo="";
	m_CBErrInfo.GetWindowText(sErrInfo);
	_RecordsetPtr pRecordset;
	CString strSQL;
	pRecordset.CreateInstance("ADODB.Recordset");
	pRecordset->CursorLocation = adUseClient;
	strSQL.Format("SELECT * FROM ErrorData WHERE 扣分类型='%s'",sErrInfo);
	pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
		adOpenDynamic, adLockOptimistic, adCmdText);
	
	_variant_t num;
	int inum;
	
	num=pRecordset->GetCollect("错误编号");
	inum = atoi(VariantToStr(num));
	
	
	if (IDOK == MessageBox("该考生本次考试评判为不合格\n是否确认?", 
		"请确认...", MB_OKCANCEL|MB_ICONQUESTION))
	{

		ManuExamStuEnd(sEndExamCard,inum);
		//日志记录
		
		
	}
	else
	{
		return;
	}
	
	
	MessageBox("人工评判添加完成！");
	
	
	SendMessage(WM_CLOSE);
}

void CStuEndExam::OnBtnoncancel() 
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
}


//日志记录
void CStuEndExam::ModifyRecord(CString sCard, CString sModify, CString sBZ, CString sOperator)
{
	_RecordsetPtr pRecordset;
	CString strSQL;
	COleDateTime modifytime;
	modifytime = COleDateTime::GetCurrentTime();
	
	_variant_t vCard, vModify, vtime, vsir1, vsir2, vBZ;
	ChangeToVariant(modifytime, &vtime, adDBDate);
	ChangeToVariant(sCard, &vCard, adBSTR);
	ChangeToVariant(sModify, &vModify, adBSTR);
	ChangeToVariant(sOperator, &vsir1, adBSTR);
	//	ChangeToVariant(m_szTeacher2Name, &vsir2, adBSTR);
	ChangeToVariant(sBZ, &vBZ, adBSTR);
	
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL = "SELECT top 10 * FROM ModifyRecord";
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		pRecordset->AddNew();
		pRecordset->PutCollect("操作对象",vCard);
		pRecordset->PutCollect("操作描述",vModify);
		pRecordset->PutCollect("时间",vtime);
		pRecordset->PutCollect("操作人员1",vsir1);
		pRecordset->PutCollect("操作人员2",vsir2);
		pRecordset->PutCollect("备注",vBZ);
		
		pRecordset->Update();
		
		pRecordset->Close();
		pRecordset.Release();
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr="记录操作日志失败!";
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}
	
}

BOOL CStuEndExam::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConnex.CreateInstance("ADODB.Connection");
	if(m_pConnex==NULL)//CoInitialize（NULL）未初始化
	{
		TRACE("初始化连接对象失败\n");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
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
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		TRACE(Sqlstr);
		res = m_pConnex->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("打开数据库成功!");
			WriteLog("打开数据库成功!");
		}
		else
		{
			TRACE("打开数据库失败!");
			WriteLog("打开数据库失败!");
			return FALSE;
		}
	}
	catch(_com_error e)		//捕捉异常
	{
		CString errormessage;
		errormessage.Format("数据库提示,连接数据库失败!,错误信息:%s,请先配置数据库连接",e.ErrorMessage());
		AfxMessageBox(errormessage);
		ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}
// AutoPrintManagerDlg.h : header file
//

#if !defined(AFX_AUTOPRINTMANAGERDLG_H__AF249C4E_9DD4_4ACF_B223_57B3628F6CC9__INCLUDED_)
#define AFX_AUTOPRINTMANAGERDLG_H__AF249C4E_9DD4_4ACF_B223_57B3628F6CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_PRINTTHR_MSG WM_USER+60
/////////////////////////////////////////////////////////////////////////////
// CAutoPrintManagerDlg dialog
struct SQLThreadInfo
{
	CString strSQLIP,strUser,strPWD,strDBName;
	int Thri;
	BOOL Bstop;
};



class CAutoPrintManagerDlg : public CDialog
{
// Construction
public:

	HMODULE hmodule;
	typedef int (ADDFUNC) (CString ,CString ,CString ,CString ,int );//定义一个函数
	ADDFUNC *lpfnadd;
	HMODULE hmodule2;
	typedef int (ADDFUNCNUM) (LPCTSTR ,LPCTSTR ,LPCTSTR ,LPCTSTR );//定义一个函数
	ADDFUNCNUM *lpfnadd2;

	HMODULE hmodule3;
	typedef int (ADDFUNCTZ) (LPCTSTR ,LPCTSTR ,LPCTSTR ,LPCTSTR );//定义一个函数
	ADDFUNCNUM *lpfnadd3;

	//////////////////////////////////////////////////////////////////////////
	CString sstrPath;  //软件的绝对路径
	TCHAR FPath[128];
	BOOL BTNMake;
	int STRS;

	CRect m_LargerRect;//大
	CRect m_SmallRect; //小
	BOOL	m_Set;
	void UpDateExamStut();
	CString Decode(CString str);
	SQLThreadInfo m_info[50];
	int dyrc[50];
	void UpdateList();
	void UpdatePass();
	CAutoPrintManagerDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL CreatDBCon();
	BOOL ConSQLCon(_ConnectionPtr &m_pConn);
	_ConnectionPtr m_pConnDB;
	BOOL ReadSQLINFO(CString strSQL,_ConnectionPtr m_pConn);
	void AllStudent(CString strSQL);
	void PrintSchoolReport();
	void UpdateSQL(CString sZKZM);
	void UpdateStudent(CString strSQL);
	BOOL updateSQLINFO(CString strSQL,_ConnectionPtr m_pConn);
// Dialog Data
	//{{AFX_DATA(CAutoPrintManagerDlg)
	enum { IDD = IDD_AUTOPRINTMANAGER_DIALOG };
	CComboBox	m_ZDprint;
	CButton	m_checkKSY1;
	CButton	m_SText;
	CListCtrl	m_ListMake;
	CDateTimeCtrl	m_sDate;
	CDateTimeCtrl	m_Date;
	CComboBox	m_commd;
	CDateTimeCtrl	m_datehg;
	CComboBox	m_CombDr;
	CListCtrl	m_ListAllKC;
	CString	m_ESer;
	CString	m_EUser;
	CString	m_EPwd;
	CString	m_KCMC;
	CString	m_Ezkzm5;
	BOOL	m_CheckSet1;
	BOOL	m_CheckSet2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoPrintManagerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAutoPrintManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBTNAutoPrint();
	afx_msg void OnLinktest();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnAdd();
	afx_msg void OnItemchangedListAllkc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBTNPrint();
	afx_msg LRESULT OnPrintThrMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnyl();
	afx_msg void OnClose();
	afx_msg void OnMakeBtnStudent();
	afx_msg void OnCheckAll2();
	afx_msg void OnButtonNAMEConfirmed();
	afx_msg void OnRclickListAllkc2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitem32773();
	afx_msg void OnMenuitem32774();
	afx_msg void OnDblclkListAllkc2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitem32782();
	afx_msg void OnMenuitem32783();
	afx_msg void OnMenuitem32784();
	afx_msg void OnMenuitem32785();
	afx_msg void OnMenuitem32789();
	afx_msg void OnMenuitem32786();
	afx_msg void OnMenuitem32787();
	afx_msg void OnMenuitem32788();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuitem32793();
	afx_msg void OnRclickListAllkc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListAllkc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckKsy1();
	afx_msg void OnMenuitem32800();
	afx_msg void OnCheckAll3();
	afx_msg void OnBTNAutoPrint1J();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOPRINTMANAGERDLG_H__AF249C4E_9DD4_4ACF_B223_57B3628F6CC9__INCLUDED_)

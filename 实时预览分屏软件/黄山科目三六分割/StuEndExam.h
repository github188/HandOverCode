#if !defined(AFX_STUENDEXAM_H__F5C11CF6_938A_4F20_991F_873EA2D58D15__INCLUDED_)
#define AFX_STUENDEXAM_H__F5C11CF6_938A_4F20_991F_873EA2D58D15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StuEndExam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStuEndExam dialog

class CStuEndExam : public CDialog
{
// Construction
public:
	void ModifyRecord(CString sCard, CString sModify, CString sBZ, CString sOperator);
	BOOL OnInitSQL();
	_ConnectionPtr m_pConnex;
	_variant_t vStuSir1,vStuSir2,vStuKSRQ,vStuRd;
	int    EndType;
	CString sEndExamCard;
	CString m_stSIR;
	CStuEndExam(CWnd* pParent = NULL);   // standard constructor
    void ManuExamEndStr(CString sCard,CString sErr);
	void ManuExamEndAddErr(CString sStuCard,int iKscs,int iDric,CString sErr); //增加错误项
	void ManuExamAllEndAddErr(CString sStuCard,int iKscs,int iDric,CString sErr); //同时两次都要结束
	void ManuExamStuEnd(CString sCard,int nErr);//合格后评判不合格
	void ManuExamAllEndAddExam(CString sStuCard,int iKscs,int iDric); //增加Exam

// Dialog Data
	//{{AFX_DATA(CStuEndExam)
	enum { IDD = IDD_ENDEXAM };
	CComboBox	m_CBItem;
	CComboBox	m_CBErrInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStuEndExam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStuEndExam)
	afx_msg void OnSelchangeComboitem();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdderr();
	afx_msg void OnBtnStuend();
	afx_msg void OnBtnoncancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STUENDEXAM_H__F5C11CF6_938A_4F20_991F_873EA2D58D15__INCLUDED_)

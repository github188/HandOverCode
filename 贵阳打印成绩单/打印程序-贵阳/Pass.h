#if !defined(AFX_PASS_H__DFE93F50_782A_4ECD_89FE_CB5DD82F8604__INCLUDED_)
#define AFX_PASS_H__DFE93F50_782A_4ECD_89FE_CB5DD82F8604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPass dialog

class CPass : public CDialog
{
// Construction
public:
	CPass(CWnd* pParent = NULL);   // standard constructor
	BOOL CreatDBCon();
	void UpdatePass();
	_ConnectionPtr m_pConnDB;
	CString strpass;
// Dialog Data
	//{{AFX_DATA(CPass)
	enum { IDD = IDD_DIALOG_Pass };
	CString	m_Pass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPass)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPass)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASS_H__DFE93F50_782A_4ECD_89FE_CB5DD82F8604__INCLUDED_)

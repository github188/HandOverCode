#if !defined(AFX_PZVIDEODLG_H__618C7767_D8D4_4067_BC38_F1C5FED860A3__INCLUDED_)
#define AFX_PZVIDEODLG_H__618C7767_D8D4_4067_BC38_F1C5FED860A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PZVideoDlg.h : header file
//
#include "ZSZButton.h"
#include "BmpDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CPZVideoDlg dialog

class CPZVideoDlg : public CDialog
{
// Construction
public:
	BOOL zbmpmessagebox(LPTSTR msg);
	CPZVideoDlg(CWnd* pParent = NULL);   // standard constructor
//////////////////////////////////////////////////////////////////////////
	HBRUSH   hbr1;
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CPZVideoDlg)
	enum { IDD = IDD_DLGPZ };
	CIPAddressCtrl	m_MediaIPAddr;
	CComboBox	m_ComboBoxML;
	CComboBox	m_ComboSXBH;
	CComboBox	m_ComSkin;
	CZSZButton	m_BTNClose;
	CIPAddressCtrl	m_IPAddrCtrl;
	CComboBox	m_ComboBoxBH;
	CListCtrl	m_ListConVideo;
	CString	m_Path;
	CString	m_Edit_BZ;
	CString	m_EditPort;
	CString	m_EditPwd;
	CString	m_EditTdh;
	CString	m_EditUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPZVideoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPZVideoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnchange();
	afx_msg void OnClose();
	afx_msg void OnBtnExit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeComboBh();
	afx_msg void OnBtnsave();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnDelete();
	afx_msg void OnItemchangedLISTBKVideo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboSkin();
	afx_msg void OnBtnskinset();
	afx_msg void OnBtnLmtip();
	afx_msg void OnBtnTyippwd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AddTBKVideo();
	BOOL UpdateCombox();
//	void loadcfg();
	BOOL OnInitSQLEX();
	BOOL UpdateList();

	_ConnectionPtr m_pConn;
	
	CStringArray ComboboxArray;
	//////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PZVIDEODLG_H__618C7767_D8D4_4067_BC38_F1C5FED860A3__INCLUDED_)

#if !defined(AFX_CBKPLAYZWNDXPPG_H__2E33B3ED_EBC6_4977_8806_B198AFC9A228__INCLUDED_)
#define AFX_CBKPLAYZWNDXPPG_H__2E33B3ED_EBC6_4977_8806_B198AFC9A228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CBKPlayzWndXPpg.h : Declaration of the CCBKPlayzWndXPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXPropPage : See CBKPlayzWndXPpg.cpp.cpp for implementation.

class CCBKPlayzWndXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCBKPlayzWndXPropPage)
	DECLARE_OLECREATE_EX(CCBKPlayzWndXPropPage)

// Constructor
public:
	CCBKPlayzWndXPropPage();

// Dialog Data
	//{{AFX_DATA(CCBKPlayzWndXPropPage)
	enum { IDD = IDD_PROPPAGE_CBKPLAYZWNDX };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCBKPlayzWndXPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKPLAYZWNDXPPG_H__2E33B3ED_EBC6_4977_8806_B198AFC9A228__INCLUDED)

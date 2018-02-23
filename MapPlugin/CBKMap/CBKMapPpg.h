#if !defined(AFX_CBKMAPPPG_H__E0263A79_9958_420C_BB47_B6E65D3663B7__INCLUDED_)
#define AFX_CBKMAPPPG_H__E0263A79_9958_420C_BB47_B6E65D3663B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CBKMapPpg.h : Declaration of the CCBKMapPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCBKMapPropPage : See CBKMapPpg.cpp.cpp for implementation.

class CCBKMapPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCBKMapPropPage)
	DECLARE_OLECREATE_EX(CCBKMapPropPage)

// Constructor
public:
	CCBKMapPropPage();

// Dialog Data
	//{{AFX_DATA(CCBKMapPropPage)
	enum { IDD = IDD_PROPPAGE_CBKMAP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCBKMapPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKMAPPPG_H__E0263A79_9958_420C_BB47_B6E65D3663B7__INCLUDED)

#if !defined(AFX_CPLAYBACKZOCXPPG_H__4E80C071_6D23_4997_A5B0_F88BE53FC9F1__INCLUDED_)
#define AFX_CPLAYBACKZOCXPPG_H__4E80C071_6D23_4997_A5B0_F88BE53FC9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CPlayBackzOcxPpg.h : Declaration of the CCPlayBackzOcxPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCPlayBackzOcxPropPage : See CPlayBackzOcxPpg.cpp.cpp for implementation.

class CCPlayBackzOcxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCPlayBackzOcxPropPage)
	DECLARE_OLECREATE_EX(CCPlayBackzOcxPropPage)

// Constructor
public:
	CCPlayBackzOcxPropPage();

// Dialog Data
	//{{AFX_DATA(CCPlayBackzOcxPropPage)
	enum { IDD = IDD_PROPPAGE_CPLAYBACKZOCX };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCPlayBackzOcxPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPLAYBACKZOCXPPG_H__4E80C071_6D23_4997_A5B0_F88BE53FC9F1__INCLUDED)

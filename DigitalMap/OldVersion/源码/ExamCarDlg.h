#if !defined(AFX_EXAMCARDLG_H__92DB6E8D_CB73_4F7B_9B54_E10E2A9AF27A__INCLUDED_)
#define AFX_EXAMCARDLG_H__92DB6E8D_CB73_4F7B_9B54_E10E2A9AF27A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define MAX_ExamCar 20
// ExamCarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExamCarDlg dialog
//考车类 用于在对话框里面使用的
class CExamCarDlg : public CDialog
{
// Construction
public:
	void ChangeTrackVideoPlayS(HTREEITEM pVCurItem);
	void ChangeTrackVideoPlayT(HTREEITEM pVCurItem);
	BOOL CloseExamCarVideoS();
	BOOL CloseExamCarVideoT();
	BOOL OpenExamCarVideoS(UINT ExamCarNo, UINT VideoAdd);
	BOOL OpenExamCarVideoT(UINT ExamCarNo,UINT VideoAdd);
	void ChangeTrackVideoPlay(HTREEITEM pVCurItem);
	void WriteLog(CString szERR);
	BOOL CloseExamCarVideo();
	BOOL OpenExamCarVideo(UINT ExamCarNo,UINT VideoAdd);
	int iExamCarVideo; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号  主驾驶视频
	int iExamCarVideot; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号  副驾驶视频
	int iExamCarVideos; //标记考车视频有没有开启 如果为-1则表示没有开启 为别的数字代表考车号  车外视频
	BOOL ArryExamCar[MAX_ExamCar]; //在线考车 1 在线 0 不在线
	int CurTrackExamCar;  //当前被跟踪的考车号
	int ArryExamCarPopbox[MAX_ExamCar];  //用来存放此时考车的提示框有没有开启 如果开启则 为 1 如果没有开启则为0
	void ChangeTrack(HTREEITEM pVCurItem);
	CExamCarDlg(CWnd* pParent = NULL);   // standard constructor
	HTREEITEM m_hDevItem;
// 	HTREEITEM m_FirstDevItem; //存储第一个进来的指针
// 	int iExamCarCount;  //保存当前在线的考车数
	CImageList *m_imagelist;
	BOOL InitMytree();
// Dialog Data
	//{{AFX_DATA(CExamCarDlg)
	enum { IDD = IDD_DIALOG_EXAM };
	CTreeCtrl	m_exam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExamCarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExamCarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMCARDLG_H__92DB6E8D_CB73_4F7B_9B54_E10E2A9AF27A__INCLUDED_)

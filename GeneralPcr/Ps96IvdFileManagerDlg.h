#pragma once
#include "filemanagedlg.h"

class CPs96IvdFileManagerDlg :public CFileManageDlg
{
public:
	CPs96IvdFileManagerDlg(CWnd* pParent = NULL);
	~CPs96IvdFileManagerDlg(void);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	virtual	void SetCtrlPos();
	virtual	void SetPageSwitchBtPos(int iStart);

	virtual	int AddNewPage(CExperimentData* pExpData,CString strFileName = _T(""));
	virtual	int DeletePage(int pageId);

public:
	CRect m_rcAddIcon;

protected:
	int m_iAddIconWHFat;
	int m_iAddIconWHThin;
	int m_iAddIconLeftIndent;
};


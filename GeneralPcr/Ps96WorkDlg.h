#pragma once
#include "WorkDlg.h"
#include "Ps96IvdFileManagerDlg.h"

class CPs96WorkDlg : public CWorkDlg
{
public:
	CPs96WorkDlg(void);
	~CPs96WorkDlg(void);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual	void InitFileManageDlg();
	virtual	int SetPageSelected(int iPageId);
	virtual int OpenFile(int iFileFormat);	//iFileFormat: 0--全格式，1-qstd格式，2-qsts格式
	virtual int OpenPowerOffProtectFile(CString strFilePath);

	virtual	void CreateCtrl();
	virtual	void InitCtrl();
	virtual	void SetCtrlPos();
	virtual	void ShowCtrl();

			void CreateSt();
			void InitSt();

			int DeleteTemplate(int iIndex);

			int RefreshExpDlgRunState(int iPageId,BOOL bOccupy);

public:
	CPs96IvdFileManagerDlg m_ps96FileManagerDlg;
};


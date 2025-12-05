#pragma once

//康研葆使用的实验信息页面

#include "ExpeInfoDlg.h"
#include "MyNewEdit.h"

class CCYExpInfoDlg : public CExpeInfoDlg
{
	DECLARE_DYNAMIC(CCYExpInfoDlg)

public:
	CCYExpInfoDlg(void);
	~CCYExpInfoDlg(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual	void CreateEd();
	virtual void InitEd();
	virtual void SetCtrlPos();
	virtual void ShowCtrl();

	virtual	void SetCtrlText();

	virtual	void GetExperiInfo(CExperimentInfo& experiInfo);
	virtual	void SetExperiInfo(CExperimentInfo& experiInfo);

	virtual	void SetCtrlEnableState(BOOL bEnabled);
	virtual	void ClearPage();
	virtual	int RefreshLanguageDisplay();
	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

public:
	CMyNewEdit m_edReportNo;
};



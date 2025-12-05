#pragma once

#include "SamInfoDlg.h"
#include "MyNewCb.h"
#include "MyNewEdit.h"

class CCYSamInfoDlg : public CSamInfoDlg
{
	DECLARE_DYNAMIC(CCYSamInfoDlg)

public:
	CCYSamInfoDlg(CWnd* pParent = NULL,CInterfaceC* pIfC = NULL);
	~CCYSamInfoDlg(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMsgDateCtrlChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam);

	virtual	void CreateCb();
	virtual	void CreateEd();
	virtual void CreateDt();

	virtual	void InitCb();
	virtual	void InitEd();
	virtual	void InitDt();

	virtual	void SetCtrlPos();
	virtual	void ShowCtrl();

	virtual	void SetCtrlText();

	virtual	void ClearSamInfo();
	virtual	void SetSamInfo(CTubeInfo& tubeInfo);

	virtual	void SetEditable(BOOL bEditable);
	virtual	BOOL GetEditable();

	virtual	int RefreshLanguageDisplay();

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

	virtual int SetCYSamInfo(CExperimentData* pExpData,BOOL bRedraw = FALSE);

public:
	CMyNewCb m_cbSamCategory;	//样品类别

	CMyNewEdit m_edSamCount;	//样品数量
	CMyNewEdit m_edSamState;	//样品状态
	CMyNewEdit m_edProductBN;	//产品批号
	CMyNewEdit m_edTestItem;	//检测项目
	CMyNewEdit m_edComment;	//实验备注，不同于单个样本孔位的备注

	COleDateTime m_oleCYDt;
	CMyDateTimeCtrl m_dtCYCollectDate;
};


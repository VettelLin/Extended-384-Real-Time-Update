#pragma once

#include "MyNewEdit.h"
#include "MyNewCb.h"
#include "MyDateTimeCtrl.h"
#include "SamNameAutoSetInfo.h"
#include "CalibAutoSetInfo.h"
#include "SelfDefType.h"
#include "TubeInfo.h"
#include "FontSet.h"
#include "EditModeAbs.h"
#include "RectManage.h"
#include "InterfaceC.h"
#include "ExperimentData.h"

class CSamInfoDlg : public CDialogEx,public CFontSet,public CEditModeAbs,public CRectManage
{
	DECLARE_DYNAMIC(CSamInfoDlg)

public:
	CSamInfoDlg(CWnd* pParent = NULL,CInterfaceC* pIfC = NULL);   // standard constructor
	virtual ~CSamInfoDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);

	afx_msg LRESULT OnMsgDateCtrlChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
			void InitDlg();
			void CreateCtrl();
			void CreateBt();
			void CreateCkbt();
			void CreateSt();
	virtual	void CreateCb();
	virtual	void CreateEd();
	virtual	void CreateDt();

			void InitCtrl();
			void InitBt();
			void InitCkbt();
			void InitSt();
	virtual	void InitCb();
			int InitOneCb(CMyNewCb* pCb);
	virtual	void InitEd();
			int InitOneEd(CMyNewEdit* pEd);
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

	virtual int SetCYSamInfo(CExperimentData* pExpData);

public:
	CMyNewCb m_cbSamType;
	CMyNewCb m_cbSexuality;
	
	CMyNewEdit m_edNewId;	//新样本ID
	CMyNewEdit m_edSamName;	//样本号（样本名称）
	CMyNewEdit m_edPaitentName;
	CMyNewEdit m_edAge;
	
	CMyNewEdit m_edSendingDoctor;
	CMyNewEdit m_edSendingSection;	//送检科室
	CMyNewEdit m_edAnamnesisNo;	//病历号
	CMyNewEdit m_edBedNo;	//病床号
	CMyNewEdit m_edHospitalId;	//住院号
	CMyNewEdit m_edOutpatientNo;	//门诊号
	CMyNewEdit m_edRemarks;	//备注

	COleDateTime m_collectDate;
	CMyDateTimeCtrl m_dtCollectDate;

	BOOL m_bEditable;	//页面是否可编辑

	CInterfaceC* m_pIfC;

	BOOL m_bClear;

	BOOL m_bCtrlCreated;
};


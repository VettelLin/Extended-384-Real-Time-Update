#pragma once

#include "MyButton.h"
#include "InstrumentInfoDlg.h"
#include "ExpeInfoDlg.h"
#include "CYExpInfoDlg.h"
#include "ExperimentInfo.h"
#include "ExperimentData.h"
#include "EditModeType.h"
#include "EditModeAbs.h"
#include "RectManage.h"
#include "resource.h"

class CBaseSetDlg : public CDialogEx,public CEditModeAbs,public CFontSet
{
	DECLARE_DYNAMIC(CBaseSetDlg)

public:
	CBaseSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBaseSetDlg();

// Dialog Data
	enum { IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
			void InitSonDlg();
			void CreateCtrl();
			void CreateSt();
			void InitCtrl();
			void InitSt();
			void SetCtrlPos();
			void ShowCtrl();

			void GetBaseInfo(CExperimentInfo& info);
			void SetBaseInfo(CExperimentInfo& info);

			void SetCtrlEnableState(BOOL bEnabled);
			void ClearPage();
	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

			void FileOpened(CExperimentData* pExpData);

			CString GetExpName();	//获取实验名称，和CExperiment中的可能不一致

			int RefreshLanguageDisplay();
			void SetSelectedInstrument(CString strModel, CString strSN);
			void ReshowInstrumentList();

public:
	CInstrumentInfoDlg m_instrumentInfoDlg;

#ifdef CAN_YOUNG
	CCYExpInfoDlg m_expeInfoDlg;
#else
	CExpeInfoDlg m_expeInfoDlg;
#endif

private:
	BOOL m_bCtrlCreated;

	CRectManage m_rcManage;
};





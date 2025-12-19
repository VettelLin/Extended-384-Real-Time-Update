#pragma once

#include "MyStatic.h"
#include "ExperimentInfo.h"
#include "RectManage.h"

class CMonitorBaseInfoDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMonitorBaseInfoDlg)

public:
	CMonitorBaseInfoDlg(BOOL bPrjManage = FALSE,CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitorBaseInfoDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateCb();
			void CreateEd();

			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitCb();
			void InitEd();
		
			void SetCtrlPos();
			void ResetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void SetCtrlEnableState(BOOL bEnabled);
			void ClearPage();

			int SetMonitorBaseInfo(CExperimentInfo* pExpInfo);

			int RefreshLanguageDisplay();

public:
	CMyStatic m_stStartTm;
	CMyStatic m_stEndTm;

	CMyStatic m_stTemLid;	
	CMyStatic m_stTemTube;

	CMyStatic m_stTotalSec;
	CMyStatic m_stRemainSec;

	// CMyProgressBar m_progressBar;	//实验进度

private:
	BOOL m_bCtrlCreated;

	CRectManage m_rcManage;
};

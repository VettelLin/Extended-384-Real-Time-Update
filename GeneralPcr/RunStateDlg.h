#pragma once

#include "MonitorBaseInfoDlg.h"
#include "RectManage.h"

class CRunStateDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CRunStateDlg)

public:
	CRunStateDlg(BOOL bPrjManage = FALSE,CWnd* pParent = NULL);   // standard constructor
	virtual ~CRunStateDlg();

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

			void InitSonDlg();
			void InitMonitorBaseInfoDlg();

			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitCb();
			void InitEd();
		
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlEnableState(BOOL bEnabled);
			void ClearPage();

			int RefreshLanguageDisplay();

public:
	CMonitorBaseInfoDlg m_mntBaseInfoDlg;

private:
	BOOL m_bCtrlCreated;

	CRectManage m_rcManage;
};

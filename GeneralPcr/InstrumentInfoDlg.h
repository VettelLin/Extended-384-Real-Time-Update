#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyCheckButton.h"
#include "MyNewCb.h"
#include "MyNewEdit.h"
#include "ExperimentInfo.h"
#include "resource.h"
#include "EditModeAbs.h"
#include "MyNewCb.h"
#include "RectManage.h"

const UINT WM_INSTRUMENT_REINITIALIZE = WM_USER + 1601; // 连接仪器重新初始化
const UINT WM_INSTRUMENT_SELECTED = WM_USER + 1602; // 当前选择使用设备序号



class CInstrumentInfoDlg : public CDialogEx,public CFontSet,public CEditModeAbs
{
	DECLARE_DYNAMIC(CInstrumentInfoDlg)

public:
	CInstrumentInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstrumentInfoDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnBnClickedFindInstrument();
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void InitCtrl();
			void InitBt();
			void InitCb();
			void SetCtrlPos();
			void ResetCtrlPos(BOOL bRedraw = FALSE);
			void ShowCtrl();

			void SetCtrlText();

			void GetInstruInfo(CExperimentInfo& info);
			void SetInstruInfo(CExperimentInfo& info);

			void SetCtrlEnableState(BOOL bEnabled);

			void ClearPage();

			int RefreshLanguageDisplay();

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

	void SetSelectedInstrument(CString strModel, CString strSN);
	void ReshowInstrumentList();

protected:
	void InitInstrument();
	CString CombineModelAndSN(CString strModel,CString strSN);
	void CanRunExperiment(int iSelDevice);

public:
	CMyNewCb m_cbInstrumentType;
	CMyButton m_btnFindInstrument; // 查找设备

	CMyCheckButton m_ckbtLidSet;
	CMyCheckButton m_ckbtTempCtrl;


private:
	BOOL m_bCtrlCreated;
};

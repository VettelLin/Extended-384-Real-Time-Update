#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyNewEdit.h"
#include "ExperimentInfo.h"
#include "EditModeAbs.h"
#include "RectManage.h"
#include "resource.h"

class CExpeInfoDlg : public CDialogEx,public CFontSet,public CEditModeAbs,public CRectManage
{
	DECLARE_DYNAMIC(CExpeInfoDlg)

public:
	CExpeInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExpeInfoDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnBnClickedSelectPath();
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateCb();
	virtual	void CreateEd();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitCb();
	virtual void InitEd();
			int InitOneEd(CMyNewEdit* pEdit);
	virtual	void SetCtrlPos();
	virtual	void ShowCtrl();

	virtual	void SetCtrlText();

	virtual	void GetExperiInfo(CExperimentInfo& experiInfo);
	virtual	void SetExperiInfo(CExperimentInfo& experiInfo);

	virtual	void SetCtrlEnableState(BOOL bEnabled);
	virtual	void ClearPage();
	virtual	int RefreshLanguageDisplay();
	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

			CString GetExpName();

public:
	CMyButton m_btSelectDir;

	CMyNewEdit m_edExpeName;
	CMyNewEdit m_edSavePath;
	CMyNewEdit m_edRemark;

private:
	BOOL m_bCtrlCreated;
};

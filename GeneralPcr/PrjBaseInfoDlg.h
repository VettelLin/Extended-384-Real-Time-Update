#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyNewEdit.h"
#include "MyNewCb.h"
#include "InterfaceB.h"
#include "EditModeAbs.h"
#include "ReagentItem.h"
#include "RectManage.h"

class CPrjBaseInfoDlg : public CDialogEx,public CFontSet,public CEditModeAbs
{
	DECLARE_DYNAMIC(CPrjBaseInfoDlg)

public:
	CPrjBaseInfoDlg(BOOL bPrjManage = FALSE,CWnd* pParent = NULL,CInterfaceB* pInterfaceB = NULL);   // standard constructor
	virtual ~CPrjBaseInfoDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam);

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

			void SetCtrlEnableState(BOOL bEnabled);
			void ClearPage();

			int SetPrjBaseInfo(CReagentItem* pReagentItem);
			int GetPrjBaseInfo(CReagentItem* pReagentItem);

			void SetEditState(BOOL bEditable,BOOL bRedraw = FALSE);

			int RefreshLanguageDisplay();

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

public:
	CMyNewCb m_cbPrjType;

	CMyNewEdit m_edPrjName;
	CMyNewEdit m_edReactSystem;
	CMyNewEdit m_edUnit;

private:
	BOOL m_bPrjMange;	//标识是否在项目管理页面使用
	BOOL m_bCtrlCreated;

	CInterfaceB* m_pInterfaceB;

	CRectManage m_rcManage;
};

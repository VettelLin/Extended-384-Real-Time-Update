#pragma once

#include "FontSet.h"
#include "MyLstCtrl.h"
#include "PrjManageBaseInfoDlg.h"
#include "ReagentItem.h"
#include "MyButton.h"
#include "RectManage.h"

// CPrjManageDlg dialog

typedef enum PrjManageSonPage{
	EPMSP_BASE,EPMSP_RSTJUDGE
}EPRJMANAGESONPAGE;

class CGeneralPcrDlg;

class CPrjManageDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CPrjManageDlg)

public:
	CPrjManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrjManageDlg();

// Dialog Data
	enum { IDD = IDD_PRJMANAGEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateLst();
			void InitCtrl();
			void InitLst();
			void SetCtrlPos();
			void ShowCtrl();
			
			void SetCtrlText();

			int SetPrjManageLstInfo();

			void SetReagentItem(CReagentItem& reagentItem,BOOL bSetEditableState = TRUE);
			void GetReagentItem(CReagentItem& reagentItem);

			void SetPrjEditableState(BOOL bEditable,BOOL bRedraw);

	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgChannelDyeNameChanged(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedSwtBtPrjBaseInfoDlg();
	afx_msg void OnBnClickedSwtBtJudgeDlg();

	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedDelete();

	void SetNewPrjBaseInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/);

			void SetPageSelected(EPRJMANAGESONPAGE ePrjManageSonPage);

			int ResetPrjManageLstText();

			int RefreshLanguageDisplay();

protected:
	void InsertOneItemToList(int iIndex, CReagentItem* pItem);

public:
	int	m_iFunBtnHeight;



	CMyLstCtrl m_lstSelPrj;

	CPrjManageBaseInfoDlg m_prjManageBaseInfoDlg;

	CGeneralPcrDlg* m_pMainDlg;

	CMyButton m_swtBtPrjBaseInfoDlg;
	CMyButton m_swtBtRstJudgeDlg;

private:
	BOOL m_bCtrlCreated;
	CMyButton m_btnNew;
	CMyButton m_btnOpen;
	CMyButton m_btnSave;
	CMyButton m_btnDelete;

	int m_iSwitchBtBkH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtBkW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值
	int m_iSwitchBtW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值

	CMyButton* m_pCurSelSwt;
	CWnd* m_pCurSelWnd;
};

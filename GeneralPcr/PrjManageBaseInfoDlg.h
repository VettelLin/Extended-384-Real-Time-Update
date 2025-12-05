
#pragma once

#include "MyButton.h"
#include "MyInsertBt.h"
#include "PrjBaseInfoDlg.h"
#include "MyLstCtrl.h"
#include "MySamPlateDlg.h"
#include "SamBaseInfoEditDlg.h"
#include "MySegDlg.h"
#include "ReagentItem.h"
#include "InterfaceB.h"
#include "RectManage.h"

class CPrjManageBaseInfoDlg : public CDialogEx,public CFontSet,public CInterfaceB
{
	DECLARE_DYNAMIC(CPrjManageBaseInfoDlg)

public:
	CPrjManageBaseInfoDlg (CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrjManageBaseInfoDlg ();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL OnInitDialog();
			void InitSonDlg();
			void InitPrjBaseInfoDlg();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateEd();
			void CreateLst();
			void CreateSegment();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitEd();
			void InitLst();
			void InitSegment();

			void InitLstBaseParam();
			void InitLstMeltParam();
			void InitLstCrossParam();
			void InitLstChanSet();
			void SetCtrlPos();
			void SetLstChanTargetColumnWidth(int iChanNum);

			void ShowCtrl();

			void SetCtrlText();

			void InitInsertBt(CMyInsertBt *pInsertBt);
			void InitOtherBt(CMyButton *pBt);

			int ResetLstChanCross(int iChanCount,CLinkList<CString>& lnkDyeName);
			int ResetLstCrossOneChannelName(int iChanId,CString strDye);	//项目的某个通道染料名称改变了

			int SetPrjInfoBaseInfo(CReagentItem& reagentItem);
			int GetPrjInfoBaseInfo(CReagentItem& reagentItem);
			
			int SetChanTargetInfo(CReagentItem& reagentItem);
			int GetChanTargetInfo(CReagentItem& reagentItem);

			int SetLstAmlAnaBaseParam(CDataAnalysisPara* pAnalysisPara);
			int GetLstAmlAnaBaseParam(CDataAnalysisPara* pAnalysisPara);

			int SetLstMeltParam(CDataAnalysisPara* pAnalysisPara);
			int GetLstMeltParam(CDataAnalysisPara* pAnalysisPara);

			int SetChanCrossParam(double* pCrosstalkPara,int iChanCount,CLinkList<CString>& lnkDyeName);
			int GetChanCrossParam(double* pCrosstalkPara);

	afx_msg void OnBnClickedInsertSegment();
	afx_msg void OnBnClickedInsertStep();
	afx_msg void OnBnClickedEditStep();
	afx_msg void OnBnClickedEditSegment();
	afx_msg void OnBnClickedDelete();

	afx_msg void OnFileMenuSegInsertBefore();
	afx_msg void OnFileMenuSegInsertAfter();
	afx_msg void OnFileMenuSegInsertLast();

	afx_msg void OnFileMenuStepInsertBefore();
	afx_msg void OnFileMenuStepInsertAfter();
	afx_msg void OnFileMenuStepInsertLast();

	afx_msg LRESULT OnMsgPrjTypeChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentLstCellChanged(WPARAM wParam,LPARAM lParam);		

	afx_msg void OnBnClickedAddTarget();
	afx_msg void OnBnClickedDeleteTarget();
	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);

			int SetReagentItem(CReagentItem& reagentItem);
			int GetReagentItem(CReagentItem& reagentItem);

			void SetEditState(BOOL bEditable,BOOL bRedraw = FALSE);

	virtual	void SetNewPrjBaseInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/);

			int SetPrjDyeInfo();

			void ResetLstAmlColumnText();
			void ResetLstMeltColumnText();
			void ResetLstChanSetText();
			void ResetLstCrossText();

			int RefreshLanguageDisplay();
			int IsAmlBaseParamLegal();

protected:
	void SetOneAmlBaseParam(int iRowIndex, tagAnaAmplifyPara* pInputParam);
	void ResetAmlListColumnWidth();
	void ResetAmlListChannelArray();

public:
	CPrjBaseInfoDlg m_prjBaseInfoDlg;

	CMyLstCtrl m_lstChanSet;

	CMyLstCtrl m_lstAmlBaseParam;	//基本参数
	CMyButton m_btnAddTarget; // 增加一个基因参数
	CMyButton m_btnDeleteTarget; // 删除一个基因参数

	CMyLstCtrl m_lstMeltParam;	//熔解曲线参数
	CMyLstCtrl m_lstCrossParam;	//交叉干扰参数


	CMySegDlg m_segDlg;

	CMyInsertBt m_btInsertSegment;
	CMyInsertBt m_btInsertStep;
	CMyButton m_btEditStep;
	CMyButton m_btEditSegment;
	CMyButton m_btDelete;

	CFontSet m_fontNormal;

	int m_iDyeChangeChanId;	//记录染料名称变更时的通道Id
	CString m_strDyeChangeName;	//记录染料名称变更时的染料名称

private:
	BOOL m_bCtrlCreated;
	BOOL m_bEditable;	//标识当前窗口是否可编辑
	CRectManage m_rcManage;
};



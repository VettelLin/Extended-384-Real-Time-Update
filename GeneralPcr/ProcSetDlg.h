#pragma once

#include "PrjBaseInfoDlg.h"
#include "MyButton.h"
#include "MyInsertBt.h"
#include "MyLstCtrl.h"
#include "MySegDlg.h"
#include "SamBaseInfoEditDlg.h"
#include "MySamPlateDlg.h"
#include "TubeChanEditInfo.h"
#include "ExperimentData.h"
#include "InterfaceA.h"
#include "LegendInfo.h"
#include "EditModeAbs.h"
#include "RectManage.h"
#include "InterfaceB.h"

class CMiddleDlg;

class CProcSetDlg : public CDialogEx,public CInterfaceA,public CFontSet,public CEditModeAbs,public CInterfaceB,public CRectManage
{
	DECLARE_DYNAMIC(CProcSetDlg)

public:
	CProcSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcSetDlg();

// Dialog Data
	enum { IDD = IDD_PROCSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL OnInitDialog();

			void InitDlg();
			void InitSegLegend();

			void InitSonDlg();
			void InitSamBaseInfoEditDlg();

			void CreateCtrl();
			void CreateBt();
			void CreateLst();
			void CreateSamPlateDlg();
			void CreateSeg();
			void InitCtrl();
			void InitBt();
			void InitInsertBt(CMyInsertBt *pInsertBt);
			void InitOtherBt(CMyButton *pBt);
			void InitLst();
			void InitLstSelectPrj();
			void InitSamPlateDlg();
			void InitSeg();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			int SetRowColNum(int iRowNum,int iColNum);

	virtual	void SetPlateSelectedSiteSamInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/);
	virtual	int SetPlateSelectedSiteSamInfo(CTubeChanEditInfo tubeChanEditInfo);

			int ReSetTubeInfo(vector<int> &vecPlateSelectedSite,vector<CTubeInfo> &vecTubeInfo);	//样本登记页面样本信息改变后，同步到选择项目页面
			int ResetSamSetDlgTubeInfo();

	afx_msg void OnBnClickedInsertSegment();
	afx_msg void OnBnClickedInsertStep();
	afx_msg void OnBnClickedEditStep();
	afx_msg void OnBnClickedEditSegment();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedAddCycle();

	afx_msg void OnFileMenuSegInsertBefore();
	afx_msg void OnFileMenuSegInsertAfter();
	afx_msg void OnFileMenuSegInsertLast();

	afx_msg void OnFileMenuStepInsertBefore();
	afx_msg void OnFileMenuStepInsertAfter();
	afx_msg void OnFileMenuStepInsertLast();

	afx_msg LRESULT OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgSamplateDeleteKeyPressed(WPARAM wParam, LPARAM lParam);

			int SimulatePlateSiteSelectChanged();

			void ClearPage(BOOL bRedraw = TRUE);
			void ClearExperiment(BOOL bRedraw = TRUE);
			int FileOpened(CExperimentData* pExpData = NULL);

			int SavePrj(int iIndex,BOOL bNew);	//项目已被保存，更新到实验页面
			int DeletePrj(int iIndex);	//删除一个项目

			int SetSelectPrjLstInfo();	//初始化项目信息列表框，默认模式
			int SetSelectPrjLstInfoIVDNoProject();	//初始化项目信息列表框，IVD模式，使用模板文件，不使用项目

			int SetProjectInfo(CReagentItem& reagentItem);	//设置选中的项目信息到实验数据中

			int HandleProjectLstRowSelectChanged();	//处理项目信息列表框行选中状态改变
			int HandleTemplateFileLstRowSelectChanged();	//处理模板文件列表框行选中状态改变

			void SetTemplateListSelectedRow(int iIndex);	//设置模板文件列表选中行
			int NewTemplateAdded(int iIndex,BOOL bRedraw = TRUE);	//打开了新的模板文件，添加到模板文件列表框中

			int SetGradientFuncShieldState(BOOL bData);
			int SetTemperatureRateShowState(BOOL bData);

			int GetAllSampleType(vector<eSampleType>& vecSamType);

			void ResetLstTitle();

			int RefreshLanguageDisplay();

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

	virtual void SetNewPrjBaseInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/);

public:
	CMyLstCtrl m_lstSelPrj;
	CPrjBaseInfoDlg m_prjBaseInfoDlg;
	CMySegDlg m_segDlg; 
	CMySamPalteDlg m_samPlateDlg;
	CSamBaseInfoEditDlg m_samBaseInfoEditDlg;

	CMiddleDlg* m_pParent;

	CMyInsertBt m_btInsertSegment;
	CMyInsertBt m_btInsertStep;
	CMyButton m_btEditStep;
	CMyButton m_btEditSegment;
	CMyButton m_btDelete;
	CMyButton m_btAddCycle;

	vector<int> m_vecPlateSelectedSite;	//记录孔板选中孔位下标
	
	CLegendInfo m_segLgdInfo;	//样本信息控件图例

private:
	BOOL m_bCtrlCreated;
};




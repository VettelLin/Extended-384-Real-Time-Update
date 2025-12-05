
#pragma once

#include "FontSet.h"
#include "MyButton.h"
#include "MySamPlateDlg.h"
#include "LegendInfo.h"
#include "RectManage.h"
#include "ExperimentData.h"
#include "EditModeAbs.h"
#include "Resource.h"
#include "MySegDlg.h"

class CPs96SimpleDlg;

class CLoadSamDlg : public CDialogEx,public CFontSet,public CEditModeAbs
{
	DECLARE_DYNAMIC(CLoadSamDlg)

public:
	CLoadSamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoadSamDlg();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnBnClickedLoadId();
	virtual BOOL OnInitDialog();
			
			void InitSegLegend();

			void CreateCtrl();
			void CreateBt();
			void CreateCkbt();
			void CreateSt();
			void CreateEd();
			void CreateLst();
			void CreateSamPlateDlg();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitOneBt(CMyButton* pBt);
			void InitCkbt();
			void InitSt();
			void InitEd();
			void InitLst();
			void InitSamPlateDlg();
			void SetCtrlPos();
			void ShowCtrl();
			void InitSegment();

			void SetCtrlText();

			int RefreshExpEmbody();
			void ClearExpEmbody();

			int ImportId();

			int SetEditMode(EEXPEDITMODE eExpEditMode);

			void SetExpDataPtr(CExperimentData* pExpData);
			int CurrentSegmentStepChangedOnRun(int iCycle,int iSeg,int iStep);

public:
	CMyButton m_btLoad;

	CMySamPalteDlg m_samPlateDlg;
	CLegendInfo m_segLgdInfo;	//样本信息控件图例

	CMyButton m_swtBtSegMonitor;
	CMySegDlg m_segDlg;


	CExperimentData* m_pExpData;
	CPs96SimpleDlg* m_pParent;

private:
	BOOL m_bCtrlCreated;
	CRectManage m_rcManage;
};




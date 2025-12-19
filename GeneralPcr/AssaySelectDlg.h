
#pragma once

#include "FontSet.h"
#include "MyButton.h"
#include "MyLstCtrl.h"
#include "RectManage.h"
#include "ExperimentData.h"
#include "EditModeAbs.h"
#include "Resource.h"
#include "MyNewCb.h"

class CPs96SimpleDlg;

class CAssaySelDlg : public CDialogEx,public CFontSet,public CEditModeAbs
{
	DECLARE_DYNAMIC(CAssaySelDlg)

public:
	CAssaySelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAssaySelDlg();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnBnClickedLoadProc();
	afx_msg void OnBnClickedAddTemplate();
	afx_msg void OnBnClickedDeleteTemplate();
	afx_msg void OnBnClickedExportIdTemplate();
	afx_msg void OnBnClickedExportResult();
	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedEditUpThreshold();

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateEd();
			void CreateCb();
			void CreateLst();

			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitEd();
			void InitCb();
			void InitLst();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			int SetEditMode(EEXPEDITMODE eExpEditMode);

			int SetLstAssays();

			int HandleTemplateFileLstRowSelectChanged();

			int RefreshExpEmbody();
			void ClearExpEmbody();

			void SetExpDataPtr(CExperimentData* pExpData);

			void SetTemplateListSelectedRow(int iIndex);
			int NewTemplateAdded(int iIndex,BOOL bRedraw = TRUE);
			void DeleteTemplateListRow(int iIndex);
			void InitOneBt(CMyButton* pBt);

			void ReshowReportTemplate();
			void ReorderTemplateList(int iIndex);

public:
	CMyButton m_btLoadProc;
	CMyButton m_btAddTem;
	CMyButton m_btDelTem;
	CMyButton m_btExportIdTemplate;
	CMyButton m_btExportResult;

	CMyLstCtrl m_lstAssaySel;

	CExperimentData* m_pExpData;
	CPs96SimpleDlg* m_pParent;

	CMyNewCb	m_cbReportTemplate;
	CMyNewEdit	m_edRemark;	

	//CMyNewCb	m_cbUpThreshold;
	//CMyNewEdit	m_edUpThreshold;	
	CMyButton	m_btnEditThreshold;

private:
	BOOL m_bCtrlCreated;
	CRectManage m_rcManage;
};




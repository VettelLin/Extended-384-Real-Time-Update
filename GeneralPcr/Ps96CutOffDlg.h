
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "MyLstCtrl.h"
#include "ReportTemplateData.h"
#include "DataAnalysisPara.h"

class CPs96CutOffDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CPs96CutOffDlg)

public:
	CPs96CutOffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPs96CutOffDlg();

	enum { IDD = IDD_POPUPDLG};
	
public:
	void SetAnalysisParam(CDataAnalysisPara* pAnalysis) { m_pAnalysisParam = pAnalysis; }
	BOOL GetBeChanged() { return m_bChanged; }

protected:
	CDataAnalysisPara* m_pAnalysisParam;
	BOOL m_bChanged;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateCkbt();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitEd();
			void InitCkbt();
			void InitOneCkbt(CMyCheckButton* pCkbt);
			void InitLst();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);
			void SetListCutOff();

public:
	CMyButton m_btnSave;
	CMyButton m_btnCancel;
	CMyLstCtrl m_listCutOff;

private:
	int m_iAngleRadius;	//圆角半径
	int m_iFrameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCapHeight;	//标题栏高度
};





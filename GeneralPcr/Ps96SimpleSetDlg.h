
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "MyLstCtrl.h"
#include "ReportTemplateData.h"

class CPs96SimpleSetDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CPs96SimpleSetDlg)

public:
	CPs96SimpleSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPs96SimpleSetDlg();

	enum { IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
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
			int SetListTemplate();

	afx_msg void OnBnClickedCkbtExportRstHori();
	afx_msg void OnBnClickedCkbtExportRstVert();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAddTemplate();
	afx_msg void OnBnClickedEditTemplate();
	afx_msg void OnBnClickedDeleteTemplate();
	afx_msg void OnBnClickedImportTemplate();
	afx_msg void OnBnClickedExportTemplate();

			BOOL SetRoundRect(int x = 5,int y = 5);

			void InitEditCtrlParam(CMyNewEdit* pEdit);



public:
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyCheckButton m_ckbtRstHori;	
	CMyCheckButton m_ckbtRstVert;


	CMyLstCtrl m_lstReportTemplate;
	CMyButton m_btNewTemplate;
	CMyButton m_btEditTemplate;
	CMyButton m_btDeleteTemplate;
	CMyButton m_btImportTemplate;
	CMyButton m_btExportTemplate;

private:
	int m_iAngleRadius;	//圆角半径
	int m_iFrameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCapHeight;	//标题栏高度

	CReportTemplateData* m_pBackupData;
};





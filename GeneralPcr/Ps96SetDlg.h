
#pragma once

#include "FontSet.h"
#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyNewCb.h"
#include "MyCheckButton.h"
#include "RectManage.h"
#include "Resource.h"

class CPs96SetDlg : public CDialogEx,public CFontSet,public CRectManage
{
	DECLARE_DYNAMIC(CPs96SetDlg)

public:
	CPs96SetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPs96SetDlg();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateCkbt();
			void CreateSt();
	virtual void CreateEd();
			void CreateCb();
			void CreateLst();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitCkbt();
			void InitSt();
	virtual void InitEd();
			int InitOneEd(CMyNewEdit* pEd);
			void InitCb();
			void InitLst();
	virtual void SetCtrlPos();
	virtual void ResetCtrlPos();
	virtual void ShowCtrl();

	virtual void SetCtrlText();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnBnClickedBtAnaSet();
	afx_msg void OnBnClickedBtSaveLidTem();
	afx_msg void OnBnClickedBtSaveLidPreheatTem();
	afx_msg void OnBnClickedBtReadLidTem();
	afx_msg void OnBnClickedBtReadLidPreheatTem();

	afx_msg void OnBnClickedCkbtExportSamHori();
	afx_msg void OnBnClickedCkbtExportSamVert();
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam);

			int SaveLidTem();
			int ReadLidTem();
			int SaveLidPreheatTem();
			int ReadLidPreheatTem();

	virtual int RefreshLanguageDisplay();

			int LanguageChanged();

public:
	CMyNewEdit m_edLidTem;	//热盖工作温度
	CMyNewEdit m_edLidTemInit;	//仪器开机热盖预热温度

	CMyNewCb m_cbLangSel;

	CMyCheckButton m_ckbtExportSamHori;	//导出样本信息时横向导出
	CMyCheckButton m_ckbtExportSamVert;	//导出样本信息时纵向导出

	CMyButton m_btSaveLidTem;
	CMyButton m_btReadLidTem;
	CMyButton m_btSaveLidPreheatTem;
	CMyButton m_btReadLidPreheatTem;
	CMyButton m_btAnaSet;

private:
	BOOL m_bCtrlCreated;

	BOOL GetShowPreheadLid();
};

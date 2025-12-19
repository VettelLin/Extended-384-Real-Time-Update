#pragma once

#include "MyButton.h"
#include "MyCheckButton.h"
#include "LinkList.h"
#include "FontSet.h"
#include "Resource.h"
#include "RectManage.h"

class CGeneralPcrDlg;

class CFuncBtDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CFuncBtDlg)

public:
	CFuncBtDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFuncBtDlg();

// Dialog Data
	enum { IDD = IDD_MIDDLEDLG };

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
			void InitCtrl();
			void InitBt();
			void SetCtrlPos();
			void SetCtrlPosModeExpProcSet();
			void SetCtrlPosModeExpSamSet();
			void SetCtrlPosModeExpDataAnalyse();

			void SetChanBtPos();

			void ShowCtrl();
			void SetChanBtShowState(BOOL bShow);

			void SetChannelBtNum(int iChanBtNum,vector<CString>& vecChanDyeName,BOOL bShowChanBt = TRUE);	//设置通道切换按钮个数，实时生效
			int SetChanBtSelectState(vector<int> vecSelState);

			int SetOneChannelBtText(int iChanId,CString strText);

			void SetCtrlText();

			void ResetRectManage();

	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSaveAs();
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedAnalyse();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedImport();

	afx_msg void OnBnClickedLoadOriginalAmpData();
	afx_msg void OnBnClickedLisFunc();

	afx_msg void OnExportExperimentData();
	afx_msg void OnExportCurvePicture();
	afx_msg void OnExportTemperatureData();

	afx_msg void OnSaveAsExperimentFile();
	afx_msg void OnSaveAsTemplateFile();

	afx_msg void OnBnClickedAnaParamSet();

	afx_msg void OnAnaParamMenuSetCross();
	afx_msg void OnAnaParamMenuSetFilter();
	afx_msg void OnSetBaseSet();
	afx_msg void OnSetMotChannelAdjust();
	afx_msg void OnSetMeltAnaParam();

	afx_msg void OnChanBtClicked(UINT nID);

			void SetModeExpBaseSet();
			void SetModeExpProcSet();
			void SetModeExpSamSet();
			void SetModeExpRunMonitor();
			void SetModeExpDataAnalyse();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

			int RefreshLanguageDisplay();

public:
	CMyButton m_btClearList;
	CMyButton m_btNew;
	CMyButton m_btOpen;
	CMyButton m_btDelete;
	CMyButton m_btSave;
	CMyButton m_btSaveAs;
	CMyButton m_btAnalyse;
	CMyButton m_btExport;
	CMyButton m_btLis;	//LIS相关功能

	CMyButton m_btPrint;
	CMyCheckButton m_ckbtVerify;
	CMyButton m_btSift;
	CMyButton m_btSetBadHole;
	
	CMyButton m_btUpLoadLisData;
	CMyButton m_btImport;	//导入

	CMyButton m_btReadExeclData; //从Excel中加载原始扩增数据

	CMyButton m_btAnaParamSet;	//分析参数设置，包括滤波参数、串扰设置

	CMyButton m_btReadEeprom;
	CMyButton m_btWriteEeprom;

	static CGeneralPcrDlg*	m_pMainWnd;

	CLinkList<CMyCheckButton*> m_lnkChanBtPtr;

private:
	BOOL m_bCtrlCreated;

	int m_iChanBtW;
	int m_ichanBtH;
	int m_iChanBtInterval;	//通道切换按钮间隔

	BOOL m_bChanShow;	//标识通道切换按钮的显示状态

	CRectManage m_rcManage;
};






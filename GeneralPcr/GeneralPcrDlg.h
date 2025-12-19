#pragma once

#include "resource.h"
#include "MyButton.h"
#include "MyStatic.h"
#include "MyProgressBar.h"
#include "WorkDlg.h"
#include "SetDlg.h"
#include "PrjManageDlg.h"
#include "RectManage.h"
#include "ExperStateInfo.h"
#include "FontSet.h"

typedef enum EMainFuncType
{
	EMFT_WORK = 1,EMFT_SET,EMFT_PRJMANAGE
}EMAINFUNCTYPE;

// CGeneralPcrDlg dialog
class CGeneralPcrDlg : public CDialogEx,public CFontSet
{
// Construction
public:
	CGeneralPcrDlg(CWnd* pParent = NULL);	// standard constructor
	~CGeneralPcrDlg();
// Dialog Data
	enum { IDD = IDD_GENERALPCR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nSide,LPRECT pRect);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	afx_msg void OnBnClickedSwtBtMainPage();
	afx_msg void OnBnClickedSwtBtPrjManage();
	afx_msg void OnBnClickedSwtBtSet();

	afx_msg void OnBnClickedSwtBtWork();
	afx_msg void OnBnClickedBtMinimum();
	afx_msg void OnBnClickedBtZoom();
	afx_msg void OnBnClickedBtClose();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void SetCtrlPos();
			void ShowCtrl();

			void InitSonDlg();
			void InitWorkDlg();
			void InitSetDlg();
			void InitPrjManageDlg();

			void SetCtrlText(BOOL bOnlyConnectStatus=FALSE);

			void SetPageSelected(EMAINFUNCTYPE eMainFuncType);

			void New();	//新建功能
			void Delete();	//删除功能
			void Open();	//打开功能

			int SetLidTem(int iLidTem);	//更改热盖温度后，更新到已经打开的实验文件
			int ReadLidTem(double& dLidTem);
			int SetLidPreheatTem(int iLidTem);
			int ReadLidPreheatTem(float& fLidTem);

			int RefreshLanguageDisplay();
			int SetDefaultGeneName();

public:
	CMyButton m_btMainPage;
	CMyButton m_btWork;
	CMyButton m_btPrjManage;
	CMyButton m_btSet;
	
	CMyButton m_btMinimum;
	CMyButton m_btZoom;
	CMyButton m_btClose;

	CMyStatic m_stUserName;
	CMyStatic m_stLisConState;
	CMyStatic m_stPrinterConState;

	CWorkDlg m_workDlg;
	CSetDlg m_setDlg;
	CPrjManageDlg m_prjManageDlg;

private:
	int m_iCapH;	//标题栏高度

	CMyButton* m_pCurSelSwt;
	CWnd* m_pCurSelWnd;

	CRectManage m_rcManage;

	CDC* m_pMemDc;
	HBITMAP m_hBitmap;

	BOOL m_bPreZeroSize;	//记录上一个窗口大小是否是0
	BOOL m_bZeroToNewSize;	//标识从零开始到正常大小

	UINT	m_nRunTimer;
	int		m_iTimerCount;
};

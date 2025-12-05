#pragma once

#include "ExperimentData.h"
#include "MyButton.h"

//历史实验记录显示模式：图形模式、表格模式
typedef enum HistoryExpDisplayMode
{
	EHEDM_FIGURE,EHEDM_LIST
}EHISTORYEXPDISPALYMODE;

class CMainPageDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMainPageDlg)

public:
	CMainPageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainPageDlg();

// Dialog Data
	enum { IDD = IDD_SAMSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBtNew();
	afx_msg void OnBnClickedBtOpen();

	virtual BOOL OnInitDialog();

	void InitDlg();
	void InitSonDlg();

	void CreateCtrl();
	void CreateBt();
	void CreateLst();
	void CreateSamPlateDlg();
	void InitCtrl();
	void InitBt();
	void InitLst();
	void InitSamPlateDlg();
	void SetCtrlPos();
	void ShowCtrl();

	void SetPageSelected(EHISTORYEXPDISPALYMODE eExpDisplayMode);

public:
	CMyButton m_btNew;
	CMyButton m_btOpen;

private:
	BOOL m_bCtrlCreated;

	CMyButton* m_pCurSelSwt;
	CWnd* m_pCurSelWnd;

	int m_iSwitchBtBkH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtBkW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值
	int m_iSwitchBtW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值
};















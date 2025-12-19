
#pragma once

#include "MyButton.h"
#include "MyLstCtrl.h"
#include "ExperimentData.h"

class CCrossSetDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CCrossSetDlg)

public:
	CCrossSetDlg(CExperimentData* pExpData,CWnd* pParent = NULL);   // standard constructor
	virtual ~CCrossSetDlg();

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
			void CreateCkBt();
			void CreateSt();
			void CreateEd();
			void CreateCb();
			void CreateLst();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitCkBt();
			void InitSt();
			void InitEd();
			void InitCb();
			void InitLst();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);

			int SetCrossSetParam();
			int GetCrossSetParam();

			int SetColumnRowNum(int iChanNum,BOOL bRedraw = FALSE);

public:
	CMyLstCtrl m_lstCrossSet;

	CMyButton m_btSave;
	CMyButton m_btCancel;

	CExperimentData* m_pExpData;
	BOOL m_bChanged;	//标识串扰系数是否改变

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCaptionHeight;	//标题栏高度
};

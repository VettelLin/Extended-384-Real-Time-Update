
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "RectManage.h"
#include "MyLstCtrl.h"
#include "resource.h"

class CSystemSetDlg : public CDialogEx,public CFontSet,public CRectManage
{
	DECLARE_DYNAMIC(CSystemSetDlg)

public:
	CSystemSetDlg(CWnd* pParent = NULL);   // standard constructor
	~CSystemSetDlg();

	enum {IDD = IDD_POPUPDLG};

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
			void CreateCkbt();
			void CreateSt();
			void CreateEd();
			void CreateLst();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitCkbt();
			void InitSt();
			void InitEd();
			void InitLst();
			void InitLstAmlAnaParam();
			void InitCrossSetAnaParam();
			void InitLstMeltParam();
			void InitLstMeltCrossSet();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedDefaultSet();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);

			void InitEditCtrlParam(CMyNewEdit* pEdit);
			void InitCheckBt(CMyCheckButton* pCkbt);

			int SetLstAmlAnaBaseParam();
			int GetLstAmlAnaBaseParam();

			int SetLstMeltParam();
			int GetLstMeltParam();

			int SetFilterParam();
			int GetFilterParam();

			int SetCrossSetParam();
			int GetCrossSetParam();
			int SetCrossSetColumnRowNum(int iChanNum,BOOL bRedraw = FALSE);

			int SetMeltCrossSetParam();
			int GetMeltCrossSetParam();
			int SetMeltCrossSetColumnRowNum(int iChanNum,BOOL bRedraw = FALSE);

public:
	CMyButton m_btDefSet;
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyCheckButton m_ckbtMedian;
	CMyCheckButton m_ckbtFilter1;
	CMyCheckButton m_ckbtFilter2;
	CMyCheckButton m_ckbtFilter3;
	CMyCheckButton m_ckbtFilter4;
	CMyCheckButton m_ckbtFilter5;

	CMyNewEdit m_edMedianWnd;
	CMyNewEdit m_edMidianTimes;

	CMyNewEdit m_edFilter1M;
	CMyNewEdit m_edFilter1N;
	CMyNewEdit m_edFilter1Times;

	CMyNewEdit m_edFilter2M;
	CMyNewEdit m_edFilter2N;
	CMyNewEdit m_edFilter2Times;

	CMyNewEdit m_edFilter3M;
	CMyNewEdit m_edFilter3N;
	CMyNewEdit m_edFilter3Times;

	CMyNewEdit m_edFilter4M;
	CMyNewEdit m_edFilter4N;
	CMyNewEdit m_edFilter4Times;

	CMyNewEdit m_edFilter5M;
	CMyNewEdit m_edFilter5N;
	CMyNewEdit m_edFilter5Times;

	HWND m_hWndParent;

	BOOL m_bChanged;	//标识最大最小值信息是否改变

	CMyLstCtrl m_lstAmlParam;
	CMyLstCtrl m_lstCrossSet;
	CMyLstCtrl m_lstMeltParam;
	CMyLstCtrl m_lstMeltCrossSet;

private:
	int m_iAngleRadius;	//圆角半径
	int m_iFrameW;	//边框宽度

	int m_iWndW;	//窗口宽度
	int m_iWndH;	//窗口高度
	int m_iCapH;	//标题栏高度
};

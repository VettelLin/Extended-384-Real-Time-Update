
#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyNewEdit.h"
#include "MyNewCb.h"
#include "MyCheckButton.h"
#include "MyStepEditInfo.h"
#include "resource.h"

//编辑非熔曲步骤
typedef enum InstrumentType{
	EIT_GENERAL,EIT_PS96
}EINSTRUMENTTYPE;

class CMyStepEditDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMyStepEditDlg)

public:
	CMyStepEditDlg(CMyStepEditInfo stepEditInfo,CWnd* pParent = NULL,BOOL bShieldGradientSet = FALSE,BOOL bShowTemRate = TRUE);   // standard constructor
	~CMyStepEditDlg();

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
			void CreateCkBt();
			void CreateSt();
			void CreateEd();
			void CreateCb();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitCkBt();
			void InitSt();
			void InitEd();
			void InitCb();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedCollect();
	afx_msg void OnBnClickedNotCollect();
	afx_msg void OnBnClickedCollectAll();
	afx_msg void OnBnClickedSelectGradient();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);

			void SetInstrumentType(EINSTRUMENTTYPE eInstrType);	//设置仪器类型，domodal调用前使用

			BOOL EraseBkgndGeneral(CDC* pDC);
			BOOL EraseBkgndPS96(CDC* pDC);

			void SetCtrlPosGeneral();
			void SetCtrlPosPS96();

			void ShowCtrlGeneral();
			void ShowCtrlPS96();

public:
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyNewEdit m_edSegName;
	CMyNewEdit m_edStepTag;
	CMyNewEdit m_edAimTem;
	CMyNewEdit m_edGradientMaxTem;
	CMyNewEdit m_edHoldSec;

	CMyNewCb m_cbMeltTemInterval;
	CMyNewCb m_cbTemRate;	//升/降温速率

	CMyCheckButton m_ckbtCollect;
	CMyCheckButton m_ckbtNotCollect;
	CMyCheckButton m_ckbtCollectAll;

	CMyCheckButton m_ckbtSelectGradient;

	HWND m_hWndParent;

	CMyStepEditInfo m_stepEditInfo;

	EINSTRUMENTTYPE m_eInstruType;	//标识仪器类型，不同仪器类型显示的编辑内容不一样

protected:
	BOOL m_bShieldGradientSet;	//是否屏蔽梯度设置项
	BOOL m_bShowTemRate;	//是否显示升降温速率

	int m_iVertDiff;	//垂直方向上两行信息垂直间隔像素数

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCapHeight;	//标题栏高度
};



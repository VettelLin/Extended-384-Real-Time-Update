
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "AxisInfo.h"
#include "resource.h"

class CAxisZoomDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CAxisZoomDlg)

public:
	CAxisZoomDlg(CWnd* pParent = NULL,BOOL bHaveLogFunc = FALSE,BOOL bLogMode = FALSE);   // standard constructor
	virtual ~CAxisZoomDlg();

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
			void CreateEd();
			void CreateCkbt();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitEd();
			void InitCkbt();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedDefaultSet();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			void SetAxisInfo(CAxisInfo axisInfo,BOOL bRedraw);
			BOOL SetRoundRect(int x = 5,int y = 5);

			void InitEditCtrlParam(CMyNewEdit* pEdit);

			BOOL GetNewLogMode();

			void SetUseDefaultFuncState(BOOL bUseState);
			BOOL GetUseDefaultFuncState();

public:
	CMyButton m_btDefSet;
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyNewEdit m_edXMin;
	CMyNewEdit m_edXMax;
	CMyNewEdit m_edYMin;
	CMyNewEdit m_edYMax;

	CMyCheckButton m_ckbtLogMode;	//曲线控件使用对数模式

	HWND m_hWndParent;

	CAxisInfo m_axisInfo;
	CAxisInfo m_newAxisInfo;
	CAxisInfo m_defLinearAxisInfo;	//默认的坐标信息，线性形式

	BOOL m_bChanged;	//标识最大最小值信息是否改变
	BOOL m_bLogModeChanged;	//对数坐标模式是否改变

private:
	int m_iCrvId;	//记录是哪个曲线绘制控件
	int m_iAngleRadius;	//圆角半径
	int m_iFrameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCapHeight;	//标题栏高度

	BOOL m_bHaveLogFunc;	//是否显示对数坐标选项
	BOOL m_bLogMode;	//是否选中对数坐标模式

	BOOL m_bNewLogMode;	//对数坐标模式编辑后的状态

	BOOL m_bUseDefFunc;	//是否使用默认（设置）功能
};





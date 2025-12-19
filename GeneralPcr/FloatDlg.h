
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "LineInfo.h"
#include "Resource.h"

#define COLORROWNUM	5
#define COLORCOLUMNNUM	6

#define WM_SELECTNEWCOLOR	WM_USER + 6033	//通知列表框颜色块改变了，占位

class CFloatDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CFloatDlg)

public:
	CFloatDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFloatDlg();

	enum { IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);

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

			BOOL SetRoundRect(int x = 5,int y = 5);
	virtual int DrawTipText(CDC* pDc);

			void SetHoriDotNum(int iData);
			int GetHoriDotNum();

			void SetVertDotNum(int iData);
			int GetVertDotNum();

public:
	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCaptionHeight;	//标题栏高度
	
	CMyPoint m_ptDotInfo;	//点信息
	CLineInfo m_lineInfo;	//点所在线条的信息

protected:
	int m_iHoriDotNum;	//X轴小数点位数
	int m_iVertDotNum;	//Y轴小数点位数

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	CWnd* m_pParent;
};

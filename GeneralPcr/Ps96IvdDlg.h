#pragma once

#include "resource.h"
#include "MyButton.h"
#include "MyStatic.h"
#include "MyProgressBar.h"
#include "Ps96WorkDlg.h"
#include "RectManage.h"
#include "FontSet.h"

// CPs96IvdDlg dialog
class CPs96IvdDlg : public CDialogEx,public CFontSet
{
// Construction
public:
	CPs96IvdDlg(CWnd* pParent = NULL);	// standard constructor
	~CPs96IvdDlg();
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
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags,CPoint point);

	afx_msg void OnBnClickedBtMinimum();
	afx_msg void OnBnClickedBtZoom();
	afx_msg void OnBnClickedBtClose();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
		
			void InitDlg();
			void InitSonDlg();
			void InitWorkDlg();

			void CreateCtrl();
			void CreateProgressBar();
			void InitCtrl();
			void SetCtrlPos();
			void ShowCtrl();

			int DlgSizeChanged();

public:
	CMyButton m_btMinimum;
	CMyButton m_btZoom;
	CMyButton m_btClose;

	CPs96WorkDlg m_workDlg;

private:
	BOOL m_bCtrlCreated;

	int m_iCapH;	//标题栏高度
	int m_iFrameW;	//边框宽度

	COLORREF m_clrFrame;	//边框颜色

	CMyButton* m_pCurSelSwt;
	CWnd* m_pCurSelWnd;

	CRectManage m_rcManage;

	CDC* m_pMemDc;
	HBITMAP m_hBitmap;

	BOOL m_bPreZeroSize;	//记录上一个窗口大小是否是0
	BOOL m_bZeroToNewSize;	//标识从零开始到正常大小

	double m_dSizeRatio;	//记录主窗口的缩放比例

	BOOL m_bZooming;	//标识是否处于窗口大小调整过程

	UINT	m_nRunTimer;
	int		m_iTimerCount;

public:
	afx_msg void OnDestroy();
};


#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "Resource.h"

#include <GdiPlus.h>
using namespace Gdiplus;

#define COLORROWNUM	5
#define COLORCOLUMNNUM	10
#define DEFAULTCOLORNUM	6

#define WM_SELECTNEWCOLOR	WM_USER + 6033	//通知列表框颜色块改变了，占位

class CColorSelectDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CColorSelectDlg)

public:
	CColorSelectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColorSelectDlg();

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

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);

			void InitColorInfo();

public:
	CMyButton m_btSave;
	CMyButton m_btCancel;
	CMyNewEdit m_edRgbValue;

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCaptionHeight;	//标题栏高度

	int m_iCellRowId;
	int m_iCellColId;

	Color m_aryClr[COLORROWNUM][COLORCOLUMNNUM];
	Color m_aryThemeClr[COLORCOLUMNNUM];
	Color m_aryStandardClr[COLORCOLUMNNUM];
	Color m_aryDefClr[DEFAULTCOLORNUM];

	CRect m_rcClrTheme;	//记录主题标题颜色块的区域
	CRect m_rcClrBody;	//记录主题body颜色块的区域
	CRect m_rcClrStandard;	//记录主题标准颜色块的区域
	CRect m_rcClrDefault;	//记录主题默认颜色块的区域

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	int m_iHoriInterval;	//颜色块水平间距
	int m_iVertInterval;	//颜色块垂直间距（主题颜色块）

	int m_iStartX;	//水平起始位置，第一列文字或者第一个颜色块
	int m_iStartY;	//垂直起始位置，第一行文字或者第一个颜色块

	int m_iClrBlockW;	//颜色块宽度
	int m_iClrBlockH;	//颜色块高度

	int m_iThemeStartX;	//临时保存数据
	int m_iThemeStartY;	//临时保存数据
	int m_iBodyStartX;	//临时保存数据
	int m_iBodyStartY;	//临时保存数据
	int m_iStandardStartX;	//临时保存数据
	int m_iStandardStartY;	//临时保存数据
	int m_iDefStartX;	//临时保存数据
	int m_iDefStartY;	//临时保存数据

	CWnd* m_pParent;
};

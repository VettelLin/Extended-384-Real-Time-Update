#pragma once

#include "FontSet.h"
#include <GdiPlus.h>

using namespace Gdiplus;

// CMyPageSwitchBt

//按钮状态参数
#define CTRL_BUTTON_NOFOCUS 1
#define CTRL_BUTTON_FOCUS 2
#define CTRL_BUTTON_SELECTED 3
#define CTRL_BUTTON_DISABLE 4

class CMyPageSwitchBt : public CButton,public CFontSet
{
	DECLARE_DYNAMIC(CMyPageSwitchBt)

public:
	CMyPageSwitchBt(BOOL bAutoDynamic = FALSE,BOOL bUseFourthState = TRUE);
	virtual ~CMyPageSwitchBt();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	int	   m_srcWidth;
	int    m_srcHeight;
	int    m_ctrlState;

private:
	BOOL m_bTracking;

	BOOL m_bHasBkPic;
	BOOL m_bHasPathBkPic;
	BOOL m_bHasPngBkPic;
	int  m_sonPicNums;
	int  m_iconW;

	int m_iconLeftIndent;
	int m_iconRightIndent;
	int m_rightEnd;

	COLORREF m_bkClrNormal;
	COLORREF m_bkClrFocused;
	COLORREF m_bkClrClicked;
	COLORREF m_bkClrDisabled;
	COLORREF m_txClrNormal;
	COLORREF m_txClrFocused;
	COLORREF m_txClrClicked;
	COLORREF m_txClrDisabled;
	COLORREF m_edgeClrNormal;
	COLORREF m_edgeClrFocused;
	COLORREF m_edgeClrClicked;
	COLORREF m_edgeClrDisabled; 

	BOOL	m_bDrawFrame;	//是否绘制边框

	CString m_strPngPicPath;

	BOOL m_bInMiddle;	//是否居中
	BOOL m_bAmoySwtStyle;	//是否是艾德页面切换按钮
	COLORREF m_AmoySwtBkClr;	//艾德页面切换按钮背景色

	BOOL m_bGradientBk;	//是否是渐变色背景
	Color m_glrStart;
	Color m_glrEnd;
	LinearGradientMode m_gradientMode;

	BOOL m_bFstPaint;
	CDC* m_pMemDc;
	BOOL m_bAutoDynamic;	//20200903添加，适用于大小固定、内容可变或者大小随着父窗口同比例变化的情况，第一次绘制时保存背景信息到m_pMemDc中，为了兼容之前的CStatic控件，保留m_bDynamic选项
	int  m_fstPaintWidth;
	int  m_fstPaintHeight;
	HBITMAP m_hBitmap;

	UINT m_txFormat;

	BOOL m_bUseFourthState;	//禁用状态是否使用背景图片中的第四种状态，为否的话，禁用状态使用背景图片中的第三种状态

	BOOL m_bCkbtFunc;	//是否是“运行、停止”使用同一张背景图（背景图包含两个小图）
	BOOL m_bAddPageIcon;	//是否包含“+”图标

	int m_curStateId;	//从0开始

	int m_iCloseIconW;	//标识关闭按钮的宽度
	int m_iCloseIconH;	//标识关闭按钮的高度
	int m_iCloseIconRightIndent;	//标识关闭按钮距离按钮右侧的距离
	CRect m_rcCloseBt;	//记录关闭按钮区域

	BOOL m_bMouseInColseBtRect;	//记录鼠标是否在关闭按钮内

public:
	BOOL m_bBtPageRunning;	//标识按钮标识的页面是否正在运行中
	
	int m_pageId;
	static int PageCount;

	CString m_strFilePath;

public:
	virtual void PreSubclassWindow();
			void SetPngBkPic(CString strPicPath,int sonPicNums,int iconWidth = 0,int iconLeftStart = 0,int iconRightStart = 0,BOOL bRedraw = FALSE);
			void SetBkColor(COLORREF bkClrNormal,COLORREF bkClrFocused,COLORREF bkClrClicked,COLORREF bkClrDisabled,BOOL bRedraw = FALSE);
			void GetBkColor(COLORREF& bkClrNormal,COLORREF& bkClrFocused,COLORREF& bkClrClicked,COLORREF& bkClrDisabled);
			void SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw = FALSE);
			void GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled);
			void SetEdgeColor(COLORREF edgeClrNormal,COLORREF edgeClrFocused,COLORREF edgeClrClicked,COLORREF edgeClrDisabled,BOOL bRedraw = FALSE);
			void GetEdgeColor(COLORREF& edgeClrNormal,COLORREF& edgeClrFocused,COLORREF& edgeClrClicked,COLORREF& edgeClrDisabled);
			
			void SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw = FALSE);
			BOOL GetDrawFrame();
			void SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw = FALSE);
			BOOL GetBeInMiddle();
			void SetTextFormat(UINT txFormat,BOOL bRedraw = FALSE);
			UINT GetTextFormat();
			void SetAmoySwtParam(BOOL bAmoySwtStyle,COLORREF bkClr,BOOL bRedraw = FALSE);
			void GetAmoySwtParam(BOOL& bAmoySwtStyle,COLORREF& bkClr);
			void SetBeAmoySwtStyle(BOOL bAmoySwtStyle,BOOL bRedraw = FALSE);
			BOOL GetBeAmoySwtStyle();
			void SetAmoySwtBkClr(COLORREF bkClr,BOOL bRedraw = FALSE);
			COLORREF GetAmoySwtBkClr();

			void SetGradientParam(BOOL bGradientBk,Color glrStart,Color glrEnd,LinearGradientMode gradientMode,BOOL bRedraw = FALSE);
			void GetGradientParam(BOOL& bGradientBk,Color& glrStart,Color& glrEnd,LinearGradientMode& gradientMode);

			void SetCkbtFuncParam(BOOL bCkbtFunc,int curState,BOOL bRedraw = FALSE);
			void GetCkbtFuncParam(BOOL& bCkbtFunc,int& curState);
			void SetBeCkbtFunc(BOOL bCkbtFunc,BOOL bRedraw = FALSE);
			BOOL GetBeCkbtFunc();
			void SetCkbtFuncCurState(int curState,BOOL bRedraw = FALSE);
			BOOL GetCkbtFuncCurState();

			void SetBeAddPageIcon(BOOL bAddPageIcon,BOOL bRedraw = FALSE);
			BOOL GetBeAddPageIcon();

			void SetBeBtPageRunning(BOOL bBtPageRunning,BOOL bRedraw = FALSE);
			BOOL GetBeBtPageRunning();

			void SetPageId(int pageId);
			int GetPageId();

			void EnableWindow(BOOL bEnabled);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);  
	afx_msg void OnCaptureChanged(CWnd* pNewWnd);
};



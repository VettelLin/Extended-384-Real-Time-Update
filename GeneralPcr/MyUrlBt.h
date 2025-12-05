#pragma once

#include "FontSet.h"
// CMyUrlBt

//按钮状态参数
#define CTRL_URLBT_NOFOCUS 1
#define CTRL_URLBT_FOCUS 2
#define CTRL_URLBT_SELECTED 3
#define CTRL_URLBT_DISABLE 4

class CMyUrlBt : public CButton,public CFontSet
{
	DECLARE_DYNAMIC(CMyUrlBt)

public:
	CMyUrlBt(BOOL bAutoDynamic = FALSE,BOOL bUseFourthState = TRUE);
	virtual ~CMyUrlBt();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	UINT   m_idNormal;
	UINT   m_idFocus;
	UINT   m_idSelected;
	UINT   m_idDisabled;

	int	   m_srcWidth;
	int    m_srcHeight;
	int    m_ctrlState;

private:
	COLORREF m_textColor;
	BOOL m_bTracking;

	BOOL m_bHasBkPic;
	BOOL m_bHasPathBkPic;
	BOOL m_bHasPngBkPic;
	int  m_sonPicNums;
	int  m_iconW;

	BOOL m_bStaticSize;// 是否是固定大小（是否在弹窗上）
	BOOL m_bOnPaneDlg;
	int  m_paneStandardWidth;
	int  m_paneStandardHeight;

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

	wstring m_pngPicPath;

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
	int m_curStateId;	//从0开始

	BOOL m_bMinBt;	//是否是最小化按钮

public:
	double m_btHoriFontRatio;
	double m_btVerFontRatio;

	BOOL m_bRoundRectBk;
	Color m_clrRoundRectBk;

public:
	virtual void PreSubclassWindow();
			void SetBmpBkPic(UINT idPic,int sonPicNums = 2,int btIconW = 0,int iconLeftIndent = 0,int iconRightIndent = 0,int rightEnd = 0,BOOL bRedraw = FALSE);
			void SetPngBkPic(wstring picPath,int sonPicNums,int iconWidth = 0,int iconLeftStart = 0,int iconRightStart = 0,BOOL bRedraw = FALSE);
			void SetNewBmpBkPic(UINT idPic,int sonPicNums,BOOL bInvalidate);
			void SetBmpBkPic(UINT idNormal,UINT idFocus,UINT idSelected,UINT idDisabled,int btIconW,int iconLeftStart = 0,int iconRightStart = 0,int rightEnd = 0);
			void SetTextColor(COLORREF textColor);
			COLORREF GetTextColor();
			void SetBkColor(COLORREF bkClrNormal,COLORREF bkClrFocused,COLORREF bkClrClicked,COLORREF bkClrDisabled,BOOL bRedraw = FALSE);
			void GetBkColor(COLORREF& bkClrNormal,COLORREF& bkClrFocused,COLORREF& bkClrClicked,COLORREF& bkClrDisabled);
			void SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw = FALSE);
			void GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled);
			void SetEdgeColor(COLORREF edgeClrNormal,COLORREF edgeClrFocused,COLORREF edgeClrClicked,COLORREF edgeClrDisabled,BOOL bRedraw = FALSE);
			void GetEdgeColor(COLORREF& edgeClrNormal,COLORREF& edgeClrFocused,COLORREF& edgeClrClicked,COLORREF& edgeClrDisabled);
			void SetRoundRectParam(BOOL bRoundRect,Color clrRoundRectBk,BOOL bRedraw = FALSE);
			void GetRoundRectParam(BOOL& bRoundRect,Color& clrRoundRectBk);
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

			void SetBeMinimizeBt(BOOL bMinBt,BOOL bRedraw = FALSE);
			BOOL GetBeMinimizeBt();

			void EnableWindow(BOOL bEnabled);
			void SetBStaticSize(BOOL bStaticSize);
			void SetRatioMode(BOOL bIsStaticRatio = FALSE,BOOL bOnPaneDlg = FALSE,BOOL bInvalidate = FALSE,int paneStandardWidth = 1920,int paneStandardHeight = 1080);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



#pragma once

#include "FontSet.h"
#include "ArrowInfo.h"

// CMyInsertBt

//按钮状态参数
#define CTRL_INSERTBT_NOFOCUS 1
#define CTRL_INSERTBT_FOCUS 2
#define CTRL_INSERTBT_SELECTED 3
#define CTRL_INSERTBT_DISABLE 4

#define RIFHTWIDTH 28	//右侧图标区域宽度

class CMyInsertBt : public CButton,public CFontSet
{
	DECLARE_DYNAMIC(CMyInsertBt)

public:
	CMyInsertBt();
	virtual ~CMyInsertBt();

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
	BOOL m_bTracking;

	BOOL m_bHasBkPic;
	BOOL m_bHasPathBkPic;
	BOOL m_bHasPngBkPic;
	int  m_sonPicNums;
	int  m_iconW;

	int m_leftIndent;
	int m_rightEnd;

	Color m_bkClrNormal;	//背景色，如果是圆角样式，指的是圆角填充背景色
	Color m_bkClrFocused;
	Color m_bkClrClicked;
	Color m_bkClrDisabled;

	Color m_clrArrowNormal;	//右侧箭头色
	Color m_clrArrowFocused;
	Color m_clrArrowClicked;
	Color m_clrArrowDisabled;

	Color m_clrSplitLineNormal;	//中间分隔线颜色
	Color m_clrSplitLineFocused;
	Color m_clrSplitLineClicked;
	Color m_clrSplitLineDisabled;

	COLORREF m_txClrNormal;
	COLORREF m_txClrFocused;
	COLORREF m_txClrClicked;
	COLORREF m_txClrDisabled;

	Color m_edgeClrNormal;
	Color m_edgeClrFocused;
	Color m_edgeClrClicked;
	Color m_edgeClrDisabled;

	BOOL	m_bDrawFrame;	//是否绘制边框

	wstring m_pngPicPath;

	BOOL m_bInMiddle;	//是否居中

	BOOL m_bCloseBt;	//标识是否是关闭按钮

	BOOL m_bTwinkle;	//是否包含闪烁功能
	BOOL m_bTwinkleShow;	//闪烁是否处于显示状态，闪烁包含显示、不显示两种状态，结合m_bHasPngBkPic使用
	int m_curState;	//当前图标类型
	int m_curState2;	//和m_curState对立的闪烁类型

	BOOL m_bTextDown;	//标识文字在图片下方

	int m_iSplitLineXPos;	//记录分割线横坐标位置

public:
	BOOL m_bRoundRectBk;	//标识是否使用圆角边框样式
	Color m_clrRoundRectBk;		//四个圆角外部颜色
	int m_iRoundAngle;	//圆角大小

	CArrowInfo m_arrowInfo;	//箭头信息

public:
	virtual void PreSubclassWindow();
			void SetBmpBkPic(UINT idPic,int sonPicNums = 2,int btIconW = 0,int leftStart = 0,int rightEnd = 0,BOOL bRedraw = FALSE);
			void SetPngBkPic(wstring picPath,int sonPicNums,int iconWidth = 0,int leftIndent = 0,BOOL bRedraw = FALSE);
			void SetNewBmpBkPic(UINT idPic,int sonPicNums,BOOL bInvalidate);
			void SetBmpBkPic(UINT idNormal,UINT idFocus,UINT idSelected,UINT idDisabled,int btIconW,int leftStart = 0,int rightEnd = 0);
			
			void SetBkColor(Color bkClrNormal,Color bkClrFocused,Color bkClrClicked,Color bkClrDisabled,BOOL bRedraw = FALSE);
			void GetBkColor(Color& bkClrNormal,Color& bkClrFocused,Color& bkClrClicked,Color& bkClrDisabled);
			void SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw = FALSE);
			void GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled);
			void SetEdgeColor(Color edgeClrNormal,Color edgeClrFocused,Color edgeClrClicked,Color edgeClrDisabled,BOOL bRedraw = FALSE);
			void GetEdgeColor(Color& edgeClrNormal,Color& edgeClrFocused,Color& edgeClrClicked,Color& edgeClrDisabled);
			void SetRoundRectParam(BOOL bRoundRect,Color clrRoundRectBk,BOOL bRedraw = FALSE);
			void GetRoundRectParam(BOOL& bRoundRect,Color& clrRoundRectBk);
			void SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw = FALSE);
			BOOL GetDrawFrame();
			void SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw = FALSE);
			BOOL GetBeInMiddle();
			void SetBeCloseBt(BOOL bCloseBt,BOOL bRedraw = FALSE);
			BOOL GetBeCloseBt();
			void SetBeTextDown(BOOL bTextDown,BOOL bRedraw = FALSE);
			BOOL GetBeTextDown();

			void SetCurState(int curState,int curState2,BOOL bRedraw = FALSE);
			int GetCurState();
			int GetCurState2();

			BOOL GetBeTwinkle();

			void EnableWindow(BOOL bEnabled);
			
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

			void StartTwinkle();
			void StopTwinkle();

			void SetSplitLineXPos(int iSplitLineXPos,BOOL bRedraw = FALSE);
			int GetSplitLineXPos();
};



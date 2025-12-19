#pragma once

#include "FontSet.h"
#include <GdiPlus.h>
#include "IconBtInfo.h"
// CMyButton

//按钮状态参数
#define CTRL_BUTTON_NOFOCUS 1
#define CTRL_BUTTON_FOCUS 2
#define CTRL_BUTTON_SELECTED 3
#define CTRL_BUTTON_DISABLE 4

//最小化、缩放、关闭按钮
typedef enum DlgSizeBtType{
	EDSBT_MINIMUM = 1,EDSBT_ZOOM,EDSBT_CLOSE
}EDLGSIZEBTTYPE;

class CMyButton : public CButton,public CFontSet
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton(BOOL bAutoDynamic = FALSE);
	virtual ~CMyButton();

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

	int m_iLeftIndent;	//图片显示位置左侧缩进
	int m_iRightIndent;	//图片显示位置右侧缩进

	int m_rightEnd;

	Color m_bkClrNormal;	//背景色
	Color m_bkClrFocused;
	Color m_bkClrClicked;
	Color m_bkClrDisabled;

	Color m_bkClrNormalEnd;	//渐变色终点背景颜色
	Color m_bkClrFocusedEnd;
	Color m_bkClrClickedEnd;
	Color m_bkClrDisabledEnd;
	
	Color m_edgeClrNormal;	//边框颜色
	Color m_edgeClrFocused;
	Color m_edgeClrClicked;
	Color m_edgeClrDisabled;

	COLORREF m_txClrNormal;
	COLORREF m_txClrFocused;
	COLORREF m_txClrClicked;
	COLORREF m_txClrDisabled;

	BOOL	m_bDrawFrame;	//是否绘制边框

	CString m_strPngPicPath;

	BOOL m_bInMiddle;	//是否居中

	BOOL m_bDlgSizeBt;	//标识是否是关闭、最小化、缩放按钮
	EDLGSIZEBTTYPE m_eDlgSizeBtType;

	BOOL m_bTwinkle;	//是否包含闪烁功能
	BOOL m_bTwinkleShow;	//闪烁是否处于显示状态，闪烁包含显示、不显示两种状态，结合m_bHasPngBkPic使用
	int m_curState;	//当前图标类型
	int m_curState2;	//和m_curState对立的闪烁类型

	BOOL m_bTextDown;	//标识文字在图片下方

	CIconBtInfo m_iConBtInfo;
	BOOL m_bIconBt;	//标识是否是图标型按钮

	BOOL m_bHaveTxPushedFormat;	//标识是否包含文字被按下样式

	DWORD m_dWordStyle;

	BOOL m_bFstPaint;
	CDC* m_pMemDc;
	BOOL m_bAutoDynamic;	//20190525添加，适用于大小固定、内容可变或者大小随着父窗口同比例变化的情况，第一次绘制时保存背景信息到m_pMemDc中，为了兼容之前的CStatic控件，保留m_bDynamic选项
	int  m_fstPaintWidth;
	int  m_fstPaintHeight;
	HBITMAP m_hBitmap;

protected:
	BOOL m_bRoundRectBk;
	COLORREF m_clrRoundRectBk;	//标识四个圆角外部背景色
	BOOL m_bGradientBk;	//标识是否是渐变色背景

public:
	virtual void PreSubclassWindow();
			void SetBmpBkPic(UINT idPic,int sonPicNums = 2,int btIconW = 0,int leftStart = 0,int rightEnd = 0,BOOL bRedraw = FALSE);
			void SetPngBkPic(CString strPicPath,int sonPicNums,int iIconWidth = 0,int iLeftIndent = 0,int iRightIndent = 0,BOOL bRedraw = FALSE);
			void SetNewBmpBkPic(UINT idPic,int sonPicNums,BOOL bInvalidate);
			void SetBmpBkPic(UINT idNormal,UINT idFocus,UINT idSelected,UINT idDisabled,int btIconW,int leftStart = 0,int rightEnd = 0);
			void SetTextColor(COLORREF textColor);
			COLORREF GetTextColor();
			void SetBkColor(Color bkClrNormal,Color bkClrFocused,Color bkClrClicked,Color bkClrDisabled,BOOL bRedraw = FALSE);
			void GetBkColor(Color& bkClrNormal,Color& bkClrFocused,Color& bkClrClicked,Color& bkClrDisabled);
			void SetBkColorEnd(Color bkClrNormal,Color bkClrFocused,Color bkClrClicked,Color bkClrDisabled,BOOL bRedraw = FALSE);
			void GetBkColorEnd(Color& bkClrNormal,Color& bkClrFocused,Color& bkClrClicked,Color& bkClrDisabled);
			void SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw = FALSE);
			void GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled);
			void SetEdgeColor(Color edgeClrNormal,Color edgeClrFocused,Color edgeClrClicked,Color edgeClrDisabled,BOOL bRedraw = FALSE);
			void GetEdgeColor(Color& edgeClrNormal,Color& edgeClrFocused,Color& edgeClrClicked,Color& edgeClrDisabled);
			void SetRoundRectParam(BOOL bRoundRect,COLORREF clrRoundRectBk,BOOL bGradientBk,BOOL bRedraw = FALSE);
			void GetRoundRectParam(BOOL& bRoundRect,COLORREF& clrRoundRectBk,BOOL& bGradientBk);
			void SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw = FALSE);
			BOOL GetDrawFrame();
			void SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw = FALSE);
			BOOL GetBeInMiddle();
			void SetDlgSizeBtParam(BOOL bDlgSizeBt,EDLGSIZEBTTYPE eDlgSizeBtType,BOOL bRedraw = FALSE);
			BOOL GetBeDlgSizeBt();
			EDLGSIZEBTTYPE GetDlgSizeBtType();
			void SetBeTextDown(BOOL bTextDown,BOOL bRedraw = FALSE);
			BOOL GetBeTextDown();

			void SetWordStyle(DWORD dwWordStyle,BOOL bRedraw = FALSE);
			DWORD GetWordStyle();

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

			void SetIconBtInfo(CIconBtInfo info,BOOL bRedraw = FALSE);
			void SetIconBtPath(CString strPath,BOOL bRedraw = FALSE);
			CIconBtInfo GetIconBtInfo();
			void SetIconBtTxRectEndX(int iEndX,BOOL bRedraw = FALSE);
			void SetIconBtIconRect(Rect rc,BOOL bRedraw = FALSE);

			void SetBeHaveTxPushedFormat(BOOL bHaveTxPushedFormat);
			BOOL GetBeHaveTxPushedFormat();
};



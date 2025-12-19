#pragma once

#include "FontSet.h"

class CMyCheckButton : public CButton,public CFontSet
{
	DECLARE_DYNAMIC(CMyCheckButton)

public:
	CMyCheckButton();
	virtual ~CMyCheckButton();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
			void SetBkPicture(UINT idChecked,UINT idUnChecked,BOOL bRedraw = FALSE);
			void SetBkPicture(UINT idPicture,int sonPicNums = 2,int iconW = 0);
			void SetPngBkPic(wstring picPath,int sonPicNums = 2,int iconW = 0,COLORREF pngBkClr = RGB(255,255,255),BOOL bInvalidate = FALSE);
			void SetBtChecked(BOOL btState,BOOL bRedraw = FALSE);
			BOOL GetCheckState();
			void SetShieldClickMsgState(BOOL bShieldClickMsg);
			BOOL GetShieldClickMsgState();
			void SetBkColor(COLORREF bkClrUnchked,COLORREF bkClrChked,BOOL bRedraw = FALSE);
			void GetBkColor(COLORREF& bkClrUnchked,COLORREF& bkClrChked);
			void SetRoundStyle(BOOL bRoundSytle,COLORREF roundStyleBkClr,BOOL bRedraw = FALSE);
			BOOL GetRoundStyle(COLORREF* pRoundStyleBkClr = NULL);

			void SetTickStyle(BOOL bTickSytle,BOOL bRedraw = FALSE);
			BOOL GetTickStyle();

			void SetWordStyle(DWORD dwWordStyle,BOOL bRedraw = FALSE);
			DWORD GetWordStyle();

			void SetFrameColor(COLORREF clrFrame,BOOL bRedraw = FALSE);
			BOOL GetFrameColor();

			void SetSelectStateSerail(int selStateSerial,BOOL bRedraw = FALSE);
			int GetSelectStateSerail();

			void SetTextColor(COLORREF txClrUnchecked,COLORREF txClrChecked = RGB(0,0,0),BOOL bRedraw = FALSE);
			COLORREF GetTextColor();

			void SetBeNeedRedrawBk(BOOL bNeedRedrawBk,BOOL bRedraw = FALSE);
			BOOL GetBeNeedRedrawBk();

			void SetPngBkColor(COLORREF pngBkClr,BOOL bRedraw = FALSE);
			COLORREF GetPngBkColor();  

			void SetBeParentProcessMouseClick(BOOL bParentProcMouseClick,BOOL bRedraw = FALSE);
			COLORREF GetBeParentProcessMouseClick();
			
			void SetIconTextInterval(int iLeftIndent,BOOL bRedraw = FALSE);	//设置左侧缩进（图标和文本之间的间隔）
			int GetIconTextInterval();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	COLORREF m_bkClrUnchked;
	COLORREF m_bkClrChked;

	BOOL m_isBtChecked;

	UINT   m_idChecked;
	UINT   m_idUnChecked;
	BOOL   m_bHasBkPic;
	BOOL   m_bHasPngBkPic;

	wstring m_picPath;

	int m_sonPicNums;
	COLORREF m_txClrUnchecked;
	COLORREF m_txClrChecked;

	COLORREF m_pngBkClr;

	BOOL m_bShieldClickMsg;

	int  m_iconW;
	int m_iLeftIndent;
	int m_rightEnd;

	double m_cheBtHoriFontRatio;
	double m_cheBtVerFontRatio;

	BOOL m_bNeedRedrawBk;	//是否需要重新填充背景
	BOOL m_bParentProcMouseClick;	//是否由父窗口处理点击事件

	BOOL m_bRoundStyle;
	BOOL m_bTickStyle;

	COLORREF m_roundStyleBkClr;	//圆形外部背景色

	DWORD m_dwWordStyle;

	COLORREF m_clrFrame;

	int m_selStateSerail;	//选中状态在背景图中的序列号
};



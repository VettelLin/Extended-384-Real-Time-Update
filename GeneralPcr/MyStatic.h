#pragma once

#include "FontSet.h"
// CMyStatic

class CMyStatic : public CStatic,public CFontSet
{
	DECLARE_DYNAMIC(CMyStatic)

public:
	CMyStatic(BOOL bAutoDynamic = FALSE);
	virtual ~CMyStatic();

private:
	COLORREF m_bkColor;
	COLORREF m_textColor;

	BOOL m_bSingleLine; //设置是否单行显示文字
	UINT m_nFormat;//定义文字显示格式
	BOOL m_bHasBkPic;
	BOOL m_bHasPngBkPic;
	BOOL m_bTwinkle;	//是否包含闪烁功能
	BOOL m_bTwinkleShow;	//闪烁是否处于显示状态，闪烁包含显示、不显示两种状态，结合m_bHasPngBkPic使用
	int  m_bkPicID;
	CString m_pngPicPath;
	int m_iconWidth;
	int m_sonPicNums;
	int m_curState;	//当前图标类型

	int m_iconLeftIndent;	//图标左侧缩进
	int m_iconRightIndent;	//图片右侧缩进

	BOOL m_bDynamic; 

	BOOL m_bFstPaint;
	CDC* m_pMemDc;
	BOOL m_bAutoDynamic;	//20190525添加，适用于大小固定、内容可变或者大小随着父窗口同比例变化的情况，第一次绘制时保存背景信息到m_pMemDc中，为了兼容之前的CStatic控件，保留m_bDynamic选项
	int  m_fstPaintWidth;
	int  m_fstPaintHeight;

	BOOL m_bHaveRoundRectBk;	//标识是否使用圆角背景样式
	COLORREF m_clrFourAngleBk;	//四角背景
	Color m_clrRoundRectBk;	//圆角背景
	int m_iArcRadius;	//圆角大小

public:
	HBITMAP m_hBitmap;

protected:
	DECLARE_MESSAGE_MAP()

public:
			void SetSingleLineState(BOOL bSingleLine = TRUE);
			BOOL GetSingleLineState();
			void SetWordStyle(UINT nFormat,BOOL bInvalidate = FALSE);
			void GetWordStyle(UINT& nFormat);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

			void SetCurState(int curState,BOOL bRedraw = FALSE);
			void SetBkColor(COLORREF bkColor);
			COLORREF GetBkColor();
			void SetTextColor(COLORREF textColor,BOOL bRedraw = FALSE);
			COLORREF GetTextColor();
			void SetBmpBkPic(int bkPicId,int iconWidth = 0,BOOL bRedraw = FALSE);
			void SetPngBkPic(CString picPath,int sonPicNums = 1,int iconWidth = 0,int iconLeftIndent = 0,int iconRightIndent = 0,BOOL bInvalidate = FALSE);

			void SetRoundRectBkParam(BOOL bHaveRoundRectBk,COLORREF clrFourAngle,Color clrRoundRectBk,int iArcRadius,BOOL bRedraw = FALSE);
			Color GetRoundRectBkColor();
			int GetRoundRectArcSize();

			void SetBDynamic(BOOL bDyNaminc);
			BOOL GetBDynamic();
		
			void StartTwinkle();
			void StopTwinkle();
		
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



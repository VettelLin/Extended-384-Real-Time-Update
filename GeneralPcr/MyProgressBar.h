#pragma once

#include "FontSet.h"
#include <GdiPlus.h>

// CMyProgressBar

class CMyProgressBar : public CStatic,public CFontSet
{
	DECLARE_DYNAMIC(CMyProgressBar)

public:
	CMyProgressBar(BOOL bAutoDynamic = FALSE);
	virtual ~CMyProgressBar();

private:
	COLORREF m_bkColor;
	COLORREF m_textColor;

	BOOL m_bSingleLine; //设置是否单行显示文字
	UINT m_nFormat;//定义文字显示格式
	int  m_bkPicID;
	wstring m_pngPicPath;
	int m_iconWidth;

	BOOL m_bFstPaint;
	CDC* m_pMemDc;
	BOOL m_bAutoDynamic;	//20190525添加，适用于大小固定、内容可变或者大小随着父窗口同比例变化的情况，第一次绘制时保存背景信息到m_pMemDc中，为了兼容之前的CStatic控件，保留m_bDynamic选项
	int  m_fstPaintWidth;
	int  m_fstPaintHeight;
	HBITMAP m_hBitmap;

protected:
	Gdiplus::Color m_clrBk;	//主背景（非进度部分）颜色
	Gdiplus::Color m_clrFrame;	//主背景边框颜色
	Gdiplus::Color m_clrRopBk;	//进度背景颜色
	Gdiplus::Color m_clrRopFrame;	//进度背景边框颜色

public:
	double m_dProgress;

protected:
	DECLARE_MESSAGE_MAP()

public:
			void SetSingleLineState(BOOL bSingleLine = TRUE);
			BOOL GetSingleLineState();
			void SetWordStyle(UINT nFormat,BOOL bInvalidate = FALSE);
			void GetWordStyle(UINT& nFormat);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

			void SetBkColor(COLORREF bkColor);
			COLORREF GetBkColor();
			void SetTextColor(COLORREF textColor,BOOL bInvalidate = FALSE);
			COLORREF GetTextColor();
			void SetBmpBkPic(int bkPicId,int iconWidth = 0,BOOL bRedraw = FALSE);
			void SetPngBkPic(wstring picPath,BOOL bInvalidate = FALSE);
			
			void SetCurProgress(double dProgress,BOOL bRedraw = FALSE);
			void SetZero(BOOL bRedraw = FALSE);
			void SetOneHundred(BOOL bRedraw = FALSE);

			double GetProgress();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



// MyScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "MyScrollInfo.h"

CMyScrollInfo::CMyScrollInfo()
{
	m_clrBk = RGB(0XFF,0XFF,0XFE);
	m_clrScrollFrame = RGB(197,197,197);
	m_clrStartArrowRegionFrame = RGB(197,197,197);
	m_clrEndArrowRegionFrame = RGB(197,197,197);
	m_clrSliderBk = Color(0XE3,0XE3,0XE3);
	m_clrSliderFrame = Color(0,0X66,0X3E);

	m_rcSlider = CRect(7,3,31,8);
	m_bInSlider = FALSE;
	m_bLeftBtDown = FALSE;
	m_bStartDragSlider = FALSE;
	m_arrowWH = 0;
	m_scrollW = 0;
	m_scrollH = 0;
	m_pixelForShow = 0;
	m_pixelTotal = 0;
	m_curPixelPos = 0;
	m_curSliderPos = 0;
	m_bScrollNeedShow = FALSE;
	m_dSliderRatio = 0;
	m_dScrollRatio = 0;
	m_validScope = 0;
	m_eScrollType = EST_NONE;
	m_bSliderDragMove = FALSE;
	m_bShowStartArrowRegion = TRUE;
	m_bShowEndArrowRegion = TRUE;
	m_bShowSliderFrame = TRUE;
	m_bShowScrollFrame = FALSE;
	m_bShowStartArrowRegionFrame = TRUE;
	m_bShowEndArrowRegionFrame = TRUE;

	m_eSliderShape = ESS_ROUNDRECTANGLE;
	m_dHoriRatio = 1;
	m_dVertRatio = 1;

	m_arrowInfo1.m_eArrowStyle = EAS_LEFTRIGHT;
	m_arrowInfo2.m_eArrowStyle = EAS_LEFTRIGHT;

	m_bInStartArrow = FALSE;
	m_bInStartPane = FALSE;
	m_bInEndPane = FALSE;
	m_bInEndArrow = FALSE;
}

CMyScrollInfo::~CMyScrollInfo()
{

}

CMyScrollInfo::CMyScrollInfo(const CMyScrollInfo& info)
{
	m_clrBk = info.m_clrBk;
	m_clrScrollFrame = info.m_clrScrollFrame;
	m_clrStartArrowRegionFrame = info.m_clrStartArrowRegionFrame;
	m_clrEndArrowRegionFrame = info.m_clrEndArrowRegionFrame;
	m_clrSliderBk = info.m_clrSliderBk;
	m_clrSliderFrame = info.m_clrSliderFrame;
	m_rcSlider = info.m_rcSlider;
	m_bInSlider = info.m_bInSlider;
	m_bLeftBtDown = info.m_bLeftBtDown;
	m_bStartDragSlider = info.m_bStartDragSlider;
	m_arrowWH = info.m_arrowWH;
	m_scrollW = info.m_scrollW;
	m_scrollH = info.m_scrollH;
	m_pixelForShow = info.m_pixelForShow;
	m_pixelTotal = info.m_pixelTotal;
	m_curPixelPos = info.m_curPixelPos;
	m_curSliderPos = info.m_curSliderPos;
	m_bScrollNeedShow = info.m_bScrollNeedShow;
	m_dSliderRatio = info.m_dSliderRatio;
	m_dScrollRatio = info.m_dScrollRatio;
	m_validScope = info.m_validScope;
	m_eScrollType = info.m_eScrollType;
	m_bSliderDragMove = info.m_bSliderDragMove;
	m_bShowStartArrowRegion = info.m_bShowStartArrowRegion;
	m_bShowEndArrowRegion = info.m_bShowEndArrowRegion;
	m_bShowSliderFrame = info.m_bShowSliderFrame;
	m_bShowScrollFrame = info.m_bShowScrollFrame;
	m_bShowStartArrowRegionFrame = info.m_bShowStartArrowRegionFrame;
	m_bShowEndArrowRegionFrame = info.m_bShowEndArrowRegionFrame;

	m_eSliderShape = info.m_eSliderShape;
	m_arrowInfo1 = info.m_arrowInfo1;
	m_arrowInfo2 = info.m_arrowInfo2;
}

void CMyScrollInfo::operator=(const CMyScrollInfo& info)
{
	m_clrBk = info.m_clrBk;
	m_clrScrollFrame = info.m_clrScrollFrame;
	m_clrStartArrowRegionFrame = info.m_clrStartArrowRegionFrame;
	m_clrEndArrowRegionFrame = info.m_clrEndArrowRegionFrame;
	m_clrSliderBk = info.m_clrSliderBk;
	m_clrSliderFrame = info.m_clrSliderFrame;
	m_rcSlider = info.m_rcSlider;
	m_bInSlider = info.m_bInSlider;
	m_bLeftBtDown = info.m_bLeftBtDown;
	m_bStartDragSlider = info.m_bStartDragSlider;
	m_arrowWH = info.m_arrowWH;
	m_scrollW = info.m_scrollW;
	m_scrollH = info.m_scrollH;
	m_pixelForShow = info.m_pixelForShow;
	m_pixelTotal = info.m_pixelTotal;
	m_curPixelPos = info.m_curPixelPos;
	m_curSliderPos = info.m_curSliderPos;
	m_bScrollNeedShow = info.m_bScrollNeedShow;
	m_dSliderRatio = info.m_dSliderRatio;
	m_dScrollRatio = info.m_dScrollRatio;
	m_validScope = info.m_validScope;
	m_eScrollType = info.m_eScrollType;
	m_bSliderDragMove = info.m_bSliderDragMove;
	m_bShowStartArrowRegion = info.m_bShowStartArrowRegion;
	m_bShowEndArrowRegion = info.m_bShowEndArrowRegion;
	m_bShowSliderFrame = info.m_bShowSliderFrame;
	m_bShowScrollFrame = info.m_bShowScrollFrame;
	m_bShowStartArrowRegionFrame = info.m_bShowStartArrowRegionFrame;
	m_bShowEndArrowRegionFrame = info.m_bShowEndArrowRegionFrame;

	m_eSliderShape = info.m_eSliderShape;
	m_arrowInfo1 = info.m_arrowInfo1;
	m_arrowInfo2 = info.m_arrowInfo2;
}

void CMyScrollInfo::CalcSliderPos()
{
	if(!m_bScrollNeedShow)
	{
		return;
	}

	int pxMargin = m_pixelTotal - m_pixelForShow;
	if(pxMargin < 0)
	{
		pxMargin = 0;
	}
	m_dSliderRatio = (double)pxMargin / m_pixelForShow;

	if(EST_HORI == m_eScrollType)
	{
		int scrollValidLen = m_rcScroll.Width() - m_arrowWH * 2;
		int sliderWH = scrollValidLen / (m_dSliderRatio + 1);
		if(scrollValidLen != sliderWH)
		{
			m_dScrollRatio = (double)pxMargin / (scrollValidLen - sliderWH);
			m_curSliderPos = m_curPixelPos / m_dScrollRatio;
		}
		else
		{
			m_dScrollRatio = 0.0;
			m_curSliderPos = 0;
		}

		if(0 != m_dScrollRatio)
		{
			m_rcSlider.left = m_rcScroll.left + m_arrowWH + m_curPixelPos / m_dScrollRatio;
		}
		else
		{
			m_rcSlider.left = m_rcScroll.left + m_arrowWH;
		}
		m_rcSlider.right = m_rcSlider.left + sliderWH;
		m_rcSlider.top = m_rcScroll.top + 8 * m_dVertRatio;
		m_rcSlider.bottom = m_rcScroll.bottom - 8 * m_dVertRatio;

		m_rcStartArrow = CRect(m_rcScroll.left,m_rcScroll.top,m_rcScroll.left + m_arrowWH,m_rcScroll.bottom);
		m_rcEndArrow = CRect(m_rcScroll.right - m_arrowWH,m_rcScroll.top,m_rcScroll.right,m_rcScroll.bottom);

		m_validScope = m_rcScroll.Width() - m_rcStartArrow.Width() - m_rcEndArrow.Width() - m_rcSlider.Width();
	}
	else if(EST_VERT == m_eScrollType)
	{
		int scrollValidLen = m_rcScroll.Height() - m_arrowWH * 2;
		int sliderWH = scrollValidLen / (m_dSliderRatio + 1);

		if(scrollValidLen != sliderWH)
		{
			m_dScrollRatio = (double)pxMargin / (scrollValidLen - sliderWH);
		}
		else
		{
			m_dScrollRatio = 0.0;
		}

		if(0 != m_dScrollRatio)
		{
			m_rcSlider.top = m_rcScroll.top + m_arrowWH + m_curPixelPos / m_dScrollRatio;
			m_curSliderPos = m_curPixelPos / m_dScrollRatio;
		}
		else
		{
			m_rcSlider.top = m_rcScroll.top + m_arrowWH;
			m_curSliderPos = 0;
		}

		int iSliderLRIndent = 8 * m_dHoriRatio;
		if(m_rcScroll.Width() < 24 * m_dHoriRatio)
		{
			iSliderLRIndent = 4 * m_dHoriRatio;
		}
		m_rcSlider.left = m_rcScroll.left + iSliderLRIndent; 
		m_rcSlider.right = m_rcScroll.right - iSliderLRIndent; 
		m_rcSlider.top = m_rcScroll.top + m_arrowWH + m_curPixelPos / m_dScrollRatio;
		m_rcSlider.bottom = m_rcSlider.top + sliderWH;

		m_rcStartArrow = CRect(m_rcScroll.left,m_rcScroll.top,m_rcScroll.right,m_rcScroll.top + m_arrowWH);
		m_rcEndArrow = CRect(m_rcScroll.left,m_rcScroll.bottom - m_arrowWH,m_rcScroll.right,m_rcScroll.bottom);

		m_validScope = m_rcScroll.Height() - m_rcStartArrow.Height() - m_rcEndArrow.Height() - m_rcSlider.Height();
	}
	
	return;
}

void CMyScrollInfo::CalcCurPixelPos()
{
	m_curPixelPos = m_curSliderPos * m_dScrollRatio;
}


void CMyScrollInfo::CalcCurPointInPane(CPoint point)
{
	if (!m_bScrollNeedShow)
		return;

	if (m_rcScroll.PtInRect(point))
	{
		m_bInStartArrow = m_rcStartArrow.PtInRect(point);
		m_bInEndArrow = m_rcEndArrow.PtInRect(point);
		m_bInSlider = m_rcSlider.PtInRect(point);

		if (m_bInStartArrow || m_bInEndArrow || m_bInSlider)
		{
			m_bInStartPane = FALSE;
			m_bInEndPane = FALSE;
		}
		else
		{
			if (m_eScrollType == EST_HORI)
			{
				m_bInStartPane = point.x > m_rcStartArrow.right && point.x < m_rcSlider.left;
				m_bInEndPane = point.x > m_rcSlider.right && point.x < m_rcEndArrow.left;
			}
			else
			{
				m_bInStartPane = point.y > m_rcStartArrow.bottom && point.y < m_rcSlider.top;
				m_bInEndPane = point.y > m_rcSlider.bottom && point.y < m_rcEndArrow.top;
			}
		}
	}
	else
	{
		m_bInSlider = FALSE;
		m_bInStartArrow = FALSE;
		m_bInStartPane = FALSE;
		m_bInEndPane = FALSE;
		m_bInEndArrow = FALSE;
	}
}

BOOL CMyScrollInfo::IsInScrollPane()
{
	return m_bInSlider || m_bInStartArrow || m_bInStartPane || m_bInEndPane || m_bInEndArrow;
}

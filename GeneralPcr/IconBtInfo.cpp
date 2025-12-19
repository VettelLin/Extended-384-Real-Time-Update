#include "StdAfx.h"
#include "IconBtInfo.h"


CIconBtInfo::CIconBtInfo(void)
{
	m_bUseIcon = TRUE;
	m_strIconPath = _T("");
	m_iSonIconNum = 1;
	m_bSonIconPosOrder = TRUE;

	m_bRoundRectBk = TRUE;
	m_iRoundRadius = 8;
	m_iBoderLineW = 1;

	int iAlpha = 255 * 0.2;
	m_clrFrameNormal = Color(iAlpha,0XFF,0XFF,0XFF);
	m_clrFrameHover = Color(iAlpha,0XFF,0XFF,0XFF);
	m_clrFrameSelected = Color(iAlpha,0X0,0X57,0X5F);
	m_clrFrameDisabled = Color(iAlpha,0X0,0X57,0X5F);
	m_clrNormal = COLOR_SOFT_THEME;
	m_clrHover = COLOR_SOFT_THEME;

	m_clrSelected = COLOR_2LEVEL_THEME;
	m_clrDisabled = COLOR_2LEVEL_THEME;

	m_bNormalNotFill = FALSE;
	m_bHoverNotFill = FALSE;
}

CIconBtInfo::~CIconBtInfo(void)
{

}

CIconBtInfo::CIconBtInfo(const CIconBtInfo& info)
{
	m_bUseIcon = info.m_bUseIcon;
	m_strIconPath = info.m_strIconPath;
	m_iSonIconNum = info.m_iSonIconNum;
	m_bSonIconPosOrder = info.m_bSonIconPosOrder;

	m_bRoundRectBk = info.m_bRoundRectBk;
	m_iRoundRadius = info.m_iRoundRadius;
	m_iBoderLineW = info.m_iBoderLineW;

	m_rcIcon = info.m_rcIcon;
	m_rcText = info.m_rcText;

	m_clrFrameNormal = info.m_clrFrameNormal;
	m_clrFrameHover = info.m_clrFrameHover;
	m_clrFrameSelected = info.m_clrFrameSelected;
	m_clrFrameDisabled = info.m_clrFrameDisabled;

	m_clrNormal = info.m_clrNormal;
	m_clrHover = info.m_clrHover;
	m_clrSelected = info.m_clrSelected;
	m_clrDisabled = info.m_clrDisabled;

	m_bNormalNotFill = info.m_bNormalNotFill;
	m_bHoverNotFill = info.m_bHoverNotFill;
}

void CIconBtInfo::operator=(const CIconBtInfo& info)
{
	m_bUseIcon = info.m_bUseIcon;
	m_strIconPath = info.m_strIconPath;
	m_iSonIconNum = info.m_iSonIconNum;
	m_bSonIconPosOrder = info.m_bSonIconPosOrder;

	m_bRoundRectBk = info.m_bRoundRectBk;
	m_iRoundRadius = info.m_iRoundRadius;
	m_iBoderLineW = info.m_iBoderLineW;

	m_rcIcon = info.m_rcIcon;
	m_rcText = info.m_rcText;

	m_clrFrameNormal = info.m_clrFrameNormal;
	m_clrFrameHover = info.m_clrFrameHover;
	m_clrFrameSelected = info.m_clrFrameSelected;
	m_clrFrameDisabled = info.m_clrFrameDisabled;

	m_clrNormal = info.m_clrNormal;
	m_clrHover = info.m_clrHover;
	m_clrSelected = info.m_clrSelected;
	m_clrDisabled = info.m_clrDisabled;

	m_bNormalNotFill = info.m_bNormalNotFill;
	m_bHoverNotFill = info.m_bHoverNotFill;
}


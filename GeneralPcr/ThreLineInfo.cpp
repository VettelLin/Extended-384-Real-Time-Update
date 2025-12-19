#include "StdAfx.h"
#include "ThreLineInfo.h"

CThreLineInfo::CThreLineInfo(void)
{
	m_threLinePixelH = 1;
	m_threLineNormalWidth = 2;
	m_threLineHighLightWidth = 4;
	m_threLineDiscernHalfH = 2;
	m_clrThreLine = Color(0,0,0);
	m_bThreLineSel = FALSE;
	m_dThre = 1;
	m_dThreAuto = 0.1;
	m_bAutoThre = TRUE;
	m_bShow = TRUE;
	m_bFstDrawThreLine = TRUE;
}

CThreLineInfo::~CThreLineInfo(void)
{

}

CThreLineInfo::CThreLineInfo(const CThreLineInfo& threLineInfo)
{
	m_threLinePixelH = threLineInfo.m_threLinePixelH;
	m_threLineNormalWidth = threLineInfo.m_threLineNormalWidth;
	m_threLineHighLightWidth = threLineInfo.m_threLineHighLightWidth;
	m_threLineDiscernHalfH = threLineInfo.m_threLineDiscernHalfH;
	m_clrThreLine = threLineInfo.m_clrThreLine;
	m_rcThreLine = threLineInfo.m_rcThreLine;
	m_bThreLineSel = threLineInfo.m_bThreLineSel;
	m_bShow = threLineInfo.m_bShow;
	m_bFstDrawThreLine = threLineInfo.m_bFstDrawThreLine;
	m_dThre = threLineInfo.m_dThre;
	m_dThreAuto = threLineInfo.m_dThreAuto;
	m_bAutoThre = threLineInfo.m_bAutoThre;
}

void CThreLineInfo::operator=(const CThreLineInfo& threLineInfo)
{
	m_threLinePixelH = threLineInfo.m_threLinePixelH;
	m_threLineNormalWidth = threLineInfo.m_threLineNormalWidth;
	m_threLineHighLightWidth = threLineInfo.m_threLineHighLightWidth;
	m_threLineDiscernHalfH = threLineInfo.m_threLineDiscernHalfH;
	m_clrThreLine = threLineInfo.m_clrThreLine;
	m_rcThreLine = threLineInfo.m_rcThreLine;
	m_bThreLineSel = threLineInfo.m_bThreLineSel;
	m_bShow = threLineInfo.m_bShow;
	m_bFstDrawThreLine = threLineInfo.m_bFstDrawThreLine;
	m_dThre = threLineInfo.m_dThre;
	m_dThreAuto = threLineInfo.m_dThreAuto;
	m_bAutoThre = threLineInfo.m_bAutoThre;
}

void CThreLineInfo::SetThreLineColor(Color clrThreLine)
{
	m_clrThreLine = clrThreLine;
	return;
}

Color CThreLineInfo::GetThreLineColor()
{
	return m_clrThreLine;
}

void CThreLineInfo::SetBeShow(BOOL bShow)
{
	m_bShow = bShow;
	return;
}

BOOL CThreLineInfo::GetBeShow()
{
	return m_bShow;
}

void CThreLineInfo::SetBeFirstDrawThreLline(BOOL bFstDrawThreLine)
{
	m_bFstDrawThreLine = bFstDrawThreLine;
	return;
}

BOOL CThreLineInfo::GetBeFirstDrawThreLline()
{
	return m_bFstDrawThreLine;
}

void CThreLineInfo::SetThreLineValue(double threLineValue)
{
	m_dThre = threLineValue;
	return;
}

double CThreLineInfo::GetThreLineValue()
{
	//if(m_bAutoThre)
	//{
	//	return m_dThreAuto;
	//}
	//else
	{
		return m_dThre;
	}
}








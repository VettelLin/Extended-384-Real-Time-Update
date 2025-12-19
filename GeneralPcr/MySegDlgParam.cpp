#include "StdAfx.h"
#include "MySegDlgParam.h"


CMySegDlgParam::CMySegDlgParam(void)
{
	m_clrGeneral = CLR_2LEVEL_THEME;
	m_clrStepTag = RGB(0X66,0X66,0X66);
	m_clrSegTag = RGB(0X39,0X45,0X65);
	m_clrRunning = RGB(0XFF,0XFF,0XFF);
}

CMySegDlgParam::~CMySegDlgParam(void)
{

}

CMySegDlgParam::CMySegDlgParam(const CMySegDlgParam& segDlgParam)
{
	m_clrGeneral = segDlgParam.m_clrGeneral;
	m_clrStepTag = segDlgParam.m_clrStepTag;
	m_clrSegTag = segDlgParam.m_clrSegTag;
	m_clrRunning = segDlgParam.m_clrRunning;
}

void CMySegDlgParam::operator=(const CMySegDlgParam& segDlgParam)
{
	m_clrGeneral = segDlgParam.m_clrGeneral;
	m_clrStepTag = segDlgParam.m_clrStepTag;
	m_clrSegTag = segDlgParam.m_clrSegTag;
	m_clrRunning = segDlgParam.m_clrRunning;
}

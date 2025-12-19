#include "StdAfx.h"
#include "MeltParamChangeInfo.h"

CMeltParamChangeInfo::CMeltParamChangeInfo(void)
{
	m_bChanged = FALSE;
	m_bMeltTemMinChanged = FALSE;
	m_bMeltTemMaxChanged = FALSE;
	m_bPeakHeightThreChanged = FALSE;
	m_bCrvMeltPeakNeedRedraw = FALSE;
	m_bRstLstNeedRedraw = FALSE;
}

CMeltParamChangeInfo::~CMeltParamChangeInfo(void)
{

}

CMeltParamChangeInfo::CMeltParamChangeInfo(const CMeltParamChangeInfo& info)
{
	m_bChanged = info.m_bChanged;
	m_bMeltTemMinChanged = info.m_bMeltTemMinChanged;
	m_bMeltTemMaxChanged = info.m_bMeltTemMaxChanged;
	m_bPeakHeightThreChanged = info.m_bPeakHeightThreChanged;
	m_bCrvMeltPeakNeedRedraw = info.m_bCrvMeltPeakNeedRedraw;
	m_bRstLstNeedRedraw = info.m_bRstLstNeedRedraw;
}

void CMeltParamChangeInfo::operator=(const CMeltParamChangeInfo& info)
{
	m_bChanged = info.m_bChanged;
	m_bMeltTemMinChanged = info.m_bMeltTemMinChanged;
	m_bMeltTemMaxChanged = info.m_bMeltTemMaxChanged;
	m_bPeakHeightThreChanged = info.m_bPeakHeightThreChanged;
	m_bCrvMeltPeakNeedRedraw = info.m_bCrvMeltPeakNeedRedraw;
	m_bRstLstNeedRedraw = info.m_bRstLstNeedRedraw;
}

BOOL CMeltParamChangeInfo::GetBeChanged()
{
	m_bChanged = m_bMeltTemMinChanged || m_bMeltTemMaxChanged || m_bPeakHeightThreChanged;
	return m_bChanged;
}


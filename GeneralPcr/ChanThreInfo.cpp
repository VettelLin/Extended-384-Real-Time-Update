#include "StdAfx.h"
#include "ChanThreInfo.h"

CChanThreInfo::CChanThreInfo(void)
{
	m_eLineDataType = ELDT_NORMALIZE;
	m_eLineShowFormat = ELSF_LINEAR;
	m_iChanId = -1;
	m_strChanName = _T("");
	m_strTargetName = _T("");
}

CChanThreInfo::~CChanThreInfo(void)
{

}

CChanThreInfo::CChanThreInfo(const CChanThreInfo& info)
{
	m_eLineDataType = info.m_eLineDataType;
	m_eLineShowFormat = info.m_eLineShowFormat; 
	m_iChanId = info.m_iChanId;
	m_strChanName = info.m_strChanName;
	m_strTargetName = info.m_strTargetName;

	m_tliNormolize = info.m_tliNormolize;
	m_tliDeltaRn = info.m_tliDeltaRn;
	m_tliLogNormolize = info.m_tliLogNormolize;
	m_tliLogDeltaRn = info.m_tliLogDeltaRn;
}

void CChanThreInfo::operator=(const CChanThreInfo& info)
{
	m_eLineDataType = info.m_eLineDataType;
	m_eLineShowFormat = info.m_eLineShowFormat; 
	m_iChanId = info.m_iChanId;
	m_strChanName = info.m_strChanName;
	m_strTargetName = info.m_strTargetName;

	m_tliNormolize = info.m_tliNormolize;
	m_tliDeltaRn = info.m_tliDeltaRn;
	m_tliLogNormolize = info.m_tliLogNormolize;
	m_tliLogDeltaRn = info.m_tliLogDeltaRn;
}

void CChanThreInfo::operator=(CChanThreInfo& info)
{
	m_eLineDataType = info.m_eLineDataType;
	m_eLineShowFormat = info.m_eLineShowFormat; 
	m_iChanId = info.m_iChanId;
	m_strChanName = info.m_strChanName;
	m_strTargetName = info.m_strTargetName;

	m_tliNormolize = info.m_tliNormolize;
	m_tliDeltaRn = info.m_tliDeltaRn;
	m_tliLogNormolize = info.m_tliLogNormolize;
	m_tliLogDeltaRn = info.m_tliLogDeltaRn;
}

CThreLineInfo* CChanThreInfo::GetCurSelThreInfo()
{ 
	if(ELSF_LINEAR == m_eLineShowFormat)
	{
		if(ELDT_NORMALIZE == m_eLineDataType)
		{
			return &m_tliNormolize; 
		}
		else if(ELDT_DELTARN == m_eLineDataType)
		{
			return &m_tliDeltaRn; 
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if(ELDT_NORMALIZE == m_eLineDataType)
		{
			return &m_tliLogNormolize; 
		}
		else if(ELDT_DELTARN == m_eLineDataType)
		{
			return &m_tliLogDeltaRn; 
		}
		else
		{
			return NULL;
		}
	}
}

CThreLineInfo* CChanThreInfo::GetCurSelAssociatedThreInfo()
{ 
	if(ELSF_LINEAR == m_eLineShowFormat)
	{
		if(ELDT_NORMALIZE == m_eLineDataType)
		{
			return &m_tliLogNormolize; 
		}
		else if(ELDT_DELTARN == m_eLineDataType)
		{
			return &m_tliLogDeltaRn; 
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if(ELDT_NORMALIZE == m_eLineDataType)
		{
			return &m_tliNormolize; 
		}
		else if(ELDT_DELTARN == m_eLineDataType)
		{
			return &m_tliDeltaRn; 
		}
		else
		{
			return NULL;
		}
	}
}

void CChanThreInfo::SetBeAutoThre(BOOL bAutoThre)
{
	m_tliNormolize.m_bAutoThre = bAutoThre;
	m_tliNormolize.m_bFstDrawThreLine = TRUE;
	m_tliDeltaRn.m_bAutoThre = bAutoThre;
	m_tliDeltaRn.m_bFstDrawThreLine = TRUE;
	m_tliLogNormolize.m_bAutoThre = bAutoThre;
	m_tliLogNormolize.m_bFstDrawThreLine = TRUE;
	m_tliLogDeltaRn.m_bAutoThre = bAutoThre;
	m_tliLogDeltaRn.m_bFstDrawThreLine = TRUE;
	return;
}


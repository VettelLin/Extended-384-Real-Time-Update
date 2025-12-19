#include "StdAfx.h"
#include "LegendInfo.h"


CLegendInfo::CLegendInfo(void)
{
	m_iLeftIndent = 41; 
	m_iRoundRadius = 6;
	m_iInrIconToTx = 8;
	m_iInrTxToIcon = 20;
	m_iOneCharW = 14;
	m_clrFrame = Color(0XD8,0XD8,0XD8);
	m_clrBk = Color(255,0XF7,0XF7,0XF7);
	m_clrBkEnd = Color(255,0XF5,0XF5,0XF5);
}


CLegendInfo::~CLegendInfo(void)
{
}

CLegendInfo::CLegendInfo(const CLegendInfo& info)
{
	m_iLeftIndent = info.m_iLeftIndent;
	m_iRoundRadius = info.m_iRoundRadius;
	m_iInrIconToTx = info.m_iInrIconToTx;
	m_iInrTxToIcon = info.m_iInrTxToIcon;
	m_iOneCharW = info.m_iOneCharW;
	m_clrFrame = info.m_clrFrame;
	m_clrBk = info.m_clrBk;
	m_clrBkEnd = info.m_clrBkEnd;
	m_lnkSingleLgdInfo = info.m_lnkSingleLgdInfo;
}

void CLegendInfo::operator=(const CLegendInfo& info)
{
	m_iLeftIndent = info.m_iLeftIndent;
	m_iRoundRadius = info.m_iRoundRadius;
	m_iInrIconToTx = info.m_iInrIconToTx;
	m_iInrTxToIcon = info.m_iInrTxToIcon;
	m_iOneCharW = info.m_iOneCharW;
	m_clrFrame = info.m_clrFrame;
	m_clrBk = info.m_clrBk;
	m_clrBkEnd = info.m_clrBkEnd;
	m_lnkSingleLgdInfo = info.m_lnkSingleLgdInfo;
}

void CLegendInfo::AddSingleLegendInfo(CSingleLgdInfo& singleLgdInfo)
{
	m_lnkSingleLgdInfo.AddTail(singleLgdInfo);
}

int CLegendInfo::GetSingleLegendCount()
{
	return m_lnkSingleLgdInfo.GetSize();
}

CSingleLgdInfo* CLegendInfo::GetSingleLegendInfo(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_lnkSingleLgdInfo.GetSize())
	{
		return NULL;
	}

	return &(*m_lnkSingleLgdInfo[iIndex]);
}

void CLegendInfo::ResetSegLegendText(CLegendInfo* pLgdInfo)
{
	if(NULL == pLgdInfo)
	{
		return;
	}

	int iLgdCount = pLgdInfo->m_lnkSingleLgdInfo.GetSize();
	if(iLgdCount < 5)
	{
		return;
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	pLgdInfo->m_lnkSingleLgdInfo[0]->m_strTx = pLangInfo->GetText(211);
	pLgdInfo->m_lnkSingleLgdInfo[1]->m_strTx = pLangInfo->GetText(212);
	pLgdInfo->m_lnkSingleLgdInfo[2]->m_strTx = pLangInfo->GetText(215);
	pLgdInfo->m_lnkSingleLgdInfo[3]->m_strTx = pLangInfo->GetText(214);
	pLgdInfo->m_lnkSingleLgdInfo[4]->m_strTx = pLangInfo->GetText(213);

	return;
}

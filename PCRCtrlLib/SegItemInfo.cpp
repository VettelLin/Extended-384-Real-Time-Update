#include "StdAfx.h"
#include "SegItemInfo.h"
#include "PublicFun.h"

CSegItemInfo::CSegItemInfo(void)
{
	m_iStepNum = 0;
	m_iSegType = 1;
	m_iCycleNum = 1;
	m_strSegName = _T("³ÌÐò¶Î");
	m_fMeltRate = 1.0;
	m_nSegType = SEGMENT_NONE;
	m_bSelected = FALSE;
}

CSegItemInfo::~CSegItemInfo(void)
{

}

CSegItemInfo::CSegItemInfo(const CSegItemInfo& segItemInfo)
{
	m_iStepNum = segItemInfo.m_iStepNum;
	m_iSegType = segItemInfo.m_iSegType;
	m_iCycleNum = segItemInfo.m_iCycleNum;
	m_strSegName = segItemInfo.m_strSegName;
	m_lnkStepItemInfo = segItemInfo.m_lnkStepItemInfo;
	m_rcCycleNum = segItemInfo.m_rcCycleNum;
	m_rcSegName = segItemInfo.m_rcSegName;
	m_fMeltRate = segItemInfo.m_fMeltRate;
	m_nSegType = segItemInfo.m_nSegType;
	m_rcSeg = segItemInfo.m_rcSeg;
	m_bSelected = segItemInfo.m_bSelected;
}

void CSegItemInfo::operator=(const CSegItemInfo& segItemInfo)
{
	m_iStepNum = segItemInfo.m_iStepNum;
	m_iSegType = segItemInfo.m_iSegType;
	m_iCycleNum = segItemInfo.m_iCycleNum;
	m_strSegName = segItemInfo.m_strSegName;
	m_lnkStepItemInfo = segItemInfo.m_lnkStepItemInfo;
	m_rcCycleNum = segItemInfo.m_rcCycleNum;
	m_rcSegName = segItemInfo.m_rcSegName;
	m_fMeltRate = segItemInfo.m_fMeltRate;
	m_nSegType = segItemInfo.m_nSegType;
	m_rcSeg = segItemInfo.m_rcSeg;
	m_bSelected = segItemInfo.m_bSelected;
}

BOOL CSegItemInfo::WriteFile(CFile* pFile)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	CPublicFun::WriteStringToFile(pFile, m_strSegName);
	pFile->Write((const char *)&m_iSegType, sizeof(int));
	pFile->Write((const char*)&m_iCycleNum, sizeof(int));
	pFile->Write((const char*)&m_fMeltRate, sizeof(float));
	pFile->Write((const char*)&m_nSegType, sizeof(eSegmentType));

	m_iStepNum = m_lnkStepItemInfo.GetSize();
	pFile->Write((const char*)&m_iStepNum, sizeof(int));

	for(int i = 0;i < m_iStepNum;++i)
	{
		m_lnkStepItemInfo[i]->WriteFile(pFile);
	}

	return TRUE;
}

BOOL CSegItemInfo::ReadFile(CFile* pFile, WORD wVersion)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	m_lnkStepItemInfo.Clear();

	m_strSegName = CPublicFun::ReadStringFromFile(pFile);
	pFile->Read((char*)&m_iSegType, sizeof(int));
	pFile->Read((char*)&m_iCycleNum, sizeof(int));
	pFile->Read((char*)&m_fMeltRate, sizeof(float));
	pFile->Read((char*)&m_nSegType, sizeof(eSegmentType));
	pFile->Read((char*)&m_iStepNum, sizeof(int));

	CStepItemInfo stepItem;
	for(int i = 0;i < m_iStepNum;++i)
	{
		m_lnkStepItemInfo.AddTail(stepItem);
		m_lnkStepItemInfo.GetTail()->data.ReadFile(pFile,wVersion);
	}

	return TRUE;
}

int CSegItemInfo::GetSegWidth(int stepId /*= -2*/)
{
	if((stepId < 0 && -2 != stepId) || -1 == stepId)
	{
		return 0;
	}

	int segWidth = 0;
	Node<CStepItemInfo>* pNode = m_lnkStepItemInfo.GetHead();
	int index = 0;
	int stepNum = stepId;
	if(-2 == stepId)
	{
		stepNum = m_lnkStepItemInfo.GetSize();
	}
	while(NULL != pNode && index++ <= stepNum)
	{
		segWidth += pNode->data.m_stepW;
		pNode = pNode->pNext;
	}
	
	return segWidth;
}

BOOL CSegItemInfo::GetBeMeltSeg()
{
	Node<CStepItemInfo>* pNode = m_lnkStepItemInfo.GetHead();
	while(NULL != pNode)
	{
		if(3 == pNode->data.m_eChkType)
		{
			m_fMeltRate = pNode->data.m_fRate;
			return TRUE;
		}
		pNode = pNode->pNext;
	}
	return FALSE;
}

int CSegItemInfo::GetMeltIncrement(double& dMeltIncrement)
{
	Node<CStepItemInfo>* pNode = m_lnkStepItemInfo.GetHead();
	while(NULL != pNode)
	{
		if(3 == pNode->data.m_eChkType)
		{
			dMeltIncrement = pNode->data.m_fIncrement;
			return 0;
		}
		pNode = pNode->pNext;
	}
	return -1;
}

BOOL CSegItemInfo::GetBeAmlSeg()
{
	Node<CStepItemInfo>* pNode = m_lnkStepItemInfo.GetHead();
	BOOL bHaveAml = FALSE;
	while(NULL != pNode)
	{
		if(3 == pNode->data.m_eChkType)
		{
			return FALSE;
		}
		if(2 == pNode->data.m_eChkType)
		{
			bHaveAml = TRUE;
		}
		pNode = pNode->pNext;
	}
	return bHaveAml;
}

BOOL CSegItemInfo::GetBeAllStepSelected()
{
	Node<CStepItemInfo>* pStepNode = m_lnkStepItemInfo.GetHead();
	while(NULL != pStepNode)
	{
		if(!pStepNode->data.m_bSel)
		{
			return FALSE;
		}
		pStepNode = pStepNode->pNext;
	}
	return TRUE;
}

BOOL CSegItemInfo::GetBeHaveSelectedStep()
{
	Node<CStepItemInfo>* pStepNode = m_lnkStepItemInfo.GetHead();
	while(NULL != pStepNode)
	{
		if(pStepNode->data.m_bSel)
		{
			return TRUE;
		}
		pStepNode = pStepNode->pNext;
	}
	return FALSE;
}

float CSegItemInfo::GetFirstStepTemperature()
{
	float ret = 4;

	if(m_lnkStepItemInfo.GetSize() > 0)
	{
		ret = m_lnkStepItemInfo.GetHead()->data.m_fAimTem;
	}

	return ret;
}

float CSegItemInfo::GetLastStepTemperatre()
{
	float ret = 4;
	int size = m_lnkStepItemInfo.GetSize();
	if(size > 0)
	{
		ret = m_lnkStepItemInfo.GetAt(size - 1)->data.m_fAimTem;
	}

	return ret;
}

int CSegItemInfo::GetStepNum()
{
	return m_lnkStepItemInfo.GetSize();
}

void CSegItemInfo::CorrectNameLanguage(CString strSource, CString strTarget)
{
	m_strSegName.Replace(strSource, strTarget);
}

void CSegItemInfo::CorrectIndex(int iIndex)
{
	CString strIndex;
	strIndex.Format(_T("%d"), iIndex);

	if (m_strSegName.Find(strIndex) == -1)
	{
		m_strSegName += _T(" ") + strIndex;
	}
}

long CSegItemInfo::CalculateTotalSeconds()
{
	long iTime = 0;
	Node<CStepItemInfo>* pStepNode = m_lnkStepItemInfo.GetHead();
	while(NULL != pStepNode)
	{
		iTime += pStepNode->data.m_iDelay;
		pStepNode = pStepNode->pNext;
	}

	return iTime;
}

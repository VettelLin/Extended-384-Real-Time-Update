#include "StdAfx.h"
#include "SamIdSiteRelation.h"

CSamIdSiteRelation::CSamIdSiteRelation(void)
{

}

CSamIdSiteRelation::~CSamIdSiteRelation(void)
{

}

void CSamIdSiteRelation::AddSamIdSite(CString strSamId,int iSiteIndex)
{
#ifdef SAMIDSITEINDEX_USE_MAP_MODE
	map<CString,vector<int>>::iterator iter = m_mapRelation.find(strSamId);
	if(m_mapRelation.end() != iter)
	{
		iter->second.push_back(iSiteIndex);
		return;
	}

	vector<int> vecTemp;
	vecTemp.push_back(iSiteIndex);
	m_mapRelation[strSamId] = vecTemp;
#else
	BOOL bFind = FALSE;
	for(int i = 0;i < m_vecSamId.size();++i)
	{
		if(strSamId == m_vecSamId[i])
		{
			bFind = TRUE;
			m_2vecSiteIndex[i].push_back(iSiteIndex);
			break;
		}
	}

	if(!bFind)
	{
		m_vecSamId.push_back(strSamId);
		vector<int> vecTemp;
		vecTemp.push_back(iSiteIndex);
		m_2vecSiteIndex.push_back(vecTemp);
	}
#endif
	return;
}

void CSamIdSiteRelation::Clear()
{
#ifdef SAMIDSITEINDEX_USE_MAP_MODE
	m_mapRelation.clear();
#else
	m_vecSamId.clear();
	m_2vecSiteIndex.clear();
#endif
	return;
}

int CSamIdSiteRelation::GetRelationCount()
{
#ifdef SAMIDSITEINDEX_USE_MAP_MODE
	return m_mapRelation.size();
#else
	return m_vecSamId.size();
#endif
}

int CSamIdSiteRelation::GetRelationAt(int iRelationIndex,CString& strId,vector<int>& vecSiteIndex)
{
#ifdef SAMIDSITEINDEX_USE_MAP_MODE
	if(iRelationIndex < 0)
	{
		return -1;
	}
	if(iRelationIndex >= m_mapRelation.size())
	{
		return -2;
	}

	int iCount = 0;
	for(map<CString,vector<int>>::iterator iter = m_mapRelation.begin();iter != m_mapRelation.end();++iter,++iCount)
	{
		if(iCount == iRelationIndex)
		{
			strId = iter->first;
			vecSiteIndex = iter->second;
			return 0;
		}
	}
	return -3;
#else
	if(iRelationIndex < 0)
	{
		return -1;
	}
	if(iRelationIndex >= m_vecSamId.size())
	{
		return -2;
	}

	strId = m_vecSamId[iRelationIndex];
	vecSiteIndex = m_2vecSiteIndex[iRelationIndex];

	return 0;
#endif
}

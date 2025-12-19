#pragma once

#include<map>
#include <vector>

using namespace std;

//#define SAMIDSITEINDEX_USE_MAP_MODE	//使用映射结构体保存对应关系

class AFX_EXT_CLASS CSamIdSiteRelation
{
public:
	CSamIdSiteRelation(void);
	~CSamIdSiteRelation(void);

	void AddSamIdSite(CString strSamId,int iSiteIndex);	//添加样本ID、孔位下标组合信息
	void Clear();	//清空map

	int GetRelationCount();
	int GetRelationAt(int iRelationIndex,CString& strId,vector<int>& vecSiteIndex);

public:

#ifdef SAMIDSITEINDEX_USE_MAP_MODE
	map<CString,vector<int>> m_mapRelation;
#else
	vector<CString> m_vecSamId;
	vector<vector<int>> m_2vecSiteIndex;
#endif
};


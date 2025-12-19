#pragma once

#include "SelfDefType.h"

class CSamNameAutoSetInfo
{
public:
	CSamNameAutoSetInfo(void);
	~CSamNameAutoSetInfo(void);

	CSamNameAutoSetInfo(const CSamNameAutoSetInfo& samIdAutoSetInfo);
	void operator=(const CSamNameAutoSetInfo& samIdAutoSetInfo);

public:
	CString m_strPrjName;
	CString m_strSamIdPrefix;
	int m_startValue;
	int m_interval;
	int m_bitNum;
	ESAMSORTWAY m_eSamSortWay;
};


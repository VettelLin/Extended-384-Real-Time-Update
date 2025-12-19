#pragma once

#include "SelfDefType.h"

class CSetInfo
{
public:
	CSetInfo(void);
	~CSetInfo(void);

	CSetInfo(const CSetInfo& setInfo);
	void operator=(const CSetInfo& setInfo);

public:
	ESAMSORTWAY m_eSamSortWay;
	ESAMSORTDIRECTION m_eSamSortDirection;
};


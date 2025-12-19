#pragma once

#include "SelfDefType.h"

typedef enum DilutionDirection{
	EDD_INCREASE,EDD_DECREASE
}EDILUTION_DIRECTION;

class CCalibAutoSetInfo
{
public:
	CCalibAutoSetInfo(void);
	~CCalibAutoSetInfo(void);

	CCalibAutoSetInfo(const CCalibAutoSetInfo& calibAutoSetInfo);
	void operator=(const CCalibAutoSetInfo& calibAutoSetInfo);

public:
	int m_selectHoleNum;	//选中孔位个数
	int m_repeatNum;	//复管个数
	double m_dStartDensity;
	EDILUTION_DIRECTION m_eDilutionDirection;
	int m_dilutionMulitiples;	//稀释倍数
	ESAMSORTWAY m_eSamSortWay;
};


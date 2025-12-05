#pragma once

#include<vector>
#include "MyPoint.h"
#include "LinkList.h"
#include "LinearFitData.h"

using namespace std;

class CStdLineTag
{
public:
	CStdLineTag();
	~CStdLineTag();

	CStdLineTag(const CStdLineTag& stdLineTag);
	void operator=(const CStdLineTag& stdLineTag);
	BOOL operator==(const CStdLineTag& stdLineTag);

public:
	int m_iChanId;
};

class CStdLine
{
public:
	CStdLine();
	~CStdLine();

	CStdLine(const CStdLine& stdLine);

	void operator=(CStdLine& stdLine);

	int LinearFit();
	int CalculateAverageDot();

	void Clear();

public:
	CStdLineTag m_stdLineTag;

	vector<double> m_vecDensity;
	vector<vector<double>> m_2vecCt;

	CLinearFitData m_linearFitData;
	double m_dEfficiency;	//扩增效率
};

class CStdProc
{
public:
	CStdProc(void);
	~CStdProc(void);

	void AddPoint(CStdLineTag stdLineTag,double dX,double dY);
	void Clear();
	int DeleteOneStdLine(CStdLineTag stdLineTag);

	CStdLine* GetStdLinePtr(CStdLineTag stdLineTag);
	CStdLine* GetStdLinePtr(int iIndex);

	int GetStdLineNum();

	void Fit();	//拟合所有曲线

public:
	vector<CStdLine> m_vecStdLine;
};


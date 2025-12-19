#pragma once


#include <vector>

using namespace std;

class AFX_EXT_CLASS CTubeFullFluInfo
{
public:
	CTubeFullFluInfo(void);
	~CTubeFullFluInfo(void);

	BOOL BeAllAllocated();	//是否都成功申请了内存空间
	void Clear();

public:
	vector<UINT>* m_pOriginFlu;
	vector<double>* m_pCrossTalkedFlu;
	vector<double>* m_pFilteredFlu;
	vector<double>* m_pNormalizedFlu;
	vector<double>* m_pLogNormalizedFlu;
	vector<double>* m_pDeltaRnedFlu;
	vector<double>* m_pLogDeltaRnedFlu;
	vector<double>* m_pDMeltTem;
	double* m_pCtBySecDev;	//二阶导方式计算出的Ct值
	double* m_pCtBySpline;	//插值法方式计算出的Ct值
	double* m_pSplineThreNormalize;	//插值法默认阈值(归一化)
	double* m_pLogSplineThreNorm;	//插值法对数阈值（归一化）
	double* m_pSplineThreDeltaRn;	//插值法默认阈值(DeltaRn)
	double* m_pLogSplineThreDeltaRn;	//插值法对数阈值（DeltaRn）
	BOOL* m_pBePeakCrv;	//标识是否是扩增曲线
};


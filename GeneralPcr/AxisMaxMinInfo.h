#pragma once
class CAxisMaxMinInfo
{
public:
	CAxisMaxMinInfo(void);
	~CAxisMaxMinInfo(void);

	CAxisMaxMinInfo(const CAxisMaxMinInfo& axisMaxMinInfo);
	void operator=(const CAxisMaxMinInfo& axisMaxMinInfo);

public:
	double m_dXMax;
	double m_dXMin;
	double m_dYMax;
	double m_dYMin;
};


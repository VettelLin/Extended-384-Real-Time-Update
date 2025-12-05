#pragma once
class CLinearFitData
{
public:
	CLinearFitData(void);
	~CLinearFitData(void);

	CLinearFitData(const CLinearFitData& data);
	void operator=(const CLinearFitData& data);
	BOOL operator==(const CLinearFitData& data);

	void Clear();
	
	int SetDataNum(int iDataNum);

	int LeastSquare();

public:
	double* m_pDX;
	double* m_pDY;
	
	double m_dSlope;
	double m_dIntercept;
	double m_dSquare;

	int m_iDataNum;
};


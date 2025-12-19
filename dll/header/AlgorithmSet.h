#pragma once

//#define in
//#define out

typedef struct LinearFitData
{
	double* pDataX;
	int xLen;
	double* pDataY;
	int yLen;
	double intercept;
	double rSquare;
	double slope;

	LinearFitData()
	{
		pDataX = nullptr;
		xLen = 0;
		pDataY = nullptr;
		yLen = 0;
		intercept = 0;
		rSquare = 0;
		slope = 0;
	}

	~LinearFitData()
	{
		if(nullptr != pDataX)
		{
			delete pDataX;
		}
		if(nullptr != pDataY)
		{
			delete pDataY;
		}
	}
}LINEARFITDATA;

typedef struct ParabolaFitData
{
	double* pDataX;
	int xLen;
	double* pDataY;
	int yLen;
	double a;
	double b;
	double c;
	double dR;

	ParabolaFitData()
	{
		pDataX = nullptr;
		xLen = 0;
		pDataY = nullptr;
		yLen = 0;
		a = 0;
		b = 0;
		c = 0;
		dR = 0;
	}

	~ParabolaFitData()
	{
		if(nullptr != pDataX)
		{
			delete pDataX;
		}
		if(nullptr != pDataY)
		{
			delete pDataY;
		}
	}
}PARABOLAFITDATA;

extern "C" __declspec(dllexport) int LinearFit(LINEARFITDATA* pFitData);
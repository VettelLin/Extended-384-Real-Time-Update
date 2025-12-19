// Algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Algorithm.h"

double Em[6][4];

//int _tmain(int argc, _TCHAR* argv[])
//{
//	double ary1[5] = {0,0.25,0,5,0.75};
//	double ary2[5] = {1,1.283,1.649,2.212,2.178};
//
//	double coefficient[5];
//	memset(coefficient,0,sizeof(double) * 5);
//	vector<double> vecX,vecY;
//	for(int i = 0;i < 5;++i)
//	{
//		vecX.push_back(ary1[i]);
//		vecY.push_back(ary2[i]);
//	}
//
//	EMatrix(vecX,vecY,5,4,coefficient);
//	printf("拟合方程为：y = %lf + %lfx + %lfx^2 + %lfx^3\n",coefficient[1],coefficient[2],coefficient[3],coefficient[4]);
//
//	return 0;
//}

double Sum(vector<double> vecData)
{
	double dSum = 0;
	for(int i = 0;i < vecData.size();++i)
	{
		dSum += vecData[i];
	}
	return dSum;
}

double MultiSum(vector<double> vecX,vector<double> vecY)
{
	double dMultiSum = 0;
	for(int i = 0;i < vecX.size();++i)
	{
		dMultiSum += vecX[i] * vecY[i];
	}
	return dMultiSum;
}

double RelatePow(vector<double> vecX,int n,int ex)
{
	double dReSum = 0;
	for(int i = 0;i < n;++i)
	{
		dReSum += pow(vecX[i],ex);
	}
	return dReSum;
}

double RelateMultiXY(vector<double> vecX,vector<double> vecY,int n,int ex)
{
	double dReMulitSum = 0;
	for(int i = 0;i < n;++i)
	{
		dReMulitSum += pow(vecX[i],ex) * vecY[i];
	}
	return dReMulitSum;
}

void EMatrix(vector<double> vecX,vector<double> vecY,int n,int ex,double coefficient[])
{
	for(int i = 1;i <= ex;++i)
	{
		for(int j = 1;j <= ex;++j)
		{
			Em[i][j] = RelatePow(vecX,n,i + j - 2);
		}
		Em[i][ex + 1] = RelateMultiXY(vecX,vecY,n,i - 1);
	}
	Em[1][1] = n;
	CalEquation(ex,coefficient);
}

void CalEquation(int exp,double coefficient[])
{
	for(int k = 1;k < exp;++k)	//消元过程
	{
		for(int i = k + 1;i < exp + 1;++i)
		{
			double dP1 = 0;
			if(0 != Em[k][k])
			{
				dP1 = Em[i][k] / Em[k][k];
			}

			for(int j = k;j < exp + 2;++j)
			{
				Em[i][j] = Em[i][j] - Em[k][j] * dP1;
			}
		}
	}

	coefficient[exp] = Em[exp][exp + 1] / Em[exp][exp];
	for(int l = exp - 1;l >= 1;--l)
	{
		coefficient[l] = (Em[1][exp + 1] - F(coefficient,l + 1,exp)) / Em[l][l];
	}
}

double F(double c[],int l,int m)
{
	double dSum = 0;
	for(int i = 1;i <= m;++i)
	{
		dSum += Em[i - 1][i] * c[i];
	}
	return dSum;
}


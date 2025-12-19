#include "StdAfx.h"
#include "DataAlgorithmPCR.h"
#include "PcrAnaSort.h"
#include <math.h>
#include "Fitting.h"


CDataAlgorithmPCR::CDataAlgorithmPCR(void)
{
	m_dd2MaxXThre = 1;
	m_dd2MaxYThre = 2;
}

CDataAlgorithmPCR::~CDataAlgorithmPCR(void)
{
}

//数字滤波器
void CDataAlgorithmPCR::DigitalFilter(double *x, double *dReturn, int nNum, eDigitalFilterType eType, int nWnd /*= 3*/)
{
	switch(eType)
	{
	case FILTERTYPE_CubicSmooth_5:
		 Kdspt(nNum,x,dReturn);
		 break;
	case FILTERTYPE_CubicSmooth_7:
		Kdspt7_3(nNum,x,dReturn);
		break;
	case FILTERTYPE_QuadraticSmooth_5:
		Kdspt5_2(nNum,x,dReturn);
		break;
	case FILTERTYPE_QuadraticSmooth_7:
		Kdspt7_2(nNum,x,dReturn);
		break;
	case FILTERTYPE_TriangularSmooths_29:
		Kdspt_TriangularSmooths_29(nNum,x,dReturn);
		break;
	case FILTERTYPE_GaussianSmooths_31:
		Kdspt_GaussianSmooths_31(nNum,x,dReturn);
		break;
	case FILTERTYPE_DTMEAN3:
		Kdspt_dtmean(nNum,3,x,dReturn);
		break;
	case FILTERTYPE_DTMEAN4:
		Kdspt_dtmean(nNum,4,x,dReturn);
		break;
	case FILTERTYPE_DTMEAN5:
		Kdspt_dtmean(nNum,5,x,dReturn);
		break;
	case FILTERTYPE_DTMEAN6:
		Kdspt_dtmean(nNum,6,x,dReturn);
		break;
	case FILTERTYPE_DTMEAN7:
		Kdspt_dtmean(nNum,7,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN3:
		Kdspt_cfmean(nNum,3,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN5:
		Kdspt_cfmean(nNum,5,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN7:
		Kdspt_cfmean(nNum,7,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN9:
		Kdspt_cfmean(nNum,9,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN11:
		Kdspt_cfmean(nNum,11,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN13:
		Kdspt_cfmean(nNum,13,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN15:
		Kdspt_cfmean(nNum,15,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN17:
		Kdspt_cfmean(nNum,17,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN19:
		Kdspt_cfmean(nNum,19,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN21:
		Kdspt_cfmean(nNum,21,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN23:
		Kdspt_cfmean(nNum,23,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN25:
		Kdspt_cfmean(nNum,25,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN27:
		Kdspt_cfmean(nNum,27,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN29:
		Kdspt_cfmean(nNum,29,x,dReturn);
		break;
	case FILTERTYPE_CFMEAN31:
		Kdspt_cfmean(nNum,31,x,dReturn);
		break;
	case FILTERTYPE_MEDIAN:
		Kdspt_Median(nNum,nWnd,x,dReturn);
		break;
	default:
		Kdspt(nNum,x,dReturn);
		break;
	}
}

void CDataAlgorithmPCR::KdsptForwardMBackN(int n, double *pY, double *pReturn,int forwardM,int backwardN)
{
	//测试五点中心滤波尾端不收缩
	if(forwardM == backwardN && 2 == forwardM)
	{
		KdsptForwardMBackN_NoShrink(n,pY,pReturn,forwardM,backwardN);
		return;
	}

	if(NULL == pY || NULL == pReturn)
	{
		return;
	}

	if(forwardM < 0 || backwardN < 0)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	if(n < forwardM + backwardN + 1)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	int start = 0;
	int end = 0;
	int count = 0;
	double dSum = 0;
	for(int i = 0;i < n;++i)
	{
		dSum = 0;
		count = 0;
		if(i < forwardM)
		{
			start = 0;
			end = i + backwardN;
		}
		else if(i >= forwardM && i < n - backwardN)
		{
			 start = i - forwardM;
			 end = i + backwardN;
		}
		else
		{
			start = i - forwardM;
			end = n - 1;
		}

		if(forwardM == backwardN)
		{
			int nPre = i - start ;
			int nBehind = end - i;
			(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
		}
		else
		{
			//if(i < forwardM)
			//{
			//	int nPre = i - start ;
			//	int nBehind = end - i;
			//	(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
			//}
			//else if(i > (n - 1 - backwardN))
			//{
			//	int nPre = i - start ;
			//	int nBehind = end - i;
			//	(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
			//	//start = end = i;
			//}

			/*if(i == n - 1)
			{
			start = end = i;
			}*/
		}

		for(int k = start;k <= end;++k)
		{
			dSum += pY[k];
			++count;
		}

		if(forwardM != backwardN)
		{
			if(i > (n - 1 - backwardN))
			{
				for(int k = i + backwardN;k > n - 1;--k)
				{
					dSum += pY[n - 1];
					++count;
				}
			}
		}

		if(0 != count)
		{
			pReturn[i] = dSum / count;
		}
		else
		{
			pReturn[i] = pY[i];
		}
	}

	return;
}

void CDataAlgorithmPCR::KdsptForwardMBackN_New(int n, double *pY, double *pReturn,int forwardM,int backwardN)
{
	if(NULL == pY || NULL == pReturn)
	{
		return;
	}

	if(forwardM < 0 || backwardN < 0)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	if(n < forwardM + backwardN + 1)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	int start = 0;
	int end = 0;
	int count = 0;
	double dSum = 0;
	for(int i = 0;i < n;++i)
	{
		dSum = 0;
		count = 0;
		if(i < forwardM)
		{
			start = 0;
			end = i + backwardN;
		}
		else if(i >= forwardM && i < n - backwardN)
		{
			 start = i - forwardM;
			 end = i + backwardN;
		}
		else
		{
			start = i - forwardM;
			end = n - 1;
		}

		if(forwardM == backwardN)
		{
			int nPre = i - start ;
			int nBehind = end - i;
			(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
		}
		else
		{
			//if(i < forwardM)
			//{
			//	int nPre = i - start ;
			//	int nBehind = end - i;
			//	(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
			//}
			//else if(i > (n - 1 - backwardN))
			//{
			//	int nPre = i - start ;
			//	int nBehind = end - i;
			//	(nPre > nBehind) ?(start = i - nBehind):(end = i + nPre);
			//	//start = end = i;
			//}

			/*if(i == n - 1)
			{
			start = end = i;
			}*/
		}

		for(int k = start;k <= end;++k)
		{
			dSum += pY[k];
			++count;
		}

		//使用最后一个数据扩充数据队列用于滤波
		if(forwardM != backwardN)
		{
			if(i > (n - 1 - backwardN))
			{
				for(int k = i + backwardN;k > n - 1;--k)
				{
					dSum += pY[n - 1];
					++count;
				}
			}
		}

		if(0 != count)
		{
			pReturn[i] = dSum / count;
		}
		else
		{
			pReturn[i] = pY[i];
		}
	}

	return;
}


void CDataAlgorithmPCR::KdsptForwardMBackN_NoShrink(int n, double *pY, double *pReturn,int forwardM,int backwardN)
{
	if(NULL == pY || NULL == pReturn)
	{
		return;
	}

	if(forwardM < 0 || backwardN < 0)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	if(n < forwardM + backwardN + 1)
	{
		memcpy(pReturn,pY,n * sizeof(double));
		return;
	}

	int start = 0;
	int end = 0;
	int count = 0;
	double dSum = 0;
	for(int i = 0;i < n;++i)
	{
		dSum = 0;
		count = 0;
		if(i < forwardM)
		{
			start = 0;
			end = i + backwardN;
		}
		else if(i >= forwardM && i < n - backwardN)
		{
			 start = i - forwardM;
			 end = i + backwardN;
		}
		else
		{
			start = i - forwardM;
			end = n - 1;

			if(2 == backwardN)
			{
				if(n - 2 == i)
				{
					start = i - 1;
					end = n - 1;
				}
				else if(n - 1 == i)
				{
					start = i - 1;
					end = n - 1;
				}
			}
		}

		for(int k = start;k <= end;++k)
		{
			dSum += pY[k];
			++count;
		}

		if(0 != count)
		{
			pReturn[i] = dSum / count;
		}
		else
		{
			pReturn[i] = pY[i];
		}
	}

	return;
}

//根据确定标准方差计算ct值，并生成标准曲线
void CDataAlgorithmPCR::GenStdCurveByStd(int nCurveNum,int nPtNum, int nStar, int nEnd, int nMultiple,double *dX, double *dY,double *dCt,double *dLinePara,bool bStdCurve,bool bBaseLine)
{
	if( nPtNum <= nEnd)
	{
		for(int i=0; i< nCurveNum; i++)
			dCt[i] = 0;
		return;
	}
	if ( nEnd -nStar <= 0)
		return;
	double *dScPoints = new double [nCurveNum];
	double xCur = 0, yCur = 0;
	double *x = new double [nPtNum];
	double *y = new double [nPtNum];
	double *temp = new double[nPtNum];
	for(int i=0; i<nCurveNum; i++)
	{
		for(int j=0; j< nPtNum; j++)
		{
			x[j] = dX[i*nPtNum+j];
			y[j] = dY[i*nPtNum+j];
		} 
		if( !bBaseLine)
		{
			double dAvg = 0;
			SortDouble(nPtNum, y, temp);
			for(int k=0; k< 5; k++)
				dAvg += temp[k];
			dAvg /= 5;
			for(int j=0; j< nPtNum; j++)
				y[j] -= dAvg;
		}
		double avg =0, std =0;
		for(int j= nStar; j< nEnd; j++)
			avg += y[j];
		avg /= (nEnd - nStar+1);
		for(int j= nStar; j< nEnd; j++)
			std += (y[j]-avg)*(y[j]-avg);
		yCur =nMultiple* sqrt(std/(nEnd - nStar));

		double dMin = y[0], dMax = y[0];
		int nMin = 0, nMax = nPtNum-1;
		for(int j=1; j< nPtNum; j++)
		{
			dMin = min(dMin, y[j]);
			if( y[j] < dMin) nMin = j;
			dMax = max(dMax,y[j]);
			if( y[j] > dMax) nMax = j;
		}
		if( yCur>dMin && yCur<dMax)
            SplineEx(y,x,nPtNum,yCur,xCur);
		else if( yCur <= dMin)
			xCur = x[nMin];
		else if( yCur >= dMax)
			xCur = x[nMax];
		dScPoints[i] = xCur;
	}
	for(int i =0; i< nCurveNum; i++)
		dCt[i] = dScPoints[i];

	if( bStdCurve)
	{
		double *sx = new double [nCurveNum];
		double *sy = new double [nCurveNum];
		for(int i=0; i<nCurveNum; i++)
		{
			sx[i] = nCurveNum-i+1;
			sy[i] = dScPoints[i];
		} // for( i=0; i<nCurveNum; i++)

		double dReCof[2] = {0};
		//最小二乘法拟合标准曲线
		LinearFit(sx,sy,nCurveNum,dReCof);
		dLinePara[0] = dReCof[0];
		dLinePara[1] = dReCof[1];
		delete [] sx;
		delete [] sy;

	}
	delete [] dScPoints;
	delete [] x;
	delete [] y;
	delete [] temp;
}

void CDataAlgorithmPCR::CalDerivative(double *x, double *y, int nNum,int nDev,double *dReturn)
{
    int n=nNum; 
    double *dy = new double [n];
	double *ddy = new double [n];
	if( x[0] == x[1])
		dy[0] = 0;
	else
		dy[0] = (y[1]-y[0])/(x[1]-x[0]);
	if( x[n-2] == x[n-1])
		dy[n-1] = 0;
	else
		dy[n-1] = (y[n-1]-y[n-2])/(x[n-1]-x[n-2]);
    Amspl(x,y,n,dy,ddy);

    if(nDev == 1)
	{
		for(int i = 0; i< n; i++)
			dReturn[i] = dy[i];
	}
	else if(nDev == 2)
	{
		for(int i = 0; i< n; i++)
			dReturn[i]= ddy[i];
	}

	delete [] dy;
	delete [] ddy;
}

void CDataAlgorithmPCR::NewCalDerivative(double *x, double *y, int nNum,int nDev,double *dReturn)
{
	if(nNum < 1 || NULL == x || NULL == y)
	{
		return;
	}

	int n=nNum; 
	double *dy = new double [n];
	double *ddy = new double [n];

	dy[n - 1] = 0;
	for(int i = 0;i < n - 1;++i)
	{
		dy[i] = y[i + 1] - y[i];
	}

	////求一阶导数后进行五点三次滤波（首尾不收缩）
	//double* pReturn = new double[n];
	//memset(pReturn,0,sizeof(double) * n);
	//KdsptForwardMBackN_NoShrink(n,dy,pReturn,2,2);
	//memcpy(dy,pReturn,sizeof(double) * n);

	ddy[0] = 0;
	ddy[n - 1] = 0;
	for(int i = 1;i < n - 1;++i)
	{
		ddy[i] = dy[i] - dy[i - 1];
	}

	////求二阶导数后进行五点三次滤波（首尾不收缩）
	//memset(pReturn,0,sizeof(double) * n);
	//KdsptForwardMBackN_NoShrink(n,ddy,pReturn,2,2);
	//memcpy(ddy,pReturn,sizeof(double) * n);

	if(nDev == 1)
	{
		for(int i = 0; i< n; i++)
			dReturn[i] = dy[i];
	}
	else if(nDev == 2)
	{
		for(int i = 0; i< n; i++)
			dReturn[i]= ddy[i];
	}

	//delete [] pReturn;

	delete [] dy;
	delete [] ddy;
}

/*
void CDataAlgorithmPCR::Spline(double *x, double *y, int nNum, double xCur,double &yCur)
{
	int k = -1;
    static double s[5];
	Akspl(x,y,nNum,k,xCur,s,1);
	yCur = s[4];
}
*/
void CDataAlgorithmPCR::Spline(double *x, double *y, int nNum, double xCur,double &yCur)
{
	if( nNum < 1)
		return;
	int k = -1;
	static double s[5];

	double dXMin = x[0]; 
	int nIndex = 0;
	//for( int i=1; i< nNum; i++)
	//{
	//	dXMin = x[i] < dXMin ? x[i],nIndex = i : dXMin;
	//}
	
	int nTempNum = nNum - nIndex;
	double *dX = new double[nTempNum];
	double *dY = new double[nTempNum];
	for(int i=0; i< nTempNum; i++)
	{
		dX[i] = x[i+nIndex];
		dY[i] = y[i+nIndex];
	}
	Akspl(dX,dY,nTempNum,k,xCur,s,1);
	yCur = s[4];
	delete [] dX;   
	delete [] dY;
}

void CDataAlgorithmPCR::SplineEx(double *x, double *y, int nNum, double xCur,double &yCur)
{
	if( nNum < 1)
		return;
	int k = -1;
	static double s[5];
	memset(s, 0, sizeof(double)*5);

	double dXMin = x[0];
	int nIndex = 0;
	//for( int i=1; i< nNum; i++)
	//{
	//	dXMin = x[i] < dXMin ? x[i],nIndex = i : dXMin;
	//}

	int nTempNum = nNum - nIndex;
	double *dX = new double[nTempNum];
	double *dY = new double[nTempNum];
	for(int i=0; i< nTempNum; i++)
	{
		dX[i] = x[i+nIndex];
		dY[i] = y[i+nIndex];
	}
	AksplEx(dX,dY,nTempNum,k,xCur,s,1);
	yCur = s[4];
	delete [] dX;   
	delete [] dY;
}

void CDataAlgorithmPCR::LinearFit(double *x, double *y,int nNum, double *dReturn)
{
	double dt[6],a[2];
	Jbsqt(x,y,nNum,a,dt);
	dReturn[0] = a[0];
	dReturn[1] = a[1];

	/////////////////R2////////////////////
	double xMean = 0, yMean = 0;
	for(int i=0; i<nNum; i++)
	{
		xMean += x[i];
		yMean += y[i];
	} // for(int i=0; i<nNum; i++)
	xMean /= nNum;
	yMean /= nNum;
	double aUp =0, bDown=0, cDown =0;
	for(int i=0; i<nNum; i++)
	{
		aUp += abs((x[i]-xMean)*(y[i]-yMean));
		bDown += (x[i]-xMean)*(x[i]-xMean);
		cDown += (y[i]-yMean)*(y[i]-yMean);
	} // for(i=0; i<nNum; i++)

	//////////////////R///////////////////////
	double dSumXY = 0,dSumX = 0, dSumY = 0, dSumX2 = 0, dSumY2 = 0;
	for(int i=0; i< nNum; i++)
	{
		dSumXY += x[i]*y[i];
		dSumX += x[i];
		dSumY += y[i];
		dSumX2 += x[i]*x[i];
		dSumY2 += y[i]*y[i];
	}
	double dUp = nNum*dSumXY - dSumX * dSumY;
	double dDown = sqrt( nNum*dSumX2 - dSumX*dSumX) * sqrt( nNum*dSumY2 - dSumY*dSumY);

	//////////////////////////////////////////
	//	if( bDown!= 0 && cDown!= 0)
	//		dReturn[2] = aUp/(sqrt(bDown*cDown));
	if( dDown != 0)
		dReturn[2] = dUp /dDown;
	else
		dReturn[2] = 0;
}

/**
 * 一阶负导数
 * \param nCurveNum 
 * \param nPtNum 
 * \param *dX 
 * \param *dY 
 * \param *dYReturn 
 */
void CDataAlgorithmPCR::FirstNegDerivative(int nCurveNum, int nPtNum, double *dX, double *dY, double *dXReturn,double *dYReturn)
{
	double *x = new double [nPtNum];
	double *y = new double [nPtNum];
	double *dFiltered = new double [nPtNum];
	double *dReturn = new double[nPtNum];
	for(int i=0; i<nCurveNum; i++)
	{
		for(int j=0; j< nPtNum; j++)
		{
			x[j] = dX[i*nPtNum+j];
			y[j] = dY[i*nPtNum+j];
		}
		//两次滤波
		DigitalFilter(x,dFiltered,nPtNum, FILTERTYPE_CFMEAN5);
		DigitalFilter(dFiltered,x,nPtNum,FILTERTYPE_CFMEAN5);

		DigitalFilter(y,dFiltered,nPtNum,FILTERTYPE_CFMEAN5);
		DigitalFilter(dFiltered,y,nPtNum,FILTERTYPE_CFMEAN5);

		//计算一阶导数
		CalDerivative(x,y,nPtNum,1,dReturn);


		DigitalFilter(x,dFiltered,nPtNum,FILTERTYPE_CFMEAN5);
		DigitalFilter(dFiltered,dXReturn,nPtNum,FILTERTYPE_CFMEAN5);

		DigitalFilter(dReturn,dFiltered,nPtNum,FILTERTYPE_CFMEAN5);
		DigitalFilter(dFiltered,dReturn,nPtNum,FILTERTYPE_CFMEAN5);


		
		for(int j=0; j< nPtNum; j++)
			dYReturn[i*nPtNum +j] = -dReturn[j];
	}
	delete [] dReturn;
	delete [] dFiltered;
	delete [] x;
	delete [] y;
}
/**
 * 数字滤波
 * \param nCurveNum 
 * \param nPtNum 
 * \param *dX 
 * \param *dY 
 * \param *dYReturn 
 */
void CDataAlgorithmPCR::DataDigitalFilter(int nCurveNum, int nPtNum, double *dX, double *dY, double *dYReturn)
{
	double *y = new double [nPtNum];
	double *dFiltered = new double [nPtNum];
	for(int i=0; i<nCurveNum; i++)
	{
		for(int j=0; j< nPtNum; j++)
		{
			y[j] = dY[i*nPtNum+j];
		}
		DigitalFilter(y,dFiltered,nPtNum);
		DigitalFilter(dFiltered,y,nPtNum);
		
		for(int j=0; j< nPtNum; j++)
			dYReturn[i*nPtNum +j] = y[j];
	}
	delete [] dFiltered;
	delete [] y;
}
/**
 * 数据平滑
 * \param nCurveNum： 曲线个数
 * \param nPtNum： 每条曲线点数
 * \param nWidth ：平滑窗宽度
 * \param *dY ：待处理数据
 * \param *dYReturn ：返回指针
 */
void CDataAlgorithmPCR::DataSmooth(int nCurveNum, int nPtNum, int nWidth, double *dY, double *dYReturn)
{
	if( nPtNum < nWidth || nWidth<= 1)
	{
		for(int i=0; i< nCurveNum; i++)
			for(int j=0; j< nPtNum; j++)
				dYReturn[i*nPtNum +j] = dY[j];
		return;
	}
	double *y = new double [nPtNum];
	double *dFiltered = new double [nPtNum];
	int nStar =0, nEnd = nPtNum;
	if( nWidth%2)
	{
		nStar = nWidth/2;
		nEnd = nPtNum - nStar;
	} // if( nWith%2)
	else
	{
		nStar = (nWidth-1)/2;
		nEnd = nPtNum - nStar-1;
	}

	for(int i=0; i<nCurveNum; i++)
	{
		for(int j=0; j< nPtNum; j++)
		{
			y[j] = dY[i*nPtNum+j];
		}
		for(int j=nStar; j< nEnd; j++)
		{
			double sum = 0;
			for( int k=-nStar; k< nWidth-nStar-1; k++)
				sum += y[j +k];
			dYReturn[i*nPtNum +j] = sum/nWidth;
		} // for( j=nStar; j< nEnd; j++)
		for(int j = 0; j< nStar; j++)
			dYReturn[i*nPtNum +j] = y[j];
		for(int j = nEnd; j< nPtNum; j++)
			dYReturn[i*nPtNum +j] = y[j];

	}
	delete [] dFiltered;
	delete [] y;
}
/**
 * 自动搜索峰值
 * \param nCrestNum ：待寻找峰的个数
 * \param nPtNum 
 * \param *dXValue 
 * \param *dYValue 
 * \param *dReturn 
 */
void CDataAlgorithmPCR::AutoFindCrest(int nCrestNum,int nPtNum, double *dXValue, double *dYValue, double *dReturn,double dThrePercent /* = 0.2 */)
{
	CArray<int,int>  arrayTemp;
	arrayTemp.RemoveAll();
	double temp[3] = {0};
	for(int i=1; i<nPtNum-2; i++)
	{
		for(int j=0; j< 3; temp[j++] = 0);
        temp[0] = dYValue[i]- dYValue[i-1];
		temp[1] = dYValue[i]- dYValue[i+1];
		temp[2] = dYValue[i]- dYValue[i+2];
		if( temp[0] >0 && temp[1] >0  && temp[2]>0)
			arrayTemp.Add(i);
	}
	int nSize = (int) arrayTemp.GetSize();
	double *dTemp = new double[nSize];
	double *dOut = new double[nSize];
	for(int i=0; i< nSize; i++)
		dTemp[i] = dYValue[arrayTemp.GetAt(i)];
	SortDouble(nSize,dTemp,dOut);
	//进一步处理，以最高峰值的20%(缺省)作为阈值线，高于阈值线的即为有效峰
	float fThre = 0;
	if (nSize >= 1)
	{
		fThre = float(dOut[nSize -1] * dThrePercent);
	}
	
	//如果找到多个峰，过滤掉峰值比较小的数,20200225,孙文郎
	for (int i = 0; i < nSize; i++)
	{
		if (dOut[i] < fThre)
			dOut[i] = -1;
	}
	int count = 0;
	for(int i=nSize-1; i>= nSize-nCrestNum && i >= 0; i--)
	{
		bool bfind = false;
		for(int j=0; j< nSize; j++)
		{
			if( dYValue[arrayTemp.GetAt(j)] == dOut[i])
			{
				bfind = true;
				dReturn[count++] = dXValue[arrayTemp.GetAt(j)];
				if(count >= nCrestNum)
				{
					goto SAMCRESTEXIST;	//两个峰值相同时，容易触发这个问题
				}
			}
		}
		if (!bfind)
		{
			dReturn[count++] = -1;
		}
	}

#ifdef _DEBUG
	if(count > 1)
	{
		int mm = 0;
		++mm;
	}
#endif

SAMCRESTEXIST:
	if(nCrestNum > nSize)
	{
		for(int i=nSize; i<nCrestNum; i++)
			dReturn[i] = -1;
	}
 
    delete [] dTemp;
	delete [] dOut;
}
/**
 * 排序
 * \param nNum 
 * \param *pInput 
 * \param *pOutput 
 */
void CDataAlgorithmPCR::SortDouble(int nNum, double *pInput, double *pOutput)
{
	int sType = 3;
	CPcrAnalysisSort<double> s(nNum);		
	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i];
	s.SelectSortType(sType, pOutput);
}

/**
 * 计算△Rn(减去前nAvgNum个数据的均值)
 * \param nNum 
 * \param nAvgNum 
 * \param *pInput 
 * \param *pOutput 
 */
void CDataAlgorithmPCR::CalculatedDeltaRn(int nNum, int nStart,int nEnd, double *pInput, double *pOutput)
{
	int nAvgNum = nEnd-nStart+1; 
	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i];
		return;	
	}
	double sum = 0;
	for(int i=nStart-1; i< nEnd; i++)
		sum += pInput[i];
	sum /= nAvgNum;

	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i] - sum;
}


void CDataAlgorithmPCR::SortInt(int nNum, int *pInput, int *pOutput)
{
	int sType = 3;
	CPcrAnalysisSort<int> s(nNum);		
	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i];
	s.SelectSortType(sType, pOutput);
}
/**
 * 调整基线
 * \param nNum 
 * \param nAvgNum 
 * \param *pInput 
 * \param *pOutput 
 */
void CDataAlgorithmPCR::AdjustBaseLine(int nNum, int nAvgNum, double *pInput, double *pOutput)
{
	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i];
		return;	
	}
	SortDouble(nNum, pInput, pOutput);
	double sum = 0;
	for(int i=0; i< nAvgNum; i++)
		sum += pOutput[i];
	sum /= nAvgNum;

	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i] - sum;
}

/**
 * 调整基线
 * \param nNum 
 * \param nAvgNum 
 * \param *pInput 
 * \param *pOutput 
 */
void CDataAlgorithmPCR::NewAdjustBaseLine(int nNum, int start,int end, double *pInput, double *pOutput)
{
	if(start > end || start >= nNum || end >= nNum)
	{
		for(int i = 0;i < nNum;++i)
			pOutput[i] = pInput[i];
		return;	
	}

	int nAvgNum = end - start + 1;

	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i = 0;i < nNum;++i)
		pOutput[i] = pInput[i];
		return;	
	}
	//SortDouble(nNum, pInput, pOutput);
	double sum = 0;
	for(int i = start - 1;i < end;++i)
		sum += pInput[i];
	sum /= nAvgNum;

	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i] - sum;
}

/**
 * 调整基线
 * \param nNum 
 * \param nAvgNum 
 * \param *pdx 
 * \param *pdyInput 
 * \param *pdyOutput 
 * \param *bNormalization
 */
void CDataAlgorithmPCR::AdjustBaseLine(int nNum, int nAvgNum, double *pdx,double *pdyInput, double *pdyOutput,bool bNormalization /* = false */)
{
	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i];
		return;	
	}
	double backval = 0;
	BOOL bAble = AbleToCalculate(pdx,pdyInput,nNum,bNormalization);
	if (bAble)
	{
		int nxpos_max = 0, nxpos_min = 0;
		//二阶导数最大值对应的位置点，向前移动5点作为起始点，向后取连续3点的均值作为背景值
		{
			double *dFiltered = new double [nNum];
			double *dReturn = new double[nNum];
			double *dTemp = new double [nNum];
			for( int k=0; k< nNum; k++)
				dTemp[k] = pdyInput[k];
		
			//滤波
			for (int itemp = 0;itemp<5;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum,FILTERTYPE_CFMEAN5);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			for (int itemp = 0;itemp<5;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			//计算二阶导数
			CalDerivative(pdx,dFiltered,nNum,2,dReturn);

			//计算最大值
			double dmax = dReturn[0];double dmin = dReturn[0];
			for (int k = 0; k < nNum;k++)
			{
				if (dReturn[k] >= dmax)
				{
					dmax = dReturn[k];
					nxpos_max = k+1;
				}
				else if (dmin > dReturn[k])
				{
					dmin =dReturn[k];
					nxpos_min = k+1;
				}
			}

			delete [] dReturn;
			delete [] dFiltered;
			delete [] dTemp;
		}

		//提取有效数据源
		int nbegin = nxpos_max - 5;
		if (nbegin < 1/*数据点从1开始计数*/)
		{
			nbegin = 1;
		}
		int nend = nbegin + 3 -1;
		if (nend > nNum)
		{
			nend = nNum;
		}
		
		double sum = 0;
		for(int i= nbegin-1; i< nend; i++)
			sum += pdyInput[i];
		backval = sum / (nend-nbegin+1);
	}
	else
	{
		//减去 nAvgNum个最小值的均值
		SortDouble(nNum, pdyInput, pdyOutput);
		double sum = 0;
		for(int i=0; i< nAvgNum; i++)
			sum += pdyOutput[i];
		backval = sum /nAvgNum;
	}

	for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i] - backval;
}

/**
 * 调整基线
 * \param nNum 
 * \param nAvgNum 
 * \param *pdx 
 * \param *pdyInput 
 * \param *pdyOutput 
 * \param *bNormalization
 */
void CDataAlgorithmPCR::NewAdjustBaseLine(int nNum,int forewardNum,int calcDotNum,double *pdx,double *pdyInput, double *pdyOutput,bool bNormalization /* = false */)
{  
	int nAvgNum = calcDotNum;
	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i];
		return;	
	}

	double backval = 0;
	BOOL bAble = AbleToCalculate(pdx,pdyInput,nNum,bNormalization);
	if (bAble)
	{
		int nxpos_max = 0, nxpos_min = 0;
		//二阶导数最大值对应的位置点，向前移动forewardNum点作为起始点，向后取连续calcDotNum点的均值作为背景值
		{
			double *dFiltered = new double [nNum];
			double *dReturn = new double[nNum];
			double *dTemp = new double [nNum];
			for( int k=0; k< nNum; k++)
				dTemp[k] = pdyInput[k];

			

			//滤波
			for (int itemp = 0;itemp<5;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum,FILTERTYPE_CFMEAN5);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			for (int itemp = 0;itemp<5;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			//计算二阶导数
			CalDerivative(pdx,dFiltered,nNum,2,dReturn);

			//计算最大值
			double dmax = dReturn[0];double dmin = dReturn[0];
			for (int k = 0; k < nNum;k++)
			{
				if (dReturn[k] >= dmax)
				{
					dmax = dReturn[k];
					nxpos_max = k+1;
				}
				else if (dmin > dReturn[k])
				{
					dmin =dReturn[k];
					nxpos_min = k+1;
				}
			}

			delete [] dReturn;
			delete [] dFiltered;
			delete [] dTemp;
		}

		//提取有效数据源
		int nbegin = nxpos_max - forewardNum;
		if (nbegin < 1/*数据点从1开始计数*/)
		{
			nbegin = 1;
		}
		int nend = nbegin + calcDotNum -1;
		if (nend > nNum)
		{
			nend = nNum;
		}
		
		double sum = 0;
		for(int i = nbegin-1;i < nend;++i)
			sum += pdyInput[i];
		backval = sum / (nend-nbegin+1);
	}
	else
	{
		//减去 nAvgNum个最小值的均值
		SortDouble(nNum, pdyInput, pdyOutput);
		double sum = 0;
		for(int i = 0;i < nAvgNum;++i)
			sum += pdyOutput[i];
		backval = sum /nAvgNum;
	}

	for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i] - backval;
}

/**
 * 归一化处理（二阶导找Ct值起峰点，取起峰点之前的3-5点荧光信号值的均值作为背景值；若不满足条件则取3个(缺省)最小值的均值作为背景值；依据背景值进一步做归一化处理）
 * \param nNum 
 * \param nAvgNum 
 * \param *pdx 
 * \param *pdyInput 
 * \param *pdyOutput 
 * \param bMinus1
 * \param bkSuppression
 * \param baseCorrection
 */
void CDataAlgorithmPCR::NormalizedAnalysisBySndDerivative(int nNum, int nAvgNum, double *pdx,double *pdyInput, double *pdyOutput,int nFiltNum /* = 4 */,bool bMinus1 /*= false*/,bool bkSuppression /*= false*/,bool baseCorrection /*= false*/)
{
	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i];
		return;	
	}

	double backval = 0;
	double dbegin = 1,dend = 1;
	BOOL bAble = /*AbleToCalculate*/AbleToCalculateEx(pdx,pdyInput,nNum,false);
	if (bAble)
	{
		int nxpos_max = 0, nxpos_min = 0;double dmax = 0 , dmin = 0;
		//二阶导数最大值对应的位置点，向前移动5点作为起始点，向后取连续3点的均值作为背景值
		{
			double *dFiltered = new double [nNum];
			double *dReturn = new double[nNum];
			double *dTemp = new double [nNum];
			for( int k=0; k< nNum; k++)
				dTemp[k] = pdyInput[k];
		
			for (int itemp = 0;itemp<nFiltNum;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			//计算二阶导数
			CalDerivative(pdx,dFiltered,nNum,2,dReturn);
			//计算最大值
			dmax = dReturn[0]; dmin = dReturn[0];

			for (int t= 0 ;t < nNum;t++)
			{
				if(dReturn[t] < 0) dReturn[t] =0;
			}
			dmax = FindCrestWithParabola( pdx, dReturn, nNum, 6);
			nxpos_max = (int)(dmax + 0.5);

			if (baseCorrection)
			{
				if ((nxpos_max > 10)&&(nxpos_max < nNum))
				{
					//归一前数据进行基线矫正
					int nTemp = nxpos_max-1-5;
					if (nTemp >= 3)
					{
						double dCoef[3]={0};
						double* dTemp = new double[nTemp];
						memcpy(dTemp,pdyInput,sizeof(double)*nTemp);
						LinearFit(pdx,dTemp,nTemp,dCoef);
						//CFitting fit;
						//double dR2 = fit.SolutionCLEG(pdx, dTemp, dCoef, nTemp, 3);
						if (dCoef[2]*dCoef[2] > 0.5)
						{
							for (int i = 0; i < nNum; i ++)
							{
								pdyInput[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
								dFiltered[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
							}
						}
						delete[] dTemp;
					}

					//重新计算二阶导最大值
					CalDerivative(pdx,dFiltered,nNum,2,dReturn);
					//计算最大值
					dmax = dReturn[0]; dmin = dReturn[0];
					for (int t= 0 ;t < nNum;t++)
					{
						if(dReturn[t] < 0) dReturn[t] =0;
					}
					dmax = FindCrestWithParabola( pdx, dReturn, nNum, 6);
					nxpos_max = (int)(dmax + 0.5);
				}
			}

			delete [] dReturn;
			delete [] dFiltered;
			delete [] dTemp;
		}

		dbegin = dmax - 5;
		if ((dbegin < 1)||(dbegin > nNum))//数据点从1开始计数
		{
			dbegin = 1;
		}
		dend = dbegin + 3 -1;
		if (dend > nNum)
		{
			dend = nNum;
		}

		double sum = 0;
		for (double i = dbegin-1; i < dend;i++)
		{
			double dytemp = 0;
			Spline(pdx,pdyInput,nNum,i,dytemp);
			sum += dytemp;
		}
		backval = sum / (dend-dbegin+1);
	}
	else
	{
		if (baseCorrection)
		{
			double dCoef[3]={0};
			LinearFit(pdx,pdyInput,nNum,dCoef);
			if (dCoef[2]*dCoef[2] > 0.5)
			{
				for (int i = 0; i < nNum; i ++)
				{
					pdyInput[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
				}
			}
		}
		//减去 nAvgNum个最小值的均值
		SortDouble(nNum, pdyInput, pdyOutput);
		double sum = 0;
		for(int i=0; i< nAvgNum; i++)
			sum += pdyOutput[i];

		backval = sum /nAvgNum;
	}

	//减1操作,便于对数曲线表现
	if (bMinus1)
	{
		for(int i=0; i<nNum; i++)
			pdyOutput[i] = pdyInput[i] / backval-1;
	}
	else
	{
		for(int i=0; i<nNum; i++)
			pdyOutput[i] = pdyInput[i] / backval;
	}
	//背景信号压制
	if (bkSuppression)
	{
		if (!bAble)
		{
			if (bMinus1)
			{
				for(int i=0; i<nNum; i++)
					pdyOutput[i] += (0-pdyOutput[i])*0.5;
			}
			else
			{
				for(int i=0; i<nNum; i++)
					pdyOutput[i] += (1-pdyOutput[i])*0.5;
			}
		}
		else
		{
			if (bMinus1)
			{
				for(int i=0; i< dbegin-1; i++)
					pdyOutput[i] += (0-pdyOutput[i])*0.5;
			}
			else
			{
				for(int i=0; i< dbegin-1; i++)
					pdyOutput[i] += (1-pdyOutput[i])*0.5;
			}
		}
	}
}

/**
 * 归一化处理（内部判别参数开放，二阶导找Ct值起峰点，取起峰点之前的3-5点荧光信号值的均值作为背景值；若不满足条件则取3个(缺省)最小值的均值作为背景值；依据背景值进一步做归一化处理）
 * \param nNum 
 * \param *pdx 
 * \param *pdyInput 
 * \param *pdyOutput 
 * \param paraminfo
 */
void CDataAlgorithmPCR::NormalizedAnalysisBySndDerivative(int nNum, double *pdx,double *pdyInput, double *pdyOutput,tagFunAmpNormalizedAnaParamInfo paraminfo)
{
	if ((nNum>3)&&(nNum<=paraminfo.nAvgNum))
	{
		paraminfo.nAvgNum=3;
	}
	if( paraminfo.nAvgNum >= nNum || paraminfo.nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i];
		return;	
	}
	
	double backval = 0;
	double dbegin = 1,dend = 1;
	BOOL bAble = paraminfo.bAbleToCalcu ;
	if (bAble)
	{
		int nxpos_max = 0, nxpos_min = 0;double dmax = 0 , dmin = 0;
		//二阶导数最大值对应的位置点，向前移动5点作为起始点，向后取连续3点的均值作为背景值
		{
			double *dFiltered = new double [nNum];
			double *dReturn = new double[nNum];
			double *dTemp = new double [nNum];
			for( int k=0; k< nNum; k++)
				dTemp[k] = pdyInput[k];
		
			//滤波
			for (int itemp = 0;itemp<paraminfo.nFiltNum;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			//计算二阶导数
			CalDerivative(pdx,dFiltered,nNum,2,dReturn);
			//计算最大值
			dmax = dReturn[0]; dmin = dReturn[0];
			for (int t= 0 ;t < nNum;t++)
			{
				if(dReturn[t] < 0) dReturn[t] =0;
			}
			dmax = FindCrestWithParabola( pdx, dReturn, nNum, 6);
			nxpos_max = (int)(dmax + 0.5);

			if (paraminfo.bBaseCorrection)
			{
				if ((nxpos_max > paraminfo.nBCXPosThre)&&(nxpos_max < nNum))
				{
					//归一前数据进行基线矫正
					int nTemp = nxpos_max-paraminfo.nBCEndPreXPos;
					nTemp -= paraminfo.nBCStart;
					if (nTemp>= 2)
					{
						double dCoef[3]={0};
						double* dTemp = new double[nTemp];
						memcpy(dTemp,pdyInput+(paraminfo.nBCStart-1),sizeof(double)*nTemp);
						LinearFit(pdx,dTemp,nTemp,dCoef);
						//CFitting fit;
						//double dR2 = fit.SolutionCLEG(pdx, dTemp, dCoef, nTemp, 3);
						if ((dCoef[2] * dCoef[2] > paraminfo.dBCR2)&&(dCoef[1]<paraminfo.dBCK)&&(dCoef[1]>paraminfo.dBCK*-1))
						{
							for (int i = 0; i < nNum; i ++)
							{
								pdyInput[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
								dFiltered[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
							}
						}
						delete[] dTemp;
					}

					//重新计算二阶导最大值
					CalDerivative(pdx,dFiltered,nNum,2,dReturn);
					//计算最大值
					dmax = dReturn[0]; dmin = dReturn[0];
					for (int t= 0 ;t < nNum;t++)
					{
						if(dReturn[t] < 0) dReturn[t] =0;
					}
					dmax = FindCrestWithParabola( pdx, dReturn, nNum, 6);
					nxpos_max = (int)(dmax + 0.5);
				}
			}

			delete [] dReturn;
			delete [] dFiltered;
			delete [] dTemp;
		}

		//提取有效数据源
		dbegin = dmax - paraminfo.nNormalizeStartPreXPos; 
		if ((dbegin < 1)||(dbegin > nNum))//数据点从1开始计数
		{
			dbegin = 1;
		}
		dend = dbegin + paraminfo.nAvgNum -1;
		if (dend > nNum)
		{
			dend = nNum;
		}

		double sum = 0;
		for (double i = dbegin-1; i < dend;i++)
		{
			double dytemp = 0;
			Spline(pdx,pdyInput,nNum,i,dytemp);
			sum += dytemp;
		}
		backval = sum / (dend-dbegin+1);
	}
	else
	{
		if (paraminfo.bBaseCorrection)
		{
			int nTemp = nNum-paraminfo.nBCEndPreNumPos;
			if (nTemp>= 2)
			{
				double dCoef[3]={0};
				LinearFit(pdx,pdyInput,nTemp,dCoef);
				if (dCoef[2]*dCoef[2] > paraminfo.dBCR2)
				{
					for (int i = 0; i < nNum; i ++)
					{
						pdyInput[i] -= (pdx[i]*dCoef[1]);
					}
				}
			}
		}
		//除去 nAvgNum个最小值的均值
		SortDouble(nNum, pdyInput, pdyOutput);
		double sum = 0;
		for(int i=0; i< paraminfo.nAvgNum; i++)
			sum += pdyOutput[i];
		backval = sum /paraminfo.nAvgNum;
	}
	
	//减1操作,便于对数曲线表现
	if (paraminfo.bMinus1)
	{
		for(int i=0; i<nNum; i++)
			pdyOutput[i] = pdyInput[i] / backval-1;
	}
	else
	{
		for(int i=0; i<nNum; i++)
			pdyOutput[i] = pdyInput[i] / backval;
	}
	//背景信号压制
	if (paraminfo.bBaseSuppression)
	{
		if (!bAble)
		{
			if (paraminfo.bMinus1)
			{
				for(int i=0; i<nNum; i++)
					pdyOutput[i] += (0-pdyOutput[i])*paraminfo.dBSRatioAllData;
			}
			else
			{
				for(int i=0; i<nNum; i++)
					pdyOutput[i] += (1-pdyOutput[i])*paraminfo.dBSRatioAllData;
			}
		}
		else
		{
			if (paraminfo.bMinus1)
			{
				for(int i=0; i< dbegin-1; i++)
					pdyOutput[i] += (0-pdyOutput[i])*paraminfo.dBSRatioBaseLine;
			}
			else
			{
				for(int i=0; i< dbegin-1; i++)
					pdyOutput[i] += (1-pdyOutput[i])*paraminfo.dBSRatioBaseLine;
			}
		}
	}
}



/**
 * 计算DeltaRn处理（内部判别参数开放，二阶导找Ct值起峰点，起峰点前推5点，前后各取3点共7点荧光信号值的均值作为背景值；若不满足条件则取全体数据的中位值作为背景值；依据背景值进一步做△Rn处理）
 * \param nNum 
 * \param *pdx 
 * \param *pdyInput 
 * \param *pdyOutput 
 * \param paraminfo
 */
void CDataAlgorithmPCR::DeltaRnAnalysisBySndDerivative(int nNum, double *pdx,double *pdyInput, double *pdyOutput,tagFunAmpNormalizedAnaParamInfo paraminfo)
{
	if ((nNum>3)&&(nNum<=paraminfo.nAvgNum))
	{
		paraminfo.nAvgNum=3;
	}
	if( paraminfo.nAvgNum >= nNum || paraminfo.nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i];
		return;	
	}

	double backval = 0;
	double dbegin = 1,dend = 1;
	int nbegin =1;
	BOOL bAble = paraminfo.bAbleToCalcu /*AbleToCalculate(pdx,pdyInput,nNum,false)*/;
	if (bAble)
	{
		int nxpos_max = 0, nxpos_min = 0;double dmax = 0 , dmin = 0;
		//二阶导数最大值对应的位置点，向前移动5点作为起始点，前后各取连续3点的均值作为背景值
		{
			double *dFiltered = new double [nNum];
			double *dReturn = new double[nNum];
			double *dTemp = new double [nNum];
			for( int k=0; k< nNum; k++)
				dTemp[k] = pdyInput[k];
		
			//滤波
			for (int itemp = 0;itemp<1;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum,FILTERTYPE_CFMEAN7);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			for (int itemp = 0;itemp<paraminfo.nFiltNum;itemp++)
			{
				DigitalFilter(dTemp,dFiltered,nNum);
				memcpy(dTemp,dFiltered,sizeof(double)*nNum);
			}
			//计算二阶导数
			CalDerivative(pdx,dFiltered,nNum,2,dReturn);
			//计算最大值
			dmax = dReturn[0]; dmin = dReturn[0];
			for (int t= 0 ;t < nNum;t++)
			{
				if(dReturn[t] < 0) dReturn[t] =0;
			}
			dmax = FindCrestWithParabola( pdx, dReturn, nNum, 6);
			nxpos_max = (int)(dmax + 0.5);

			if (paraminfo.bBaseCorrection)
			{
				if ((nxpos_max > paraminfo.nBCXPosThre)&&(nxpos_max < nNum))
				{
					//归一前数据进行基线矫正
					int nTemp = nxpos_max-paraminfo.nBCEndPreXPos;
					nTemp -= paraminfo.nBCStart;
					if (nTemp>= 2)
					{
						double dCoef[3]={0};
						double* dTemp = new double[nTemp];
						memcpy(dTemp,pdyInput+(paraminfo.nBCStart-1),sizeof(double)*nTemp);
						LinearFit(pdx,dTemp,nTemp,dCoef);
						//CFitting fit;
						//double dR2 = fit.SolutionCLEG(pdx, dTemp, dCoef, nTemp, 3);
						if ((dCoef[2]*dCoef[2] > paraminfo.dBCR2)&&(dCoef[1]<paraminfo.dBCK)&&(dCoef[1]>paraminfo.dBCK*-1))
						{
							for (int i = 0; i < nNum; i ++)
							{
								pdyInput[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
								dFiltered[i] -= (pdx[i]*dCoef[1]/*+pdx[i]*pdx[i]*dCoef[2]*/);
							}
						}
						delete[] dTemp;
					}

					//重新计算二阶导最大值
					CalDerivative(pdx,dFiltered,nNum,2,dReturn);
					//计算最大值
					dmax = dReturn[0]; dmin = dReturn[0];
					for (int t= 0 ;t < nNum;t++)
					{
						if(dReturn[t] < 0) dReturn[t] =0;
					}
					dmax = FindCrestWithParabola( pdx, dReturn, nNum, 6);
					nxpos_max = (int)(dmax + 0.5);
				}
			}

			delete [] dReturn;
			delete [] dFiltered;
			delete [] dTemp;
		}

		//提取有效数据源
		//提取有效数据源
		nbegin = nxpos_max - paraminfo.nNormalizeStartPreXPos;
		if ((nbegin < 1)||(nbegin > nNum))//数据点从1开始计数
		{
			nbegin = 1;
		}
		int nend = nbegin + paraminfo.nAvgNum -1;
		if (nend > nNum)
		{
			nend = nNum;
		}

		double sum = 0;
		for(int i= nbegin-1; i< nend; i++)
			sum += pdyInput[i];
		backval = sum / (nend-nbegin+1);
	}
	else
	{
		if (paraminfo.bBaseCorrection)
		{
			int nTemp = nNum-paraminfo.nBCEndPreNumPos;
			if (nTemp>= 2)
			{
				double dCoef[3]={0};
				LinearFit(pdx,pdyInput,nTemp/*nNum*/,dCoef);
				if (dCoef[2]*dCoef[2] > paraminfo.dBCR2)
				{
					for (int i = 0; i < nNum; i ++)
					{
						pdyInput[i] -= (pdx[i]*dCoef[1]);
					}
				}
			}
		}
		//中位值
		backval = Median(pdyInput,nNum);
		//SortDouble(nNum, pdyInput, pdyOutput);
		//double sum = 0;
		//for(int i=0; i< paraminfo.nAvgNum; i++)
		//	sum += pdyOutput[i];
		//backval = sum /paraminfo.nAvgNum;
	}
	
	//计算△Rn
	for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i] - backval;
}

/**
 * 归一化处理（计算归到0-1区间的最小值位置、最大值位置）
 * \param nNum 
 * \param nAvgNum 
 * \param *pdx 
 * \param *pdyInput 
 * \param *pdyOutput 
 */
void CDataAlgorithmPCR::FindMinPosAndMaxPosByNormalizeFrom0To1ByFirstDerivative(int nNum,double *pdx,double *pdyInput,int &nMin,int &nMax,int nFiltNum /* = 4 */, double dThrePercent /* = 0.2 */)
{
	nMin = -1;
	nMax = -1;

	//BOOL bAble = AbleToCalculate(pdx,pdyInput,nNum,false);
	//if (bAble)
	{
		//最前面的峰，一阶导数最大值对应的位置点，向前移动寻找小于等于阈值的数据点，作为nMin
		//最后面的峰，一阶导数最大值对应的位置点，向后移动寻找小于等于阈值的数据点，作为nMax
		
		double *dFiltered = new double [nNum];
		double *dReturn = new double[nNum];
		double *dTemp = new double [nNum];
		for( int k=0; k< nNum; k++)
			dTemp[k] = pdyInput[k];
		
		for (int itemp = 0;itemp<nFiltNum;itemp++)
		{
			DigitalFilter(dTemp,dFiltered,nNum);
			memcpy(dTemp,dFiltered,sizeof(double)*nNum);
		}
		//计算一阶导数
		CalDerivative(pdx,dFiltered,nNum,1,dReturn);
		//基线斜率线性矫正
		SortDouble(nNum,dReturn,dTemp);
		double dCorrectionthre = dTemp[nNum/2];//中位值
		double *dCorrect = new double[nNum];
		for (int i = 0; i < nNum; i ++)
		{
			dReturn[i]>dCorrectionthre ? dCorrect[i] = dCorrectionthre : dCorrect[i] = dReturn[i];
		}
		double dCoef[3] = {0};
		LinearFit(pdx,dCorrect,nNum,dCoef);
		for (int i = 0; i < nNum; i ++)
		{
			dReturn[i] -= (pdx[i]*dCoef[1] + dCoef[0]);
		}
		delete []dCorrect;
		//计算有效阈值=最大值*dThrePercent;
		SortDouble(nNum,dReturn,dTemp);
		double dthresholdvalue =0;
		if (dThrePercent < 0.1) dThrePercent = 0.1;
		dthresholdvalue = dTemp[nNum-1]*dThrePercent;
		for (int i = 0;i<nNum;i++)
		{
			if (dReturn[i] <dthresholdvalue)
			{
				dReturn[i] = dthresholdvalue;
			}
		}
		//处理后的一阶导数寻峰
		std::vector<int> diff_v(nNum, 0);
		std::vector<int> pos_v;
		for (std::vector<int>::size_type i = 0; i < diff_v.size(); i++)
		{
			diff_v[i] = int(dReturn[i]);
		}
		findPeak(diff_v,pos_v);
		//
		int ipeaknums = pos_v.size();
		if (ipeaknums >= 1)
		{
			int nsearch = pos_v[0];
			for(;nsearch > 0;nsearch--)
			{
				if (dReturn[nsearch] <= dthresholdvalue)
				{
					nMin = nsearch;
					break;
				}
			}
			nsearch = pos_v[ipeaknums-1];
			for (;nsearch < nNum; nsearch++)
			{
				if (dReturn[nsearch] <= dthresholdvalue)
				{
					nMax = nsearch;
					break;
				}
			}
		}

		if ((nMin < 0)||(nMin >= nNum)) nMin = 0;
		if ((nMax < 0)||(nMax >= nNum)) nMax = nNum-1;

		delete [] dReturn;
		delete [] dFiltered;
		delete [] dTemp;	
	}
}

/**
 * 归一化处理（按标准公式做归一化处理，(dy - dmin)/(dmax - dmin)）
 * \param nNum 
 * \param nmaxpos 
 * \param nminpos 
 * \param *pdyInput 
 * \param *pdyOutput 
 */
void CDataAlgorithmPCR::NormalizedAnalysisFrom0To1BySndDerivative(int nNum,int nmaxpos,int nminpos,double dminTemp,double dmaxTemp,double *pdx,double *pdyInput, double *pdyOutput)
{
	if( nminpos < 0 || nmaxpos > nNum) 
	{
		for(int i=0; i<nNum; i++)
		pdyOutput[i] = pdyInput[i];
		return;	
	}

	//BOOL bAble = AbleToCalculate(pdx,pdyInput,nNum,false);
	//if (bAble)
	{
		double backval = 0;double dThreval = 0;
		int nxpos_max = nmaxpos, nxpos_min = nminpos;

		//提取有效数据源
		int nbegin = nxpos_min - 5;
		if ((nbegin < 1/*数据点从1开始计数*/)||(nbegin > nNum))
		{
			nbegin = 1;
		}
		int nend = nbegin + 3 -1;
		if (nend > nNum)
		{
			nend = nNum;
		}

		//double sum = 0;
		//for(int i= nbegin-1; i< nend; i++)
		//	sum += pdyInput[i];
		//backval = sum / (nend-nbegin+1);
		backval = pdyInput[nbegin-1];


		double *pn = new double[nNum];
		double *pm = new double[nNum];
		memset(pn,0,sizeof(double)*nNum);
		memset(pm,0,sizeof(double)*nNum);
		for (int t = 0; t <nNum; t++)
		{
			pn[t] = fabs(pdx[t] - dminTemp);
		}
		SortDouble(nNum,pn,pm);
		for (int t = 0; t <nNum; t++)
		{
			if (pn[t] == pm[0])
			{
				nbegin = t;
				break;
			}
		}
		backval = pdyInput[nbegin];
		//拟合插补进一步修正
		if (nNum >= 7)
		{
			nbegin -= 3; if (nbegin < 0) nbegin = 0;

			int nCoefNum = 4; int nDataCount = 7;
			double *pdCoef  = new double[nCoefNum];
			memset(pdCoef, 0, sizeof(double)*nCoefNum);
			double *pdXData = new double[nDataCount];
			double *pdYData = new double[nDataCount];
			memset(pdXData, 0, sizeof(double)*nDataCount);
			memset(pdYData, 0, sizeof(double)*nDataCount);

			for (int k = nbegin; k < nbegin+nDataCount; k++)
			{
				pdXData[k-nbegin] = pdx[k];
				pdYData[k-nbegin] = pdyInput[k];
			}
			CFitting fit;//3次多项式拟合，插补找最大值位置
			fit.SolutionCLEG(pdXData, pdYData, pdCoef, nDataCount, nCoefNum);


			double dYData = 0;
			for(int i = nCoefNum - 1 ; i >= 0 ; i--)
			{
				dYData += pdCoef[i] * pow(dminTemp, i);
			}
			backval = dYData;
			
			delete[] pdCoef;
			delete[] pdXData;
			delete[] pdYData;
		}



		nbegin = nxpos_max + 2;
		if ((nbegin < 1/*数据点从1开始计数*/)||(nbegin > nNum))
		{
			nbegin = nNum - 3 ;
		}
		nend = nbegin + 3 -1;
		if (nend > nNum)
		{
			nend = nNum;
		}

		//sum = 0;
		//for(int i= nbegin-1; i< nend; i++)
		//	sum += pdyInput[i];
		//dThreval = sum / (nend-nbegin+1);
		dThreval = pdyInput[nbegin-1];

		memset(pn,0,sizeof(double)*nNum);
		memset(pm,0,sizeof(double)*nNum);
		for (int t = 0; t <nNum; t++)
		{
			pn[t] = ::fabs(pdx[t] - dmaxTemp);
		}
		SortDouble(nNum,pn,pm);
		for (int t = 0; t <nNum; t++)
		{
			if (pn[t] == pm[0])
			{
				nbegin = t;
				break;
			}
		}
		dThreval = pdyInput[nbegin];
		//拟合插补进一步修正
		if (nNum >= 7)
		{
			nbegin -= 3; if (nbegin + 7 > nNum) nbegin = nNum - 7;

			int nCoefNum = 4; int nDataCount = 7;
			double *pdCoef  = new double[nCoefNum];
			memset(pdCoef, 0, sizeof(double)*nCoefNum);
			double *pdXData = new double[nDataCount];
			double *pdYData = new double[nDataCount];
			memset(pdXData, 0, sizeof(double)*nDataCount);
			memset(pdYData, 0, sizeof(double)*nDataCount);

			for (int k = nbegin; k < nbegin+nDataCount; k++)
			{
				pdXData[k-nbegin] = pdx[k];
				pdYData[k-nbegin] = pdyInput[k];
			}
			CFitting fit;//3次多项式拟合，插补找最大值位置
			fit.SolutionCLEG(pdXData, pdYData, pdCoef, nDataCount, nCoefNum);


			double dYData = 0;
			for(int i = nCoefNum - 1 ; i >= 0 ; i--)
			{
				dYData += pdCoef[i] * pow(dmaxTemp, i);
			}
			dThreval = dYData;

			delete[] pdCoef;
			delete[] pdXData;
			delete[] pdYData;
		}
		delete[] pn;
		delete[] pm;

		for(int i=0; i<nNum; i++)			
		   pdyOutput[i] = (pdyInput[i] - dThreval) / (backval - dThreval);		
	}
}

/**
 * 归一化处理
 * \param nNum 
 * \param nAvgNum 
 * \param nNormType 0，下归一；1，上归一
 * \param *pInput 
 * \param *pOutput 
 */
void CDataAlgorithmPCR::NormalizedAnalysis(int nNum, int nAvgNum,int nNormType,bool bMinus1,double *pInput, double *pOutput)
{
	if( nNum <1 ) 
		return ;
	if( nAvgNum >= nNum || nAvgNum <= 0) 
	{
		for(int i=0; i<nNum; i++)
			pOutput[i] = pInput[i];
		return;	
	}
	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i];

	SortDouble(nNum, pInput, pOutput);

	double dNormThre=0, dsum=0;
	switch(nNormType)
	{
	case 0:
		for (int i=0;i<nAvgNum;i++)
			dsum+=pOutput[i];
		dNormThre=dsum/nAvgNum;
		break;
	case 1:
		for (int i=nNum-nAvgNum;i<nNum;i++)
			dsum+=pOutput[i];
		dNormThre=dsum/nAvgNum;
		break;
	default:
		break;
	}

	if(0==dNormThre)
	{
		return;
	}
	for(int i=0; i<nNum; i++)
		pOutput[i] = pInput[i] / dNormThre;

	if (bMinus1)
	{
		for(int i=0; i<nNum; i++)
			pOutput[i] -= 1;
	}
}

/**
 * 判断数据点是否能够计算ct值
 * \param *dx 
 * \param *dy 
 * \param nNum 
 * \return 
 */
BOOL CDataAlgorithmPCR::AbleToCalculate(double *dx, double *dy, int nNum,bool bNormalization /* = false */,bool bCt /* = true */)
{
	if( nNum <1 ) 
		return false;
    double avg =0, std =0;
	double max = abs(dy[0]);
	double *y = new double [nNum];
	for(int i=0; i< nNum; i++) y[i] = dy[i];
	for(int i=0; i< nNum; i++)
		max = max > abs(y[i]) ? max :abs(y[i]);
	// 如果数据全为0，退出
	if ( max == 0)
	{
		delete [] y;
		return false;
	}

	double* temp = new double [nNum];
	SortDouble(nNum, y, temp);

	double dForeAvg =0 , dEndAvg = 0;
	int nForeID = (int)(nNum / 3.0+0.5) , nEndID =(int)(nNum *2/3.0+0.5);
	for(int i=0; i< nForeID; i++)
	{
		if (bCt)
		{
			dForeAvg += /*y[i]*/temp[i];
		}
		else
		{
			dForeAvg += y[i];
		}
	}
	//尾端最多取5个点均值，实测数据效果相对更好
	if(nEndID >= 10)  nEndID = nNum-5;
	for(int i= nEndID; i< nNum; i++)
		dEndAvg += y[i];
	dForeAvg = nForeID > 0 ? dForeAvg/(nForeID/*+1*/): 0;
	dEndAvg = nEndID > 0? dEndAvg/(nNum -nEndID/*+1*/): 0;

	//计算变异系数
	for(int i=0; i< nNum; i++)
		avg += y[i];
	avg /= nNum;
	for(int i= 0; i< nNum; i++)
		std += (y[i]-avg)*(y[i]-avg);
	std = sqrt(std/(nNum-1));
	double cv = 0;
	if( avg != 0)
		cv = std / avg;
	else
		cv = 0;
	delete [] y;
	delete [] temp;
	//主要用标准方差判断，阈值/阈值范围可根据本系统试验数据进行调整
	if (!bNormalization)
	{
		if (bCt)
		{
			if ( std > 45 && dForeAvg*1.03 < dEndAvg /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > 45 && dForeAvg > dEndAvg*1.1 /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (bCt)
		{
			if ( std > /*0.1*/0.007 && dForeAvg*1.03 < dEndAvg /*&& cv >= 0.04 */)//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > /*0.1*/0.01 && dForeAvg > dEndAvg*1.1 /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}

	}
}

BOOL CDataAlgorithmPCR::AbleToCalculate(double *dx, double *dy, int nNum,tagFunAbleToCalcuParamInfo paraminfo)
{
	if( nNum <1 ) 
		return false;
	double avg =0, std =0;
	double max = abs(dy[0]);
	double *y = new double [nNum];
	for(int i=0; i< nNum; i++) y[i] = dy[i];
	for(int i=0; i< nNum; i++)
		max = max > abs(y[i]) ? max :abs(y[i]);
	// 如果数据全为0，退出
	if ( max == 0)
	{
		delete [] y;
		return false;
	}

	double* temp = new double [nNum];
	SortDouble(nNum, y, temp);

	double dForeAvg =0 , dEndAvg = 0;
	int nForeID = 0, nEndID = 0;
	if (2 == paraminfo.nCurveType)//蛋白熔曲
	{
		nForeID = (int)(nNum / 5.0+0.5) ; nEndID =(int)(nNum *4/5.0+0.5);
	}
	else
	{
		//0,扩增曲线；1，普通熔曲
		nForeID = (int)(nNum / 3.0+0.5) ; nEndID =(int)(nNum *2/3.0+0.5);
	}
	for(int i=0; i< nForeID; i++)
	{
		if (/*0*/2 == paraminfo.nCurveType)
		{
			dForeAvg += /*y[i]*/temp[i];
		}
		else
		{
			dForeAvg += y[i];
		}
	}
	if (2 == paraminfo.nCurveType)
	{
		//蛋白熔曲，取排序后尾端的数据
		for(int i= nEndID; i< nNum; i++)
			dEndAvg += temp[i];
	}
	else
	{
		//尾端最多取5个点均值，实测数据效果相对更好
		if(nEndID >= 10)  nEndID = nNum-5;
		for(int i= nEndID; i< nNum; i++)
			dEndAvg += y[i];
	}

	dForeAvg = nForeID > 0 ? dForeAvg/(nForeID/*+1*/): 0;
	dEndAvg = nEndID > 0? dEndAvg/(nNum -nEndID/*+1*/): 0;

	//计算变异系数
	for(int i=0; i< nNum; i++)
		avg += y[i];
	avg /= nNum;
	for(int i= 0; i< nNum; i++)
		std += (y[i]-avg)*(y[i]-avg);
	std = sqrt(std/(nNum-1));
	double cv = 0;
	if( avg != 0)
		cv = std / avg;
	else
		cv = 0;
	//进一步计算一阶导数
	double dMax=0,dRatio=0;
	if (1 != paraminfo.nCurveType)
	{
		CalDerivative(dx,y,nNum,1,temp);

		//寻峰，查找数据极大值Max；计算大于0的数据的中位值Median，计算Ratio=Max/Median;
		double *dMaxPos = new double [1];
		AutoFindCrest(1,nNum,dx,temp,dMaxPos);
		if (dMaxPos[0] == -1)
		{
			//Todo...
			dMax=0;
		}
		else
		{
			dMax=temp[(int)dMaxPos[0]];
		}
		//
		int npositiveNum=0;
		for (int i=0;i<nNum;i++)
		{
			temp[i]>0 ? npositiveNum++ : 1;
		}
		if (npositiveNum>=3)
		{
			double* positive=new double[npositiveNum];
			memset(positive,0,sizeof(double)*npositiveNum);
			for (int k=0,i=0;i<nNum;i++)
			{
				if(temp[i]>0)
				{
					positive[k]=temp[i];
					k++;
				}
			}
			double dMean =Median(positive,npositiveNum);
			dRatio = dMax/dMean;
			delete[] positive;
		}
		else
		{
			dRatio=0;
		}
		delete[] dMaxPos;
	}
	delete [] y;
	delete [] temp;
	//主要用标准方差判断，阈值/阈值范围可根据本系统试验数据进行调整
	if (!paraminfo.bNormalization)
	{
		if (1 != paraminfo.nCurveType)
		{
			if ( std > paraminfo.dAmpNoNormalizeStd && dForeAvg*paraminfo.dAmpNoNormalizeRatio < dEndAvg && dMax>paraminfo.dFDNoNormalizeVal && dRatio>paraminfo.dFDNoNormalizeRatio/*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > paraminfo.dMeltNoNormalizeStd && dForeAvg > dEndAvg*paraminfo.dMeltNoNormalizeRatio /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (1 != paraminfo.nCurveType)
		{
			if ( std > /*0.1*/paraminfo.dAmpNormalizeStd && dForeAvg*paraminfo.dAmpNormalizeRatio < dEndAvg && dMax>paraminfo.dFDNormalizeVal && dRatio>paraminfo.dFDNormalizeRatio /*&& cv >= 0.04 */)//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > /*0.1*/paraminfo.dMeltNormalizeStd && dForeAvg > dEndAvg*paraminfo.dMeltNormalizeRatio /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}

	}
}

BOOL CDataAlgorithmPCR::NewAbleToCalculate(double *dx, double *dy, int nNum,tagFunAbleToCalcuParamInfo paraminfo,int newStart,int newForwardNum ,float newStdNum)
{
	if( nNum <1 ) 
		return false;
	double avg =0, std =0;
	double max = abs(dy[0]);
	double *y = new double [nNum];
	for(int i=0; i< nNum; i++) y[i] = dy[i];
	for(int i=0; i< nNum; i++)
		max = max > abs(y[i]) ? max :abs(y[i]);
	// 如果数据全为0，退出
	if ( max == 0)
	{
		delete [] y;
		return false;
	}

	double* temp = new double [nNum];
	SortDouble(nNum, y, temp);

	double* dFiltered = new double [nNum];
	::memset(dFiltered,0,nNum);

	double dForeAvg =0 , dEndAvg = 0;
	int nForeID = 0, nEndID = 0;
	if (2 == paraminfo.nCurveType)//蛋白熔曲
	{
		nForeID = (int)(nNum / 5.0+0.5); 
		nEndID =(int)(nNum *4/5.0+0.5);
	}
	else
	{
		//0,扩增曲线；1，普通熔曲
		nForeID = (int)(nNum / 3.0+0.5); 
		nEndID =(int)(nNum * 8 / 9 + 0.5);
	}
	for(int i=0; i< nForeID; i++)
	{
		if (/*0*/2 == paraminfo.nCurveType)
		{
			dForeAvg += /*y[i]*/temp[i];
		}
		else
		{
			dForeAvg += y[i];
		}
	}
	if (2 == paraminfo.nCurveType)
	{
		//蛋白熔曲，取排序后尾端的数据
		for(int i= nEndID; i< nNum; i++)
			dEndAvg += temp[i];
	}
	else
	{
		//尾端最多取5个点均值，实测数据效果相对更好
		if(nEndID >= 10)
		{
			nEndID = nNum-/*5*/3;
		}
		for(int i= nEndID; i< nNum; i++)
		{
			dEndAvg += y[i];
		}
	}

	dForeAvg = nForeID > 0 ? dForeAvg/(nForeID/*+1*/): 0;
	dEndAvg = nEndID > 0? dEndAvg/(nNum -nEndID/*+1*/): 0;

	//计算变异系数
	for(int i=0; i< nNum; i++)
		avg += y[i];
	avg /= nNum;
	for(int i= 0; i< nNum; i++)
		std += (y[i]-avg)*(y[i]-avg);
	std = sqrt(std/(nNum-1));
	double cv = 0;
	if( avg != 0)
		cv = std / avg;
	else
		cv = 0;

	BOOL bFindNormalD2Max = TRUE;

	double* dTemp = new double [nNum];
	memcpy(dTemp,y,nNum * sizeof(double));

	double* dReturn = new double [nNum];
	::memset(dReturn,0,nNum);

	double dCrestBkRatio = 0;

	{
		for (int itemp = 0;itemp < 4;itemp++)
		{
			DigitalFilter(dTemp,dFiltered,nNum);  
			memcpy(dTemp,dFiltered,sizeof(double)*nNum);
		}

		//计算二阶导数
		NewCalDerivative(dx,dFiltered,nNum,2,dReturn); 
		//计算最大值
		double dmax = dReturn[0]; 
		double dmin = dReturn[0];

		double dXPosMax = 1;

		for (int t= 0 ;t < nNum;t++)
		{
			if(dReturn[t] < 0) dReturn[t] =0;
		}
		dXPosMax = FindCrestWithParabola( dx, dReturn, nNum, 6);

		int nxpos_max = (int)(dXPosMax + 0.5);
		dmax = dReturn[nxpos_max - 1];

		int newCalcBaseNum = nxpos_max - newForwardNum - newStart + 1;

		if(newCalcBaseNum > 1)
		{
			double newAvg = 0,newStd = 0;
			//计算变异系数
			for(int i = 0; i < newCalcBaseNum;++i)
				newAvg += dReturn[i];
			newAvg /= newCalcBaseNum;
			for(int i= 0; i< nxpos_max - 3; i++)
				newStd += (dReturn[i]-newAvg)*(dReturn[i]-newAvg);
			newStd = sqrt(newStd/(newCalcBaseNum -1));

			if(dmax > (newAvg + newStdNum * newStd))
			{
				bFindNormalD2Max = TRUE;
			}
			else
			{
				bFindNormalD2Max = FALSE;
			}
		}
		else
		{
			bFindNormalD2Max = FALSE;
		}

		//计算二阶导最大值（从离散数据中寻找）和背景值（取滤波后数据前N个点）的比值
		double dBkSum = 0,dBkAvg = 0;
		for(int i = 0;i < paraminfo.bkCalcDotNum;++i)
		{
			dBkSum += dy[i];
		}
		if(paraminfo.bkCalcDotNum > 0)
		{
			dBkAvg = dBkSum / paraminfo.bkCalcDotNum;
		}

		if(0 != dBkAvg)
		{
			dCrestBkRatio = abs(dmax / dBkAvg);
		}

		delete [] dTemp;
		delete [] dFiltered;
		delete [] dReturn;
	}

	delete [] y;
	delete [] temp;
	//主要用标准方差判断，阈值/阈值范围可根据本系统试验数据进行调整
	if (!paraminfo.bNormalization)
	{
		if (1 != paraminfo.nCurveType)
		{
			if ( bFindNormalD2Max && dCrestBkRatio > paraminfo.dCrestBkRatio && std > paraminfo.dAmpNoNormalizeStd && dForeAvg*paraminfo.dAmpNoNormalizeRatio < dEndAvg )
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > paraminfo.dMeltNoNormalizeStd && dForeAvg > dEndAvg*paraminfo.dMeltNoNormalizeRatio /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (1 != paraminfo.nCurveType)
		{
			if ( bFindNormalD2Max && dCrestBkRatio > paraminfo.dCrestBkRatio && std > paraminfo.dAmpNormalizeStd && dForeAvg*paraminfo.dAmpNormalizeRatio < dEndAvg )
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > /*0.1*/paraminfo.dMeltNormalizeStd && dForeAvg > dEndAvg*paraminfo.dMeltNormalizeRatio /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}

	}
}


/**
 * 判断数据点是否能够计算ct值（附加一阶导数判定）
 * \param *dx 
 * \param *dy 
 * \param nNum 
 * \return 
 */
BOOL CDataAlgorithmPCR::AbleToCalculateEx(double *dx, double *dy, int nNum,bool bNormalization /* = false */,bool bCt /* = true */)
{
	if( nNum <1 ) 
		return false;
    double avg =0, std =0;
	double max = abs(dy[0]);
	double *y = new double [nNum];
	for(int i=0; i< nNum; i++) y[i] = dy[i];
	for(int i=0; i< nNum; i++)
		max = max > abs(y[i]) ? max :abs(y[i]);
	// 如果数据全为0，退出
	if ( max == 0)
	{
		delete [] y;
		return false;
	}

	double* temp = new double [nNum];
	SortDouble(nNum, y, temp);

	double dForeAvg =0 , dEndAvg = 0;
	int nForeID = (int)(nNum / 3.0+0.5) , nEndID =(int)(nNum *2/3.0+0.5);
	for(int i=0; i< nForeID; i++)
	{
		if (bCt)
		{
			dForeAvg += /*y[i]*/temp[i];
		}
		else
		{
			dForeAvg += y[i];
		}
	}
	//尾端最多取5个点均值，实测数据效果相对更好
	if(nEndID >= 10)  nEndID = nNum-5;
	for(int i= nEndID; i< nNum; i++)
		dEndAvg += y[i];
	dForeAvg = nForeID > 0 ? dForeAvg/(nForeID/*+1*/): 0;
	dEndAvg = nEndID > 0? dEndAvg/(nNum -nEndID/*+1*/): 0;

	//计算变异系数
	for(int i=0; i< nNum; i++)
		avg += y[i];
	avg /= nNum;
	for(int i= 0; i< nNum; i++)
		std += (y[i]-avg)*(y[i]-avg);
	std = sqrt(std/(nNum-1));
	double cv = 0;
	if( avg != 0)
		cv = std / avg;
	else
		cv = 0;
	//进一步计算一阶导数
	double dMax=0,dRatio=0;
	if (bCt)
	{
		CalDerivative(dx,y,nNum,1,temp);
		//寻峰，查找数据极大值Max；计算大于0的数据的中位值Median，计算Ratio=Max/Median;
		
		double *dMaxPos = new double [1];
		AutoFindCrest(1,nNum,dx,temp,dMaxPos);
		if (dMaxPos[0] == -1)
		{
			//Todo...
			dMax=0;
		}else
		{
			dMax=temp[(int)dMaxPos[0]];
		}
		//
		int npositiveNum=0;
		for (int i=0;i<nNum;i++)
		{
			temp[i]>0 ? npositiveNum++ : 1;
		}
		if (npositiveNum>=3)
		{
			double* positive=new double[npositiveNum];
			memset(positive,0,sizeof(double)*npositiveNum);
			for (int k=0,i=0;i<nNum;i++)
			{
				if(temp[i]>0)
				{
					positive[k]=temp[i];
					k++;
				}
			}
			double dMean =Median(positive,npositiveNum);
			dRatio = dMax/dMean;
			delete[] positive;
		}
		else
		{
			dRatio=0;
		}
		delete[] dMaxPos;
	}
	delete [] y;
	delete [] temp;
	//主要用标准方差判断，阈值/阈值范围可根据本系统试验数据进行调整
	if (!bNormalization)
	{
		if (bCt)
		{
			if ( std > 45 && dForeAvg*1.03 < dEndAvg &&  dMax>40 && dRatio>2/*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > 45 && dForeAvg > dEndAvg*1.1 /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (bCt)
		{
			if ( std > /*0.1*/0.007 && dForeAvg*1.03 < dEndAvg &&  dMax>0.01 && dRatio>2/*&& cv >= 0.04 */)//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}
		else
		{
			//Tm值
			if ( std > /*0.1*/0.01 && dForeAvg > dEndAvg*1.1 /*&& cv >= 0.04*/ )//&& dForeAvg*8 < dEndAvg)
				return true;
			else
				return false;
		}

	}
}


void CDataAlgorithmPCR::Reverse_array(double *x,int nNum, double *dReturn)
{
	double* src = new double[nNum];
	memset(src,0,sizeof(double)*nNum);
	memcpy(src,x,sizeof(double)*nNum);
	for(int i=0;i<nNum;i++)
	{
		*(dReturn + i) = *(src + nNum - 1 -i);
	}
	delete []src;
}

/***************************************************************************************************
*
*函数名:findPeak
*
*参数：
*
*vector<int> v     原始数据
*vector<int> peakPositions        峰值（凸峰或凹峰）对应的数据点位置值
*
*返回值:无
*
*功能：查找数据波形中的峰值位置
*
****************************************************************************************************/

void CDataAlgorithmPCR:: findPeak(const std::vector<int>& v, std::vector<int>& peakPositions)
{
	std::vector<int> diff_v(v.size() - 1, 0);
	// 计算V的一阶差分和符号函数trend
	for (std::vector<int>::size_type i = 0; i != diff_v.size(); i++)
	{
		if (v[i + 1] - v[i]>0)
			diff_v[i] = 1;
		else if (v[i + 1] - v[i] < 0)
			diff_v[i] = -1;
		else
			diff_v[i] = 0;
	}
	// 对Trend作了一个遍历
	for (int i = diff_v.size() - 1; i >= 0; i--)
	{
		if (diff_v[i] == 0 && i == diff_v.size() - 1)
		{
			diff_v[i] = 1;
		}
		else if (diff_v[i] == 0)
		{
			if (diff_v[i + 1] >= 0)
				diff_v[i] = 1;
			else
				diff_v[i] = -1;
		}
	}

	for (std::vector<int>::size_type i = 0; i != diff_v.size() - 1; i++)
	{
		if (diff_v[i + 1] - diff_v[i] == -2)
			peakPositions.push_back(i + 1);
	}
}

double CDataAlgorithmPCR::GetDoublePrecision(double dValue,int nPres)
{
	double factor = pow(10.0,nPres);
	double dReturnValue = (double)((/*int*/__int64)(dValue*factor + 0.5))/factor;
	return dReturnValue;
}

double CDataAlgorithmPCR::Median(double* x,int nNum)
{
	double dVal = 0;
	if (nNum <= 0) return dVal;
	double* temp=new double[nNum];
	memset(temp,0,sizeof(double)*nNum);
	SortDouble(nNum,x,temp);
	if (nNum%2 ==0)//若为偶数，取中间的两个数做平均
	{
		dVal =((temp[nNum/2] +temp[nNum/2 -1])) /2 ;
	}
	else
	{
		dVal =temp[nNum/2];
	}
	delete[] temp;
	return dVal;
}

int CDataAlgorithmPCR::CalcCycleOfMaxMOTRatioBy(int nCycleCount, double* pdXValue, double* pdYValue, double dMinRatio)
{
	int nCalcCycle = -1;

	// 先归一化
	double dMaxValue = pdYValue[0];
	double dMinValue = dMaxValue;
	double* pdTempValueY = new double[nCycleCount];
	for (int i=1; i<nCycleCount; i++)
	{
		if (pdYValue[i] > dMaxValue)
		{
			dMaxValue = pdYValue[i];
		}
		else if (pdYValue[i] < dMinValue)
		{
			dMinValue = pdYValue[i];
		}
	}

	if (dMaxValue < 1 || dMinValue < 1)
	{
		// 没有数据
		delete [] pdTempValueY;
		return nCalcCycle;
	}

	//添加于20220401，孙文郎
	if(dMaxValue == dMinValue)
	{
		delete [] pdTempValueY;
		return nCalcCycle;
	}

	for (int i=0; i<nCycleCount; i++)
	{
		// pdTempValueY[i] = (pdYValue[i] - dMinValue) / (dMaxValue - dMinValue);
		pdTempValueY[i] = pdYValue[i];
	}

	double* dDerivateValue = new double[nCycleCount];
	CalMeltFirstDerivative(pdXValue, pdTempValueY, nCycleCount, TRUE, 1, dDerivateValue);

	double dMaxValueY = dDerivateValue[0];
	for (int i=1; i<nCycleCount;i++)
	{
		if (dDerivateValue[i] > dMaxValueY)
		{
			dMaxValueY = dDerivateValue[i];
			nCalcCycle = int(pdXValue[i]);
		}
	}

	delete [] pdTempValueY;
	delete [] dDerivateValue;

	if (dMaxValueY < dMinRatio)
	{
		nCalcCycle = -1;
	}

	return nCalcCycle;
}

void CDataAlgorithmPCR::CalMeltFirstDerivative(double *x, double *y, int nNum,BOOL bBackMinusFront,int stepNum,double *dReturn)
{
	int n=nNum; 
	double *dy = new double [n];

	if(1 == stepNum)
	{
		if(bBackMinusFront)
		{
			dy[n - 1] = 0;
			for(int i = 0;i < n - 1;++i)
			{
				dy[i] = y[i+1] - y[i];
			}
		}
		else
		{
			dy[0] = 0;
			for(int i = 1;i <= n - 1;++i)
			{
				dy[i] = y[i - 1] - y[i];
			}
		}
	}
	else
	{
		if(bBackMinusFront)
		{
			dy[n - 1] = 0;
			for(int i = 0;i < n - 1;++i)
			{
				if(i + stepNum < n && i + stepNum > -1)
				{
					dy[i] = y[i + stepNum] - y[i];
				}
				else
				{
					dy[i] = 0;
				}
			}
		}
		else
		{
			dy[0] = 0;
			for(int i = 1;i <= n - 1;++i)
			{
				if(i - stepNum < n && i - stepNum > -1)
				{
					dy[i] = y[i - stepNum] - y[i];
				}
				else
				{
					dy[i] = 0;
				}
			}
		}
	}

	for(int i = 0; i< n; i++)
	{
		dReturn[i] = dy[i];
	}

	delete [] dy;
}

// 在没有找到Ct的情况下，在数据前半部分找到规定范围内Std变化最小的区间位置基线位置，宽度默认为6
void CDataAlgorithmPCR::CalcAutoBaselineByMinStd(int nPtCount, double *pInputY, int &iOutStart, int &iOutEnd)
{
	int nTempStart = 1;
	int nTempEnd = nPtCount / 2;

	int nStep = (nTempEnd - nTempStart) / 2;
	if (nStep > 6)
	{
		nStep = 6;
	}
	else if (nStep < 3)
	{
		nTempEnd = nPtCount - 1;
		nStep = (nTempEnd - nTempStart) / 2;
		if (nStep > 6)
		{
			nStep = 6;
		}
		else if (nStep < 3)
		{
			nStep = 3;
			if (nTempEnd - nStep <= nTempStart)
			{
				nStep = 1;
				nTempEnd = nTempStart+1;
			}
		}
	}

	BOOL bFirst = TRUE;
	double dMinStd;
	for (int i=nTempStart; i<=nTempEnd-nStep; i++)
	{
		double dTempSum = 0;
		for (int j=0; j<nStep; j++)
		{
			dTempSum += pInputY[i+j];
		}
		double dTempAverage = dTempSum / nStep;

		double dTempStd = 0;
		for (int x=0; x<nStep; x++)
		{
			dTempStd += (pInputY[i+x] - dTempAverage) * (pInputY[i+x] - dTempAverage);
		}

		if (bFirst)
		{
			dMinStd = dTempStd;
			iOutStart = i;
			bFirst = FALSE;
		}
		else
		{
			if (dMinStd > dTempStd)
			{
				dMinStd = dTempStd;
				iOutStart = i;
			}
		}
	}

	iOutEnd = iOutStart + nStep;
}


double CDataAlgorithmPCR::FindCrestWithParabola(double *x, double *y, int nNum, int nWndWidth)
{
	if(nNum < 1 || NULL == x || NULL == y)
	{
		return -1;
	}

	if ( nNum < nWndWidth || nWndWidth < 3)
	{
		return -1;
	}

	double *dTempY = new double[nNum];
	memcpy( dTempY, y, nNum*sizeof(double));

	for( int k=1; k< nNum-1; k++)
	{
		if( dTempY[k] < y[k-1] && dTempY[k] < y[k+1])
			dTempY[k] = ( y[k-1] + y[k+1]) /2;
	}

	double dSum , dMax = -1000, dMaxCrest = -1000;
	int nCrestPos = 0;

	for( int i = 0; i< nNum - nWndWidth; i++)
	{
		dSum = 0;
		for( int j = 0; j<nWndWidth; j++)
		{
			dSum += dTempY[i+j];
		}
		if (dMax < dSum )
		{
			dMax = dSum;
			nCrestPos = i;
		}
		dMaxCrest = dMaxCrest < dTempY[i] ? dTempY[i] : dMaxCrest;
	}

	dMaxCrest /= 10;
	int end, start = 0;
	start = nCrestPos;
	end = nCrestPos + nWndWidth-1;

	int sumn = (int)(end - start) + 1; 

	if (sumn < 3) 
	{
		return -1;
	}

	double sumx = 0.0,  sumx2 = 0.0, sumx3 = 0.0,  sumx4 = 0.0;
	double sumy = 0.0,  sumxy = 0.0,   sumx2y = 0.0;

	double doubj;
	for (int j = start; j <= end; j++) 
	{
		doubj = (double) j - start;

		sumx += doubj;
		sumx2 += doubj * doubj;
		sumx3 += doubj * doubj * doubj;
		sumx4 += doubj * doubj * doubj * doubj;
		sumy += dTempY[j] / dMaxCrest;                      // Division is to normalize data
		sumxy += doubj * (dTempY[j] / dMaxCrest);
		sumx2y += doubj * doubj * (dTempY[j] / dMaxCrest);  // Division is to normalize data
	}

	double dXPos = 0.0;
	delete [] dTempY;
	double determ = sumn*(sumx2*sumx4 - sumx3*sumx3) - sumx*(sumx*sumx4 - sumx2*sumx3) + sumx2*(sumx*sumx3 - sumx2*sumx2);
	if (fabs(determ) < 1.0E-12) 
	{
		return -1;
	}
	double parba = sumy*(sumx2*sumx4 - sumx3*sumx3) - sumxy*(sumx*sumx4 - sumx2*sumx3) + sumx2y*(sumx*sumx3 - sumx2*sumx2);

	parba /= determ;
	double parbb = -sumy*(sumx*sumx4 - sumx2*sumx3) + sumxy*(sumn*sumx4 - sumx2*sumx2) - sumx2y*(sumn*sumx3 - sumx*sumx2);

	parbb /= determ;
	double parbc = sumy*(sumx*sumx3 - sumx2*sumx2) - sumxy*(sumn*sumx3 - sumx*sumx2) + sumx2y*(sumn*sumx2 - sumx*sumx);

	parbc /= determ;
	dXPos = -(parbb)/(2.0 * (parbc));

	dXPos += start;
	//TRACE(_T("dXPos = %.3f\n"),  dXPos);

	//该函数有bug，尚未修复
	if ((int)(dXPos+0.5) >= nNum)
	{
		dXPos = nNum-1;
	}
	else if ((int)(dXPos+0.5) <= 1)
	{
		dXPos = start+sumn/2.0;
	}

	if (dXPos > end)
	{
		dXPos = end -1 + dXPos - int(dXPos);
	}
	else if (dXPos < start)
	{
		dXPos = start + dXPos - int(dXPos);
	}

	return dXPos;
}

// 计算自动基线的位置，通过二阶导数找Ct,返回Ct值
double CDataAlgorithmPCR::CalcAutoBaselinePos(int nPtCount, double *pdInputX, double *pdInputY, int &iOutStart, int &iOutEnd)
{
	double dMaxPosX = CalcCtPos(nPtCount, pdInputX, pdInputY);
	// 计算基线位置
	CalcAutoBaselinePosBySndDrvCt(dMaxPosX, nPtCount, pdInputX, pdInputY, iOutStart, iOutEnd);
	return dMaxPosX;
}

void CDataAlgorithmPCR::CalcAutoBaselinePosBySndDrvCt(double dSndDrvCt, int nPtCount, double *pdInputX, double *pdInputY, int &iOutStart, int &iOutEnd)
{
	// 计算基线位置
	if (dSndDrvCt > 4)
	{
		int iPosMax = int(dSndDrvCt/* + 0.5*/);
		CalcBaselinePosByCt(iPosMax, iOutStart, iOutEnd);

		if (iOutEnd - iOutStart > 4)
		{
			BOOL bFindPlateau = FALSE;

			double dCurCoef[3] = {0};
			double* pdTempX = new double[iOutEnd-iOutStart+1];
			double* pdTempY = new double[iOutEnd-iOutStart+1];
			int iWndWidth = (iOutEnd - iOutStart)/3;
			if (iWndWidth < 3)
				iWndWidth = 3;
			double fFirstSlope = 0;
			double dMinSlope = 0;
			int		iMinStart = 0;	
			BOOL bFindFirstSlope = FALSE;

			for (int i=iOutEnd; i>=iOutStart+iWndWidth; i--)
			{
				for (int j=0; j<iWndWidth; j++)
				{
					pdTempX[j] = pdInputX[i-iWndWidth+1+j];
					pdTempY[j] = pdInputY[i-iWndWidth+1+j];
				}
				LinearFit(pdTempX, pdTempY, iWndWidth, dCurCoef);

				if (i == iOutEnd)
				{
					dMinSlope = dCurCoef[1];
					iMinStart = i;
				}
				else
				{
					if (fabs(dMinSlope) > fabs(dCurCoef[1]))
					{
						dMinSlope = dCurCoef[1];
						iMinStart = i;
					}
				}

				if (!bFindFirstSlope)
				{
					if (dCurCoef[1] < 10)
					{
						fFirstSlope = dCurCoef[1];
						iOutEnd = i;
						bFindFirstSlope = TRUE;
						bFindPlateau = TRUE;
					}
				}
				else
				{
					if (fabs(dCurCoef[1]) < 10)
						continue;

					if (fabs(dCurCoef[1]-fFirstSlope) > fabs(fFirstSlope) * 0.5)
					{
						iOutStart = i-iWndWidth+1;
						break;
					}
				}
			}

			if (!bFindPlateau)
			{
				// 根据最小斜率的位置向前向后找接近斜率的位
				BOOL bFindStart = FALSE;
				for (int i=iMinStart-1; i>=iOutStart+iWndWidth; i--)
				{
					for (int j=0; j<iWndWidth; j++)
					{
						pdTempX[j] = pdInputX[i-iWndWidth+1+j];
						pdTempY[j] = pdInputY[i-iWndWidth+1+j];
					}
					LinearFit(pdTempX, pdTempY, iWndWidth, dCurCoef);
					if (fabs(dCurCoef[1]-dMinSlope) > fabs(dMinSlope) * 0.5)
					{
						iOutStart = i-iWndWidth+1;
						bFindStart = TRUE;
						break;
					}
				}
				if (!bFindStart)
					iOutStart = iMinStart - iWndWidth + 1;

				BOOL bFindEnd = FALSE;
				for (int i=iMinStart+1; i<=iOutEnd; i++)
				{
					for (int j=0; j<iWndWidth; j++)
					{
						pdTempX[j] = pdInputX[i-iWndWidth+1+j];
						pdTempY[j] = pdInputY[i-iWndWidth+1+j];
					}
					LinearFit(pdTempX, pdTempY, iWndWidth, dCurCoef);
					if (fabs(dCurCoef[1]-dMinSlope) > fabs(dMinSlope) * 0.5)
					{
						iOutEnd = i;
						bFindEnd = TRUE;
						break;
					}
				}
				if (!bFindEnd)
					iOutEnd = iMinStart;
			}

			// 反向修正基线范围
			bFindFirstSlope = FALSE;
			iWndWidth = (iOutEnd - iOutStart)/3;
			if (iWndWidth < 3)
				iWndWidth = 3;
			for (int i=iOutStart; i<=iOutEnd-iWndWidth+1; i++)
			{
				for (int j=0; j<iWndWidth; j++)
				{
					pdTempX[j] = pdInputX[i+j];
					pdTempY[j] = pdInputY[i+j];
				}
				LinearFit(pdTempX, pdTempY, iWndWidth, dCurCoef);

				if (i == iOutStart)
				{
					fFirstSlope = dCurCoef[1];
					iMinStart = i;
				}
				else
				{
					if (fabs(dCurCoef[1]) < 10)
						continue;
				}

				if (fabs(dCurCoef[1]-fFirstSlope) > fabs(fFirstSlope) * 0.5)
				{
					iOutEnd = i+iWndWidth-1;
					break;
				}
			}



			delete pdTempX;
			delete pdTempY;
		}
	}
	else
	{
		if (nPtCount > 20)
		{
			iOutStart = 3;
			iOutEnd = nPtCount - 3;
			iOutStart = iOutEnd - nPtCount * 2/3;
		}
		else
		{
			iOutStart = 1;
			iOutEnd = nPtCount;
		}
	}
}


// 根据Ct位置计算基线位置
void CDataAlgorithmPCR::CalcBaselinePosByCt(int nPosMax, int &iOutStart, int &iOutEnd)
{
	int iTempStart = 3;
	int iTempEnd = nPosMax - 5;
	if (iTempEnd <= iTempStart)
	{
		iTempStart = 1;
		if (iTempEnd - iTempStart < 3)
		{
			iTempEnd += 3;
		}
		if (iTempEnd < iTempStart)
		{
			iTempEnd = iTempStart;
		}
	}
	else  if (iTempEnd - iTempStart < 3)
	{
		iTempStart = 1;
		if (iTempEnd - iTempStart < 5)
		{
			iTempEnd += 1;
		}
	}
	else if (iTempEnd - iTempStart < 5)
	{
		iTempStart -= 1;
	}

	iOutStart = iTempStart;
	iOutEnd = iTempEnd;
}


// 根据基线位置计算基线均值
double CDataAlgorithmPCR::CalcBaselineAverage(int nPtCount, double *pInputX, double *pInputY, int iStart, int iEnd)
{
	if(nPtCount < 1 || iStart < 0  || iEnd < 0)
	{
		return 0;
	}

	if(iStart >= nPtCount)
	{
		iStart = 1; 
	}
	if(iEnd >= nPtCount)
	{
		iEnd = nPtCount;
	}
	if(iStart > iEnd)
	{
		iStart = 1; 
		iEnd = nPtCount;
	}

	double dBaselineSum = 0;
	for(int i=iStart-1; i<iEnd; i++)
	{
		dBaselineSum += pInputY[i];
	}

	double dReturn = dBaselineSum / (iEnd - iStart + 1);
	return dReturn;
}

// 计算一阶导数（增量）
void CDataAlgorithmPCR::CalcFirstDerivative(int iPtCount, double *pdInputY, double* pdOutputY)
{
	pdOutputY[0] = 0;
	for(int i = 1; i<iPtCount; i++)
	{
		pdOutputY[i] = pdInputY[i] - pdInputY[i-1];
	}
}

// 计算二阶导数（二次增量）
void CDataAlgorithmPCR::CalcSecondDerivative(int iPtCount, double *pdInputY, double* pdOutputY)
{
	double *dy = new double[iPtCount];

	CalcFirstDerivative(iPtCount, pdInputY, dy);

	pdOutputY[0] = 0;
	for(int i=1; i<iPtCount ; i++)
	{
		pdOutputY[i] = dy[i] - dy[i-1];
	}

	delete [] dy;
}

// 修改函数，在二阶导数中使用宽度为6的窗口找出最大和最大的位置，然后在一阶导数相应位置开始找出连续大于0的最大值位置，反向找出连续下降的开始位置
double CDataAlgorithmPCR::FindCtPosWithParabola(int iPtCount, double* pdInputX, double *pdInputY, double* pdFirstDer, double* pdSecondDer)
{
	int iWinWidth = 6;
	int iMaxSumStartPos = -1;
	double dMaxSum = 0;
	for (int i=0; i<iPtCount-iWinWidth; i++)
	{
		double dSum = 0;
		for (int j=0; j<iWinWidth; j++)
		{
			dSum += pdSecondDer[i+j];
		}

		if (dMaxSum < dSum)
		{
			dMaxSum = dSum;
			iMaxSumStartPos = i;
		}
	}
	if (iMaxSumStartPos == -1) // 不存在合规的Ct
		return 0;

	double dMaxFirstDer = pdFirstDer[iMaxSumStartPos];
	int iMaxPos = iMaxSumStartPos;
	//for (int i=iMaxSumStartPos; i<iPtCount; i++)
	for (int i=iMaxSumStartPos; i<iMaxSumStartPos+iWinWidth; i++)
	{
		if (pdFirstDer[i] < 0)
			break;

		if (pdFirstDer[i] > dMaxFirstDer)
		{
			dMaxFirstDer = pdFirstDer[i];
			iMaxPos = i;
		}
	}
	int iMinPos = iMaxSumStartPos;
	for (int i=iMaxPos-1; i>=iMaxSumStartPos; i--)
	{
		if (pdFirstDer[i] > pdFirstDer[i+1])
		{
			iMinPos = i;
			break;
		}
	}
	//ASSERT(iMinPos == iMaxSumStartPos);

	double dmaxTemp = pdSecondDer[0];
	for (int t= 0 ;t < iPtCount;t++)
	{
		if(pdSecondDer[t] < 0)
		{
			pdSecondDer[t] = 0;
		}
	}

	double dMaxPosX = 0;
	int iStartPos = iMinPos;
	do 
	{
		dMaxPosX = FindCrestWithParabola(pdInputX+iStartPos, pdSecondDer+iStartPos, iPtCount-iStartPos, iWinWidth);
		int iPosMax = int(dMaxPosX + 0.5) + iStartPos;
		if (pdFirstDer[iPosMax] > 0 /* && iPosMax > iMinPos  && IsLegalCtByFluo(iPtCount, pdInputX, pdInputY, iPosMax)*/) 
		{
			// 加上最大二阶导数值限制，现在数据不使用归一化数据，增加增量限制
			double dTempSum = 0;
			for (int xx=iPosMax-iWinWidth/2; xx<iPosMax+iWinWidth/2; xx++)
			{
				dTempSum += pdSecondDer[xx];
			}

			if (dTempSum > 100)
			{
				dMaxPosX += iStartPos;
				break;
			}
			else
			{
				iStartPos = iPosMax;
				dMaxPosX = 0;
			}
		}
		else
		{
			iStartPos = iPosMax;
			dMaxPosX = 0;
		}
	} while (iStartPos < iPtCount - 5);


	return dMaxPosX;
}


double CDataAlgorithmPCR::CalcCtPosBySndDerivative(int iPtCount, double* pdInputX, double* pdInputY)
{
	double *pdTempTemp = new double [iPtCount];
	for( int k=0; k< iPtCount; k++)
	{
		pdTempTemp[k] = pdInputY[k];
	}

	//计算一阶导数和二阶导数
	double* pdFirstDer = new double[iPtCount];
	CalcFirstDerivative(iPtCount, pdTempTemp, pdFirstDer);
	//OutputPointerData(iPtCount, pdFirstDer);
	double* pdSecondDer = new double[iPtCount];
	CalcSecondDerivative(iPtCount, pdTempTemp, pdSecondDer);
	delete [] pdTempTemp;
	//OutputPointerData(iPtCount, pdSecondDer);

	//计算二阶导数正值的最大值
	double dMaxPosX = FindCtPosWithParabola(iPtCount, pdInputX, pdInputY, pdFirstDer, pdSecondDer);

	int iPosMax = int(dMaxPosX + 0.5);
	double dMaxY = pdSecondDer[iPosMax-1];
	delete pdFirstDer;
	delete pdSecondDer;

	BOOL bAble = FALSE;
	if(iPosMax > 3 && (dMaxPosX < m_dd2MaxXThre*iPtCount) && dMaxY > m_dd2MaxYThre)
	{
		if(iPtCount > 5)
		{
			bAble = TRUE;
			//if(pdInputY[iPtCount-1] > pdInputY[0] && pdInputY[iPtCount-1] > pdInputY[2] && pdInputY[iPtCount-2] > pdInputY[0] && pdInputY[iPtCount-2] > pdInputY[2])
			//{
			//	bAble = TRUE;
			//}
		}
	}

	if (!bAble)
		dMaxPosX = 0;

	return dMaxPosX;
}

//最大二阶导数自动计算Ct
double CDataAlgorithmPCR::GenerateCtOfSndDerivative(int iCycleCount, double *pdInputX, double *pdInputY, int iFilterCount)
{
	if(iCycleCount < 1 || NULL == pdInputX || NULL == pdInputY)
	{
		return -1;
	}

	double *pdReturn = new double[iCycleCount];
	double* pdTempY = new double[iCycleCount];
	memcpy(pdTempY, pdInputY, sizeof(double)*iCycleCount);
	//滤波
	for (int i=0; i<iFilterCount; i++)
	{
		DigitalFilter(pdTempY, pdReturn, iCycleCount);
		memcpy(pdTempY, pdReturn, sizeof(double)*iCycleCount);
	}

	//计算二阶导数
	NewCalDerivative(pdInputX, pdTempY, iCycleCount, 2, pdReturn);
	//计算最大值
	double dCt = FindCrestWithParabola(pdInputX, pdReturn, iCycleCount, 6);

	delete [] pdReturn;
	delete [] pdTempY;

	return dCt;
}

//根据确定的荧光阈值计算ct值
double CDataAlgorithmPCR::GenerateCtOfSpline(int iCycleCount, double *pdInputX, double *pdInputY, double dThreshold)
{
	double* pdTempY = new double[iCycleCount];
	double *pdReturn = new double[iCycleCount];

	//两次滤波
	DigitalFilter(pdInputY, pdTempY, iCycleCount);
	DigitalFilter(pdTempY, pdReturn, iCycleCount);

	double dCt = 0.0;
	SplineEx(pdReturn, pdInputX, iCycleCount, dThreshold, dCt);
	if (dCt < 3 || dCt > iCycleCount -1)
	{
		dCt = 0;
	}
	// 修正差值法的BUG
	int iCtPos = int(dCt);
	if ( (pdInputY[iCtPos+1] < dThreshold && pdInputY[iCtPos] < dThreshold))
	{
		dCt = 0;
	}
	else
	{
		BOOL bFindAfterCtLessThreshold = FALSE;
		for (int i=iCtPos+1; i<5; i++)
		{
			if (i>= iCycleCount)
				break;
			if (pdInputY[i] < dThreshold)
			{
				bFindAfterCtLessThreshold = TRUE;
				break;
			}
		}

		BOOL bFindBeforeCtMoreThreshold = FALSE;
		for (int i=iCtPos-5; i<iCtPos; i++)
		{
			if (i < 0)
				continue;
			if (pdInputY[i] > dThreshold)
			{
				bFindBeforeCtMoreThreshold = TRUE;
				break;
			}
		}

		if (bFindAfterCtLessThreshold || bFindBeforeCtMoreThreshold)
			dCt = 0;
	}

	delete [] pdTempY;
	delete [] pdReturn;

	return dCt;
}


void CDataAlgorithmPCR::CalcBaselinePosByLineFit(int nPtCount, double *pInputX, double *pInputY, int iEndPos, double dMinR, int &iOutStart, int &iOutEnd)
{
	int iStart = 3;
	int iEnd = iEndPos-3;
	if (iEnd - iStart < 3)
	{
		iStart = 1;
		iEnd = iEndPos;
	}

	double* pdTempX = new double[nPtCount];
	double* pdTempY = new double[nPtCount];

	BOOL bFindLegal = FALSE;
	int iTempCount = iEnd - iStart + 1;
	int iTempStart = iStart-1;
	double dTempCoef[3] = {0};

	while (TRUE)
	{
		if (iTempCount < 4)
			break;

		for(int i=0; i<iTempCount; i++)
		{
			pdTempX[i] = pInputX[i+iTempStart];
			pdTempY[i] = pInputY[i+iTempStart];
		}

		LinearFit(pdTempX, pdTempY, iTempCount, dTempCoef);

		if (dTempCoef[2]*dTempCoef[2] > dMinR)
		{
			iOutStart = iTempStart + 1;
			iOutEnd = iTempStart + iTempCount;
			bFindLegal = TRUE;
			break;
		}

		iTempStart += 1;
		iTempCount -= 1;
	}

	if (pdTempX != NULL)
	{
		delete pdTempX;
		pdTempX = NULL;
	}
	if (pdTempY != NULL)
	{
		delete pdTempY;
		pdTempY = NULL;
	}

	if (!bFindLegal) // 没有基线
	{
		iOutStart = -1;
		iOutEnd = -1;
	}
}

void CDataAlgorithmPCR::CalcBaselinePosByDeviations(int nPtCount, double *pInputX, double *pInputY, int iEndPos, double dMinR, int &iOutStart, int &iOutEnd)
{
	int iStart = 3;
	int iEnd = iEndPos-3;
	if (iEnd - iStart < 3)
	{
		iStart = 1;
		iEnd = iEndPos;
	}

	double dSum = 0;
	for (int i=iStart-1; i<iEnd; i++)
	{
		dSum += pInputY[i];
	}

	double dAverage = dSum / (iEnd - iStart + 1);

	for (int i=iStart; i<=iEnd; i++)
	{
		if (fabs(pInputY[i] - pInputY[i-1]) > dAverage * 0.1)
		{
			iOutStart = i;
		}
		else
		{
			iOutStart = i;
			break;
		}
	}

	iOutEnd = iEnd;
}

int CDataAlgorithmPCR::FindCtPosBySlope(int nPtCount, double *pInputX, double *pInputY, int iWndWidth)
{
	int iFindCt = 0;
	double dPreCoef[3] = {0};
	double dCurCoef[3] = {0};
	double* pdTempX = new double[iWndWidth];
	double* pdTempY = new double[iWndWidth];
	double dMaxSlope = 0;


	for (int i=3; i<nPtCount-iWndWidth+1; i++)
	{
		BOOL bContinueRise = TRUE;
		for (int j=0; j<iWndWidth-1; j++)
		{
			if (pInputY[j+i+1] < pInputY[j+i])
			{
				bContinueRise = FALSE;
				break;
			}
		}

		if (bContinueRise) // 计算前后点的斜率
		{
			int iTempCount = iWndWidth;
			for(int ii=0; ii<iTempCount; ii++)
			{
				pdTempX[ii] = pInputX[ii+i];
				pdTempY[ii] = pInputY[ii+i];
			}
			LinearFit(pdTempX, pdTempY, iTempCount, dCurCoef);
			if (dCurCoef[1] < 50)
				continue;

			if (i < iWndWidth)
			{
				iTempCount = i+1;
			}

			for(int ii=0; ii<iTempCount; ii++)
			{
				pdTempX[ii] = pInputX[i-iTempCount+1+ii];
				pdTempY[ii] = pInputY[i-iTempCount+1+ii];
			}
			LinearFit(pdTempX, pdTempY, iTempCount, dPreCoef);


			if (dCurCoef[1] - dPreCoef[1] > dMaxSlope)
			{
				dMaxSlope = dCurCoef[1] - dPreCoef[1];
				iFindCt = i+3;
			}
		}
	}

	delete pdTempX;
	delete pdTempY;

	if (dMaxSlope < 50)
		iFindCt = 0;

	return iFindCt;
}

BOOL CDataAlgorithmPCR::IsLegalCtByFluo(int nPtCount, double *pInputX, double *pInputY, int iCtPos)
{
	BOOL bReturn = FALSE;
	if (iCtPos < nPtCount / 3)
	{



	}
	else
	{
		bReturn = TRUE;
	}

	return bReturn;
}


double CDataAlgorithmPCR::CalcCtPos(int iPtCount, double* pdInputX, double* pdInputY)
{
	double dCt = CalcCtPosBySndDerivative(iPtCount, pdInputX, pdInputY);
	if (dCt < 5 || dCt > iPtCount-3)
	{
		dCt = FindCtPosBySlope(iPtCount, pdInputX, pdInputY);
	}

	return dCt;
}

void CDataAlgorithmPCR::OutputPointerData(int iDataCount, double* pOutData)
{
#ifdef _DEBUG
	for (int i=0; i<iDataCount; i++)
	{
		TRACE(_T("%.3f\n"),  pOutData[i]);
	}
#endif
}

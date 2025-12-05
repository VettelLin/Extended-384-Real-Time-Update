
#pragma once

#include <string>
#include <vector>
#include <tchar.h>
#include <fstream>
#include <math.h>
#include <GdiPlus.h>

#include "LegendInfo.h"

using namespace std;
using namespace Gdiplus;

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

CString GetExeFileDirectory();

void GetCurrentTmTime(CString& tmString);

wstring GetCurrentTmTimeW();

string GetCurrentTmTimeA();

CStringA GetLisCurrentTmTimeA();

time_t GetCurrentTmTime(CString& strTmTime,int format = 1,CString yMDSeparator = _T("/"),CString hMSSeparator = _T(":"),CString dHSeparator = _T(" "));

CString GetCurrentTmTime(int format = 1,CString yMDSeparator = _T("/"),CString hMSSeparator = _T(":"),CString dHSeparator = _T(" "));

CString TotalSecondToHourMinSec(int totalSec);

template<typename T>
inline int round(T r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

template<typename T>
/*inline*/ int MyFloor(T r)
{
	return r + 0.5;
}

template<typename T>
int GetMinMaxValue(T* pData,int dataLen,T& minValue,T& maxValue)
{
	if(NULL == pData)
	{
		return -1;
	}
	if(dataLen < 1)
	{
		return -2;
	}

	minValue = maxValue = pData[0];
	for(int i = 1;i < dataLen;++i)
	{
		if(pData[i] > maxValue)
		{
			maxValue = pData[i];
		}
		else if(pData[i] < minValue)
		{
			minValue = pData[i];
		}
	}

	return 0;
}

void DrawRoundRectangle(CDC* pDC,float x,float y,float width,float height,float arcSize,Color lineColor,float lineWidth,BOOL bFillPath,Color fillColor,int rountType = 1);	//roundType 1--四角圆角2--上下圆角3--左右圆角 4--左右半圆角

void DrawRoundRectangleNew(CDC* pDC,float x,float y,float width,float height,float arcSizeLT,float arcSizeRT,float arcSizeRB,float arcSizeLB,BOOL bShowLine,Color lineColor,float lineWidth,BOOL bFillPath,Color fillColor,int rountType = 1);	//四角使用不同圆角

void DrawGradientRoundRectangle(CDC* pDC,float x,float y,float width,float height,float arcSizeLT,float arcSizeRT,float arcSizeRB,float arcSizeLB,BOOL bShowLine,Color lineColor,float lineWidth,BOOL bFillPath,Color clrFillStart,Color clrFillEnd,int rountType = 1);

void DrawRound(CDC* pDC,CRect rect,BOOL bLine,Color lineColor,float lineWidth,BOOL bFillPath,Color fillColor);

int GetSystemComNames(vector<CString>& vecComName);

int SiteStrToSamId(CString strSite);

int CheckAndCreateFileOrDirPath(wstring fileDirPath);

BOOL ValidateFileOrDirWChar(const wchar_t& fileDirWChar);

CString SampleIdToHoleInfo(int samId);

void IndexToRowColId(int index,int& rowId,int& colId);

CString SecondToHourMinuteSecond(int iTotalSec);

int DrawLegend(CDC* pDc,CRect rcLegend,CLegendInfo& lgdInfo);

void KdsptForwardMBackN_New(int n, double *pY, double *pReturn,int forwardM,int backwardN);

int LeastSquare(double *pDx,double *pDy,int dataNum,double &dSlope,double &dIntercept);	//最小二乘法拟合K、B值

CString GetFileNameFromPath(CString strFilePath);

int GetIDPD(CString strData);	//获取字符串的小数位数

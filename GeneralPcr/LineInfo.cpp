#include "StdAfx.h"
#include "LineInfo.h"

COLORREF CLineInfo::m_staChanClr[6] = {RGB(0X33,0X83,0XFD),RGB(0XFF,0X8F,0X2B),RGB(0X20,0X9F,0X85),RGB(0XF2,0X31,0X31),RGB(0XF9,0X61,0X61),RGB(0XFA,0X91,0X91)};

CLineInfo::CLineInfo(void)
{
	m_tubeId = 0;
	m_rowId = 0;
	m_colId = 0;
	m_chanId = 0;
	m_bShow = FALSE;
	m_lineClr = RGB(0,0,0);
	m_bShowExpression = FALSE;
	m_strDyeName = _T("");
	m_strTaget = _T("");
	m_strHoleTag = _T("");

	m_dEfficiency = 0;
	m_bXAxisMsTime = FALSE;
}

CLineInfo::~CLineInfo(void)
{
}

CLineInfo::CLineInfo(const CLineInfo& lineInfo)
{
	m_tubeId = lineInfo.m_tubeId;
	m_chanId = lineInfo.m_chanId;
	m_bShow = lineInfo.m_bShow;
	m_lnkDots = lineInfo.m_lnkDots;
	m_lineClr = lineInfo.m_lineClr;
	
	m_rowId = lineInfo.m_rowId;
	m_colId = lineInfo.m_colId;

	m_strDyeName = lineInfo.m_strDyeName;
	m_strTaget = lineInfo.m_strTaget;
	m_strHoleTag = lineInfo.m_strHoleTag;

	m_dEfficiency = lineInfo.m_dEfficiency; 

	if(NULL != m_linearFitData.pDataX)
	{
		delete [] m_linearFitData.pDataX;
	}
	if(NULL != m_linearFitData.pDataY)
	{
		delete [] m_linearFitData.pDataY;
	}
	m_linearFitData.xLen = lineInfo.m_linearFitData.xLen;
	m_linearFitData.yLen = lineInfo.m_linearFitData.yLen;
	m_linearFitData.intercept = lineInfo.m_linearFitData.intercept;
	m_linearFitData.rSquare = lineInfo.m_linearFitData.rSquare;
	m_linearFitData.slope = lineInfo.m_linearFitData.slope;
	if(m_linearFitData.xLen > 0 && m_linearFitData.xLen == m_linearFitData.yLen)
	{
		m_linearFitData.pDataX = new double[m_linearFitData.xLen];
		if(NULL == m_linearFitData.pDataX)
		{
			return;
		}
		m_linearFitData.pDataY = new double[m_linearFitData.yLen];
		if(NULL == m_linearFitData.pDataY)
		{
			return;
		}

		for(int i = 0;i < m_linearFitData.xLen;++i)
		{
			m_linearFitData.pDataX[i] = lineInfo.m_linearFitData.pDataX[i];
			m_linearFitData.pDataY[i] = lineInfo.m_linearFitData.pDataY[i];
		}
	}

	m_linearMinPt = lineInfo.m_linearMinPt;
	m_linearMaxPt = lineInfo.m_linearMaxPt;
	m_bShowExpression = lineInfo.m_bShowExpression;	
	m_bXAxisMsTime = lineInfo.m_bXAxisMsTime;
}

void CLineInfo::operator=(const CLineInfo& lineInfo)
{
	m_tubeId = lineInfo.m_tubeId;
	m_chanId = lineInfo.m_chanId;
	m_bShow = lineInfo.m_bShow;
	m_lnkDots = lineInfo.m_lnkDots;
	m_lineClr = lineInfo.m_lineClr;

	m_rowId = lineInfo.m_rowId;
	m_colId = lineInfo.m_colId;

	m_strDyeName = lineInfo.m_strDyeName;
	m_strTaget = lineInfo.m_strTaget;
	m_strHoleTag = lineInfo.m_strHoleTag;

	m_dEfficiency = lineInfo.m_dEfficiency; 

	if(NULL != m_linearFitData.pDataX)
	{
		delete [] m_linearFitData.pDataX;
	}
	if(NULL != m_linearFitData.pDataY)
	{
		delete [] m_linearFitData.pDataY;
	}
	m_linearFitData.xLen = lineInfo.m_linearFitData.xLen;
	m_linearFitData.yLen = lineInfo.m_linearFitData.yLen;
	m_linearFitData.intercept = lineInfo.m_linearFitData.intercept;
	m_linearFitData.rSquare = lineInfo.m_linearFitData.rSquare;
	m_linearFitData.slope = lineInfo.m_linearFitData.slope;
	if(m_linearFitData.xLen > 0 && m_linearFitData.xLen == m_linearFitData.yLen)
	{
		m_linearFitData.pDataX = new double[m_linearFitData.xLen];
		if(NULL == m_linearFitData.pDataX)
		{
			return;
		}
		m_linearFitData.pDataY = new double[m_linearFitData.yLen];
		if(NULL == m_linearFitData.pDataY)
		{
			return;
		}

		for(int i = 0;i < m_linearFitData.xLen;++i)
		{
			m_linearFitData.pDataX[i] = lineInfo.m_linearFitData.pDataX[i];
			m_linearFitData.pDataY[i] = lineInfo.m_linearFitData.pDataY[i];
		}
	}

	m_linearMinPt = lineInfo.m_linearMinPt;
	m_linearMaxPt = lineInfo.m_linearMaxPt;
	m_bShowExpression = lineInfo.m_bShowExpression;	
	m_bXAxisMsTime = lineInfo.m_bXAxisMsTime;
}

void CLineInfo::AddDot(CMyPoint& pt)
{
	m_lnkDots.AddTail(pt);
}

int CLineInfo::SetLinearFitData(LINEARFITDATA* pLinearFitData)
{
	if(NULL == pLinearFitData)
	{
		return -1;
	}
	if(NULL == pLinearFitData->pDataX)
	{
		return -2;
	}
	if(NULL == pLinearFitData->pDataY)
	{
		return -3;
	}
	if(pLinearFitData->xLen < 2)
	{
		return -4;
	}
	if(pLinearFitData->yLen < 2)
	{
		return -5;
	}
	if(pLinearFitData->xLen != pLinearFitData->yLen)
	{
		return -6;
	}

	MyDeletePtr(&m_linearFitData.pDataX);
	MyDeletePtr(&m_linearFitData.pDataY);

	m_linearFitData.pDataX = new double[pLinearFitData->xLen];
	if(NULL == m_linearFitData.pDataX)
	{
		return -7;
	}
	m_linearFitData.pDataY = new double[pLinearFitData->xLen];
	if(NULL == m_linearFitData.pDataY)
	{
		MyDeletePtr(&m_linearFitData.pDataX);
		return -8;
	}

	m_linearFitData.xLen = m_linearFitData.yLen = pLinearFitData->xLen;
	m_linearFitData.intercept = pLinearFitData->intercept;
	m_linearFitData.rSquare = pLinearFitData->rSquare;
	m_linearFitData.slope = pLinearFitData->slope;

	if(0 != m_linearFitData.slope)
	{
		m_dEfficiency = pow(10.0,-1 / m_linearFitData.slope) - 1;
	}

	for(int i = 0;i < pLinearFitData->xLen;++i)
	{
		m_linearFitData.pDataX[i] = pLinearFitData->pDataX[i];
		m_linearFitData.pDataY[i] = pLinearFitData->pDataY[i];
	}

	//计算X最大值、浓度最小值
	GetMinMaxValue(m_linearFitData.pDataX,m_linearFitData.xLen,m_linearMinPt.m_X,m_linearMaxPt.m_X);
	//计算Y最大值、最小值
	GetMinMaxValue(m_linearFitData.pDataY,m_linearFitData.xLen,m_linearMinPt.m_Y,m_linearMaxPt.m_Y);

	m_linearMaxPt.m_Y = m_linearMaxPt.m_X * m_linearFitData.slope + m_linearFitData.intercept;
	m_linearMinPt.m_Y = m_linearMinPt.m_X * m_linearFitData.slope + m_linearFitData.intercept;

	return 0;
}

int CLineInfo::SetLinearFitData(CLinearFitData* pLinearFitData)
{
	if(NULL == pLinearFitData)
	{
		return -1;
	}
	if(NULL == pLinearFitData->m_pDX)
	{
		return -2;
	}
	if(NULL == pLinearFitData->m_pDY)
	{
		return -3;
	}
	if(pLinearFitData->m_iDataNum < 2)
	{
		return -4;
	}

	MyMulitDeletePtr(&m_linearFitData.pDataX);
	MyMulitDeletePtr(&m_linearFitData.pDataY);

	m_linearFitData.pDataX = new double[pLinearFitData->m_iDataNum];
	if(NULL == m_linearFitData.pDataX)
	{
		return -7;
	}
	m_linearFitData.pDataY = new double[pLinearFitData->m_iDataNum];
	if(NULL == m_linearFitData.pDataY)
	{
		MyMulitDeletePtr(&m_linearFitData.pDataX);
		return -8;
	}

	m_linearFitData.xLen = m_linearFitData.yLen = pLinearFitData->m_iDataNum;
	m_linearFitData.intercept = pLinearFitData->m_dIntercept;
	m_linearFitData.rSquare = pLinearFitData->m_dSquare;
	m_linearFitData.slope = pLinearFitData->m_dSlope;

	if(0 != m_linearFitData.slope)
	{
		m_dEfficiency = pow(10.0,-1 / m_linearFitData.slope) - 1;
	}

	for(int i = 0;i < pLinearFitData->m_iDataNum;++i)
	{
		m_linearFitData.pDataX[i] = pLinearFitData->m_pDX[i];
		m_linearFitData.pDataY[i] = pLinearFitData->m_pDY[i];
	}

	//计算X最大值、浓度最小值
	GetMinMaxValue(m_linearFitData.pDataX,m_linearFitData.xLen,m_linearMinPt.m_X,m_linearMaxPt.m_X);
	//计算Y最大值、最小值
	GetMinMaxValue(m_linearFitData.pDataY,m_linearFitData.xLen,m_linearMinPt.m_Y,m_linearMaxPt.m_Y);

	m_linearMaxPt.m_Y = m_linearMaxPt.m_X * m_linearFitData.slope + m_linearFitData.intercept;
	m_linearMinPt.m_Y = m_linearMinPt.m_X * m_linearFitData.slope + m_linearFitData.intercept;

	return 0;
}

void CLineInfo::SetExpressionShowState(BOOL bShowExpression)
{
	m_bShowExpression = bShowExpression;
}

BOOL CLineInfo::GetExpressionShowState()
{
	return m_bShowExpression;
}

BOOL CLineInfo::BeHaveFitData()
{
	if(NULL == m_linearFitData.pDataX || m_linearFitData.xLen < 2)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CLineInfo::GetDotXYMinMax(double& minX,double& maxX,double& minY,double& maxY)
{
	if(NULL == m_linearFitData.pDataX || m_linearFitData.xLen < 2)
	{
		return FALSE;
	}

	minX = m_linearFitData.pDataX[0];
	maxX = m_linearFitData.pDataX[0]; 
	minY = m_linearFitData.pDataY[0]; 
	maxY = m_linearFitData.pDataY[0]; 

	for(int i = 1;i < m_linearFitData.xLen;++i)
	{
		if(minX > m_linearFitData.pDataX[i])
		{
			minX = m_linearFitData.pDataX[i];
		}
		if(maxX < m_linearFitData.pDataX[i])
		{
			maxX = m_linearFitData.pDataX[i];
		}
		if(minY > m_linearFitData.pDataY[i])
		{
			minY = m_linearFitData.pDataY[i];
		}
		if(maxY < m_linearFitData.pDataY[i])
		{
			maxY = m_linearFitData.pDataY[i];
		}
	}

	return TRUE;
}

BOOL CLineInfo::GetLineXYMinMax(double& minX,double& maxX,double& minY,double& maxY)
{
	if(NULL == m_linearFitData.pDataX || m_linearFitData.xLen < 2)
	{
		return FALSE;
	}

	minX = m_linearMinPt.m_X;
	maxX = m_linearMaxPt.m_X; 
	minY = m_linearMinPt.m_Y; 
	maxY = m_linearMaxPt.m_Y; 

	if(maxY < minY)
	{
		double tempY = minY;
		minY = maxY;
		maxY = tempY;
	}

	return TRUE;
}

void CLineInfo::SetChanId(int iChanId)
{
	m_chanId = iChanId;
}

int CLineInfo::GetChanId()
{
	return m_chanId;
}

void CLineInfo::SetDyeName(CString strData)
{
	m_strDyeName = strData;
}

CString CLineInfo::GetDyeName()
{
	return m_strDyeName;
}

BOOL CLineInfo::GetGeneralDotMinMax(double& minX,double& maxX,double& minY,double& maxY)
{
	if(m_lnkDots.GetSize() <= 0)
	{
		return FALSE;
	}

	Node<CMyPoint>* pNode = m_lnkDots.GetHead();

	minX = pNode->data.m_X;
	maxX = pNode->data.m_X; 
	minY = pNode->data.m_Y; 
	maxY = pNode->data.m_Y;

	pNode = pNode->pNext;
	while(NULL != pNode)
	{
		if(minX > pNode->data.m_X)
		{
			minX = pNode->data.m_X;
		}
		if(maxX < pNode->data.m_X)
		{
			maxX = pNode->data.m_X;
		}
		if(minY > pNode->data.m_Y)
		{
			minY = pNode->data.m_Y;
		}
		if(maxY < pNode->data.m_Y)
		{
			maxY = pNode->data.m_Y;
		}

#ifdef _DEBUG
		if(pNode->data.m_Y < -15)
		{
			int tt = 0;
			++tt;
		}
#endif
		pNode = pNode->pNext;
	}

	return TRUE;
}

COLORREF CLineInfo::GetLineClr(ELINECLRMODE eGetLineClr)
{
	COLORREF clrLine = 0;

	if(ECM_BYCHANNEL == eGetLineClr)
	{
		if(m_chanId < 0 || m_chanId >= 6)
		{
			return clrLine;
		}

		clrLine = m_staChanClr[m_chanId];
	}
	else if(ECM_BYROW == eGetLineClr)
	{
		if(0 == m_rowId)
		{
			clrLine = RGB(27,125,245);
		}
		else if(1 == m_rowId)
		{
			clrLine = RGB(31,181,88);
		}
		else if(2 == m_rowId)
		{
			clrLine = RGB(209,56,204);
		}
		else if(3 == m_rowId)
		{
			clrLine = RGB(250,144,22);
		}
		if(4 == m_rowId)
		{
			clrLine = RGB(19,191,191);
		}
		else if(5 == m_rowId)
		{
			clrLine = RGB(142,90,215);
		}
		else if(6 == m_rowId)
		{
			clrLine = RGB(250,55,65);
		}
		else if(7 == m_rowId)
		{
			clrLine = RGB(140,140,141);
		}
	}
	else if(ECM_BYCOL == eGetLineClr)
	{
		if(0 == m_colId)
		{
			clrLine = RGB(27,125,245);
		}
		else if(1 == m_colId)
		{
			clrLine = RGB(31,181,88);
		}
		else if(2 == m_colId)
		{
			clrLine = RGB(209,56,204);
		}
		else if(3 == m_colId)
		{
			clrLine = RGB(250,144,22);
		}
		if(4 == m_colId)
		{
			clrLine = RGB(19,191,191);
		}
		else if(5 == m_colId)
		{
			clrLine = RGB(142,90,215);
		}
		else if(6 == m_colId)
		{
			clrLine = RGB(250,55,65);
		}
		else if(7 == m_colId)
		{
			clrLine = RGB(140,140,141);
		}
		else if(8 == m_colId)
		{
			clrLine = RGB(250,219,20);
		}
		else if(9 == m_colId)
		{
			clrLine = RGB(67,74,222);
		}
		else if(10 == m_colId)
		{
			clrLine = RGB(250,100,49);
		}
		else if(11 == m_colId)
		{
			clrLine = RGB(77,106,176);
		}
	}
	else if(ECM_BYTARGET == eGetLineClr)
	{
		if(0 == m_chanId)
		{
			clrLine = RGB(69,159,255);
		}
		else if(1 == m_chanId)
		{
			clrLine = RGB(65,196,106);
		}
		else if(2 == m_chanId)
		{
			clrLine = RGB(227,100,218);
		}
		else if(3 == m_chanId)
		{
			clrLine = RGB(250,167,57);
		}
	}
	else
	{
		clrLine = m_lineClr;
	}

	return clrLine;
}

void CLineInfo::SetBeXAxisMillisecondTime(BOOL bXAxisMsTime)
{
	m_bXAxisMsTime = bXAxisMsTime;
}

BOOL CLineInfo::GetBeXAxisMillisecondTime()
{
	return m_bXAxisMsTime;
}

CString CLineInfo::GetEfficiencyString()
{
	CString strTemp(_T(""));
	strTemp.Format(_T("%.2lf%%"),m_dEfficiency * 100);
	return strTemp;
}

CString CLineInfo::GetExpressionString()
{
	if(!BeHaveFitData())
	{
		return _T("");
	}

	CString strExpression(_T("")),strSign(_T("+"));
	if(m_linearFitData.intercept < 0)
	{
		strSign = _T("-");
	}

	if(!m_strDyeName.IsEmpty())
	{
		strExpression = m_strDyeName + _T(": ");
	}

	if(0 != m_linearFitData.intercept)
	{
		strExpression.AppendFormat(_T("y = %.2lf * x %s %.2lf R = %.5lf  eff = %s"),m_linearFitData.slope,strSign,abs(m_linearFitData.intercept),m_linearFitData.rSquare,GetEfficiencyString());
	}
	else
	{
		strExpression.AppendFormat(_T("y = %.2lf * x R = %.5lf  eff = %s"),m_linearFitData.slope,m_linearFitData.rSquare,GetEfficiencyString());
	}

	return strExpression;
}

BOOL CLineInfo::IsExpressionVisible()
{
	return m_bShow && m_bShowExpression && BeHaveFitData();
}



#include "StdAfx.h"
#include "StdProc.h"

CStdLineTag::CStdLineTag()
{
	m_iChanId = 1;
}

CStdLineTag::~CStdLineTag()
{

}

CStdLineTag::CStdLineTag(const CStdLineTag& stdLineTag)
{
	m_iChanId = stdLineTag.m_iChanId;
}

void CStdLineTag::operator=(const CStdLineTag& stdLineTag)
{
	m_iChanId = stdLineTag.m_iChanId;
}

BOOL CStdLineTag::operator==(const CStdLineTag& stdLineTag)
{
	return m_iChanId == stdLineTag.m_iChanId;
}

CStdLine::CStdLine()
{
	m_dEfficiency = 0; 
}

CStdLine::~CStdLine()
{

}

CStdLine::CStdLine(const CStdLine& stdLine)
{
	m_vecDensity.clear();
	m_2vecCt.clear();

	m_stdLineTag = stdLine.m_stdLineTag;
	int densityNum = stdLine.m_vecDensity.size();
	if(densityNum > 0)
	{
		for(int i = 0;i < densityNum;++i)
		{
			m_vecDensity.push_back(stdLine.m_vecDensity[i]);
			m_2vecCt.push_back(stdLine.m_2vecCt[i]);
		}
	}

	m_linearFitData = stdLine.m_linearFitData;
	m_dEfficiency = stdLine.m_dEfficiency;
}

void CStdLine::operator=(CStdLine& stdLine)
{
	m_vecDensity.clear();
	m_2vecCt.clear();

	m_stdLineTag = stdLine.m_stdLineTag;
	int densityNum = stdLine.m_vecDensity.size();
	if(densityNum > 0)
	{
		for(int i = 0;i < densityNum;++i)
		{
			m_vecDensity.push_back(stdLine.m_vecDensity[i]);
			m_2vecCt.push_back(stdLine.m_2vecCt[i]);
		}
	}

	m_linearFitData = stdLine.m_linearFitData;
	m_dEfficiency = stdLine.m_dEfficiency;
}

int CStdLine::LinearFit()
{
	if(0 != CalculateAverageDot())
	{
		return -1;
	}

	if(0 != m_linearFitData.LeastSquare())
	{
		return -2;
	}
	return 0;
}

int CStdLine::CalculateAverageDot()
{
	int iDensityNum = m_vecDensity.size();
	if(iDensityNum < 1)
	{
		return -1;
	}

	if(iDensityNum != m_2vecCt.size())
	{
		return -2;
	}

	if(0 != m_linearFitData.SetDataNum(iDensityNum))
	{
		return -3;
	}

	double dSum = 0;
	int iCtNum = 0;
	
	for(int i = 0;i < iDensityNum;++i)
	{
		dSum = 0;
		m_linearFitData.m_pDX[i] = m_vecDensity[i];
		iCtNum = m_2vecCt[i].size();
		for(int j = 0;j < iCtNum;++j)
		{
			dSum += m_2vecCt[i][j];
		}
		if(0 != iCtNum)
		{
			m_linearFitData.m_pDY[i] = dSum / iCtNum;
		}
		else
		{
			m_linearFitData.m_pDY[i] = 0;
		}
	}

	return 0;
}

void CStdLine::Clear()
{
	m_vecDensity.clear();
	m_2vecCt.clear();
}

CStdProc::CStdProc(void)
{
}


CStdProc::~CStdProc(void)
{
}

void CStdProc::AddPoint(CStdLineTag stdLineTag,double dX,double dY)
{
	int iStdLineNum = m_vecStdLine.size();
	int iStadLineIndex = 0;
	BOOL bFindStdLine = FALSE;
	if(iStdLineNum > 0)
	{
		for(int k = 0;k < iStdLineNum;++k)
		{
			if(stdLineTag == m_vecStdLine[k].m_stdLineTag)
			{
				iStadLineIndex = k;
				bFindStdLine = TRUE;
				break;
			}
		}
	}

	if(bFindStdLine)
	{
		int iDensityNum = m_vecStdLine[iStadLineIndex].m_vecDensity.size();
		BOOL bFind = FALSE;
		int iDotIndex = 0;
		for(int i = 0;i < iDensityNum;++i)
		{
			if(dX == m_vecStdLine[iStadLineIndex].m_vecDensity[i])
			{
				bFind = TRUE;
				iDotIndex = i;
				break;
			}
		}

		if(bFind)
		{
			m_vecStdLine[iStadLineIndex].m_2vecCt[iDotIndex].push_back(dY);
		}
		else
		{
			m_vecStdLine[iStadLineIndex].m_vecDensity.push_back(dX);
			vector<double> vecCt;
			vecCt.push_back(dY);
			m_vecStdLine[iStadLineIndex].m_2vecCt.push_back(vecCt);
		}
	}
	else
	{
		CStdLine stdLine;
		stdLine.m_stdLineTag = stdLineTag;
		stdLine.m_vecDensity.push_back(dX);
		vector<double> vecCt;
		vecCt.push_back(dY);
		stdLine.m_2vecCt.push_back(vecCt);
		m_vecStdLine.push_back(stdLine);
	}

	return;
}

void CStdProc::Clear()
{
	m_vecStdLine.clear();
}

int CStdProc::DeleteOneStdLine(CStdLineTag stdLineTag)
{
	for(vector<CStdLine>::iterator it = m_vecStdLine.begin();it != m_vecStdLine.end();++it)
	{
		if(stdLineTag == it->m_stdLineTag)
		{
			m_vecStdLine.erase(it);
			return 0;
		}
	}

	return 1;
}

CStdLine* CStdProc::GetStdLinePtr(CStdLineTag stdLineTag)
{
	for(int i = 0;i < m_vecStdLine.size();++i)
	{
		if(stdLineTag == m_vecStdLine[i].m_stdLineTag)
		{
			return &(m_vecStdLine[i]);
		}
	}
	return NULL;
}

CStdLine* CStdProc::GetStdLinePtr(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_vecStdLine.size())
	{
		return NULL;
	}
	return &(m_vecStdLine[iIndex]);
}

int CStdProc::GetStdLineNum()
{
	return m_vecStdLine.size();
}

void CStdProc::Fit()
{
	for(int i = 0;i < m_vecStdLine.size();++i)
	{
		m_vecStdLine[i].LinearFit();
	}
	return;
}






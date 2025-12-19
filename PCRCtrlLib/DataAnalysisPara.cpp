#include "StdAfx.h"
#include "DataAnalysisPara.h"
#include "PublicFun.h"

CDataAnalysisPara::CDataAnalysisPara(void)
{
	m_bUseRule = FALSE;

	m_arrayAmplifyPara.RemoveAll();
	m_arrayMeltCurvePara.RemoveAll();
	m_arrayInspectScope.RemoveAll();
	m_arrayBasicRule.RemoveAll();
}

CDataAnalysisPara::~CDataAnalysisPara(void)
{
	DeleteAll();
}

// 删除全部指标相关分析参数
void CDataAnalysisPara::DeleteAll()
{
	DeleteAmplifyPara();
	DeleteMeltCurvePara();
	DeleteInspectScope();
	DeleteBasicRule();
}

// 删除扩增段数据分析相关参数
void CDataAnalysisPara::DeleteAmplifyPara()
{
	int nCount = m_arrayAmplifyPara.GetCount();
	for (int i=0; i<nCount; i++)
	{
		tagAnaAmplifyPara* pAnalysis = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(i);
		SAFE_DELETE(pAnalysis);
	}
	m_arrayAmplifyPara.RemoveAll();
}

// 删除熔解段数据分析相关参数
void CDataAnalysisPara::DeleteMeltCurvePara()
{
	int nCount = m_arrayMeltCurvePara.GetCount();
	for (int i=0; i<nCount; i++)
	{
		tagAnaMeltCurvePara* pMeltPara = (tagAnaMeltCurvePara*)m_arrayMeltCurvePara.GetAt(i);
		SAFE_DELETE(pMeltPara);
	}
	m_arrayMeltCurvePara.RemoveAll();
}

// 删除全部项目定量检测范围参数指针
void CDataAnalysisPara::DeleteInspectScope()
{
	int nCount = m_arrayInspectScope.GetCount();
	for (int i=0; i<nCount; i++)
	{
		tagAnaInspectScope* pInspect = (tagAnaInspectScope*)m_arrayInspectScope.GetAt(i);
		SAFE_DELETE(pInspect);
	}
	m_arrayInspectScope.RemoveAll();
}

// 删除全部结果判定规则指针
void CDataAnalysisPara::DeleteBasicRule()
{
	int nCount = m_arrayBasicRule.GetCount();
	for (int i=0; i<nCount; i++)
	{
		tagAnaBasicRule* pRule = (tagAnaBasicRule*)m_arrayBasicRule.GetAt(i);
		SAFE_DELETE(pRule);
	}
	m_arrayBasicRule.RemoveAll();
}

// 写入数据到文件中
BOOL CDataAnalysisPara::WriteFile(CFile* pFile,WORD wVersion,BOOL bProject)
{
	WriteAmplifyPara(pFile,wVersion,bProject);
	WriteMeltCurvePara(pFile);
	WriteInspectScope(pFile);
	WriteResultRule(pFile);

	return TRUE;
}

BOOL CDataAnalysisPara::WriteAmplifyPara(CFile* pFile,WORD wVersion,BOOL bProject)
{
	int nCount = m_arrayAmplifyPara.GetCount();
	pFile->Write((const char*)&nCount, sizeof(int));
	for(int i=0; i<nCount; i++)
	{
		tagAnaAmplifyPara* pAnalysis = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(i);
		CPublicFun::WriteStringToFile(pFile, pAnalysis->strTarget);
		CPublicFun::WriteStringToFile(pFile, pAnalysis->strTubeFlag);
		pFile->Write((const char *)&pAnalysis->iChannelNo, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pAnalysis->strDye);
		int nTemp = pAnalysis->nAnaType;
		pFile->Write((const char *)&nTemp, sizeof(int));
		pFile->Write((const char *)&pAnalysis->bAutoBaseline, sizeof(BOOL));
		pFile->Write((const char *)&pAnalysis->iBaselineStart, sizeof(int));
		pFile->Write((const char *)&pAnalysis->iBaselineEnd, sizeof(int));

		nTemp = pAnalysis->nMethodCt;
		pFile->Write((const char *)&nTemp, sizeof(int));
		nTemp = pAnalysis->nDefaultDataType;
		pFile->Write((const char *)&nTemp, sizeof(int));
		nTemp = pAnalysis->nThresholdType;
		pFile->Write((const char *)&nTemp, sizeof(int));
		pFile->Write((const char *)&pAnalysis->dNormalizeThreshold, sizeof(double));
		pFile->Write((const char *)&pAnalysis->dDeltaRnThreshold, sizeof(double));
		pFile->Write((const char *)&pAnalysis->bUseFilter, sizeof(BOOL));

		if((bProject && wVersion >= 0x0101) || (!bProject && wVersion >= wPCRVersion103))
		{
			pFile->Write((const char *)&pAnalysis->dCalcNormalizeThreshold,sizeof(double));
			pFile->Write((const char *)&pAnalysis->dCalcDeltaRnThreshold,sizeof(double));
		}

		pFile->Write((const char *)&pAnalysis->dLowerThreshold, sizeof(double));
		pFile->Write((const char *)&pAnalysis->dUpperThreshold, sizeof(double));
	}

	return TRUE;
}

BOOL CDataAnalysisPara::WriteMeltCurvePara(CFile* pFile)
{
	int nCount = m_arrayMeltCurvePara.GetCount();
	pFile->Write((const char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagAnaMeltCurvePara* pMeltCurve = (tagAnaMeltCurvePara*)m_arrayMeltCurvePara.GetAt(i);
		CPublicFun::WriteStringToFile(pFile, pMeltCurve->strTarget);
		CPublicFun::WriteStringToFile(pFile, pMeltCurve->strTubeFlag);
		pFile->Write((const char *)&pMeltCurve->iChannelNo, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pMeltCurve->strDye);
		pFile->Write((const char *)&pMeltCurve->dMinTemp, sizeof(double));
		pFile->Write((const char *)&pMeltCurve->dMaxTemp, sizeof(double));
		pFile->Write((const char *)&pMeltCurve->dThreshold, sizeof(double));
	}

	return TRUE;
}

int CDataAnalysisPara::WriteInspectScope(CFile* pFile)
{
	int nCount = m_arrayInspectScope.GetCount();
	pFile->Write((const char*)&nCount, sizeof(int));
	for(int i=0; i<nCount; i++)
	{
		tagAnaInspectScope* pInspect = (tagAnaInspectScope*)m_arrayInspectScope.GetAt(i);
		CPublicFun::WriteStringToFile(pFile, pInspect->strTarget);
		pFile->Write((const char *)&pInspect->nChannelNo, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pInspect->strDye);
		pFile->Write((const char *)&pInspect->dLowerLimit, sizeof(double));
		pFile->Write((const char *)&pInspect->dTopLimit, sizeof(double));
	}

	return TRUE;
}

BOOL CDataAnalysisPara::WriteResultRule(CFile* pFile)
{
	pFile->Write((const char *)&m_bUseRule, sizeof(BOOL));

	int nCount = m_arrayBasicRule.GetCount();
	pFile->Write((const char*)&nCount, sizeof(int));
	for(int i=0; i<nCount; i++)
	{
		tagAnaBasicRule* pBasicRule = (tagAnaBasicRule*)m_arrayBasicRule.GetAt(i);
		int nTemp = pBasicRule->nRuleType;
		pFile->Write((const char *)&nTemp, sizeof(int));
		nTemp = pBasicRule->nSampleType;
		pFile->Write((const char *)&nTemp, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pBasicRule->strTarget);
		pFile->Write((const char *)&pBasicRule->nChannelNo, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pBasicRule->strDye);
		nTemp = pBasicRule->nDataType;
		pFile->Write((const char *)&nTemp, sizeof(int));
		pFile->Write((const char *)&pBasicRule->dValueFront, sizeof(double));
		nTemp = pBasicRule->nOperFront;
		pFile->Write((const char *)&nTemp, sizeof(int));
		pFile->Write((const char *)&pBasicRule->dValueBack, sizeof(double));
		nTemp = pBasicRule->nOperBack;
		pFile->Write((const char *)&nTemp, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pBasicRule->strConclusion);
		pFile->Write((const char *)&nTemp, sizeof(int));
		pFile->Write((const char *)&pBasicRule->clrResult, sizeof(COLORREF));

		pFile->Write((const char *)&pBasicRule->bUseHighRule, sizeof(BOOL));
		CPublicFun::WriteStringToFile(pFile, pBasicRule->strRuleFile);
		CPublicFun::WriteStringToFile(pFile, pBasicRule->strHighRule);
	}

	return TRUE;
}

// 从文件中读取数据
BOOL CDataAnalysisPara::ReadFile(CFile* pFile, WORD wVersion,BOOL bProject)
{
	DeleteAll();

	ReadAmplifyPara(pFile, wVersion,bProject);
	ReadMeltCurvePara(pFile, wVersion);
	ReadInspectScope(pFile, wVersion);
	ReadResultRule(pFile, wVersion);
	return TRUE;
}


BOOL CDataAnalysisPara::ReadAmplifyPara(CFile* pFile, WORD wVersion,BOOL bProject)
{
	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagAnaAmplifyPara* pAnalysis = new tagAnaAmplifyPara;
		m_arrayAmplifyPara.Add(pAnalysis);
		pAnalysis->strTarget = CPublicFun::ReadStringFromFile(pFile);
		pAnalysis->strTubeFlag = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pAnalysis->iChannelNo, sizeof(int));
		pAnalysis->strDye = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pAnalysis->nAnaType, sizeof(int));
		pFile->Read((char*)&pAnalysis->bAutoBaseline, sizeof(BOOL));
		pFile->Read((char*)&pAnalysis->iBaselineStart, sizeof(int));
		pFile->Read((char*)&pAnalysis->iBaselineEnd, sizeof(int));

		pFile->Read((char*)&pAnalysis->nMethodCt, sizeof(int));
		pFile->Read((char*)&pAnalysis->nDefaultDataType, sizeof(int));
		pFile->Read((char*)&pAnalysis->nThresholdType, sizeof(int));
		pFile->Read((char*)&pAnalysis->dNormalizeThreshold, sizeof(double));
		pFile->Read((char*)&pAnalysis->dDeltaRnThreshold, sizeof(double));
		pFile->Read((char*)&pAnalysis->bUseFilter, sizeof(BOOL));

		if((bProject && wVersion >= 0x0101) || (!bProject && wVersion >= wPCRVersion103))
		{
			pFile->Read((char*)&pAnalysis->dCalcNormalizeThreshold, sizeof(double));
			pFile->Read((char*)&pAnalysis->dCalcDeltaRnThreshold, sizeof(double));
		}

		if((bProject && wVersion >= wPCRItemVersion104) || (!bProject && wVersion >= wPCRVersion117))
		{
			pFile->Read((char*)&pAnalysis->dLowerThreshold, sizeof(double));
			pFile->Read((char*)&pAnalysis->dUpperThreshold, sizeof(double));
		}
	}

	return TRUE;
}

BOOL CDataAnalysisPara::ReadMeltCurvePara(CFile* pFile, WORD wVersion)
{
	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagAnaMeltCurvePara* pMeltCurve = new tagAnaMeltCurvePara;
		m_arrayMeltCurvePara.Add(pMeltCurve);
		pMeltCurve->strTarget = CPublicFun::ReadStringFromFile(pFile);
		pMeltCurve->strTubeFlag = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pMeltCurve->iChannelNo, sizeof(int));
		pMeltCurve->strDye = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pMeltCurve->dMinTemp, sizeof(double));
		pFile->Read((char*)&pMeltCurve->dMaxTemp, sizeof(double));
		pFile->Read((char*)&pMeltCurve->dThreshold, sizeof(double));
	}	

	return TRUE;
}

BOOL CDataAnalysisPara::ReadInspectScope(CFile* pFile, WORD wVersion)
{
	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagAnaInspectScope* pInspect = new tagAnaInspectScope;
		m_arrayInspectScope.Add(pInspect);
		pInspect->strTarget = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pInspect->nChannelNo, sizeof(int));
		pInspect->strDye = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pInspect->dLowerLimit, sizeof(double));
		pFile->Read((char*)&pInspect->dTopLimit, sizeof(double));
	}
	return TRUE;
}

BOOL CDataAnalysisPara::ReadResultRule(CFile* pFile, WORD wVersion)
{
	pFile->Read((char*)&m_bUseRule, sizeof(BOOL));

	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagAnaBasicRule* pBasicRule = new tagAnaBasicRule;
		m_arrayBasicRule.Add(pBasicRule);

		pFile->Read((char *)&pBasicRule->nRuleType, sizeof(int));
		pFile->Read((char *)&pBasicRule->nSampleType, sizeof(int));
		pBasicRule->strTarget = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char*)&pBasicRule->nChannelNo, sizeof(int));
		pBasicRule->strDye = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char *)&pBasicRule->nDataType, sizeof(int));
		pFile->Read((char*)&pBasicRule->dValueFront, sizeof(double));
		pFile->Read((char *)&pBasicRule->nOperFront, sizeof(int));
		pFile->Read((char*)&pBasicRule->dValueBack, sizeof(double));
		pFile->Read((char *)&pBasicRule->nOperBack, sizeof(int));
		pBasicRule->strConclusion = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char *)&pBasicRule->clrResult, sizeof(COLORREF));

		pFile->Read((char *)&pBasicRule->bUseHighRule, sizeof(BOOL));
		pBasicRule->strRuleFile = CPublicFun::ReadStringFromFile(pFile);
		pBasicRule->strHighRule = CPublicFun::ReadStringFromFile(pFile);
	}

	return TRUE;
}

// 从项目定义数据中复制相关分析参数
void CDataAnalysisPara::CopyFrom(CDataAnalysisPara* pAnalysis)
{
	if(NULL == pAnalysis)
	{
		return;
	}

	DeleteAll();

	// 复制扩增段分析参数
	int iCount = pAnalysis->GetAmplifyParaCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaAmplifyPara* pTemp = pAnalysis->GetAmplifyParaBy(i);
		tagAnaAmplifyPara* pAddPara = new tagAnaAmplifyPara;
		*pAddPara = *pTemp;
		m_arrayAmplifyPara.Add(pAddPara);
	}

	// 复制熔解段分析参数
	iCount = pAnalysis->GetMeltCurveParaCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaMeltCurvePara* pTemp = pAnalysis->GetMeltCurveParaBy(i);
		tagAnaMeltCurvePara* pAddPara = new tagAnaMeltCurvePara;
		*pAddPara = *pTemp;
		m_arrayMeltCurvePara.Add(pAddPara);
	}

	// 复制目标定量检测范围参数
	iCount = pAnalysis->GetInspectScopeCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaInspectScope* pTemp = pAnalysis->GetInspectScopeBy(i);
		tagAnaInspectScope* pAddPara = new tagAnaInspectScope;
		*pAddPara = *pTemp;
		m_arrayInspectScope.Add(pAddPara);
	}

	// 复制结果判定规则参数
	m_bUseRule = pAnalysis->GetUseRule();
	iCount = pAnalysis->GetBasicRuleCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaBasicRule* pTemp = pAnalysis->GetBasicRuleBy(i);
		tagAnaBasicRule* pAddPara = new tagAnaBasicRule;
		*pAddPara = *pTemp;
		m_arrayBasicRule.Add(pAddPara);
	}
}

// 根据指标名称得到扩增段数据分析参数
tagAnaAmplifyPara* CDataAnalysisPara::GetAmplifyParaBy(CString strTarget, CString strDye, int iChannel)
{
	tagAnaAmplifyPara* pReturn = NULL;
	int iCount = m_arrayAmplifyPara.GetCount();
	for (int i=0; i<iCount; i++) // 按照基因名找
	{
		tagAnaAmplifyPara* pTemp = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(i);
		if (pTemp->iChannelNo == iChannel)
		{
			if (!strTarget.IsEmpty() && pTemp->strTarget == strTarget)
			{
				pReturn = pTemp;
				break;
			}
		}
	}

	if (pReturn == NULL)
	{
		for (int i=0; i<iCount; i++) // 按照基因名为空找
		{
			tagAnaAmplifyPara* pTemp = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(i);
			if (pTemp->iChannelNo == iChannel)
			{
				if (pTemp->strTarget.IsEmpty() && pTemp->strDye == strDye)
				{
					pReturn = pTemp;
					break;
				}
			}
		}
	}

	if (pReturn == NULL)
	{
		for (int i=0; i<iCount; i++) // 按照通道找
		{
			tagAnaAmplifyPara* pTemp = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(i);
			if (pTemp->iChannelNo == iChannel)
			{
				if (pTemp->strDye == strDye)
				{
					pReturn = pTemp;
					break;
				}
			}
		}
	}

	return pReturn;
}

// 根据指标名称得到熔解段数据分析参数
tagAnaMeltCurvePara* CDataAnalysisPara::GetMeltCurveParaBy(CString strTarget)
{
	tagAnaMeltCurvePara* pReturn = FALSE;
	int iCount = m_arrayMeltCurvePara.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaMeltCurvePara* pTemp = (tagAnaMeltCurvePara*)m_arrayMeltCurvePara.GetAt(i);
		if (pTemp->strTarget == strTarget)
		{
			pReturn = pTemp;
			break;
		}
	}

	return pReturn;
}

// 根据指标名称得到指标定量检测范围参数
tagAnaInspectScope* CDataAnalysisPara::GetInspectScopeBy(CString strTarget)
{
	tagAnaInspectScope* pReturn = FALSE;
	int iCount = m_arrayInspectScope.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaInspectScope* pTemp = (tagAnaInspectScope*)m_arrayInspectScope.GetAt(i);
		if (pTemp->strTarget == strTarget)
		{
			pReturn = pTemp;
			break;
		}
	}

	return pReturn;
}

// 根据指标名称得到判定规则参数
tagAnaBasicRule* CDataAnalysisPara::GetBasicRuleBy(CString strTarget)
{
	tagAnaBasicRule* pReturn = FALSE;
	int iCount = m_arrayBasicRule.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaBasicRule* pTemp = (tagAnaBasicRule*)m_arrayBasicRule.GetAt(i);
		if (pTemp->strTarget == strTarget)
		{
			pReturn = pTemp;
			break;
		}
	}

	return pReturn;
}

tagAnaAmplifyPara* CDataAnalysisPara::GetAmplifyParaBy(int nIndex)
{
	tagAnaAmplifyPara* pReturn = NULL;

	if (nIndex >= 0 && nIndex < m_arrayAmplifyPara.GetCount())
	{
		pReturn = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(nIndex);
	}

	return pReturn;
}

int CDataAnalysisPara::SetAmplifyParaCount(int iCount)
{
	if(iCount == GetAmplifyParaCount())
	{
		return 1;
	}

	DeleteAmplifyPara();

	tagAnaAmplifyPara* pAnaAmpCrvParam = NULL;
	for(int i = 0;i < iCount;++i)
	{
		pAnaAmpCrvParam = new tagAnaAmplifyPara;
		if(NULL == pAnaAmpCrvParam)
		{
			return -1;
		}
		m_arrayAmplifyPara.Add(pAnaAmpCrvParam);
	}
	return 0;
}


// 修改指标分析参数，不需要申请指针
void CDataAnalysisPara::SetAmplifyParaAt(int nIndex, tagAnaAmplifyPara* pAnalysis)
{
	tagAnaAmplifyPara* pSetAnalysis = GetAmplifyParaBy(nIndex);
	if (pSetAnalysis != NULL)
	{
		pSetAnalysis->strTarget = pAnalysis->strTarget;
		pSetAnalysis->strTubeFlag = pAnalysis->strTubeFlag;
		pSetAnalysis->iChannelNo = pAnalysis->iChannelNo;
		pSetAnalysis->nAnaType = pAnalysis->nAnaType;
		pSetAnalysis->bAutoBaseline = pAnalysis->bAutoBaseline;
		pSetAnalysis->iBaselineStart = pAnalysis->iBaselineStart;
		pSetAnalysis->iBaselineEnd = pAnalysis->iBaselineEnd;
		pSetAnalysis->nMethodCt = pAnalysis->nMethodCt;
		pSetAnalysis->nDefaultDataType = pAnalysis->nDefaultDataType;
		pSetAnalysis->nThresholdType = pAnalysis->nThresholdType;
		pSetAnalysis->dNormalizeThreshold = pAnalysis->dNormalizeThreshold;
		pSetAnalysis->dDeltaRnThreshold = pAnalysis->dDeltaRnThreshold;
		pSetAnalysis->bUseFilter = pAnalysis->bUseFilter;
	}
}

tagAnaAmplifyPara* CDataAnalysisPara::GetAmplifyParaByTarget(CString strTarget, CString strDye)
{
	tagAnaAmplifyPara* pReturnPara = NULL;
	int iCount = m_arrayAmplifyPara.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaAmplifyPara* pTempAnalysis = GetAmplifyParaBy(i);
		if (pTempAnalysis->strTarget == strTarget && pTempAnalysis->strDye == strDye)
		{
			pReturnPara = pTempAnalysis;
			break;
		}
	}

	return pReturnPara;
}

tagAnaAmplifyPara* CDataAnalysisPara::GetAmplifyParaByDye(CString strDye)
{
	tagAnaAmplifyPara* pReturnPara = NULL;
	int iCount = m_arrayAmplifyPara.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagAnaAmplifyPara* pTempAnalysis = GetAmplifyParaBy(i);
		if (pTempAnalysis->strDye == strDye && pTempAnalysis->strTarget.IsEmpty())
		{
			pReturnPara = pTempAnalysis;
			break;
		}
	}

	return pReturnPara;
}

void CDataAnalysisPara::DeleteOneAmplifyPara(int iIndex)
{
	if (iIndex >=0 && iIndex < m_arrayAmplifyPara.GetCount())
	{
		tagAnaAmplifyPara* pAmplify = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(iIndex);
		SAFE_DELETE(pAmplify);
		m_arrayAmplifyPara.RemoveAt(iIndex);
	}
}


int CDataAnalysisPara::SetMeltCurveParaCount(int iCount)
{
	if(iCount == GetMeltCurveParaCount())
	{
		return 1;
	}

	DeleteMeltCurvePara();

	tagAnaMeltCurvePara* pAnaMeltCrvParam = NULL;
	for(int i = 0;i < iCount;++i)
	{
		pAnaMeltCrvParam = new tagAnaMeltCurvePara;
		if(NULL == pAnaMeltCrvParam)
		{
			return -1;
		}
		m_arrayMeltCurvePara.Add(pAnaMeltCrvParam);
	}
	return 0;
}

tagAnaMeltCurvePara* CDataAnalysisPara::GetMeltCurveParaBy(int nIndex)
{
	if (nIndex < 0 && nIndex >= m_arrayMeltCurvePara.GetCount())
	{
		ASSERT(FALSE);
		return NULL;
	}

	return (tagAnaMeltCurvePara*)m_arrayMeltCurvePara.GetAt(nIndex);
}

// 修改参数，不需要申请指针
void CDataAnalysisPara::SetMeltCurveParaAt(int nIndex, tagAnaMeltCurvePara* pMeltCurve)
{
	tagAnaMeltCurvePara* pSetMeltCurve = GetMeltCurveParaBy(nIndex);
	if (pSetMeltCurve != NULL)
	{
		pSetMeltCurve->strTarget = pMeltCurve->strTarget;
		pSetMeltCurve->strTubeFlag = pMeltCurve->strTubeFlag;
		pSetMeltCurve->iChannelNo = pMeltCurve->iChannelNo;
		pSetMeltCurve->dMinTemp = pMeltCurve->dMinTemp;
		pSetMeltCurve->dMaxTemp = pMeltCurve->dMaxTemp;
		pSetMeltCurve->dThreshold = pMeltCurve->dThreshold;
	}
}

tagAnaInspectScope* CDataAnalysisPara::GetInspectScopeBy(int nIndex)
{
	if (nIndex < 0 && nIndex >= m_arrayInspectScope.GetCount())
	{
		ASSERT(FALSE);
		return NULL;
	}

	return (tagAnaInspectScope*)m_arrayInspectScope.GetAt(nIndex);
}

// 修改项目定量检测范围参数，不需要申请指针
void CDataAnalysisPara::SetInspectScopeAt(int nIndex, tagAnaInspectScope* pInspectScope)
{
	tagAnaInspectScope* pSetInspectScope = GetInspectScopeBy(nIndex);
	if (pSetInspectScope != NULL)
	{
		pSetInspectScope->strTarget = pInspectScope->strTarget;
		pSetInspectScope->dLowerLimit = pInspectScope->dLowerLimit;
		pSetInspectScope->dTopLimit = pInspectScope->dTopLimit;
	}
}

tagAnaBasicRule* CDataAnalysisPara::GetBasicRuleBy(int nIndex)
{
	if (nIndex < 0 && nIndex >= m_arrayBasicRule.GetCount())
	{
		ASSERT(FALSE);
		return NULL;
	}

	return (tagAnaBasicRule*)m_arrayBasicRule.GetAt(nIndex);
}

// 修改结果判定规则参数，不需要申请指针
void CDataAnalysisPara::SetBasicRule(int nIndex, tagAnaBasicRule* pBasicRule)
{
	tagAnaBasicRule* pSetBasicRule = GetBasicRuleBy(nIndex);
	if (pSetBasicRule != NULL)
	{
		pSetBasicRule->nRuleType = pBasicRule->nRuleType;
		pSetBasicRule->nSampleType = pBasicRule->nSampleType;
		pSetBasicRule->strTarget = pBasicRule->strTarget;
		pSetBasicRule->nDataType = pBasicRule->nDataType;
		pSetBasicRule->dValueFront = pBasicRule->dValueFront;
		pSetBasicRule->nOperFront = pBasicRule->nOperFront;
		pSetBasicRule->dValueBack = pBasicRule->dValueBack;
		pSetBasicRule->nOperBack = pBasicRule->nOperBack;
		pSetBasicRule->strConclusion = pBasicRule->strConclusion;
		pSetBasicRule->clrResult = pBasicRule->clrResult;
		pSetBasicRule->bUseHighRule = pBasicRule->bUseHighRule;
		pSetBasicRule->strRuleFile = pBasicRule->strRuleFile;
		pSetBasicRule->strHighRule = pBasicRule->strHighRule;
	}
}


BOOL CDataAnalysisPara::IsNewAmplifyPara(CString strStartupPath, tagAnaAmplifyPara* pInputPara)
{
	BOOL bFind = FALSE;
	for (int i=0; i<m_arrayAmplifyPara.GetCount(); i++)
	{
		tagAnaAmplifyPara* pTemp = (tagAnaAmplifyPara*)m_arrayAmplifyPara.GetAt(i);
		if (pTemp->strDye == pInputPara->strDye && pTemp->strTarget == pInputPara->strTarget)
		{
			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
	{
		ReadAmplifyParaFromIniFile(pInputPara, strStartupPath);
		m_arrayAmplifyPara.Add(pInputPara);
	}


	return !bFind;
}

void CDataAnalysisPara::Clear()
{
	DeleteAll();
}

// 从文件中读取扩增分析参数
void CDataAnalysisPara::ReadAmplifyParaFromIniFile(tagAnaAmplifyPara* pInputPara, CString strStartupPath)
{

}

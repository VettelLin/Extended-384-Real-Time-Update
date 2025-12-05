#include "StdAfx.h"
#include "ReagentItem.h"
#include "PublicFun.h"

//*****************************************************************************************************************
// 项目通道信息类
CItemChannelInfo::CItemChannelInfo(void)
{
	m_bUse = FALSE;
	m_nChannel = 0;
	m_strDye.Empty();
	m_nExWavelength = 0;
	m_nEmWavelength = 0;
	m_arrayTarget.RemoveAll();
	m_dNegPosUpLimit = 40;
	m_dNegPosDownLimit = 36;
}

CItemChannelInfo::~CItemChannelInfo(void)
{
	DeleteAll();
}

void CItemChannelInfo::DeleteAll()
{
	int iTagetCount = m_arrayTarget.GetCount();
	for (int i=0; i<iTagetCount; i++)
	{
		tagItemTargetInfo* pTargetInfo = (tagItemTargetInfo*)m_arrayTarget.GetAt(i);
		SAFE_DELETE(pTargetInfo);
	}

	m_arrayTarget.RemoveAll();
}

BOOL CItemChannelInfo::WriteFile(CFile* pFile)
{
	pFile->Write((const char *)&m_bUse, sizeof(BOOL));
	pFile->Write((const char *)&m_nChannel, sizeof(int));
	CPublicFun::WriteStringToFile(pFile, m_strDye);
	pFile->Write((const char *)&m_nExWavelength, sizeof(int));
	pFile->Write((const char *)&m_nEmWavelength, sizeof(int));

	int nCount = m_arrayTarget.GetCount();
	pFile->Write((const char *)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(i);
		CPublicFun::WriteStringToFile(pFile, pTarget->strTubeFlag);
		CPublicFun::WriteStringToFile(pFile, pTarget->strTarget);
		pFile->Write((const char *)&(pTarget->clrTarget), sizeof(COLORREF));
	}

	pFile->Write((const char *)&m_dNegPosUpLimit,sizeof(double));
	pFile->Write((const char *)&m_dNegPosDownLimit,sizeof(double));

	return TRUE;
}

BOOL CItemChannelInfo::ReadFile(CFile* pFile, WORD wVersion,BOOL bProject)
{
	pFile->Read((char *)&m_bUse, sizeof(BOOL));
	pFile->Read((char *)&m_nChannel, sizeof(int));
	m_strDye = CPublicFun::ReadStringFromFile(pFile);
	pFile->Read((char *)&m_nExWavelength, sizeof(int));
	pFile->Read((char *)&m_nEmWavelength, sizeof(int));

	int nCount = 0;
	pFile->Read((char *)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagItemTargetInfo* pTarget = new tagItemTargetInfo;
		pTarget->strTubeFlag = CPublicFun::ReadStringFromFile(pFile);
		pTarget->strTarget = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char *)&(pTarget->clrTarget), sizeof(COLORREF));
		m_arrayTarget.Add(pTarget);
	}

	if((bProject && wVersion >= 0X0102) || (!bProject && wVersion >= wPCRVersion105))
	{
		pFile->Read((char *)&m_dNegPosUpLimit,sizeof(double));
		pFile->Read((char *)&m_dNegPosDownLimit,sizeof(double));
	}

	return TRUE;
}

void CItemChannelInfo::CopyFrom(CItemChannelInfo* pInputInfo)
{
	DeleteAll();

	m_bUse = pInputInfo->GetUse();
	m_nChannel = pInputInfo->GetChannel();
	m_strDye = pInputInfo->GetDyeName();
	m_nExWavelength = pInputInfo->GetExWavelength();
	m_nEmWavelength = pInputInfo->GetEmWavelength();
	
	int nCount = pInputInfo->GetTargetCount();
	for(int i=0; i<nCount; i++)
	{
		tagItemTargetInfo* pTarget = new tagItemTargetInfo;
		pTarget->strTubeFlag = pInputInfo->GetTubeFlag(i);
		pTarget->strTarget = pInputInfo->GetTargetName(i);
		pTarget->clrTarget = pInputInfo->GetTargetColor(i);
		m_arrayTarget.Add(pTarget);
	}

	m_dNegPosUpLimit = pInputInfo->m_dNegPosUpLimit;
	m_dNegPosDownLimit = pInputInfo->m_dNegPosDownLimit;
}

void CItemChannelInfo::AddTarget(CString strTubeFlag, CString strName, COLORREF color)
{
	tagItemTargetInfo* pTarget = new tagItemTargetInfo;
	pTarget->strTubeFlag = strTubeFlag;
	pTarget->strTarget = strName;
	pTarget->clrTarget = color;
	m_arrayTarget.Add(pTarget);
}

void CItemChannelInfo::AddTarget(CString strTargetName)
{
	tagItemTargetInfo* pTarget = new tagItemTargetInfo;
	pTarget->strTarget = strTargetName;
	m_arrayTarget.Add(pTarget);
}

CString CItemChannelInfo::GetTubeFlag(int iIndex)
{
	tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(iIndex);
	return pTarget->strTubeFlag; 
}

void CItemChannelInfo::SetTubeFlag(int iIndex, CString strFlag)
{
	tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(iIndex);
	pTarget->strTubeFlag = strFlag; 
}

CString CItemChannelInfo::GetTargetName(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_arrayTarget.GetSize())
	{
		return _T("");
	}
	tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(iIndex);
	return pTarget->strTarget; 
}

CString CItemChannelInfo::GetTargetName(CString strTubeFlag)
{
	CString strReturn;
	strReturn.Empty();
	for (int i=0; i<m_arrayTarget.GetCount(); i++)
	{
		tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(i);
		if (pTarget->strTubeFlag == strTubeFlag)
		{
			strReturn = pTarget->strTarget;
			break;
		}
	}

	return strReturn;
}

void CItemChannelInfo::SetTargetName(int iIndex, CString string)
{
	tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(iIndex);
	pTarget->strTarget = string; 
}

COLORREF CItemChannelInfo::GetTargetColor(int iIndex)
{
	tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(iIndex);
	return pTarget->clrTarget; 

}

COLORREF CItemChannelInfo::GetTargetColor(CString strTubeFlag)
{
	COLORREF clrTarget = RGB(255, 0, 0);
	for (int i=0; i<m_arrayTarget.GetCount(); i++)
	{
		tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(i);
		if (pTarget->strTubeFlag == strTubeFlag)
		{
			clrTarget = pTarget->clrTarget;
			break;
		}
	}

	return clrTarget;
}

void CItemChannelInfo::SetTargetColor(int iIndex, COLORREF color)
{
	tagItemTargetInfo* pTarget = (tagItemTargetInfo*)m_arrayTarget.GetAt(iIndex);
	pTarget->clrTarget = color; 
}


//*****************************************************************************************************************
// 项目信息类

CReagentItem::CReagentItem(void)
{
	m_strName.Empty();
	m_nType = EXP_TYPE_ABSOLUTE;
	m_strBarcode.Empty();
	m_strReagent.Empty();
	m_iReactVolume = 20;
	m_strUnit = _T("copies/ml");
	m_strComments.Empty();
	m_strUser.Empty();
	m_strDate.Empty();

	m_iTubeNumPerSample = 1;
	m_nSortMode = SORT_MODE_VERT;
	m_arrayTarget.RemoveAll();

	m_nSelChanCount = 0;
	m_pCrosstalkPara = NULL;

	m_pAnalysisPara = new CDataAnalysisPara;

	m_nStatus = STATUS_NEW;
}

CReagentItem::~CReagentItem(void)
{
	DeleteAll();
}

void CReagentItem::DeleteAll()
{
	DeleteAllChannelTarget();
	SAFE_DELETE(m_pCrosstalkPara);
	m_nSelChanCount = 0;

	DeleteAllSegement();
	
	SAFE_DELETE(m_pAnalysisPara);

	m_lnkDyeName.Clear();
}

BOOL CReagentItem::SaveFile(CString strFileName)
{
	CFile file;
	CFileException e;
	if (!file.Open(strFileName, CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		if (e.m_cause == CFileException::fileNotFound)
		{
			if (!file.Open(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone, &e))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	// 写入文件标识和版本
	WORD wFileMark = wFileMarkPCRItem;
	file.Write((const char*)&wFileMark,sizeof(WORD));
	WORD wVersion = wPCRItemVersion104;
	file.Write((const char*)&wVersion,sizeof(WORD));

	// 保存项目信息
	WriteFile(&file, wVersion, TRUE);
	file.Close();

	m_nStatus = STATUS_SAVE;

	return TRUE;
}

BOOL CReagentItem::OpenFile(CString strFileName)
{
	CFile file;       
	if (!file.Open(strFileName,CFile::modeRead | CFile::shareDenyNone))
	{
		return FALSE;
	}

	WORD wFileMark = 0;
	file.Read((char*)&wFileMark, sizeof(WORD));
	if(wFileMark !=  wFileMarkPCRItem)
	{
		file.Close();
		return FALSE;
	}

	WORD wVersion = 0;
	file.Read((char*)&wVersion, sizeof(WORD));
	if (wVersion > wPCRItemVersion104)
	{
		file.Close();
		return FALSE;
	}

	ReadFile(&file, wVersion,TRUE);
	file.Close();

	m_nStatus = STATUS_SAVE;

	return TRUE;
}

BOOL CReagentItem::WriteFile(CFile* pFile,WORD wVersion,BOOL bProject)
{
	WriteBasicInfo(pFile);
	WriteChannelTarget(pFile,wVersion,bProject);
	WriteSegment(pFile);

	m_pAnalysisPara->WriteFile(pFile,wVersion,bProject);

	return TRUE;
}

// 保存项目基本参数
BOOL CReagentItem::WriteBasicInfo(CFile* pFile)
{
	CPublicFun::WriteStringToFile(pFile, m_strName);

	int tempInt = m_nType;
	pFile->Write((const char *)&tempInt, sizeof(int));
	CPublicFun::WriteStringToFile(pFile, m_strBarcode);
	CPublicFun::WriteStringToFile(pFile, m_strReagent);

	pFile->Write((const char *)&m_iReactVolume, sizeof(int));
	CPublicFun::WriteStringToFile(pFile, m_strUnit);
	CPublicFun::WriteStringToFile(pFile, m_strComments);
	CPublicFun::WriteStringToFile(pFile, m_strUser);
	m_strDate = CPublicFun::GetCurrentDateTime(FALSE);
	CPublicFun::WriteStringToFile(pFile, m_strDate);

	return TRUE;
}

// 保存项目检测指标相关参数
BOOL CReagentItem::WriteChannelTarget(CFile* pFile,WORD wVersion,BOOL bProject)
{
	pFile->Write((const char *)&m_iTubeNumPerSample, sizeof(int));
	int tempInt = m_nSortMode;
	pFile->Write((const char*)&tempInt, sizeof(int));

	int nCount = m_arrayTarget.GetCount();
	pFile->Write((const char*)&nCount, sizeof(int));
	for(int i=0; i<nCount; i++)
	{
		CItemChannelInfo* pTarget = (CItemChannelInfo*)m_arrayTarget.GetAt(i);
		pTarget->WriteFile(pFile);
	}

	// 保存交叉干扰系数
	 pFile->Write((const char *)&m_nSelChanCount,sizeof(int));
	 if(m_nSelChanCount > 0)
	 {
		pFile->Write((const char *)m_pCrosstalkPara,sizeof(double) * m_nSelChanCount * m_nSelChanCount);
	 }

	 if((bProject && wVersion >= 0X103) || (!bProject && wVersion >= wPCRVersion108)) 
	 {
		 int iLnkSize = m_lnkDyeName.GetSize();
		 pFile->Write((const char*)&iLnkSize, sizeof(int));
		 Node<CString>* pNode = m_lnkDyeName.GetHead();
		 while(NULL != pNode)
		 {
			 CPublicFun::WriteStringToFile(pFile,pNode->data);
			 pNode = pNode->pNext;
		 }
	 }

	return TRUE;
}

// 保存项目实验程序
BOOL CReagentItem::WriteSegment(CFile* pFile)
{
	int nCount = m_lnkSegment.GetSize();
	pFile->Write((const char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		m_lnkSegment.GetAt(i)->data.WriteFile(pFile);
	}

	return TRUE;
}

BOOL CReagentItem::ReadFile(CFile* pFile, WORD wVersion,BOOL bProject)
{
	DeleteAll();

	ReadBasicInfo(pFile, wVersion);
	ReadChannelTarget(pFile, wVersion,bProject);
	ReadSegment(pFile, wVersion);

	if(NULL == m_pAnalysisPara)
	{
		m_pAnalysisPara = new CDataAnalysisPara;
	}
	m_pAnalysisPara->ReadFile(pFile,wVersion,bProject);

	return TRUE;
}

BOOL CReagentItem::ReadBasicInfo(CFile* pFile, WORD wVersion)
{
	m_strName = CPublicFun::ReadStringFromFile(pFile);
	pFile->Read((char*)&m_nType, sizeof(int));
	m_strBarcode = CPublicFun::ReadStringFromFile(pFile);
	m_strReagent = CPublicFun::ReadStringFromFile(pFile);

	pFile->Read((char*)&m_iReactVolume, sizeof(int));
	m_strUnit = CPublicFun::ReadStringFromFile(pFile);
	m_strComments = CPublicFun::ReadStringFromFile(pFile);
	m_strUser = CPublicFun::ReadStringFromFile(pFile);
	m_strDate = CPublicFun::ReadStringFromFile(pFile);

	return TRUE;
}

BOOL CReagentItem::ReadChannelTarget(CFile* pFile, WORD wVersion,BOOL bProject)
{
	pFile->Read((char*)&m_iTubeNumPerSample, sizeof(int));
	int tempInt = 0;
	pFile->Read((char*)&tempInt, sizeof(int));
	m_nSortMode = (eTubeSortMode)tempInt;

	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for(int i=0; i<nCount; i++)
	{
		CItemChannelInfo* pChannel = new CItemChannelInfo;
		pChannel->ReadFile(pFile, wVersion,bProject);
		m_arrayTarget.Add(pChannel);
	}

	 pFile->Read((char*)&m_nSelChanCount,sizeof(int));
	 if(m_nSelChanCount > 0)
	 {
		 if(NULL != m_pCrosstalkPara)
		 {
			 delete [] m_pCrosstalkPara;
		 }

		int iDataCount = m_nSelChanCount * m_nSelChanCount;
		m_pCrosstalkPara = new double[iDataCount];
		pFile->Read((char*)m_pCrosstalkPara,sizeof(double) * iDataCount);
	 }

	 if((bProject && wVersion >= 0X103) || (!bProject && wVersion >= wPCRVersion108))
	 {
		 m_lnkDyeName.Clear();
		 int iDataCount = 0;
		 CString strTemp(_T(""));
		 pFile->Read((char*)&iDataCount, sizeof(int));
		for(int i = 0;i < iDataCount;++i)
		{
			strTemp = CPublicFun::ReadStringFromFile(pFile);
			m_lnkDyeName.AddTail(strTemp);
		}
	 }

	return TRUE;
}

BOOL CReagentItem::ReadSegment(CFile* pFile, WORD wVersion)
{
	m_lnkSegment.Clear();

	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		CSegItemInfo segItem;
		segItem.ReadFile(pFile, wVersion);
		m_lnkSegment.AddTail(segItem);
	}

	return TRUE;
}

// 取得指定序号的通道指标参数指针
CItemChannelInfo* CReagentItem::GetChannelTargetBy(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrayTarget.GetCount())
	{
		return NULL;
	}

	return (CItemChannelInfo*)m_arrayTarget.GetAt(nIndex);
}

// 修改指定序号的通道指标参数，不需要申请指针
void CReagentItem::SetChannelTargetAt(int nIndex, CItemChannelInfo* pChannel)
{
	CItemChannelInfo* pSetChannel = GetChannelTargetBy(nIndex);
	if (pSetChannel != NULL)
	{
		pSetChannel->CopyFrom(pChannel);
	}
}

CItemChannelInfo* CReagentItem::GetChannelTargetBy(CString strName)
{
	CItemChannelInfo* pInfo = NULL;
	int nCount = m_arrayTarget.GetCount();
	for(int i=0; i<nCount; i++)
	{
		CItemChannelInfo* pChannel = (CItemChannelInfo*)m_arrayTarget.GetAt(i);
		if (pChannel->GetDyeName() == strName)
		{
			pInfo = pChannel;
			break;
		}
	}

	return pInfo;
}


void CReagentItem::DeleteAllChannelTarget()
{
	int nCount = m_arrayTarget.GetCount();
	for(int i=0; i<nCount; i++)
	{
		CItemChannelInfo* pChannel = (CItemChannelInfo*)m_arrayTarget.GetAt(i);
		SAFE_DELETE(pChannel);
	}
	m_arrayTarget.RemoveAll();
}

// 删除全部的通道指标参数指针
void CReagentItem::SetSelChannelCount(int nCount)
{
	if (nCount != m_nSelChanCount)
	{
		SAFE_DELETE(m_pCrosstalkPara);
		m_nSelChanCount = nCount;
		m_pCrosstalkPara = new double[m_nSelChanCount * m_nSelChanCount];
		memset(m_pCrosstalkPara, 0, m_nSelChanCount * m_nSelChanCount * sizeof(double));
	}
}

void CReagentItem::GetUsedChannel(CIntArray* pArrayChannel)
{
	int iCount = m_arrayTarget.GetCount();
	for (int i=0; i<iCount; i++)
	{
		CItemChannelInfo* pChannel = (CItemChannelInfo*)m_arrayTarget.GetAt(i);
		if (pChannel->GetUse())
		{
			pArrayChannel->Add(pChannel->GetChannel());
		}
	}
}

CSegItemInfo CReagentItem::GetSegmentBy(int nIndex)
{
	CSegItemInfo segItem;

	if (nIndex < 0 && nIndex >= m_lnkSegment.GetSize())
	{
		ASSERT(FALSE);
		return segItem;
	}

	return *(m_lnkSegment[nIndex]);
}

void CReagentItem::SetSegment(CLinkList<CSegItemInfo>& lnkSegItem)
{
	m_lnkSegment = lnkSegItem;
}

// 修改参数，不需要申请指针
void CReagentItem::SetSegmentAt(int nIndex, CSegItemInfo segItem)
{
	if (nIndex < 0 && nIndex >= m_lnkSegment.GetSize())
	{
		return;
	}

	m_lnkSegment.GetAt(nIndex)->data = segItem;

	return;
}

void CReagentItem::DeleteSegmentAt(int nIndex)
{
	if (nIndex < 0 && nIndex >= m_lnkSegment.GetSize())
	{
		return;
	}

	m_lnkSegment.DeleteAt(nIndex);
	return;
}

void CReagentItem::DeleteAllSegement()
{
	m_lnkSegment.Clear();
}

// 保存输入项目的信息，如果bUseSegmentEx为真，则使用扩展类
void CReagentItem::CopyFrom(CReagentItem* pInputItem)
{
	DeleteAll();

	m_strName = pInputItem->GetName();
	m_nType = pInputItem->GetType();
	m_strBarcode = pInputItem->GetBarcode();
	m_strReagent = pInputItem->GetReagent();
	m_iReactVolume = pInputItem->GetReactVolume();
	m_strUnit = pInputItem->GetUnit();
	m_strComments = pInputItem->GetComments();
	m_strUser = pInputItem->GetUser();
	m_strDate = pInputItem->GetDate();

	// 检测目标
	m_iTubeNumPerSample = pInputItem->GetTubeNumPerSample();
	m_nSortMode = pInputItem->GetTubeSortMode();

	int iCount = pInputItem->GetChannelTargetCount();
	for (int i=0; i<iCount; i++)
	{
		CItemChannelInfo* pChannel = new CItemChannelInfo;
		pChannel->CopyFrom(pInputItem->GetChannelTargetBy(i));
		m_arrayTarget.Add(pChannel);
	}

	// 交叉干扰系数
	m_nSelChanCount = pInputItem->GetSelChannelCount();
	m_pCrosstalkPara = new double[m_nSelChanCount * m_nSelChanCount];
	memcpy(m_pCrosstalkPara, pInputItem->GetCrosstalkPara(), sizeof(double) * m_nSelChanCount * m_nSelChanCount);

	// 实验程序
	m_lnkSegment = pInputItem->m_lnkSegment;
	m_lnkDyeName = pInputItem->m_lnkDyeName;

	// 目标分析参数
	m_pAnalysisPara = new CDataAnalysisPara;
	m_pAnalysisPara->CopyFrom(pInputItem->GetAnalysisPara());

	return;
}

void CReagentItem::CopyChanTargetInfo(CReagentItem* pInputItem)
{
	DeleteAllChannelTarget();

	int iCount = pInputItem->GetChannelTargetCount();
	for (int i = 0;i < iCount;++i)
	{
		CItemChannelInfo* pChannel = new CItemChannelInfo;
		pChannel->CopyFrom(pInputItem->GetChannelTargetBy(i));
		m_arrayTarget.Add(pChannel);
	}

	return;
}

// 根据用户定义染料和仪器通道初始化可用扫描通道
void CReagentItem::InitChannelInfo(CDyeInfoList* pListDye, CPtrArray* pArrayChannel)
{
	DeleteAllChannelTarget();

	int iChannelCount = pArrayChannel->GetCount();
	for (int i=0; i<iChannelCount; i++)
	{
		tagScanChannelInfo* pChannel = (tagScanChannelInfo*)pArrayChannel->GetAt(i);
		if (pChannel->iType == 4) // 不考虑校准光路
			continue;

		CItemChannelInfo* pItemChannel = new CItemChannelInfo;
		pItemChannel->SetChannel(pChannel->iExPosition);
		pItemChannel->SetExWavelength(pChannel->iExWaveLen);
		pItemChannel->SetEmWavelength(pChannel->iEmWaveLen);
		m_arrayTarget.Add(pItemChannel);

		POSITION pos = pListDye->GetHeadPosition();
		while(pos != NULL)
		{
			CDBDyeInfo* pDyeDB = pListDye->GetNext(pos);

			BOOL bSameEx = ::abs(pDyeDB->GetExWavelength() - pChannel->iExWaveLen) < pChannel->iExRange / 2;
			BOOL bSameEm = ::abs(pDyeDB->GetEmWavelength() - pChannel->iEmWaveLen) < pChannel->iEmRange / 2;
			if(bSameEx || bSameEm)
			{
				pItemChannel->SetDyeName(pDyeDB->GetDye());
				break;
			}
		}
	}
}

void CReagentItem::InitReagentItem(CDyeInfoList* pListDye, CPtrArray* pArrayChannel)
{
	if(NULL == pListDye)
	{
		return;
	}
	if(NULL == pArrayChannel)
	{
		return;
	}

	DeleteAll();

	int iChannelCount = pArrayChannel->GetCount();
	if(iChannelCount < 1)
	{
		return;
	}

	m_pAnalysisPara = new CDataAnalysisPara;
	if(NULL == m_pAnalysisPara)
	{
		return;
	}

	CString strDyeName(_T(""));
	for (int i = 0;i < iChannelCount;++i)
	{
		tagScanChannelInfo* pChannel = (tagScanChannelInfo*)pArrayChannel->GetAt(i);
		if (pChannel->iType == 4) // 不考虑校准光路
			continue;

		strDyeName.Empty();

		CItemChannelInfo* pItemChannel = new CItemChannelInfo;
		pItemChannel->SetChannel(pChannel->iExPosition);
		pItemChannel->SetExWavelength(pChannel->iExWaveLen);
		pItemChannel->SetEmWavelength(pChannel->iEmWaveLen);
		m_arrayTarget.Add(pItemChannel);

		tagAnaAmplifyPara* pAnaAmpParam = new tagAnaAmplifyPara;
		if(NULL != pAnaAmpParam)
		{
			pAnaAmpParam->iChannelNo = pChannel->iExPosition;
		}

		tagAnaMeltCurvePara* pAnaMeltParam = new tagAnaMeltCurvePara;
		if(NULL != pAnaMeltParam)
		{
			pAnaMeltParam->iChannelNo = pChannel->iExPosition;
		}

		POSITION pos = pListDye->GetHeadPosition();
		while(pos != NULL)
		{
			CDBDyeInfo* pDyeDB = pListDye->GetNext(pos);

			BOOL bSameEx = ::abs(pDyeDB->GetExWavelength() - pChannel->iExWaveLen) < pChannel->iExRange / 2;
			BOOL bSameEm = ::abs(pDyeDB->GetEmWavelength() - pChannel->iEmWaveLen) < pChannel->iEmRange / 2;
			if(bSameEx || bSameEm)
			{
				strDyeName = pDyeDB->GetDye();
				pItemChannel->SetDyeName(strDyeName);

				if(NULL != pAnaAmpParam)
				{
					pAnaAmpParam->strDye = strDyeName;
				}
				if(NULL != pAnaMeltParam)
				{
					pAnaMeltParam->strDye = strDyeName;
				}

				break;
			}
		}

		if(NULL != pAnaAmpParam)
		{
			m_pAnalysisPara->AddAmplifyPara(pAnaAmpParam);
		}
		if(NULL != pAnaMeltParam)
		{
			m_pAnalysisPara->AddMeltCurvePara(pAnaMeltParam);
		}

		m_lnkDyeName.AddTail(strDyeName);
		++m_nSelChanCount;
	}

	int iDataSize = sizeof(double) * m_nSelChanCount * m_nSelChanCount;
	if(iDataSize > 0)
	{
		m_pCrosstalkPara = new double[iDataSize];
		if(NULL == m_pCrosstalkPara)
		{
			return;
		}
		memset(m_pCrosstalkPara,0,iDataSize);
	}
	
	return;
}

void CReagentItem::SetStatus(eSaveStatus nStatus) 
{ 
	if (m_nStatus == STATUS_NEW)
		return;

	m_nStatus = nStatus; 
}

// 判断编辑是否合法，返回标志参数，说明哪项内容不合法
BOOL CReagentItem::IsLegal(int &iOutTip)
{
	

	return TRUE;
} 

BOOL CReagentItem::GetBeEditable()
{
	return STATUS_USING != m_nStatus;
}

CLinkList<CString>& CReagentItem::GetLnkDyeName()
{
	return m_lnkDyeName;
}

int CReagentItem::DyeNameChanged(int iChanId,CString strDye)
{
	if(iChanId < 0)
	{
		return -1;
	}
	if(iChanId >= m_lnkDyeName.GetSize())
	{
		return -2;
	}

	*m_lnkDyeName[iChanId] = strDye;

	return 0;
}



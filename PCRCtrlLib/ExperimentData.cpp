#include "StdAfx.h"
#include "ExperimentData.h"
#include "PublicFun.h"
#include "math.h"
#include "PublicInfo.h"
#include "LanguageInfo.h"
#include "Define.h"
#include "InstrumentPCR.h"
#include "DataAlgorithmPCR.h"
#include "Fitting.h"

#include "libxl.h"
using namespace libxl;

CExperimentData::CExperimentData(void)
{
	m_wFileFlag = wFileMarkPCR; 

	m_wFileVersion = wPCRVersion119; 
	m_wSoftFileMaxVer = wPCRVersion119;

	m_nCalibrateMethod = CALIBRATE_TURBIDITY;
	m_iChannelCount = 0;

	m_nStatus = STATUS_NEW;
	m_strFileOpenPath = _T("");
	m_strFileName = _T("");
	m_strBackupFilePath.Empty();
	m_strBackupFileCreateTime.Empty();

	m_bTemplateFile = FALSE;

	m_bNormalizeSelLog = FALSE;
	m_bDeltaRnSelLog = FALSE;
	m_bFilterSelLog = FALSE;

	m_strIVDFileRstSaveFolder.Empty();
	m_strIVDFileRstSaveName.Empty();
	m_strUserName.Empty();

	m_pInstance = NULL;

	m_strCYSamCategory.Empty();
	m_strCYSamCount.Empty();
	m_strCYSamState.Empty();
	m_strCYProductBN.Empty();
	m_strCYTestItem.Empty();
	m_strCYComment.Empty();
	
	m_strTester.Empty();
	m_strReviewer.Empty();

	m_strTemplateName.Empty();
	m_bExpRunning = FALSE;
	m_bHasZeroValues = FALSE;

	//��ʼ��CY�����ɼ�����
	time_t timer = time(NULL);
	struct tm* localTime = localtime(&timer);
	m_strCYCollectDate.Format(_T("%d%s%02d%s%02d%s%02d%s%02d%s%02d"),localTime->tm_year + 1900,_T("-"),localTime->tm_mon + 1,_T("-"),localTime->tm_mday,_T(" "),localTime->tm_hour,_T(":"),localTime->tm_min,_T(":"),localTime->tm_sec);

	Initialize();

	return;
}

CExperimentData::~CExperimentData(void)
{
	DeleteAll();
}

void CExperimentData::Initialize()
{
	m_pAlgorithm = new CDataAnalysisPCR;
	m_pExpInfo = new CExperimentInfo;
	m_pReagentItem = new CReagentItem;
	m_pPlateInfo = new CPlateTubeInfo;
	m_pTempData = new CTemperatureData;
	m_pAmplifyData = new CPlateAmplifyTubeData;
	m_pMeltData = new CPlateMeltTubeData;
	m_ppFCrosstalkPara = NULL;
	m_ppFMeltCrosstalk = NULL;
	m_pTurbidityPara = NULL;
	m_pAnalysisPara = new CDataAnalysisPara;

	m_bReachLidTemp = TRUE;
	m_iNotReachTempCount = 0;

	m_dCurTem = -1;

	m_iReconnectState = 0;
	m_iConnInterruptCount = 0;
}

void CExperimentData::DeleteAll()
{
	SAFE_DELETE(m_pAlgorithm);

	SAFE_DELETE(m_pExpInfo);
	SAFE_DELETE(m_pReagentItem);

	DeleteDyePara();
	DeleteCrosstalkPara();
	DeleteFilterPara();
	SAFE_DELETE(m_pTurbidityPara);
	SAFE_DELETE(m_pAnalysisPara);

	DeleteRunData(FALSE);
	SAFE_DELETE(m_pPlateInfo);

	m_lnkSegment.Clear();

	m_strIVDFileRstSaveFolder.Empty();
	m_strIVDFileRstSaveName.Empty();

	m_arrayLog.RemoveAll();
}

// ɾ��ɨ��ͨ����Ⱦ�ϲ���
void CExperimentData::DeleteDyePara()
{
	int iCount = m_arrayDye.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagDyeChannelInfo* pItem = (tagDyeChannelInfo*)m_arrayDye.GetAt(i);
		SAFE_DELETE(pItem);
	}

	m_arrayDye.RemoveAll();
}

// ɾ�����������ز���
void CExperimentData::DeleteCrosstalkPara()
{
	for(int t = 0;t < m_iChannelCount;++t)
	{
		delete [] m_ppFCrosstalkPara[t];
	}
	delete [] m_ppFCrosstalkPara;
	m_ppFCrosstalkPara = NULL;

	for(int t = 0;t < m_iChannelCount;++t)
	{
		delete [] m_ppFMeltCrosstalk[t];
	}
	delete [] m_ppFMeltCrosstalk;
	m_ppFMeltCrosstalk = NULL;

	m_iChannelCount = 0;
}

// ɾ���˲��㷨���ò���
void CExperimentData::DeleteFilterPara()
{
	int iCount = m_arrayFilter.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagFilterItem* pItem = (tagFilterItem*)m_arrayFilter.GetAt(i);
		SAFE_DELETE(pItem);
	}

	m_arrayFilter.RemoveAll();
}

// ɾ��ʵ��ӫ������
void CExperimentData::DeleteRunData(BOOL bOnlyData /*= TRUE*/)
{
	if (bOnlyData)
	{
		int iChannelCount = m_pTempData->GetChannelCount();
		m_pTempData->AllocateMemory(iChannelCount);
		
		iChannelCount = m_pAmplifyData->GetChannelCount();
		int iTubeCount = m_pPlateInfo->GetTubeCount();
		m_pAmplifyData->AllocateMemory(iTubeCount, iChannelCount);
		m_pMeltData->AllocateMemory(iTubeCount, iChannelCount);
	}
	else
	{
		SAFE_DELETE(m_pTempData);
		SAFE_DELETE(m_pAmplifyData);
		SAFE_DELETE(m_pMeltData);
	}

	int iCount = m_arrayAddCycle.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagAddCycleInfo* pCycleInfo = (tagAddCycleInfo*)m_arrayAddCycle.GetAt(i);
		SAFE_DELETE(pCycleInfo);
	}
	m_arrayAddCycle.RemoveAll();

	if(NULL != m_pExpInfo)
	{
		m_pExpInfo->SetHasRunSec(0);
	}

	m_arrayLog.RemoveAll();

	m_iReconnectState = 0;
	m_iConnInterruptCount = 0;
}

// ɾ��ʵ��ӫ������
void CExperimentData::DeleteMeltData(BOOL bOnlyData /*= TRUE*/)
{
	if(bOnlyData)
	{
		int iChannelCount = m_pMeltData->GetChannelCount();
		int iTubeCount = m_pPlateInfo->GetTubeCount();
		m_pMeltData->AllocateMemory(iTubeCount,iChannelCount);
	}
	else
	{
		SAFE_DELETE(m_pMeltData);
	}

	return;
}

BOOL CExperimentData::HasRunData()
{
	if (m_pTempData != NULL)
	{
		if (m_pTempData->HasData())
			return TRUE;
	}

	if (m_pAmplifyData != NULL)
	{
		if (m_pAmplifyData->HasData())
			return TRUE;
	}

	if (m_pMeltData != NULL)
	{
		if (m_pMeltData->HasData())
			return TRUE;
	}

	return FALSE;
}

// �����û�����Ⱦ�Ϻ�����ͨ����ʼ������ɨ��ͨ��
void CExperimentData::InitDyeChannelInfo(CDyeInfoList* pListDye, CPtrArray* pArrayChannel)
{
	DeleteDyePara();

	int iChannelCount = pArrayChannel->GetCount();
	for (int i=0; i<iChannelCount; i++)
	{
		tagScanChannelInfo* pChannel = (tagScanChannelInfo*)pArrayChannel->GetAt(i);
		if (pChannel->iType == 4) // ������У׼��·
			continue;

		POSITION pos = pListDye->GetHeadPosition();
		while(pos != NULL)
		{
			CDBDyeInfo* pDyeDB = pListDye->GetNext(pos);

			BOOL bSameEx = ::abs(pDyeDB->GetExWavelength() - pChannel->iExWaveLen) < pChannel->iExRange / 2;
			BOOL bSameEm = ::abs(pDyeDB->GetEmWavelength() - pChannel->iEmWaveLen) < pChannel->iEmRange / 2;
			if(bSameEx || bSameEm)
			{
				tagDyeChannelInfo* pDye = new tagDyeChannelInfo;
				pDye->iChannel = pChannel->iExPosition;
				pDye->strTarget = pDyeDB->GetTarget();
				pDye->strDye = pDyeDB->GetDye();
				pDye->strQuencher = pDyeDB->GetQuencher();
				pDye->clrTarget = CPublicFun::ConverToColor(pDyeDB->GetColor());
				pDye->bDefault = pDyeDB->GetDefault();

				m_arrayDye.Add(pDye);
			}
		}
	}
}

int CExperimentData::SetTubeDefaultTargetColor()
{
	int iTubeNum = GetTubeCount();
	if(iTubeNum < 1)
	{
		return -1;
	}

	if(m_iChannelCount < 1)
	{
		return -2;
	}

	int iDyeChanNum = m_arrayDye.GetCount();
	if(iDyeChanNum < 1)
	{
		return -3;
	}

	COLORREF* pClrAll = new COLORREF[m_iChannelCount];
	if(NULL == pClrAll)
	{
		return -4;
	}

	tagDyeChannelInfo* pDyeChanInfo = NULL;
	for(int i = 0;i < m_iChannelCount;++i)
	{
		pClrAll[i] = RGB(255,0,0);
		for(int j = 0;j < iDyeChanNum;++j)
		{
			pDyeChanInfo = (tagDyeChannelInfo*)m_arrayDye.GetAt(j);
			if(NULL == pDyeChanInfo || !pDyeChanInfo->bDefault)
			{
				continue;
			}
			if(i + 1 == pDyeChanInfo->iChannel)
			{
				pClrAll[i] = pDyeChanInfo->clrTarget;
				break;
			}
		}
	}

	CTubeInfo* pTubeInfo = NULL;
	int iTargetCount = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;

	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = m_pPlateInfo->GetTubeInfo(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		
		iTargetCount = pTubeInfo->GetTargetCount();
		if(iTargetCount < 1 || iTargetCount != m_iChannelCount)
		{
			continue;
		}

		for(int j = 0;j < m_iChannelCount;++j)
		{
			pDyeTargetInfo = pTubeInfo->FindTargetInfoByChannel(j + 1);
			if(NULL == pDyeTargetInfo)
			{
				continue;
			}
			pDyeTargetInfo->SetTargetColor(pClrAll[j]);
		}
	}

	delete [] pClrAll;

	return 0;
}

COLORREF CExperimentData::GetChannelColor(int iChanId)
{
	int iTubeNum = GetTubeCount();
	if(iTubeNum < 1)
	{
		return 0;
	}

	if(m_iChannelCount < 1)
	{
		return 0;
	}

	if(iChanId < 0 || iChanId >= m_iChannelCount)
	{
		return 0;
	}

	CTubeInfo* pTubeInfo = m_pPlateInfo->GetTubeInfo(0);
	if(NULL == pTubeInfo)
	{
		return 0;
	}

	return pTubeInfo->GetSampleDyeTargetColor(iChanId);
}

int CExperimentData::SetTubeDefaultDyeName()
{
	int iTubeNum = GetTubeCount();
	if(iTubeNum < 1)
	{
		return -1;
	}

	if(m_iChannelCount < 1)
	{
		return -2;
	}

	CString* pStrDyeName = new CString[m_iChannelCount];
	if(NULL == pStrDyeName)
	{
		return -3;
	}

	BOOL bPs96 = FALSE;
	if(m_hardwareInfo.GetBeEmptyHardware())
	{
#ifdef PS96INSTRUMENT
		bPs96 = TRUE;
#endif
	}
	else
	{
		if(GetBePS96ExperFile())
		{
			bPs96 = TRUE;
		}
	}

	if(bPs96)
	{
		for(int i = 0;i < m_iChannelCount;++i)
		{
			if(0 == i)
			{
				pStrDyeName[i] = _T("FAM");
			}
			else if(1 == i)
			{
				pStrDyeName[i] = _T("Cy5");
			}
			else if(2 == i)
			{
				pStrDyeName[i] = _T("VIC");
			}
			else if(3 == i)
			{
				pStrDyeName[i] = _T("Cy5.5");
			}
			else if(4 == i)
			{
				pStrDyeName[i] = _T("ROX");
			}
			else if(5 == i)
			{
				pStrDyeName[i] = _T("MOT");
			}
			else 
			{
				pStrDyeName[i] = _T("");
			}
		}
	}
	else
	{
		for(int i = 0;i < m_iChannelCount;++i)
		{
			if(0 == i)
			{
				pStrDyeName[i] = _T("FAM");
			}
			else if(1 == i)
			{
				pStrDyeName[i] = _T("Cy5");
			}
			else if(2 == i)
			{
				pStrDyeName[i] = _T("VIC");
			}
			else if(3 == i)
			{
				pStrDyeName[i] = _T("ROX");
			}
			else if(4 == i)
			{
				pStrDyeName[i] = _T("Cy5.5");
			}
			else
			{
				pStrDyeName[i] = _T("");
			}
		}
	}

	CTubeInfo* pTubeInfo = NULL;
	int iTargetCount = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;

	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = m_pPlateInfo->GetTubeInfo(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}

		iTargetCount = pTubeInfo->GetTargetCount();
		if(iTargetCount < 1 || iTargetCount != m_iChannelCount)
		{
			continue;
		}

		for(int j = 0;j < m_iChannelCount;++j)
		{
			pDyeTargetInfo = pTubeInfo->FindTargetInfoByChannel(j + 1);
			if(NULL == pDyeTargetInfo)
			{
				continue;
			}
			pDyeTargetInfo->SetDyeName(pStrDyeName[j]);
		}
	}

	delete [] pStrDyeName;

	return 0;
}

vector<CString>& CExperimentData::GetDefaultDyeName()
{
	m_vecInstruDefDyeName.clear();

	int iTubeNum = GetTubeCount();
	if(iTubeNum < 1)
	{
		return m_vecInstruDefDyeName;
	}

	if(m_iChannelCount < 1)
	{
		return m_vecInstruDefDyeName;
	}

	for(int i = 0;i < m_iChannelCount;++i)
	{
		m_vecInstruDefDyeName.push_back(_T(""));
	}

	BOOL bPs96 = FALSE;
	if(m_hardwareInfo.GetBeEmptyHardware())
	{
#ifdef PS96INSTRUMENT
		bPs96 = TRUE;
#endif
	}
	else
	{
		if(GetBePS96ExperFile())
		{
			bPs96 = TRUE;
		}
	}

	if(bPs96)
	{
		for(int i = 0;i < m_iChannelCount;++i)
		{
			if(0 == i)
			{
				m_vecInstruDefDyeName[i] = _T("FAM");
			}
			else if(1 == i)
			{
				m_vecInstruDefDyeName[i] = _T("Cy5");
			}
			else if(2 == i)
			{
				m_vecInstruDefDyeName[i] = _T("VIC");
			}
			else if(3 == i)
			{
				m_vecInstruDefDyeName[i] = _T("Cy5.5");
			}
			else if(4 == i)
			{
				m_vecInstruDefDyeName[i] = _T("ROX");
			}
			else if(5 == i)
			{
				m_vecInstruDefDyeName[i] = _T("MOT");
			}
		}
	}
	else
	{
		for(int i = 0;i < m_iChannelCount;++i)
		{
			if(0 == i)
			{
				m_vecInstruDefDyeName[i] = _T("FAM");
			}
			else if(1 == i)
			{
				m_vecInstruDefDyeName[i] = _T("Cy5");
			}
			else if(2 == i)
			{
				m_vecInstruDefDyeName[i] = _T("VIC");
			}
			else if(3 == i)
			{
				m_vecInstruDefDyeName[i] = _T("ROX");
			}
			else if(4 == i)
			{
				m_vecInstruDefDyeName[i] = _T("Cy5.5");
			}
		}
	}

	return m_vecInstruDefDyeName;
}

BOOL CExperimentData::WriteSegmentInfo(CFile* pFile)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	int nCount = m_lnkSegment.GetSize();
	pFile->Write((const char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		m_lnkSegment.GetAt(i)->data.WriteFile(pFile);
	}

	return TRUE;
}

BOOL CExperimentData::ReadSegmentInfo(CFile* pFile,WORD wVersion)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

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

// ���桢��ȡȾ��ͨ����Ϣ
BOOL CExperimentData::WriteDyeInfo(CFile* pFile)
{
	int nCount = m_arrayDye.GetCount();
	pFile->Write((const char*)&nCount, sizeof(int));
	for(int i=0; i<nCount; i++)
	{
		tagDyeChannelInfo* pDyeInfo = (tagDyeChannelInfo*)m_arrayDye.GetAt(i);
		pFile->Write((const char *)&pDyeInfo->iChannel, sizeof(int));
		CPublicFun::WriteStringToFile(pFile, pDyeInfo->strTarget);
		CPublicFun::WriteStringToFile(pFile, pDyeInfo->strDye);
		CPublicFun::WriteStringToFile(pFile, pDyeInfo->strQuencher);
		pFile->Write((const char *)&pDyeInfo->clrTarget, sizeof(COLORREF));
		pFile->Write((const char *)&pDyeInfo->bDefault, sizeof(BOOL));
	}

	return TRUE;
}

BOOL CExperimentData::ReadDyeInfo(CFile* pFile,WORD wVersion)
{
	int nCount = 0;
	pFile->Read((char*)&nCount, sizeof(int));
	for (int i=0; i<nCount; i++)
	{
		tagDyeChannelInfo* pDyeInfo = new tagDyeChannelInfo;
		m_arrayDye.Add(pDyeInfo);
		pFile->Read((char*)&pDyeInfo->iChannel, sizeof(int));
		pDyeInfo->strTarget = CPublicFun::ReadStringFromFile(pFile);
		pDyeInfo->strDye = CPublicFun::ReadStringFromFile(pFile);
		pDyeInfo->strQuencher = CPublicFun::ReadStringFromFile(pFile);
		pFile->Read((char *)&pDyeInfo->clrTarget, sizeof(COLORREF));
		pFile->Read((char*)&pDyeInfo->bDefault, sizeof(BOOL));
	}
	return TRUE;
}

BOOL CExperimentData::WriteDefaultAxisInfo(CFile* pFile)
{
	if(NULL == pFile)
	{
		return FALSE;
	}
	
	int iAxisNum = m_lnkDefAxisInfo.GetSize();
	pFile->Write((const char*)&iAxisNum,sizeof(int));

	Node<CAxisInfo>* pNode = m_lnkDefAxisInfo.GetHead();
	while(NULL != pNode)
	{
		pFile->Write((const char*)&(pNode->data.m_dXMin),sizeof(double));
		pFile->Write((const char*)&(pNode->data.m_dXMax),sizeof(double));
		pFile->Write((const char*)&(pNode->data.m_dYMin),sizeof(double));
		pFile->Write((const char*)&(pNode->data.m_dYMax),sizeof(double));
		pNode = pNode->pNext;
	}
	
	return TRUE;
}

BOOL CExperimentData::ReadDefaultAxisInfo(CFile* pFile,WORD wVersion)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	m_lnkDefAxisInfo.Clear();
	int iAxisNum = m_lnkDefAxisInfo.GetSize();
	pFile->Read((char*)&iAxisNum,sizeof(int));

	for(int i = 0;i < iAxisNum;++i)
	{
		CAxisInfo axisInfo;
		pFile->Read((char*)&axisInfo.m_dXMin,sizeof(double));
		pFile->Read((char*)&axisInfo.m_dXMax,sizeof(double));
		pFile->Read((char*)&axisInfo.m_dYMin,sizeof(double));
		pFile->Read((char*)&axisInfo.m_dYMax,sizeof(double));
		m_lnkDefAxisInfo.AddTail(axisInfo);
	}

	return TRUE;
}

BOOL CExperimentData::SaveFile(int iFile)
{
	if (iFile == 0) // ���������ļ�
	{
		if (m_strFileOpenPath.IsEmpty())
		{
			m_strFileOpenPath = m_pExpInfo->GetSaveDataFileName();
		}

		return SaveFile(m_strFileOpenPath);
	}
	else
	{ // ���汸���ļ�
		return SaveFile(m_strBackupFilePath);
	}
}

//lzh:����Ϊ������ģ���ļ��ĺ���
BOOL CExperimentData::SaveFile(CString strFilePath, BOOL bTemplate)
{
	CString strSuffix = _T(".");
	if (bTemplate)
	{
		strSuffix += PCR_TEMPLATE_FILE_SUFFIX;
	}
	else
	{
		strSuffix += PCR_DATA_FILE_SUFFIX;
	}
	if (strFilePath.Find(strSuffix) == -1)
	{
		strFilePath += strSuffix;
	}

	CFile file;
	CFileException e;
	if (!file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		if (e.m_cause == CFileException::fileNotFound)
		{
			if (!file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	// д���ļ���ʶ�Ͱ汾
	file.Write((const char*)&m_wFileFlag, sizeof(WORD));
	file.Write((const char*)&m_wFileVersion, sizeof(WORD));

	// д��ʵ�������Ϣ
	m_pExpInfo->WriteFile(&file,bTemplate);

	// д���Լ���Ŀ�����Ϣ���������̡����������
	m_pReagentItem->WriteFile(&file,m_wFileVersion,FALSE);

	// д��װ�༭��Ϣ
	m_pPlateInfo->WriteFile(&file,m_wFileVersion);

	//д��ʵ�鲽����Ϣ
	WriteSegmentInfo(&file);

	// д���ʹ�õ�Ⱦ��ͨ��
	WriteDyeInfo(&file);

	// д������ʹ�õĽ������ϵ��
	file.Write((const char*)&m_iChannelCount, sizeof(int));
	for(int i = 0;i < m_iChannelCount;++i)
	{
		for(int j = 0;j < m_iChannelCount;++j)
		{
			float fTemp = m_ppFCrosstalkPara[i][j];
			file.Write((const char*)&fTemp, sizeof(float));
		}
	}

	if(m_wFileVersion >= 0x0110)
	{
		// д������ʹ�õĽ������ϵ��
		for(int i = 0;i < m_iChannelCount;++i)
		{
			for(int j = 0;j < m_iChannelCount;++j)
			{
				float fTemp = m_ppFMeltCrosstalk[i][j];
				file.Write((const char*)&fTemp, sizeof(float));
			}
		}
	}

	// д���˲��㷨��ز���
	int iCount = m_arrayFilter.GetCount();
	file.Write((const char*)&iCount, sizeof(int));
	for(int i=0; i<iCount; i++)
	{
		tagFilterItem* pItem = (tagFilterItem*)m_arrayFilter.GetAt(i);
		file.Write((const char*)&(pItem->iFilterUser), sizeof(int));
		file.Write((const char*)&(pItem->nType), sizeof(int));
		file.Write((const char*)&(pItem->iTimes), sizeof(int));
		file.Write((const char*)&(pItem->iPara1), sizeof(int));
		file.Write((const char*)&(pItem->iPara2), sizeof(int));
	}

	// д��У׼������Ϣ
	file.Write((const char*)&m_nCalibrateMethod, sizeof(int));
	if (m_pTurbidityPara != NULL)
	{
		m_pTurbidityPara->WriteFile(&file);
	}

	// д���������
	m_pAnalysisPara->WriteFile(&file,m_wFileVersion,FALSE);

	if (!bTemplate)
	{
		// д���¶�����
		m_pTempData->WriteFile(&file);
		// д����������
		m_pAmplifyData->WriteFile(&file);
		// д���۽�����
		m_pMeltData->WriteFile(&file);

		iCount = m_arrayAddCycle.GetCount();
		file.Write((const char*)&iCount, sizeof(int));
		for(int i=0; i<iCount; i++)
		{
			tagAddCycleInfo* pAddInfo = (tagAddCycleInfo*)m_arrayAddCycle.GetAt(i);
			file.Write((const char*)&(pAddInfo->iSegmentNo), sizeof(int));
			file.Write((const char*)&(pAddInfo->iAddNum), sizeof(int));
		}

		// д��ʵ����־

	}

	//����Ӳ����Ϣ���������кŵȣ�
	m_hardwareInfo.WriteFile(&file,bTemplate);

	//�������߿ؼ�����ģʽ������Ϣ
	if(m_wFileVersion >= wPCRVersion107)
	{
		file.Write((const char*)&m_bNormalizeSelLog,sizeof(BOOL));
		file.Write((const char*)&m_bDeltaRnSelLog,sizeof(BOOL));
		file.Write((const char*)&m_bFilterSelLog,sizeof(BOOL));
	}

	if(m_wFileVersion >= 0x0110)
	{
		//���������������Сֵ��Ϣ
		WriteDefaultAxisInfo(&file);
	}

	if(m_wFileVersion >= 0X111)
	{
		//�����û���
		CPublicFun::WriteStringToFile(&file,m_strUserName);
	}

	//���濵���ṫ����Ϣ
	if(m_wFileVersion >= 0X112)
	{
		CPublicFun::WriteStringToFile(&file,m_pExpInfo->GetCYReportNo());
		CPublicFun::WriteStringToFile(&file,m_strCYSamCategory);
		CPublicFun::WriteStringToFile(&file,m_strCYSamCount);
		CPublicFun::WriteStringToFile(&file,m_strCYSamState);
		CPublicFun::WriteStringToFile(&file,m_strCYProductBN);
		CPublicFun::WriteStringToFile(&file,m_strCYTestItem);
		CPublicFun::WriteStringToFile(&file,m_strCYComment);
		CPublicFun::WriteStringToFile(&file,m_strCYCollectDate);
	}
	
	if(m_wFileVersion >= 0X113)
	{
		CPublicFun::WriteStringToFile(&file,m_strTester);
		CPublicFun::WriteStringToFile(&file,m_strReviewer);
	}

	if(m_wFileVersion >= 0X114)
	{
		m_hardwareInfo.WriteScanChannelInfoToFile(&file,bTemplate);
	}

	WriteLog(&file);

	file.Write((const char*)&m_iReconnectState, sizeof(int));
	file.Write((const char*)&m_iConnInterruptCount, sizeof(int));
	file.Write((const char*)&m_bHasZeroValues, sizeof(BOOL));

	file.Close();

	if (!bTemplate) // ����Ǳ���ģ�壬�򲻸ı䵱ǰ�ļ�״̬
	{
		if (m_nStatus != STATUS_USING) // ʵ������б����ļ�����Ҫ�ı�״̬
		{
			if (HasRunData())
			{
				m_nStatus = STATUS_SAVE_DATA;
			}
			else
			{
				m_nStatus = STATUS_SAVE;
			}
		}
	}

	return TRUE;
}

int CExperimentData::OpenFile(CString strFilePath, BOOL bTemplate)
{
	m_strFileOpenPath = strFilePath;

	if (!bTemplate) // �����ģ���ļ���Ϊ�����ļ�ѡ��
	{
		CString strSuffix = _T(".") + PCR_TEMPLATE_FILE_SUFFIX;
		if (strFilePath.Find(strSuffix) != -1)
		{
			bTemplate = TRUE;
		}
	}

	CFile file;       
	if (!file.Open(strFilePath,CFile::modeRead | CFile::shareDenyNone))
	{
		return -1;
	}

	int iErrCode = -2;

	// ��ȡ�ļ���ʶ�Ͱ汾
	WORD wFileMark = 0;
	file.Read(&wFileMark, sizeof(WORD));
	if (wFileMark != wFileMarkPCR)
	{
		goto OPENFAILED;
	}

	WORD wFileVersion = 0;
	file.Read(&wFileVersion,sizeof(WORD));

	if(wFileVersion > m_wSoftFileMaxVer)
	{
		//�ļ��汾�ϸߣ������޷���
		iErrCode = -3;
		goto OPENFAILED;
	}

	// ��ȡʵ�������Ϣ
	if(!m_pExpInfo->ReadFile(&file,wFileVersion))
	{
		iErrCode = -4;
		goto OPENFAILED;
	}

	// ��ȡ�Լ���Ŀ�����Ϣ���������̡����������
	if(!m_pReagentItem->ReadFile(&file,wFileVersion,FALSE))
	{
		iErrCode = -5;
		goto OPENFAILED;
	}

	// ��ȡ�װ�༭��Ϣ
	if(!m_pPlateInfo->ReadFile(&file,wFileVersion))
	{
		iErrCode = -6;
		goto OPENFAILED;
	}

	//��ȡʵ�鲽����Ϣ
	if(!ReadSegmentInfo(&file,wFileVersion))
	{
		iErrCode = -7;
		goto OPENFAILED;
	}

	// ��ȡ��ʹ�õ�Ⱦ��ͨ��
	if(!ReadDyeInfo(&file,wFileVersion))
	{
		iErrCode = -8;
		goto OPENFAILED;
	}

	int iTempInt = 0;
	int iCount = 0;
	float fTemp = 0.0f;
	//��ȡ����ʹ�õĽ������ϵ��
	file.Read(&iCount, sizeof(int));
	SetChannelCount(iCount, TRUE);
	for(int i = 0;i < iCount;++i)
	{
		for(int j = 0;j < iCount;++j)
		{
			file.Read(&fTemp,sizeof(float));
			m_ppFCrosstalkPara[i][j] = fTemp;
		}
	}

	//��ȡ����ʹ�õĽ������ϵ��
	if(wFileVersion >= 0x0110)
	{
		for(int i = 0;i < iCount;++i)
		{
			for(int j = 0;j < iCount;++j)
			{
				file.Read(&fTemp,sizeof(float));
				m_ppFMeltCrosstalk[i][j] = fTemp;
			}
		}
	}

	// ��ȡ�˲��㷨��ز���
	file.Read(&iCount, sizeof(int));
	for (int i=0; i<iCount; i++)
	{
		tagFilterItem* pItem = new tagFilterItem;
		file.Read(&(pItem->iFilterUser), sizeof(int));
		file.Read(&iTempInt, sizeof(int));
		pItem->nType = (eFilterType)iTempInt;
		file.Read(&(pItem->iTimes), sizeof(int));
		file.Read(&(pItem->iPara1), sizeof(int));
		file.Read(&(pItem->iPara2), sizeof(int));

		m_arrayFilter.Add(pItem);
	}

	//��ȡУ׼������Ϣ
	file.Read(&iTempInt, sizeof(int));
	m_nCalibrateMethod = (eCalibrateMethod)iTempInt;
	if (m_nCalibrateMethod == CALIBRATE_TURBIDITY)
	{
		if (m_pTurbidityPara == NULL)
		{
			m_pTurbidityPara = new CCalibrateParaTurbidity;
		}
		m_pTurbidityPara->ReadFile(&file,wFileVersion);
		if (wFileVersion <= wPCRVersion101)
		{
			m_pTurbidityPara->ResetChannel(&m_arrayDye);
		}
	}

	// ��ȡ��������
	if(!m_pAnalysisPara->ReadFile(&file,wFileVersion,FALSE))
	{
		iErrCode = -9;
		goto OPENFAILED;
	}

	m_bTemplateFile = bTemplate;
	if (bTemplate)
	{
		m_pExpInfo->SetExpName(m_pExpInfo->CreateExperimentName());
		m_pExpInfo->SetFilePath(CPublicInfo::GetInstance()->GetUserDataSavePath());
	}
	else
	{
		// ��ȡ�¶����� 
		if(!m_pTempData->ReadFile(&file,wFileVersion))
		{
			iErrCode = -10;
			goto OPENFAILED;
		}

		// ��ȡ��������
		if(!m_pAmplifyData->ReadFile(&file,wFileVersion))
		{
			iErrCode = -11;
			goto OPENFAILED;
		}

		// ��ȡ�۽�����
		if(!m_pMeltData->ReadFile(&file,wFileVersion))
		{
			iErrCode = -12;
			goto OPENFAILED;
		}

		file.Read(&iCount, sizeof(int));
		for (int i=0; i<iCount; i++)
		{
			tagAddCycleInfo* pAddInfo = new tagAddCycleInfo;
			file.Read(&(pAddInfo->iSegmentNo), sizeof(int));
			file.Read(&(pAddInfo->iAddNum), sizeof(int));
			m_arrayAddCycle.Add(pAddInfo);
		}
	}

	//��ȡӲ����Ϣ
	m_hardwareInfo.ReadFile(&file,wFileVersion);

	//��ȡ���߿ؼ�����ģʽ������Ϣ
	if(wFileVersion >= wPCRVersion107)
	{
		file.Read(&m_bNormalizeSelLog,sizeof(BOOL));
		file.Read(&m_bDeltaRnSelLog,sizeof(BOOL));
		file.Read(&m_bFilterSelLog,sizeof(BOOL));
	}

	if(wFileVersion >= 0x0110)
	{
		//��ȡ�����������Сֵ��Ϣ
		ReadDefaultAxisInfo(&file,wFileVersion);
	}

	if(wFileVersion >= 0X111)
	{
		//��ȡ�û���
		m_strUserName = CPublicFun::ReadStringFromFile(&file);
	}

	//��ȡ�����ṫ����Ϣ
	if(wFileVersion >= 0X112)
	{
		m_pExpInfo->SetCYReportNo(CPublicFun::ReadStringFromFile(&file));
		m_strCYSamCategory = CPublicFun::ReadStringFromFile(&file);
		m_strCYSamCount = CPublicFun::ReadStringFromFile(&file);
		m_strCYSamState = CPublicFun::ReadStringFromFile(&file);
		m_strCYProductBN = CPublicFun::ReadStringFromFile(&file);
		m_strCYTestItem = CPublicFun::ReadStringFromFile(&file);
		m_strCYComment = CPublicFun::ReadStringFromFile(&file);
		m_strCYCollectDate = CPublicFun::ReadStringFromFile(&file);
	}

	if(wFileVersion >= 0X113)
	{
		m_strTester = CPublicFun::ReadStringFromFile(&file);
		m_strReviewer = CPublicFun::ReadStringFromFile(&file);
	}

	if(wFileVersion >= 0X114)
	{
		m_hardwareInfo.ReadScanChannelInfoFromFile(&file,wFileVersion,bTemplate);
	}

	if (wFileVersion >= wPCRVersion116)
	{
		ReadLog(&file, wFileVersion);
	}

	if (wFileVersion >= wPCRVersion118)
	{
		file.Read(&m_iReconnectState, sizeof(int));
		file.Read(&m_iConnInterruptCount, sizeof(int));
	}
	if (wFileVersion >= wPCRVersion119)
	{
		file.Read(&m_bHasZeroValues, sizeof(BOOL));
	}

	file.Close();

	if (HasRunData())
	{
		m_nStatus = STATUS_SAVE_DATA;
	}
	else
	{
		m_nStatus = STATUS_SAVE;
	}

	if (m_bTemplateFile)
	{
		if (m_pAnalysisPara->GetAmplifyParaCount() == 0)
		{
			ASSERT(FALSE);
		}

		//m_pAnalysisPara->Clear();
		//InitAmplifyParamByTubeTarget();
	}

	m_bExpRunning = FALSE;

	return 0;

OPENFAILED:
	file.Close();
	return iErrCode;
}

// �Լ���Ŀ��ز�������
void CExperimentData::PasteReagentItemInfo(CReagentItem* pInputItem)
{
	m_pReagentItem->CopyFrom(pInputItem);

	// ����ʵ������
	if (pInputItem->GetSegment().GetSize() != 0)
		SetSegment(pInputItem->GetSegment());

	// ���ƽ������ϵ��
	CIntArray	arrayChannel;
	pInputItem->GetUsedChannel(&arrayChannel);
	double* pInputCrosetalk =  pInputItem->GetCrosstalkPara();
	int iUseCount = arrayChannel.GetCount();
	for (int iRow=0; iRow<iUseCount; iRow++)
	{
		int iRowChannel = arrayChannel.GetAt(iRow);
		for (int iCol=0; iCol<iUseCount; iCol++)
		{
			int iColChannel = arrayChannel.GetAt(iCol);
			if (pInputCrosetalk != NULL)
			{
				m_ppFCrosstalkPara[iRowChannel][iColChannel] = (float)pInputCrosetalk[iRow*iUseCount + iCol];
				m_ppFMeltCrosstalk[iRowChannel][iColChannel] = (float)pInputCrosetalk[iRow*iUseCount + iCol];
			}
		}
	}

	// �������ݷ�������
	if (m_pAnalysisPara == NULL)
	{
		m_pAnalysisPara = new CDataAnalysisPara;
	}

	if (pInputItem->GetAnalysisPara()->GetAmplifyParaCount() != 0)
	{
		m_pAnalysisPara->CopyFrom(pInputItem->GetAnalysisPara());
	}
}

// �����Լ���Ŀ��Ϣ���ÿװ����ԣ��������Ϊ�գ���ʹ�ó�Ա��Ŀ��Ϣ
void CExperimentData::InitDyeTargetInfo(int nTubeID)
{
	// ������Ŀ��Ϣ�Լ��ָ��������и�ֵ
	m_pPlateInfo->InitDyeTargetInfo(nTubeID, m_pReagentItem);
}

int CExperimentData::GetTempChannelCount()
{
	if(NULL == m_pTempData)
	{
		return 0;
	}

	return m_pTempData->GetChannelCount();
}

void CExperimentData::SetTempChannelCount(int iCount)
{
	if(NULL == m_pTempData)
	{
		m_pTempData = new CTemperatureData;
	}

	m_pTempData->AllocateMemory(iCount);
}

// ����һ���¶�ֵ
void CExperimentData::AddTempData(DWORD dwTime,int nTempCount,UINT* pInputTemp,BOOL bMeltStepRunning)
{
	m_pTempData->AddOneTempData(dwTime, nTempCount, pInputTemp,bMeltStepRunning);
}

// ȡ�����PCR���¶�
UINT CExperimentData::GetCurTemp(CTemperatureData::eTempChannel nTempChannel, int iChannelNo)
{
	return m_pTempData->GetCurTemp(nTempChannel, iChannelNo);
}

DWORD CExperimentData::GetCurTempCollectTime()
{
	return m_pTempData->GetCurTempCollectTime();
}

void CExperimentData::SetLidHeater(BOOL bUseLid) 
{
	return m_pTempData->SetUseLidHeater(bUseLid);
}

int CExperimentData::GetTempDataCount()
{
	return m_pTempData->GetDataCount();
}

void CExperimentData::GetTempTimeData(int iDataCount, double* pdValueTime)
{
	return m_pTempData->GetTimeData(iDataCount, pdValueTime);
}

void CExperimentData::GetTempChannelData(CTemperatureData::eTempChannel nTempChannel, int iChannelNo, int iDataCount, double* pdValueTemp)
{
	return m_pTempData->GetChannelData(nTempChannel, iChannelNo, iDataCount, pdValueTemp);
}


void CExperimentData::SetScanStartPos(eScanStartPos nPos)
{
	if (m_pAmplifyData == NULL)
	{
		m_pAmplifyData = new CPlateAmplifyTubeData;
	}
	m_pAmplifyData->SetScanStartPos(nPos);
}

CTemperatureData* CExperimentData::GetTemperatureData()
{
	return m_pTempData;
}

void CExperimentData::AddAmplifyCycleX(CUIntArray* pArrayCycle)
{
	return m_pAmplifyData->AddCycleX(pArrayCycle);
}

// ׷��������ԭʼ����
int CExperimentData::AddAmplifyFluOriginalData(CUIntArray& aryOriData,int chanId)
{
	int iReturn = m_pAmplifyData->AddOriginalData(aryOriData, chanId);
	return iReturn;
}

void CExperimentData::AddMeltCycleX(CUIntArray* pArrayCycle)
{
	return m_pMeltData->AddCycleX(pArrayCycle);
}

// ׷���۽��ԭʼ����
int CExperimentData::AddMeltFluOriginalData(CUIntArray& aryOriData,int chanId,double dTem)
{
	int iReturn = m_pMeltData->AddOriginalData(aryOriData,chanId,dTem);
	return iReturn;
}

void CExperimentData::GetAmplifyCycleX(int iTubeID, int iCycleCount, double* pdValueX)
{
	m_pAmplifyData->GetCycleX(iTubeID, iCycleCount, pdValueX, m_pExpInfo->GetTempMode() == TEMP_MODE_CONSTANT);
}

UINT CExperimentData::GetCurrentAmplifyCycleX(int iTubeID)
{
	return m_pAmplifyData->GetCurrentCycleX(iTubeID);
}

void CExperimentData::GetAmplifyChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdValueY)
{
	m_pAmplifyData->GetChannelFluY(iTubeID, nType, iChannel, iCycleCount, pdValueY);
}

double CExperimentData::GetAmplifyChannelCycleFluY(int iTubeID, eDataType nType, int iChannel, int iCycleId)
{
	return m_pAmplifyData->GetChannelCycleFluY(iTubeID, nType, iChannel, iCycleId);
}

void CExperimentData::GetMeltCycleX(int iTubeID, int iCycleCount, double* pdValueX)
{
	m_pMeltData->GetCycleX(iTubeID, iCycleCount, pdValueX, FALSE);
}

double CExperimentData::GetMeltCycleX(int iTubeID, int iCycleId)
{
	return m_pMeltData->GetCycleX(iTubeID,iCycleId,FALSE);
}

void CExperimentData::GetMeltChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdValueY)
{
	m_pMeltData->GetChannelFluY(iTubeID, nType, iChannel, iCycleCount, pdValueY);
}

double CExperimentData::GetMeltChannelCycleFluY(int iTubeID, eDataType nType, int iChannel, int iCycleId)
{
	return m_pMeltData->GetChannelCycleFluY(iTubeID, nType, iChannel, iCycleId);
}

int CExperimentData::GetAmplifyDataNum()
{
	return m_pAmplifyData->GetDataNum();
}

int CExperimentData::GetMeltDataNum()
{
	return m_pMeltData->GetDataNum();
}

int CExperimentData::AddCycle(int iSegment,int iCount)
{
	if(iSegment < 0)
	{
		return -1;
	}
	if(iSegment >= m_lnkSegment.GetSize())
	{
		return -2;
	}

	m_lnkSegment[iSegment]->m_iCycleNum += iCount;

	return 0;

	/*tagAddCycleInfo* pAddCycle = new tagAddCycleInfo;
	pAddCycle->iSegmentNo = iSegment;
	pAddCycle->iAddNum = iCount;
	m_arrayAddCycle.Add(pAddCycle);*/
}

void CExperimentData::SetTubeInfo(vector<CTubeInfo>& vecTubeInfo)
{
	if(NULL != m_pPlateInfo)
	{
		m_pPlateInfo->SetTubeInfo(vecTubeInfo);
	}

	return;
}

void CExperimentData::SetTubeInfo(vector<CTubeInfo*>& vecPTubeInfo)
{
	if(NULL != m_pPlateInfo)
	{
		m_pPlateInfo->SetTubeInfo(vecPTubeInfo);
	}

	return;
}

int CExperimentData::SetTubeInfo(CTubeInfo* pTotalTubeInfo,int iTubeNum)
{
	if(NULL == m_pPlateInfo)
	{
		return -1;
	}

	if(0 != m_pPlateInfo->SetTubeInfo(pTotalTubeInfo,iTubeNum))
	{
		return -2;
	}

	return 0;
}

void CExperimentData::GetTubeInfo(vector<CTubeInfo>& vecTubeInfo)
{
	if(NULL != m_pPlateInfo)
	{
		m_pPlateInfo->GetTubeInfo(vecTubeInfo);
	}

	return;
}

void CExperimentData::GetAllTubeInfoPtr(vector<CTubeInfo*>& vecPTubeInfo)
{
	return m_pPlateInfo->GetAllTubeInfoPtr(vecPTubeInfo);
}

CPtrArray* CExperimentData::GetTubeInfoArrayPtr()
{
	if(NULL == m_pPlateInfo)
	{
		return NULL;
	}

	return m_pPlateInfo->GetTubeInfoArrayPtr();
}

CTubeInfo* CExperimentData::GetTubeInfoAt(int iIndex)
{
	if(NULL == m_pPlateInfo)
	{
		return NULL;
	}

	return m_pPlateInfo->GetTubeInfo(iIndex);
}

// ����������ӫ������
BOOL CExperimentData::ProcessAmplifyFluData(int nFlag /*= 5*/)
{
	int iAmpDataLen = GetAmplifyDataNum();
	if(iAmpDataLen < 1)
	{
		return FALSE;
	}

	// ��ѡ��Ӧ�ܵ����ݽ���ͨ�����š��˲����Ƕȵ���
	int iTubeCount = m_pPlateInfo->GetTubeCount();
	for(int i = 0;i < iTubeCount;++i)
	{
		CTubeInfo* pTubeInfo = (CTubeInfo*)m_pPlateInfo->GetTubeInfo(i);
		if(!pTubeInfo->HasUsed())
		{
			continue;
		}

		if(CALIBRATE_TURBIDITY == m_nCalibrateMethod)
		{
			pTubeInfo->SetCalibrateChannel(m_pTurbidityPara->GetChannelMOT());
		}

		if(nFlag >= 4)
		{
			// �������ϵ������
			m_pAmplifyData->FluChannelCrosstalkAdjust(pTubeInfo,m_ppFCrosstalkPara);
		}

		if(nFlag >= 3 && !m_bExpRunning)
		{
			// �˲�
			//m_pAmplifyData->FluDigitalFilter(pTubeInfo,&m_arrayFilter,m_pAlgorithm);
			m_pAmplifyData->FluDigitalFilter(pTubeInfo,&m_arrayFilter,m_pAlgorithm, 1); // ֻ������ֵ�˲�
		}
		if(nFlag >= 2 && !m_bExpRunning)
		{
			// ���ߵ���
			m_pAmplifyData->FluoAdjustBaseline(pTubeInfo, m_pAnalysisPara, m_pAlgorithm, m_pTurbidityPara);
			// �Ƕȵ���
			m_pAmplifyData->FluTurbidityAdjust(pTubeInfo, m_pTurbidityPara, m_pAlgorithm);
		}

		if(nFlag >= 3)
		{
			// �˲�
			if (m_bExpRunning)
				m_pAmplifyData->FluDigitalFilter(pTubeInfo,&m_arrayFilter,m_pAlgorithm); // ����ȫ���˲�
			else
				m_pAmplifyData->FluDigitalFilter(pTubeInfo,&m_arrayFilter,m_pAlgorithm, 2); // ֻ���о�ֵ�˲�
		}

		if(nFlag >= 1 && !m_bExpRunning)
		{
			// ��һ����DeltaRn
			m_pAmplifyData->FluNormalizeAndDeltaRn(pTubeInfo,m_pAnalysisPara,m_pAlgorithm, m_pTurbidityPara);
		}
	}

	return TRUE;
}

// �����۽��ӫ������
BOOL CExperimentData::ProcessMeltFluData()
{
	int iCrvPtNum = GetMeltDataNum();
	if(iCrvPtNum < 1)
	{
		return FALSE;
	}

	int iTubeNum = GetTubeCount();
	if(iTubeNum < 1)
	{
		return FALSE;
	}

	double* pDX = new double[iCrvPtNum];
	if(NULL == pDX)
	{
		return FALSE;
	}
	double* pDY = new double[iCrvPtNum];
	if(NULL == pDY)
	{
		delete [] pDX;
		return FALSE;
	}
	double* pDReturn = new double[iCrvPtNum];
	if(NULL == pDReturn)
	{
		delete [] pDX;
		delete [] pDY;
		return FALSE;
	}
	double* pDCrossData = new double[iCrvPtNum];
	if(NULL == pDCrossData)
	{
		delete [] pDX;
		delete [] pDY;
		delete [] pDReturn;
		return FALSE;
	}
	
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	tagAnaMeltCurvePara *pAnaMeltCrvParam = NULL;  

	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			continue;
		}

		for(int i = 0;i < m_iChannelCount;++i)
		{
			pAnaMeltCrvParam = m_pAnalysisPara->GetMeltCurveParaBy(i);
			if(NULL == pAnaMeltCrvParam)
			{
				continue;
			}

			pDyeTargetInfo = pTubeInfo->GetTargetInfo(i);
			if(NULL == pDyeTargetInfo)
			{
				continue;;
			}

#ifdef _DEBUG
			if(0 == i && 41 == j)
			{
				int ll = 0;
				++ll;
			}
#endif

			GetMeltCycleX(j + 1,iCrvPtNum,pDX);
			GetMeltChannelFluY(j + 1,FLU_ORIGINAL,i,iCrvPtNum,pDY);

			//ȥ������
			if(NULL != m_ppFMeltCrosstalk)
			{
				for(int k = 0;k < m_iChannelCount;++k)
				{
					if(i == k)
					{
						continue;
					}

					GetMeltChannelFluY(j + 1,FLU_ORIGINAL,k,iCrvPtNum,pDCrossData);
					for(int t = 0;t < iCrvPtNum;++t)
					{
						pDY[t] -= pDCrossData[t] * m_ppFMeltCrosstalk[k][i];
						if(pDY[t] < 0)
						{
							pDY[t] = 0;
						}
					}
				}
			}

			//��ǰ�˲�
			m_pAlgorithm->DigitalFilter(pDY,pDReturn,iCrvPtNum,CDataAlgorithmPCR::FILTERTYPE_MEDIAN,5);
			memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);
			for(int m = 0;m < 3;++m)
			{
				m_pAlgorithm->KdsptForwardMBackN(iCrvPtNum,pDY,pDReturn,2,2);
				memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);	
			}

			m_pMeltData->SetChannelFluY(j + 1,FLU_FILTER,i,iCrvPtNum,pDY);

			//����һ�׸�����
			m_pAlgorithm->CalMeltFirstDerivative(pDX,pDY,iCrvPtNum,TRUE,1,pDReturn);
			memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);
			for(int k = 0;k < iCrvPtNum;++k)
			{
				pDY[k] *= -1;
			}

			m_pMeltData->SetChannelFluY(j + 1,MELT_FSTNEGRECIPROCAL,i,iCrvPtNum,pDY);

			//�󵼺��˲�
			m_pAlgorithm->DigitalFilter(pDY,pDReturn,iCrvPtNum,CDataAlgorithmPCR::FILTERTYPE_MEDIAN,5);
			memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);
			for(int m = 0;m < 3;++m)
			{
				m_pAlgorithm->KdsptForwardMBackN(iCrvPtNum,pDY,pDReturn,2,2);
				memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);	
			}

			////���洦��
			//if(!pRunInfo->m_meltAnaParam.m_aryMeltAnaParam[i].m_bUseAutoGain)
			//{
			//	for(int k = 0;k < nCrvPtNum;++k)
			//	{
			//		pDY[k] *= pRunInfo->m_meltAnaParam.m_aryMeltAnaParam[i].m_dHandSetGain;
			//	}
			//}

			m_pMeltData->SetChannelFluY(j + 1,MELT_GAINDATA,i,iCrvPtNum,pDY);

			//����Tmֵ
			double dCrestValue = -1;
			int crestIndex = 0;
			double dMeltMinPeakValue = pAnaMeltCrvParam->dThreshold;
			m_pAlgorithm->NewAutoFindCrest(1,iCrvPtNum,pDX,pDY,&dCrestValue,&crestIndex,0.2,dMeltMinPeakValue);

			if(-1 != dCrestValue)
			{
				if(pDY[crestIndex] < 0)
				{
					for(int tt = 0;tt < 1;++tt)
					{
						dCrestValue = -1;
					}
				}
			}

			//����ʽ��Ͻ�һ������Tmֵ
			if (iCrvPtNum >= 9)
			{
				for (int kn = 0;kn < 1;++kn)
				{
					if (-1 == dCrestValue) 
					{
						continue;
					}
					bool bfind = false;
					int nbegin = 1,nend = 1;
					for (int t = 0;t < iCrvPtNum;++t)
					{
						if (pDX[t] == dCrestValue)
						{
							nbegin = t -4;
							nend = t + 4;
							bfind = true;
							break;
						}
					}

					if(bfind)
					{
						int nCoefNum = 4; 
						int nDataCount = 9;
						double *pdCoef  = new double[nCoefNum];
						memset(pdCoef,0,sizeof(double) * nCoefNum);
						double *pdXData = new double[nDataCount];
						double *pdYData = new double[nDataCount];
						memset(pdXData,0,sizeof(double) * nDataCount);
						memset(pdYData,0,sizeof(double) * nDataCount);

						if (nbegin < 0) 
						{
							nbegin = 0;
						}

						if(nbegin + nDataCount > iCrvPtNum) 
						{
							nbegin = iCrvPtNum - nDataCount;
						}

						for(int k = nbegin;k < nbegin + nDataCount;++k)
						{
							pdXData[k - nbegin] = pDX[k];
							pdYData[k - nbegin] = pDY[k];
						}

						//3�ζ���ʽ��ϣ��岹�����ֵλ��
						CFitting fit;
						fit.SolutionCLEG(pdXData,pdYData,pdCoef,nDataCount,nCoefNum);
						int nCBnum = 0;
						nCBnum = (pdXData[nDataCount - 1] - pdXData[0]) / 0.01 + 10;
						//�岹����Դ��ֵ
						double *pdCbX = new double[nCBnum];
						memset(pdCbX,0,sizeof(double) * nCBnum);
						double *pdCbY = new double[nCBnum];
						memset(pdCbY,0,sizeof(double) * nCBnum);
						double *pdTemp = new double[nCBnum];
						memset(pdTemp,0,sizeof(double) * nCBnum);
						for (int f = 0; f < nCBnum;++f)
						{
							pdCbX[f] = pdXData[0] + 0.01*f;
						}
						for (int f = 0; f < nCBnum;++f)
						{
							double dYData = 0;
							for(int i = nCoefNum - 1;i >= 0;--i)
							{
								dYData += pdCoef[i] * pow(pdCbX[f],i);
							}
							pdCbY[f] = dYData;
						}

						m_pAlgorithm->SortDouble(nCBnum,pdCbY,pdTemp);

						for (int t = 0;t < nCBnum;++t)
						{
							if (pdCbY[t] == pdTemp[nCBnum - 1])
							{
								dCrestValue = pdCbX[t];
								break;
							}
						}

						delete[] pdCoef;
						delete[] pdXData;
						delete[] pdYData;
						delete[] pdCbX;
						delete[] pdCbY;
						delete[] pdTemp;
					}
				}
			}	

			pDyeTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_TM,dCrestValue);
		}
	}

	delete [] pDX;
	delete [] pDY;
	delete [] pDReturn;
	delete [] pDCrossData;

	return TRUE;
}

//ʵ������ʱ�������µ��������ݣ����ض����ݵ������д���
BOOL CExperimentData::MonitorProcessMeltFluData(int iPtCount)
{
	if(iPtCount < 1 || iPtCount > GetMeltDataNum())
	{
		return FALSE;
	}

	int iCrvPtNum = iPtCount;
	int iTubeNum = GetTubeCount();
	if(iTubeNum < 1)
	{
		return FALSE;
	}

	double* pDX = new double[iCrvPtNum];
	if(NULL == pDX)
	{
		return FALSE;
	}
	double* pDY = new double[iCrvPtNum];
	if(NULL == pDY)
	{
		delete [] pDX;
		return FALSE;
	}
	double* pDReturn = new double[iCrvPtNum];
	if(NULL == pDReturn)
	{
		delete [] pDX;
		delete [] pDY;
		return FALSE;
	}
	double* pDCrossData = new double[iCrvPtNum];
	if(NULL == pDCrossData)
	{
		delete [] pDX;
		delete [] pDY;
		delete [] pDReturn;
		return FALSE;
	}

	CTubeInfo* pTubeInfo = NULL;
	tagAnaMeltCurvePara *pAnaMeltCrvParam = NULL;  

	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			continue;
		}

		for(int i = 0;i < m_iChannelCount;++i)
		{
			pAnaMeltCrvParam = m_pAnalysisPara->GetMeltCurveParaBy(i);
			if(NULL == pAnaMeltCrvParam)
			{
				continue;
			}

			GetMeltCycleX(j + 1,iCrvPtNum,pDX);
			GetMeltChannelFluY(j + 1,FLU_ORIGINAL,i,iCrvPtNum,pDY);

			//ȥ������
			if(NULL != m_ppFMeltCrosstalk)
			{
				for(int k = 0;k < m_iChannelCount;++k)
				{
					if(i == k)
					{
						continue;
					}

					GetMeltChannelFluY(j + 1,FLU_ORIGINAL,k,iCrvPtNum,pDCrossData);
					for(int t = 0;t < iCrvPtNum;++t)
					{
						pDY[t] -= pDCrossData[t] * m_ppFMeltCrosstalk[k][i];
						if(pDY[t] < 0)
						{
							pDY[t] = 0;
						}
					}
				}
			}

			//��ǰ�˲�
			m_pAlgorithm->DigitalFilter(pDY,pDReturn,iCrvPtNum,CDataAlgorithmPCR::FILTERTYPE_MEDIAN,5);
			memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);
			for(int m = 0;m < 3;++m)
			{
				m_pAlgorithm->KdsptForwardMBackN(iCrvPtNum,pDY,pDReturn,2,2);
				memcpy(pDY,pDReturn,sizeof(double) * iCrvPtNum);	
			}

			m_pMeltData->SetChannelFluY(j + 1,FLU_FILTER,i,iCrvPtNum,pDY);
		}
	}

	delete [] pDX;
	delete [] pDY;
	delete [] pDReturn;
	delete [] pDCrossData;

	return TRUE;
}

// ���ݸ����ķ�������Ct
void CExperimentData::GenerateCt(BOOL bCalcDefThre,eMethodCalculateCt nMethod, eDataType nDataType)
{
	if (m_bHasZeroValues)
		return;

	//���ļ�����£�����Ҫ���¼���Ĭ����ֵ
	if(bCalcDefThre)
	{
		//GenerateAutoSplineThresholdForChannel(nDataType,bCalcDefThre);
		GenerateAutoSplineThresholdForChannel(FLU_NORMALIZATE,bCalcDefThre);
		GenerateAutoSplineThresholdForChannel(FLU_DELTARN,bCalcDefThre);
	}

	int iCalibChannelIndex = GetCalibrateChannel() - 1;

	// ����Ct
	int iTubeCount = m_pPlateInfo->GetTubeCount();
	for(int iTubeIndex = 0;iTubeIndex < iTubeCount; ++iTubeIndex)
	{
		CTubeInfo* pTubeInfo = m_pPlateInfo->GetTubeInfo(iTubeIndex);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		if(!pTubeInfo->HasUsed())
		{
			continue;
		}

		int iTargetCount = pTubeInfo->GetTargetCount();
		for(int i = 0;i < iTargetCount;++i)
		{
			CDyeTargetInfo* pTargetInfo = pTubeInfo->GetTargetInfo(i);
			if(NULL == pTargetInfo || !pTargetInfo->GetBeSelected())
			{
				continue;
			}

			//У׼ͨ��������Ctֵ�������Խ��
			if(i == iCalibChannelIndex)
			{
				pTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_CT,0);
				pTargetInfo->SetAnalyseResult(EAR_UNHANDLED);
				continue;
			}

			BOOL bNewPara = FALSE;
			tagAnaAmplifyPara* pAmplifyPara = m_pAnalysisPara->GetAmplifyParaBy(pTargetInfo->GetTargetName(), pTargetInfo->GetDyeName(), pTargetInfo->GetChannel());
			
			//ʹ��ͨ����������20220418
			pAmplifyPara = NULL;
			//ʹ�û�����δ�ҵ���Ӧ��������ʱ��ʹ��ͨ�����ٴλ�ȡ������������ֹ�ֶ���ֵ����Ϣ��ʧ��������20220418��������
			if(NULL == pAmplifyPara)
			{
				pAmplifyPara = m_pAnalysisPara->GetAmplifyParaBy(pTargetInfo->GetChannel() - 1);
			}
			
			if (pAmplifyPara == NULL)
			{
				bNewPara = TRUE;
				pAmplifyPara = new tagAnaAmplifyPara;
				pAmplifyPara->strDye = pTargetInfo->GetDyeName();
				pAmplifyPara->strTarget = pTargetInfo->GetTargetName();
				pAmplifyPara->iChannelNo = pTargetInfo->GetChannel();
			}

			double dCt = m_pAmplifyData->GenerateCtByTubeID(nDataType, pTubeInfo->GetTubeID(), nMethod, pAmplifyPara, m_pAlgorithm);
			pTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_CT, dCt);

			int iDataNum = m_pAmplifyData->GetDataNum();
			BOOL bAbnormalCt = (dCt < 1 || dCt >= iDataNum);

			//�ж���������Ϣ
			if(!bNewPara)
			{
				EANALYSERESULT eAnaRst = EAR_UNHANDLED;
				if(bAbnormalCt || dCt > pAmplifyPara->dUpperThreshold || dCt < pAmplifyPara->dLowerThreshold)
				{
					eAnaRst = EAR_NEGATIVE;
				}
				else 
				{
					eAnaRst = EAR_POSITVE;
				}
				pTargetInfo->SetAnalyseResult(eAnaRst);
			}

			if (bNewPara)
			{
				delete pAmplifyPara;
			}
		}
	}

	return;
}

// ���ݸ����ķ�������Ct
void CExperimentData::GenerateOneChannelCt(eMethodCalculateCt nMethod, eDataType nDataType,int iChanId)
{
	if (m_bHasZeroValues)
		return;

	int iCalibChannelIndex = GetCalibrateChannel();
	//У׼ͨ��������Ctֵ�������Խ��
	if(iChanId == iCalibChannelIndex)
	{
		return;
	}

	// ����Ct
	int iTubeCount = m_pPlateInfo->GetTubeCount();
	for(int iTubeIndex = 0;iTubeIndex < iTubeCount;++iTubeIndex)
	{
		CTubeInfo* pTubeInfo = m_pPlateInfo->GetTubeInfo(iTubeIndex);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		if(!pTubeInfo->HasUsed())
		{
			continue;
		}

		int iTargetCount = pTubeInfo->GetTargetCount();
		for(int i = 0;i < iTargetCount;++i)
		{
			CDyeTargetInfo* pTargetInfo = pTubeInfo->GetTargetInfo(i);
			if(NULL == pTargetInfo)
			{
				continue;
			}

			if(iChanId != pTargetInfo->GetChannel())
			{
				continue;
			}

#ifdef _DEBUG
			if(77 == iTubeIndex)
			{
				int kk = 0;
				++kk;
			}
#endif

			BOOL bNewPara = FALSE;
			tagAnaAmplifyPara* pAmplifyPara = m_pAnalysisPara->GetAmplifyParaBy(pTargetInfo->GetTargetName(), pTargetInfo->GetDyeName(), pTargetInfo->GetChannel());
			
			//ʹ��ͨ����������20220415
			pAmplifyPara = NULL;
			//ʹ�û�����δ�ҵ���Ӧ��������ʱ��ʹ��ͨ�����ٴλ�ȡ������������ֹ�ֶ���ֵ����Ϣ��ʧ��������20220401��������
			if(NULL == pAmplifyPara)
			{
				pAmplifyPara = m_pAnalysisPara->GetAmplifyParaBy(iChanId - 1);
			}

			if (pAmplifyPara == NULL)
			{
				bNewPara = TRUE;
				pAmplifyPara = new tagAnaAmplifyPara;
				pAmplifyPara->strDye = pTargetInfo->GetDyeName();
				pAmplifyPara->strTarget = pTargetInfo->GetTargetName();
				pAmplifyPara->iChannelNo = pTargetInfo->GetChannel();
			}
			double dCt = m_pAmplifyData->GenerateCtByTubeID(nDataType, pTubeInfo->GetTubeID(), nMethod, pAmplifyPara, m_pAlgorithm);
			pTargetInfo->SetCalcValue(CDyeTargetInfo::VALUE_CT, dCt);

			int iDataNum = m_pAmplifyData->GetDataNum();
			BOOL bAbnormalCt = (dCt < 1 || dCt >= iDataNum);

			//�ж���������Ϣ
			if(!bNewPara)
			{
				EANALYSERESULT eAnaRst = EAR_UNHANDLED;
				if(bAbnormalCt || dCt > pAmplifyPara->dUpperThreshold || dCt < pAmplifyPara->dLowerThreshold)
				{
					eAnaRst = EAR_NEGATIVE;
				}
				else 
				{
					eAnaRst = EAR_POSITVE;
				}
				pTargetInfo->SetAnalyseResult(eAnaRst);
			}

			if (bNewPara)
			{
				delete pAmplifyPara;
			}
		}
	}

	return;
}

int CExperimentData::GenerateAutoSplineThresholdForChannel(eDataType nDataType,BOOL bAutoThreCopyToHandThre /*= FALSE*/)
{
	if(m_iChannelCount < 1)
	{
		return -1;
	}

	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	tagAnaAmplifyPara* pAmplifyPara = NULL;

	int iCtCount = 0;
	double dThresholdSum = 0;
 	int iTubeCount = m_pPlateInfo->GetTubeCount();
	double dMaxYSumPC = 0;
	double dMaxFluoValue = 0;
	int	iCtCountPC = 0;

	for(int j = 0;j < m_iChannelCount;++j)
	{
		pAmplifyPara = m_pAnalysisPara->GetAmplifyParaBy(j);
		if(NULL == pAmplifyPara /*|| pAmplifyPara->nThresholdType != SPLINE_THRESHOLD_AUTO*/)
			continue;

		iCtCount = 0;
		dThresholdSum = 0;
		dMaxYSumPC = 0;
		iCtCountPC = 0;
		dMaxFluoValue = 0;
		for(int iTubeIndex = 0;iTubeIndex < iTubeCount;++iTubeIndex)
		{
			pTubeInfo = m_pPlateInfo->GetTubeInfo(iTubeIndex);
			if(NULL == pTubeInfo || !pTubeInfo->HasUsed())
				continue;

			pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
			if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
				continue;

			double dMaxValue = m_pAmplifyData->GetChannelMaxValueY(nDataType,pTubeInfo->GetTubeID(),pDyeTargetInfo->GetChannel());
			if (dMaxValue > dMaxFluoValue)
				dMaxFluoValue = dMaxValue;
			double dThreshold = m_pAmplifyData->GenerateAutoSplineThreshold(nDataType,pTubeInfo->GetTubeID(),pDyeTargetInfo->GetChannel(),m_pAlgorithm);
			if(dThreshold > 0)
			{
				dThresholdSum += dThreshold;
				++iCtCount;
				if (pTubeInfo->GetSampleType() == SAMPLE_TYPE_POSITIVE)
				{
					dMaxYSumPC += dMaxValue;
					iCtCountPC ++;
				}
			}
		}

		if (pAmplifyPara->nThresholdType == SPLINE_THRESHOLD_AUTO)// �Զ���ֵʱ
 		{			
			if(iCtCount > 0)
			{
				double dThre = CPublicFun::GetDoublePrecision(dThresholdSum / iCtCount);
				if(FLU_NORMALIZATE == nDataType)
				{
					pAmplifyPara->dCalcNormalizeThreshold = dThre;
				}
				else if(FLU_DELTARN == nDataType)
				{
					pAmplifyPara->dCalcDeltaRnThreshold = dThre;
				}
			}
			else
			{
				if(FLU_NORMALIZATE == nDataType)
				{
					pAmplifyPara->dCalcNormalizeThreshold = 0.1;
				}
				else if(FLU_DELTARN == nDataType)
				{
					pAmplifyPara->dCalcDeltaRnThreshold = 1000;
				}
			}
		}
		else if (iCtCountPC > 0)
		{
			if(FLU_NORMALIZATE == nDataType)
			{
				double dThre = pAmplifyPara->dNormalizeThreshold * (dMaxYSumPC/iCtCountPC);
				pAmplifyPara->dCalcNormalizeThreshold = CPublicFun::GetDoublePrecision(dThre);
			}
			else if(FLU_DELTARN == nDataType)
			{
				double dThre = pAmplifyPara->dDeltaRnThreshold * (dMaxYSumPC/iCtCountPC);
				pAmplifyPara->dCalcDeltaRnThreshold = CPublicFun::GetDoublePrecision(dThre);
			}
		}
		else
		{
			if(FLU_NORMALIZATE == nDataType)
			{
				double dThre = pAmplifyPara->dNormalizeThreshold * dMaxFluoValue;
				pAmplifyPara->dCalcNormalizeThreshold = CPublicFun::GetDoublePrecision(dThre);
			}
			else if(FLU_DELTARN == nDataType)
			{
				double dThre = pAmplifyPara->dDeltaRnThreshold * dMaxFluoValue;
				pAmplifyPara->dCalcDeltaRnThreshold = CPublicFun::GetDoublePrecision(dThre);
			}
		}

		if (pAmplifyPara->dCalcDeltaRnThreshold < 200)
			pAmplifyPara->dCalcDeltaRnThreshold = 200;

		//����Ĭ����ֵʹ�á��Ƕ������ݡ����㣬�����Ľ����ȡ����
		if(FLU_NORMALIZATE == nDataType && m_bNormalizeSelLog)
		{
			pAmplifyPara->dCalcNormalizeThreshold = log10(pAmplifyPara->dCalcNormalizeThreshold);
		}
		if(FLU_DELTARN == nDataType && m_bDeltaRnSelLog)
		{
			pAmplifyPara->dCalcDeltaRnThreshold = log10(pAmplifyPara->dCalcDeltaRnThreshold);
		}
	}

	return 0;
}

// ����У׼����
void CExperimentData::SetCalibrateMethod(eCalibrateMethod nMethod)
{
	m_nCalibrateMethod = nMethod;
	if (nMethod == CALIBRATE_NONE)
	{
		SAFE_DELETE(m_pTurbidityPara);
	}

	if(nMethod == CALIBRATE_TURBIDITY && m_pTurbidityPara == NULL)
	{
		CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
		m_pTurbidityPara = new CCalibrateParaTurbidity;
		m_pTurbidityPara->ReadPara(pPublicInfo->GetIniFilePath(CPublicInfo::INI_FILE_CALIBRATE));
	}
}

BOOL CExperimentData::GetBeUseTurbidityCalibration()
{
	return CALIBRATE_TURBIDITY == m_nCalibrateMethod;
}

BOOL CExperimentData::SaveCalibratePara()
{
	if (m_nCalibrateMethod == CALIBRATE_TURBIDITY)
	{
		CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
		m_pTurbidityPara->SavePara(pPublicInfo->GetIniFilePath(CPublicInfo::INI_FILE_CALIBRATE, FALSE));
	}

	return FALSE;
}

void CExperimentData::SetCalibrateChannel(int iChannel)
{
	if (m_pTurbidityPara != NULL)
	{
		m_pTurbidityPara->SetChannelMOT(iChannel);
	}
}

int CExperimentData::GetCalibrateChannel()
{
	if(NULL != m_pTurbidityPara)
	{
		return m_pTurbidityPara->GetChannelMOT();
	}

	return -1;
}

void CExperimentData::InitAlgorithmPara(CString strIniFile)
{
	m_pAlgorithm->InitPara(strIniFile);
}


// �������ļ��ж�ȡĬ�ϵĴ���ϵ��
BOOL CExperimentData::InitCrosstalkPara(CString strIniFile)
{
	ASSERT(m_iChannelCount != 0);

	TCHAR c1[250] = {0};
	CString strAppName,strKeyName;
	for (int i = 0;i < m_iChannelCount;++i)
	{
		strAppName.Format(_T("Channel(%d)"),i + 1);

		for (int j = 0;j < m_iChannelCount;++j)
		{
			strKeyName.Format(_T("ToChannel%d"),j + 1);
			GetPrivateProfileString(strAppName,strKeyName,_T("0.0"),c1,sizeof(c1),strIniFile);
			m_ppFCrosstalkPara[i][j] = (float)_ttof(c1);
		}
	}

	for(int i = 0;i < m_iChannelCount;++i)
	{
		strAppName.Format(_T("Channel(%d)Melt"),i + 1);

		for (int j = 0;j < m_iChannelCount;++j)
		{
			strKeyName.Format(_T("ToChannel%d"),j + 1);
			GetPrivateProfileString(strAppName,strKeyName,_T("0.0"),c1,sizeof(c1),strIniFile);
			m_ppFMeltCrosstalk[i][j] = (float)_ttof(c1);
		}
	}

	return TRUE;
}

BOOL CExperimentData::InitAmpCrosstalkParam(float** ppFData,int iChanNum)
{
	if(NULL == ppFData)
	{
		return FALSE;
	}

	if(iChanNum != m_iChannelCount)
	{
		return FALSE;
	}

	for(int i = 0;i < m_iChannelCount;++i)
	{
		for (int j = 0;j < m_iChannelCount;++j)
		{
			m_ppFCrosstalkPara[i][j] = ppFData[i][j];
		}
	}

	return TRUE;
}

BOOL CExperimentData::InitMeltCrosstalkParam(float** ppFData,int iChanNum)
{
	if(NULL == ppFData)
	{
		return FALSE;
	}

	if(iChanNum != m_iChannelCount)
	{
		return FALSE;
	}

	for(int i = 0;i < m_iChannelCount;++i)
	{
		for (int j = 0;j < m_iChannelCount;++j)
		{
			m_ppFMeltCrosstalk[i][j] = ppFData[i][j];
		}
	}

	return TRUE;
}

// �������ļ��ж�ȡĬ�ϵ��˲�����
BOOL CExperimentData::InitFilterInfo(CString strIniFile)
{
	DeleteFilterPara();

	int nCount = ::GetPrivateProfileInt(_T("FilterPara"), _T("Count"), 0, strIniFile);

	CString strAppName;
	tagFilterItem* pItem = NULL;
	for (int i=0; i<nCount; i++)
	{
		pItem = new tagFilterItem;

		strAppName.Format(_T("Method(%d)"), i+1);
		pItem->iFilterUser = ::GetPrivateProfileInt(strAppName, _T("User"), 0, strIniFile);
		pItem->nType = (eFilterType)::GetPrivateProfileInt(strAppName, _T("Type"), 1, strIniFile);
		pItem->iTimes = ::GetPrivateProfileInt(strAppName, _T("Times"), 1, strIniFile);
		pItem->iPara1 = ::GetPrivateProfileInt(strAppName, _T("Para1"), 5, strIniFile);
		pItem->iPara2 = ::GetPrivateProfileInt(strAppName, _T("Para2"), 0, strIniFile);

		m_arrayFilter.Add(pItem);
	}

	return TRUE;
}

BOOL CExperimentData::InitFilterInfo(vector<tagFilterItem>& vecFlt)
{
	DeleteFilterPara();

	tagFilterItem* pItem = NULL;
	for(int i = 0;i < vecFlt.size();++i)
	{
		pItem = new tagFilterItem;
		if(NULL == pItem)
		{
			return FALSE;
		}
		*pItem = vecFlt[i];
		m_arrayFilter.Add(pItem);
	}

	return TRUE;
}

// ����ɨ��ͨ����
void CExperimentData::SetChannelCount(int iCount, BOOL bOpenFile)
{
	int iTubeCount = m_pPlateInfo->GetTubeCount();
	ASSERT(iTubeCount > 0);
	// ����Ϣͨ����ʼ��
	for (int i=0; i<iTubeCount; i++)
	{
		CTubeInfo* pTube = m_pPlateInfo->GetTubeInfo(i);
		ASSERT(pTube != NULL);
		if (pTube != NULL && !bOpenFile)
		{
			pTube->InitDyeChannel(iCount);
		}
	}

	// �������ϵ����ʼ��
	if (iCount != m_iChannelCount)
	{
		DeleteCrosstalkPara();
		m_iChannelCount = iCount;

		m_ppFCrosstalkPara = new float*[iCount];
		for (int t= 0; t< iCount;t++)
		{
			m_ppFCrosstalkPara[t] = new float[iCount];
			for (int x=0; x<iCount; x++)
			{
				m_ppFCrosstalkPara[t][x] = 0;
			}
		}	

		m_ppFMeltCrosstalk = new float*[iCount];
		for (int t= 0; t< iCount;t++)
		{
			m_ppFMeltCrosstalk[t] = new float[iCount];
			for (int x=0; x<iCount; x++)
			{
				m_ppFMeltCrosstalk[t][x] = 0;
			}
		}	
	}

	if (m_pAmplifyData == NULL)
	{
		m_pAmplifyData = new CPlateAmplifyTubeData;
	}
	m_pAmplifyData->AllocateMemory(iTubeCount, iCount);

	if (m_pMeltData == NULL)
	{
		m_pMeltData = new CPlateMeltTubeData;
	}
	m_pMeltData->AllocateMemory(iTubeCount, iCount);
}

// ����ɨ��ͨ����
int CExperimentData::SetTubeChannelSelectState(int iChanId,BOOL bSelected)
{
	int iTubeCount = m_pPlateInfo->GetTubeCount();
	if(iTubeCount <= 0)
	{
		return -1;
	}

	CTubeInfo* pTube = NULL;
	for (int i = 0;i < iTubeCount;++i)
	{
		pTube = m_pPlateInfo->GetTubeInfo(i);
		if(NULL == pTube)
		{
			continue;
		}
		
		pTube->SetSampleChannelSelectState(iChanId,bSelected);
	}
	return 0;
}

tagDyeChannelInfo* CExperimentData::GetDyeChannelInfo(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_arrayDye.GetCount())
		return NULL;

	tagDyeChannelInfo* pDye = (tagDyeChannelInfo*)m_arrayDye.GetAt(iIndex);
	return pDye;
}


void CExperimentData::SetSegment(CLinkList<CSegItemInfo>& lnkSegItem)
{
	m_lnkSegment = lnkSegItem;
}

CLinkList<CSegItemInfo>& CExperimentData::GetSegment()
{
	return m_lnkSegment;
}

// ʵ������ǰ�ж����ݲ��������Ƿ����úͺ����������ʼ��ΪĬ��ֵ
BOOL CExperimentData::CheckParaBeforeRun(CString strStartupPath)
{
	// Ϊÿ����Ӧ�ܵ�ʹ��ָ�����
	int iTubeCount = m_pPlateInfo->GetTubeCount();
	for (int i=0; i<iTubeCount; i++)
	{
		CTubeInfo* pTubeInfo = m_pPlateInfo->GetTubeInfo(i);
		if (pTubeInfo == NULL)
			continue;

		pTubeInfo->PerfectTargetDyeInfo(&m_arrayDye);
	}

	//������20220416�������ɣ������������
	//// �жϷ��������Ƿ�����
	//CPtrArray arrayUsedTarget; 
	//m_pPlateInfo->GetUsedTarget(&arrayUsedTarget);

	//for (int i=0; i<arrayUsedTarget.GetCount(); i++)
	//{
	//	tagAnaAmplifyPara* pTempPara = (tagAnaAmplifyPara*)arrayUsedTarget.GetAt(i);
	//	// �ж��Ƿ��Ѿ��ڷ��������ж���
	//	if (m_pAnalysisPara->IsNewAmplifyPara(strStartupPath, pTempPara))
	//	{
	//		arrayUsedTarget.SetAt(i, NULL);
	//	}
	//}

	//for (int x=0; x<arrayUsedTarget.GetCount(); x++)
	//{
	//	tagAnaAmplifyPara* pTempPara = (tagAnaAmplifyPara*)arrayUsedTarget.GetAt(x);
	//	SAFE_DELETE(pTempPara);
	//}
	//arrayUsedTarget.RemoveAll();

	return TRUE;
}

int CExperimentData::GetFirstEndCollectCycleNum()
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	while(NULL != pSegNode)
	{
		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollect())
			{
				return pSegNode->data.m_iCycleNum;
			}
			pStepNode = pStepNode->pNext;
		}
		pSegNode = pSegNode->pNext;
	}

	return 0;
}

int CExperimentData::GetFirstMeltCollectStartAndEndTemperature(double& dTemStart,double& dTemEnd)
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	while(NULL != pSegNode)
	{
		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollectAll())
			{
				if(NULL != pStepNode->pPrevious)
				{
					dTemStart = pStepNode->pPrevious->data.m_fAimTem;
				}
				else if(NULL != pSegNode->pPrevious && pSegNode->pPrevious->data.m_lnkStepItemInfo.GetSize() > 0)
				{
					dTemStart = pSegNode->pPrevious->data.m_lnkStepItemInfo.GetTail()->data.m_fAimTem;
				}
				else
				{
					dTemStart = 40;	//������������ǳ����һ�����裬��Ĭ����ʼ�¶���40�棬20220715��������
				}
				dTemEnd = pStepNode->data.m_fAimTem;
				return 0;
			}
			pStepNode = pStepNode->pNext;
		}
		pSegNode = pSegNode->pNext;
	}

	dTemStart = 60;
	dTemEnd = 90;
	return -1;
}

tagAnaAmplifyPara* CExperimentData::GetAmplifyParaBy(int nIndex)
{
	if(nIndex < 0)
	{
		return NULL;
	}
	if(NULL == m_pAnalysisPara)
	{
		return NULL;
	}
	if(nIndex >= m_pAnalysisPara->GetAmplifyParaCount())
	{
		return NULL;
	}

	return m_pAnalysisPara->GetAmplifyParaBy(nIndex);
}

tagAnaMeltCurvePara* CExperimentData::GetMeltAnaParamAt(int iIndex)
{
	if(iIndex < 0)
	{
		return NULL;
	}
	if(NULL == m_pAnalysisPara)
	{
		return NULL;
	}
	if(iIndex >= m_pAnalysisPara->GetMeltCurveParaCount())
	{
		return NULL;
	}

	return m_pAnalysisPara->GetMeltCurveParaBy(iIndex);
}

void CExperimentData::SetStatus(eDataStatus nStatus)
{
	if (m_nStatus == STATUS_NEW && (nStatus == STATUS_CHANGE || nStatus == STATUS_PLATE))
	{
		return;
	}

	if (m_nStatus == STATUS_SAVE_DATA && nStatus == STATUS_CHANGE) // ��ʵ������ʱ�������޸Ŀװ��������Ϣ
	{
		ASSERT(FALSE);
		return;
	}

	m_nStatus = nStatus;
}

void CExperimentData::SetMonitorChannelBtSelectState(vector<int> vecData)
{
	m_vecMonitorChanSelState = vecData;
}

vector<int> CExperimentData::GetMonitorChannelBtSelectState()
{
	return m_vecMonitorChanSelState;
}

void CExperimentData::SetDataAnaChannelBtSelectState(vector<int> vecData)
{
	m_vecDataAnaChanSelState = vecData;
}

vector<int> CExperimentData::GetDataAnaChannelBtSelectState()
{
	return m_vecDataAnaChanSelState;
}

void CExperimentData::SetMonitorChannelBtSelectState(int iChanId,int iSelState)
{
	if(iChanId < 0 || iChanId >= m_vecMonitorChanSelState.size())
	{
		return;
	}

	m_vecMonitorChanSelState[iChanId] = iSelState;
	return;
}

int CExperimentData::GetMonitorChannelBtSelectState(int iChanId)
{
	if(iChanId < 0 || iChanId >= m_vecMonitorChanSelState.size())
	{
		return 0;
	}
	return m_vecMonitorChanSelState[iChanId];
}	

void CExperimentData::SetDataAnaChannelBtSelectState(int iChanId,int iSelState)
{
	if(iChanId < 0 || iChanId >= m_vecDataAnaChanSelState.size())
	{
		return;
	}

	m_vecDataAnaChanSelState[iChanId] = iSelState;
	return;
}

int CExperimentData::GetDataAnaChannelBtSelectState(int iChanId)
{
	if(iChanId < 0 || iChanId >= m_vecDataAnaChanSelState.size())
	{
		return 0;
	}
	return m_vecDataAnaChanSelState[iChanId];
}

void CExperimentData::SetMonitorChannelBtAllSelected()
{
	for(int i = 0;i < m_vecMonitorChanSelState.size();++i)
	{
		m_vecMonitorChanSelState[i] = 1;
	}
	return;
}

void CExperimentData::SetDataAnaChannelBtAllSelected()
{
	for(int i = 0;i < m_vecDataAnaChanSelState.size();++i)
	{
		m_vecDataAnaChanSelState[i] = 1;
	}
	return;
}

void CExperimentData::ResetMonitorChannelBtSelectState()
{
	for(int i = 0;i < m_vecMonitorChanSelState.size();++i)
	{
		m_vecMonitorChanSelState[i] = 0;
	}
	return;
}

void CExperimentData::ResetDataAnaChannelBtSelectState()
{
	for(int i = 0;i < m_vecDataAnaChanSelState.size();++i)
	{
		m_vecDataAnaChanSelState[i] = 0;
	}
	return;
}

void CExperimentData::SetChannelDyeName(vector<CString>& vecChanDyeName)
{
	m_vecChanDyeName = vecChanDyeName;
}

void CExperimentData::SetChannelDyeNameIfEmpty(vector<CString>& vecChanDyeName)
{
	int iMinChanCount = min(m_vecChanDyeName.size(),vecChanDyeName.size());
	for(int i = 0;i < iMinChanCount;++i)
	{
		if(m_vecChanDyeName[i].IsEmpty())
		{
			m_vecChanDyeName[i] = vecChanDyeName[i];
		}
	}
	return;
}

vector<CString> CExperimentData::GetChannelDyeName()
{
	return m_vecChanDyeName;
}

BOOL CExperimentData::GetBeValidChannelDyeName()
{
	if(m_vecChanDyeName.size() < 1)
	{
		return FALSE;
	}

	BOOL bFind = FALSE;
	for(int i = 0;i < m_vecChanDyeName.size();++i)
	{
		if(!m_vecChanDyeName[i].IsEmpty())
		{
			bFind = TRUE;
		}
	}

	return bFind;
}

CString CExperimentData::GetOneChannelDyeName(int iChanId)
{
	if(iChanId < 0 || iChanId >= m_iChannelCount || iChanId >= m_vecChanDyeName.size())
	{
		return _T("");
	}

	return m_vecChanDyeName[iChanId];
}

void CExperimentData::SetOneChannelDyeName(int iChanId,CString strChanDyeName)
{
	if(iChanId < 0 || iChanId >= m_iChannelCount || iChanId >= m_vecChanDyeName.size())
	{
		return;
	}

	m_vecChanDyeName[iChanId] = strChanDyeName;

	//���÷���������Ϣ�е�Ⱦ����
	if(NULL == m_pAnalysisPara)
	{
		return;
	}

	tagAnaAmplifyPara* pAnaParam = m_pAnalysisPara->GetAmplifyParaBy(iChanId);
	if(NULL != pAnaParam)
	{
		pAnaParam->strDye = strChanDyeName;
	}

	tagAnaMeltCurvePara* pMeltAnaParam = m_pAnalysisPara->GetMeltCurveParaBy(iChanId);
	if(NULL != pMeltAnaParam)
	{
		pMeltAnaParam->strDye = strChanDyeName;
	}

	return;
}

int CExperimentData::GetChannelDyeNameFromTubeInfo()
{
	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return -1;
	}

	if(m_iChannelCount < 1)
	{
		return -2;
	}

	m_vecChanDyeName.clear();

	CTubeInfo* pTubeInfo = NULL;
	CString strTemp(_T(""));

	for(int j = 0;j < m_iChannelCount;++j)
	{
		m_vecChanDyeName.push_back(_T(""));
		for(int i = 0;i < iTubeCount;++i)
		{
			pTubeInfo = GetTubeInfoAt(i);
			if(NULL == pTubeInfo)
			{
				continue;
			}

			strTemp = pTubeInfo->GetDyeName(j);
			if(!strTemp.IsEmpty())
			{
				m_vecChanDyeName[j] = strTemp;
				break;
			}
		}
	}
	
	return 0;
}

int CExperimentData::GetRound(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

int CExperimentData::GetTubeCount()
{
	if(NULL == m_pPlateInfo)
	{
		return 0;
	}
	return m_pPlateInfo->GetTubeCount();
}

void CExperimentData::SetAnalysisPara(CDataAnalysisPara *pAnaParam)
{
	if(m_pAnalysisPara == pAnaParam)
	{
		return;
	}

	if(NULL != m_pAnalysisPara)
	{
		delete m_pAnalysisPara;
	}
	m_pAnalysisPara = pAnaParam;
	return;
}

int CExperimentData::SetExpFileSaveDirectory(CString strDir)
{
	if(NULL == m_pExpInfo)
	{
		return -1;
	}

	m_pExpInfo->SetFilePath(strDir);
	return 0;
}

void CExperimentData::CreateBackupFilePath(CString strStarupPath)
{
	m_strBackupFilePath = strStarupPath + _T("Data");
	if(!PathFileExists(m_strBackupFilePath))
	{
		BOOL bRtn = -1;
		bRtn = CreateDirectory(m_strBackupFilePath,NULL);
	}

	CString strBackupFile = m_pExpInfo->CreateExperimentName();
	m_strBackupFileCreateTime = strBackupFile;
	m_strBackupFilePath += _T("\\") + strBackupFile;
}

//��������������Ϣ
//��������������Ϣ
//lzh:��ȡexcelģ��
int CExperimentData::InputSampleSetInfo(vector<eSampleType>* pVecSamType /*= NULL*/)
{
	if (NULL != pVecSamType && pVecSamType->size() < 1)
	{
		return -1;
	}

	CString strDefaultName = CPublicInfo::GetInstance()->GetStartupPath() + _T("Sample ID Template\\");
	CFileDialog dlgFile(TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Excel File(*.xlsx)|*.xlsx|Excel 97-2003 File(*.xls)|*.xls||"), NULL);
	dlgFile.m_ofn.lpstrInitialDir = strDefaultName;
	if (IDCANCEL == dlgFile.DoModal())
	{
		return 1;
	}

	CString strFilename = dlgFile.GetPathName();
	CString strExt = dlgFile.GetFileExt();

	Book* book = NULL;
	if (strExt == _T("xls"))
	{
		book = xlCreateBook();
	}
	else
	{
		book = xlCreateXMLBook();
	}

	if (NULL == book)
	{
		AfxMessageBox(_T("Fail to create excel book!"));
		return -2;
	}
	book->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe"));

	if (!book->load(strFilename))
	{
		AfxMessageBox(_T("Fail to open excel file!"));
		book->release();
		return -3;
	}

	CLanguageInfo* pLanguageInfo = CLanguageInfo::GetInstance();

	Sheet* sheet = book->getSheet(0);
	if (NULL == sheet)
	{
		AfxMessageBox(_T("Fail to get sheet!"));
		book->release();
		return -4;
	}

	CString strRead = sheet->readStr(0, 0);
	if (strRead != pLanguageInfo->GetText(328, LANGUAGE_CHS) && strRead != pLanguageInfo->GetText(328, LANGUAGE_ENG))
	{
		book->release();
		return -5;
	}


	BOOL bPS96Instru = GetBePS96ExperFile();

	int iPatientCol = -1;
	int iPatientIDCol = -1;
	int iPatientGenderCol = -1;
	int iPatientAgeCol = -1;
	int iSamlingDateCol = -1; // ��������
	int iRemarksCol = -1;
	int iTesterCol = -1;
	int iReviewerCol = -1;

	for (int iCol=5; iCol<13; iCol++)
	{
		strRead = sheet->readStr(1, iCol); 
		if(strRead == pLanguageInfo->GetText(66,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(66,LANGUAGE_ENG))// ��������
		{
			iPatientCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(418,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(418,LANGUAGE_ENG))
		{
			iPatientIDCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(67,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(67,LANGUAGE_ENG))
		{
			iPatientGenderCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(68,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(68,LANGUAGE_ENG))
		{
			iPatientAgeCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(69,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(69,LANGUAGE_ENG))
		{
			iSamlingDateCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(514,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(514,LANGUAGE_ENG))
		{
			iRemarksCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(410,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(410,LANGUAGE_ENG))
		{
			iTesterCol = iCol;
		}
		else if(strRead == pLanguageInfo->GetText(411,LANGUAGE_CHS) || strRead == pLanguageInfo->GetText(411,LANGUAGE_ENG))
		{
			iReviewerCol = iCol;
		}
	}

	int iFirstRow = sheet->firstRow();	//��һ��
	int iLastRow = sheet->lastRow();	//�����ַ���Ϣ�����һ�е���һ�У���Ӧ�հ��У�
	int iFirstCol = sheet->firstCol();
	int iLastCol = sheet->lastCol();
	int iTubeCount = GetTubeCount();

	int iTemp = 0;
	CTubeInfo* pTubeInfo = NULL;
	CString strSampleType = _T(""),strTubeLabel = _T(""),strSampleID = _T(""),strTemp(_T(""));
	CPublicInfo* pPublic = CPublicInfo::GetInstance();

	if(NULL != pVecSamType && iLastRow - iFirstRow - 2 > pVecSamType->size())
	{
		iLastRow = iFirstRow + 2 + pVecSamType->size();
	}

	//PS96����������Ϣʱ����Ҫ�ж��¡��ļ��е��������ͺͿװ������õ�������Ϣ�Ƿ�һ�£���һ�±�����
	if(NULL != pVecSamType)
	{
		if(iLastRow - iFirstRow - 2 < pVecSamType->size())
		{
			book->release();
			return -6;
		}

		int iTubeId = 0;
		for(int i = iFirstRow + 2;i < iLastRow;++i)
		{
			strTubeLabel = sheet->readStr(i,2);
			strSampleType = sheet->readStr(i,3);

			if(strTubeLabel.IsEmpty())
			{
				book->release();
				return -7;
			}

			pTubeInfo = m_pPlateInfo->GetTubeInfoByLabel(strTubeLabel);
			if(NULL == pTubeInfo)
			{
				book->release();
				return -8;
			}

			iTubeId = pTubeInfo->GetTubeID();
			//lzh
			if (iTubeId < 1)// || iTubeId > pVecSamType->size())
			{
				book->release();
				return -9;
			}

			if ((*pVecSamType)[iTubeId - 1] != pPublic->TextToSampleType(strSampleType))
			{
				book->release();
				return -10;
			}
		}
	}

	for(int i = iFirstRow + 2;i < iLastRow;++i)
	{
		strTubeLabel = sheet->readStr(i,2);
		strSampleType = sheet->readStr(i,3);
		strSampleID = sheet->readStr(i,4);
		
		if(strTubeLabel.IsEmpty())
		{
			continue;
		}

		pTubeInfo = m_pPlateInfo->GetTubeInfoByLabel(strTubeLabel);
		if(NULL == pTubeInfo)
		{
			continue;
		}

		pTubeInfo->SetSampleType(pPublic->TextToSampleType(strSampleType));
#ifdef PS96INSTRUMENT
		if(pTubeInfo->GetBeNoneSampleType() && !strSampleID.IsEmpty())
		{
			pTubeInfo->SetSampleType(SAMPLE_TYPE_UNKNOWN);	//������ID�Ķ��Ǵ�������
		}
#endif

		pTubeInfo->SetSampleID(strSampleID);

		if(bPS96Instru)
		{
			if(iPatientCol != -1)
			{
				strTemp = sheet->readStr(i,iPatientCol);
				pTubeInfo->SetPatientName(strTemp);
			}

			if(iPatientIDCol != -1)
			{
				strTemp = sheet->readStr(i,iPatientIDCol);
				pTubeInfo->SetPaitentID(strTemp);
			}

			if(iPatientGenderCol != -1)
			{
				strTemp = sheet->readStr(i, iPatientGenderCol);
				pTubeInfo->SetSexuality(strTemp);
			}

			if(iPatientAgeCol != -1)
			{
				iTemp = sheet->readNum(i, iPatientAgeCol);
				pTubeInfo->SetPaitentAge(iTemp);
			}

			if(iSamlingDateCol != -1)
			{
				strTemp.Empty();
				if(sheet->isDate(i, iSamlingDateCol))
				{
					double db = sheet->readNum(i,iSamlingDateCol);
					int iY = 0,iM = 0,iD = 0;
					if(book->dateUnpack(db,&iY,&iM,&iD))
					{
						strTemp.Format(_T("%d-%d-%d"),iY,iM,iD);
					}
				}
				else
				{
					strTemp = sheet->readStr(i,iSamlingDateCol);
				}

				pTubeInfo->SetCollectDateYMD(strTemp);
			}

			if(iRemarksCol != -1)
			{
				strTemp = sheet->readStr(i, iRemarksCol);
				pTubeInfo->SetComments(strTemp);
			}

			if(iTesterCol != -1)
			{
				m_strTester = sheet->readStr(i, iTesterCol);
				pTubeInfo->SetProposer(m_strTester); // �ͼ�ҽ��
			}

			if(iReviewerCol != -1)
			{
				m_strReviewer = sheet->readStr(i, iReviewerCol);
				pTubeInfo->SetApplyDate(m_strReviewer);
			}
		}
	}

	book->release();

	m_nStatus = STATUS_CHANGE;

	return 0;
}

// ��������������Ϣ
BOOL CExperimentData::ExportSampleSetInfo(int iFlag,int iIdVersion /*= 0*/)
{
	CString strDefault;
	strDefault.Format(_T("%s_Sample.xlsx"), m_pExpInfo->GetExpName());

	CFileDialog dlgFile(FALSE, strDefault, NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Excel File(*.xlsx)|*.xlsx|Excel 97-2003 File(*.xls)|*.xls||"),NULL);
	if(dlgFile.DoModal() == IDCANCEL) 
		return FALSE;

	CString strFilename = dlgFile.GetFileName();
	CString strPathName = dlgFile.GetPathName();
	CString strExt = dlgFile.GetFileExt();

	Book* book;
	if(strExt == _T("xls"))
	{
		book = xlCreateBook();
	}
	else
	{
		book = xlCreateXMLBook();
	}
	book->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe"));
	if(!book)
	{
		AfxMessageBox(_T("Fail to create excel file!"));
		return FALSE;
	}

	CLanguageInfo* pLanguageInfo = CLanguageInfo::GetInstance();
	
	BOOL bCanYoung = (1 == iIdVersion);

	Sheet* sheet = book->addSheet(pLanguageInfo->GetText(328));
	if(NULL == sheet)
	{
		return FALSE;
	}

	sheet->writeStr(0,0,pLanguageInfo->GetText(328));
	sheet->setCol(0,0,16);	//width�����ؿ���1:9

	// ���������
	int iRow = 1,iCol = 0;
	sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(111));
	sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(76));
	sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(47));
	sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(64));

	if(bCanYoung)
	{
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(66));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(67));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(68));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(69));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(418));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(514));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(410));
		sheet->writeStr(iRow,++iCol,pLanguageInfo->GetText(411));
	}

	sheet->setCol(1,1,7);
	sheet->setCol(3,10,13);	

	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();

	++iRow;
	tagPlateModeInfo* pPlateInfo = m_pPlateInfo->GetPlateInfo();
	int iCount = 0,iAge = -1;
	CString strTemp(_T(""));

	if(0 == iFlag)
	{
		for(int i = 0;i < pPlateInfo->iVertCount;++i)
		{
			for(int j = 0;j < pPlateInfo->iHoriCount;++j)
			{
				int iTubeIndex = i * pPlateInfo->iHoriCount + j;
				CTubeInfo* pTube = m_pPlateInfo->GetTubeInfo(iTubeIndex);
				if (NULL == pTube)
				{
					continue;
				}
					
				strTemp.Format(_T("%d"),++iCount);
			
				iCol = 0;
				sheet->writeStr(iRow,++iCol,strTemp);
				{
					int displayCols = GetColCount();
					int displayRows = GetRowCount();
					if (displayRows == 16 && displayCols == 24)
					{
						displayCols = 12;
					}
					sheet->writeStr(iRow,++iCol,pTube->GetTubeLabel(displayCols));
				}
				sheet->writeStr(iRow,++iCol,pPublicInfo->GetSampleTypeAbbrText(pTube->GetSampleType()));
				sheet->writeStr(iRow,++iCol,pTube->GetSampleID());
				
				if(bCanYoung)
				{
					sheet->writeStr(iRow,++iCol,pTube->GetPatientName());
					sheet->writeStr(iRow,++iCol,pTube->GetSexualityString());
					iAge = pTube->GetPaitentAge();
					if(iAge <= 0)
					{
						++iCol;
					}
					else
					{
						sheet->writeNum(iRow,++iCol,iAge);
					}
					
					sheet->writeStr(iRow,++iCol,pTube->GetCollectYMD());
					sheet->writeStr(iRow,++iCol, pTube->GetPatientID());
					sheet->writeStr(iRow,++iCol, pTube->GetComments());
					sheet->writeStr(iRow,++iCol, pTube->GetProposer());
					sheet->writeStr(iRow,++iCol, pTube->GetApplyDate());
				}

				++iRow;
			}
		}
	}
	else
	{
		for(int i = 0;i < pPlateInfo->iHoriCount;++i)
		{
			for(int j = 0;j < pPlateInfo->iVertCount;++j)
			{
				int iTubeIndex = j * pPlateInfo->iHoriCount + i;
				CTubeInfo* pTube = m_pPlateInfo->GetTubeInfo(iTubeIndex);
				if(NULL == pTube)
				{
					continue;
				}

				strTemp.Format(_T("%d"),++iCount);

				iCol = 0;
				sheet->writeStr(iRow,++iCol,strTemp);
				{
					int displayCols = GetColCount();
					int displayRows = GetRowCount();
					if (displayRows == 16 && displayCols == 24)
					{
						displayCols = 12;
					}
					sheet->writeStr(iRow,++iCol,pTube->GetTubeLabel(displayCols));
				}
				sheet->writeStr(iRow,++iCol,pPublicInfo->GetSampleTypeAbbrText(pTube->GetSampleType()));
				sheet->writeStr(iRow,++iCol,pTube->GetSampleID());

				if(bCanYoung)
				{
					sheet->writeStr(iRow,++iCol,pTube->GetPatientName());
					sheet->writeStr(iRow,++iCol,pTube->GetSexualityString());
					iAge = pTube->GetPaitentAge();
					if(iAge <= 0)
					{
						++iCol;
					}
					else
					{
						sheet->writeNum(iRow,++iCol,iAge);
					}

					sheet->writeStr(iRow,++iCol,pTube->GetCollectYMD());
					sheet->writeStr(iRow,++iCol, pTube->GetPatientID());
					sheet->writeStr(iRow,++iCol, pTube->GetComments());
					sheet->writeStr(iRow,++iCol, pTube->GetProposer());
					sheet->writeStr(iRow,++iCol, pTube->GetApplyDate());
				}

				++iRow;
			}
		}
	}

	if(book->save(strPathName)) 
	{
		::ShellExecute(NULL,_T("open"),strPathName,NULL,NULL,SW_SHOW); 
	}
	else
	{
		book->errorMessage();
	}

	book->release();

	return TRUE;
}

int CExperimentData::GetFilterCount()
{
	return m_arrayFilter.GetCount();
}

tagFilterItem* CExperimentData::GetFilterBy(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_arrayFilter.GetCount())
	{
		return NULL;
	}

	return (tagFilterItem*)m_arrayFilter.GetAt(iIndex);
}

eSplineThresholdType CExperimentData::GetSplineThresholdType()
{
	tagAnaAmplifyPara* pAmplifyPara = GetAmplifyParaBy(0);
	if(NULL == pAmplifyPara)
	{
		return SPLINE_THRESHOLD_DELTARN;
	}

	return pAmplifyPara->nThresholdType;
}

int CExperimentData::GetRowCount()
{
	if(NULL == m_pPlateInfo)
	{
		return 0;
	}
	tagPlateModeInfo* pPlateModeInfo = m_pPlateInfo->GetPlateInfo();
	if(NULL == pPlateModeInfo)
	{
		return 0;
	}
	return pPlateModeInfo->iVertCount;
}

int CExperimentData::GetColCount()
{
	if(NULL == m_pPlateInfo)
	{
		return 0;
	}
	tagPlateModeInfo* pPlateModeInfo = m_pPlateInfo->GetPlateInfo();
	if(NULL == pPlateModeInfo)
	{
		return 0;
	}
	return pPlateModeInfo->iHoriCount;
}

BOOL CExperimentData::CheckSegmentExistDataCollect()
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	while(NULL != pSegNode)
	{
		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollect() || pStepNode->data.GetBeCollectAll())
			{
				return TRUE;
			}
			pStepNode = pStepNode->pNext;
		}
		pSegNode = pSegNode->pNext;
	}

	return FALSE;
}

BOOL CExperimentData::IsExistAmpSegmentLessThanCycleNum(int iCycleNum)
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	while(NULL != pSegNode)
	{
		if(pSegNode->data.GetBeAmlSeg() && pSegNode->data.m_iCycleNum < iCycleNum)
		{
			return TRUE;
		}
		pSegNode = pSegNode->pNext;
	}

	return FALSE;
}

void CExperimentData::SetBeTemplateFile(BOOL bTemplateFile)
{
	m_bTemplateFile = bTemplateFile;
}

BOOL CExperimentData::GetBeTemplateFile()
{
	return m_bTemplateFile;
}

void CExperimentData::SetIVDFileRstSaveFolder(CString strData)
{
	m_strIVDFileRstSaveFolder = strData;
}

CString CExperimentData::GetIVDFileRstSaveFolder()
{
	return m_strIVDFileRstSaveFolder;
}

void CExperimentData::SetIVDFileRstSaveName(CString strData)
{
	m_strIVDFileRstSaveName = strData;
}

CString CExperimentData::GetIVDFileRstSaveName()
{
	return m_strIVDFileRstSaveName;
}

CString CExperimentData::GetIVDResultFileSavePath()
{
	if(!m_strIVDFileRstSaveFolder.IsEmpty())
	{
		CString strTemp = m_strIVDFileRstSaveFolder.Mid(m_strIVDFileRstSaveFolder.GetLength() - 1);
		if(-1 == strTemp.ReverseFind(_T('\\')) && -1 == strTemp.ReverseFind(_T('/')))
		{
			m_strIVDFileRstSaveFolder += _T("\\");
		}
	}

	if(m_strIVDFileRstSaveName.IsEmpty())
	{
		m_strIVDFileRstSaveName = _T("Result");
	}

	return m_strIVDFileRstSaveFolder + m_strIVDFileRstSaveName + _T(".xlsx");
}

CString CExperimentData::GetIVDResultFileSavePath2()
{
	if(!m_strIVDFileRstSaveFolder.IsEmpty())
	{
		CString strTemp = m_strIVDFileRstSaveFolder.Mid(m_strIVDFileRstSaveFolder.GetLength() - 1);
		if(-1 == strTemp.ReverseFind(_T('\\')) && -1 == strTemp.ReverseFind(_T('/')))
		{
			m_strIVDFileRstSaveFolder += _T("\\");
		}
	}

	if(m_strIVDFileRstSaveName.IsEmpty())
	{
		m_strIVDFileRstSaveName = _T("Result");
	}

	return m_strIVDFileRstSaveFolder + m_strIVDFileRstSaveName + PS96IVD_OLD_RESULT_SUFFIX + _T(".xlsx");
}

void CExperimentData::SetFileOpenPath(CString strData)
{
	m_strFileOpenPath = strData;
	return;
}

CString CExperimentData::GetFileOpenPath()
{
	return m_strFileOpenPath;
}

void CExperimentData::SetDefaultAxisCount(int iCount)
{
	m_lnkDefAxisInfo.Clear();
	CAxisInfo axisInfo;
	for(int i = 0;i < iCount;++i)
	{
		m_lnkDefAxisInfo.AddTail(axisInfo);
	}
	return;
}

int CExperimentData::GetDefaultAxisCount()
{
	return m_lnkDefAxisInfo.GetSize();
}

int CExperimentData::SetDefaultAxisInfoAt(int iIndex,CAxisInfo& axisInfo)
{
	if(iIndex < 0)
	{
		return -1;
	}

	if(iIndex >= m_lnkDefAxisInfo.GetSize())
	{
		return -2;
	}

	*m_lnkDefAxisInfo[iIndex] = axisInfo;
	return 0;
}

CAxisInfo CExperimentData::GetDefaultAxisInfoAt(int iIndex)
{
	CAxisInfo axisInfo;
	if(iIndex < 0 || iIndex >= m_lnkDefAxisInfo.GetSize())
	{
		return axisInfo;
	}

	return *m_lnkDefAxisInfo[iIndex];
}

int CExperimentData::GetPreStepDelay(int iSeg,int iStep)
{
	if(iSeg < 0 || iSeg >= m_lnkSegment.GetSize())
	{
		return 0;
	}

	Node<CSegItemInfo>* pNodeSegItem = m_lnkSegment.GetAt(iSeg);
	if(NULL == pNodeSegItem)
	{
		return 0;
	}

	if(iStep < 0 || iStep >= pNodeSegItem->data.m_lnkStepItemInfo.GetSize())
	{
		return 0;
	}

	Node<CStepItemInfo>* pNodeStepItemInfo = pNodeSegItem->data.m_lnkStepItemInfo.GetAt(iStep);
	if(NULL == pNodeStepItemInfo)
	{
		return 0;
	}

	if(NULL != pNodeStepItemInfo->pPrevious)
	{
		return pNodeStepItemInfo->pPrevious->data.m_iDelay;
	}

	if(NULL != pNodeSegItem->pPrevious && pNodeSegItem->pPrevious->data.m_lnkStepItemInfo.GetSize() > 0)
	{
		return pNodeSegItem->pPrevious->data.m_lnkStepItemInfo.GetTail()->data.m_iDelay;
	}

	return 0;
}

float CExperimentData::GetPreStepTemperature(int iSeg,int iStep)
{
	if(iSeg < 0 || iSeg >= m_lnkSegment.GetSize())
	{
		return 0;
	}

	Node<CSegItemInfo>* pNodeSegItem = m_lnkSegment.GetAt(iSeg);
	if(NULL == pNodeSegItem)
	{
		return 0;
	}

	if(iStep < 0 || iStep >= pNodeSegItem->data.m_lnkStepItemInfo.GetSize())
	{
		return 0;
	}

	Node<CStepItemInfo>* pNodeStepItemInfo = pNodeSegItem->data.m_lnkStepItemInfo.GetAt(iStep);
	if(NULL == pNodeStepItemInfo)
	{
		return 0;
	}

	if(NULL != pNodeStepItemInfo->pPrevious)
	{
		return pNodeStepItemInfo->pPrevious->data.m_fAimTem;
	}

	if(NULL != pNodeSegItem->pPrevious && pNodeSegItem->pPrevious->data.m_lnkStepItemInfo.GetSize() > 0)
	{
		return pNodeSegItem->pPrevious->data.m_lnkStepItemInfo.GetTail()->data.m_fAimTem;
	}

	return 0;
}

double CExperimentData::GetStepVolumeDelaySec(double dStepTem)
{
	if(!GetBeN96ExperFile())
	{
		return 0;
	}

	if(NULL == m_pReagentItem)
	{
		return 0;
	}

	int iVolume = m_pReagentItem->GetReactVolume();
	if(iVolume <= m_volumeDelayParam.m_dBaseVolume)
	{
		return 0;
	}

	if(dStepTem > m_volumeDelayParam.m_dDownTemThre && dStepTem < m_volumeDelayParam.m_dUpTemThre)
	{
		return 0;
	}

	double dVolumeDiff = iVolume - m_volumeDelayParam.m_dBaseVolume;
	if(dStepTem <= m_volumeDelayParam.m_dDownTemThre)
	{
		return GetRound(dVolumeDiff * m_volumeDelayParam.m_dDownK);
	}
	else if(dStepTem >= m_volumeDelayParam.m_dUpTemThre)
	{
		return GetRound(dVolumeDiff * m_volumeDelayParam.m_dUpK);
	}

	return 0;
}

double CExperimentData::GetTotalVolumeDelaySec(BOOL bIgnoreInstruType)
{
	if(!bIgnoreInstruType && !GetBeN96ExperFile())
	{
		return 0;
	}

	if(NULL == m_pReagentItem)
	{
		return 0;
	}

	int iVolume = m_pReagentItem->GetReactVolume();
	if(iVolume <= m_volumeDelayParam.m_dBaseVolume)
	{
		return 0;
	}

	int iVolumeDelaySec = 0;
	double dVolumeDiff = iVolume - m_volumeDelayParam.m_dBaseVolume;
	
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	Node<CStepItemInfo>* pStepNode = NULL;
	
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		
		while(NULL != pStepNode)
		{
			CStepItemInfo& stepItem = pStepNode->data;
			if(!stepItem.GetBeCollectAll())
			{
				int iIncrementSec = 0;
				if(stepItem.m_fAimTem >= m_volumeDelayParam.m_dUpTemThre)
				{
					iIncrementSec = GetRound(dVolumeDiff * m_volumeDelayParam.m_dUpK);
					if(iIncrementSec < 0)
					{
						iIncrementSec = 0;
					}
				}
				else if(stepItem.m_fAimTem < m_volumeDelayParam.m_dDownTemThre)
				{
					iIncrementSec = GetRound(dVolumeDiff * m_volumeDelayParam.m_dDownK);
					if(iIncrementSec < 0)
					{
						iIncrementSec = 0;
					}
				}

				iVolumeDelaySec += (iIncrementSec * pSegNode->data.m_iCycleNum);
			}

			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}

	return iVolumeDelaySec;
}

void CExperimentData::SetUserName(CString strData)
{
	m_strUserName = strData;
	return;
}

CString CExperimentData::GetUserName()
{
	return m_strUserName;
}

void CExperimentData::SetCYSampleCategory(CString strData)
{
	m_strCYSamCategory = strData;
	return;
}

CString CExperimentData::GetCYSampleCategory()
{
	return m_strCYSamCategory;
}

void CExperimentData::SetCYSampleCount(CString strData)
{
	m_strCYSamCount = strData;
	return;
}

CString CExperimentData::GetCYSampleCount()
{
	return m_strCYSamCount;
}

void CExperimentData::SetCYSampleState(CString strData)
{
	m_strCYSamState = strData;
	return;
}

CString CExperimentData::GetCYSampleState()
{
	return m_strCYSamState;
}

void CExperimentData::SetCYProductBatchNumber(CString strData)
{
	m_strCYProductBN = strData;
	return;
}

CString CExperimentData::GetCYProductBatchNumber()
{
	return m_strCYProductBN;
}

void CExperimentData::SetCYTestItem(CString strData)
{
	m_strCYTestItem = strData;
	return;
}

CString CExperimentData::GetCYTestItem()
{
	return m_strCYTestItem;
}

void CExperimentData::SetCYComment(CString strData)
{
	m_strCYComment = strData;
	return;
}

CString CExperimentData::GetCYComment()
{
	return m_strCYComment;
}

void CExperimentData::SetCYReportNo(CString strData)
{
	if(NULL == m_pExpInfo)
	{
		return;
	}
	m_pExpInfo->SetCYReportNo(strData);

	return;
}

CString CExperimentData::GetCYReportNo()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	return m_pExpInfo->GetCYReportNo();
}

void CExperimentData::SetCYCollectDate(CString strData)
{
	m_strCYCollectDate = strData;
	return;
}

CString CExperimentData::GetCYCollectDate()
{
	return m_strCYCollectDate;
}


CItemChannelInfo* CExperimentData::GetReagentChanInfo(int iChanId)
{
	if(NULL == m_pReagentItem)
	{
		return NULL;
	}

	return m_pReagentItem->GetChannelTargetBy(iChanId);
}

CString CExperimentData::GetProjectName()
{
	if(NULL == m_pReagentItem)
	{
		return _T("");
	}

	return m_pReagentItem->GetName();
}

void CExperimentData::SetHardwareInfo(CHardwareInfo& info)
{
	m_hardwareInfo = info;
}

CHardwareInfo CExperimentData::GetHardwareInfo()
{
	return m_hardwareInfo;
}

CString CExperimentData::GetStartTimeString()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	return m_pExpInfo->GetStartTime();
}

CString CExperimentData::GetEndTimeString()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	return m_pExpInfo->GetEndTime();
}

CString CExperimentData::GetEndTimeYMDString()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	CString strTemp = m_pExpInfo->GetEndTime();
	if(strTemp.GetLength() > 10)
	{
		strTemp = strTemp.Left(10);
	}
	return strTemp;
}

CString CExperimentData::GetTotalTimeString()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	return m_pExpInfo->GetTotalTimeString();
}

CString CExperimentData::GetExperName()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	return m_pExpInfo->GetExpName();
}

CString CExperimentData::GetExperSaveFolder()
{
	if(NULL == m_pExpInfo)
	{
		return _T("");
	}

	return m_pExpInfo->GetFilePath();
}

BOOL CExperimentData::GetBePS96ExperFile()
{
	// return PCR_CODE_IAMP_PS96 == m_hardwareInfo.m_strCode || m_pExpInfo->GetBePS96ExperimentFile();
	return TRUE;
}

BOOL CExperimentData::GetBeXINYIM16ExperFile()
{
	return PCR_CODE_M16_XINYI == m_hardwareInfo.m_strCode;
}

BOOL CExperimentData::GetBeNQ16ExperFile()
{
	return PCR_CODE_N16_STANDARD == m_hardwareInfo.m_strCode;
}

BOOL CExperimentData::GetBeN96ExperFile()
{
	return PCR_CODE_QPCR_STANDARD == m_hardwareInfo.m_strCode;
}

BOOL CExperimentData::GetBeUseAvarageTemperature()
{
	return PCR_CODE_IAMP_PS96 == m_hardwareInfo.m_strCode;
}

BOOL CExperimentData::GetBeNewExperFile()
{
	return STATUS_NEW == m_nStatus;
}

void CExperimentData::SetNormalizeSelectLogState(BOOL bSelLogData)
{
	m_bNormalizeSelLog = bSelLogData;
}

BOOL CExperimentData::GetNormalizeSelectLogState()
{
	return m_bNormalizeSelLog;
}

void CExperimentData::SetDeltaRnSelectLogState(BOOL bSelLogData)
{
	m_bDeltaRnSelLog = bSelLogData;
}

BOOL CExperimentData::GetDeltaRnSelectLogState()
{
	return m_bDeltaRnSelLog;
}

void CExperimentData::SetFilterSelectLogState(BOOL bSelLogData)
{
	m_bFilterSelLog = bSelLogData;
}

BOOL CExperimentData::GetFilterSelectLogState()
{
	return m_bFilterSelLog;
}

int CExperimentData::AutoTransformThresholdValue(int iDataType)
{
	if(iDataType < 0)
	{
		return -1;
	}
	if(iDataType > 1)
	{
		return -2;
	}

	tagAnaAmplifyPara* pAnaParam = NULL;
	for(int i = 0;i < m_iChannelCount;++i)
	{
		pAnaParam = GetAmplifyParaBy(i);
		if(NULL == pAnaParam)
		{
			continue;
		}

		if(0 == iDataType)
		{
			if(m_bNormalizeSelLog)
			{
				pAnaParam->dCalcNormalizeThreshold = log10(pAnaParam->dCalcNormalizeThreshold);
			}
			else
			{
				pAnaParam->dCalcNormalizeThreshold = pow(10.0,pAnaParam->dCalcNormalizeThreshold);
			}
		}
		else if(1 == iDataType)
		{
			if(m_bDeltaRnSelLog)
			{
				pAnaParam->dCalcDeltaRnThreshold = log10(pAnaParam->dCalcDeltaRnThreshold);
			}
			else
			{
				pAnaParam->dCalcDeltaRnThreshold = pow(10.0,pAnaParam->dCalcDeltaRnThreshold);
			}
		}
	}

	return 0;
}

int CExperimentData::ReadOriginalDataFromExcel(CString strFileName,vector<double>* pVecData)
{
	if(strFileName.IsEmpty())
	{
		return -1;
	}

	if(NULL == pVecData)
	{
		return -2;
	}

	int iIndex = strFileName.ReverseFind(_T('.'));
	if(iIndex < 0)
	{
		return -3;
	}

	CString strExt = strFileName.Mid(iIndex + 1);

	Book* book;
	if(_T("xls") == strExt)
	{
		book = xlCreateBook();
	}
	else if(_T("xlsx") == strExt)
	{
		book = xlCreateXMLBook();
	}
	else
	{
		return -4;
	}

	book->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe"));
	if(NULL == book)
	{
		AfxMessageBox(_T("Fail to create excel file!"));
		return -5;
	}

	if (!book->load(strFileName))
	{
		AfxMessageBox(_T("Fail to open excel file!"));
		return -6;
	}

	Sheet* pSheet = book->getSheet(0);
	if (NULL == pSheet)
	{
		book->release();
		return -7;
	}

	int iRow = 2,iCol = 1,iNextRow = 0,iPreRow = 2,iPreCol = 1;
	CString strTemp = pSheet->readStr(iRow,iCol),strChanName = pSheet->readStr(iRow - 1,iCol + 1),strSamIndex(_T(""));
	
	if(_T("Cycle") != strTemp)
	{
		AfxMessageBox(_T("This is not a PCR source data file!"));
		book->release();
		return -8;
	}

	int iCycleCount = 0,iTubeNum = 0,iChanId = 0;;
	double dData = 0;

	while(_T("Cycle") == strTemp)
	{
		iCycleCount = 0;
		do 
		{
			dData = pSheet->readNum(++iRow,iCol);
			if(dData > 0)
			{
				++iCycleCount;
			}
		} while(dData > 0);

		iNextRow = iRow;
		iRow = iPreRow;

		iCol += 1;
		iTubeNum = 0;
		int iIndex = -1,iSamIndex = 0;
		vector<int> vecSamIndex;
		do 
		{
			strTemp = pSheet->readStr(iRow,++iCol);
			iIndex = strTemp.Find(_T("Tube_"));
			if(-1 == iIndex)
			{
				break;
			}

			strSamIndex = strTemp.Mid(iIndex + 5);
			if(strSamIndex.GetLength() < 2)
			{
				break;
			}

			iSamIndex = (strSamIndex.GetAt(0) - L'A') * 8 + _ttoi(strSamIndex.Mid(1)) - 1;
			vecSamIndex.push_back(iSamIndex);

			++iTubeNum;
		} while(1);

		iCol = iPreCol + 1;
		
		iChanId = 0;
		if(_T("FAM") == strChanName)
		{
			iChanId = 0;
		}
		else if(_T("Cy5") == strChanName)
		{
			iChanId = 1;
		}
		else if(_T("VIC") == strChanName)
		{
			iChanId = 2;
		}
		else if(_T("ROX") == strChanName)
		{
			iChanId = 3;
		}

		for(int i = 0;i < iCycleCount;++i)
		{
			++iRow;
			iCol = iPreCol + 1;
			for(int j = 0;j < iTubeNum;++j)
			{
				dData = pSheet->readNum(iRow,++iCol);
				pVecData[iChanId * 16 + vecSamIndex[j]].push_back(dData);
			}
		}

		//++iChanId;

		iRow += 2;
		iCol = iPreCol;
		strTemp = pSheet->readStr(iRow,iCol);
		strChanName = pSheet->readStr(iRow - 1,iCol + 1);
		iPreRow = iRow;
	}

	book->release();

	return 0;
}

int CExperimentData::SetTubeData(vector<double>* pVecData,int iTubeNum,int iChanNum)
{
	if(NULL == m_pAmplifyData)
	{
		return -10;
	}

	return m_pAmplifyData->SetTubeData(pVecData,iTubeNum,iChanNum);
}

int CExperimentData::SetTubeInfoToBeAllProcessed()
{
	if(m_iChannelCount < 1)
	{
		return -1;
	}

	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return -2;
	}

	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i <iTubeCount;++i)
	{
		pTubeInfo = GetTubeInfoAt(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		pTubeInfo->SetSampleType(SAMPLE_TYPE_UNKNOWN);

		for(int j = 0;j < m_iChannelCount;++j)
		{
			pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
			if(NULL == pDyeTargetInfo)
			{
				continue;
			}
			pDyeTargetInfo->SetBeSelected(TRUE);
		}
	}

	return 0;
}

void CExperimentData::DeleteRunSegment()
{
	m_lnkSegment.Clear();
}

void CExperimentData::DeleteTubeInfo()
{
	int iTubeCount = GetTubeCount();
	CTubeInfo* pTubeInfo = NULL;
	for(int i = 0;i < iTubeCount;++i)
	{
		pTubeInfo = GetTubeInfoAt(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		pTubeInfo->Reset(FALSE);
	}
	
	return;
}

void CExperimentData::GetSampleIdSiteIndexRelation(BOOL bHori)
{
	m_samIdSiteRelation.Clear();

	vector<int> vecInt;
	m_samIdSiteRelation.m_vecSamId.push_back(_T("NC"));
	m_samIdSiteRelation.m_2vecSiteIndex.push_back(vecInt);
	m_samIdSiteRelation.m_vecSamId.push_back(_T("PC"));
	m_samIdSiteRelation.m_2vecSiteIndex.push_back(vecInt);

	CTubeInfo* pTubeInfo = NULL;
	int iTubeNum = GetTubeCount();

	int iPlateRowNum = GetRowCount(),iPalteColNum = GetColCount();
	int iTubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	if(bHori)
	{
		kEnd = iPlateRowNum;
		tEnd = iPalteColNum;
	}
	else
	{
		kEnd = iPalteColNum;
		tEnd = iPlateRowNum;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(bHori)
			{
				iTubeIndex = k * iPalteColNum + t;
			}
			else
			{
				iTubeIndex = t * iPalteColNum + k;
			}

			pTubeInfo = GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo)
			{
				continue;
			}

			if(pTubeInfo->GetBeNegtive())
			{
				m_samIdSiteRelation.m_2vecSiteIndex[0].push_back(iTubeIndex);
				continue;
			}
			else if(pTubeInfo->GetBePositive())
			{
				m_samIdSiteRelation.m_2vecSiteIndex[1].push_back(iTubeIndex);
				continue;
			}

			m_samIdSiteRelation.AddSamIdSite(pTubeInfo->GetSampleID(),iTubeIndex);
		}
	}

	return;
}

int CExperimentData::GetResultFileDyeTargetTitle(vector<CString>& vecTitle)
{
	vecTitle.clear();

	if(m_iChannelCount < 1)
	{
		return -1;
	}

	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return -2;
	}

	CString strTitle(_T("")),strGeneName(_T("")),strDyeName(_T(""));
	vector<CString> vecGeneName;
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < m_iChannelCount;++i)
	{
		strDyeName.Empty();
		strTitle.Empty();
		vecGeneName.clear();
		for(int j = 0;j < iTubeCount;++j)
		{
			pTubeInfo = GetTubeInfoAt(j);
			if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType())
			{
				continue;
			}

			pDyeTargetInfo = pTubeInfo->GetTargetInfo(i);
			if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
			{
				continue;
			}

			strGeneName = pDyeTargetInfo->GetTargetName();
			if(0 == j)
			{
				strDyeName = pDyeTargetInfo->GetDyeName();
			}

			BOOL bFind = FALSE;
			if(!strGeneName.IsEmpty())
			{
				int iTempIndex = strGeneName.Find(_T("\r\n"));
				if (iTempIndex == -1)
					iTempIndex = strGeneName.Find(_T("\n"));
				if (iTempIndex != -1)
				{
					strGeneName = strGeneName.Left(iTempIndex);
				}

				for(int k = 0;k < vecGeneName.size();++k)
				{
					if(strGeneName == vecGeneName[k])
					{
						bFind = TRUE;
						break;
					}
				}
				if(!bFind)
				{
					vecGeneName.push_back(strGeneName);
				}
			}
		}

		if(vecGeneName.size() > 0)
		{
			strTitle = vecGeneName[0];
			for(int t = 1;t < vecGeneName.size();++t)
			{
				strTitle += _T(";");
				strTitle += vecGeneName[t];
			}
			strTitle += _T("-");
		}
		strTitle += strDyeName;

		vecTitle.push_back(strTitle);
	}

	return 0;
}

int CExperimentData::GetChannelIsSelectedByAtLeastOneTube(vector<BOOL>& vecChanSelState)
{
	vecChanSelState.clear();

	if(m_iChannelCount < 1)
	{
		return -1;
	}

	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return -2;
	}

	BOOL bSelected = FALSE;
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < m_iChannelCount;++i)
	{
		bSelected = FALSE;
		for(int j = 0;j < iTubeCount;++j)
		{
			pTubeInfo = GetTubeInfoAt(j);
			if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType())
			{
				continue;
			}

			pDyeTargetInfo = pTubeInfo->GetTargetInfo(i);
			if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())
			{
				continue;
			}

			bSelected = TRUE;
			break;
		}

		vecChanSelState.push_back(bSelected);
	}

	return 0;
}

BOOL CExperimentData::IsSampleIdSet()
{
	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return FALSE;
	}

	CTubeInfo* pTubeInfo = NULL;
	for(int j = 0;j < iTubeCount;++j)
	{
		pTubeInfo = GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			continue;
		}

		if(!pTubeInfo->GetSampleID().IsEmpty())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CExperimentData::IsAllNoneSampleType()
{
	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return TRUE;
	}

	CTubeInfo* pTubeInfo = NULL;
	for(int j = 0;j < iTubeCount;++j)
	{
		pTubeInfo = GetTubeInfoAt(j);
		if(NULL != pTubeInfo && !pTubeInfo->GetBeNoneSampleType())
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CExperimentData::IsSampleIdSetAtExceptNoneSampleType()
{
	int iTubeCount = GetTubeCount();
	if(iTubeCount < 1)
	{
		return FALSE;
	}

	CTubeInfo* pTubeInfo = NULL;
	for(int j = 0;j < iTubeCount;++j)
	{
		pTubeInfo = GetTubeInfoAt(j);
		if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType())
		{
			continue;
		}

		if(!pTubeInfo->GetSampleID().IsEmpty())
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CExperimentData::SetChannelCrossTalkParam(double* pCrosstalkPara,int iChanCount)
{
	if(NULL == pCrosstalkPara)
	{
		return -1;
	}

	if(iChanCount != m_iChannelCount)
	{
		return -2;
	}

	if(iChanCount < 1)
	{
		DeleteCrosstalkPara();
		return 0;
	}

	if(NULL == m_ppFCrosstalkPara)
	{
		return -3;
	}

	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			m_ppFCrosstalkPara[i][j] = pCrosstalkPara[i * iChanCount + j];
		}
	}

	return 0;
}

CExpStateInfo CExperimentData::GetExperimentStateInfo()
{
	m_ExpStateInfo.Clear();

	if(NULL != m_pExpInfo)
	{
		m_ExpStateInfo.m_strExpName = m_pExpInfo->GetExpName();
		m_ExpStateInfo.m_iRemainSec = m_pExpInfo->GetRemainSec();

		int iTotalSec = m_pExpInfo->GetTotalSec();
		int iHasRunSec = m_pExpInfo->GetHasRunSec();
		if(0 != iTotalSec)
		{
			m_ExpStateInfo.m_dCurProgress = double(iHasRunSec) / iTotalSec;
		}
		else
		{
			m_ExpStateInfo.m_dCurProgress = 0;
		}
		m_ExpStateInfo.m_eExpType = m_pExpInfo->GetExpType();
	}

	switch(m_nStatus)
	{
	case STATUS_PLATE:
	case STATUS_CHANGE:
	case STATUS_SAVE:
	case STATUS_SAVE_DATA:
	case STATUS_NEW: m_ExpStateInfo.m_eRunBtState = ERBS_RUN; m_ExpStateInfo.m_eBinBtState = EBBS_HIDE; break;
	case STATUS_USING: m_ExpStateInfo.m_eRunBtState = ERBS_STOP; m_ExpStateInfo.m_eBinBtState = EBBS_HIDE; break;
	default: break;
	 }

	if(STATUS_USING == m_nStatus)
	{
		m_ExpStateInfo.m_dCurTem = m_dCurTem;
	}
	else
	{
		m_ExpStateInfo.m_dCurTem = -1;
	}

	BOOL bHaveIdleInstru = FALSE;
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL != pInstrument)
	{
		CString strModel = m_pExpInfo->GetInstrument();
		CString strSN = m_pExpInfo->GetInstrumentSN();
		CInstancePCR* pInstance = pInstrument->GetInstancePCR(strModel,strSN);
		if(NULL != pInstance && !pInstance->IsDemo() && pInstance->IsInstanceIdle())
		{
			bHaveIdleInstru = TRUE;
		}
	}

	if(!bHaveIdleInstru)
	{
		if(ERBS_RUN == m_ExpStateInfo.m_eRunBtState)
		{
			m_ExpStateInfo.m_eRunBtState = ERBS_RUNFORBIDDEN;
		}
		if(EBBS_IN == m_ExpStateInfo.m_eBinBtState)
		{
			m_ExpStateInfo.m_eBinBtState = EBBS_INFORBIDDEN;
		}
		if(EBBS_OUT == m_ExpStateInfo.m_eBinBtState)
		{
			m_ExpStateInfo.m_eBinBtState = EBBS_OUTFORBIDDEN;
		}
	}
	
	return m_ExpStateInfo;
}

double CExperimentData::GetExperimentProgerss()
{
	if(NULL == m_pExpInfo)
	{
		return 0;
	}
	if(STATUS_NEW == m_nStatus)
	{
		return 0;
	}

	int iTotalSec = m_pExpInfo->GetTotalSec();
	if(iTotalSec <= 0)
	{
		return 0;
	}

	int iHasRunSec = m_pExpInfo->GetHasRunSec();
	return double(iHasRunSec) / iTotalSec;
}

BOOL CExperimentData::GetGradientFuncShieldState()
{
	return !GetBeN96ExperFile();
}

float CExperimentData::GetFirstMeltStepStartTem()
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	Node<CStepItemInfo>* pStepNode = NULL;
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollectAll())
			{
				if(NULL != pStepNode->pPrevious)
				{
					return pStepNode->pPrevious->data.m_fAimTem;
				}
				else
				{
					if(NULL != pSegNode->pPrevious)
					{
						return pSegNode->pPrevious->data.m_lnkStepItemInfo.GetTail()->data.m_fAimTem;
					}
				}
			}
			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}

	return 0;
}

float CExperimentData::GetFirstMeltStepEndTem()
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	Node<CStepItemInfo>* pStepNode = NULL;
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollectAll())
			{
				return pStepNode->data.m_fAimTem;
			}
			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}

	return 0;
}

void CExperimentData::CorrectSegmentNameByLanguage(CString strSource, CString strTarget)
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegment.GetHead();
	while(NULL != pSegNode)
	{
		pSegNode->data.CorrectNameLanguage(strSource, strTarget);
		pSegNode = pSegNode->pNext;
	}
}

BOOL CExperimentData::WriteLog(CFile* pFile)
{
	int iCount = m_arrayLog.GetCount();
	pFile->Write((const char*)&iCount, sizeof(int));
	for (int i=0; i<iCount; i++)
	{
		CPublicFun::WriteStringToFile(pFile, m_arrayLog.GetAt(i));
	}

	return TRUE;
}

BOOL CExperimentData::ReadLog(CFile* pFile,WORD wVersion)
{
	int iCount = 0;
	pFile->Read((char*)&iCount, sizeof(int));
	for (int i=0; i<iCount; i++)
	{
		CString strTemp = CPublicFun::ReadStringFromFile(pFile);
		m_arrayLog.Add(strTemp);
	}

	return TRUE;
}

CString CExperimentData::GetAbnormalLog()
{
	CString strLog;
	for (int i=0; i<m_arrayLog.GetCount(); i++)
	{
		if (!strLog.IsEmpty())
		{
			strLog += _T(";");
		}
		strLog += m_arrayLog.GetAt(i);
	}

	return strLog;
}

void CExperimentData::InitAmplifyParamByTubeTarget()
{
	int iChannelCount = m_pReagentItem->GetChannelTargetCount();
	for (int i=0; i<iChannelCount; i++)
	{
		CItemChannelInfo* pChannel = m_pReagentItem->GetChannelTargetBy(i);
		tagAnaAmplifyPara* pAmplifyParam = new tagAnaAmplifyPara;
		pAmplifyParam->strDye = pChannel->GetDyeName();
		pAmplifyParam->strTarget = pChannel->GetTargetName(0);
		pAmplifyParam->iChannelNo = pChannel->GetChannel();
		m_pAnalysisPara->AddAmplifyPara(pAmplifyParam);
	}

	int iTubeCount = m_pPlateInfo->GetTubeCount();
	for (int i=0; i<iTubeCount; i++)
	{
		CTubeInfo* pTube = m_pPlateInfo->GetTubeInfo(i);
		if (pTube == NULL)
			continue;
		
		int iTargetCount = pTube->GetTargetCount();
		for (int j=0; j<iTargetCount; j++)
		{
			CDyeTargetInfo* pTarget = pTube->GetTargetInfo(j);
			if (pTarget == NULL || pTarget->GetDyeName() == _T("MOT") || pTarget->GetDyeName().IsEmpty())
				continue;

			if (pTarget->GetTargetName().IsEmpty())
			{
				tagAnaAmplifyPara* pTempParam = m_pAnalysisPara->GetAmplifyParaByDye(pTarget->GetDyeName());
				if (pTempParam == NULL)
				{
					tagAnaAmplifyPara* pAmplifyParam = new tagAnaAmplifyPara;
					pAmplifyParam->strDye = pTarget->GetDyeName();
					pAmplifyParam->iChannelNo = pTarget->GetChannel();
					m_pAnalysisPara->AddAmplifyPara(pAmplifyParam);
				}
			}
			else
			{
				tagAnaAmplifyPara* pTempParam = m_pAnalysisPara->GetAmplifyParaByTarget(pTarget->GetTargetName(), pTarget->GetDyeName());
				if (pTempParam != NULL)
					continue;

				pTempParam = m_pAnalysisPara->GetAmplifyParaByDye(pTarget->GetDyeName());
				if (pTempParam != NULL)
				{
					pTempParam->strTarget = pTarget->GetTargetName();
				}
				else
				{
					tagAnaAmplifyPara* pAmplifyParam = new tagAnaAmplifyPara;
					pAmplifyParam->strDye = pTarget->GetDyeName();
					pAmplifyParam->strTarget = pTarget->GetTargetName();
					pAmplifyParam->iChannelNo = pTarget->GetChannel();
					m_pAnalysisPara->AddAmplifyPara(pAmplifyParam);
				}
			}
		}
	}

	return;
}

void CExperimentData::ConnInterruptOnce() 
{
	m_iConnInterruptCount ++;
}

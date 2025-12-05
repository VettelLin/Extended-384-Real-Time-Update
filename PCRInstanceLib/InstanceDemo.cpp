#include "StdAfx.h"
#include "InstanceDemo.h"
#include "Define.h"

CInstanceDemo::CInstanceDemo(CSerialPortPCR* pComPort, CString strVersion) : CInstancePCR(pComPort, strVersion)
{
	m_iTubeCount = 0;
	m_iCurCycleNo = -1;
	m_iFirstChannelNo = -1;
	m_iCurTempIndex = 0;
	m_pCurFluIndex = NULL;
	InitStructurePara();
}


CInstanceDemo::~CInstanceDemo(void)
{
	m_iTubeCount = 0;
	if(NULL != m_pCurFluIndex)
	{
		delete [] m_pCurFluIndex;
	}
}

BOOL CInstanceDemo::Initialize()
{
	InitDevicePara();
	InitDeviceStatus();
	InitStructurePara();

	BOOL bReturn = Hello();
	if (!bReturn)
		return bReturn;

	// 读取设备参数
	bReturn = ReadDeviceParamsFromEPROM();

#ifndef PS96INSTRUMENT
	//N16,N96QPCR Demo模式默认为四通道,PS96默认模式为5 + 1通道
	char* pChannel1 = "T505,ET470/10,ET525/10";
	memcpy(m_sDevicePara.cKeyFilter1, pChannel1, strlen(pChannel1));
	pChannel1 = "T650,ET620/16,ET680/20";
	memcpy(m_sDevicePara.cKeyFilter2, pChannel1, strlen(pChannel1));
	pChannel1 = "T550,ET515/20,ET575/20";
	memcpy(m_sDevicePara.cKeyFilter3, pChannel1, strlen(pChannel1));
	pChannel1 = "T585,ET560/20,ET620/20";	
	memcpy(m_sDevicePara.cKeyFilter4, pChannel1, strlen(pChannel1));

	char* pTemp = "11110000";
	memcpy(m_sDevicePara.cFilterConfig, pTemp,8);
#endif

	if(bReturn)
	{
		bReturn = ReadStructureParamsFromEPROM();
	}

	InitFilterPara();


	// 各通道温度数据
	CString strFileTemp = m_strStartPath + _T("DemoData\\TempData.ini");
	CString strAppName = _T("TempData");
	CString strKeyName = _T("Count");
	int iTempCount = ::GetPrivateProfileInt(strAppName, strKeyName, 0, strFileTemp);
	for (int i=0; i<iTempCount; i++)
	{
		strKeyName.Format(_T("Temp%d"), i+1);
		UINT nTempData = ::GetPrivateProfileInt(strAppName, strKeyName, 0, strFileTemp);
		m_arrayTemp.Add(nTempData);
	}

	// 各通道荧光数据
	InitFluData();

	return bReturn;
}

void CInstanceDemo::InitStructurePara()
{
	CInstancePCR::InitStructurePara();

#ifdef PS96INSTRUMENT
	//lzh
	//m_sStructurePara.cPlateRowCount = 8;
	//m_sStructurePara.cPlateColCount = 12;
	m_sStructurePara.cPlateRowCount = 3;
	m_sStructurePara.cPlateColCount = 6;
#else

#ifdef N96INSTRUMENT 
	m_sStructurePara.cPlateRowCount = 8;
	m_sStructurePara.cPlateColCount = 12;
#else
	m_sStructurePara.cPlateRowCount = 2;
	m_sStructurePara.cPlateColCount = 8;
#endif

#endif

	m_sStructurePara.cLidTempMaxTime = 1;
	m_sStructurePara.cTempCtrlMaxTime = 5;

#ifdef PS96INSTRUMENT
	m_sStructurePara.cCalibratePara = 1; // 使用浊度校准
#endif
}

BOOL CInstanceDemo::Hello()
{
	::Sleep(100);
	return TRUE;
}

BOOL CInstanceDemo::ReadDeviceParamsFromEPROM()
{
	return TRUE;
}

BOOL CInstanceDemo::ReadStructureParamsFromEPROM()
{
	return TRUE;
}

BOOL CInstanceDemo::Home()
{
	::Sleep(100);
	return TRUE;
}

// 默认X电机
BOOL CInstanceDemo::MotorScanRow(WORD wPulse, BYTE cDirection)
{
	::Sleep(100);
	return TRUE;
}

BOOL CInstanceDemo::SetStateLED(BYTE cMode)
{
	::Sleep(100);
	return TRUE;
}

BOOL CInstanceDemo::SetPMTGain(WORD wGainPara)
{
	::Sleep(100);
	return TRUE;
}

// 控温方式？是否控温
BOOL CInstanceDemo::SetTempCtrlMode(eTempCtrlMode nCtrlMode)
{
	::Sleep(100);
	return TRUE;
}

// 设置热盖温度
BOOL CInstanceDemo::SetLidHeaterTemp(float fTemp)
{
	::Sleep(100);
	return TRUE;
}

BOOL CInstanceDemo::SetMultiTemp(float fTemp1, float fTemp2, float fTemp3, float fRate, BYTE cMode)
{
	::Sleep(100);
	return TRUE;
}

BOOL CInstanceDemo::SetLaserStatus(BYTE cType, BYTE cStatus)
{
	::Sleep(100);
	return TRUE;
}

BOOL CInstanceDemo::ScanOnce(BOOL *pBeAskTop)
{
	::Sleep(2000);
	return TRUE;
}

// 读取指定通道温度
BOOL CInstanceDemo::ReadChannelTemp(BYTE cChannel, float &fTempCh, BOOL& bToTarget)
{
	::Sleep(100);
	fTempCh = 65.0f;
	bToTarget = TRUE;

	return TRUE;
}

// 读取PCR仪温度
BOOL CInstanceDemo::ReadPCRTemp(float &fTempPCR, BOOL& bToTarget)
{
	::Sleep(100);

	fTempPCR = (float)m_arrayTemp.GetAt(m_iCurTempIndex) / RESOLUTION_TEMP;
	bToTarget = TRUE;

	m_iCurTempIndex ++;
	if (m_iCurTempIndex >= m_arrayTemp.GetCount())
	{
		m_iCurTempIndex -= m_iCurTempIndex / 4;
	}

	return TRUE;
}

BOOL CInstanceDemo::ReadFLUData(CUIntArray& arrayData, BYTE cChannel, BYTE cFrmIndex)
{
	if (m_iCurCycleNo < 0)
	{
		m_iCurCycleNo = 0;
	}
	else if (m_iFirstChannelNo == cChannel)
	{
		m_iCurCycleNo ++;
	}

	if (m_iFirstChannelNo < 0)
	{
		m_iFirstChannelNo = cChannel;
	}

	::Sleep(100);

	for (int i=0; i<m_iTubeCount; i++)
	{
		UINT nFluData = 0;
		BOOL bToEnd = FALSE;
		switch (cChannel)
		{
		case 1:
			nFluData = GetFluDataBy(i, &m_arrayFluFam);
			break;
		case 2:
			nFluData = GetFluDataBy(i, &m_arrayFluCy5);
			break;
		case 3:
			nFluData = GetFluDataBy(i, &m_arrayFluHex);
			break;
		case 4:
			nFluData = GetFluDataBy(i, &m_arrayFluCy55);
			break;
		case 5:
			nFluData = GetFluDataBy(i, &m_arrayFluRox);
			break;
		case 6:
			nFluData = GetFluDataBy(i, &m_arrayFluCy3);
			break;
		default:
			ASSERT(FALSE);
			break;
		}

		arrayData.Add(nFluData);
	}

	return TRUE;
}

void CInstanceDemo::InitDemoData(int iTubeCount)
{
	m_iCurCycleNo = -1;
	m_iFirstChannelNo = -1;
	m_iCurTempIndex = 0;
	
	if (m_iTubeCount != iTubeCount)
	{
		SAFE_DELETE(m_pCurFluIndex);
		m_iTubeCount = iTubeCount;
		m_pCurFluIndex = new int[iTubeCount];
	}
	
	for (int i=0; i<m_iTubeCount; i++)
	{
		m_pCurFluIndex[i] = ::rand() % 12; // 开始位置随机
	}
}

// 初始化各通道荧光数据
void CInstanceDemo::InitFluData()
{
	ASSERT(!m_strStartPath.IsEmpty());
	CString strIniFile = m_strStartPath + _T("DemoData\\Fam.ini");
	CFileFind finder;
	BOOL bReturn = finder.FindFile(strIniFile);
	if (bReturn)
	{
		InitOneDyeFluData(&m_arrayFluFam, strIniFile);
	}

	strIniFile = m_strStartPath + _T("DemoData\\Cy5.ini");
	bReturn = finder.FindFile(strIniFile);
	if (bReturn)
	{
		InitOneDyeFluData(&m_arrayFluCy5, strIniFile);
	}

	strIniFile = m_strStartPath + _T("DemoData\\Hex.ini");
	bReturn = finder.FindFile(strIniFile);
	if (bReturn)
	{
		InitOneDyeFluData(&m_arrayFluHex, strIniFile);
	}

	strIniFile = m_strStartPath + _T("DemoData\\Cy55.ini");
	bReturn = finder.FindFile(strIniFile);
	if (bReturn)
	{
		InitOneDyeFluData(&m_arrayFluCy55, strIniFile);
	}

	strIniFile = m_strStartPath + _T("DemoData\\Rox.ini");
	bReturn = finder.FindFile(strIniFile);
	if (bReturn)
	{
		InitOneDyeFluData(&m_arrayFluRox, strIniFile);
	}

	strIniFile = m_strStartPath + _T("DemoData\\Cy3.ini");
	bReturn = finder.FindFile(strIniFile);
	if (bReturn)
	{
		InitOneDyeFluData(&m_arrayFluCy3, strIniFile);
	}
}

void CInstanceDemo::InitOneDyeFluData(CUIntArray* pArrayFlu, CString strIniFile)
{
	pArrayFlu->RemoveAll();
	int iCount = ::GetPrivateProfileInt(_T("FluData"), _T("Count"), 0, strIniFile);
	int iFluData = 0;
	CString strKey;
	for (int i=0; i<iCount; i++)
	{
		strKey.Format(_T("C%d"), i+1);
		iFluData = ::GetPrivateProfileInt(_T("FluData"), strKey, 0, strIniFile);
		pArrayFlu->Add(iFluData);
	}
}


UINT CInstanceDemo::GetFluDataBy(int iTubeIndex, CUIntArray* pArrayFlu)
{
	UINT nFluData = 100;
	int iIndex = m_pCurFluIndex[iTubeIndex] + m_iCurCycleNo;
	BOOL bToEnd = FALSE;
	if (iIndex > pArrayFlu->GetCount())
	{
		iIndex = pArrayFlu->GetCount();
		bToEnd = TRUE;
	}
	
	if (iIndex > 0)
	{
		nFluData = pArrayFlu->GetAt(iIndex-1);
		if (bToEnd)
		{
			nFluData += ::rand() % 200;
		}
	}

	return nFluData;
}

BOOL CInstanceDemo::ReadDeviceStatus()
{
	::Sleep(50);
	return TRUE;
}

BOOL CInstanceDemo::ReadEPROM(UINT nAddress, BYTE cGetLength,BYTE* pReceive)
{
	if(NULL != pReceive)
	{
		*pReceive = 90;
	}
	return TRUE;
}

BOOL CInstanceDemo::WriteEPROM(UINT nAddress,int iData,int iDataLen)
{
	return FALSE;
}

// 取得热盖加热最长时间
int CInstanceDemo::GetMaxLidHeatSecond()
{
	int iMaxTime = CInstancePCR::GetMaxLidHeatSecond();
	if (iMaxTime > 5)
		iMaxTime = 5;
	return iMaxTime;
}

CString CInstanceDemo::GetModel()
{
	CString strModel = m_strModel;
	strModel += _T("_Demo");

	return strModel;
}

int CInstanceDemo::GetInitChanDyeName(vector<CString>& vecChanDyeName)
{
#ifdef PS96INSTRUMENT
	vecChanDyeName.clear();
	int iChanCount = GetScanChannelCount();
	if(iChanCount < 1)
	{
		return -1;
	}

	for(int i = 0;i < iChanCount;++i)
	{
		if(0 == i)
		{
			vecChanDyeName.push_back(_T("FAM"));
		}
		else if(1 == i)
		{
			vecChanDyeName.push_back(_T("Cy5")); 
		}
		else if(2 == i)
		{
			vecChanDyeName.push_back(_T("VIC"));
		}
		else if(3 == i)
		{
			vecChanDyeName.push_back(_T("Cy5.5"));
		}
		else if(4 == i)
		{
			vecChanDyeName.push_back(_T("ROX")); 
		}
		else if(5 == i)
		{
			vecChanDyeName.push_back(_T("MOT")); 
		}
		else
		{
			CString strTemp(_T(""));
			strTemp.Format(_T("%d"),i + 1);
			vecChanDyeName.push_back(strTemp);
		}
	}

	return 0;
#else
	return CInstancePCR::GetInitChanDyeName(vecChanDyeName);
#endif
}

BOOL CInstanceDemo::IsDemo()
{
	return TRUE;
}

void CInstanceDemo::ReadParaFromDemo(CString strIniFile)
{
	CString strValue;

	CString strAppName = _T("Instrument");
	TCHAR c1[250];
	::GetPrivateProfileString(strAppName, _T("PCRModel"), _T(""), c1, 250, strIniFile);
	m_strModel = c1;
	::GetPrivateProfileString(strAppName, _T("PCRVersion"), _T(""), c1, 250, strIniFile);
	m_strVersion = c1;

	m_sStructurePara.cTempMode = (BYTE)::GetPrivateProfileInt(strAppName, _T("TempMode"), 0, strIniFile);
	m_sStructurePara.cTempCtrl = (BYTE)::GetPrivateProfileInt(strAppName, _T("TempCtrlMode"), 1, strIniFile);
	m_sStructurePara.cLidHeader = (BYTE)::GetPrivateProfileInt(strAppName, _T("HasLid"), 0, strIniFile);

	m_sStructurePara.cPlateType = ::GetPrivateProfileInt(strAppName, _T("PlateType"), 0, strIniFile);
	m_sStructurePara.cPlateCount = ::GetPrivateProfileInt(strAppName, _T("PlateCount"), 1, strIniFile);
	m_sStructurePara.cPlateSortType = ::GetPrivateProfileInt(strAppName, _T("SortType"), 0, strIniFile);

#ifdef PS96INSTRUMENT
	//lzh
	//m_sStructurePara.cPlateRowCount = 8;
	//m_sStructurePara.cPlateColCount = 12;
	m_sStructurePara.cPlateRowCount = 3;
	m_sStructurePara.cPlateColCount = 6;
#else

#ifdef N96INSTRUMENT 
	m_sStructurePara.cPlateRowCount = 8;
	m_sStructurePara.cPlateColCount = 12;
#else
	m_sStructurePara.cPlateRowCount = 2;
	m_sStructurePara.cPlateColCount = 8;
#endif

#endif
}

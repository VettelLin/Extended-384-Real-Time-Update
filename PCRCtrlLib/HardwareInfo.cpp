#include "StdAfx.h"
#include "HardwareInfo.h"
#include "PublicFun.h"

CHardwareInfo::CHardwareInfo(void)
{
	m_strModel.Empty();
	m_strSN.Empty();
	m_strCode.Empty();
	m_strVersion.Empty();
	m_strSoftwareVer.Empty();
}

CHardwareInfo::~CHardwareInfo(void)
{

}

CHardwareInfo::CHardwareInfo(const CHardwareInfo& info)
{
	m_strModel = info.m_strModel;
	m_strSN = info.m_strSN;
	m_strCode = info.m_strCode;
	m_strVersion = info.m_strVersion;
	m_strSoftwareVer = info.m_strSoftwareVer;
	m_lnkScanChanInfo = info.m_lnkScanChanInfo;
}

void CHardwareInfo::operator=(const CHardwareInfo& info)
{
	m_strModel = info.m_strModel;
	m_strSN = info.m_strSN;
	m_strCode = info.m_strCode;
	m_strVersion = info.m_strVersion;
	m_strSoftwareVer = info.m_strSoftwareVer;
	m_lnkScanChanInfo = info.m_lnkScanChanInfo;
}

BOOL CHardwareInfo::WriteFile(CFile* pFile,BOOL bTemplate)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	if(bTemplate)
	{	
		//模板文件保存的硬件信息设置为空
		CPublicFun::WriteStringToFile(pFile,_T(""));
		CPublicFun::WriteStringToFile(pFile,_T(""));
		CPublicFun::WriteStringToFile(pFile,_T(""));
		CPublicFun::WriteStringToFile(pFile,_T(""));
		CPublicFun::WriteStringToFile(pFile,_T(""));
	}
	else
	{
		CPublicFun::WriteStringToFile(pFile,m_strModel);
		CPublicFun::WriteStringToFile(pFile,m_strSN);
		CPublicFun::WriteStringToFile(pFile,m_strCode);
		CPublicFun::WriteStringToFile(pFile,m_strVersion);
		CPublicFun::WriteStringToFile(pFile,m_strSoftwareVer);
	}
	return TRUE;
}

BOOL CHardwareInfo::ReadFile(CFile* pFile,WORD wVersion)
{
	if(wVersion < 0x0106)
	{
		return TRUE;
	}

	if(NULL == pFile)
	{
		return FALSE;
	}
	m_strModel = CPublicFun::ReadStringFromFile(pFile);
	m_strSN = CPublicFun::ReadStringFromFile(pFile);
	m_strCode = CPublicFun::ReadStringFromFile(pFile);
	m_strVersion = CPublicFun::ReadStringFromFile(pFile);
	m_strSoftwareVer = CPublicFun::ReadStringFromFile(pFile);

	return TRUE;
}

BOOL CHardwareInfo::WriteScanChannelInfoToFile(CFile* pFile,BOOL bTemplate)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	if(bTemplate)
	{
		return TRUE;
	}

	int iCount = m_lnkScanChanInfo.GetSize();
	pFile->Write((const char *)&iCount,sizeof(int));

	for(int i = 0;i < iCount;++i)
	{
		tagScanChannelInfo& scanChanInfo = *m_lnkScanChanInfo[i];
		pFile->Write((const char *)&scanChanInfo,sizeof(tagScanChannelInfo));
	}

	return TRUE;
}

BOOL CHardwareInfo::ReadScanChannelInfoFromFile(CFile* pFile,WORD wVersion,BOOL bTemplate)
{
	if(NULL == pFile)
	{
		return FALSE;
	}

	if(bTemplate)
	{
		return TRUE;
	}

	m_lnkScanChanInfo.Clear();

	int iCount = 0;
	pFile->Read((char*)&iCount,sizeof(int));

	tagScanChannelInfo scanChanInfo;
	for(int i = 0;i < iCount;++i)
	{
		pFile->Read((char*)&scanChanInfo,sizeof(tagScanChannelInfo));
		m_lnkScanChanInfo.AddTail(scanChanInfo);
	}

	return TRUE;
}

void CHardwareInfo::ResetHardwareInfo()
{
	m_strModel.Empty();
	m_strSN.Empty();
	m_strCode.Empty();
	m_strVersion.Empty();
	m_strSoftwareVer.Empty();
}

BOOL CHardwareInfo::GetBeEmptyHardware()
{
	return m_strModel.IsEmpty() &&
	m_strSN.IsEmpty() &&
	m_strCode.IsEmpty() &&
	m_strVersion.IsEmpty() &&
	m_strSoftwareVer.IsEmpty();
}


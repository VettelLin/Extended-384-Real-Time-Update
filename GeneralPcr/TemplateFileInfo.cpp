#include "StdAfx.h"
#include "TemplateFileInfo.h"

CTemplateFileInfo::CTemplateFileInfo(void)
{
	m_strFilePath.Empty();
	m_strFileName.Empty();
	m_strTitle.Empty();
}

CTemplateFileInfo::~CTemplateFileInfo(void)
{

}

CTemplateFileInfo::CTemplateFileInfo(const CTemplateFileInfo& info)
{
	m_strFilePath = info.m_strFilePath;
	m_strFileName = info.m_strFileName;
	m_strTitle = info.m_strTitle;
}

void CTemplateFileInfo::operator=(const CTemplateFileInfo& info)
{
	m_strFilePath = info.m_strFilePath;
	m_strFileName = info.m_strFileName;
	m_strTitle = info.m_strTitle;
}

BOOL CTemplateFileInfo::IsFileExist()
{
	CFileFind fileFind;
	return fileFind.FindFile(m_strFilePath);
}





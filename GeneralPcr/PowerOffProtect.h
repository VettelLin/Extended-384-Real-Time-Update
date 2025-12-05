#pragma once

#include "LinkList.h"
#include "RunProgressRecord.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

class CPowerOffProtect
{
public:
	CPowerOffProtect(void);
	~CPowerOffProtect(void);

			int InitPowerOffProtect(CString strPath);
			int SetRunProgress(int iPageId,int iSegId,int iStepId,int iCycleId);
			int SetRunProgress(int iPageId,float fMeltTem);
			int AddRunProgress(CRunProgressRecord& runPrgRcd);
			int DeleteRunProgress(int iPageId);
			int DeleteOldProgress(int iLnkIndex);

			int SearchPowerOffProtect(CString strSN);

			int GetRunProgressRecord(int iIndex,CRunProgressRecord& runPrjRcd);

private:
			int ReadFile();
			int SaveFile();

			int WriteString(CString& strText);
			int ReadString(ifstream& ifs,CString& strText);

public:
	CLinkList<CRunProgressRecord> m_lnkRunPrgRcd;
	ofstream m_fs;

private:
	HANDLE m_hSyncLnk;	//同步m_lnkRunPrgRcd访问
	BOOL m_bProtectWork;	//标识断电保护功能是否可用

	CString m_strFileDir;	//未完成实验记录文件所在的文件夹
	CString m_strFileName;	//未完成实验记录文件的文件名（包含后缀名）
	CString m_strFilePath;	//未完成实验记录文件的完整路径名

	int m_iFileTag;	//文件标识
};


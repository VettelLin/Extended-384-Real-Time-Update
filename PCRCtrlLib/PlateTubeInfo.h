#pragma once

#include  "TubeInfo.h"
#include <vector>

using namespace std;

const WORD wFileMarkPCRPlate = 0x6363;   // PCR孔板数据文件标识
const WORD wPCRPlateVersion100 =  0x0100;    // PCR孔板数据文件版本

class AFX_EXT_CLASS CPlateTubeInfo
{
public:
	CPlateTubeInfo(void);
	~CPlateTubeInfo(void);

// Operations
public:
	BOOL OpenFile(CString strFilePath);

	BOOL WriteFile(CFile* pFile,WORD wVersion);
	BOOL ReadFile(CFile* pFile,WORD wVersion);

	void SetMeltTmCount(int nCount);
	void SetPlateInfo(tagPlateModeInfo* pInputPara);
	void InitDyeTargetInfo(int iTubeID, CReagentItem* pReagentItem);

	void SetTubeInfo(vector<CTubeInfo>& vecTubeInfo);
	void SetTubeInfo(vector<CTubeInfo*>& vecPTubeInfo);
	int SetTubeInfo(CTubeInfo* pTotalTubeInfo,int iTubeNum);

	void GetTubeInfo(vector<CTubeInfo>& vecTubeInfo);

	void GetUsedDyeChannel(CStringArray* pArrayDye, CIntArray* pArrayChannel);
	void GetUsedTarget(CPtrArray* pArrrayTarget);

	// 孔板列标行号标签
	void GetTubeRowAndColLabel(CStringArray* pArrayRow, CStringArray* pArrayCol);

protected:
	void DeleteAll(BOOL bOnlyTube=FALSE);
	void DeleteAllTubes();
	int CalculateTubeCount(tagPlateModeInfo* pInputInfo);
	CString CreateTubeLabel(int iIndex);

	void CreateLetterLabel(CStringArray* pArray, int iCount, CString strIndex=_T(""));
	void CreateNumberLabel(CStringArray* pArray, int iCount, CString strIndex=_T(""));

// Attributes 
public:
	tagPlateModeInfo* GetPlateInfo() { return m_pPlateInfo; }
	int GetTubeCount() { return m_arrayTube.GetCount(); }
	CTubeInfo* GetTubeInfo(int iIndex);
	void GetAllTubeInfoPtr(vector<CTubeInfo*>& vecPTubeInfo);
	CPtrArray* GetTubeInfoArrayPtr();
	CTubeInfo* GetTubeInfoByID(int iTubeID);
	CTubeInfo* GetTubeInfoByLabel(CString strLabel);

protected:
	tagPlateModeInfo* m_pPlateInfo; // 孔板信息
	CPtrArray	m_arrayTube; // 孔板信息指针数组
};


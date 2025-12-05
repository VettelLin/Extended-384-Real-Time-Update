#pragma once

#include "StepItemInfo.h"
#include "LinkList.h"

class AFX_EXT_CLASS CSegItemInfo
{
	enum eSegmentType
	{
		SEGMENT_NONE = 0,
		SEGMENT_CYCLE, // 循环扩增
		SEGMENT_MELT, // 熔曲
		SEGMENT_HOLD, // 保持，ABI
	};

public:
	CSegItemInfo(void);
	~CSegItemInfo(void);

	CSegItemInfo(const CSegItemInfo& segItemInfo);
	void operator=(const CSegItemInfo& segItemInfo);

	BOOL WriteFile(CFile* pFile);
	BOOL ReadFile(CFile* pFile, WORD wVersion);

	int GetSegWidth(int stepId = -2);	//stepId默认值不能是-1，因为0 - 1 = -1；
	BOOL GetBeMeltSeg();
	int GetMeltIncrement(double& dMeltIncrement);
	BOOL GetBeAmlSeg();

	BOOL GetBeAllStepSelected();
	BOOL GetBeHaveSelectedStep();

	float GetFirstStepTemperature();
	float GetLastStepTemperatre();

	int GetStepNum();

	void CorrectNameLanguage(CString strSource, CString strTarget);
	void CorrectIndex(int iIndex);

	long CalculateTotalSeconds();

//数据属性
public:
	int m_iStepNum;       ///< 步骤中子项的个数
	int	m_iSegType;        ///< 步骤类型 1--none,2--扩增,3--熔曲
	int m_iCycleNum;     ///< 循环次数
	CString m_strSegName;    ///< 步骤名称
	float m_fMeltRate;			//记录融解段升降温速率

	BOOL m_bSelected;	//标识程序段是否被选中

	eSegmentType m_nSegType; // 步骤类型

	CRect m_rcCycleNum;	//记录循环数区域（窗口区域，非内存区域）
	CRect m_rcSegName;	//记录程序段名称区域（窗口区域，非内存区域）
	CRect m_rcSeg;	//记录程序段区域（窗口区域，非内存区域）

	CLinkList<CStepItemInfo> m_lnkStepItemInfo;	///<保存step信息
};


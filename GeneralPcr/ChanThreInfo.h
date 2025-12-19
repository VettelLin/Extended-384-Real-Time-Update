#pragma once

#include "ThreLineInfo.h"

//曲线显示类型（线性曲线，对数曲线）
typedef enum LineShowFormat
{
	ELSF_LINEAR,ELSF_LOG
}ELINESHOWFORMAT;

//曲线数据类型（原始曲线、归一化曲线、DeltaRn曲线）
typedef enum LineDataType{
	ELDT_NORMALIZE,ELDT_DELTARN
}ELINEDATATYPE;

class AFX_EXT_CLASS CChanThreInfo
{
public:
	CChanThreInfo(void);
	~CChanThreInfo(void);

	CChanThreInfo(const CChanThreInfo& info);
	void operator=(const CChanThreInfo& info);
	void operator=(CChanThreInfo& info);

	CThreLineInfo* GetCurSelThreInfo();
	CThreLineInfo* GetCurSelAssociatedThreInfo();	//获取当前选中阈值线关联阈值信息（线性阈值线和对数阈值线互为关联）

	void SetBeAutoThre(BOOL bAutoThre);
	
public:
	CThreLineInfo m_tliNormolize;
	CThreLineInfo m_tliDeltaRn;
	CThreLineInfo m_tliLogNormolize;
	CThreLineInfo m_tliLogDeltaRn;
	
	ELINEDATATYPE m_eLineDataType;
	ELINESHOWFORMAT m_eLineShowFormat;

	int m_iChanId;
	CString m_strChanName;
	CString m_strTargetName;
};


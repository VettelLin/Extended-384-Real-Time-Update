#pragma once

/*该类用于记录实验文件的状态信息，切换实验时更新软件主页面相应控件的状态和内容,20220712,孙文郎*/

#include "DeclaresPCR.h"

//运行图标状态（可运行）、停止图标状态、运行图标禁用状态、停止图标禁用状态、隐藏状态
typedef enum RunBtState{
	ERBS_RUN,ERBS_STOP,ERBS_RUNFORBIDDEN,ERBS_STOPFORBIDDEN,ERBS_HIDE
}ERUNBTSTATE;

//进仓图标状态（可进仓）、出仓图标状态、进仓图标禁用状态、出仓图标禁用状态、隐藏状态
typedef enum BinBtState{
	EBBS_IN,EBBS_OUT,EBBS_INFORBIDDEN,EBBS_OUTFORBIDDEN,EBBS_HIDE
}EBINBTSTATE;

class AFX_EXT_CLASS CExpStateInfo
{
public:
	CExpStateInfo(void);
	~CExpStateInfo(void);

	CExpStateInfo(const CExpStateInfo& info);
	void operator=(const CExpStateInfo& info);

	CString GetRemainTimeStr();
	CString GetCurrentTemperatureStr();

	void Clear();

public:
	CString m_strExpName;	//实验名称
	int m_iRemainSec;	//剩余时间
	double m_dCurTem;	//当前温度
	double m_dCurProgress;	//当前进度
	eExperimentType m_eExpType;	//实验类型

	ERUNBTSTATE m_eRunBtState;	//运行按钮状态
	EBINBTSTATE m_eBinBtState;	//进出仓按钮状态
};


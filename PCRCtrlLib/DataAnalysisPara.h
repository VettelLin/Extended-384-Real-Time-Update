#pragma once

#include "DeclaresPCR.h"

// 数据分析参数类，包括扩增参数、熔解参数、结果分析参数、结果判定参数等
class AFX_EXT_CLASS CDataAnalysisPara
{
public:
	CDataAnalysisPara(void);
	~CDataAnalysisPara(void);

public:
	BOOL WriteFile(CFile* pFile,WORD wVersion,BOOL bProject);
	BOOL ReadFile(CFile* pFile, WORD wVersion,BOOL bProject);
	void CopyFrom(CDataAnalysisPara* pAnalysis);

	BOOL IsNewAmplifyPara(CString strStartupPath, tagAnaAmplifyPara* pInputPara);

	void Clear();

protected:
	void DeleteAll();
	void DeleteAmplifyPara();
	void DeleteMeltCurvePara();
	void DeleteInspectScope();
	void DeleteBasicRule();

	BOOL WriteAmplifyPara(CFile* pFile,WORD wVersion,BOOL bProject);
	BOOL WriteMeltCurvePara(CFile* pFile);
	BOOL WriteInspectScope(CFile* pFile);
	BOOL WriteResultRule(CFile* pFile);

	BOOL ReadAmplifyPara(CFile* pFile, WORD wVersion,BOOL bProject);
	BOOL ReadMeltCurvePara(CFile* pFile, WORD wVersion);
	BOOL ReadInspectScope(CFile* pFile, WORD wVersion);
	BOOL ReadResultRule(CFile* pFile, WORD wVersion);

	void ReadAmplifyParaFromIniFile(tagAnaAmplifyPara* pInputPara, CString strStartupPath);

// Attributes
public:
	tagAnaAmplifyPara* GetAmplifyParaBy(CString strTarget, CString strDye, int iChannel); 
	tagAnaMeltCurvePara* GetMeltCurveParaBy(CString strTarget); 
	tagAnaInspectScope* GetInspectScopeBy(CString strTarget); 
	tagAnaBasicRule* GetBasicRuleBy(CString strTarget); 

	tagAnaAmplifyPara* GetAmplifyParaBy(int nIndex); 

	// 指标分析参数相关参数
	int SetAmplifyParaCount(int iCount);
	int GetAmplifyParaCount() { return m_arrayAmplifyPara.GetCount(); }
	void AddAmplifyPara(tagAnaAmplifyPara* pAnalysis) { m_arrayAmplifyPara.Add(pAnalysis); } // 增加一种指标参数，需要先申请指针
	void SetAmplifyParaAt(int nIndex, tagAnaAmplifyPara* pAnalysis); // 修改参数，不需要申请指针
	tagAnaAmplifyPara* GetAmplifyParaByTarget(CString strTarget, CString strDye); 
	tagAnaAmplifyPara* GetAmplifyParaByDye(CString strDye);
	void DeleteOneAmplifyPara(int iIndex);

	// 熔解曲线相关参数
	int SetMeltCurveParaCount(int iCount);
	int GetMeltCurveParaCount() { return m_arrayMeltCurvePara.GetCount(); }
	void AddMeltCurvePara(tagAnaMeltCurvePara* pMeltCurve) { m_arrayMeltCurvePara.Add(pMeltCurve); } // 增加一种指标参数，需要先申请指针
	tagAnaMeltCurvePara* GetMeltCurveParaBy(int nIndex); 
	void SetMeltCurveParaAt(int nIndex, tagAnaMeltCurvePara* pMeltCurve); // 修改参数，不需要申请指针

	// 目标定量检测范围参数相关
	int GetInspectScopeCount() { return m_arrayInspectScope.GetCount(); }
	void AddInspectScope(tagAnaInspectScope* pInspectScope) { m_arrayInspectScope.Add(pInspectScope); } // 增加一种指标参数，需要先申请指针
	tagAnaInspectScope* GetInspectScopeBy(int nIndex); 
	void SetInspectScopeAt(int nIndex, tagAnaInspectScope* pInspectScope); // 修改参数，不需要申请指针

	// 结果判定规则参数相关
	BOOL GetUseRule() { return m_bUseRule; }
	void SetUseRule(BOOL bUse) { m_bUseRule = bUse; }
	int GetBasicRuleCount() { return m_arrayBasicRule.GetCount(); }
	void AddBasicRule(tagAnaBasicRule* pBasicRule) { m_arrayBasicRule.Add(pBasicRule); } // 增加一种指标参数，需要先申请指针
	tagAnaBasicRule* GetBasicRuleBy(int nIndex); 
	void SetBasicRule(int nIndex, tagAnaBasicRule* pBasicRule); // 修改参数，不需要申请指针
	
	CPtrArray* GetArrayAmplifyParaPtr(){return &m_arrayAmplifyPara;}
	CPtrArray* GetArrayMeltCurveParaPtr(){return &m_arrayMeltCurvePara;}

protected:
	CPtrArray	m_arrayAmplifyPara; // 目标分析参数 tagAnaAmplifyPara
	CPtrArray	m_arrayMeltCurvePara; // 熔解曲线参数 tagAnaMeltCurvePara
	CPtrArray	m_arrayInspectScope; // 目标定量检测范围 tagAnaInspectScope
	BOOL		m_bUseRule; // 是否使用判定规则
	CPtrArray	m_arrayBasicRule; // 基本判定规则 tagAnaBasicRule
};


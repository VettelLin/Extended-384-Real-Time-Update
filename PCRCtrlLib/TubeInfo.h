#pragma once
#include "SampleInfo.h"
#include "DeclaresPCR.h"
#include "DyeTargetInfo.h"

//删除样本信息时的删除类型（删除所有、删除孔板编辑页面内容（染料名称和颜色除外、删除病人信息（包括样本类型）））
typedef enum DeleteType{
	EDT_DELETEALL,EDT_DELETE1,EDT_DELETE2
}EDELETETYPE;

//样本ID相同的孔位视为同一个样本，报结果时多个孔位只报一个结果
typedef enum SampleTotalResult{
	ESTR_NEGATIVE,ESTR_INVALID,ESTR_NORMAL
}ESAMPLETOTALRESULT;

class AFX_EXT_CLASS CTubeInfo : public CSampleInfo
{
public:
	CTubeInfo();
	CTubeInfo(int nID);
	~CTubeInfo(void);

	CTubeInfo(const CTubeInfo& tubeInfo);
	void operator=(const CTubeInfo& tubeInfo);

	BOOL WriteFile(CFile* pFile,WORD wVersion);
	BOOL ReadFile(CFile* pFile,WORD wVersion);

	BOOL HasUsed() { return m_nSampleType != SAMPLE_TYPE_NONE; }
	void InitDyeChannel(int iChannelCount); // 初始化染料通道
	void PasteTargetInfoFromItem(CReagentItem* pItem); // 复制项目中定义的染料信息
	void PerfectTargetDyeInfo(CPtrArray* pArrayChannel);

protected:
	void DeleteAllDyeTargetInfo();

public:
	void SetTmCount(int nCount);
	int GetTubeID() { return m_iTubeID; }
	CString GetTubeLabel(int iColNum);
	void SetTubeLabel(CString string) { m_strTubeLabel = string; }
	CString GetTubeFlag() { return m_strTubeFlag; }
	void SetTubeFlag(CString string) { m_strTubeFlag = string; }
	CString GetSampleID() { return m_strSampleID; }
	void SetSampleID(CString string) { m_strSampleID = string; }
	CString GetSampleName() { return m_strSampleName; }
	void SetSampleName(CString string) { m_strSampleName = string; }
	COLORREF GetSampleColor() { return m_clrSample; }
	void SetSampleColor(COLORREF color) { m_clrSample = color; }

	eSampleType GetSampleType() { return m_nSampleType; }
	void SetSampleType(eSampleType nType) { m_nSampleType = nType; }
	BOOL GetIsRepeatTube() { return m_bRepeatTube; }
	void SetIsRepeatTube(BOOL bSet) { m_bRepeatTube = bSet; }
	
	int GetRepeatNo() { return m_iRepeatNo; }
	CString GetRepeatNoString();

	void SetRepeatNo(int iNo) { m_iRepeatNo = iNo; }
	CString GetComments() { return m_strComments; }
	void SetComments(CString string) { m_strComments = string;}

	int GetTargetCount() { return m_arrayDyeTarget.GetCount(); }
	CDyeTargetInfo* GetTargetInfo(int iIndex);

	void AddDyeTargetInfo(CDyeTargetInfo* pDyeInfo) { m_arrayDyeTarget.Add(pDyeInfo); }
	CDyeTargetInfo* FindTargetInfoByTarget(CString strTarget);
	CDyeTargetInfo* FindTargetInfoByChannel(int iChannel);

	void ResetDyeInfo();	//重置（清空）染料信息，不会释放指针
	void ResetDyeTargetInfoExceptDyeNameAndColor();	//重置（清空）染料信息，除了染料名和颜色，

	CString GetDyeName(int iIndex);
	void SetDyeName(int iIndex,CString strDyeName);

	CString GetTargetName(int iIndex);
	void SetTargetName(int iIndex,CString strTarget);

	int GetDyeTargetInfo(int iIndex,CString& strDyeName,CString strTargetName);

	void SetCalibrateChannel(int iChannel) { m_iCalibrateChannel = iChannel; }
	int GetCalibrateChannel()  { return m_iCalibrateChannel; }

	BOOL GetBeNeedDensity();	//获取是否需要设置样本浓度值（非反算浓度值）
	BOOL GetBeNoneSampleType();	//判断样本类型是否为空
	BOOL GetBeUnknown();	//判断是否是待测样本
	BOOL GetBePositive();	//判断样本类型是否是阳性对照
	BOOL GetBeNegtive();	//判断样本类型是否是阴性对照

	COLORREF GetSampleDyeTargetColor(int iChanIndex);
	void SetSampleDyeTargetColor(int iChanIndex,COLORREF clrDyeTarget);

	BOOL GetSampleChannelSelectState(int iChanIndex);
	void SetSampleChannelSelectState(int iChanIndex,BOOL bSelectState);

	double GetFirstNonZeroDensity();	//获取第一个非零（标准品、校准品）浓度值
	CString GetFirstNonZeroDensityString(int iDPd = 4);	//iDPd--小数点位数
	CString GetAllChannelDensityString();	//获取所有通道的浓度字符串，用换行符分隔

	double GetDensity(int iChanIndex);	//获取特定通道（标准品、校准品）浓度值
	CString GetDensityString(int iChanIndex,int iDPd = 4);	//iDPd--小数点位数

	double GetCalculatedDensity(int iChanIndex);	//获取特定通道的反算浓度值
	CString GetCalculatedDensityString(int iChanIndex,int iDPd = 4);	//iDPd--小数点位数

	BOOL ResetChannelCalculatedDensity(int iChanIndex);	//重置通道反算的浓度值，设置为-1

	virtual void Reset(BOOL bTemplateMode);

	void ResetInfo1(BOOL bTemplateMode);	//重置孔板编辑-通道编辑页面可编辑的样本信息
	void ResetInfo2(BOOL bTemplateMode);	//重置样本登记页面可编辑的样本信息

	int GetChannelNum();
	
	BOOL IsAllSelectedChannelPositive();	//判断所有选中的通道是否都是阳性（正常起峰）
	BOOL IsAllSelectedChannelNegative();	//判断所有选中的通道是否都是阴性（未起峰）

	ESAMPLETOTALRESULT GetSampleTotalResultIfIncludeIC();	//如果样本孔位包含IC基因，返回该样本多通道组合判定结果

	BOOL IsExistInternalControl();	//判断管内是否存在内标基因
	BOOL IsExistInternalControl(BOOL& bICPositive);

	BOOL IsExistPositiveExceptICGene();	//判断除了内标基因，是否包含其它阳性基因

	static BOOL IsInternalControl(CString& str);	//判断是否是内标基因
	static BOOL IsInternalControlSPC(CString strSPC);

	void SetCycleOfMaxMOTRatio(int iCycle) { m_iCycleOfMaxMOTRatio = iCycle; }
	int GetCycleOfMaxMOTRatio() { return m_iCycleOfMaxMOTRatio; }

protected:
	int			m_iTubeID; // 管序号，从1开始，与按行或按列排列有关  
	CString		m_strTubeLabel; // 孔位标志，如A1,B1
	CString		m_strTubeFlag; // 管名，与Target相对应，单人份所含试管数
	CString		m_strSampleID;  // 样本ID
	CString		m_strSampleName; // 样本名称
	COLORREF	m_clrSample; // 样本颜色

	eSampleType m_nSampleType; // 样本类型
	BOOL		m_bRepeatTube; // 是否为复管
	int			m_iRepeatNo; // 复管编号
	CString		m_strComments; // 注释

	CPtrArray	m_arrayDyeTarget; // 类CDyeTargetInfo 指针，染料通道、浓度、计算结果Ct、Tm等

	// 传递校准通道，不需要保存
	int		m_iCalibrateChannel;

	int		m_iCycleOfMaxMOTRatio; // 浊度调整时最大下降比率时的循环数，方便调整荧光数据，-1表示没有找到
};
















#pragma once

#include "DeclaresPCR.h"
#include <fstream>
#include "SegItemInfo.h"
#include "LinkList.h"
#include "DataAnalysisPara.h"
#include "ConfigureDBInfo.h"


using namespace std;

// 通道染料目标设置
class AFX_EXT_CLASS CItemChannelInfo
{
public:
	CItemChannelInfo(void);
	~CItemChannelInfo(void);

	struct tagItemTargetInfo
	{
		CString strTubeFlag; // 管标记
		CString strTarget; // 指标名称
		COLORREF clrTarget; //  颜色
	};

public:
	BOOL WriteFile(CFile* pFile);
	BOOL ReadFile(CFile* pFile, WORD wVersion,BOOL bProject);

	void CopyFrom(CItemChannelInfo* pInputInfo);

protected:
	void DeleteAll();

// Attributes
public:
	BOOL GetUse() { return m_bUse; }
	void SetUse(BOOL bUse) { m_bUse = bUse; }
	int GetChannel() { return m_nChannel; }
	void SetChannel(int nChannel) { m_nChannel = nChannel; }
	CString GetDyeName() { return m_strDye; }
	void SetDyeName(CString string) { m_strDye = string; }
	int GetExWavelength() { return m_nExWavelength; }
	void SetExWavelength(int iLength) { m_nExWavelength = iLength; }
	int GetEmWavelength() { return m_nEmWavelength; }
	void SetEmWavelength(int iLength) { m_nEmWavelength = iLength; }

	void AddTarget(CString strTubeFlag, CString strName, COLORREF color);
	void AddTarget(CString strTargetName);
	int GetTargetCount() { return m_arrayTarget.GetCount(); }
	CString GetTubeFlag(int iIndex);
	void SetTubeFlag(int iIndex, CString strFlag);
	CString GetTargetName(int iIndex);
	CString GetTargetName(CString strTubeFlag);
	void SetTargetName(int iIndex, CString string);
	COLORREF GetTargetColor(int iIndex);
	COLORREF GetTargetColor(CString strTubeFlag);
	void SetTargetColor(int iIndex, COLORREF color);

	//double GetNegPosUpLimit();
	//double GetNegPosDownLimit();
	//void SetNegPosUpLimit(double dValue);
	//void SetNegPosDownLimit(double dValue);

protected:
	BOOL m_bUse; // 通道是否使用
	int	 m_nChannel; // 通道
	CString m_strDye; // 染料名称
	int m_nExWavelength; // 激发波长
	int m_nEmWavelength; // 发射波长
	CPtrArray m_arrayTarget; // 指标和对应的颜色，tagItemTargetInfo
	
	double m_dNegPosUpLimit;	//阴阳性判定阈值上限，20220609，孙文郎，临时使用(需要永远保存在文件中，即使后续不再使用该参数)，最终要使用用户自定义的判定规则
	double m_dNegPosDownLimit;	//阴阳性判定阈值下限，20220609，孙文郎，临时使用(需要永远保存在文件中，即使后续不再使用该参数)，最终要使用用户自定义的判定规则
};

// 试剂检测项目
class AFX_EXT_CLASS CReagentItem
{
public:
	CReagentItem(void);
	~CReagentItem(void);

	// 相同样本的试管排列方式
	enum eTubeSortMode
	{
		SORT_MODE_VERT = 0, // 竖排
		SORT_MODE_HORI, // 横排
	};

	enum eSaveStatus
	{
		STATUS_NEW = 0, // 新建
		STATUS_SAVE, // 保存
		STATUS_CHANGE, // 修改
		STATUS_USING, // 正在使用
	};

public:
	BOOL SaveFile(CString strFileName);
	BOOL OpenFile(CString strFileName);

	BOOL WriteFile(CFile* pFile,WORD wVersion,BOOL bProject);
	BOOL ReadFile(CFile* pFile, WORD wVersion,BOOL bProject);

	void CopyFrom(CReagentItem* pInputItem);
	void CopyChanTargetInfo(CReagentItem* pInputItem);

	int GetPreStepDelay(int iCurSegment, int iCurStep) { return 60; } // 取得指定程序段和步骤的前面一步的等待时间

	void InitChannelInfo(CDyeInfoList* pListDye, CPtrArray* pArrayChannel); // 初始化可用光路
	void InitReagentItem(CDyeInfoList* pListDye, CPtrArray* pArrayChannel);

	BOOL IsLegal(int &iOutTip); // 判断编辑是否合法

	BOOL GetBeEditable();	//获取“项目信息是否可编辑”

	CLinkList<CString>& GetLnkDyeName();	//获取所有的通道（染料）名称
	int DyeNameChanged(int iChanId,CString strDye);	//项目的某个通道染料名称改变了

protected:
	virtual void DeleteAll();
	void DeleteAllChannelTarget();

	BOOL WriteBasicInfo(CFile* pFile);
	BOOL WriteChannelTarget(CFile* pFile,WORD wVersion,BOOL bProject);
	BOOL WriteSegment(CFile* pFile);

	BOOL ReadBasicInfo(CFile* pFile, WORD wVersion);
	BOOL ReadChannelTarget(CFile* pFile, WORD wVersion,BOOL bProject);
	BOOL ReadSegment(CFile* pFile, WORD wVersion);

public:
	CString GetName() { return m_strName; }
	void SetName(CString string) { m_strName = string; }
	eExperimentType GetType() { return m_nType; }
	void SetType(eExperimentType nType) { m_nType = nType; }
	CString GetBarcode() { return m_strBarcode; }
	void SetBarcode(CString string) { m_strBarcode = string; }
	CString GetReagent() { return m_strReagent; }
	void SetReagent(CString string) { m_strReagent = string; }

	int GetReactVolume() { return m_iReactVolume; }
	void SetReactVolume(int nVolume) { m_iReactVolume = nVolume; }
	CString GetUnit() { return m_strUnit; }
	void SetUnit(CString string) { m_strUnit = string; }
	CString GetComments() { return m_strComments; }
	void SetComments(CString string) { m_strComments = string; }
	CString GetUser() { return m_strUser; }
	void SetUser(CString string) { m_strUser = string; }
	CString GetDate() { return m_strDate; }

	int GetTubeNumPerSample() { return m_iTubeNumPerSample; }
	void SetTubeNumPerSample(int nCount) { m_iTubeNumPerSample = nCount; }
	eTubeSortMode GetTubeSortMode() { return m_nSortMode; }
	void SetTubeSortMode(eTubeSortMode nMode) { m_nSortMode = nMode; }

	// 通道染料目标
	int GetChannelTargetCount() { return m_arrayTarget.GetCount(); }
	void AddChannelTarget(CItemChannelInfo* pChannel) { m_arrayTarget.Add(pChannel); } // 增加一种指标参数，需要先申请指针
	CItemChannelInfo* GetChannelTargetBy(int nIndex); 
	void SetChannelTargetAt(int nIndex, CItemChannelInfo* pChannel); // 修改参数，不需要申请指针
	CItemChannelInfo* GetChannelTargetBy(CString strName); 


	// 交叉干扰相关函数
	int GetSelChannelCount() { return m_nSelChanCount; }
	void SetSelChannelCount(int nCount);
	double* GetCrosstalkPara() { return m_pCrosstalkPara; }
	void GetUsedChannel(CIntArray* pArrayChanel);

	// 实验流程相关参数
	int GetSegementCount() { return m_lnkSegment.GetSize(); }
	void AddSegment(CSegItemInfo segItem) { m_lnkSegment.AddTail(segItem); } // 增加一种指标参数，需要先申请指针
	CSegItemInfo GetSegmentBy(int nIndex); 
	void SetSegment(CLinkList<CSegItemInfo>& lnkSegItem);
	void SetSegmentAt(int nIndex, CSegItemInfo segItem); // 修改参数，不需要申请指针
	void DeleteSegmentAt(int nIndex);
	void DeleteAllSegement();
	CLinkList<CSegItemInfo>& GetSegment() { return m_lnkSegment; }
	void SetSegmentInfo(CLinkList<CSegItemInfo>& lnkSegItemInfo) { m_lnkSegment = lnkSegItemInfo; }

	CDataAnalysisPara* GetAnalysisPara() { return m_pAnalysisPara; }

	eSaveStatus GetStatus() { return m_nStatus; }
	void SetStatus(eSaveStatus nStatus);

protected:
	CString		m_strName; // 项目名称
	eExperimentType	m_nType; // 项目类型
	CString		m_strBarcode; // 试剂条码
	CString		m_strReagent; // Chemistry/试剂
	int			m_iReactVolume; // 反应体系
	CString		m_strUnit; // 单位
	CString		m_strComments; // 注释
	CString		m_strUser; // 用户
	CString		m_strDate; // 编辑时间

	// 检测目标
	int				m_iTubeNumPerSample; // 每个样本所含试管数
	eTubeSortMode	m_nSortMode;  // 试管排列方式
	CPtrArray		m_arrayTarget; // 通道染料目标信息, CItemChannelInfo

	int			m_nSelChanCount; // 选择使用的通道数
	double*		m_pCrosstalkPara; // 通道交叉干扰参数
	CLinkList<CString> m_lnkDyeName;	//记录所有的通道（染料）名称

	CLinkList<CSegItemInfo> m_lnkSegment;	//实验步骤信息

	CDataAnalysisPara* m_pAnalysisPara; // 分析参数

	eSaveStatus 	m_nStatus; // 项目文件状态
};




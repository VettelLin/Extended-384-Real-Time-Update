
#pragma once

#include "SetInfo.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "LisSetInfo.h"
#include "LinkList.h"
#include "DeclaresPCR.h"
#include "ExperimentData.h"
#include "ConfigureDBInfo.h"
#include "TemplateFileInfo.h"
#include "PowerOffProtect.h"
#include "Ps96IvdSet.h"
#include "CYInfo.h"
#include "SamInfoExportMode.h"
#include "DeclaresPCR.h"

#include <map>

using namespace std;

class CGeneralPcrApp : public CWinApp
{
public:
	CGeneralPcrApp();
	~CGeneralPcrApp();

// Overrides
public:
	virtual BOOL InitInstance();
	int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	void InitRatioParam();

	void InitMapSampleTypeToCbIndex();	//初始化样本类型在下拉选择框中的下标信息
	int GetSampleTypeCbIndex(eSampleType eType);	//获取样本类型在下拉选择框中的下标信息

	int ReadParam();	//从文件中读取参数信息
	int WriteParam();	//保存参数信息到文件中

	int InitLanguageSet();	//从文件中读取语言设置
	int WriteLanguageSet();	//保存语言设置到文件中

	int ReadCYInfo();
	int WriteCYInfo();

	int ReadPs96IvdSet();	//从文件中读取PS96 IVD模式设置
	int WritePs96IvdSet();	//保存PS96 IVD模式设置到文件中

	int ReadSystemAnalyseParam();	
	int WriteSystemAnalyseParam();

	int ReadDefaultGeneName();	
	int WriteDefaultGeneName();

	BOOL GetBeIVDUser();	//判断用户类型是否是IVD用户

	BOOL GetBeEnglish();	//判断当前软件语言是否为英文

	void SetLanguage(eLanguageUI eLang,BOOL bSaveToFile = TRUE);

	BOOL GetBeAdminPermission();	//判断当前用户是否是管理员用户
	BOOL GetBeOrdinaryPermission();	//判断当前用户是否是普通用户

	int InitTemplateFileInfo();	//初始化模板文件列表信息
	int SaveTemplateFileInfo();	//保存模板文件列表信息

	int ReadDefaultTemplateFileInfo();	//保存默认的模板文件列表信息
	int SaveDefaultTemplateFileInfo();	//保存默认的模板文件列表信息

	int ReadUserOpenedTemplateFileInfo();	//保存用户以打开方式添加的模板文件列表信息
	int SaveUserOpenedTemplateFileInfo();	//保存用户以打开方式添加的模板文件列表信息

	int IsTemplateExist(CString strTemplateTitle);	//判断用户新打开的模板文件是否已经存在于模板文件列表中
	int IsTemplateExist(CString strTemplateTitle,int& iIndex);	//判断用户新打开的模板文件是否已经存在于模板文件列表中

	int AddNewTemplate(CTemplateFileInfo& temFileInfo);	//添加新的模板文件信息
	int AddNewTemplate(CTemplateFileInfo& temFileInfo,int& iIndex);	//添加新的模板文件信息

	int DeleteTemplate(int iIndex);

	BOOL InitilizePCR(CString strStartupPath);
	BOOL InitilizePCRStartPage(CString strStartupPath);

	void ResetTextFont();

	void InitChanNum();
	void InitDefaultDyeName();
	void InitFilterParam();
	void InitAmpCrossSetValue();
	void InitMeltCrossSetValue();

	int DeleteSystemAnaParam();

	int AddDefaultGeneName(CString strData,BOOL bSave);
	int DeleteDefaultGeneName(int iIndex, BOOL bSave=TRUE);

	void GetChannelDyeInfo();

	BOOL HasExpNameExist(CString strExpName);
	BOOL HasExpSaveFileUsed(CString strExpSavePath);
	void DeleteSavePathName(CString strPathName);
	BOOL HasExpSaveFileExist(CString strExpSavePath);

	void SaveTempalteOrder();

protected:
	BOOL FindAndInputUser(CString strStartupPath);

	void DeleteDyeInfo();

	BOOL IsTemplateFile(CString strFilePath);
	void ReadTemplateOrder(CStringArray* pArrayOrder);
	int IsUserOpenTemplateExist(CString strTempalteName);
	int IsDefaultTemplateExist(CString strTempalteName);

public:
	CRect m_rcWorkArea;	//记录工作区大小
	CRect m_rcScreen;	//记录屏幕大小

	ULONG_PTR m_gdiplusToken;

	HANDLE m_hMutex;	//确保同一台PC同一时间只能有一个实例运行

	CSetInfo m_setInfo;

	CLisSetInfo m_lisSetInfo;

	CPs96IvdSet m_ps96IvdSet; 

	int m_iFileManageDlgH;	//实验文件管理窗口高度

	CDyeInfoList m_listDyeInfo; // 系统定义的染料目标信息

	CDBUserInfo m_curUserInfo;	//记录当前登录用户的信息

	ESAMINFOEXPORTMODE m_eSamExportMode;	//样本信息导出模式,临时使用，最终要放到设置信息类里,20220508

	CLanguageInfo* m_pLangInfo;	//记录CLanguageInfo单例模式的指针，方便使用，20220512，孙文郎

	map<eSampleType,int> m_mapSamTypeToCbIndex;	//记录样本类型再下拉选择中的下标

	CLinkList<CTemplateFileInfo> m_lnkUserOpenedTemFileInfo;	//记录用户自己以打开方式添加的模板文件信息
	CLinkList<CTemplateFileInfo> m_lnkDefTemFileInfo;	//记录软件自带的模板文件信息
	CLinkList<CTemplateFileInfo> m_lnkTotalTemFileInfo;	//记录所有的模板文件信息，默认在前，用户自己以打开方式添加的在后

	BOOL m_bInitNormal;	//记录软件是否已经打开

	CPowerOffProtect m_powerOffProtect;
	CLinkList<int> m_lnkLnkIndex;	//保存需要运行的（被断电保护的）实验流程在m_powerOffProtect中的下标

	Color m_gclrSoftTheme;
	Color m_gclrSoftThemeEnd;
	Color m_gclr2LevelTheme;
	Color m_gclr3LevelTheme;
	COLORREF m_clrSoftTheme;
	COLORREF m_clr2LevelTheme;
	COLORREF m_clr3LevelTheme;
	COLORREF m_clrSoftLight;
	COLORREF m_clrDarkTx;

	LANGID m_pcLangId;	//保存软件启动时本机的语言环境

	int m_iStdDensityDpd;	//标准品浓度的小数点位数

	CCYInfo m_cyInfo;	//康研葆使用的相关信息

	//系统默认的分析参数
	vector<CString> m_vecDefDyeName;
	int m_iDefChanNum;
	float**	m_ppFCrosstalkPara; // 交叉干扰参数，扩增分析使用
	float**	m_ppFMeltCrosstalk; // 交叉干扰参数，熔曲分析使用
	vector<tagFilterItem> m_vecFlt;	//滤波参数
	vector<tagAnaAmplifyPara> m_vecAmplifyPara; //扩增分析参数
	vector<tagAnaMeltCurvePara>	m_vecMeltCurvePara;	//熔曲分析参数

	//默认基因名
	CLinkList<CString> m_lnkDefGN;

protected:
	CStringArray	m_arrayExpInitName;
	CStringArray	m_arrayExpSaveFileUsed;
};

extern CGeneralPcrApp theApp;




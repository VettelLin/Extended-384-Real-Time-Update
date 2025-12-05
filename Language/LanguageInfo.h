#pragma once

#include <map>

typedef CArray<int, int> CIntArray;

using namespace std;

enum eLanguageUI
{
	LANGUAGE_CHS = 0, // 中文
	LANGUAGE_ENG, // 英语
};

#define LANGUAGEFILEEXT	_T("xlsx")	//存储各语言文本信息的文件后缀名
#define LANGUAGEFILE_TAG	_T("7760FC7D-893B-46E1-8409-9259B9F9950C")	//存储各语言文本信息的文件标识（excel第一个单元格内容）

// 语言信息数据
class AFX_EXT_CLASS CLanguageInfo
{
public:
	CLanguageInfo(void);
	~CLanguageInfo(void);

// Operations
public:
	static CLanguageInfo* GetInstance(void);
	static void CleanUp();

	int InitLanguageText(CString strPath);	//从文件中读取各语言文本内容 
	CString GetText(int iId);	//从映射对象中查询文本信息
	CString GetText(int iId, eLanguageUI eLanguage);
	CString GetCapitalText(int iId); // 读取英文的大写拼写字符串，中文忽略
	CString GetTextWithColon(int iID);
	CString GetTextWithColon(int iId, eLanguageUI eLanguage);
	CString GetCapitalTextWithColon(int iId);

	void SetLanguage(eLanguageUI nLanguage) { m_nLanguage = nLanguage; }
	eLanguageUI GetLanguage(){return m_nLanguage;}

	BOOL GetBeEnglish();
	CString ConvertTo(CString strInput, eLanguageUI eLanguage);

protected:
	static CLanguageInfo* m_pInstance;
	CIntArray	m_arrayIndex;
	eLanguageUI	m_nLanguage; // 使用的语言
	map<int,CString> m_mapIdToChinese;	//使用映射保存中文信息
	map<int,CString> m_mapIdToEnglish;	//使用映射保存英文信息
};


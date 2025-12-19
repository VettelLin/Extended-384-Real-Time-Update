#pragma once

#include "ReagentItem.h"



// 试剂项目管理器，对多个试剂项目进行保存、读取、删除等
class AFX_EXT_CLASS CReagentItemManager
{
public:
	CReagentItemManager(void);
	~CReagentItemManager(void);
	 
// Operations
public:
	static CReagentItemManager* GetInstance();
	static void DestroyInstance();

	void Initialize(CString strUserPath, CString strStartupPath); // 读取项目目录下全部项目
	void InputItems(); // 选择项目文件导入
	void OutputItems(CString strSavePath, CStringArray* pArrayName=NULL); // 选择项目文件导出

	CReagentItem* FindItemBy(CString strName);
	void AddItem(CReagentItem* pItem) { m_arrayItems.Add(pItem); }
	BOOL DeleteItem(CString strName);
	BOOL DeleteItem(int iIndex);
	CPtrArray* GetArrayItems(){return &m_arrayItems;}
	BOOL CanDelete(int iIndex);
	CReagentItem* NewOneItem(); // 创建一个新的项目

	BOOL GetBeNewPrj(int iIndex);

protected:
	CString CreateFileNameByName(CString strPath, CString strName);
	void InitializeFromPath(CString strPath);

	void DeleteAll();

	void FindNameIndex();

// Attributes
public:
	void SetItemChanged(BOOL bChange) { m_bItemChanged = bChange; }
	BOOL GetItemChanged() { return m_bItemChanged; }

protected:
	static CReagentItemManager* m_pManager;

	CPtrArray	m_arrayItems; // 项目目录下的全部项目	
	CString		m_strProjectPath; // 项目目录
	BOOL	m_bItemChanged; // 项目信息改变，需要提示保存
	int		m_iNameIndex; // 自动命名的序号
};


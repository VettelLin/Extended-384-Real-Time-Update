#pragma once

#include "MaxMinInfo.h"
#include "LinkList.h"
#include "ArrowInfo.h"

typedef enum CellDataType{
		ECDT_INT,ECDT_DOUBLE,ECDT_BYTE,ECDT_CHAR,ECDT_PCHAR,ECDT_WCHAR,ECDT_PWCHAR,
	}ECELLDATATYPE;

class CCellInfo
{
public:
	CCellInfo(void);
	~CCellInfo(void);

	CCellInfo(const CCellInfo& info);
	void operator=(const CCellInfo& info);

	void SetItemText(CString strText);
	CString GetItemText();

	void SetItemHeight(int itemH);
	int GetItemHeight();

	void SetItemCheckState(BOOL bChecked);
	BOOL GetItemCheckState();

	void SetItemColorParam(COLORREF clr,int clrW,int clrH,int iRoundAngle);	//颜色块参数编辑
	void GetItemColorParam(COLORREF& clr,int& clrW,int& clrH,int& iRoundAngle);

	void SetItemColor(COLORREF clr);	//颜色块颜色编辑
	COLORREF GetItemColor();

	void SetColorDisabled(COLORREF clr);
	COLORREF GetColorDisabled();

	void DeleteData();

	CString GetStringFormatData();
	void SetStringFormatData(CString strFormatData);

	void SetCheckable(BOOL bCheckable);
	BOOL GetCheckable();

public:
	int m_rowId;
	int m_colId;
	DWORD m_dwTextAlign;	//文本对齐方式

	BOOL m_bEditable;	//标识单元格是否可使用编辑框编辑
	BOOL m_bRowColEqualUneditable;	//单元格行列相等时是否可编辑
	BOOL m_bCbAble;	//标识单元格是否可使用下拉框编辑
	BOOL m_bCbEditable;	//标识下拉框是否可手动输入
	BOOL m_bCbDeleteRow; // 标识下拉框是否可行删除

	CMaxMinInfo m_maxMinInfo;	//记录单元格的最大最小值信息

	BOOL m_bDisabled;	//标识单元格是否处于禁用状态

	CRect m_rcInCDc;	//单元格在内存CDC中的位置（此内存CDC宽度>=rcbody宽度）,已去除行列分割线的像素

	CArrowInfo m_arrowInfo;	//记录下拉框箭头参数信息
	CLinkList<CString> m_lnkCbStr;	//记录下拉框包含的所有选择项

	BOOL m_bUseCellLnkCbStr;	//标识“是否使用单元格自身的下拉选项信息”
	BOOL m_bEditableCb;	//标识“下拉框是否可输入信息”

	int m_iTxLeftIndent;	//显示文本时，左侧缩进
	int m_iTxRightIndent;	//显示文本时，右侧缩进

	CRect m_rcClrBlock;	//记录颜色块的位置

private:
	CString m_strText;
	int m_rowH;	//行高
	BOOL m_bChecked;	//是否是选中状态

	COLORREF m_clr;	//显示颜色块
	COLORREF m_clrDisabled;	//单元格禁用状态颜色
	int m_clrW;	//颜色块高度
	int m_clrH;	//颜色块高度
	int m_iClrBlockRoundAngle;	//颜色块圆角大小

	BOOL m_bCheckable;	//标识单元格是否是可选中状态
};


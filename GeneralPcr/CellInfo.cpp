#include "StdAfx.h"
#include "CellInfo.h"


CCellInfo::CCellInfo(void)
{
	m_strText = _T("");
	m_rowId = -1;
	m_colId = -1;
	m_rowH = 30;
	m_dwTextAlign = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	m_bChecked = FALSE;
	m_clr = RGB(0,0,0);
	m_clrW = 20;
	m_clrH = 20;
	m_iClrBlockRoundAngle = 4;

	m_bEditable = FALSE;
	m_bRowColEqualUneditable = FALSE;
	m_bCbAble = FALSE;
	m_bCbEditable = FALSE;
	m_bDisabled = FALSE;
	m_bCbDeleteRow = FALSE;

	m_clrDisabled = RGB(0XF7,0XF7,0XF7);
	m_bUseCellLnkCbStr = FALSE;
	m_bEditableCb = FALSE;
	m_iTxLeftIndent = 0;
	m_iTxRightIndent = 0;
	m_bCheckable = FALSE;
}

CCellInfo::~CCellInfo(void)
{

}

CCellInfo::CCellInfo(const CCellInfo& info)
{
	m_rowId = info.m_rowId;
	m_colId = info.m_colId;
	m_strText = info.m_strText;
	m_rowH = info.m_rowH;
	m_dwTextAlign = info.m_dwTextAlign;
	m_bChecked = info.m_bChecked;
	m_clr = info.m_clr;
	m_clrW = info.m_clrW;
	m_clrH = info.m_clrH;

	m_bEditable = info.m_bEditable;
	m_bRowColEqualUneditable = info.m_bRowColEqualUneditable;
	m_bCbAble = info.m_bCbAble;
	m_bCbEditable = info.m_bCbEditable;
	m_bDisabled = info.m_bDisabled;
	m_clrDisabled = info.m_clrDisabled;
	m_arrowInfo = info.m_arrowInfo;
	m_lnkCbStr = info.m_lnkCbStr;
	m_bUseCellLnkCbStr = info.m_bUseCellLnkCbStr;
	m_bEditableCb = info.m_bEditableCb;
	m_iTxLeftIndent = info.m_iTxLeftIndent;
	m_iTxRightIndent = info.m_iTxRightIndent;
	m_bCheckable = info.m_bCheckable;
	m_rcClrBlock = info.m_rcClrBlock;
}

void CCellInfo::operator=(const CCellInfo& info)
{
	m_rowId = info.m_rowId;
	m_colId = info.m_colId;
	m_strText = info.m_strText;
	m_rowH = info.m_rowH;
	m_dwTextAlign = info.m_dwTextAlign;
	m_bChecked = info.m_bChecked;
	m_clr = info.m_clr;
	m_clrW = info.m_clrW;
	m_clrH = info.m_clrH;
	m_bEditable = info.m_bEditable;
	m_bRowColEqualUneditable = info.m_bRowColEqualUneditable;
	m_bCbAble = info.m_bCbAble;
	m_bCbEditable = info.m_bCbEditable;
	m_bDisabled = info.m_bDisabled;
	m_clrDisabled = info.m_clrDisabled;
	m_arrowInfo = info.m_arrowInfo;
	m_lnkCbStr = info.m_lnkCbStr;
	m_bUseCellLnkCbStr = info.m_bUseCellLnkCbStr;
	m_bEditableCb = info.m_bEditableCb;
	m_iTxLeftIndent = info.m_iTxLeftIndent;
	m_iTxRightIndent = info.m_iTxRightIndent;
	m_bCheckable = info.m_bCheckable;
	m_rcClrBlock = info.m_rcClrBlock;
}

void CCellInfo::SetItemText(CString strText)
{
	m_strText = strText;
}

CString CCellInfo::GetItemText()
{
	return m_strText;
}

void CCellInfo::SetItemHeight(int itemH)
{
	m_rowH = itemH;
	return;
}

int CCellInfo::GetItemHeight()
{
	return m_rowH;
}

void CCellInfo::SetItemCheckState(BOOL bChecked)
{
	m_bChecked = bChecked;
	return;
}

BOOL CCellInfo::GetItemCheckState()
{
	return m_bChecked;
}

void CCellInfo::SetItemColorParam(COLORREF clr,int clrW,int clrH,int iRoundAngle)
{
	m_clr = clr;
	m_clrW = clrW;
	m_clrH = clrH;
	m_iClrBlockRoundAngle = iRoundAngle;
	return;
}

void CCellInfo::GetItemColorParam(COLORREF& clr,int& clrW,int& clrH,int& iRoundAngle)
{
	clr = m_clr;
	clrW = m_clrW;
	clrH = m_clrH;
	iRoundAngle = m_iClrBlockRoundAngle;
	return;
}

void CCellInfo::SetItemColor(COLORREF clr)
{
	m_clr = clr;
	return;
}

COLORREF CCellInfo::GetItemColor()
{
	return m_clr;
}

void CCellInfo::SetColorDisabled(COLORREF clr)
{
	m_clrDisabled = clr;
	return;
}

COLORREF CCellInfo::GetColorDisabled()
{
	return m_clrDisabled;
}

void CCellInfo::DeleteData()
{
	m_strText = _T("");
	m_bChecked = FALSE;
	m_clr = RGB(0,0,0);
	m_clrW = 12;
	m_clrH = 12;

	m_bEditable = FALSE;
	m_bRowColEqualUneditable = FALSE;
	m_bCbAble = FALSE;
	
	m_bDisabled = FALSE;
	m_clrDisabled = RGB(241,243,245);

	m_maxMinInfo.ClearMaxMinLimit();

	m_lnkCbStr.Clear();
	m_bUseCellLnkCbStr = FALSE;
	m_bEditableCb = FALSE;
	m_bCheckable = FALSE;

	return;
}

CString CCellInfo::GetStringFormatData()
{
	return m_strText;
}

void CCellInfo::SetStringFormatData(CString strFormatData)
{
	m_strText = strFormatData;
}

void CCellInfo::SetCheckable(BOOL bCheckable)
{
	m_bCheckable = bCheckable;
}

BOOL CCellInfo::GetCheckable()
{
	return m_bCheckable;
}


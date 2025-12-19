#include "StdAfx.h"
#include "TitleInfo.h"


CTitleInfo::CTitleInfo(void)
{
	m_strTitle = _T("");
	m_bSelected = FALSE;
	m_clrBkSlected = RGB(0XF1,0XF4,0XF9);
	m_clrBkUnselected = RGB(0XF1,0XF4,0XF9);
	m_colWidth = 60;
	m_dwTextAlign = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	m_dwTxAlign2 = DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS;
	m_bCheckFunc = FALSE;
	m_bColorFunc = FALSE;
	m_bClrEditale = FALSE;
	m_bEditable = FALSE;
	m_bDisabled = FALSE;
	m_bCbtable = FALSE;
	m_bCbEditable = FALSE;
	m_iCbTxLeftIndent = 6; 
	m_iCbTxRightIndent = 28;
	m_bTellParentCellChanged = FALSE;
	m_bMultiDensity = FALSE;
}

CTitleInfo::~CTitleInfo(void)
{

}

CTitleInfo::CTitleInfo(const CTitleInfo& info)
{
	m_strTitle = info.m_strTitle;
	m_bSelected = info.m_bSelected;
	m_colWidth = info.m_colWidth;
	m_dwTextAlign = info.m_dwTextAlign;
	m_clrBkSlected = info.m_clrBkSlected;
	m_clrBkUnselected = info.m_clrBkUnselected;
	m_bCheckFunc = info.m_bCheckFunc;
	m_bColorFunc = info.m_bColorFunc;
	m_bClrEditale = info.m_bClrEditale;
	m_bEditable = info.m_bEditable;
	m_bDisabled = info.m_bDisabled;
	m_bCbtable = info.m_bCbtable;
	m_bCbEditable = info.m_bCbEditable;
	m_maxMinInfo = info.m_maxMinInfo;
	m_lnkCbStr = info.m_lnkCbStr;
	m_arrowInfo = info.m_arrowInfo;
	m_iCbTxLeftIndent = info.m_iCbTxLeftIndent;
	m_iCbTxRightIndent = info.m_iCbTxRightIndent;
	m_bTellParentCellChanged = info.m_bTellParentCellChanged;
	return;
}

void CTitleInfo::operator=(const CTitleInfo& info)
{
	m_strTitle = info.m_strTitle;
	m_bSelected = info.m_bSelected;
	m_colWidth = info.m_colWidth;
	m_dwTextAlign = info.m_dwTextAlign;
	m_clrBkSlected = info.m_clrBkSlected;
	m_clrBkUnselected = info.m_clrBkUnselected;
	m_bCheckFunc = info.m_bCheckFunc;
	m_bColorFunc = info.m_bColorFunc;
	m_bClrEditale = info.m_bClrEditale;
	m_bEditable = info.m_bEditable;
	m_bDisabled = info.m_bDisabled;
	m_bCbtable = info.m_bCbtable;
	m_bCbEditable = info.m_bCbEditable;
	m_maxMinInfo = info.m_maxMinInfo;
	m_lnkCbStr = info.m_lnkCbStr;
	m_arrowInfo = info.m_arrowInfo;
	m_iCbTxLeftIndent = info.m_iCbTxLeftIndent;
	m_iCbTxRightIndent = info.m_iCbTxRightIndent;
	m_bTellParentCellChanged = info.m_bTellParentCellChanged;
	return;
}

void CTitleInfo::SetBeEditable(BOOL bEditable)
{
	m_bEditable = bEditable;
}

BOOL CTitleInfo::GetBeEditable()
{
	return m_bEditable;
}

void CTitleInfo::SetBeDisabled(BOOL bDisabled)
{
	m_bDisabled = bDisabled;
}

BOOL CTitleInfo::GetBeDisabled()
{
	return m_bDisabled;
}

void CTitleInfo::SetDataParam(CDataParam dtParam)
{
	m_dtParam = dtParam;
}

CDataParam CTitleInfo::GetDataParam()
{
	return m_dtParam;
}

int CTitleInfo::DataFilter(CString& strData)
{
	return m_dtParam.DataFilter(strData);
}

void CTitleInfo::SetMultiDensityState(BOOL bData)
{
	m_bMultiDensity = bData;
	return;
}

BOOL CTitleInfo::GetMultiDensityState()
{
	return m_bMultiDensity;
}

void CTitleInfo::SetTextAlignFormat2(DWORD dwData)
{
	m_dwTxAlign2 = dwData;
	return;
}

DWORD CTitleInfo::GetTextAlignFormat2()
{
	return m_dwTxAlign2;
}

// ColorComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorComboBox.h"


// CColorComboBox

IMPLEMENT_DYNAMIC(CColorComboBox, CComboBox)

CColorComboBox::CColorComboBox()
{
	m_bShowText = FALSE;
	m_iMaxItem = 256;
	m_pItemColor = new COLORREF[m_iMaxItem];
	for (int i=0; i<m_iMaxItem; i++)
	{
		m_pItemColor[i] = RGB(255, 0, 0);
	}

	m_sizeColorArea = CSize(16, 16);
	m_iColorBorder = 2;
	m_bIsDropList = FALSE;
	m_iEditHeight = 0;
}

CColorComboBox::~CColorComboBox()
{
	delete []m_pItemColor;
}


BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
END_MESSAGE_MAP()



// CColorComboBox message handlers

void CColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)
	{
		return;
	}

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SetBkMode(TRANSPARENT);
	RECT& rcItem = lpDrawItemStruct->rcItem;

	CString strText;
	CComboBox::GetLBText(lpDrawItemStruct->itemID, strText);

	// 是否处于选中状态
	BOOL bIsSelected = (lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED);
	// 是否处于焦点状态
	BOOL bIsFocused = (lpDrawItemStruct->itemAction | ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS);

	CRect rectItem(rcItem);
	// 绘制背景
	if (bIsSelected)
	{
		COLORREF oldColor = pDC->GetBkColor();
		pDC->FillSolidRect(rectItem, ::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetBkColor(oldColor);
	}
	else
	{
		//pDC->FillSolidRect(rectItem, pDC->GetBkColor());
		pDC->FillSolidRect(rectItem, RGB(255, 255,255));
	}

	CRect rectColor = rectItem;
	rectColor.right = rectColor.left + rectItem.Height();

	rectColor.left = rectColor.left + (rectColor.Width() - m_sizeColorArea.cx) / 2;
	rectColor.right = rectColor.left + m_sizeColorArea.cx;
	rectColor.top = rectColor.top + (rectColor.Height() - m_sizeColorArea.cy) / 2;
	rectColor.bottom = rectColor.top + m_sizeColorArea.cy;
	pDC->FillSolidRect(rectColor, m_pItemColor[lpDrawItemStruct->itemID]);

	if (m_bShowText && !strText.IsEmpty())
	{
		if (bIsSelected)
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		}

		CRect rectText = rectItem;
		rectText.left = rectColor.right + 5;
		pDC->DrawText(strText, rectText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
	}
}

void CColorComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CDC *pDC = GetDC();
	LPCTSTR lpszText = (LPCTSTR)lpMeasureItemStruct->itemData;
	CSize szText = pDC->GetTextExtent(lpszText);
	ReleaseDC(pDC);

	lpMeasureItemStruct->itemHeight = szText.cy > m_sizeColorArea.cy + 2 * m_iColorBorder ? szText.cy :  m_sizeColorArea.cy + 2 * m_iColorBorder;

	// 检测是否是DropList
	if (!m_bIsDropList)
	{
		DWORD dwStyle = GetStyle();
		if (DWORD(CBS_DROPDOWNLIST & dwStyle) == (DWORD)CBS_DROPDOWNLIST)
		{
			m_bIsDropList = TRUE;
		}
	}

	// 设置EditCtrl的高度
	if (m_bIsDropList && lpMeasureItemStruct->itemHeight > UINT(m_iEditHeight))
	{
		m_iEditHeight = lpMeasureItemStruct->itemHeight;
		SetItemHeight(-1, m_iEditHeight);
	}
}


int CColorComboBox::AddString(LPCTSTR lpszString, COLORREF clrItem)
{
	int iIndex = CComboBox::AddString(lpszString);
	if (iIndex >= m_iMaxItem)
	{
		DeleteString(iIndex);
		CString strTip;
		strTip.Format(_T("The max sum of strings is %d!"), m_iMaxItem);
		AfxMessageBox(strTip);
		return -1;
	}

	if (iIndex != CB_ERR && iIndex != CB_ERRSPACE)
	{
		m_pItemColor[iIndex] = clrItem;
	}

	return iIndex;

}

COLORREF CColorComboBox::GetColor(int iIndex)
{
	if (iIndex >= 0 && iIndex < m_iMaxItem)
		return m_pItemColor[iIndex];
	else
		return RGB(255, 255, 255);
}

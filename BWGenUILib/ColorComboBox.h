#pragma once


// CColorComboBox

class AFX_EXT_CLASS CColorComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CColorComboBox)

public:
	CColorComboBox();
	virtual ~CColorComboBox();

public:
	int AddString(LPCTSTR lpszString, COLORREF clrItem);
	COLORREF GetColor(int iIndex);
	void EnableShowText(BOOL bEnable) { m_bShowText = bEnable; }

protected:
	BOOL m_bShowText;

	int	m_iMaxItem;
	COLORREF*	m_pItemColor;
	CSize	m_sizeColorArea;
	int		m_iColorBorder;
	BOOL	m_bIsDropList;
	int		m_iEditHeight;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	DECLARE_MESSAGE_MAP()
};



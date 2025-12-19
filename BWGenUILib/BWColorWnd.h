#pragma once

const CString CLASSNAME_BWCOLORWND =  _T("BWCOLORWND");

// CBWColorWnd

class AFX_EXT_CLASS CBWColorWnd : public CWnd
{
	DECLARE_DYNAMIC(CBWColorWnd)

public:
	CBWColorWnd();
	CBWColorWnd(int iItem, int iSubItem, COLORREF clrInit, int iCount, COLORREF* pArrayColor);
	virtual ~CBWColorWnd();

// Attributes
public:
	void SetColor(int iCount, COLORREF* pInputColor);
	BOOL HasChanged() { return m_bChanged; }
	void SetChanged(BOOL bChanged) { m_bChanged = bChanged; }

	int GetColorCount() { return m_iColorCount; }
	COLORREF GetColor(int iIndex) { return m_pArrayColor[iIndex]; }

	void SetPopup(BOOL bEnable) { m_bPopup = bEnable; }
	COLORREF GetInitColor() { return m_clrInit; }

protected:
	int			m_iColorCount;
	COLORREF*	m_pArrayColor;
	int		m_iColCount;
	int		m_iRowCount;
	BOOL	m_bChanged;

	typedef CArray<CRect, CRect> CCRectArray;
	CCRectArray	m_arrayCellArea;

	int m_iRowClicked;
	int m_iColClicked;

	BOOL	m_bPopup;

	int m_iItem;
	int	m_iSubItem;
	COLORREF m_clrInit;

protected:
	BOOL RegisterWindowClass();

	void CalculateColAndRow(int iCount);
	void SendDisplayInfo(int iIndex = -1);

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};



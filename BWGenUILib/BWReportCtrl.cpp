// BioReportCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BWReportCtrl.h"
#include "BioMemDC.h"

const UINT IDC_LISTEDIT	= 2003;
const UINT IDC_COLOR_EDIT	= 2004;

// CBWReportCtrl

IMPLEMENT_DYNAMIC(CBWReportCtrl, CWnd)
CBWReportCtrl::CBWReportCtrl()
{
    RegisterWindowClass();
	
	m_nMsgID = 0;
	m_bHasHeader = FALSE;
	m_bAutoFilling = TRUE;
	m_iHeaderHeight = 32;
	m_iMinColWidth = 5;
	m_clrHeader = ::GetSysColor(COLOR_BTNFACE);
	
	m_iRowHeight = 32;
	m_clrBkg = RGB(255, 255, 255);
	m_sizeVirtual = CSize(0, 0);

	m_iRowCount = m_iColCount = 0;
	m_iFixedRowCount = m_iFixedColCount = 0;
	m_arrayRowHeight.RemoveAll();

	m_bIsColSizing = FALSE;
	m_iSizingColIndex = -1;
	m_lDrawingPos = -1;
	
	m_bEnableMultiSelected = FALSE;
	m_iLastSelectedRow = -1;

	m_bIsColClicked = FALSE;
	m_iColClickedIndex = -1;

	m_bEnableSort = FALSE;
	m_bEnableEdit = FALSE;

	m_pInEdit = NULL;
	m_pInComboBox = NULL;
	m_pColorWnd = NULL;
	m_iColorWndWidth = 200;
	m_iColorWndHeight = 150;

	m_bOddEvenColor = TRUE;
	m_clrOdd = m_clrBkg;
	m_clrEven = RGB(249, 249, 249);
	m_clrSelected = RGB(164,217,249);

	// 设置字体
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CString strFontName = _T("微软雅黑");
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, strFontName, strFontName.GetLength());
	lf.lfHeight = 90;
	//lf.lfWeight = FW_BOLD;
	//m_fontHeader.CreateFontIndirect(&lf);
	m_fontHeader.CreatePointFontIndirect(&lf);

	lf.lfWeight = FW_NORMAL;
	m_fontItem.CreatePointFontIndirect(&lf);
}

CBWReportCtrl::~CBWReportCtrl()
{
	RemoveAll();

	if (m_pInEdit != NULL)
	{
		delete m_pInEdit;
		m_pInEdit = NULL;
	}

	if (m_pInComboBox != NULL)
	{
		delete m_pInComboBox;
		m_pInComboBox = NULL;
	}

	if (m_pColorWnd != NULL)
	{
		delete m_pColorWnd;
		m_pColorWnd = NULL;
	}
}

void CBWReportCtrl::RemoveAll(BOOL bHasCol, BOOL bHasAttri)
{
	// 删除行数据
	for (int i=0; i<m_iRowCount; i++)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		if (pRowData != NULL)
			delete pRowData;
	}
	m_arrayAllData.RemoveAll();

	if (bHasCol)
	{
		tagReportColData* pColData = NULL;
		for(int i=0; i< m_arrayColData.GetCount(); i++)
		{
			pColData = (tagReportColData*)m_arrayColData.GetAt(i);
			if (pColData != NULL)
				delete pColData;
		}
		m_arrayColData.RemoveAll();
		m_iColCount = 0;
	}

	if (bHasAttri)
	{
		// 删除列属性数据
		for(int i=0; i<m_arrayColAttr.GetSize(); i++)
		{	
			sListColAttribute sColText = m_arrayColAttr.GetAt(i);
			if (sColText.dwParam != 0)
			{
				if (sColText.nType == LIST_ITEM_COMBO)
				{
					CStringArray* pArray = (CStringArray*)sColText.dwParam;
					delete pArray;
					sColText.dwParam = 0;

				}
				else if (sColText.nType == LIST_ITEM_COLOR)
				{
					COLORREF* pColor = (COLORREF*)sColText.dwParam;
					delete []pColor;
					sColText.dwParam = 0;
				}
			}
		}
		m_arrayColAttr.RemoveAll();
	}

	m_iRowCount = 0;
	m_iLastSelectedRow = -1;
	
	ResetScrollBars();
}

void CBWReportCtrl::RemoveRow(int nRow)
{
	CReportRowItem* pRowData = m_arrayAllData.GetAt(nRow);
	if (pRowData != NULL)
	{
		delete pRowData;
		pRowData = NULL;
	}
	
	m_arrayAllData.RemoveAt(nRow);
	m_iRowCount = (UINT)m_arrayAllData.GetSize();
	if (m_iLastSelectedRow == nRow)
	{
		m_iLastSelectedRow = -1;
	}
	else if (m_iLastSelectedRow >= m_arrayAllData.GetCount())
	{
		m_iLastSelectedRow = m_arrayAllData.GetCount();
	}

	if (m_bOddEvenColor)
	{
		COLORREF clr;
		for (int i=nRow; i<m_iRowCount; i++)
		{
			if((i + 1) % 2 == 0)
			{
				clr = m_clrEven;
			}
			else
			{
				clr = m_clrOdd;
			}
			for (int j=0; j<m_iColCount; j++)
			{
				CReportItem* pItemData = GetRowItem(i, j);
				pItemData->m_clrBkg = clr;
			}
		}
	}
	if (m_hWnd != NULL)
		Invalidate();
}


BEGIN_MESSAGE_MAP(CBWReportCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CBWReportCtrl::RegisterWindowClass()
{
	WNDCLASS wndClass;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CLASSNAME_REPORTCTRL, &wndClass)))
    {
        // otherwise we need to register a new class
        wndClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = ::DefWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
        wndClass.hInstance = hInst;
        wndClass.hIcon = NULL;
        wndClass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndClass.hbrBackground = NULL;
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = CLASSNAME_REPORTCTRL;

        if (!AfxRegisterClass(&wndClass)) {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

// CBWReportCtrl message handlers


BOOL CBWReportCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::Create(CLASSNAME_REPORTCTRL, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CBWReportCtrl::OnEraseBkgnd(CDC* pDC)
{
	
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}


void CBWReportCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectClient, rectHeader;
	GetClientRect(rectClient);

	CBioMemDC memDC(&dc, rectClient);
	
	// 填充背景
	memDC.FillSolidRect(rectClient, m_clrBkg);

	CSize sizeTopLeft = GetTopleftNonFixedIndex();
	// 绘制标题栏
	rectHeader = rectClient;
	//rectHeader.left -= 1;
	rectHeader.bottom = rectHeader.top + m_iHeaderHeight;
	DrawHeader(&memDC, rectHeader, sizeTopLeft.cx);

	// 绘制锁定行数据
	CRect rectRow = rectHeader;
	for (int i=0; i< m_iFixedRowCount; i++)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		rectRow.top = rectRow.bottom;
		rectRow.bottom = rectRow.top + m_arrayRowHeight.GetAt(i);

		DrawRow(&memDC, rectRow, pRowData, sizeTopLeft.cx, TRUE);
	}

	for (int i=sizeTopLeft.cy; i< m_iRowCount; i++)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		rectRow.top = rectRow.bottom;
		rectRow.bottom = rectRow.top + m_arrayRowHeight.GetAt(i);
		if (rectRow.top > rectClient.bottom)
			break;

		DrawRow(&memDC, rectRow, pRowData, sizeTopLeft.cx, FALSE, IsRowSelected(i));		
	}
}

/**
 * 绘制标题栏
 * \param pDC 
 * \param rectHeader
 * \param nStartCol 非锁定栏的最左边开始序号
 */
void CBWReportCtrl::DrawHeader(CDC* pDC, CRect rectHeader, UINT nStartCol)
{
	int nHorzPos = GetScrollPos32(SB_HORZ);
	long lOffset = 0;
	CRect rectItem = rectHeader; // 高度不变
	tagReportColData* pColData = NULL;
	
	// 填充区域
	pDC->FillSolidRect(rectHeader, m_clrHeader);

	// 绘制锁定的栏
	for (int i=0; i< m_iFixedColCount; i++)
	{
		pColData = (tagReportColData*)m_arrayColData.GetAt(i);
        rectItem.left = lOffset;
		rectItem.right = rectItem.left + pColData->iWidth;
		DrawColItem(pDC, rectItem, pColData, i == 0);

		lOffset += rectItem.Width();
	}

	// 绘制正常栏
	for (int i=nStartCol; i< m_iColCount; i++)
	{
		pColData = (tagReportColData*)m_arrayColData.GetAt(i);
        rectItem.left = lOffset;
		rectItem.right = rectItem.left + pColData->iWidth;
		
		if (rectItem.left > rectHeader.right)
			break;
		DrawColItem(pDC, rectItem, pColData, i == 0);

		lOffset += rectItem.Width();
	}

	// 绘制剩余部分
	rectItem.left = lOffset;
	rectItem.right = rectHeader.right;
	pDC->Draw3dRect(rectItem, RGB(0x92, 0x9c, 0xb2), RGB(0xc0, 0xc6, 0xcf));

}

void CBWReportCtrl::DrawColItem(CDC* pDC, CRect rectItem, tagReportColData* pColData, BOOL bIsFirst)
{
	CRect rectText;
	
	rectText = rectItem;
	if (pColData->bClicked)
		pDC->Draw3dRect(rectItem, RGB(0xc0, 0xc6, 0xcf), RGB(0x92, 0x9c, 0xb2));
	else
		pDC->Draw3dRect(rectItem, RGB(0x92, 0x9c, 0xb2), RGB(0xc0, 0xc6, 0xcf));
	// 绘制升降序标记
	if (pColData->nSortType != SORT_NO)
	{
		int nMargin = 5;
		int nWidth = 7;

		if (rectItem.Width() > 2 * nMargin + nWidth)
		{
			CRect rectIcon = rectItem;
			rectIcon.right = rectItem.right - nMargin;
			rectIcon.left = rectIcon.right - nWidth;
			
			DrawSortIcon(pDC, rectIcon, pColData->nSortType);

			rectText.right = rectIcon.left - 5;
		}
	}

	// 绘制文本
	CFont* pOldFont = pDC->SelectObject(&m_fontHeader);
	COLORREF clrTextOld = pDC->SetTextColor(RGB(0x2a, 0x2a, 0x2a));
	DrawText(pDC, rectText, pColData->strText, pColData->nFormat);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(clrTextOld);
}

void CBWReportCtrl::DrawRow(CDC* pDC, CRect rectRow, CReportRowItem* pRowData, UINT nStartCol, BOOL bIsLocked, BOOL bIsSelected)
{
	int nHorzPos = GetScrollPos32(SB_HORZ);
	long lOffset = 0;
	CRect rectItem = rectRow; // 高度不变
	CReportItem* pItemData = NULL;

	// 绘制选择行
	if (bIsSelected)
	{
		pDC->FillSolidRect(CRect(rectRow.left, rectRow.top+1, rectRow.right, rectRow.bottom -1), m_clrSelected);
	}

	// 绘制锁定的栏
	for (int i=0; i< m_iFixedColCount; i++)
	{
		pItemData = pRowData->GetChildAt(i);
		rectItem.left = lOffset;
		rectItem.right = rectItem.left + GetColumnWidth(i);
		DrawRowItem(pDC, i, rectItem, pItemData, TRUE, bIsSelected);
		lOffset += rectItem.Width();
	}

	// 绘制正常栏
	for (int i=nStartCol; i< m_iColCount; i++)
	{
		pItemData = pRowData->GetChildAt(i);
		rectItem.left = lOffset;
		rectItem.right = rectItem.left + GetColumnWidth(i);
		if (rectItem.left > rectRow.right)
			break;
		DrawRowItem(pDC, i, rectItem, pItemData, FALSE | bIsLocked, bIsSelected);
		lOffset += rectItem.Width();
	}
}

void CBWReportCtrl::DrawRowItem(CDC* pDC, int iRow, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected)
{
	sListColAttribute* pColAttribute = NULL;
	int iColType = GetItemTypeBy(iRow, -1, pColAttribute);
	if (iColType == LIST_ITEM_CHECK)
	{
		DrawCheckBoxItem(pDC, rectItem, pItemData, bIsLocked, bIsSelected);
	}
	else if (iColType == LIST_ITEM_COLOR)
	{
		DrawColorItem(pDC, rectItem, pItemData, bIsLocked, bIsSelected);
	}
	else if (iColType == LIST_ITEM_DELETE)
	{
		DrawDeleteItem(pDC, rectItem, pItemData, bIsLocked, bIsSelected);
	}
	else
	{
		DrawTextItem(pDC, rectItem, pItemData, bIsLocked, bIsSelected, iColType == LIST_ITEM_DISABLE || !pItemData->m_bEnable);
	}
}


void CBWReportCtrl::DrawItemFrame(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected, BOOL bDisable)
{
	if (!bIsLocked)
	{
		// 绘制右边线、下底线
		DrawLine(pDC, CPoint(rectItem.right, rectItem.top), CPoint(rectItem.right, rectItem.bottom), RGB(0xc0, 0xc6, 0xcf));
		DrawLine(pDC, CPoint(rectItem.left, rectItem.bottom), CPoint(rectItem.right, rectItem.bottom), RGB(0xc0, 0xc6, 0xcf));
		if (!bIsSelected)
		{
			CRect rectTemp = rectItem;
			rectTemp.DeflateRect(1, 1);
			if (bDisable)
			{
				pDC->FillSolidRect(rectTemp, m_clrHeader);
			}
			else
			{
				pDC->FillSolidRect(rectTemp, pItemData->m_clrBkg);
			}
		}
	}
	else
	{
		pDC->FillSolidRect(rectItem, m_clrHeader);
		if (bIsSelected)
			pDC->Draw3dRect(rectItem, RGB(0xc0, 0xc6, 0xcf), RGB(0x92, 0x9c, 0xb2));
		else
			pDC->Draw3dRect(rectItem, RGB(0x92, 0x9c, 0xb2), RGB(0xc0, 0xc6, 0xcf));
	}
}


void CBWReportCtrl::DrawTextItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected, BOOL bDisable)
{
	DrawItemFrame(pDC, rectItem, pItemData, bIsLocked, bIsSelected, bDisable);

	COLORREF clrTextOld;
	if (!bIsLocked)
	{
		if (bIsSelected)
		{
			clrTextOld = pDC->SetTextColor(RGB(255 - GetRValue(m_clrSelected), 255 - GetGValue(m_clrSelected), 255 - GetBValue(m_clrSelected)));
		}
		else// 填充颜色
		{
			clrTextOld = pDC->SetTextColor(RGB(0x3a, 0x3a, 0x3a));
		}
	}

	CFont* pOldFont = pDC->SelectObject(&m_fontItem);
	DrawText(pDC, rectItem, pItemData->m_strText, pItemData->m_nFormat);
	pDC->SelectObject(pOldFont);

	if (!bIsLocked)
	{
		pDC->SetTextColor(clrTextOld);
	}
}


void CBWReportCtrl::DrawCheckBoxItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected)
{
	DrawItemFrame(pDC, rectItem, pItemData, bIsLocked, bIsSelected);

	int iWidth = rectItem.Width() / 2;
	if (iWidth > rectItem.Height() / 2)
	{
		iWidth = rectItem.Height() / 2;
	}

	CRect rectDraw;
	rectDraw.left = rectItem.left + (rectItem.Width() - iWidth) / 2;
	rectDraw.right = rectDraw.left + iWidth;
	rectDraw.top = rectItem.top + (rectItem.Height() - iWidth) / 2;
	rectDraw.bottom = rectDraw.top + iWidth;
	rectDraw.InflateRect(1, 1);

	CPen penDraw(PS_SOLID, 1, RGB(96, 96, 96));
	CPen* pOldPen = pDC->SelectObject(&penDraw);

	pDC->RoundRect(rectDraw, CPoint(2,2));
	if (pItemData->m_iStatus == 0) // 未选中
	{
	}
	else if (pItemData->m_iStatus == 1) // 选中
	{
		CPen penLine(PS_SOLID, 3, RGB(64, 64, 64));
		pDC->SelectObject(&penLine);
		rectDraw.DeflateRect(3, 3);
		pDC->MoveTo(rectDraw.left, rectDraw.CenterPoint().y);
		pDC->LineTo(rectDraw.CenterPoint().x - 2, rectDraw.bottom - 2);
		pDC->LineTo(rectDraw.right, rectDraw.top);
	}
	else// 部分选中
	{
		CPen penLine(PS_SOLID, 3, RGB(128, 128, 128));
		pDC->SelectObject(&penLine);
		int iOffset = rectDraw.Width() / 4;
		rectDraw.DeflateRect(iOffset, iOffset);
		pDC->MoveTo(rectDraw.left, rectDraw.CenterPoint().y);
		pDC->LineTo(rectDraw.right, rectDraw.CenterPoint().y);
	}

	pDC->SelectObject( pOldPen );
}

void CBWReportCtrl::DrawColorItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected)
{
	DrawItemFrame(pDC, rectItem, pItemData, bIsLocked, bIsSelected);

	int iWidth = rectItem.Width() / 2;
	if (iWidth > rectItem.Height() / 2)
	{
		iWidth = rectItem.Height() / 2;
	}

	CRect rectDraw;
	rectDraw.left = rectItem.left + (rectItem.Width() - iWidth) / 2;
	rectDraw.right = rectDraw.left + iWidth;
	rectDraw.top = rectItem.top + (rectItem.Height() - iWidth) / 2;
	rectDraw.bottom = rectDraw.top + iWidth;
	pDC->FillSolidRect(rectDraw, pItemData->m_clrFill);
	//pDC->FillSolidRect(rectDraw, RGB(0, 255, 0));
}

void CBWReportCtrl::DrawDeleteItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected)
{
	DrawItemFrame(pDC, rectItem, pItemData, bIsLocked, bIsSelected);

	int iWidth = rectItem.Width() / 3;
	if (iWidth > rectItem.Height() / 3)
	{
		iWidth = rectItem.Height() / 3;
	}

	CRect rectDraw;
	rectDraw.left = rectItem.left + (rectItem.Width() - iWidth) / 2;
	rectDraw.right = rectDraw.left + iWidth;
	rectDraw.top = rectItem.top + (rectItem.Height() - iWidth) / 2;
	rectDraw.bottom = rectDraw.top + iWidth;

	CPen penDraw(PS_SOLID, 3, RGB(96, 96, 96));
	CPen* pOldPen = pDC->SelectObject(&penDraw);

	pDC->MoveTo(rectDraw.TopLeft());
	pDC->LineTo(rectDraw.BottomRight());
	pDC->MoveTo(rectDraw.left, rectDraw.bottom);
	pDC->LineTo(rectDraw.right, rectDraw.top);

	pDC->SelectObject( pOldPen );
}


void CBWReportCtrl::DrawText(CDC* pDC, CRect rect, CString strText, UINT nFormat)
{
	CString strDraw = GetTextWithDots(pDC, strText, rect.Width());
	rect.DeflateRect(4,1);
	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strDraw, rect, nFormat);
	pDC->SetBkMode(iOldMode);
}

void CBWReportCtrl::DrawSortIcon(CDC* pDC, CRect rectIcon, UINT nSortType)
{
	CPen penLight(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	CPen penShadow(PS_SOLID, 1, ::GetSysColor( COLOR_3DSHADOW));
	CPen* pOldPen = pDC->SelectObject(&penLight);
	int nSpace = rectIcon.Width();
	
	if( nSortType == SORT_ASCEND )
	{
		CPoint ptTop, ptLeft, ptRight;

		ptTop.y = rectIcon.top + (rectIcon.Height() - nSpace)/2 -1;
		ptLeft.x = rectIcon.left;
		ptLeft.y = ptTop.y + nSpace;
		ptTop.x = rectIcon.CenterPoint().x;
		ptRight.x = rectIcon.right;
		ptRight.y = ptLeft.y;

		// draw the arrow pointing upwards.
		pDC->MoveTo(ptTop);
		pDC->LineTo(ptRight);
		pDC->LineTo(ptLeft);
		(void)pDC->SelectObject( &penShadow );
		pDC->MoveTo(ptLeft);
		pDC->LineTo(ptTop);		
	}
	else
	{
		CPoint ptBottom, ptLeft, ptRight;

		ptLeft.y = rectIcon.top + (rectIcon.Height() - nSpace)/2;
		ptLeft.x = rectIcon.left;
		ptBottom.y = ptLeft.y + nSpace;
		ptBottom.x = rectIcon.CenterPoint().x;
		ptRight.x = rectIcon.right;
		ptRight.y = ptLeft.y;
		// draw the arrow pointing downwards.
		pDC->MoveTo(ptRight);
		pDC->LineTo(ptBottom);
		(void)pDC->SelectObject( &penShadow );
		pDC->MoveTo(ptBottom);
		pDC->LineTo(ptLeft);
		pDC->LineTo(ptRight);		
	}

	// restore the pen.
	(void)pDC->SelectObject( pOldPen );
}

/**
 * 得到超长文本的简化形式
 * \param pDC 设备上下文指针
 * \param strText 输入文本
 * \param nWidth 限制宽度
 * \return 限制后修改文本
 */
CString CBWReportCtrl::GetTextWithDots(CDC* pDC, CString strText , int nWidth)
{
	CString strReturn = strText;
    BOOL bNeedDots = FALSE;
    int nMaxWidth = nWidth - 4;

    while ((strReturn.GetLength()>0) && (pDC->GetTextExtent(strReturn).cx > (nMaxWidth - 4))) 
	{
        strReturn = strReturn.Left(strReturn.GetLength() - 1);
        bNeedDots = TRUE;
    }

    if (bNeedDots) 
	{
        if (strReturn.GetLength () >= 1)
		{
			if (strReturn.GetLength() % 2 == 1)
				strReturn = strReturn.Left(strReturn.GetLength() - 1);
			else
				strReturn = strReturn.Left(strReturn.GetLength() - 2);
		}
        strReturn += "...";
    }

	return strReturn;
}

int CBWReportCtrl::AddColumn(CString strText, int nWidth)
{
	return AddColumn(strText, nWidth, DATA_TEXT, DT_VCENTER | DT_SINGLELINE | DT_CENTER, -1);
}

int CBWReportCtrl::AddColumn(CString strText, int nWidth, UINT nType, UINT nFormat, int iImage)
{
	m_bHasHeader = TRUE;
	tagReportColData* pColData = new tagReportColData;
	pColData->nType = nType;
	pColData->nFormat = nFormat;
	pColData->strText = strText;
	pColData->iWidth = nWidth;
	pColData->iImage = iImage;

	int nIndex = (int)m_arrayColData.Add(pColData);
	m_iColCount = (UINT)m_arrayColData.GetCount();
	//ResetScrollBars();

	return nIndex;
}

void CBWReportCtrl::SetColumnFormat(int nIndex, UINT nFormat)
{
	if (nIndex < 0 || nIndex >= (int)m_iColCount)
		return;
	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nIndex);
	pColData->nFormat = nFormat;
}

UINT CBWReportCtrl::GetColumnFormat(int nIndex)
{
	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nIndex);
	return pColData->nFormat;
}

void CBWReportCtrl::SetColumnType(int nIndex, int nType)
{
	if (nIndex < 0 || nIndex >= (int)m_iColCount)
		return;

	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nIndex);
	pColData->nType = nType;
}

void CBWReportCtrl::SetColumnResize(int nIndex, BOOL bEnable)
{
	if (nIndex < 0 || nIndex >= (int)m_iColCount)
		return;

	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nIndex);
	pColData->bResize = bEnable;
}

void CBWReportCtrl::SetColumnText(int nIndex, CString strText)
{
	if (nIndex < 0 || nIndex >= (int)m_iColCount)
		return;

	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nIndex);
	pColData->strText = strText;
}

void CBWReportCtrl::SetColumnData(int nIndex, DWORD dwParam)
{
	if (nIndex < 0 || nIndex >= (int)m_iColCount)
		return;

	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nIndex);
	pColData->dwParam = dwParam;

}

DWORD CBWReportCtrl::GetColumnData(int iIndex)
{
	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(iIndex);
	return pColData->dwParam;
}


/**
 * 插入一行数据的第一列的数据
 * \param strText 
 * \param nRow 
 */
int CBWReportCtrl::InsertRow(int nRow, CString strText)
{
	CReportRowItem* pRowData = new CReportRowItem;
	CReportItem* pItemData = new CReportItem;
	pItemData->m_strText = strText;

	if (pItemData->m_nFormat == 0)
		pItemData->m_nFormat = GetColumnFormat(0);

	COLORREF clr = m_clrBkg;
	if(m_bOddEvenColor)
	{
		if((nRow + 1) % 2 == 0)
		{
			clr = m_clrEven;
		}
		else
		{
			clr = m_clrOdd;
		}
	}
	else
	{
		clr = m_clrBkg;
	}
	//pItemData->m_clrBkg = m_clrBkg;
	pItemData->m_clrBkg = clr;
	pRowData->AddChild(pItemData);

	// 填充剩下子项
	while(pRowData->GetChildCount() < m_iColCount)
	{
		pItemData = new CReportItem;
		//pItemData->m_clrBkg = m_clrBkg;
		pItemData->m_clrBkg = clr;
		pRowData->AddChild(pItemData);
	}
	
	if (nRow >=0 && nRow < m_arrayAllData.GetSize())
	{
		m_arrayAllData.InsertAt(nRow, pRowData);
	}
	else
	{
		nRow = (int)m_arrayAllData.Add(pRowData);
	}

	m_arrayRowHeight.Add(m_iRowHeight);// 默认值
	m_iRowCount = (UINT)m_arrayAllData.GetCount();

	if (m_bOddEvenColor)
	{
		for (int i=nRow+1; i<m_iRowCount; i++)
		{
			if((i + 1) % 2 == 0)
			{
				clr = m_clrEven;
			}
			else
			{
				clr = m_clrOdd;
			}
			for (int j=0; j<m_iColCount; j++)
			{
				CReportItem* pItemData = GetRowItem(i, j);
				pItemData->m_clrBkg = clr;
			}
		}
	}

    ResetScrollBars();
	Invalidate();

	return nRow;
}

int CBWReportCtrl::AddRow(CString strText)
{
	int nRow = (int)m_arrayAllData.GetSize();
	return InsertRow(nRow, strText);
}

// 设置一行数据中指定子项的文本
void CBWReportCtrl::SetItemFormat(int nRow, int nSubItem, UINT nFormat)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_nFormat = nFormat;
		Invalidate();
	}
}

CString CBWReportCtrl::GetItemText(int nRow, int nCol)
{
	CReportRowItem* pRowData = m_arrayAllData.GetAt(nRow);
	CReportItem* pItemData = (CReportItem*)pRowData->GetChildAt(nCol);

	return pItemData->m_strText;
}

// 设置一行数据中指定子项的文本
void CBWReportCtrl::SetItemText(int nRow, int nSubItem, CString strText)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_strText = strText;
		if (pItemData->m_nFormat == 0)
		{
			pItemData->m_nFormat = GetColumnFormat(nSubItem);
		}
		Invalidate();
	}
}

void CBWReportCtrl::SetItemBkgColor(int nRow, int nSubItem, COLORREF clrBkg)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_clrBkg = clrBkg;
		Invalidate();
	}
}

void CBWReportCtrl::SetItemTextColor(int nRow, int nSubItem, COLORREF clrText)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_clrText = clrText;
		Invalidate();
	}
}

void CBWReportCtrl::SetItemEnable(int nRow, int nSubItem, BOOL bEnable)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_bEnable = bEnable;
		Invalidate();
	}
}

void CBWReportCtrl::SetItemFillColor(int nRow, int nSubItem, COLORREF clrFill)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_clrFill = clrFill;
		Invalidate();
	}
}

COLORREF CBWReportCtrl::GetItemFillColor(int iRow, int iSubItem)
{
	CReportItem* pItemData = GetRowItem(iRow, iSubItem);
	return pItemData->m_clrFill;
}

void CBWReportCtrl::SetItemStatus(int nRow, int nSubItem, int iStatus)
{
	CReportItem* pItemData = GetRowItem(nRow, nSubItem);
	if (pItemData != NULL)
	{
		pItemData->m_iStatus = iStatus;
		Invalidate();
	}
}

int CBWReportCtrl::GetItemStatus(int iRow, int iSubItem)
{
	CReportItem* pItemData = GetRowItem(iRow, iSubItem);
	return pItemData->m_iStatus;
}


void CBWReportCtrl::ResetScrollBars()
{
    if (!::IsWindow(GetSafeHwnd())) 
        return;

    CRect rect;
    GetClientRect(rect);

	// 重新设置栏宽
	if (m_bAutoFilling)
	{
		RecalculateColWidth(rect);
	}

    rect.left += GetFixedColumnWidth();
    rect.top += GetFixedRowHeight();
    if (rect.left >= rect.right || rect.top >= rect.bottom) 
		return;

	int iVirtualWidth = GetVirtualWidth();
	if (m_bAutoFilling)
	{
		if (iVirtualWidth > rect.Width())
			iVirtualWidth = rect.Width();
	}

    CRect rectVisible(GetFixedColumnWidth(), GetFixedRowHeight(), rect.right, rect.bottom);
    CRect rectVirtual(GetFixedColumnWidth(), GetFixedRowHeight(), iVirtualWidth, GetVirtualHeight());
	m_sizeVirtual.cx = rectVisible.Width() < rectVirtual.Width() ? rectVirtual.Width()-1 : 0;
	m_sizeVirtual.cy = rectVisible.Height() < rectVirtual.Height() ? rectVirtual.Height()-1 : 0;
    ASSERT(m_sizeVirtual.cx < INT_MAX && m_sizeVirtual.cy < INT_MAX); // This should be fine :)

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax  = m_sizeVirtual.cx;
	si.nPage = rectVisible.Width();
	SetScrollInfo(SB_HORZ, &si, TRUE); 

	si.nMax  = m_sizeVirtual.cy;
	si.nPage = rectVisible.Height();
	SetScrollInfo(SB_VERT, &si, FALSE);
}

void CBWReportCtrl::RecalculateColWidth(CRect recClient)
{
	int iColWidth = 0;
	int iResizeColWidth = 0;
	int iFixedColWidth = 0;

	long lVirtualWidth = 0;
	for (int i = 0; i < m_iColCount; i++)
	{
		tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(i);
		iColWidth += pColData->iWidth;
		if (pColData->bResize)
		{
			iResizeColWidth += pColData->iWidth;
		}
		else
		{
			iFixedColWidth += pColData->iWidth;
		}
	}

	if (iResizeColWidth > 0)
	{
		float fScaleRatio = float(recClient.Width() - iFixedColWidth) / iResizeColWidth;
		for (int j=0; j<m_iColCount; j++)
		{
			tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(j);
			if (pColData->bResize)
			{
				pColData->iWidth = int(pColData->iWidth * fScaleRatio + 0.5);
			}
		}
	}
}

// Get/Set scroll position using 32 bit functions
int CBWReportCtrl::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else 
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CBWReportCtrl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

int CBWReportCtrl::GetFixedRowHeight() const
{
    int nHeight = 0;
	if (m_bHasHeader)
	{
		nHeight += m_iHeaderHeight;
	}

    for (int i = 0; i < m_iFixedRowCount; i++)
	{
        nHeight += GetRowHeight(i);
	}

    return nHeight;
}

int CBWReportCtrl::GetFixedColumnWidth() const
{
    int nWidth = 0;
    for (int i = 0; i < m_iFixedColCount; i++)
	{
        nWidth += GetColumnWidth(i);
	}

    return nWidth;
}

long CBWReportCtrl::GetVirtualWidth() const
{
    long lVirtualWidth = 0;
    for (int i = 0; i < m_iColCount; i++)
	{
		lVirtualWidth += GetColumnWidth(i);
	}

    return lVirtualWidth;
}

long CBWReportCtrl::GetVirtualHeight() const
{
	long lReturn = m_iRowHeight * m_iRowCount + 20;
	return lReturn;
}

int CBWReportCtrl::GetRowHeight(int iRow) const
{
    if (iRow < 0 || iRow >= m_iRowCount) 
		return -1;

    return m_arrayRowHeight[iRow];
}

int CBWReportCtrl::GetColumnWidth(int iCol) const
{
    if (iCol < 0 || iCol >= m_iColCount) 
		return -1;
	
	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(iCol);
	return pColData->iWidth;
}

CString CBWReportCtrl::GetColumnName(int iCol) const
{
	CString strName;
	strName.Empty();
	if (iCol < 0 || iCol >= m_iColCount) 
		return strName;

	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(iCol);

	strName = pColData->strText;
	return strName;
}

void CBWReportCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();  // Auto-destroy any InPlaceEdit's

    int iScrollPos = GetScrollPos32(SB_HORZ);
    CSize sizeTopLeft = GetTopleftNonFixedIndex();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
    case SB_LINERIGHT:
        if (iScrollPos < m_sizeVirtual.cx)
        {
			int iHorzScroll = GetColumnWidth(sizeTopLeft.cx);
            SetScrollPos32(SB_HORZ, iScrollPos + iHorzScroll);
            if (GetScrollPos32(SB_HORZ) == iScrollPos)
				break;

            rect.left = GetFixedColumnWidth() + iHorzScroll;
            ScrollWindow(-iHorzScroll, 0, rect);
            rect.left = rect.right - iHorzScroll -1;
            InvalidateRect(rect);
        }
        break;

    case SB_LINELEFT:
		if (iScrollPos > 0 && sizeTopLeft.cx > (long)m_iFixedColCount)
        {
            int iHorzScroll = GetColumnWidth(sizeTopLeft.cx -1);
            SetScrollPos32(SB_HORZ, max(0,iScrollPos - iHorzScroll));
            rect.left = GetFixedColumnWidth();
            ScrollWindow(iHorzScroll, 0, rect);
            rect.right = rect.left + iHorzScroll;
            InvalidateRect(rect);
			//Invalidate();
        }
        break;

    case SB_PAGERIGHT:
		if (iScrollPos < m_sizeVirtual.cx)
        {
            rect.left = GetFixedColumnWidth();
            int offset = rect.Width();
			int pos = min(m_sizeVirtual.cx, iScrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            InvalidateRect(rect);
        }
        break;

    case SB_PAGELEFT:
        if (iScrollPos > 0)
        {
            rect.left = GetFixedColumnWidth();
            int offset = -rect.Width();
            int pos = max(0, iScrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            InvalidateRect(rect);
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
            CSize sizeNewTopLeft = GetTopleftNonFixedIndex();
			if (sizeNewTopLeft.cx != sizeTopLeft.cx)
            {
                rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
        }
        break;

    case SB_LEFT:
        if (iScrollPos > 0)
        {
            SetScrollPos32(SB_HORZ, 0);
            Invalidate();
        }
        break;

    case SB_RIGHT:
		if (iScrollPos < m_sizeVirtual.cx)
        {
			SetScrollPos32(SB_HORZ, m_sizeVirtual.cx);
            Invalidate();
        }
        break;

        default: 
			break;
    }

	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBWReportCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();        // Auto-destroy any InPlaceEdit's

    // Get the scroll position ourselves to ensure we get a 32 bit value
    int iScrollPos = GetScrollPos32(SB_VERT);

    CSize sizeTopLeft = GetTopleftNonFixedIndex();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
        case SB_LINEDOWN:
			if (iScrollPos < m_sizeVirtual.cy)
            {
                int iVertScroll = GetRowHeight(sizeTopLeft.cy);
                SetScrollPos32(SB_VERT, iScrollPos + iVertScroll);
                if (GetScrollPos32(SB_VERT) == iScrollPos) 
                    break;          // didn't work

                rect.top = GetFixedRowHeight() + iVertScroll;
                ScrollWindow( 0, -iVertScroll, rect);
                rect.top = rect.bottom - iVertScroll;
				InvalidateRect(rect);
			}
            break;

        case SB_LINEUP:
			if (iScrollPos > 0 && sizeTopLeft.cy > (long)m_iFixedRowCount)
            {
				int iVertScroll = GetRowHeight(sizeTopLeft.cy -1);
                SetScrollPos32(SB_VERT, max(0, iScrollPos - iVertScroll));
                rect.top = GetFixedRowHeight();
                ScrollWindow(0, iVertScroll, rect);
                rect.bottom = rect.top + iVertScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEDOWN:
			if (iScrollPos < m_sizeVirtual.cy)
            {
                rect.top = GetFixedRowHeight();
				iScrollPos = min(m_sizeVirtual.cy, iScrollPos + rect.Height());
                SetScrollPos32(SB_VERT, iScrollPos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEUP:
            if (iScrollPos > 0)
            {
                rect.top = GetFixedRowHeight();
                int offset = -rect.Height();
                int pos = max(0, iScrollPos + offset);
                SetScrollPos32(SB_VERT, pos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
                SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
                CSize sizeNewTopLeft = GetTopleftNonFixedIndex();
				if (sizeNewTopLeft.cy != sizeTopLeft.cy)
                {
                    rect.top = GetFixedRowHeight();
                    InvalidateRect(rect);
                }
            }
            break;

        case SB_TOP:
            if (iScrollPos > 0)
            {
                SetScrollPos32(SB_VERT, 0);
                Invalidate();
            }
            break;

        case SB_BOTTOM:
			if (iScrollPos < m_sizeVirtual.cy)
            {
                SetScrollPos32(SB_VERT, m_sizeVirtual.cy);
                Invalidate();
            }

        default: break;
    }
	
}


BOOL CBWReportCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT nMouseScrollLines = GetMouseScrollLines();
	int iRowsScrolled = (int)nMouseScrollLines * zDelta / 120;

	if (iRowsScrolled>0)
	{
		for (int i=0; i<iRowsScrolled; i++)
			PostMessage(WM_VSCROLL, SB_LINEUP, 0);
	} // if (iRowsScrolled>0)
	else
	{
		for (int i=0; i>iRowsScrolled; i--)
			PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	}

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

UINT CBWReportCtrl::GetMouseScrollLines()
{
	UINT nScrollLines = 3; // Reasonable default
	HKEY hKey;

	if(RegOpenKeyEx(
		HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
		0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS
	) {
		TCHAR szData[128];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = sizeof(szData);

		if(RegQueryValueEx(
			hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
			(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS
		)
			nScrollLines = _tcstoul(szData, NULL, 10);

		RegCloseKey(hKey);
	}

	return nScrollLines;
}


CSize CBWReportCtrl::GetTopleftNonFixedIndex() const
{
    int iVertScroll = GetScrollPos(SB_VERT);
    int iHorzScroll = GetScrollPos(SB_HORZ);

	int nColumn = m_iFixedColCount;
	int nRight = 0;

    while (nRight < iHorzScroll && nColumn < (m_iColCount -1))
	{
        nRight += GetColumnWidth(nColumn++);
	}

    int nRow = m_iFixedRowCount;
	int nTop = 0;
    while (nTop < iVertScroll && nRow < (m_iRowCount -1))
	{
        nTop += GetRowHeight(nRow++);
	}

    return CSize(nColumn, nRow);;
}

// 绘制指定线段
void CBWReportCtrl::DrawLine(CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF clrPen)
{
	CPen pen(PS_SOLID, 1, clrPen);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

void CBWReportCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	ResetScrollBars();
}

void CBWReportCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	this->SetFocus();

	if (m_iSizingColIndex != -1) // 修改栏宽度
	{
		m_bIsColSizing = TRUE;
		
		// 绘制分割线
		DrawSplittingLine(point.x);

		SetCapture();
	}
	else 
	{
		if (point.y >  m_iHeaderHeight)
		{ // 进行行选择
			long lNewRow = GetRowIndexBy(point);
			if (lNewRow >= (long)m_iFixedRowCount)
			{
				// 处理选择行，包括多选
				HandleRowSelected(nFlags, lNewRow);

				int iNewCol =  GetColIndexBy(point);
				if (iNewCol >= (int)m_iFixedColCount && CanEdit(lNewRow, iNewCol))
				{
					// 进行编辑
					sListColAttribute* pColAttr;
					int nItemType = GetItemTypeBy(iNewCol, lNewRow, pColAttr);
					switch (nItemType)
					{
					case LIST_ITEM_DISABLE:
						break;
					case LIST_ITEM_EDIT:
					case LIST_ITEM_NUMEDIT:
						HandleEditSubitem(lNewRow, iNewCol, nItemType, pColAttr);
						break;
					case LIST_ITEM_COMBO:
						HandleComboBoxSubitem(lNewRow, iNewCol, pColAttr);
						break;
					case LIST_ITEM_CHECK:
						HandleCheckBoxSubitem(lNewRow, iNewCol, pColAttr);
						break;
					case LIST_ITEM_COLOR:
						HandleColorSubitem(lNewRow, iNewCol, pColAttr);
						break;
					case LIST_ITEM_DELETE:
						HandleDeleteSubitem(lNewRow, iNewCol, pColAttr);
						break;
					}
				}
				
				Invalidate();
			}
		}
		else // 在标题栏上
		{
			int iNewCol =  GetColIndexBy(point);
			if (m_bEnableSort && iNewCol != -1)
			{
				if (nFlags & MK_CONTROL)
					SetColSortState(iNewCol, TRUE);
				else
					SetColSortState(iNewCol, FALSE);

				SetColClickedState(iNewCol, TRUE);
				
				// 排序
				SortRows(iNewCol);
				
				m_iColClickedIndex = iNewCol;
				Invalidate();
			}
			
			m_bIsColClicked = TRUE;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CBWReportCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsColSizing)
	{
		int iColWidth = point.x - m_lSizingColLeft;
		if (iColWidth < m_iMinColWidth)
			iColWidth = m_iMinColWidth;
		ASSERT(iColWidth >= 0);
		tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(m_iSizingColIndex);
		if (iColWidth != pColData->iWidth)
		{
			pColData->iWidth = iColWidth;
			ResetScrollBars();
			Invalidate();
		}
		m_bIsColSizing = FALSE;
		ReleaseCapture();
	}
	
	if (m_bIsColClicked)
	{
		SetColClickedState(m_iColClickedIndex, FALSE);
		Invalidate();
		
		m_iColClickedIndex = -1;
		m_bIsColClicked = FALSE;
	}


	CWnd::OnLButtonUp(nFlags, point);
}

void CBWReportCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsColSizing)
	{
		int iColWidth = point.x - m_lSizingColLeft;
		if (iColWidth < m_iMinColWidth)
			iColWidth = m_iMinColWidth;
		DrawSplittingLine(m_lSizingColLeft + iColWidth);
	}

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CBWReportCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!m_bAutoFilling && !m_bIsColSizing)
	{
		CPoint pCursor;
		if (::GetCursorPos(&pCursor))
		{
			ScreenToClient(&pCursor);
			if (PtInColSizingRect(pCursor))
			{
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				return TRUE;
			}
		} // if (::GetCursorPos(&pCursor))
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

/**
 * 判断鼠标是否在修改栏宽度的区域
 * \param point 
 * \return 
 */
BOOL CBWReportCtrl::PtInColSizingRect(CPoint point)
{
	long lOffset = 0;
    CSize sizeTopLeft = GetTopleftNonFixedIndex();
	CRect rectSizing;
	m_iSizingColIndex = -1;
	m_lSizingColLeft = 0;
	
	for (int i=0; i<m_iFixedColCount; i++)
	{
		lOffset += GetColumnWidth(i);
		rectSizing = GetColSizingRect(lOffset);
		if (rectSizing.PtInRect(point))
		{			
			m_iSizingColIndex = i;
			m_lSizingColLeft = lOffset - GetColumnWidth(i);
			m_lDrawingPos = lOffset;
			return TRUE;
		}
	}
	
	for (int j=sizeTopLeft.cx; j<m_iColCount; j++)
	{
		lOffset += GetColumnWidth(j);
		rectSizing = GetColSizingRect(lOffset);
		if (rectSizing.PtInRect(point))
		{
			
			m_iSizingColIndex = j;
			m_lSizingColLeft = lOffset - GetColumnWidth(j);
			m_lDrawingPos = lOffset;
			return TRUE;
		}
	}

	return FALSE;
}

CRect CBWReportCtrl::GetColSizingRect(long lOffset)
{
	CRect rectReturn(0,0,0,0);
	
	rectReturn.left = rectReturn.right = lOffset;
	rectReturn.bottom = m_iHeaderHeight;
	rectReturn.InflateRect(2, 0);

	return rectReturn;
}

/**
 * 绘制变化中的分割线
 * \param lHorzPos 水平位置
 */
void CBWReportCtrl::DrawSplittingLine(long lHorzPos)
{
	if (m_lDrawingPos == lHorzPos)
		return;

	CClientDC dc(this);
	CRect rectClient;
	CPen pen;
	COLORREF XorColor = dc.GetBkColor() ^ RGB(0,0,0);
	pen.CreatePen(PS_SOLID, 1, XorColor);
	int	oldmode = dc.SetROP2(R2_XORPEN);
	CPen* oldpen = dc.SelectObject(&pen);
	GetClientRect(rectClient);
	
	dc.MoveTo(m_lDrawingPos, rectClient.top);
	dc.LineTo(m_lDrawingPos, rectClient.bottom);
	dc.MoveTo(m_lDrawingPos+1, rectClient.top);
	dc.LineTo(m_lDrawingPos+1, rectClient.bottom);
	

	dc.MoveTo(lHorzPos, rectClient.top);
	dc.LineTo(lHorzPos, rectClient.bottom);
	dc.MoveTo(lHorzPos+1, rectClient.top);
	dc.LineTo(lHorzPos+1, rectClient.bottom);

	m_lDrawingPos = lHorzPos;

	dc.SetROP2(oldmode);
	dc.SelectObject(oldpen);
}

/**
 * 判断给定点所在的行
 * \param point 
 * \return 
 */
long CBWReportCtrl::GetRowIndexBy(CPoint point)
{
	long lOffset = m_iHeaderHeight;
    CSize sizeTopLeft = GetTopleftNonFixedIndex();
	
	for (int i=0; i<m_iFixedRowCount; i++)
	{
		if (point.y > lOffset && point.y <= lOffset+GetRowHeight(i))
		{			
			return i;
		}
		lOffset += GetRowHeight(i);
	}
	
	for (int j=sizeTopLeft.cy; j<m_iRowCount; j++)
	{
		if (point.y > lOffset && point.y <= lOffset+GetRowHeight(j))
		{
			return j;
		}
		lOffset += GetRowHeight(j);
	}

	return -1;
}

/**
 * 判断给定点所在标题栏的列序号
 * \param point 
 * \return 
 */
int CBWReportCtrl::GetColIndexBy(CPoint point)
{
	long lOffset = 0;
    CSize sizeTopLeft = GetTopleftNonFixedIndex();
	
	for (int i=0; i<m_iFixedColCount; i++)
	{
		if (point.x > lOffset && point.x <= lOffset+GetColumnWidth(i))
		{			
			return i;
		}
		lOffset += GetColumnWidth(i);
	}
	
	for (int j=sizeTopLeft.cx; j<m_iColCount; j++)
	{
		if (point.x > lOffset && point.x <= lOffset+GetColumnWidth(j))
		{
			return j;;
		}
		lOffset += GetColumnWidth(j);
	}

	return -1;
}

/**
 * 点击标题栏进行排序
 * \param nCol 被点击的标题栏
 * \param bIsAdd 是否是增加的多栏排序
 */
void CBWReportCtrl::SetColSortState(int nCol, BOOL bIsAdd)
{
	tagReportColData* pColData = NULL;

	for (int i=0; i<m_iColCount; i++)
	{
		pColData = (tagReportColData*)m_arrayColData.GetAt(i);

		if (i == nCol)
		{
			if (pColData->nSortType == SORT_NO)
				pColData->nSortType = SORT_ASCEND;
			else if (pColData->nSortType == SORT_ASCEND)
				pColData->nSortType = SORT_DESCEND;
			else
				pColData->nSortType = SORT_ASCEND;
		} // if (i == nCol)
		else
		{
			if (!bIsAdd)
			{
				pColData->nSortType = SORT_NO;
			}
		}
	}
}

void CBWReportCtrl::SetColClickedState(int nCol, BOOL bIsClicked)
{
	if (nCol == -1 || nCol>= m_iColCount)
		return;
	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nCol);
	pColData->bClicked = bIsClicked;
}

/**
 * 根据给定列数排序
 * \param nSortCol 
 */
void CBWReportCtrl::SortRows(int nSortCol)
{
	if (nSortCol == -1 || nSortCol>= m_iColCount)
		return;

	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nSortCol);
	if (pColData->nSortType == SORT_NO)
		return;

	CUIntArray* pSortIndexArray = new CUIntArray();
	GetOtherSortIndex(pSortIndexArray, nSortCol); // 得到需排序的列序号

	if (pSortIndexArray->GetSize() == 0)
	{
		SortRowsByRange(nSortCol, 0, m_iRowCount, pColData->nSortType);
	}
	else
	{
		int nBeginRow, nEndRow;

		nBeginRow = nEndRow= 0;
		while (nEndRow < m_iRowCount)
		{
			nEndRow = GetNextDifferentDataIndex(nBeginRow, pSortIndexArray);
			if (nEndRow - nBeginRow > 1) // 超过一行才需排序
				SortRowsByRange(nSortCol, nBeginRow, nEndRow, pColData->nSortType, nEndRow == m_iRowCount-1);

			nBeginRow = nEndRow;
		}

	}

	delete pSortIndexArray;
}

/**
 * 得到除去指定列数外的需排序列序号
 * \param pSortIndexArray 保存列序号的数值指针
 * \param nSortCol 需排除的列序号
 */
void CBWReportCtrl::GetOtherSortIndex(CUIntArray* pSortIndexArray, int nSortCol)
{
	tagReportColData* pColData = NULL;

	for (int i=0; i<m_iColCount; i++)
	{
		pColData = (tagReportColData*)m_arrayColData.GetAt(i);
		
		if (i == nSortCol)
			continue;

		if (pColData->nSortType != SORT_NO)
		{
			pSortIndexArray->Add(i);
		}
	}
}

/**
 * 将给定范围内的行进行排序，使用合并排序算法
 * \param nSortCol 排序列
 * \param nBeginRow 排序开始行（包含）
 * \param nEndRow 排序结束行（不包含）
 * \param nSortType 排序类型
 * \param bIsIncludeEnd 排序是否包含结束行
 */
void CBWReportCtrl::SortRowsByRange(int nSortCol, int nBeginRow, int nEndRow, UINT nSortType, BOOL bIsIncludeEnd)
{
	int nEndIndex = nEndRow;
	
	if (!bIsIncludeEnd)
		nEndIndex -= 1;
	
	CReportDataArray arrayData1, arrayData2;
	
	// 将需要处理的数据赋值
	for (int i=nBeginRow; i<= nEndIndex; i++)
	{
		arrayData1.Add(m_arrayAllData.GetAt(i));
	}
	
	int s = 1;
	int nCount = (int)arrayData1.GetSize();
	
	arrayData2.SetSize(nCount);
	
	while (s < nCount)
	{
		MergePass(&arrayData1, &arrayData2, s, nCount, nSortCol, nSortType);
		s += s;

		//WriteReportData(&arrayData1, nSortCol);
		//WriteReportData(&arrayData2, nSortCol);

		MergePass(&arrayData2, &arrayData1, s, nCount, nSortCol, nSortType);
		s += s;

		//WriteReportData(&arrayData1, nSortCol);
		//WriteReportData(&arrayData2, nSortCol);
	} // while (s < nCount)

	// 将排序后的数据保存
	for (int j=0; j< nCount; j++)
	{
		m_arrayAllData.SetAt(j+nBeginRow, arrayData1.GetAt(j));
	}
}

/**
 * 合并排好序的相邻数组段
 * \param pData1 被合并数组
 * \param pData2 合并到数组
 * \param s 合并步长
 * \param n 数组长度
 * \param nSortCol 排序列
 * \param nSortType 排序类型
 */
void CBWReportCtrl::MergePass(CReportDataArray* pData1, CReportDataArray* pData2, 
							   int s, int n, UINT nSortCol, UINT nSortType)
{
	int i=0;

	while (i <= n - 2*s)
	{
		// 合并大小为s的相邻2段子数组
		Merge(pData1, pData2, i, i+s-1, i+2*s-1, nSortCol, nSortType);
		i = i+2*s;
	} // while (i <= n - 2*s)

	// 剩下的元素个数少于2s
	if (i+s < n)
		Merge(pData1, pData2, i, i+s-1, n-1, nSortCol, nSortType);
	else
	{
		for (int j=i; j< n; j++)
		{
			pData2->SetAt(j, pData1->GetAt(j));
		}
	}
}

/**
 * 将一个数组中的相邻两部分数据排序合并到另一个数组中
 * \param pData1 需合并排序的数组
 * \param pData2 保存排序后数据的数组
 * \param l 需合并排序的数组的第一部分数据的开始序号
 * \param m 需合并排序的数组的第一部分数据的结束序号
 * \param r 需合并排序的数组的第二部分数据的结束序号
 * \param nSortCol 需排序的列
 * \param nSortType 排序类型
 */
void CBWReportCtrl::Merge(CReportDataArray* pData1, CReportDataArray* pData2, 
						   int l, int m, int r, UINT nSortCol, UINT nSortType)
{
	// 合并pData1[l:m] 和 pData1[m+1:r] 到pData2[l:r]
	int i =l;
	int j = m+1;
	int k = l;
	
    while (( i<= m ) && (j <= r))
	{
		if (nSortType == SORT_ASCEND)
		{
			if (CompareReportItem(pData1, nSortCol, i, j) < 0)
			{
				pData2->SetAt(k, pData1->GetAt(i));
				k ++;
				i ++;
			}
			else
			{
				pData2->SetAt(k, pData1->GetAt(j));
				k ++;
				j ++;
			}
		}
		else
		{
			if (CompareReportItem(pData1, nSortCol, i, j) > 0)
			{
				pData2->SetAt(k, pData1->GetAt(i));
				k ++;
				i ++;
			}
			else
			{
				pData2->SetAt(k, pData1->GetAt(j));
				k ++;
				j ++;
			}
		}
	} // while (( i<= m ) && (j <= r))

	if ( i> m)
	{
		for ( int q=j; q<= r; q++)
		{
			pData2->SetAt(k, pData1->GetAt(q));
			k ++;
		}
	}
	else
	{
		for ( int q=i; q<= m; q++)
		{
			pData2->SetAt(k, pData1->GetAt(q));
			k ++;
		}
	}
}

/**
 * 比较给定数组的两个数值的大小
 * \param pData 
 * \param nSortCol 
 * \param nIndex1 
 * \param nIndex2 
 * \return 
 */
int CBWReportCtrl::CompareReportItem(CReportDataArray* pData, UINT nSortCol, UINT nIndex1, UINT nIndex2)
{
	tagReportColData* pColData = (tagReportColData*)m_arrayColData.GetAt(nSortCol);

	CReportRowItem* pRowData1 = pData->GetAt(nIndex1);
	CReportRowItem* pRowData2 = pData->GetAt(nIndex2);

	CReportItem* pItemData1 = pRowData1->GetChildAt(nSortCol);
	CReportItem* pItemData2 = pRowData2->GetChildAt(nSortCol);
	
	if (pColData->nType == DATA_INTEGER)
	{
		long lData1, lData2;
		lData1 = atol((char*)pItemData1->m_strText.GetBuffer());
		pItemData1->m_strText.ReleaseBuffer();
		lData2 = atol((char*)pItemData2->m_strText.GetBuffer());
		pItemData2->m_strText.ReleaseBuffer();
		if (lData1 == lData2)
			return 0;
		else if (lData1 > lData2)
			return 1;
		else 
			return -1;

	}
	else if (pColData->nType == DATA_FLOAT)
	{
		double dData1, dData2;
		dData1 = ::_tstof(pItemData1->m_strText);
		dData2 = ::_tstof(pItemData2->m_strText);
		if (dData1 == dData2)
			return 0;
		else if (dData1 > dData2)
			return 1;
		else 
			return -1;
	}
	else
		return pItemData1->m_strText.CompareNoCase(pItemData2->m_strText);
}


/**
 * 得到开始数据不同的行号
 * \param nBeginRow 
 * \param pSortIndexArray 
 * \return 
 */
UINT CBWReportCtrl::GetNextDifferentDataIndex(UINT nBeginRow, CUIntArray* pSortIndexArray)
{
	UINT nReturn = nBeginRow;

	CReportRowItem* pRowData = m_arrayAllData.GetAt(nBeginRow);
	CReportRowItem* pNextRowData = NULL;

	for (int i=nBeginRow+1; i< m_iRowCount; i++)
	{
 		pNextRowData = m_arrayAllData.GetAt(i);
		
		if (!IsTwoItemDataSameOfIndex(pRowData, pNextRowData, pSortIndexArray))
		{
			nReturn = i;
			break;
		}
	}
	
	if (nReturn == nBeginRow)
		nReturn = m_iRowCount;

	return nReturn;
}

/**
 * 判断两行数据的指定列是否数据相同
 * \param pRowData1 
 * \param pRowData2 
 * \param pSortIndex 
 * \return 
 */
BOOL CBWReportCtrl::IsTwoItemDataSameOfIndex(CReportRowItem* pRowData1, CReportRowItem* pRowData2, CUIntArray* pSortIndex)
{
	BOOL bReturn = TRUE;
	CReportItem* pItemData1 = NULL;
	CReportItem* pItemData2 = NULL;

	for (int i=0; i<pSortIndex->GetSize(); i++)
	{
		pItemData1 = pRowData1->GetChildAt(pSortIndex->GetAt(i));
		pItemData2 = pRowData2->GetChildAt(pSortIndex->GetAt(i));

		if (pItemData1->m_strText != pItemData2->m_strText)
		{
			bReturn = FALSE;
			break;
		}
	}

	return bReturn;
}

void CBWReportCtrl::SetRowColor(int nRow, COLORREF clrBkg)
{
	CReportRowItem* pRowData = m_arrayAllData.GetAt(nRow);
	CReportItem* pItemData = NULL;
	for (int i=0; i<m_iColCount; i++)
	{
		pItemData = (CReportItem*)pRowData->GetChildAt(i);
		pItemData->m_clrBkg = clrBkg;
		
	}
}


/**
 * 对子项时文本编辑时进行编辑
 * \param nIndex 子项所在的行
 * \param nCol 子项所在的列
 * \param nType 子项对象的类型
 */
void CBWReportCtrl::HandleEditSubitem(int nIndex, int nCol, int nType, sListColAttribute* pColAttr)
{
	CString strText = GetItemText(nIndex, nCol);
	CRect rtItemRect = GetSubItemRect(nIndex, nCol);

	//DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | CBS_HASSTRINGS;
	DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT| ES_AUTOHSCROLL | CBS_HASSTRINGS;

	m_pInEdit = new CBioInEdit(nIndex, nCol, strText);
	LOGFONT lf;
	m_fontItem.GetLogFont(&lf);
	m_pInEdit->SetUserFont(lf);
	if (nType == LIST_ITEM_NUMEDIT)
	{
		m_pInEdit->EnableNumEdit(pColAttr->nMin, pColAttr->nMax);
		m_pInEdit->SetPrecision(pColAttr->dwParam);
	}
	m_pInEdit->Create(dwStyle, rtItemRect, this, IDC_LISTEDIT);
}

/**
 * 对子项是选择框时进行选择
 * \param nIndex 子项所在的行
 * \param nCol 子项所在的列
 */
void CBWReportCtrl::HandleComboBoxSubitem(int nIndex, int nCol, sListColAttribute* pColAttr)
{
	CString strText = GetItemText(nIndex, nCol);
	CRect rtItemRect = GetSubItemRect(nIndex, nCol);
	rtItemRect.bottom = rtItemRect.top + 150;

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_DROPDOWNLIST;
	CStringArray* pArray = (CStringArray*)pColAttr->dwParam;
	m_pInComboBox = new CBioInComboBox(nIndex, nCol, strText, pArray);
	LOGFONT lf;
	m_fontItem.GetLogFont(&lf);
	m_pInComboBox->SetUserFont(lf);
	m_pInComboBox->Create(dwStyle, rtItemRect, this, IDC_LISTEDIT);
}

// 对CheckBox 进行选择
void CBWReportCtrl::HandleCheckBoxSubitem(int nIndex, int nCol, sListColAttribute* pColAttr)
{
	CReportItem* pItem = GetRowItem(nIndex, nCol);
	if (pItem != NULL)
	{
		LV_DISPINFO dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = LVN_ENDLABELEDIT;
		dispinfo.item.mask = LVIF_IMAGE;
		dispinfo.item.iItem = nIndex;
		dispinfo.item.iSubItem = nCol;
		dispinfo.item.lParam = pItem->m_iStatus;
		GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	}
}

void CBWReportCtrl::HandleColorSubitem(int nIndex, int nCol, sListColAttribute* pColAttr)
{
	COLORREF clrItem = GetItemFillColor(nIndex, nCol);
	CRect rtItemRect = GetSubItemRect(nIndex, nCol);
	rtItemRect.bottom = rtItemRect.top + 150;
	CRect rectShow = rtItemRect;
	rectShow.right = rectShow.left + m_iColorWndWidth;
	rectShow.bottom = rectShow.top + m_iColorWndHeight;

	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	m_pColorWnd = new CBWColorWnd(nIndex, nCol, clrItem, pColAttr->iDataCount, (COLORREF*)pColAttr->dwParam);
	m_pColorWnd->Create(dwStyle, rectShow, this, IDC_COLOR_EDIT);
}

void CBWReportCtrl::HandleDeleteSubitem(int nIndex, int nCol, sListColAttribute* pColAttr)
{
	CReportItem* pItem = GetRowItem(nIndex, nCol);
	if (pItem != NULL)
	{
		LV_DISPINFO dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = LVN_ENDLABELEDIT;
		dispinfo.item.mask = LVIF_IMAGE;
		dispinfo.item.iItem = nIndex;
		dispinfo.item.iSubItem = nCol;
		GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	}
}


/**
 * 取得给定行数和列数的列表子项的矩形区域
 * \param point 选择点位置
 * \return  子项的区域矩形
 */
CRect CBWReportCtrl::GetSubItemRect(int nIndex, int nCol)
{
	CRect rtReturn(0,0,0,0);

	rtReturn.top = rtReturn.bottom = m_iHeaderHeight;
	//long lOffset = m_iHeaderHeight;
    CSize sizeTopLeft = GetTopleftNonFixedIndex();
	BOOL bRowFind = FALSE;
	BOOL bColFind = FALSE;

	// 得到矩形上下位置
	for (int i=0; i<m_iFixedRowCount; i++)
	{
		rtReturn.top = rtReturn.bottom;
		rtReturn.bottom = rtReturn.top + GetRowHeight(i);
		if (nIndex == i)
		{	
			bRowFind = TRUE;
			break;
		}
	}
	
	if (!bRowFind)
	{
		for (int j=sizeTopLeft.cy; j<m_iRowCount; j++)
		{
			rtReturn.top = rtReturn.bottom;
			rtReturn.bottom = rtReturn.top + GetRowHeight(j);
			if (nIndex == j)
			{	
				bRowFind = TRUE;
				break;
			}
		}
	}

	ASSERT(bRowFind);

	for (int i=0; i<m_iFixedColCount; i++)
	{
		rtReturn.left = rtReturn.right;
		rtReturn.right = rtReturn.left + GetColumnWidth(i);
		if (nCol == i)
		{	
			bColFind = TRUE;
			break;
		}
	}
	
	if (!bColFind)
	{
		for (int j=sizeTopLeft.cx; j<m_iColCount; j++)
		{
			rtReturn.left = rtReturn.right;
			rtReturn.right = rtReturn.left + GetColumnWidth(j);
			if (nCol == j)
			{
				bColFind = TRUE;
				break;
			}
		}
	}
	ASSERT(bColFind);

	return rtReturn;
}

/**
 * 根据列数判断此数据项的类型，包括edit,combobox 等
 * \param nCol 需判断的列数
 * \return 子项对象的类型
 */
int CBWReportCtrl::GetItemTypeBy(int nCol, int nRow, sListColAttribute* &pColAttr)
{
	int nReturn = LIST_ITEM_EDIT; // 默认为CEdit控件
	pColAttr = NULL;
	// 判断是否CComboBox控件
	sListColAttribute sColText;
	for (int i=0; i<m_arrayColAttr.GetSize(); i++)
	{
		sColText = m_arrayColAttr.GetAt(i);
		if(sColText.nCol == nCol && sColText.nRow == -1)
		{
			pColAttr = &m_arrayColAttr.GetAt(i);
			nReturn = sColText.nType;
		} // if(sColText.nCol == nCol && sColText.nRow == -1)
		else if (sColText.nCol == nCol && sColText.nRow ==nRow)
		{
			pColAttr = &m_arrayColAttr.GetAt(i);
			nReturn = sColText.nType;
			break;
		}
	}
	
	return nReturn;
}

/**
 * 处理列表子项对象发的的标签编辑消息
 * \param *pNMHDR 子项信息指针
 * \param *pResult 结果标记
 */
void CBWReportCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	
	int nItem = pDispInfo->item.iItem;
	int nSubItem = pDispInfo->item.iSubItem;

	if (m_pInEdit != NULL)
	{
		CString strText = pDispInfo->item.pszText;

		if (m_pInEdit->GetInitText() != strText)
		{
			SetItemText(nItem, nSubItem, strText);

			// 发送消息
			pDispInfo->hdr.code = LVN_SETDISPINFO;
			GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)pDispInfo);
			//SendMessage(REPORT_ITEM_CHANGE, (WPARAM)&(pDispInfo->item));
		}
		delete m_pInEdit;
		m_pInEdit = NULL;
	}

	if(m_pInComboBox != NULL)
	{
		CString strText = pDispInfo->item.pszText;
		if (m_pInComboBox->GetInitText() != strText)
		{
			SetItemText(nItem, nSubItem, strText);
			// 发送消息
			pDispInfo->hdr.code = LVN_SETDISPINFO;
			GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)pDispInfo);
			//SendMessage(REPORT_ITEM_CHANGE, (WPARAM)&(pDispInfo->item));
		}
		delete m_pInComboBox;
		m_pInComboBox = NULL;
	}

	if(m_pColorWnd != NULL)
	{
		COLORREF clrSelected = pDispInfo->item.lParam;
		COLORREF clrInit = m_pColorWnd->GetInitColor() ;
		if (clrSelected != clrInit)
		{
			SetItemFillColor(nItem, nSubItem, clrSelected);
			// 发送消息
			pDispInfo->hdr.code = LVN_SETDISPINFO;
			GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)pDispInfo);
		}
		delete m_pColorWnd;
		m_pColorWnd = NULL;
	}

	*pResult = 0;
}

/**
 * 设置给定栏的列表文本数据
 * \param nCol 列表的列数
 * \param &arrayColText 对应列的可供选择数据项数组
 */
void CBWReportCtrl::SetColumnComboBox(int nCol, int nRow, CStringArray &arrayColText)
{
	// 查找是否已经设置可选项
	sListColAttribute sColText;
	int nIndex = FindItemBy(LIST_ITEM_COMBO, nCol, nRow);
	if (nIndex != -1)
	{
		sListColAttribute sColAttri = m_arrayColAttr.GetAt(nIndex);
		CStringArray* pArray = (CStringArray*)sColAttri.dwParam;
		if (pArray != NULL)
		{
			delete pArray;
			sColAttri.dwParam = 0;

		}
		m_arrayColAttr.RemoveAt(nIndex);
	}

	sColText.nCol = nCol;
	sColText.nRow = nRow;
	sColText.nType = LIST_ITEM_COMBO;
	sColText.nMin = 0;
	sColText.nMax = 0;
	CStringArray* pArray = new CStringArray;
	pArray->Copy(arrayColText);
	sColText.dwParam = (DWORD)pArray;
	m_arrayColAttr.Add(sColText);
}

/**
 * 设置列表项为数字编辑控件
 * \param nCol 列表的列数
 * \param lMin 数字的最小值，当前没有使用
 * \param lMax 数字的最大值，当前没有使用
 */
void CBWReportCtrl::SetColumnNumEdit(int nCol, int nRow, int iPrecision, int nMin, int nMax)
{
	sListColAttribute sColText;
	sColText.nCol = nCol;
	sColText.nRow = nRow;
	sColText.nType = LIST_ITEM_NUMEDIT;
	sColText.nMin = nMin;
	sColText.nMax = nMax;
	sColText.dwParam = iPrecision;

	int nIndex = FindItemBy(LIST_ITEM_NUMEDIT, nCol, nRow);
	if (nIndex != -1)
	{
		m_arrayColAttr.SetAt(nIndex, sColText);
	}
	else
	{
		m_arrayColAttr.Add(sColText);
	}
}

/**
 * 设置项不可编辑
 * \param nCol 
 * \param nRow 
 */
void CBWReportCtrl::SetColumnDisable(int nCol, int nRow)
{
	sListColAttribute sColText;
	sColText.nCol = nCol;
	sColText.nRow = nRow;
	sColText.nType = LIST_ITEM_DISABLE;
	sColText.nMin = 0;
	sColText.nMax = 0;
	sColText.dwParam = 0;

	int nIndex = FindItemBy(nCol, nRow);
	if (nIndex != -1)
	{
		m_arrayColAttr.SetAt(nIndex, sColText);
	}
	else
	{
		m_arrayColAttr.Add(sColText);
	}
}

void CBWReportCtrl::SetColumnCheckBox(int iCol, int iRow)
{
	sListColAttribute sColText;
	sColText.nCol = iCol;
	sColText.nRow = iRow;
	sColText.nType = LIST_ITEM_CHECK;
	sColText.nMin = 0;
	sColText.nMax = 0;
	sColText.dwParam = 0;

	int nIndex = FindItemBy(iCol, iRow);
	if (nIndex != -1)
	{
		m_arrayColAttr.SetAt(nIndex, sColText);
	}
	else
	{
		m_arrayColAttr.Add(sColText);
	}
}

void CBWReportCtrl::SetColumnColor(int iCol, int iRow, int iColorCount, COLORREF* pInputColor)
{
	int nIndex = FindItemBy(iCol, iRow);
	if (nIndex != -1)
	{
		sListColAttribute sColAttri = m_arrayColAttr.GetAt(nIndex);
		COLORREF* pColor = (COLORREF*)sColAttri.dwParam;
		if (pColor != NULL)
		{
			delete []pColor;
			sColAttri.dwParam = 0;

		}
		m_arrayColAttr.RemoveAt(nIndex);
	}

	sListColAttribute sColText;
	sColText.nCol = iCol;
	sColText.nRow = iRow;
	sColText.nType = LIST_ITEM_COLOR;
	sColText.nMin = 0;
	sColText.nMax = 0;
	sColText.dwParam = 0;
	if (pInputColor != NULL && iColorCount != 0)
	{
		COLORREF* pColor = new COLORREF[iColorCount];
		memcpy(pColor, pInputColor, sizeof(COLORREF)*iColorCount);
		sColText.dwParam = (DWORD)pColor;
		sColText.iDataCount = iColorCount;
	}

	m_arrayColAttr.Add(sColText);
}

void CBWReportCtrl::SetColumnDelete(int iCol, int iRow)
{
	sListColAttribute sColText;
	sColText.nCol = iCol;
	sColText.nRow = iRow;
	sColText.nType = LIST_ITEM_DELETE;
	sColText.nMin = 0;
	sColText.nMax = 0;
	sColText.dwParam = 0;

	int nIndex = FindItemBy(iCol, iRow);
	if (nIndex != -1)
	{
		m_arrayColAttr.SetAt(nIndex, sColText);
	}
	else
	{
		m_arrayColAttr.Add(sColText);
	}
}

/**
 * 查找符合条件的项的序号
 * \param nType 
 * \param nCol 
 * \param nRow 
 * \return 
 */
int CBWReportCtrl::FindItemBy(int nType, int nCol, int nRow)
{
	sListColAttribute sColText;
	for (int i=0; i<m_arrayColAttr.GetSize(); i++)
	{
		sColText = m_arrayColAttr.GetAt(i);
		if (sColText.nCol == nCol && sColText.nRow == nRow && sColText.nType == nType)
		{
			return i;
		}
	}

	return -1;
}

/**
* 查找符合条件的项的序号
* \param nCol 
* \param nRow 
* \return 
*/
int CBWReportCtrl::FindItemBy(int nCol, int nRow)
{
	sListColAttribute sColText;
	for (int i=0; i<m_arrayColAttr.GetSize(); i++)
	{
		sColText = m_arrayColAttr.GetAt(i);
		if (sColText.nCol == nCol && sColText.nRow == nRow)
		{
			return i;
		}
	}

	return -1;
}

BOOL CBWReportCtrl::IsRowSelected(UINT nRow)
{
	CReportRowItem* pRowData = m_arrayAllData.GetAt(nRow);
	return pRowData->m_bSelected;
}

void CBWReportCtrl::HandleRowSelected(UINT nFlags, int nNewRow)
{
	if (nNewRow < m_iFixedRowCount || nNewRow < 0)
		return;

	CReportRowItem* pRowData = m_arrayAllData.GetAt(nNewRow);
	if (m_iLastSelectedRow == -1)
	{
		pRowData->m_bSelected = TRUE;
		m_iLastSelectedRow = nNewRow;
	}
	else
	{
		if (m_bEnableMultiSelected)
		{
			if (nFlags & MK_SHIFT)
			{
				int nFirtRow, nEndRow;

				if (m_iLastSelectedRow > nNewRow)
				{
					nFirtRow = nNewRow;
					nEndRow = m_iLastSelectedRow;
				}
				else
				{
					nFirtRow = m_iLastSelectedRow;
					nEndRow = nNewRow;
				}

				for (int i=0; i<GetRowCount(); i++)
				{
					CReportRowItem* pTempRow = m_arrayAllData.GetAt(i);
					if (i>= nFirtRow && i<= nEndRow)
					{
						pTempRow->m_bSelected = TRUE;
					}
					else
					{
						pTempRow->m_bSelected = FALSE;
					}
				}
			}
			else if (nFlags & MK_CONTROL)
			{
				pRowData->m_bSelected = !pRowData->m_bSelected;
				m_iLastSelectedRow = nNewRow;
			}
			else
			{
				for (int i=0; i<GetRowCount(); i++)
				{
					CReportRowItem* pTempRow = m_arrayAllData.GetAt(i);
					pTempRow->m_bSelected = i == nNewRow;
				}
				m_iLastSelectedRow = nNewRow;
			}
		}
		else // 不支持多选
		{
			CReportRowItem* pTempRow = m_arrayAllData.GetAt(m_iLastSelectedRow);
			pTempRow->m_bSelected = FALSE;
			pRowData->m_bSelected = TRUE;
			m_iLastSelectedRow = nNewRow;
		}
	}

	SendMessageOfSelectChanged();
}

void CBWReportCtrl::SendMessageOfSelectChanged()
{
	if (m_iLastSelectedRow != -1)
	{
		GetParent()->SendMessage(BWREPORT_SEL_CHANGE, m_nMsgID, (WPARAM)m_iLastSelectedRow);
	}
}

int CBWReportCtrl::GetSelectedRow()
{
	if (m_bEnableMultiSelected)
	{
		ASSERT(FALSE);
		return -1;
	}
	
	return m_iLastSelectedRow;
}

int CBWReportCtrl::GetSelectedItems(UINT* &pOutItems)
{
	if (m_bEnableMultiSelected)
	{
		int nRowCount = GetRowCount();
		pOutItems = new UINT[nRowCount];
		
		int nCount = 0;
		for (int i=0; i<nRowCount; i++)
		{
			CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
			if (pRowData->m_bSelected)
			{
				pOutItems[nCount] = i;
				nCount ++;
			}
		}

		return nCount;
	}
	else
	{
		ASSERT(FALSE);
		return -1;
	}
}

void CBWReportCtrl::SelectAll()
{
	if (!m_bEnableMultiSelected)
	{
		ASSERT(FALSE);
		return;
	}

	for (int i=0; i<GetRowCount(); i++)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		pRowData->m_bSelected = TRUE;
	}
	
	if (GetRowCount() > 0)
		m_iLastSelectedRow = GetRowCount()-1;

	Invalidate();
	this->SendMessageOfSelectChanged();
}

void CBWReportCtrl::SetSelectedRow(int nRow)
{
	ASSERT(!m_bEnableMultiSelected);

	for (int i=0; i<GetRowCount(); i++)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		pRowData->m_bSelected = i == nRow;
	}
	m_iLastSelectedRow = nRow;

	Invalidate();
	this->SendMessageOfSelectChanged();
}

void CBWReportCtrl::SetSelectedItems(int nCount, UINT* pOutItems)
{
	if (!m_bEnableMultiSelected)
	{
		ASSERT(FALSE);
		return;
	}

	for (int i=0; i<GetRowCount(); i++)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		
		BOOL bSelected = FALSE;
		for (int j=0; j<nCount; j++)
		{
			if (pOutItems[j] == i)
			{
				bSelected = TRUE;
				break;
			}
		}

		pRowData->m_bSelected = bSelected;
	}
	if (nCount > 0)
	{
		m_iLastSelectedRow = pOutItems[nCount-1];
	}

	Invalidate();
	this->SendMessageOfSelectChanged();
}

void CBWReportCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE || nChar == VK_BACK)
	{
		DeleteSelectedRows();
	}
	else if (nChar == VK_UP || nChar == VK_NUMPAD8)
	{
		OnVScroll(SB_LINEUP, 0, NULL);	
	}
	else if (nChar == VK_DOWN || nChar == VK_NUMPAD2)
	{
		OnVScroll(SB_LINEDOWN, 0, NULL);	
	}
	else if (nChar == VK_PRIOR || nChar == VK_NUMPAD9)
	{
		OnVScroll(SB_PAGEUP, 0, NULL);	
	}
	else if (nChar == VK_NEXT || nChar == VK_NUMPAD3)
	{
		OnVScroll(SB_PAGEDOWN, 0, NULL);	
	}
	else if (nChar == VK_LEFT || nChar == VK_NUMPAD4)
	{
		OnHScroll(SB_LINELEFT, 0, NULL);	
	}
  	else if (nChar == VK_RIGHT || nChar == VK_NUMPAD6)
	{
		OnHScroll(SB_LINERIGHT, 0, NULL);	
	}
  
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBWReportCtrl::DeleteSelectedRows()
{
	if (!m_bEnableEdit)
		return;

	for (int i=GetRowCount()-1; i>=0; i--)
	{
		CReportRowItem* pRowData = m_arrayAllData.GetAt(i);
		if (pRowData->m_bSelected)
		{
			RemoveRow(i);
		}
	}
	
	m_iLastSelectedRow = -1;
	m_iRowCount = (UINT)m_arrayAllData.GetSize();
	Invalidate();

	SendMessageOfSelectChanged();
}

BOOL CBWReportCtrl::CanEdit(int nRow, int nCol)
{
	CReportItem* pItem = GetRowItem(nRow, nCol);
	if (pItem != NULL)
	{
		return m_bEnableEdit && pItem->m_bEnable;
	}

	return m_bEnableEdit;
}

BOOL CBWReportCtrl::SetRowData(int nRow, DWORD dwData)
{
	CReportRowItem* pRowData = (CReportRowItem*)m_arrayAllData.GetAt(nRow);
	pRowData->m_dwParam = dwData;
	return TRUE;
}

DWORD CBWReportCtrl::GetRowData(int nRow)
{
	CReportRowItem* pRowData = (CReportRowItem*)m_arrayAllData.GetAt(nRow);
	return pRowData->m_dwParam;
}

void CBWReportCtrl::SetOddEvenColor( COLORREF clrOdd, COLORREF clrEven )
{
	m_clrOdd = clrOdd;
	m_clrEven = clrEven;
	m_bOddEvenColor = TRUE;
}

void CBWReportCtrl::Print( CDC* pDC, const RECT& rect, float fScale )
{
	CFont fontPrint;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 72);
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("微软雅黑"), 10);
	fontPrint.CreateFontIndirect(&lf);
	pDC->SelectObject(&fontPrint);

	CRect rectPrint = rect;
	int nSpace = (rect.right - rect.left) /  20;
	rectPrint.left = rectPrint.left + nSpace;
	rectPrint.right = rectPrint.right - nSpace;

	int nRowPrintHeight = int(m_iHeaderHeight * fScale + 0.5);
	int nItemPrintWidth = rectPrint.Width() / (m_iFixedColCount + m_iColCount);

	CRect rectItem = rectPrint;
	rectItem.right = rectItem.left + nItemPrintWidth;
	rectItem.bottom = rectItem.top + nRowPrintHeight;

	// 打印标题
	tagReportColData* pColData = NULL;
	for (int i = 0; i < (m_iFixedColCount + m_iColCount); i++)
	{
		pColData = (tagReportColData*)m_arrayColData.GetAt(i);
		PrintHeadItem(pDC, rectItem, pColData->strText, pColData->nFormat, i == 0);
		rectItem.left = rectItem.right;
		rectItem.right = rectItem.left + nItemPrintWidth;
	}

	// 打印数据
	rectItem.left = rectPrint.left;
	rectItem.right = rectItem.left + nItemPrintWidth;
	rectItem.top = rectItem.bottom;
	rectItem.bottom = rectItem.top + nRowPrintHeight;

	CReportRowItem* pRowData = NULL;
	for (int i = 0; i < (m_iFixedRowCount + m_iRowCount); i++)
	{
		pRowData = m_arrayAllData.GetAt(i);

		for (int j = 0; j < (m_iFixedColCount + m_iColCount); j++)
		{
			PrintItem(pDC, rectItem, pRowData->GetChildAt(j), j == 0);
			rectItem.left = rectItem.right;
			rectItem.right = rectItem.left + nItemPrintWidth;
		}
		rectItem.left = rectPrint.left;
		rectItem.right = rectItem.left + nItemPrintWidth;
		rectItem.top = rectItem.bottom;
		rectItem.bottom = rectItem.top + nRowPrintHeight;
	}
}

void CBWReportCtrl::PrintItem( CDC* pDC, const RECT& rect, CReportItem* pItemData, BOOL IsFirst )
{
	if (IsFirst)
	{
		PrintLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom), RGB(0, 0, 0));
	}
	// 绘制右边线、下底线
	PrintLine(pDC, CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom), RGB(0, 0, 0));
	PrintLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), RGB(0, 0, 0));		
	// 绘制文本
	//CFont* pOldFont = pDC->SelectObject(&m_fontItem);
	DrawText(pDC, rect, pItemData->m_strText, pItemData->m_nFormat);
	//pDC->SelectObject(pOldFont);

	//if (bIsSelected)
	//pDC->SetTextColor(clrTextOld);
} // if (!bIsLocked)

void CBWReportCtrl::PrintHeadItem( CDC* pDC, const RECT& rect, CString strText, UINT format, BOOL IsFirst )
{
	if(IsFirst)
	{
		PrintLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom), RGB(0, 0, 0));
	}
	PrintLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), RGB(0, 0, 0));
	PrintLine(pDC, CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom), RGB(0, 0, 0));
	PrintLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top), RGB(0, 0, 0));		
	// 绘制文本
	//CFont* pOldFont = pDC->SelectObject(&m_fontItem);
	DrawText(pDC, rect, strText, format);
}

void CBWReportCtrl::PrintLine( CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF clrPen /*= RGB(0,0,0)*/ )
{
	CPen pen(PS_SOLID, 2, clrPen);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);

	pDC->SelectObject(pOldPen);
}

BOOL CBWReportCtrl::PreTranslateMessage( MSG* pMsg )
{
	if ((pMsg->message == WM_KEYDOWN) && (VK_RETURN == (int)pMsg->wParam))
	{
		pMsg->wParam = VK_TAB;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

CBWReportCtrl::CReportItem* CBWReportCtrl::GetRowItem(int iRow, int iCol)
{
	if (iRow < 0 || iRow > (int)m_iRowCount || iCol < 0 || iCol > m_iColCount)
		return NULL;

	CReportRowItem* pRowData = (CReportRowItem*)m_arrayAllData.GetAt(iRow);
	CReportItem* pItemData = pRowData->GetChildAt(iCol);
	return pItemData;
}

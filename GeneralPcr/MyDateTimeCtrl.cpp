// MyDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyDateTimeCtrl.h"

// CMyDateTimeCtrl

IMPLEMENT_DYNAMIC(CMyDateTimeCtrl, CDateTimeCtrl)

CMyDateTimeCtrl::CMyDateTimeCtrl()
{
	m_bTellParentDtChanged = FALSE;
	m_iDtTypeTag = TRUE;
}

CMyDateTimeCtrl::~CMyDateTimeCtrl()
{

}

BEGIN_MESSAGE_MAP(CMyDateTimeCtrl, CDateTimeCtrl)
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(DTN_DATETIMECHANGE,OnDtnDatetimechange)
END_MESSAGE_MAP()

void CMyDateTimeCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void CMyDateTimeCtrl::OnNcPaint()
{
	
}

void CMyDateTimeCtrl::OnPaint()
{
	CPaintDC dc(this); 

	CRect rect;
	GetClientRect(&rect);
	
	int iIconW = 40 * m_dHoriRatio;
	int iIconH = iIconW;

	CDC memDc;
	memDc.CreateCompatibleDC(&dc);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(&rect,RGB(255,255,255));

	CBrush brush;
	brush.CreateSolidBrush(RGB(197,197,197));
	memDc.FrameRect(&rect,&brush);
	brush.DeleteObject();

	Graphics graphics(memDc.m_hDC);
	Image image(_T(".\\BkPic\\date.png"));

	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);

	Rect destRect;
	destRect.X = rect.Width() - image.GetWidth() - 10;
	destRect.Y = (rect.Height() - image.GetHeight()) / 2;
	destRect.Width = image.GetWidth();
	destRect.Height = image.GetHeight();

	graphics.DrawImage(&image,destRect,0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&ImgAtt);
	graphics.ReleaseHDC(memDc.m_hDC);
	
	COleDateTime curOleDateTime;
	GetTime(curOleDateTime);

	CString strText(curOleDateTime.Format(_T("%Y/%m/%d")));
	if(!strText.IsEmpty())
	{
		CRect rcText = rect;
		rcText.left += 5 * m_dHoriRatio;
		rcText.right -= (iIconW + 10 * m_dHoriRatio);

		int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
		CFont* pOldFont = memDc.SelectObject(&m_fontSet);
		memDc.SetTextColor(RGB(86,86,86));
		memDc.DrawText(strText,&rcText,DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		memDc.SelectObject(&pOldFont);
		memDc.SetBkMode(nOldBkMode);
	}	

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);
	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return;
}

void CMyDateTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}


BOOL CMyDateTimeCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyDateTimeCtrl::OnDtnDatetimechange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	*pResult = 0;
	Invalidate(FALSE);

	CWnd* pParent = GetParent();
	if(m_bTellParentDtChanged && NULL != pParent->GetSafeHwnd())
	{
		pParent->PostMessage(WM_DATECTRLCHANGED,m_iDtTypeTag,0);
	}

	return;
}

void CMyDateTimeCtrl::SetTellParentDtChangedParam(BOOL bTell,int iDtTypeTag)
{
	m_bTellParentDtChanged = bTell;
	m_iDtTypeTag = iDtTypeTag;

	return;
}

BOOL CMyDateTimeCtrl::IsTllParentDtChanged()
{
	return m_bTellParentDtChanged;
}

int CMyDateTimeCtrl::GetDtTypeTag()
{
	return m_iDtTypeTag;
}


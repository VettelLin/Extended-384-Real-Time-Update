#include "StdAfx.h"
#include "Ps96IvdFileManagerDlg.h"


CPs96IvdFileManagerDlg::CPs96IvdFileManagerDlg(CWnd* pParent /*= NULL*/)
{
	m_pWorkDlg = (CWorkDlg*)pParent;
	m_iAddIconWHFat = 16;
	m_iAddIconWHThin = 4;
	m_iAddIconLeftIndent = 5;
	m_iRightIndent = 290;
}

CPs96IvdFileManagerDlg::~CPs96IvdFileManagerDlg(void)
{

}

BEGIN_MESSAGE_MAP(CPs96IvdFileManagerDlg,CFileManageDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CPs96IvdFileManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(0XFF,0XFF,0XFF));

	//»æÖÆlogo
	Graphics graphics(memDc.m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	Rect destRect;
	destRect.X = rect.right - m_iRightIndent + m_iAddIconWHFat + 12 * m_dHoriRatio;
	destRect.Y = 2 * m_dVertRatio;
	destRect.Width = 76 * m_dHoriRatio;
	destRect.Height = 40 * m_dVertRatio;
	graphics.DrawImage(&m_imgLogo,destRect,0,0,m_imgLogo.GetWidth(),m_imgLogo.GetHeight(),UnitPixel,&ImgAtt);

	graphics.ReleaseHDC(memDc.m_hDC);

	//»æÖÆÈí¼þ°æ±¾ºÅ£¬20220513
	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);
	memDc.DrawText(SOFRVERION_STRING,CRect(destRect.GetRight(),0,rect.right,rect.bottom),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

	//»æÖÆÌí¼ÓÒ³Âë·ûºÅ
	if(!m_rcAddIcon.IsRectEmpty())
	{
		int iWH = 4 * m_dHoriRatio;

		memDc.FillSolidRect(m_rcAddIcon.left,m_rcAddIcon.top + (m_rcAddIcon.Height() - iWH) / 2,m_rcAddIcon.Width(),iWH,CLR_SOFT_THEME);
		memDc.FillSolidRect(m_rcAddIcon.left + (m_rcAddIcon.Width() - iWH) / 2,m_rcAddIcon.top,iWH,m_rcAddIcon.Height(),CLR_SOFT_THEME);
	}

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	clock_t ckStart = clock();	
	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	ckStart = clock() - ckStart;

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

int CPs96IvdFileManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iAddIconWHFat *= m_dHoriRatio;
	m_iAddIconWHThin *= m_dHoriRatio;
	m_iAddIconLeftIndent *= m_dHoriRatio;

	return CFileManageDlg::OnCreate(lpCreateStruct);
}

void CPs96IvdFileManagerDlg::OnLButtonDown(UINT nFlags,CPoint point)
{
	if(m_rcAddIcon.PtInRect(point))
	{
		AddPage();
	}

	return CDialogEx::OnLButtonDown(nFlags, point);
}

void CPs96IvdFileManagerDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iStartX = m_iLeftIndent;
	for(int i = 0;i < m_lnkPageSwitchBtPtr.GetSize();++i)
	{
		(*m_lnkPageSwitchBtPtr[i])->SetWindowPos(this,iStartX,BT_TOP_START,m_iBtDefWidth,m_iBtHeight,SWP_NOZORDER);
		iStartX += m_iBtDefWidth;
	}

	int iBtTotalW = m_iBtRealWidth * m_lnkPageSwitchBtPtr.GetSize();

	m_rcAddIcon.left = iBtTotalW + m_iAddIconLeftIndent;
	m_rcAddIcon.right = iBtTotalW + m_iAddIconWHFat;
	m_rcAddIcon.top = BT_TOP_START + (m_iBtHeight - m_iAddIconWHFat) / 2;
	m_rcAddIcon.bottom = m_rcAddIcon.top + m_iAddIconWHFat;

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();
	m_rcManage.AddRectToDelete(iStartX,BT_TOP_START,iBtTotalW,m_iBtHeight);
	m_rcManage.CalcRectResult();

	return;
}

void CPs96IvdFileManagerDlg::SetPageSwitchBtPos(int iStart)
{
	CFileManageDlg::SetPageSwitchBtPos(iStart);

	m_rcAddIcon.left = m_iLeftIndent + m_iBtRealWidth * m_lnkPageSwitchBtPtr.GetSize() + m_iAddIconLeftIndent;
	m_rcAddIcon.right = m_rcAddIcon.left + m_iAddIconWHFat;
	m_rcAddIcon.top = BT_TOP_START + (m_iBtHeight - m_iAddIconWHFat) / 2;
	m_rcAddIcon.bottom = m_rcAddIcon.top + m_iAddIconWHFat;

	return;
}

int CPs96IvdFileManagerDlg::AddNewPage(CExperimentData* pExpData,CString strFileName /*= _T("")*/)
{
	int iRet = CFileManageDlg::AddNewPage(pExpData,strFileName);
	Invalidate();
	return iRet;
}

int CPs96IvdFileManagerDlg::DeletePage(int pageId)
{
	int iRet = CFileManageDlg::DeletePage(pageId);
	Invalidate();
	return iRet;
}


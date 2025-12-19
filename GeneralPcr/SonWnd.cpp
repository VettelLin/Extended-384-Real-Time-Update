// SonWnd.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "SonWnd.h"


// CSonWnd

IMPLEMENT_DYNAMIC(CSonWnd, CDialogEx)

CSonWnd::CSonWnd()
{

}

CSonWnd::~CSonWnd()
{
}


BEGIN_MESSAGE_MAP(CSonWnd, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CSonWnd::OnPaint()
{
	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient,RGB(255,0,0));

	return;
}

BOOL CSonWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSonWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
		ReleaseCapture();
	}
	return;
}

void CSonWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
		ReleaseCapture();
	}
	return;
}

void CSonWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(point))
	{
		int tt = 0;
		++tt;
	}

	return;
}


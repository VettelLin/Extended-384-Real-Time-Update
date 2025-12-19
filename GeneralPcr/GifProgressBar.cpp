// Gif.cpp : implementation file
//

#include "stdafx.h"
#include "GifProgressBar.h"

// CGifProgressBar

IMPLEMENT_DYNAMIC(CGifProgressBar, CStatic)

CGifProgressBar::CGifProgressBar()
{
	m_pImg = NULL;
	m_gifFilePath = _T("");
	m_dimenCount = 0;
	m_pDimenIds = NULL;
	m_frameCount = 0;
	m_pPropertyItem = NULL;
	m_curCount = 0;	//当前显示的是哪一帧
	m_bFstPaint = TRUE;
	m_pMemDc = NULL;
	m_hBitmap = NULL;
	m_hGdiObj = NULL;
}

CGifProgressBar::~CGifProgressBar()
{
	if(NULL != m_pImg)
	{
		delete m_pImg;
		m_pImg = NULL;
	}
	
	if(NULL != m_pPropertyItem)
	{
		delete m_pPropertyItem;
		m_pPropertyItem = NULL;
	}

	if(NULL != m_pMemDc)
	{
		m_pMemDc->SelectObject(m_hGdiObj);
		m_pMemDc->DeleteDC();
		delete m_pMemDc;
		m_pMemDc = NULL;

		if(NULL != m_hBitmap)
		{
			::DeleteObject(m_hBitmap);
		}
	}
}

BEGIN_MESSAGE_MAP(CGifProgressBar, CStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CGifProgressBar message handlers

void CGifProgressBar::OnPaint()
{
	CPaintDC dc(this); 

#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CRect rcClient;
	GetClientRect(&rcClient);

	if(m_bFstPaint) 
	{
		m_pMemDc = new CDC;
		m_pMemDc->CreateCompatibleDC(&dc);
		m_hBitmap = ::CreateCompatibleBitmap(dc.m_hDC,rcClient.Width(),rcClient.Height()); //创建兼容位图
		HGDIOBJ m_hGdiObj = m_pMemDc->SelectObject(m_hBitmap);
		m_pMemDc->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&dc,0,0,SRCCOPY);

		m_bFstPaint = FALSE;
	}

	CDC memDc;
	memDc.CreateCompatibleDC(&dc);
	HBITMAP hBmp = ::CreateCompatibleBitmap(dc.m_hDC,rcClient.Width(),rcClient.Height()); //创建兼容位图
	HGDIOBJ hOldGdiObject = memDc.SelectObject(hBmp);
	memDc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),m_pMemDc,0,0,SRCCOPY);

	memDc.FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),RGB(255,255,255));

	Graphics gc(memDc.m_hDC);

	m_pImg->SelectActiveFrame(&m_guid,m_curCount);
	RectF destRectF(0,0,rcClient.Width(),rcClient.Height());
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	if(m_pImg->GetWidth() > rcClient.Width() && m_pImg->GetHeight() > rcClient.Height())
	{
		gc.DrawImage(m_pImg,destRectF,(m_pImg->GetWidth() - rcClient.Width()) / 2,(m_pImg->GetHeight() - rcClient.Height()) / 2 - 6,rcClient.Width(),rcClient.Height(),UnitPixel,&ImgAtt);
	}
	else
	{
		gc.DrawImage(m_pImg,destRectF,0,0,m_pImg->GetWidth(),m_pImg->GetHeight(),UnitPixel,&ImgAtt);
	}
	
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDc,0,0,SRCCOPY);
	memDc.SelectObject(hOldGdiObject);
	::DeleteObject(hBmp);
	memDc.DeleteDC();

#ifdef _DEBUG
	clock_t endMs = clock();
	if(endMs - startMs > 15)
	{
		int k = 0;
		++k;
	}
#endif
}

void CGifProgressBar::OnDestroy()
{
	CStatic::OnDestroy();
}

BOOL CGifProgressBar::SetGifFilePath(wstring gifFilePath,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == GetSafeHwnd())
	{
		return FALSE;
	}

	if(NULL != m_pImg)
	{
		delete m_pImg;
		m_pImg = NULL;
	}
	if(NULL != m_pPropertyItem)
	{
		delete m_pPropertyItem;
		m_pPropertyItem = NULL;
	}

	m_gifFilePath = gifFilePath;
	m_pImg = Image::FromFile(m_gifFilePath.c_str());
	m_dimenCount = m_pImg->GetFrameDimensionsCount();
	GUID* pDimenIds = (GUID*)new GUID[m_dimenCount];
	m_pImg->GetFrameDimensionsList(pDimenIds,m_dimenCount);
	TCHAR strGuid[39] = {0};
	StringFromGUID2(pDimenIds[0],strGuid,39);
	m_frameCount = m_pImg->GetFrameCount(&(pDimenIds[0]));
	delete pDimenIds;
	pDimenIds = NULL;
	int size = m_pImg->GetPropertyItemSize(PropertyTagFrameDelay);
	byte* p = new byte[size];  
	m_pPropertyItem = (PropertyItem*)p;
	m_pImg->GetPropertyItem(PropertyTagFrameDelay,size,m_pPropertyItem);
	m_guid = FrameDimensionTime;

	m_curCount = 0;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return TRUE;
}

wstring CGifProgressBar::GetGifFilePath()
{
	return m_gifFilePath;
}

UINT CGifProgressBar::GetDimenCount()
{
	return m_dimenCount;
}

const GUID* CGifProgressBar::GetDimenIdsPtr()
{
	return m_pDimenIds;
}

UINT CGifProgressBar::GetFrameCount()
{
	return m_frameCount;
}

const PropertyItem* CGifProgressBar::GetPropertyItemPtr()
{
	return m_pPropertyItem;
}

GUID& CGifProgressBar::GetGifGuid()
{
	return m_guid;
}

void CGifProgressBar::SetCurFrame(int curFrame,BOOL bRedraw /*= FALSE*/)
{
	if(curFrame < 0)
	{
		return;
	}

	if(curFrame >= m_frameCount)
	{
		curFrame = m_frameCount - 1;
	}

	m_curCount = curFrame;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}
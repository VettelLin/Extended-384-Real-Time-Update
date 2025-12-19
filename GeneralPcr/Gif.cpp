// Gif.cpp : implementation file
//

#include "stdafx.h"
#include "Gif.h"

// CGif

IMPLEMENT_DYNAMIC(CGif, CStatic)

CGif::CGif()
{
	m_pImg = NULL;
	m_gifFilePath = _T("");
	m_dimenCount = 0;
	m_pDimenIds = NULL;
	m_frameCount = 0;
	m_pPropertyItem = NULL;
	m_curCount = 0;	//当前显示的是哪一帧
	m_bGifRun = FALSE;
	m_timerId = 1;
	m_bFstPaint = TRUE;
	m_pMemDc = NULL;
	m_hBitmap = NULL;
	m_hGdiObj = NULL;
}

CGif::~CGif()
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

BEGIN_MESSAGE_MAP(CGif, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CGif message handlers

void CGif::OnPaint()
{
	CPaintDC dc(this); 
	if(m_bGifRun)
	{
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

		Graphics gc(memDc.m_hDC);
		m_pImg->SelectActiveFrame(&m_guid,m_curCount++);
		RectF destRectF(0,0,rcClient.Width(),rcClient.Height());
		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);
		gc.DrawImage(m_pImg,destRectF,0,0,m_pImg->GetWidth(),m_pImg->GetHeight(),UnitPixel,&ImgAtt);
		dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDc,0,0,SRCCOPY);
		memDc.SelectObject(hOldGdiObject);
		::DeleteObject(hBmp);
		memDc.DeleteDC();

		if(m_curCount == m_frameCount)
		{
			m_curCount = 0;
		}

#ifdef _DEBUG
		clock_t endMs = clock();
		if(endMs - startMs > 15)
		{
			int k = 0;
			++k;
		}
#endif
	}
}

void CGif::OnTimer(UINT_PTR nIDEvent)
{
	if(0 == KillTimer(nIDEvent))
	{
		CMyGeneTipInfo geneTipInfo;
		geneTipInfo.m_strTipInfo = _T("定时器关闭失败");
		PopTipInfo(geneTipInfo);
	}

	if(m_timerId == nIDEvent)
	{
		Invalidate();
		UpdateWindow();
 		if(0 == (m_timerId = SetTimer(nIDEvent,((long*)m_pPropertyItem->value)[m_curCount] * 6,NULL)))
		{
			CMyGeneTipInfo geneTipInfo;
			geneTipInfo.m_strTipInfo = _T("定时器启动失败");
			PopTipInfo(geneTipInfo);
		}
	}
}

void CGif::OnDestroy()
{
	if(m_bGifRun)
	{
		if(0 == KillTimer(m_timerId))
		{
			AfxMessageBox(_T("定时器关闭失败！"));
		}
	}

	CStatic::OnDestroy();
}

BOOL CGif::SetGifFilePath(wstring gifFilePath,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == GetSafeHwnd())
	{
		return FALSE;
	}

	if(m_bGifRun)
	{
		m_bGifRun = FALSE;
		if(0 == KillTimer(m_timerId))
		{
			AfxMessageBox(_T("SetGifFilePath多次执行时定时器关闭失败"));
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
	}

	m_gifFilePath = gifFilePath;
	m_pImg = Image::FromFile(m_gifFilePath.c_str());
	//if (m_pImg->lastResult != Ok)
	//{

	//}
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

	m_bGifRun = TRUE;
	if(0 == (m_timerId = SetTimer(m_timerId,((long*)m_pPropertyItem->value)[m_curCount],NULL)))
	{
		AfxMessageBox(_T("定时器启动失败"));
	}

	if(bRedraw)
	{
		Invalidate();
	}

	return TRUE;
}

wstring CGif::GetGifFilePath()
{
	return m_gifFilePath;
}

void CGif::SetTimerID(UINT_PTR timerId)
{
	m_timerId = timerId;
}

UINT_PTR CGif::GetTimerID()
{
	return m_timerId;
}

UINT CGif::GetDimenCount()
{
	return m_dimenCount;
}

const GUID* CGif::GetDimenIdsPtr()
{
	return m_pDimenIds;
}

UINT CGif::GetFrameCount()
{
	return m_frameCount;
}

const PropertyItem* CGif::GetPropertyItemPtr()
{
	return m_pPropertyItem;
}

GUID& CGif::GetGifGuid()
{
	return m_guid;
}

BOOL CGif::Start()
{
	if(NULL == GetSafeHwnd())
	{
		return FALSE;
	}

	if(!m_bGifRun)
	{
		if(0 == (m_timerId = SetTimer(m_timerId,((long*)m_pPropertyItem->value)[m_curCount],NULL)))
		{
			AfxMessageBox(_T("定时器启动失败"));
			return FALSE;
		}
		m_bGifRun = TRUE;
	}
	return TRUE;
}

BOOL CGif::Stop()
{
	if(NULL == GetSafeHwnd())
	{
		return FALSE;
	}

	if(m_bGifRun)
	{
		if(0 == KillTimer(m_timerId))
		{
			AfxMessageBox(_T("定时器关闭失败！"));
			return FALSE;
		}
		m_bGifRun = FALSE;
	}
	return TRUE;
}
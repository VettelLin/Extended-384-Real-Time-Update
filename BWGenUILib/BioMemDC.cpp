#include "StdAfx.h"
#include "BioMemDC.h"

IMPLEMENT_DYNAMIC(CBioMemDC, CDC)

CBioMemDC::CBioMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor)
{
	ASSERT(pDC != NULL);
	m_pDC = pDC;
	
	// If rect is NULL, use the device context's clip box.
	if (rect.IsRectEmpty())
		m_pDC->GetClipBox(&m_rc);
	else
		m_rc.CopyRect(&rect);

    // Create the memory DC, set Map Mode
	if (CreateCompatibleDC(m_pDC))
	{
		SetMapMode(m_pDC->GetMapMode());
		
		// Create a bitmap big enough to hold the window's image
		m_bitmap.CreateCompatibleBitmap(m_pDC, m_rc.Width(), m_rc.Height());
		
		// Select the bitmap into the memory DC
		m_hOldBitmap = (HBITMAP)SelectObject(&m_bitmap)->GetSafeHandle();
		
		// Repaint the background, this takes the place of WM_ERASEBKGND.
		if (clrColor != -1)
		{
			//P.Baranov
			CRect rc( m_rc );

			rc.left    = 0;
			rc.right  -= m_rc.left;
			rc.top     = 0;
			rc.bottom -= m_rc.top;

			FillSolidRect(rc, clrColor);
		}
		
		m_bValid = TRUE;
	}

	// Something bad happened, could be GDI leak, didn't create device context.
	else
	{
		m_bValid = FALSE;
		m_hOldBitmap = NULL;
	}
}

CBioMemDC::~CBioMemDC(void)
{
	if (m_bValid)
	{
		// Blt it
		m_pDC->BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), this, 0, 0, SRCCOPY);
	}

	// Select the original bitmap back in
	if (m_hOldBitmap != NULL)
	{
		SelectObject(m_hOldBitmap);
		m_bitmap.DeleteObject();
	}

	DeleteDC();
}

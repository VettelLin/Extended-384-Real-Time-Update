#pragma once

#include <afxwin.h>

class AFX_EXT_CLASS CBioMemDC : public CDC
{
    DECLARE_DYNAMIC(CBioMemDC);
public:
	CBioMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor=::GetSysColor(COLOR_3DFACE));
	~CBioMemDC(void);

	void Discard();

 	void FromDC();
	CBitmap& GetBitmap();

protected:
	CDC* m_pDC; // Saves the CDC passed in constructor.
	CRect m_rc;  // Rectangle of the drawing area.
	CBitmap m_bitmap;  // Offscreen bitmap.
	HBITMAP m_hOldBitmap; // Original GDI object.
	BOOL m_bValid; // Flag used for autodraw in destructor.
};

#pragma once

#include<GdiPlus.h>
#include <xstring>

using namespace Gdiplus;
using namespace std;
// CGifProgressBar

class CGifProgressBar : public CStatic
{
	DECLARE_DYNAMIC(CGifProgressBar)

public:
	CGifProgressBar();
	virtual ~CGifProgressBar();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
			BOOL SetGifFilePath(wstring gifFilePath,BOOL bRedraw = FALSE);	//窗口创建后该函数才有效
			wstring GetGifFilePath();
	
			UINT GetDimenCount();
	const GUID* GetDimenIdsPtr();
			UINT GetFrameCount();
	const PropertyItem* GetPropertyItemPtr();
			GUID& GetGifGuid();

			void SetCurFrame(int curFrame,BOOL bRedraw = FALSE);

private:
	wstring m_gifFilePath;
	UINT m_dimenCount;
	GUID* m_pDimenIds;
	UINT m_frameCount;
	PropertyItem* m_pPropertyItem;
	GUID m_guid;

	Image* m_pImg;

	int m_curCount;	//当前显示的是哪一帧

	BOOL m_bFstPaint;
	CDC* m_pMemDc;

public:
	HBITMAP m_hBitmap;	//第一次绘制之前的背景图
	HGDIOBJ m_hGdiObj;
};



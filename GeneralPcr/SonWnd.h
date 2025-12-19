#pragma once


// CSonWnd

class CSonWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CSonWnd)

public:
	CSonWnd();
	virtual ~CSonWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



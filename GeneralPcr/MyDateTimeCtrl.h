#pragma once

#include "FontSet.h"

// CMyDateTimeCtrl

class CMyDateTimeCtrl : public CDateTimeCtrl,public CFontSet
{
	DECLARE_DYNAMIC(CMyDateTimeCtrl)

public:
	CMyDateTimeCtrl();
	virtual ~CMyDateTimeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDtnDatetimechange(NMHDR *pNMHDR, LRESULT *pResult);

			void SetTellParentDtChangedParam(BOOL bTell,int iDtTypeTag);
			BOOL IsTllParentDtChanged();
			int GetDtTypeTag();

protected:
	BOOL m_bTellParentDtChanged;	//通知父窗口日期已变更
	int m_iDtTypeTag;
};



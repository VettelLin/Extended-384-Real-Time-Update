#pragma once

#include "MyNewEdit.h"
#include "Ps96SetDlg.h"

class CCYNormSetDlg :public CPs96SetDlg
{
	DECLARE_DYNAMIC(CCYNormSetDlg)

public:
	CCYNormSetDlg(void);
	~CCYNormSetDlg(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam,LPARAM lParam);

	virtual void CreateEd();
	virtual void InitEd();

	virtual void SetCtrlPos();
	virtual void ResetCtrlPos();
	virtual void ShowCtrl();

	virtual void SetCtrlText();

	virtual int RefreshLanguageDisplay();

public:
	CMyNewEdit m_edClient;	//委托单位
	CMyNewEdit m_edCpAdr;	//委托单位的地址
};


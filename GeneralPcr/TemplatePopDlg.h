#pragma once

#include "FontSet.h"
#include "Resource.h"

class CTemplatePopDlg: public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CTemplatePopDlg)

public:
	CTemplatePopDlg(CWnd* pParent = NULL);
	~CTemplatePopDlg(void);

	enum {IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();

			void InitDlg();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateCb();
			void CreateEd();
			void CreateLst();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitCb();
			void InitEd();
			void InitLst();
			void SetCtrlPos();
			void ShowCtrl();

public:
	CString m_strTitle;

private:
	int m_iWidth;	//窗口宽度
	int m_iHeight;	//窗口高度
	int m_iHeaderH;	//标题高度

	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度
};


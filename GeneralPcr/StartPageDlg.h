#pragma once

#include "FontSet.h"
#include "MyStatic.h"
#include "Gif.h"
#include "Resource.h"

class CStartPageDlg: public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CStartPageDlg)

public:
	CStartPageDlg(CWnd* pParent = NULL);
	~CStartPageDlg(void);

	enum {IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMsgDiscernInstrumentRst(WPARAM wParam,LPARAM lParam);

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

	static	UINT DiscernInstrumentTd(LPVOID pParam);	//仪器识别线程

public:
	CString m_strStartPath;
	CString m_strWarningTip;	//警告信息（自检结果）
	vector<CString> m_vecInstruSerial;

	CMyStatic m_stTipInfo;	//提示信息
	CGif m_gifHanding;	//处理中gif

private:
	int m_iWidth;	//窗口宽度
	int m_iHeight;	//窗口高度
	int m_iHeaderH;	//标题高度

	int m_iRoundAngle;	//圆角数值
	int m_iFrameW;	//边框宽度
};


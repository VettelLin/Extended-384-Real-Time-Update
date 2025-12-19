////////////////////////////////////////////////////////////////
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
#pragma once
//
#include <AFXHTML.H>
#include "WebBrowserPrint.h"

class CHtmlCtrl : public CHtmlView
{
public:
	CHtmlCtrl() {m_bPrintPreview=FALSE; }
	~CHtmlCtrl() { }

	BOOL CreateFromStatic(UINT nID, CWnd* pParent);

	// Normally, CHtmlView destroys itself in PostNcDestroy,
	// but we don't want to do that for a control since a control
	// is usually implemented as a stack object in a dialog.
	//
	virtual void PostNcDestroy() {  }

	// overrides to bypass MFC doc/view frame dependencies
	afx_msg void OnDestroy();
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT nIDEvent);

	// override to trap "app:" pseudo protocol
	virtual void OnBeforeNavigate2( LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel );

	// override to handle links to "app:mumble...". lpszWhere will be "mumble"
	virtual void OnAppCmd(LPCTSTR lpszWhere);

	//to determin if print preview the source
	virtual void OnDownloadComplete( ){}
	virtual void OnDocumentComplete(LPCTSTR);

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(CHtmlCtrl)

public:
	//打印设置
	void InitWebBrowserPrint(CString strHeader = _T(""),CString strFooter = _T(""),BOOL bLandScapeOrientation = FALSE);
	//打印动作
	void PrintPreview();

	void Navigate2AsPrintPreview(CString strURL,BOOL bPrintPreview = TRUE);

	BOOL IsPrinterExit()
	{
		return m_WebBrowserPrint.IsPrinterExist() ;
	}

private:
	//负责打印
	CWebBrowserPrint m_WebBrowserPrint;
	BOOL m_bPrintPreview;
};


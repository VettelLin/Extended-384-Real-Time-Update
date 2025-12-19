////////////////////////////////////////////////////////////////
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
// ---
// AboutHtml shows how to implement an HTML About Dialog using a
// new class, CHtmlCtrl, that lets you use CHtmlView as a control in a dialog.


#include "StdAfx.h"
#include "HtmlCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CHtmlCtrl, CHtmlView)
BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////
// Create control in same position as an existing static control with
// the same ID (could be any kind of control, really)
//
BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
{
	//CStatic wndStatic;
	//if (!wndStatic.SubclassDlgItem(nID, pParent))
	//	return FALSE;

	// Get static control rect, convert to parent's client coords.
	CRect rc(0,0,0,0);
	//wndStatic.GetWindowRect(&rc);
	//pParent->ScreenToClient(&rc);
	//wndStatic.DestroyWindow();

	// create HTML control (CHtmlView)
	return Create(NULL,						 // class name
		NULL,										 // title
		(WS_CHILD /*| WS_VISIBLE*/ ),			 // style
		rc,										 // rectangle
		pParent,									 // parent
		nID,										 // control ID
		NULL);									 // frame/doc context not used
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
void CHtmlCtrl::OnDestroy()
{
//	// This is probably unecessary since ~CHtmlView does it, but
//	// safer to mimic CHtmlView::OnDestroy.
//	if (m_pBrowserApp) {
//		m_pBrowserApp->Release();
//		m_pBrowserApp = NULL;
//	}
	CWnd::OnDestroy(); // bypass CView doc/frame stuff
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	// bypass CView doc/frame stuff
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

//////////////////
// Override navigation handler to pass to "app:" links to virtual handler.
// Cancels the navigation in the browser, since app: is a pseudo-protocol.
//
void CHtmlCtrl::OnBeforeNavigate2( LPCTSTR lpszURL,
	DWORD nFlags,
	LPCTSTR lpszTargetFrameName,
	CByteArray& baPostedData,
	LPCTSTR lpszHeaders,
	BOOL* pbCancel )
{
	const TCHAR APP_PROTOCOL[] = _T("app:");
	int len = int(_tcslen(APP_PROTOCOL));
	if (_tcsnicmp(lpszURL, APP_PROTOCOL, len)==0) {
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
	}
}

//////////////////
// Called when the browser attempts to navigate to "app:foo"
// with "foo" as lpszWhere. Override to handle app commands.
//
void CHtmlCtrl::OnAppCmd(LPCTSTR lpszWhere)
{
	// default: do nothing
}

void CHtmlCtrl::InitWebBrowserPrint(CString strHeader,CString strFooter ,BOOL bLandScapeOrientation )
{
	m_WebBrowserPrint.SetWebBrowser(m_pBrowserApp);
	if (bLandScapeOrientation)
	{
		m_WebBrowserPrint.m_Orientation = CWebBrowserPrint::OrientationLandscape;
	}
	else
	{
		m_WebBrowserPrint.m_Orientation = CWebBrowserPrint::OrientationPortrait;
	}
	m_WebBrowserPrint.m_fTopMargin = 10;
	m_WebBrowserPrint.m_fRightMargin = 10;
	m_WebBrowserPrint.m_fLeftMargin = 8;
	m_WebBrowserPrint.m_fBottomMargin = 10;
	m_WebBrowserPrint.m_sHeader = (strHeader);
	m_WebBrowserPrint.m_sFooter = (strFooter);
	m_WebBrowserPrint.m_lCopies = 1;

}
void CHtmlCtrl::PrintPreview()
{
//	if (!m_WebBrowserPrint.ReadDlgSettings())
//		return;

//	m_WebBrowserPrint.m_Orientation = CWebBrowserPrint::OrientationPortrait;
//	m_WebBrowserPrint.m_fTopMargin = 10;
//	m_WebBrowserPrint.m_fRightMargin = 10;
//	m_WebBrowserPrint.m_fLeftMargin = 8;
//	m_WebBrowserPrint.m_fBottomMargin = 10;
//	m_WebBrowserPrint.m_sHeader = _T("");
//	m_WebBrowserPrint.m_sFooter = _T("");
//	m_WebBrowserPrint.m_lCopies = 1;

	CString sPrinterName;
	ULONG count = m_WebBrowserPrint.GetPrinterCount();
	for (ULONG i = 0; i < count; ++i)
		sPrinterName = m_WebBrowserPrint.GetPrinterName(i);

	m_WebBrowserPrint.m_sPrinterName = sPrinterName;  // last printer in list!	

	m_WebBrowserPrint.Print();
}
//void CHtmlCtrl::OnDownloadComplete( 	)
void CHtmlCtrl::OnDocumentComplete(LPCTSTR)
{
	if (m_bPrintPreview) 
	{
		PrintPreview ();
		m_bPrintPreview = FALSE ;
	}
}
void CHtmlCtrl::Navigate2AsPrintPreview(CString strURL,BOOL bPrintPreview)
{
	m_bPrintPreview = bPrintPreview;
	Navigate2((strURL),navNoReadFromCache  ,NULL);
//	Refresh2(REFRESH_NORMAL);
//	SetTimer (1, 1, NULL) ;
}
void CHtmlCtrl::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		PrintPreview () ;
		KillTimer (1) ;
	}

	CHtmlView::OnTimer(nIDEvent);
}

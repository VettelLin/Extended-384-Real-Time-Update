#include "stdafx.h"
#include "BioEdit.h"
#include "Resource.h"
#include <Float.h>
#include "BWGenUILib.h"

// CBioEdit

CBioEdit::CBioEdit()
{
	m_bIsMinus = FALSE;
	m_nMode = EditMode_Normal;
	m_nButtonWidth = 20;
	m_rectButton.SetRectEmpty ();
	m_bIsButtonPressed = FALSE;
	m_bIsButtonHighlighted = FALSE;
	m_bIsButtonCaptured = FALSE;
	m_sizeImage = CSize (0, 0);

	m_strValue.Empty();
	m_dMinValue = -DBL_MAX;
	m_dMaxValue = DBL_MAX;
	m_dStepValue = 1;
	m_nPrecision = 0;

	m_bIsInTop = FALSE;
	m_nTimer = 1;
	m_bAutoCheck = TRUE;

	m_strValue.Empty();
	m_nSleepTime = 500;

	m_bWantAllKeys = FALSE;
}

CBioEdit::~CBioEdit()
{
}

BEGIN_MESSAGE_MAP(CBioEdit, CEdit)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	//ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

// CBioEdit message handlers
void CBioEdit::OnNcPaint() 
{
	CEdit::OnNcPaint();

	if (m_nMode == EditMode_Normal || m_nMode == EditMode_Number)
	{
		return;
	}

	CWindowDC dc(this);

	CRect rectWindow;
	GetWindowRect(&rectWindow);

	m_rectButton = rectWindow;
	m_rectButton.left = m_rectButton.right -  m_nButtonWidth;

	CRect rect = m_rectButton;

	rect.OffsetRect(-rectWindow.left, -rectWindow.top);

	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect (&rect);

	dc.SelectClipRgn(&rgnClip);
	
	if ((GetStyle () & WS_BORDER) || (GetExStyle () & WS_EX_WINDOWEDGE) ||
		(GetExStyle () & WS_EX_CLIENTEDGE))
	{
		rect.DeflateRect (0, 1, 1, 1);
	}

	DrawButton(&dc, rect, m_bIsButtonPressed, m_bIsButtonHighlighted);
	dc.SelectClipRgn (NULL);

	ScreenToClient(&m_rectButton);
}

void CBioEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((m_nMode != EditMode_Normal && m_nMode != EditMode_Number) && m_rectButton.PtInRect(point))
	{
		SetFocus();

		m_bIsButtonPressed = TRUE;
		m_bIsButtonCaptured = TRUE;

		SetCapture();

		if (m_nMode == EditMode_Spin && m_bAutoCheck)
		{
			m_nTimer = (UINT)SetTimer(m_nTimer, 80, NULL);
		}

		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		return;
	}
	
	CEdit::OnLButtonDown(nFlags, point);
}

void CBioEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bIsButtonCaptured)
	{
		ReleaseCapture();

		m_bIsButtonPressed = FALSE;
		m_bIsButtonCaptured = FALSE;
		m_bIsButtonHighlighted = FALSE;

		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		
		if (m_nMode == EditMode_Spin)
		{
			KillTimer(m_nTimer);
		}
		else if(m_rectButton.PtInRect(point))
		{
			OnClickButton();
		}

		return;
	}
	
	CEdit::OnLButtonUp(nFlags, point);
}

void CBioEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bIsButtonCaptured)
	{
		BOOL bIsButtonPressed = m_rectButton.PtInRect (point);
		if (bIsButtonPressed != m_bIsButtonPressed)
		{
			m_bIsButtonPressed = bIsButtonPressed;
			RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}

		return;
	}
	
	if (m_bIsButtonHighlighted)
	{
		if (!m_rectButton.PtInRect(point))
		{
			m_bIsButtonHighlighted = FALSE;
			ReleaseCapture();
		} // if (!m_rectButton.PtInRect (point))
		else
		{
			if (m_nMode == EditMode_Spin)
			{
				if (m_rectTop.PtInRect(point))
					m_bIsInTop = TRUE;
				else
					m_bIsInTop = FALSE;
			}
		}
		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	
	CEdit::OnMouseMove(nFlags, point);
}

void CBioEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (m_nMode != EditMode_Normal && m_nMode != EditMode_Number)
	{
		lpncsp->rgrc [0].right -= m_nButtonWidth;
	}
}
//
//UINT CBioEdit::OnNcHitTest(CPoint point) 
//{
//	CPoint ptClient = point;
//	ScreenToClient (&ptClient);
//
//	if ((m_nMode != EditMode_Normal && m_nMode != EditMode_Number) && m_rectButton.PtInRect (ptClient))
//	{
//		return HTCAPTION;
//	}
//	
//	return CEdit::OnNcHitTest(point);
//}

void CBioEdit::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	if (!m_bIsButtonCaptured)
	{
		CPoint ptClient = point;
		ScreenToClient (&ptClient);

		if (m_rectButton.PtInRect(ptClient))
		{
			SetCapture();
			m_bIsButtonHighlighted = TRUE;
			if (m_nMode == EditMode_Spin)
			{
				if (m_rectTop.PtInRect(ptClient))
					m_bIsInTop = TRUE;
				else
					m_bIsInTop = FALSE;
			}

			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}
	
	CEdit::OnNcMouseMove(nHitTest, point);
}

void CBioEdit::OnCancelMode() 
{
	CEdit::OnCancelMode();
	
	if (IsWindowEnabled ())
	{
		ReleaseCapture ();
	}

	m_bIsButtonPressed = FALSE;
	m_bIsButtonCaptured = FALSE;
	m_bIsButtonHighlighted = FALSE;

	RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

void CBioEdit::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == m_nTimer)
	{
		if (m_nMode == EditMode_Spin)
		{
			double dValue;
			GetValue(dValue);

			if (m_bIsInTop)
			{
				dValue += m_dStepValue;
			}
			else
			{
				dValue -= m_dStepValue;
			}

			SetValue(dValue);
		}
		else if (m_nMode == EditMode_Number)
		{
			KillTimer(m_nTimer);
			JudgeNumberIsLegal();
		}
	}
	else
        CEdit::OnTimer(nIDEvent);
}

void CBioEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_nMode != EditMode_Number && m_nMode != EditMode_Spin)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	else if (nChar == VK_TAB)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	// 只处理数字的情况
	BOOL bChanged = FALSE;
	if (m_strValue.IsEmpty())
		GetWindowText(m_strValue);

	if (isdigit(nChar) || nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		bChanged = TRUE;
	}
	else if (m_bIsMinus && nChar == 45/*处理负号*/)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		bChanged = TRUE;
		/*CString strText;
		GetWindowText(strText);
		if (strText.Find(_T("-")) == -1)
		{
		}*/
	}
	else if (nChar == 46/*处理小数点*/ && m_nPrecision != 0)
	{
		CString strText;
		GetWindowText(strText);
		if (strText.Find(_T(".")) == -1)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			bChanged = TRUE;
		}
	}
	else if ( nChar == VK_RETURN)
	{
		GetParent()->SetFocus();
	}

	if (bChanged && m_bAutoCheck)
	{
		KillTimer(m_nTimer);
		SetTimer(m_nTimer, m_nSleepTime, NULL);
	} 
}

/**
 * 绘制按钮
 * \param pDC 
 * \param rect 按钮区域矩形
 * \param bIsButtonPressed 是否按钮被按下
 * \param bHighlight 按钮是否被加亮
 */
void CBioEdit::DrawButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bHighlight)
{
	CBrush brush(::GetSysColor(COLOR_BTNFACE));
	CBrush brush2(RGB(182, 189, 210));

	if (m_nMode != EditMode_Spin)
	{
		pDC->FillRect(&rect, bHighlight ? &brush2 : &brush);
	}
	else
	{	
		CRect rectTop = m_rectTop;
		CRect rectBottom = m_rectBottom;
		rectTop.OffsetRect(1, 3);
		rectTop.bottom -= 1;
		rectBottom.OffsetRect(1, 1);

		if (m_bIsInTop)
		{
			pDC->FillRect(&rectTop, bHighlight ? &brush2 : &brush);
			pDC->FillRect(&rectBottom, &brush);
		} // if (m_bIsInTop)
		else
		{
			pDC->FillRect(&rectBottom, bHighlight ? &brush2 : &brush);
			pDC->FillRect(&rectTop, &brush);
		}
	}

	CPoint ptImage;
	ptImage.x = rect.CenterPoint ().x - m_sizeImage.cx / 2;
	ptImage.y = rect.CenterPoint ().y - m_sizeImage.cy / 2;

	switch(m_nMode)
	{
	case EditMode_Default:
		DrawDefaultButtonLabel(pDC, rect, bIsButtonPressed);
		break;
	case EditMode_Folder:
		m_ImageButton.Draw (pDC, 0, ptImage, ILD_NORMAL);
		break;
	case EditMode_File:
		m_ImageButton.Draw (pDC, 1, ptImage, ILD_NORMAL);
		break;
	case EditMode_Spin:
		m_ImageButton.Draw (pDC, 2, ptImage, ILD_NORMAL);
		break;
	} // switch(m_nMode)

	// 绘制
	DrawButtonFrame(pDC, rect, bIsButtonPressed);
}

/**
 * 绘制在默认按钮时的标签
 * \param pDC 
 * \param rect 
 * \param bIsButtonPressed 
 */
void CBioEdit::DrawDefaultButtonLabel(CDC* pDC, CRect rect, BOOL bIsButtonPressed)
{
	if (m_strLabel.IsEmpty())
		return;

	COLORREF clrText = ::GetSysColor(COLOR_BTNTEXT);
	COLORREF clrTextOld = pDC->SetTextColor(clrText);
	int nTextMode = pDC->SetBkMode (TRANSPARENT);
	CFont* pFont = (CFont*) pDC->SelectStockObject(DEFAULT_GUI_FONT);

	CRect rectText = rect;
	rectText.DeflateRect (1, 2);
	rectText.OffsetRect (0, -2);

	if (bIsButtonPressed)
	{
		rectText.OffsetRect (1, 1);
	}

	pDC->DrawText (m_strLabel, rectText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkMode (nTextMode);
	pDC->SelectObject (pFont);
}

/**
 * 绘制按钮的外框
 * \param pDC 
 * \param rect 
 * \param bIsPressed 
 */
void CBioEdit::DrawButtonFrame(CDC* pDC, CRect rect, BOOL bIsPressed)
{
	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);
	pDC->Draw3dRect (rectFrame, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DDKSHADOW));

	if (m_nMode == EditMode_Spin)
	{
		CRect rectTop = m_rectTop;
		CRect rectBottom = m_rectBottom;
		rectTop.OffsetRect(1, 3);
		rectTop.bottom -= 1;
		rectBottom.OffsetRect(1, 1);

		if (m_bIsInTop)
		{
			pDC->DrawEdge (rectTop, bIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);
			pDC->DrawEdge (rectBottom, BDR_RAISEDINNER, BF_RECT);
		}
		else
		{
			pDC->DrawEdge (rectTop, BDR_RAISEDINNER, BF_RECT);
			pDC->DrawEdge (rectBottom, bIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);
		}
	} // if (m_nMode == EditMode_Spin)
	else
	{
		rectFrame.DeflateRect (1, 1);
		pDC->DrawEdge (rectFrame, bIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);
	}
}

/**
 * 重新设置编辑框的大小
 */
void CBioEdit::ChangeLayout()
{
	SetWindowPos(NULL, 0, 0, 0, 0, 
		SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	if (m_nMode != EditMode_Normal && m_nMode != EditMode_Number)
	{
		GetWindowRect (&m_rectButton);
		m_rectButton.left = m_rectButton.right -  m_nButtonWidth;

		ScreenToClient(&m_rectButton);
	}
	else
	{
		m_rectButton.SetRectEmpty();
	}

	if (m_nMode == EditMode_Spin)
	{
		m_rectTop = m_rectBottom = m_rectButton;
		m_rectTop.bottom = m_rectTop.top + m_rectButton.Height()/2;
		m_rectBottom.top = m_rectBottom.bottom - m_rectButton.Height()/2;
	}
}

void CBioEdit::OnNcLButtonDblClk(UINT /*nHitTest*/, CPoint /*point*/)
{
}

/**
 * 处理按钮被点按消息
 */
void CBioEdit::OnClickButton()
{
	if(m_nMode == EditMode_Folder)
	{
		OnClickFolderButton();
	}
	else if (m_nMode == EditMode_File)
	{
		OnClickFileButton();
	} 

	SetFocus();
}

/**
 * 处理文件选择
 */
void CBioEdit::OnClickFileButton()
{
	CString strFile;
	GetWindowText(strFile);

	CFileDialog dlg(TRUE, m_strFileExt, strFile, 0, m_strFileFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		SetWindowText(dlg.GetPathName());
	}
}

/**
 * 处理路径选择
 */
void CBioEdit::OnClickFolderButton()
{
	CString strReturn;
	TCHAR szDisplayName[MAX_PATH];
	BROWSEINFO bi;
	ZeroMemory (&bi, sizeof (bi));
	strReturn.Empty();
	
	CString strPath;
	GetWindowText(strPath);
	m_lpszInitPath = strPath;

	bi.lpszTitle = m_strFolderTitle;
	bi.pszDisplayName = szDisplayName;
	bi.hwndOwner = GetSafeHwnd ();
	bi.lpfn = BrowseFolderCallback;
	bi.lParam = (LPARAM) this;
	bi.pidlRoot = NULL;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.iImage = -1;

	LPITEMIDLIST pidlRes = SHBrowseForFolder (&bi);
	if (pidlRes != NULL)
	{
		TCHAR szPath [MAX_PATH];
		if (SHGetPathFromIDList (pidlRes, szPath))
		{
			strReturn = szPath;
		}
	} // if (pidlRes != NULL)

	if (!strReturn.IsEmpty())
	{
		SetWindowText(strReturn);
	}
}

/**
 * 处理路径选择的回调函数
 * \param hwnd 
 * \param uMsg 
 * \param lParam
 * \param lpData 
 * \return 
 */
int CALLBACK CBioEdit::BrowseFolderCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	::ZeroMemory(&szDir, sizeof(TCHAR));
	
	if (uMsg == BFFM_INITIALIZED)
	{
		CBioEdit* pThis = (CBioEdit*) lpData;
		ASSERT_VALID (pThis);

		if (pThis->m_lpszInitPath != NULL)
		{
			::SendMessage (hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)pThis->m_lpszInitPath);
		}
	}

	return 0;
}

/**
 * 将编辑框的模式设置为默认按钮模式
 * \param strLabel 按钮显示标签
 */
void CBioEdit::EnableDefaultButton(CString strLabel)
{
	m_nMode = EditMode_Default;
	m_strLabel = strLabel;

	m_ImageButton.DeleteImageList();
	m_sizeImage = CSize(0, 0);

	ChangeLayout();
}

/**
 * 将编辑框的模式设置为文件选择按钮模式
 * \param strFileEx 文件后缀
 * \param strFilter 文件过滤器
 */
void CBioEdit::EnableFileBrowseButton(CString strFileEx, CString strFilter)
{
	m_nMode = EditMode_File;

	m_strFileExt = strFileEx;
	m_strFileFilter = strFilter;

	SetIntenalImage();

	m_nButtonWidth = max(m_nButtonWidth, m_sizeImage.cx + 8);
	ChangeLayout();
}

/**
 * 将编辑框的模式设置为路径选择按钮模式
 * \param strFolderTitle 路径选择对话框的标题
 */
void CBioEdit::EnableFolderBrowseButton(CString strFolderTitle)
{
	m_nMode = EditMode_Folder;
	m_strFolderTitle = strFolderTitle;

	SetIntenalImage();
	m_nButtonWidth = max(m_nButtonWidth, m_sizeImage.cx + 8);
	ChangeLayout();
}

/**
 * 将编辑框的模式设置为数字模式，带Spin按钮
 */
void CBioEdit::EnableSpinNumButton()
{
	m_nMode = EditMode_Spin;

	SetIntenalImage();
	m_nButtonWidth = 18;
	ChangeLayout();
}

/**
 * 将编辑框的模式设置为数字模式
 */
void CBioEdit::EnableNumberEdit()
{
	m_nMode = EditMode_Number;
}


/**
 * 设置按钮图标
 */
void CBioEdit::SetIntenalImage()
{
	CBWGenUILibResource libResource;

	if (m_ImageButton.GetSafeHandle() != NULL)
	{
		m_ImageButton.DeleteImageList();
	}

	m_ImageButton.Create(IDB_EDIT_BUTTON, 16, 0, RGB (255, 0, 255));
	m_sizeImage = CSize(16, 16);
}


/**
 * 取得浮点数值
 * \param &dOutput 浮点数值
 */
void CBioEdit::GetValue(double &dOutput)
{
	CString strText;
	GetWindowText(strText);
	dOutput = ::_tstof(strText);
}

/**
 * 设置浮点数值
 * \param dInput 浮点数值
 */
void CBioEdit::SetValue(double dInput)
{
	if (dInput < m_dMinValue)
		dInput = m_dMinValue;
	if (dInput > m_dMaxValue)
		dInput = m_dMaxValue;

	CString strText, strFormat;
	strFormat.Format(_T("%%.%df"), m_nPrecision);
	strText.Format(strFormat, dInput);
	m_strValue = strText;
	/*if (strText.GetLength() > 1 && strText.Find(_T(".")) != -1)
	{
		CString string;
		string = strText.Right(1);
		while ((string == _T("0") || string == _T(".")) && strText.GetLength() > 1)
		{
			strText = strText.Left(strText.GetLength() -1);
			if (string == _T("."))
				break;
			string = strText.Right(1);
		}
	}*/

	SetWindowText(strText);
}

/**
 * 取得整型数值
 * \param &lOutput 数值
 */
void CBioEdit::GetValue(long &lOutput)
{
	CString strText;
	GetWindowText(strText);
	lOutput = _ttoi(strText);
}

/**
 * 设置整型数值
 * \param lInput 数值
 */
void CBioEdit::SetValue(long lInput)
{
	if (lInput < m_dMinValue)
		lInput = (long)m_dMinValue;
	if (lInput > m_dMaxValue)
		lInput = (long)m_dMaxValue;

	CString strText;
	strText.Format(_T("%d"), lInput);
	m_strValue = strText;
	SetWindowText(strText);
}

/**
 * 设置数据的最小值与最大值
 * \param dMinValue 最小值
 * \param dMaxValue 最大值
 */
void CBioEdit::SetRange(double dMinValue , double dMaxValue)
{
	m_dMinValue = dMinValue;
	m_dMaxValue = dMaxValue;
}

/**
 * 取得数据的最小值与最大值
 * \param dMinValue 最小值
 * \param dMaxValue 最大值
 */
void CBioEdit::GetRange(double& dMinValue, double& dMaxValue)
{
	dMinValue = m_dMinValue;
	dMaxValue = m_dMaxValue;
}

/**
 * 设置步长值
 * \param dStep 步长值
 */
void CBioEdit::SetStepValue(double dStep)
{
	m_dStepValue = dStep;
}

/**
 * 得到步长值
 * \param void 
 * \return 步长值
 */
double CBioEdit::GetStepValue(void)
{
	return m_dStepValue;
}

/**
 * 设置浮点数的精度，在整型时忽略
 * \param nInPrecision 精度
 */
void CBioEdit::SetPrecision(UINT nInPrecision)
{
	m_nPrecision = nInPrecision;
}

void CBioEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	
	if (!m_bAutoCheck && nChar == VK_RETURN )
	{
		JudgeNumberIsLegal();
		return;
	}

	if (nChar != VK_DELETE)
		return;

	if (m_nMode == EditMode_Number || m_nMode == EditMode_Spin)
	{
		if (m_strValue.IsEmpty())
			GetWindowText(m_strValue);
		
		if (m_bAutoCheck)
		{
			KillTimer(m_nTimer);
			SetTimer(m_nTimer, m_nSleepTime, NULL);
		}
	}
}

/**
 * 判断修改的数据是否正确
 * \param void 
 */
void CBioEdit::JudgeNumberIsLegal(void)
{
	// 当输入为数字时不能为空
	CString strText;
 	GetWindowText(strText);
	if (strText.IsEmpty() || strText == _T("-"))
	{
		CBWGenUILibResource resource;
		AfxMessageBox(IDS_EDIT_ERROR_NULL);
		SetWindowText(m_strValue);
		return;
	}
	else if (strText.Find(_T("-")) > 0 || strText.Find(_T(".")) == 0)// 负号或小数点输入位置错误
	{
		CBWGenUILibResource resource;
		AfxMessageBox(IDS_EDIT_ERROR_VALUE);
		SetWindowText(m_strValue);
		return;
	}
	else if (strText.GetLength() > 1) // 判断格式，避免00,-00,-.等格式出现
	{
		if (strText.GetAt(0) == '-')
		{
			if (strText.GetAt(1) == '.' || (strText.GetLength() > 2 && strText.GetAt(1) == '0' && strText.GetAt(2) == '0'))
			{
				CBWGenUILibResource resource;
				AfxMessageBox(IDS_EDIT_ERROR_VALUE);
				SetWindowText(m_strValue);
				return;
			}
		}
		else if (strText.GetAt(0) == '0')
		{
			if (strText.GetAt(1) == '0')
			{
				CBWGenUILibResource resource;
				AfxMessageBox(IDS_EDIT_ERROR_VALUE);
				SetWindowText(m_strValue);
				return;
			}
		}
	}

	if (strText.Find(_T(".")) != -1) // 处理小数点后位数
	{
		int nPos = strText.Find(_T("."));
		if (UINT(strText.GetLength() - nPos) > m_nPrecision+1)
		{
			SetWindowText(m_strValue);
			return;
		}
	}

	// 判断最大最小值
	double dValue;
	GetValue(dValue);

	if (dValue - m_dMaxValue > 0.00001)
	{
		CBWGenUILibResource resource;
		CString string, strValue, strFormat;
		strFormat.Format(_T("%%.%df"), m_nPrecision);
		strValue.Format(strFormat, m_dMaxValue);
		string.Format(IDS_INFO_MAX_ERROR, strValue);
		AfxMessageBox(string);

		SetWindowText(m_strValue);
	}
	else if (m_dMinValue - dValue > 0.00001)
	{
		CBWGenUILibResource resource;
		CString string, strValue, strFormat;
		strFormat.Format(_T("%%.%df"), m_nPrecision);
		strValue.Format(strFormat, m_dMinValue);
		string.Format(IDS_INFO_MIN_ERROR, strValue);
		AfxMessageBox(string);

		SetWindowText(m_strValue);
	}
	else
	{
		CString strTemp;
		GetWindowText(strTemp);
		if (strTemp != m_strValue)
		{
			// 发送消息
			GetParent()->SendMessage(WM_EDIT_VALUE_CHANGED, (WPARAM)this, 0);
			GetWindowText(m_strValue);
		}
	}
}

UINT CBioEdit::OnGetDlgCode()
{
	if (m_bWantAllKeys)
	{
		UINT nCode = DLGC_WANTALLKEYS;
		return nCode;
	}
	return CEdit::OnGetDlgCode();
}

#include "StdAfx.h"
#include "SamLst.h"

CSamLst::CSamLst(void)
{

}

CSamLst::~CSamLst(void)
{

}

int CSamLst::DrawBody(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(m_colNum < 1)
	{
		return -2;
	}

	if(m_lnkTitleInfo.GetSize() != m_colNum)
	{
		return -3;
	}

	CDC memDcBody;
	memDcBody.CreateCompatibleDC(pDC);

	CBitmap bitMap;
	int tempCdcH = m_cdcH;
	if(-1 == m_rowIdStartShow || m_cdcH < m_rcBody.Height())
	{
		tempCdcH = m_rcBody.Height();
	}
	bitMap.CreateCompatibleBitmap(pDC,m_cdcW,tempCdcH);
	CBitmap* pOldBitmap = memDcBody.SelectObject(&bitMap);

	memDcBody.FillSolidRect(0,0,m_cdcW,tempCdcH,RGB(m_clrBodyBk.GetR(),m_clrBodyBk.GetG(),m_clrBodyBk.GetB()));

	CRect tempRc;
	tempRc.top = m_pxHeaderH;
	CFont* pOldFont = memDcBody.SelectObject(&m_fontSet);
	memDcBody.SetTextColor(m_txClr1);
	memDcBody.SetBkMode(TRANSPARENT);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,m_pxRowColLine,m_clrRowColLine);
	CPen* pOldPen = memDcBody.SelectObject(&penLine);

	Graphics graphics(memDcBody.m_hDC);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if(-1 == m_rowIdStartShow || m_cdcH < m_rcBody.Height())
	{
		if(m_bShowRowColLine)
		{
			//绘制水平分割线
			for(int i = m_rowIdSeparatorStart;i <= m_rowIdSeparatorEnd;++i)
			{
				POINT pt;
				pt.x = 0;
				pt.y = (i + 1) * m_pxRowH - m_pxRowColLine;

				//如果底部分隔线离列表框底部太近，则不绘制该直线
				if(abs(pt.y - m_rcBody.Height()) > 8)
				{
					memDcBody.MoveTo(pt);
					pt.x = m_cdcW;
					memDcBody.LineTo(pt);
				}
			}

			//绘制垂直分割线
			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				if(j != m_colIdEndShow)
				{
					POINT pt;
					pt.x = GetColStartDrawInMemoryHoriPixel(j + 1) - m_pxRowColLine;
					pt.y = 0;
					memDcBody.MoveTo(pt);
					pt.y = tempCdcH;
					memDcBody.LineTo(pt);
				}
			}
		}
	}

	if(-1 != m_rowIdStartShow)
	{
		for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
		{
			CRowInfo& rowInfo = *m_lnkRowInfo[i];

			tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
			tempRc.left = 0;
			tempRc.right = m_cdcW;
			tempRc.bottom = tempRc.top + rowInfo.m_rowH;
			if(rowInfo.m_bSelected && m_bShowRowSelState)
			{
				memDcBody.FillSolidRect(&tempRc,m_clrRowBkSel);
			}
			else
			{
				memDcBody.FillSolidRect(&tempRc,m_clrRowBkUnSel);
			}

			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				CTitleInfo& titleInfo = m_lnkTitleInfo.GetAt(j)->data;
				CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(i)->data.GetAt(j)->data;
				tempRc.left = GetColStartDrawInMemoryHoriPixel(j);
				tempRc.right = tempRc.left + m_lnkTitleInfo[j]->m_colWidth;

				cellInfo.m_rcInCDc = tempRc;
				cellInfo.m_rcInCDc.top += (m_rcHeader.Height() + m_pxFrame);
				if(i != m_rowIdStartShow)
				{
					cellInfo.m_rcInCDc.top += 1; //“+1”是因为每一行底部负责画分割线,第一显示行除外
				}

				cellInfo.m_rcInCDc.bottom += (m_rcHeader.Height() + m_pxFrame - 1);
				cellInfo.m_rcInCDc.left += 1;	//“+1”是因为每一列右侧负责画分割线
				if(cellInfo.m_rcInCDc.right > m_rcBody.right)	//防止“垂直滚动条存在时，最右侧一列双击鼠标左键编辑内容时，编辑框越界”
				{
					cellInfo.m_rcInCDc.right = m_rcBody.right;
				}

				cellInfo.m_rcInCDc.InflateRect(m_ssColLeftIndent,m_ssRowTopIndent,-m_ssColLeftIndent,-m_ssRowTopIndent);

				if(!m_bEditable || rowInfo.m_bDisabled || titleInfo.GetBeDisabled() || cellInfo.m_bDisabled)
				{
					CRect rcDisabledBk = tempRc;
					if(i != m_rowIdStartShow)
					{
						rcDisabledBk.DeflateRect(1,1,1,1);
					}
					else
					{
						rcDisabledBk.DeflateRect(1,0,1,1);
					}
					memDcBody.FillSolidRect(&rcDisabledBk,cellInfo.GetColorDisabled());
				}

				if(cellInfo.GetCheckable())
				{
					Image* pImg = NULL;
					if(m_bUseLargeChkIcon)
					{
						pImg = &m_img2;
					}
					else
					{
						pImg = &m_image;
					}

					int sonPicNums = 4,tempInt = 0;
					int iconW = pImg->GetWidth() / sonPicNums;

					Rect rect;
					tempInt = tempRc.left + (tempRc.Width() - iconW) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.X = tempInt;

					tempInt = tempRc.top + (tempRc.Height() - pImg->GetHeight()) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.Y = tempInt;

					rect.Width = iconW;
					rect.Height = pImg->GetHeight();

					int iStartX = 0;
					if(!m_bEditable || rowInfo.m_bDisabled || cellInfo.m_bDisabled)
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW * 3;
						}
						else
						{
							iStartX = iconW * 2;
						}
					}
					else
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW;
						}
					}
					graphics.DrawImage(pImg,rect,iStartX,0,iconW,pImg->GetHeight(),UnitPixel,&ImgAtt);
				}
				else if(titleInfo.m_bCheckFunc)
				{
					Image* pImg = NULL;
					if(m_bUseLargeChkIcon)
					{
						pImg = &m_img2;
					}
					else
					{
						pImg = &m_image;
					}

					int sonPicNums = 4,tempInt = 0;
					int iconW = pImg->GetWidth() / sonPicNums;

					Rect rect;
					tempInt = tempRc.left + (tempRc.Width() - iconW) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.X = tempInt;

					tempInt = tempRc.top + (tempRc.Height() - pImg->GetHeight()) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.Y = tempInt;

					rect.Width = iconW;
					rect.Height = pImg->GetHeight();

					int iStartX = 0;
					if(!m_bEditable || rowInfo.m_bDisabled || cellInfo.m_bDisabled)
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW * 3;
						}
						else
						{
							iStartX = iconW * 2;
						}	
					}
					else
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW;
						}
					}
					graphics.DrawImage(pImg,rect,iStartX,0,iconW,pImg->GetHeight(),UnitPixel,&ImgAtt);
				}
				else if(titleInfo.m_bColorFunc)
				{
					COLORREF clr = 0;
					int clrW = 0,clrH = 0,iRoundAngle = 2;;
					cellInfo.GetItemColorParam(clr,clrW,clrH,iRoundAngle);
					int iTempIntLeft1 = tempRc.left + (tempRc.Width() - clrW) / 2, iTempIntTop1 = tempRc.top + (tempRc.Height() - clrH) / 2;
					cellInfo.m_rcClrBlock = CRect(iTempIntLeft1,iTempIntTop1,iTempIntLeft1 + clrW,iTempIntTop1 + clrH);
					Color clrTemp;
					clrTemp.SetFromCOLORREF(cellInfo.GetItemColor());
					DrawRoundRectangleNew(&memDcBody,cellInfo.m_rcClrBlock.left,cellInfo.m_rcClrBlock.top,cellInfo.m_rcClrBlock.Width() - 1,cellInfo.m_rcClrBlock.Height() - 1,iRoundAngle,iRoundAngle,iRoundAngle,iRoundAngle,TRUE,clrTemp,1,TRUE,clrTemp);
					cellInfo.m_rcClrBlock.DeflateRect(0,m_pxHeaderH,0,-m_pxHeaderH);
				}
				else
				{
					CRect rcText = tempRc;
					rcText.left += 2;
					CRect rcRightRgn = tempRc;
					rcRightRgn.right -= 2;
					if(cellInfo.m_bCbAble)
					{
						SolidBrush brush(titleInfo.m_arrowInfo.m_clrArrow);
						rcText.left += titleInfo.m_iCbTxLeftIndent;
						rcText.right -= titleInfo.m_iCbTxRightIndent;
						rcRightRgn.left = rcRightRgn.right - titleInfo.m_iCbTxRightIndent;
						titleInfo.m_arrowInfo.CalcPtData(rcRightRgn);
						graphics.FillPolygon(&brush,titleInfo.m_arrowInfo.m_aryPtTriangle,3,FillModeAlternate);
						memDcBody.DrawText(cellInfo.GetItemText(),&rcText,titleInfo.m_dwTextAlign);
					}
					else if(titleInfo.m_bCbtable)
					{
						SolidBrush brush(titleInfo.m_arrowInfo.m_clrArrow);
						rcText.left += titleInfo.m_iCbTxLeftIndent;
						rcText.right -= titleInfo.m_iCbTxRightIndent;
						rcRightRgn.left = rcRightRgn.right - titleInfo.m_iCbTxRightIndent;
						titleInfo.m_arrowInfo.CalcPtData(rcRightRgn);
						graphics.FillPolygon(&brush,titleInfo.m_arrowInfo.m_aryPtTriangle,3,FillModeAlternate);
						memDcBody.DrawText(cellInfo.GetItemText(),&rcText,titleInfo.m_dwTextAlign);
					}
					else
					{
						if(titleInfo.GetMultiDensityState())
						{
							CFont* pOldFt2 = memDcBody.SelectObject(&m_ftDensity);
							
							rcText.left += cellInfo.m_iTxLeftIndent;
							rcText.right -= cellInfo.m_iTxRightIndent;
							memDcBody.DrawText(cellInfo.GetItemText(),&rcText,titleInfo.GetTextAlignFormat2());

							memDcBody.SelectObject(pOldFt2);
						}
						else
						{
							rcText.left += cellInfo.m_iTxLeftIndent;
							rcText.right -= cellInfo.m_iTxRightIndent;
							memDcBody.DrawText(cellInfo.GetItemText(),&rcText,cellInfo.m_dwTextAlign);
						}
					}
				} 
			}
		}

		if(m_bShowRowColLine)
		{
			//绘制水平分割线
			for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
			{
				if(i != m_rowIdEndShow)
				{
					POINT pt;
					pt.x = 0;
					pt.y = GetRowStartDrawInMemoryVertPixel(i + 1) - m_pxRowColLine;
					memDcBody.MoveTo(pt);
					pt.x = m_cdcW;
					memDcBody.LineTo(pt);
				}
				else if(m_cdcH < m_rcBody.Height() && abs(m_cdcH - m_rcBody.Height()) > 8)
				{
					POINT pt;
					pt.x = 0;
					pt.y = GetRowStartDrawInMemoryVertPixel(i + 1) - m_pxRowColLine;
					memDcBody.MoveTo(pt);
					pt.x = m_cdcW;
					memDcBody.LineTo(pt);
				}
			}

			//绘制垂直分割线
			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				if(j != m_colIdEndShow)
				{
					POINT pt;
					pt.x = GetColStartDrawInMemoryHoriPixel(j + 1) - m_pxRowColLine;
					pt.y = 0;
					memDcBody.MoveTo(pt);
					pt.y = m_cdcH;
					memDcBody.LineTo(pt);
				}
			}
		}
	}

	graphics.ReleaseHDC(memDcBody.m_hDC);

	if(m_bRoundRectStyle && !m_bScrollInsideFrame)
	{
		//检查使用稳定性
		BOOL bRet1 = memDcBody.TransparentBlt(m_ssColLeftIndent,m_ssRowTopIndent,m_rcBody.Width(),m_rcBody.Height(),pDC,m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		BOOL bRet2 = pDC->TransparentBlt(m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),&memDcBody,m_ssColLeftIndent,m_ssRowTopIndent, m_rcBody.Width(),m_rcBody.Height(),m_clrFourAngleBk);
		if(!bRet1 || !bRet2)
		{
			int tt = 0;
			++tt;
		}
	}
	else
	{
		pDC->BitBlt(m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),&memDcBody,m_ssColLeftIndent,m_ssRowTopIndent,SRCCOPY);   
	}

	memDcBody.SelectObject(&pOldPen);
	memDcBody.SelectObject(pOldFont);
	memDcBody.SelectObject(pOldBitmap);

	memDcBody.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}

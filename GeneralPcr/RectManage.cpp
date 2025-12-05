#include "StdAfx.h"
#include "RectManage.h"


CRectManage::CRectManage(void)
{
}


CRectManage::~CRectManage(void)
{
}

CRectManage::CRectManage(const CRectManage& info)
{

}

void CRectManage::operator=(const CRectManage& info)
{

}

int CRectManage::RectMinusRect(CRect rcSource,CRect rect,CLinkList<CRect>& rcRst)
{
	rcRst.Clear();

	if(rcSource.IsRectEmpty())
	{
		return -1;
	}

	if(rect.IsRectEmpty())
	{
		rcRst.AddTail(rcSource);
		return -2;
	}

	if(rect == rcSource)
	{
		return 2;
	}

	if(rect.left >= rcSource.right || rect.right <= rcSource.left || rect.top >= rcSource.bottom || rect.bottom <= rcSource.top)
	{
		rcRst.AddTail(rcSource);
		return -3;
	}

	if(rcSource.left >= rect.left && rcSource.right <= rect.right && rcSource.top >= rect.top && rcSource.bottom <= rect.bottom)
	{
		return -4;
	}

	int x1 = -1,x2 = -1,x3 = -1,x4 = -1,y1 = -1,y2 = -1,y3 = -1,y4 = -1;
	
	if(rect.left > rcSource.left && rect.left < rcSource.right)
	{
		x1 = rcSource.left;
		x2 = rect.left;
	}
	if(rect.right < rcSource.right && rect.right > rcSource.left)
	{
		x3 = rect.right;
		x4 = rcSource.right;
	}
	if(rect.top > rcSource.top && rect.top < rcSource.bottom)
	{
		y1 = rcSource.top;
		y2 = rect.top;
	}
	if(rect.bottom < rcSource.bottom && rect.bottom > rcSource.top)
	{
		y3 = rect.bottom;
		y4 = rcSource.bottom;
	}

	CRect rcTemp,rcTemp2;
	BOOL bLeftTopAdded = FALSE,bLeftBottomAdded = FALSE,bRightTopAdded = FALSE,bRightBottomAdded = FALSE;

	if(-1 != x1 && -1 != x2)
	{
		rcTemp.left = x1;
		rcTemp.right = x2;
		if(-1 != y2)
		{
			if(-1 != y1)
			{
				rcTemp2.left = x1;
				rcTemp2.right = x2;
				rcTemp2.top = y1;
				rcTemp2.bottom = y2;
				rcRst.AddTail(rcTemp2);
				bLeftTopAdded = TRUE;
			}

			if(-1 != y3)
			{
				if(-1 != y4)
				{
					rcTemp2.left = x1;
					rcTemp2.right = x2;
					rcTemp2.top = y3;
					rcTemp2.bottom = y4;
					rcRst.AddTail(rcTemp2);
					bLeftBottomAdded = TRUE;
				}

				rcTemp.top = y2;
				rcTemp.bottom = y3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.top = y2;
				rcTemp.bottom = rcSource.bottom;
				rcRst.AddTail(rcTemp);
			}
		}
		else
		{
			if(-1 != y3)
			{
				if(-1 != y4)
				{
					rcTemp2.left = x1;
					rcTemp2.right = x2;
					rcTemp2.top = y3;
					rcTemp2.bottom = y4;
					rcRst.AddTail(rcTemp2);
					bLeftBottomAdded = TRUE;
				}

				rcTemp.top = rcSource.top;
				rcTemp.bottom = y3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.top = rcSource.top;
				rcTemp.bottom = rcSource.bottom;
				rcRst.AddTail(rcTemp);
			}
		}	
	}

	if(-1 != x3 && -1 != x4)
	{
		rcTemp.left = x3;
		rcTemp.right = x4;
		if(-1 != y2)
		{
			if(-1 != y1)
			{
				rcTemp2.left = x3;
				rcTemp2.right = x4;
				rcTemp2.top = y1;
				rcTemp2.bottom = y2;
				rcRst.AddTail(rcTemp2);
				bRightTopAdded = TRUE;
			}

			if(-1 != y3)
			{
				if(-1 != y4)
				{
					rcTemp2.left = x3;
					rcTemp2.right = x4;
					rcTemp2.top = y3;
					rcTemp2.bottom = y4;
					rcRst.AddTail(rcTemp2);
					bRightBottomAdded = TRUE;
				}

				rcTemp.top = y2;
				rcTemp.bottom = y3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.top = y2;
				rcTemp.bottom = rcSource.bottom;
				rcRst.AddTail(rcTemp);
			}
		}
		else
		{
			if(-1 != y3)
			{
				if(-1 != y4)
				{
					rcTemp2.left = x3;
					rcTemp2.right = x4;
					rcTemp2.top = y3;
					rcTemp2.bottom = y4;
					rcRst.AddTail(rcTemp2);
					bRightBottomAdded = TRUE;
				}

				rcTemp.top = rcSource.top;
				rcTemp.bottom = y3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.top = rcSource.top;
				rcTemp.bottom = rcSource.bottom;
				rcRst.AddTail(rcTemp);
			}
		}
	}

	if(-1 != y1 && -1 != y2)
	{
		rcTemp.top = y1;
		rcTemp.bottom = y2;
		if(-1 != x2)
		{
			if(!bLeftTopAdded && -1 != x1)
			{
				rcTemp2.left = x1;
				rcTemp2.right = x2;
				rcTemp2.top = y1;
				rcTemp2.bottom = y2;
				rcRst.AddTail(rcTemp2);
			}

			if(-1 != x3)
			{
				if(!bRightTopAdded && -1 != x4)
				{
					rcTemp2.left = x3;
					rcTemp2.right = x4;
					rcTemp2.top = y1;
					rcTemp2.bottom = y2;
					rcRst.AddTail(rcTemp2);
				}

				rcTemp.left = x2;
				rcTemp.right = x3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.left = x2;
				rcTemp.right = rcSource.right;
				rcRst.AddTail(rcTemp);
			}
		}
		else 
		{
			if(-1 != x3)
			{
				if(!bRightTopAdded && -1 != x4)
				{
					rcTemp2.left = x3;
					rcTemp2.right = x4;
					rcTemp2.top = y1;
					rcTemp2.bottom = y2;
					rcRst.AddTail(rcTemp2);
				}

				rcTemp.left = rcSource.left;
				rcTemp.right = x3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.left = rcSource.left;
				rcTemp.right = rcSource.right;
				rcRst.AddTail(rcTemp);
			}
		}
	}

	if(-1 != y3 && -1 != y4)
	{
		rcTemp.top = y3;
		rcTemp.bottom = y4;
		if(-1 != x2)
		{
			rcTemp.left = x2;

			if(!bLeftBottomAdded && -1 != x1)
			{
				rcTemp2.left = x1;
				rcTemp2.right = x2;
				rcTemp2.top = y3;
				rcTemp2.bottom = y4;
				rcRst.AddTail(rcTemp2);
			}

			if(-1 != x3)
			{
				if(!bRightBottomAdded && -1 != x4)
				{
					rcTemp2.left = x3;
					rcTemp2.right = x4;
					rcTemp2.top = y3;
					rcTemp2.bottom = y4;
					rcRst.AddTail(rcTemp2);
				}

				rcTemp.left = x2;
				rcTemp.right = x3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.left = x2;
				rcTemp.right = rcSource.right;
				rcRst.AddTail(rcTemp);
			}
		}
		else
		{
			if(-1 != x3)
			{
				rcTemp.left = rcSource.left;
				rcTemp.right = x3;
				rcRst.AddTail(rcTemp);
			}
			else
			{
				rcTemp.left = rcSource.left;
				rcTemp.right = rcSource.right;
				rcRst.AddTail(rcTemp);
			}
		}
	}

	return 0;
}

int CRectManage::CalcRectResult()
{
	m_lnkRcRst.Clear();
	
	if(m_rcInit.IsRectEmpty())
	{
		return 1;
	}

	if(m_lnkRcToDelete.GetSize() < 1)
	{
		m_lnkRcRst.AddTail(m_rcInit);
		return 2;
	}

	CLinkList<CRect> rcLnkTemp,rcLnkTemp2;

	Node<CRect>* pNodeToDelete = m_lnkRcToDelete.GetHead();
	if(NULL != pNodeToDelete)
	{
		RectMinusRect(m_rcInit,pNodeToDelete->data,m_lnkRcRst);
		pNodeToDelete = pNodeToDelete->pNext;
	}

	while(NULL != pNodeToDelete)
	{
		rcLnkTemp2.Clear();
		Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
		while(NULL != pNodeRst)
		{
			RectMinusRect(pNodeRst->data,pNodeToDelete->data,rcLnkTemp);
			rcLnkTemp2 + rcLnkTemp;
			pNodeRst = pNodeRst->pNext;
		}
		m_lnkRcRst = rcLnkTemp2;
		pNodeToDelete = pNodeToDelete->pNext;
	}

	return 0;
}

int CRectManage::AddRectToDelete(CRect rect)
{
	if(rect.IsRectEmpty())
	{
		return -1;
	}

	m_lnkRcToDelete.AddTail(rect);
	return 0;
}

int CRectManage::AddRectToDelete(int iLeft,int iTop,int iWidth,int iHeight)
{
	CRect rect;
	rect.left = iLeft;
	rect.right = rect.left + iWidth;
	rect.top = iTop;
	rect.bottom = rect.top + iHeight;

	if(rect.IsRectEmpty())
	{
		return -1;
	}

	m_lnkRcToDelete.AddTail(rect);
	return 0;
}

int CRectManage::ClearRectToDelete()
{
	m_lnkRcToDelete.Clear();
	return 0;
}

int CRectManage::AddRect(CRect rect)
{
	Node<CRect>* pNode = m_lnkRcRst.GetHead();
	while(NULL != pNode)
	{
		if(rect.left >= pNode->data.left && rect.top >= pNode->data.top && rect.right <= pNode->data.right && rect.bottom <= pNode->data.bottom)
		{
			return 1;
		}
		pNode = pNode->pNext;
	}

	m_lnkRcRst.AddTail(rect);
	return 0;
}

int CRectManage::AddRect(int iLeft,int iTop,int iWidth,int iHeight)
{
	Node<CRect>* pNode = m_lnkRcRst.GetHead();
	while(NULL != pNode)
	{
		if(iLeft >= pNode->data.left && iTop >= pNode->data.top && (iLeft + iWidth) <= pNode->data.right && (iTop + iHeight) <= pNode->data.bottom)
		{
			return 1;
		}
		pNode = pNode->pNext;
	}

	m_lnkRcRst.AddTail(CRect(iLeft,iTop,iLeft + iWidth,iTop + iHeight));
	return 0;
}

int CRectManage::DeleteRect(CRect rect)
{
	CLinkList<CLinkList<CRect>> lnk2RcRst;
	int iRet = 0,iIndex = 0;

	Node<CRect>* pNode = m_lnkRcRst.GetHead();
	while(NULL != pNode)
	{
		CLinkList<CRect> lnkRcRst;
		iRet = RectMinusRect(pNode->data,rect,lnkRcRst);
		if(iRet >= 0)
		{
			pNode = pNode->pNext;
			m_lnkRcRst.DeleteAt(iIndex);
			lnk2RcRst.AddTail(lnkRcRst);
			continue;
		}

		pNode = pNode->pNext;
		++iIndex;
	}

	int iCount = lnk2RcRst.GetSize();
	for(int i = 0;i < iCount;++i)
	{
		pNode = lnk2RcRst[i]->GetHead();
		while(NULL != pNode)
		{
			m_lnkRcRst.AddTail(pNode->data);
			pNode = pNode->pNext;
		}
	}

	return 0;
}

int CRectManage::AddRectRoundFrame(CRect rect,int iRoundH,int iFrameW)
{
	int i2RoundH = iRoundH * 2;
	//按钮的四角对应的四个矩形
	AddRect(rect.left,rect.top,iRoundH,iRoundH);
	AddRect(rect.left + rect.Width() - iRoundH,rect.top,iRoundH,iRoundH);
	AddRect(rect.left + rect.Width() - iRoundH,rect.top + rect.Height() - iRoundH,iRoundH,iRoundH);
	AddRect(rect.left,rect.top + rect.Height() - iRoundH,iRoundH,iRoundH);
	//圆角边框色带透明度
	AddRect(rect.left + iRoundH,rect.top,rect.Width() - i2RoundH,iFrameW);
	AddRect(rect.left + rect.Width() - iFrameW,rect.top + iRoundH,iFrameW,rect.Height() - i2RoundH);
	AddRect(rect.left + iRoundH,rect.top + rect.Height() - iFrameW,rect.left + rect.Width() - iRoundH,iFrameW);
	AddRect(rect.left,rect.top + iRoundH,iFrameW,rect.Height() - i2RoundH);

	return 0;
}

int CRectManage::AddRectRoundFrame(int iLeft,int iTop,int iWidth,int iHeight,int iRoundH,int iFrameW)
{
	int i2RoundH = iRoundH * 2;
	//按钮的四角对应的四个矩形
	AddRect(iLeft,iTop,iRoundH,iRoundH);
	AddRect(iLeft + iWidth - iRoundH,iTop,iRoundH,iRoundH);
	AddRect(iLeft + iWidth - iRoundH,iTop + iHeight - iRoundH,iRoundH,iRoundH);
	AddRect(iLeft,iTop + iHeight - iRoundH,iRoundH,iRoundH);
	//圆角边框色带透明度
	AddRect(iLeft + iRoundH,iTop,iWidth - i2RoundH,iFrameW);
	AddRect(iLeft + iWidth - iFrameW,iTop + iRoundH,iFrameW,iHeight - i2RoundH);
	AddRect(iLeft + iRoundH,iTop + iHeight - iFrameW,iLeft + iWidth - iRoundH,iFrameW);
	AddRect(iLeft,iTop + iRoundH,iFrameW,iHeight - i2RoundH);

	return 0;
}

#include "StdAfx.h"
#include "CaretInfo.h"


CCaretInfo::CCaretInfo(void)
{
	m_pxXStart = 0;
	m_pxYStart = 0;
	m_pxWidth = 1;
	m_pxHeight = 14;
	m_iCurCharIndex = -1;
}

CCaretInfo::~CCaretInfo(void)
{

}

CCaretInfo::CCaretInfo(const CCaretInfo& caretInfo)
{
	m_pxXStart = caretInfo.m_pxXStart;
	m_pxYStart = caretInfo.m_pxYStart;
	m_pxWidth = caretInfo.m_pxWidth;
	m_pxHeight = caretInfo.m_pxHeight;
	m_iCurCharIndex = caretInfo.m_iCurCharIndex;
}

void CCaretInfo::operator=(const CCaretInfo& caretInfo)
{
	m_pxXStart = caretInfo.m_pxXStart;
	m_pxYStart = caretInfo.m_pxYStart;
	m_pxWidth = caretInfo.m_pxWidth;
	m_pxHeight = caretInfo.m_pxHeight;
	m_iCurCharIndex = caretInfo.m_iCurCharIndex;
}

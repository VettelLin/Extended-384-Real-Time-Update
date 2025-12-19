#include "StdAfx.h"
#include "FtSetDensity.h"
#include "FontSet.h"

CFtSetDensity::CFtSetDensity(void)
{
	InitFontDensity(16);
}

CFtSetDensity::~CFtSetDensity(void)
{
	m_ftDensity.DeleteObject();
}

void CFtSetDensity::InitFontDensity(int lfHeight)
{
	m_logFtDensity.lfHeight = lfHeight;
	m_logFtDensity.lfWidth = 0;
	m_logFtDensity.lfEscapement = 0;
	m_logFtDensity.lfOrientation = 400;
	m_logFtDensity.lfWeight = FW_NORMAL;
	m_logFtDensity.lfItalic = FALSE;
	m_logFtDensity.lfUnderline = FALSE;
	m_logFtDensity.lfStrikeOut = 0;
	m_logFtDensity.lfCharSet = ANSI_CHARSET;
	m_logFtDensity.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFtDensity.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logFtDensity.lfQuality = PROOF_QUALITY;
	m_logFtDensity.lfPitchAndFamily = VARIABLE_PITCH | FF_SCRIPT;
	_tcscpy(m_logFtDensity.lfFaceName,  _T("Î¢ÈíÑÅºÚ"));

	m_ftDensity.CreateFontIndirect(&m_logFtDensity);  
}

void CFtSetDensity::SetMyDensityFont(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/) //nHeightµ¥Î»Îª °õ£¬¼´1/72Ó¢´ç
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFtDensity.lfHeight = (double(nHeight * CFontSet::fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFtDensity.lfHeight = nHeight;
	}

	_tcscpy(m_logFtDensity.lfFaceName, fontName);
	m_logFtDensity.lfWeight = nWeight;
	m_ftDensity.DeleteObject();
	m_ftDensity.CreateFontIndirect(&m_logFtDensity);
}

void CFtSetDensity::SetMyDensityFont() 
{
	m_ftDensity.DeleteObject();
	m_ftDensity.CreateFontIndirect(&m_logFtDensity);
}


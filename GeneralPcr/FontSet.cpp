
#include "stdafx.h"
#include "FontSet.h"

int CFontSet::fontSetDesDpi = 96;

int CFontSet::GeneDpi = 96;
CRect CFontSet::GeneCRDlg = CRect(0,0,1920,1080);
int CFontSet::m_uiGeneDesDpi = 96;
int CFontSet::m_uiGeneDesWidth = 1920;
int CFontSet::m_uiGeneDesHeight = 1030;
int CFontSet::GeneScreenXWidth = 1920;
int CFontSet::GeneScreenYHeight = 1080;

double CFontSet::m_dHoriRatio = 1;
double CFontSet::m_dVertRatio = 1;
double CFontSet::m_dTxRatio = 1;
CString CFontSet::m_stTxFont = TEXTFONT_CHINESE;

CFontSet::CFontSet(BOOL bCkBt /*= FALSE*/)
{
	m_bCkBt = bCkBt;
	m_cfsTxClr = RGB(0,0,0);
	m_txClr1 = RGB(0,0,0);
	InitFont(16);
}

CFontSet::~CFontSet()
{
	m_fontSet.DeleteObject();
	if(m_bCkBt)
	{
		m_fontSetChecked.DeleteObject();
	}
}

void CFontSet::SetMyFont(LOGFONT& logFont)
{
	m_fontSet.DeleteObject();
	m_fontSet.CreateFontIndirect(&logFont);
}

void CFontSet::SetMyFont(int nHeight,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)  //nHeightµ¥Î»Îª °õ£¬¼´1/72Ó¢´ç
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFont.lfHeight = (double(nHeight * fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFont.lfHeight = nHeight;
	}
	SetMyFont();
	m_fontOriginalHeight = m_logFont.lfHeight;
}

void CFontSet::SetMyFont(CString fontName)
{
	_tcscpy(m_logFont.lfFaceName, fontName);
	SetMyFont();
}

void CFontSet::SetMyFont(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/) //nHeightµ¥Î»Îª °õ£¬¼´1/72Ó¢´ç
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFont.lfHeight = (double(nHeight * fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFont.lfHeight = nHeight;
	}
	
	_tcscpy(m_logFont.lfFaceName, fontName);
	m_logFont.lfWeight = nWeight;
	SetMyFont();
	m_fontOriginalHeight = m_logFont.lfHeight;
}

void CFontSet::SetMyFont()
{
	m_fontSet.DeleteObject();
	m_fontSet.CreateFontIndirect(&m_logFont);
}

void CFontSet::SetMyFontChecked()
{
	m_fontSetChecked.DeleteObject();
	m_fontSetChecked.CreateFontIndirect(&m_logFontChecked);
}

void CFontSet::SetMyFontChecked(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFontChecked.lfHeight = (double(nHeight * fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFontChecked.lfHeight = nHeight;
	}
	
	_tcscpy(m_logFontChecked.lfFaceName,fontName);
	m_logFontChecked.lfWeight = nWeight;
	SetMyFontChecked();
	m_fontOriginHChecked = m_logFontChecked.lfHeight;
}

void CFontSet::InitFont(int lfHeight)
{
	m_logFont.lfHeight = lfHeight;
	m_logFont.lfWidth = 0;
	m_logFont.lfEscapement = 0;
	m_logFont.lfOrientation = 400;
	m_logFont.lfWeight = FW_NORMAL;
	m_logFont.lfItalic = FALSE;
	m_logFont.lfUnderline = FALSE;
	m_logFont.lfStrikeOut = 0;
	m_logFont.lfCharSet = /*GB2312_CHARSET*/ANSI_CHARSET;
	m_logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logFont.lfQuality = PROOF_QUALITY;
	m_logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SCRIPT;
	_tcscpy(m_logFont.lfFaceName,  _T("Î¢ÈíÑÅºÚ"));

	m_fontSet.CreateFontIndirect(&m_logFont);  
	m_fontOriginalHeight = 17;

	if(m_bCkBt)
	{
		m_logFontChecked.lfHeight = lfHeight;
		m_logFontChecked.lfWidth = 0;
		m_logFontChecked.lfEscapement = 0;
		m_logFontChecked.lfOrientation = 400;
		m_logFontChecked.lfWeight = FW_NORMAL;
		m_logFontChecked.lfItalic = FALSE;
		m_logFontChecked.lfUnderline = FALSE;
		m_logFontChecked.lfStrikeOut = 0;
		m_logFontChecked.lfCharSet = ANSI_CHARSET;
		m_logFontChecked.lfOutPrecision = OUT_DEFAULT_PRECIS;
		m_logFontChecked.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		m_logFontChecked.lfQuality = PROOF_QUALITY;
		m_logFontChecked.lfPitchAndFamily = VARIABLE_PITCH | FF_SCRIPT;
		_tcscpy(m_logFontChecked.lfFaceName,  _T("Î¢ÈíÑÅºÚ"));

		m_fontSetChecked.CreateFontIndirect(&m_logFont);  
		m_fontOriginHChecked = 17;
	}
}

LOGFONT& CFontSet::GetLogFont()
{
	return m_logFont;
}

LOGFONT& CFontSet::GetLogFontChecked()
{
	return m_logFontChecked;
}

void CFontSet::SetCfsTextClr(COLORREF txClr)
{
	m_cfsTxClr = txClr;
}

COLORREF& CFontSet::GetCfsTextClr()
{
	return m_cfsTxClr;
}

void CFontSet::SetTextClr1(COLORREF txClr)
{
	m_txClr1 = txClr;
}

COLORREF& CFontSet::GetTextClr1()
{
	return m_txClr1;
}

void CFontSet::SetMultiDpiAdaptive()
{
	double horiFontRatio = (double(m_fontOriginalHeight * GeneDpi)) / m_uiGeneDesDpi / GeneScreenXWidth;
	double verFontRatio = (double(m_fontOriginalHeight * GeneDpi)) / m_uiGeneDesDpi / GeneScreenYHeight;
	GetLogFont().lfHeight = GeneCRDlg.Width() * horiFontRatio * 0.5 + GeneCRDlg.Height() * verFontRatio * 0.5;
	SetMyFont();
}
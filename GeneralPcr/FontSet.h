
#pragma once

#include <string>
#include "EnumFtUnitType.h"

using namespace std;

class CFontSet
{
public:
	CFontSet(BOOL bCkBt = FALSE);
	~CFontSet();

public:
	CFont m_fontSet;
	LOGFONT m_logFont;
	CFont m_fontSetChecked;
	LOGFONT m_logFontChecked;

	COLORREF m_cfsTxClr;
	COLORREF m_txClr1;

	int m_fontOriginalHeight;
	int m_fontOriginHChecked;

	static int fontSetDesDpi;
	static int GeneDpi;
	static CRect GeneCRDlg;
	static int m_uiGeneDesDpi;
	static int m_uiGeneDesWidth;
	static int m_uiGeneDesHeight;
	static int GeneScreenXWidth;
	static int GeneScreenYHeight;

	static double m_dHoriRatio;
	static double m_dVertRatio;
	static double m_dTxRatio;	//文字大小自动适配比率值，用于缩放
	static CString m_stTxFont;	//记录字体名称

private:
	BOOL m_bCkBt;

public:
	void SetMyFont();
	void SetMyFontChecked();
	void SetMyFont(LOGFONT& logFont);
	void SetMyFont(int nHeight,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL);  //nHeight单位为 磅，即1/72英寸
	void SetMyFont(CString fontName);
	void SetMyFont(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸
	void SetMyFontChecked(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_PIXEL); //nHeight单位为 磅，即1/72英寸
	void InitFont(int lfHeight);  //默认字体为“微软雅黑”，高度为17像素值
	LOGFONT& GetLogFont();
	LOGFONT& GetLogFontChecked();

	void SetCfsTextClr(COLORREF txClr);
	COLORREF& GetCfsTextClr();

	void SetTextClr1(COLORREF txClr);
	COLORREF& GetTextClr1();

	void SetMultiDpiAdaptive();	//窗口中直接输出文字时，需使用此函数适配字体大小
};


#ifndef _EXCEL_OFFICE
#define _EXCEL_OFFICE
#include <atlbase.h>
#include "excel.h"
/************************************************************************/
/*Excel 存储格式为2007版本                                                                      */
/************************************************************************/
using namespace myExcel;
class AFX_EXT_CLASS CExcelOffice
{
public:
	CExcelOffice(void);
	virtual ~CExcelOffice(void);

public:
	BOOL SaveDataByOle(int ivalue ,CString filePath,int irowindex=1);
	BOOL SaveDataByOle(const CUIntArray& uiPhotonArray,CString filePath,int irowindex=1);
	BOOL SaveDataByOleNCol(const CUIntArray& arydata ,CString filePath,int idatanumspercol,int irowindex/* =1 */);
	BOOL SaveDataByOleNsheetMcol(const CUIntArray& sheet1,const CUIntArray& sheet2,const CUIntArray& sheet3,const CUIntArray& sheet4,const CUIntArray& sheet5,const CUIntArray& sheet6,int idatanumspercol,CString filePath,int irowindex/* =1 */);
public:

	CFile file1;
	CFileException fileException1;
	CApplication0 ExcelApp;
	CWorkbook0 book;
	CWorkbooks0 books;
	CWorksheet0 sheet;
	CWorksheets0 sheets;
	CRange0 range;
	LPDISPATCH lpDisp;
};
#endif

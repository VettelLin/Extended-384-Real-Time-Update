#include "StdAfx.h"
#include "ExcelOffice.h"
#include "comutil.h"


CExcelOffice::CExcelOffice(void)
	:lpDisp(NULL)
{
}


CExcelOffice::~CExcelOffice(void)
{
}


BOOL CExcelOffice::SaveDataByOle(int ivalue ,CString filePath,int irowindex/* =1 */)
{
	bool bOleInit = false;
	if (filePath == _T(""))
	{
		return FALSE;
	}
	BOOL bRet=FALSE;

	CFileFind filefind;
	BOOL bfind = filefind.FindFile(filePath);
	if (bfind)
	{
		if (file1.Open(filePath,CFile::modeReadWrite|CFile::shareExclusive  ,&fileException1))
		{
			file1.SeekToEnd();
			file1.Close();
		}
		else
		{
			AfxMessageBox(filePath +_T("	已打开，请先关闭!"));
			return FALSE;
			//file1.Close();
			//file1.SeekToEnd();
		}
	}
	else
	{
		if (file1.Open(filePath,CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate  ,&fileException1))
		{
			file1.SeekToEnd();
			file1.Close();
		}
		else
		{
			AfxMessageBox(filePath +_T("	创建文件失败!"));
			return FALSE;
		}
	}

	//创建Excel 服务器(启动Excel)
	if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
	{
		if (!AfxOleInit())
		{
			AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
			return FALSE ;
		}
		else
		{
			bOleInit = true;
			if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
			{
				AfxMessageBox(_T("启动Excel服务器失败!"));
				AfxOleTerm();
				return FALSE;
			}
		}
	}

	//ExcelApp.put_Visible(TRUE);
	ExcelApp.put_UserControl(FALSE);

	/*得到工作簿容器*/
	books.AttachDispatch(ExcelApp.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	CString strBookPath = filePath;
	try
	{
		/*打开一个工作簿*/
		lpDisp = books.Open(strBookPath, 
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing, vtMissing, vtMissing);
		book.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		/*增加一个新的工作簿*/
		lpDisp = books.Add(vtMissing);
		book.AttachDispatch(lpDisp);
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());

	/*打开一个Sheet，如不存在，就新增一个Sheet*/
	CString strSheetName = _T("Data");
	try
	{
		/*打开一个已有的Sheet*/
		lpDisp = sheets.get_Item(_variant_t(strSheetName));
		sheet.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		/*创建一个新的Sheet*/
		lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		sheet.AttachDispatch(lpDisp);
		sheet.put_Name(strSheetName);
	}

	CRange0   usedRange;   
	usedRange.AttachDispatch(sheet.get_UsedRange());   
	range.AttachDispatch(usedRange.get_Rows());   
	long   iRowNum=range.get_Count();  
	range.ReleaseDispatch();
	range.AttachDispatch(usedRange.get_Columns());   
	long   iColNum=range.get_Count();                                       //已经使用的列数   
	range.ReleaseDispatch();
	usedRange.ReleaseDispatch();
	if ((1==iRowNum)&&(1 == iColNum))
	{
		irowindex = 1;
	}
	else
	{
		irowindex = iRowNum +1;
	}

	CString csPosA=_T("");
	csPosA.Format(_T("%d"),irowindex);
	csPosA= _T("A") +csPosA;
	range =sheet.get_Range(_variant_t(csPosA),_variant_t(csPosA));
	range.put_NumberFormat(_variant_t(_T("yyyy-MM-dd hh:mm:ss")));
	CString strTime;
	CTime tTime = CTime::GetCurrentTime();
	strTime = tTime.Format("%Y-%m-%d %H:%M:%S");
	range.put_Value2(_variant_t(strTime));

	CString csPosB=_T("");
	csPosB.Format(_T("%d"),irowindex);
	csPosB= _T("B") +csPosB;
	range =sheet.get_Range(_variant_t(csPosB),_variant_t(csPosB));
	range.put_NumberFormat(_variant_t(_T("0")));
	range.put_Value2(_variant_t(ivalue));
	//调整列宽
	range.AttachDispatch(sheet.get_Columns());
	range.AutoFit();
	/*根据文件的后缀名选择保存文件的格式*/
	CString strSaveAsName = filePath;
	CString strSuffix = strSaveAsName.Mid(strSaveAsName.ReverseFind(_T('.')));

	ExcelApp.put_DisplayAlerts(FALSE);

	try
	{
		book.SaveAs(_variant_t(strSaveAsName),vtMissing/* _variant_t((long)56)*/, vtMissing, vtMissing, vtMissing, 
			vtMissing, 0,vtMissing , vtMissing, vtMissing, 
			vtMissing,vtMissing);
		book.put_Saved(TRUE); //将Workbook的保存状态设置为已保存，即不让系统提示是否人工保存
	}
	catch (...)
	{

	}
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	book.Close(covOptional, COleVariant(strSaveAsName), covOptional);
	books.Close();
	ExcelApp.put_DisplayAlerts(TRUE);


	bRet=TRUE;


	/*释放资源*/
	range.ReleaseDispatch();    
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();

	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();
	if (bOleInit)
	{
		AfxOleTerm();
	}
	return bRet;
}

BOOL CExcelOffice::SaveDataByOle(const CUIntArray& uiPhotonArray,CString filePath,int irowindex/* =1 */)
{
	bool bOleInit = false;
	int icount =uiPhotonArray.GetCount();
	if (icount ==0)
	{
		CString strErrorInfo = _T("");
		strErrorInfo = _T("IDS_Count_SourceData_Empty");
		AfxMessageBox(strErrorInfo);
		return TRUE;
	}

	if (filePath == _T(""))
	{
		return FALSE;
	}

	BOOL bRet=FALSE;

	CFileFind filefind;
	BOOL bfind = filefind.FindFile(filePath);
	if (bfind)
	{
		if (file1.Open(filePath,CFile::modeReadWrite|CFile::shareExclusive  ,&fileException1))
		{
			file1.SeekToEnd();
			file1.Close();
		}
		else
		{
			AfxMessageBox(filePath +_T("	已打开，请先关闭!"));
			return FALSE;
		}
	}
	else
	{
		if (file1.Open(filePath,CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate  ,&fileException1))
		{
			file1.SeekToEnd();
			file1.Close();
		}
		else
		{
			AfxMessageBox(filePath +_T("	创建文件失败!"));
			return FALSE;
		}
	}

		//创建Excel 服务器(启动Excel)
		if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
		{
			if (!AfxOleInit())
			{
				AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
				return FALSE ;
			}
			else
			{
				bOleInit = true;
				if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
				{
					AfxMessageBox(_T("IDS_Excel_CreateDispatch_Application"));
					AfxOleTerm();
					return FALSE;
				}
			}
		}


		ExcelApp.put_UserControl(FALSE);

		/*得到工作簿容器*/
		books.AttachDispatch(ExcelApp.get_Workbooks());
		/*打开一个工作簿，如不存在，则新增一个工作簿*/
		CString strBookPath = filePath;
		try
		{
			/*打开一个工作簿*/
			lpDisp = books.Open(strBookPath, 
				vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
				vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
				vtMissing, vtMissing, vtMissing, vtMissing);
			book.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			/*增加一个新的工作簿*/
			lpDisp = books.Add(vtMissing);
			book.AttachDispatch(lpDisp);
		}
		/*得到工作簿中的Sheet的容器*/
		sheets.AttachDispatch(book.get_Sheets());

		/*打开一个Sheet，如不存在，就新增一个Sheet*/
		CString strSheetName;


		strSheetName = _T("Data");

		try
		{
			/*打开一个已有的Sheet*/
			lpDisp = sheets.get_Item(_variant_t(strSheetName));
			sheet.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			/*创建一个新的Sheet*/
			lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
			sheet.AttachDispatch(lpDisp);
			sheet.put_Name(strSheetName);
		}


		CRange0   usedRange;   
		usedRange.AttachDispatch(sheet.get_UsedRange());   
		range.AttachDispatch(usedRange.get_Rows());   
		long   iRowNum=range.get_Count();  
		range.ReleaseDispatch();
		range.AttachDispatch(usedRange.get_Columns());   
		long   iColNum=range.get_Count();                                       //已经使用的列数   
		range.ReleaseDispatch();
		usedRange.ReleaseDispatch();
		if ((1==iRowNum)&&(1 == iColNum))
		{
			irowindex = 1;
		}
		else
		{
			irowindex = iRowNum +1;
		}



		VARTYPE vt; /*数组元素的类型 */
		SAFEARRAYBOUND sabWrite[2]; /*用于定义数组的维数和下标的起始值*/

		CString csPosA=_T("");
		csPosA.Format(_T("%d"),irowindex);
		csPosA= _T("A") +csPosA;


		CString str;
		/*向Sheet中写入多个单元格,规模为icount*1 */
		str.Format(_T("%d"),icount+irowindex-1);
		str=_T("B")+str;
		lpDisp = sheet.get_Range(_variant_t(csPosA), _variant_t(str));
		range.AttachDispatch(lpDisp);
		vt =VT_I4; /*数组元素的类型  _32 int*/
		sabWrite[0].cElements = icount;
		sabWrite[0].lLbound = 0;
		sabWrite[1].cElements = 2;
		sabWrite[1].lLbound = 0;

		COleSafeArray olesaWrite;
		olesaWrite.Create(vt, sizeof(sabWrite)/sizeof(SAFEARRAYBOUND), sabWrite);

		/*对二维数组的元素进行逐个赋值*/
		long index[2] = {0, 0};
		long lFirstLBound = 0;
		long lFirstUBound = 0;
		long lSecondLBound = 0;
		long lSecondUBound = 0;
		olesaWrite.GetLBound(1, &lFirstLBound);
		olesaWrite.GetUBound(1, &lFirstUBound);
		olesaWrite.GetLBound(2, &lSecondLBound);
		olesaWrite.GetUBound(2, &lSecondUBound);

		for (long i = lFirstLBound; i <= lFirstUBound; i++)
		{
			index[0] = i;
			for (long j = lSecondLBound; j <= lSecondUBound; j++)
			{
				index[1] = j;
				unsigned int lElement;
				if (index[1]==0)
				{
					/*数据的序号*/
					lElement = i+1;
				}
				else
				{
					/*数据的数值*/
					lElement =uiPhotonArray[i];
				}
				olesaWrite.PutElement(index, &lElement);
			}
		}

		/*把ColesaWritefeArray变量转换为VARIANT,并写入到Excel表格中*/
		_variant_t varWrite = (_variant_t)(olesaWrite);
		range.put_Value2(varWrite);

		//调整列宽
		range.AttachDispatch(sheet.get_Columns());
		range.AutoFit();
		/*根据文件的后缀名选择保存文件的格式*/
		CString strSaveAsName = filePath;
		CString strSuffix = strSaveAsName.Mid(strSaveAsName.ReverseFind(_T('.')));

		ExcelApp.put_DisplayAlerts(FALSE);

		try
		{
			book.SaveAs(_variant_t(strSaveAsName), vtMissing/*_variant_t((long)56)*/, vtMissing, vtMissing, vtMissing, 
				vtMissing, 0,vtMissing , vtMissing, vtMissing, 
				vtMissing,vtMissing);
			book.put_Saved(TRUE); //将Workbook的保存状态设置为已保存，即不让系统提示是否人工保存
		}
		catch (...)
		{

		}
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		book.Close(covOptional, COleVariant(strSaveAsName), covOptional);
		books.Close();
		ExcelApp.put_DisplayAlerts(TRUE);

		bRet=TRUE;


		/*释放资源*/
		range.ReleaseDispatch();    
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();

		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		if (bOleInit)
		{
			AfxOleTerm();
		}
		return bRet;
}

BOOL CExcelOffice::SaveDataByOleNCol(const CUIntArray& arydata ,CString filePath,int idatanumspercol,int irowindex/* =1 */)
{
	bool bOleInit = false;
	int idatanum = arydata.GetCount();
	if (filePath == _T("")||idatanum<=0)
	{
		return FALSE;
	}
	BOOL bRet=FALSE;

	CFileFind filefind;
	BOOL bfind = filefind.FindFile(filePath);
	if (bfind)
	{
		if (file1.Open(filePath,CFile::modeReadWrite|CFile::shareExclusive  ,&fileException1))
		{
			file1.SeekToEnd();
			file1.Close();
		}
		else
		{
			AfxMessageBox(filePath +_T("	已打开，请先关闭!"));
			return FALSE;
		}
	}

	//创建Excel 服务器(启动Excel)
	if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
	{
		if (!AfxOleInit())
		{
			AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
			return FALSE ;
		}
		else
		{
			bOleInit = true;
			if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
			{
				AfxMessageBox(_T("启动Excel服务器失败!"));
				AfxOleTerm();
				return FALSE;
			}
		}
	}

	//ExcelApp.put_Visible(TRUE);
	ExcelApp.put_UserControl(FALSE);

	/*得到工作簿容器*/
	books.AttachDispatch(ExcelApp.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	CString strBookPath = filePath;
	try
	{
		/*打开一个工作簿*/
		lpDisp = books.Open(strBookPath, 
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing, vtMissing, vtMissing);
		book.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		/*增加一个新的工作簿*/
		lpDisp = books.Add(vtMissing);
		book.AttachDispatch(lpDisp);
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());

	/*打开一个Sheet，如不存在，就新增一个Sheet*/
	CString strSheetName = _T("Data");
	try
	{
		/*打开一个已有的Sheet*/
		lpDisp = sheets.get_Item(_variant_t(strSheetName));
		sheet.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		/*创建一个新的Sheet*/
		lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		sheet.AttachDispatch(lpDisp);
		sheet.put_Name(strSheetName);
	}

	CRange0   usedRange;   
	usedRange.AttachDispatch(sheet.get_UsedRange());   
	range.AttachDispatch(usedRange.get_Rows());   
	long   iRowNum=range.get_Count();  
	range.ReleaseDispatch();
	range.AttachDispatch(usedRange.get_Columns());   
	long   iColNum=range.get_Count();                                       //已经使用的列数   
	range.ReleaseDispatch();
	usedRange.ReleaseDispatch();
	if ((1==iRowNum)&&(1 == iColNum))
	{
		irowindex = 1;
	}
	else
	{
		irowindex = iRowNum +1;
	}

	CString csPosA=_T("");
	csPosA.Format(_T("%d"),irowindex);
	csPosA= _T("A") +csPosA;
	CString csPosB=_T("");
	csPosB.Format(_T("%d"),irowindex);
	csPosB= _T("B") +csPosB;

	range =sheet.get_Range(_variant_t(csPosA), _variant_t(csPosB));
	range.ClearContents();  
	range.Merge(_variant_t((long)1));
	range.put_Value2(_variant_t(_T("QPCR 原始数据")));
	range.put_HorizontalAlignment(_variant_t((long)-4108));

	irowindex++;
	csPosA=_T("");
	csPosA.Format(_T("%d"),irowindex);
	csPosA= _T("A") +csPosA;
	csPosB=_T("");
	csPosB.Format(_T("%d"),irowindex);
	csPosB= _T("B") +csPosB;

	range =sheet.get_Range(_variant_t(csPosA),_variant_t(csPosA) );
	range.put_Value2(_variant_t(_T("获取时间:")));
	range =sheet.get_Range(_variant_t(csPosB),_variant_t(csPosB) );
	range.put_NumberFormat(_variant_t(_T("yyyy-MM-dd hh:mm:ss")));
	CString strTime;
	CTime tTime = CTime::GetCurrentTime();
	strTime = tTime.Format("%Y-%m-%d %H:%M:%S");
	range.put_Value2(_variant_t(strTime));



	irowindex++;
	
	int iDataCountPerRowCol = idatanumspercol;
	csPosA=_T("");
	csPosA.Format(_T("%d"),irowindex);
	csPosA= _T("A") +csPosA;
	csPosB=_T("");
	csPosB.Format(_T("%d"),irowindex+iDataCountPerRowCol-1);
	int iColcount = idatanum / iDataCountPerRowCol + 1;
	int iParam1 = iColcount/26 +1;
	if (iParam1 <= 1)
	{
		csPosB= _T("Z") +csPosB;
	}
	else
	{
		char collable = 'A';
		collable += (iParam1-2);
		CString str(collable);
		csPosB= str + _T("Z") +csPosB;
	}

	/*向Sheet中写入多个单元格,规模为-- */
	lpDisp = sheet.get_Range(_variant_t(csPosA), _variant_t(csPosB));
	range.AttachDispatch(lpDisp);

	VARTYPE vt; /*数组元素的类型 */
	SAFEARRAYBOUND sabWrite[2]; /*用于定义数组的维数和下标的起始值*/
	vt =VT_I4; /*数组元素的类型  _32 int*/
	sabWrite[0].cElements = iDataCountPerRowCol;
	sabWrite[0].lLbound = 0;
	sabWrite[1].cElements = iParam1*26;
	sabWrite[1].lLbound = 0;

	COleSafeArray olesaWrite;
	olesaWrite.Create(vt, sizeof(sabWrite)/sizeof(SAFEARRAYBOUND), sabWrite);

	/*对二维数组的元素进行逐个赋值*/
	long index[2] = {0, 0};
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaWrite.GetLBound(1, &lFirstLBound);
	olesaWrite.GetUBound(1, &lFirstUBound);
	olesaWrite.GetLBound(2, &lSecondLBound);
	olesaWrite.GetUBound(2, &lSecondUBound);

	for (long i = lFirstLBound; i <= lFirstUBound; i++)
	{
		index[0] = i;
		for (long j = lSecondLBound; j <= lSecondUBound; j++)
		{
			index[1] = j;
			unsigned int lElement;
			if( (j * (lFirstUBound+1) + i) < idatanum )
			{
				lElement = arydata.GetAt(j * (lFirstUBound+1) + i);
			}
			else
			{
				break;
			}
			olesaWrite.PutElement(index, &lElement);
		}
	}
	/*把ColesaWritefeArray变量转换为VARIANT,并写入到Excel表格中*/
	_variant_t varWrite = (_variant_t)(olesaWrite);
	range.put_Value2(varWrite);

	//调整列宽
	range.AttachDispatch(sheet.get_Columns());
	range.AutoFit();
	/*根据文件的后缀名选择保存文件的格式*/
	CString strSaveAsName = filePath;
	CString strSuffix = strSaveAsName.Mid(strSaveAsName.ReverseFind(_T('.')));

	ExcelApp.put_DisplayAlerts(FALSE);

	try
	{
		book.SaveAs(_variant_t(strSaveAsName), vtMissing, vtMissing, vtMissing, vtMissing, 
			vtMissing, 0,vtMissing , vtMissing, vtMissing, 
			vtMissing,vtMissing);
		book.put_Saved(TRUE); //将Workbook的保存状态设置为已保存，即不让系统提示是否人工保存
	}
	catch (...)
	{
		int irror = GetLastError();
		int ifj= 9;
		ifj++;
	}
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	book.Close(covOptional, COleVariant(strSaveAsName), covOptional);
	books.Close();
	ExcelApp.put_DisplayAlerts(TRUE);


	bRet=TRUE;


	/*释放资源*/
	range.ReleaseDispatch();    
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();

	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();

	if (bOleInit)
	{
		AfxOleTerm();
	}

	return bRet;
}

BOOL CExcelOffice::SaveDataByOleNsheetMcol(const CUIntArray& sheet1,const CUIntArray& sheet2,const CUIntArray& sheet3,const CUIntArray& sheet4,const CUIntArray& sheet5,const CUIntArray& sheet6,int idatanumspercol,CString filePath,int irowindex)
{
	bool bOleInit = false;
	if (filePath == _T(""))
	{
		return FALSE;
	}
	BOOL bRet=FALSE;

	CFileFind filefind;
	BOOL bfind = filefind.FindFile(filePath);
	if (bfind)
	{
		if (file1.Open(filePath,CFile::modeReadWrite|CFile::shareExclusive  ,&fileException1))
		{
			file1.SeekToEnd();
			file1.Close();
		}
		else
		{
			AfxMessageBox(filePath +_T("	已打开，请先关闭!"));
			return FALSE;
		}
	}

	//创建Excel 服务器(启动Excel)
	if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
	{
		if (!AfxOleInit())
		{
			AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
			return FALSE ;
		}
		else
		{
			bOleInit = true;
			if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
			{
				AfxMessageBox(_T("启动Excel服务器失败!"));
				AfxOleTerm();
				return FALSE;
			}
		}
	}

	//ExcelApp.put_Visible(TRUE);
	ExcelApp.put_UserControl(FALSE);

	/*得到工作簿容器*/
	books.AttachDispatch(ExcelApp.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	CString strBookPath = filePath;
	try
	{
		/*打开一个工作簿*/
		lpDisp = books.Open(strBookPath, 
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing, vtMissing, vtMissing);
		book.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		/*增加一个新的工作簿*/
		lpDisp = books.Add(vtMissing);
		book.AttachDispatch(lpDisp);
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());

	for (int isheetindex = 1;isheetindex <=6;isheetindex++)
	{
		/*打开一个Sheet，如不存在，就新增一个Sheet*/
		CString strSheetName =_T("");
		strSheetName.Format(_T("Sheet%d"),isheetindex);
		switch(isheetindex)
		{
		case 1:
			{
				strSheetName = _T("FAM1");
				break;
			}
		case 2:
			{
				strSheetName = _T("Cy5");
				break;
			}
		case 3:
			{
				strSheetName = _T("VIC");
				break;
			}
		case 4:
			{
				strSheetName = _T("FAM4");
				break;
			}
		case 5:
			{
				strSheetName = _T("ROX");
				break;
			}
		case 6:
			{
				strSheetName = _T("Cy3");
				break;
			}
		}
		try
		{
			/*打开一个已有的Sheet*/
			lpDisp = sheets.get_Item(_variant_t(strSheetName));
			sheet.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			/*创建一个新的Sheet*/
			LPDISPATCH lpDispLast =sheets.get_Item(COleVariant(sheets.get_Count()));
			//lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
			lpDisp = sheets.Add(vtMissing, _variant_t(lpDispLast), _variant_t((long)1), vtMissing);
			sheet.AttachDispatch(lpDisp);
			sheet.put_Name(strSheetName);
		}
		CUIntArray arydata;
		switch(isheetindex)
		{
		case 1:
			{
				arydata.Copy(sheet1);
				break;
			}
		case 2:
			{
				arydata.Copy(sheet2);
				break;
			}
		case 3:
			{
				arydata.Copy(sheet3);
				break;
			}
		case 4:
			{
				arydata.Copy(sheet4);
				break;
			}
		case 5:
			{
				arydata.Copy(sheet5);
				break;
			}
		case 6:
			{
				arydata.Copy(sheet6);
				break;
			}
		}
		int idatanum = arydata.GetCount();

		CRange0   usedRange;   
		usedRange.AttachDispatch(sheet.get_UsedRange());   
		range.AttachDispatch(usedRange.get_Rows());   
		long   iRowNum=range.get_Count();  
		range.ReleaseDispatch();
		range.AttachDispatch(usedRange.get_Columns());   
		long   iColNum=range.get_Count();                                       //已经使用的列数   
		range.ReleaseDispatch();
		usedRange.ReleaseDispatch();
		if ((1==iRowNum)&&(1 == iColNum))
		{
			irowindex = 1;
		}
		else
		{
			irowindex = iRowNum +1;
		}

		CString csPosA=_T("");
		csPosA.Format(_T("%d"),irowindex);
		csPosA= _T("A") +csPosA;
		CString csPosB=_T("");
		csPosB.Format(_T("%d"),irowindex);
		csPosB= _T("B") +csPosB;

		range =sheet.get_Range(_variant_t(csPosA), _variant_t(csPosB));
		range.ClearContents();  
		range.Merge(_variant_t((long)1));
		range.put_Value2(_variant_t(_T("QPCR 原始数据")));
		range.put_HorizontalAlignment(_variant_t((long)-4108));

		irowindex++;
		csPosA=_T("");
		csPosA.Format(_T("%d"),irowindex);
		csPosA= _T("A") +csPosA;
		csPosB=_T("");
		csPosB.Format(_T("%d"),irowindex);
		csPosB= _T("B") +csPosB;

		range =sheet.get_Range(_variant_t(csPosA),_variant_t(csPosA) );
		range.put_Value2(_variant_t(_T("获取时间:")));
		range =sheet.get_Range(_variant_t(csPosB),_variant_t(csPosB) );
		range.put_NumberFormat(_variant_t(_T("yyyy-MM-dd hh:mm:ss")));
		CString strTime;
		CTime tTime = CTime::GetCurrentTime();
		strTime = tTime.Format("%Y-%m-%d %H:%M:%S");
		range.put_Value2(_variant_t(strTime));



		irowindex++;

		int iDataCountPerRowCol = idatanumspercol;
		csPosA=_T("");
		csPosA.Format(_T("%d"),irowindex);
		csPosA= _T("A") +csPosA;
		csPosB=_T("");
		csPosB.Format(_T("%d"),irowindex+iDataCountPerRowCol-1);
		int iColcount = idatanum / iDataCountPerRowCol + 1;
		int iParam1 = iColcount/26 +1;
		if (iParam1 <= 1)
		{
			csPosB= _T("Z") +csPosB;
		}
		else
		{
			char collable = 'A';
			collable += (iParam1-2);
			CString str(collable);
			csPosB= str + _T("Z") +csPosB;
		}

		/*向Sheet中写入多个单元格,规模为-- */
		lpDisp = sheet.get_Range(_variant_t(csPosA), _variant_t(csPosB));
		range.AttachDispatch(lpDisp);

		VARTYPE vt; /*数组元素的类型 */
		SAFEARRAYBOUND sabWrite[2]; /*用于定义数组的维数和下标的起始值*/
		vt =VT_I4; /*数组元素的类型  _32 int*/
		sabWrite[0].cElements = iDataCountPerRowCol;
		sabWrite[0].lLbound = 0;
		sabWrite[1].cElements = iParam1*26;
		sabWrite[1].lLbound = 0;

		COleSafeArray olesaWrite;
		olesaWrite.Create(vt, sizeof(sabWrite)/sizeof(SAFEARRAYBOUND), sabWrite);

		/*对二维数组的元素进行逐个赋值*/
		long index[2] = {0, 0};
		long lFirstLBound = 0;
		long lFirstUBound = 0;
		long lSecondLBound = 0;
		long lSecondUBound = 0;
		olesaWrite.GetLBound(1, &lFirstLBound);
		olesaWrite.GetUBound(1, &lFirstUBound);
		olesaWrite.GetLBound(2, &lSecondLBound);
		olesaWrite.GetUBound(2, &lSecondUBound);

		for (long i = lFirstLBound; i <= lFirstUBound; i++)
		{
			index[0] = i;
			for (long j = lSecondLBound; j <= lSecondUBound; j++)
			{
				index[1] = j;
				unsigned int lElement;
				if( (j * (lFirstUBound+1) + i) < idatanum )
				{
					lElement = arydata.GetAt(j * (lFirstUBound+1) + i);
				}
				else
				{
					break;
				}
				olesaWrite.PutElement(index, &lElement);
			}
		}
		/*把ColesaWritefeArray变量转换为VARIANT,并写入到Excel表格中*/
		_variant_t varWrite = (_variant_t)(olesaWrite);
		range.put_Value2(varWrite);

		//调整列宽
		range.AttachDispatch(sheet.get_Columns());
		range.AutoFit();

		//释放资源
		range.ReleaseDispatch();    
		sheet.ReleaseDispatch();
	}


	/*根据文件的后缀名选择保存文件的格式*/
	CString strSaveAsName = filePath;
	CString strSuffix = strSaveAsName.Mid(strSaveAsName.ReverseFind(_T('.')));

	ExcelApp.put_DisplayAlerts(FALSE);

	try
	{
		book.SaveAs(_variant_t(strSaveAsName),vtMissing/* _variant_t((long)56)*/, vtMissing, vtMissing, vtMissing, 
			vtMissing, 0,vtMissing , vtMissing, vtMissing, 
			vtMissing,vtMissing);
		book.put_Saved(TRUE); //将Workbook的保存状态设置为已保存，即不让系统提示是否人工保存
	}
	catch (...)
	{

	}
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	book.Close(covOptional, COleVariant(strSaveAsName), covOptional);
	books.Close();
	ExcelApp.put_DisplayAlerts(TRUE);


	bRet=TRUE;


	/*释放资源*/
	//range.ReleaseDispatch();    
	//sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();

	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();

	if (bOleInit)
	{
		AfxOleTerm();
	}

	return bRet;
}

#pragma once

#include<Windows.h>

typedef struct _INSPECT_DATA
{
	char* pData;
	int   dataLen;

public:
	/*_INSPECT_DATA()
	{
		pData   = NULL;
		dataLen = 0;
	}*/
}INSPECT_DATA;

typedef union _RESP_DATA  //pCodeInfo、pInspectData由外部调用者释放
{
	BYTE     IO_STATE;
	int      EP_DATA;
	USHORT   TEMP;
	char*    pCodeInfo;
	INSPECT_DATA inspectData;
} RESP_DATA;

typedef enum ComFunc{TEST_COM_ID_FUNC,NORMAL_COM_FUNC}COMFUNC;



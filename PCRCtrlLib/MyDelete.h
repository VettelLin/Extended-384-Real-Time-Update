
#pragma once

template<typename T>
void MyDeletePtr(T** pData)
{
	if((NULL != pData) && (NULL != *pData))
	{
		delete *pData;
		*pData = NULL;
	}
}

template<typename T>
void MyMulitDeletePtr(T** pData)
{
	if((NULL != pData) && (NULL != *pData))
	{
		delete [] *pData;
		*pData = NULL;
	}
}








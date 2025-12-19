#pragma once

#include "MyDelete.h"

template<typename T>
class Node
{
public:
	Node<T>* pNext;
	Node<T>* pPrevious;
	T		 data;
};

template<typename T>
class CLinkList
{
public:
	CLinkList();
	~CLinkList();

	void AddTail(T& element);
	void AddTail();
	void AddHead(T& element);
	void AddHead();
	BOOL InsertPreNode(Node<T>* pNode,T& element);	//在节点前添加节点
	BOOL InsertAfterNode(Node<T>* pNode,T& element);	//在节点后添加节点
	int	 InsertAtPre(int nodeId,T& element);	//在第nodeId + 1节点之前插入新的节点
	int	 InsertAtPre(int nodeId);	//在第nodeId + 1节点之前插入新的节点
	int	 InsertAtAfter(int nodeId,T& element);	//在第nodeId + 1节点之后插入新的节点
	int	 InsertAtAfter(int nodeId);	//在第nodeId + 1节点之后插入新的节点
	void DeleteTail();
	void DeleteHead();
	BOOL DeleteNode(Node<T>* pNode);
	BOOL DeleteData(T* pData);	//删除与pData指向数据相同的所有项,若未找到相同的项，仍然返回TRUE
	BOOL DeleteAt(int nodeId);
	BOOL DataInLink(T* pData);
	BOOL IsEmpty();
	void Clear();
	Node<T>* GetHead() const;
	Node<T>* GetTail();
	int  GetSize();
	Node<T>* GetAt(int index);
	void SetLimitMaxParam(BOOL bLimitMax,int maxNum);

	void operator=(const CLinkList<T>& lnkData);
	int operator+(CLinkList<T>& lnkData);
	T* operator[] (int index);

private:
	Node<T>* m_pHead;
	Node<T>* m_pTail;

	int m_size;
	int m_maxSize;	//最大样本个数
	BOOL m_bLimitMax;	//是否设置最大样本个数,如果超过最大限制，尾部添加时则删除头部数据，头部添加时反之,后插入则删除头部数据，前插入则删除尾部数据
};

#include "LinkList.h"

template<typename T>
CLinkList<T>::CLinkList()
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_size = 0;
	m_maxSize = 100;
	m_bLimitMax = FALSE;
}

template<typename T>
CLinkList<T>::~CLinkList()
{
	Node<T>* pElement = m_pHead;
	while (NULL != pElement)
	{
		Node<T>* pTemp = pElement->pNext;
		delete pElement;
		pElement = pTemp;
	}
}	

template<typename T>
void CLinkList<T>::AddTail(T& element)
{
	if(NULL == this)
	{
		return;
	}

	if(NULL == m_pHead && NULL == m_pTail)
	{
		m_pHead = new Node<T>;
		m_pHead->pNext = NULL;
		m_pHead->pPrevious = NULL;
		m_pHead->data = element;
		m_pTail = m_pHead;
	}
	else
	{
		Node<T>* pTemp = new Node<T>;
		pTemp->pNext = NULL;
		pTemp->pPrevious = m_pTail;
		pTemp->data = element;
		m_pTail->pNext = pTemp;
		m_pTail = pTemp;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteHead();
	}

	return;
}

template<typename T>
void CLinkList<T>::AddTail()
{
	if(NULL == this)
	{
		return;
	}

	if(NULL == m_pHead && NULL == m_pTail)
	{
		m_pHead = new Node<T>;
		m_pHead->pNext = NULL;
		m_pHead->pPrevious = NULL;
		m_pTail = m_pHead;
	}
	else
	{
		Node<T>* pTemp = new Node<T>;
		pTemp->pNext = NULL;
		pTemp->pPrevious = m_pTail;
		m_pTail->pNext = pTemp;
		m_pTail = pTemp;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteHead();
	}

	return;
}

template<typename T>
void CLinkList<T>::AddHead(T& element)
{
	if(NULL == this)
	{
		return;
	}

	if(NULL == m_pHead && NULL == m_pTail)
	{
		m_pHead = new Node<T>;
		m_pHead->pNext = NULL;
		m_pHead->pPrevious = NULL;
		m_pHead->data = element;
		m_pTail = m_pHead;
	}
	else
	{
		Node<T>* pTemp = new Node<T>;
		pTemp->pNext = m_pHead;
		pTemp->pPrevious = NULL;
		pTemp->data = element;
		m_pHead->pPrevious = pTemp;   
		m_pHead = pTemp;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteTail();
	}

	return;
}

template<typename T>
void CLinkList<T>::AddHead()
{
	if(NULL == this)
	{
		return;
	}

	if(NULL == m_pHead && NULL == m_pTail)
	{
		m_pHead = new Node<T>;
		m_pHead->pNext = NULL;
		m_pHead->pPrevious = NULL;
		m_pTail = m_pHead;
	}
	else
	{
		Node<T>* pTemp = new Node<T>;
		pTemp->pNext = m_pHead;
		pTemp->pPrevious = NULL;
		m_pHead->pPrevious = pTemp;   
		m_pHead = pTemp;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteTail();
	}

	return;
}

template<typename T>
BOOL CLinkList<T>::InsertAfterNode(Node<T>* pNode,T& element)
{
	if(NULL == this)
	{
		return FALSE;
	}

	//添加pNode合法性验证,20190610,添加简单性验证，后续可逐渐优化
	if(NULL == pNode)
	{
		return FALSE;
	}

	Node<T>* pNewNode = new Node<T>;
	pNewNode->data = element;
	pNewNode->pPrevious = pNode;
	if(NULL == pNode->pNext)
	{
		pNode->pNext = pNewNode;
		pNewNode->pNext = NULL;
		m_pTail = pNewNode;
	}
	else
	{
		Node<T>* pTemp = pNode->pNext;
		pTemp->pPrevious = pNewNode;
		pNewNode->pNext = pTemp;
		pNode->pNext = pNewNode;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteHead();
	}

	return TRUE;
}

template<typename T>
int	CLinkList<T>::InsertAtPre(int nodeId,T& element)
{
	if(NULL == this)
	{
		return -1;
	}
	if(nodeId < 0)
	{
		return -2;
	}
	if(nodeId >= m_size)
	{
		AddTail(element);
		return 0;
	}

	Node<T>* pNode = m_pHead;
	int index = 0;
	while(NULL != pNode && index++ < nodeId)
	{
		pNode = pNode->pNext;
	}
	
	Node<T>* pNewNode = new Node<T>;
	pNewNode->data = element;
	pNewNode->pNext = pNode;
	if(NULL == pNode->pPrevious)
	{
		pNode->pPrevious = pNewNode;
		pNewNode->pPrevious = NULL;
		m_pHead = pNewNode;
	}
	else
	{
		Node<T>* pTemp = pNode->pPrevious;
		pTemp->pNext = pNewNode;
		pNode->pPrevious = pNewNode;
		pNewNode->pPrevious = pTemp;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteTail();
	}

	return 0;
}

template<typename T>
int	CLinkList<T>::InsertAtPre(int nodeId)
{
	if(NULL == this)
	{
		return -1;
	}
	if(nodeId < 0)
	{
		return -2;
	}
	if(nodeId >= m_size)
	{
		AddTail();
		return 0;
	}

	Node<T>* pNode = m_pHead;
	int index = 0;
	while(NULL != pNode && index++ < nodeId)
	{
		pNode = pNode->pNext;
	}

	Node<T>* pNewNode = new Node<T>;
	pNewNode->pNext = pNode;
	if(NULL == pNode->pPrevious)
	{
		pNode->pPrevious = pNewNode;
		pNewNode->pPrevious = NULL;
		m_pHead = pNewNode;
	}
	else
	{
		Node<T>* pTemp = pNode->pPrevious;
		pTemp->pNext = pNewNode;
		pNode->pPrevious = pNewNode;
		pNewNode->pPrevious = pTemp;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteTail();
	}

	return 0;
}

template<typename T>
int	CLinkList<T>::InsertAtAfter(int nodeId,T& element)
{
	if(NULL == this)
	{
		return -1;
	}

	if(nodeId < 0)
	{
		return -2;
	}
	if(nodeId >= m_size)
	{
		AddTail(element);
		return 0;
	}

	Node<T>* pNode = m_pHead;
	int index = 0;
	while(NULL != pNode && index++ < nodeId)
	{
		pNode = pNode->pNext;
	}

	Node<T>* pNewNode = new Node<T>;
	pNewNode->data = element;
	pNewNode->pPrevious = pNode;
	if(NULL == pNode->pNext)
	{
		pNode->pNext = pNewNode;
		pNewNode->pNext = NULL;
		m_pTail = pNewNode;
	}
	else
	{
		Node<T>* pTemp = pNode->pNext;
		pTemp->pPrevious = pNewNode;
		pNode->pNext = pNewNode;
		pNewNode->pNext = pTemp;
	}

	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteHead();
	}

	return 0;
}

template<typename T>
int	CLinkList<T>::InsertAtAfter(int nodeId)
{
	if(NULL == this)
	{
		return -1;
	}

	if(nodeId < 0)
	{
		return -2;
	}
	if(nodeId >= m_size)
	{
		AddTail();
		return 0;
	}

	Node<T>* pNode = m_pHead;
	int index = 0;
	while(NULL != pNode && index++ < nodeId)
	{
		pNode = pNode->pNext;
	}

	Node<T>* pNewNode = new Node<T>;
	pNewNode->pPrevious = pNode;
	if(NULL == pNode->pNext)
	{
		pNode->pNext = pNewNode;
		pNewNode->pNext = NULL;
		m_pTail = pNewNode;
	}
	else
	{
		Node<T>* pTemp = pNode->pNext;
		pTemp->pPrevious = pNewNode;
		pNode->pNext = pNewNode;
		pNewNode->pNext = pTemp;
	}

	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteHead();
	}

	return 0;
}

template<typename T>
void CLinkList<T>::DeleteTail()
{
	if(NULL == this)
	{
		return;
	}

	if(NULL == m_pTail)
	{
		return;
	}
	Node<T>* pTemp = m_pTail->pPrevious;
	delete m_pTail;
	if(NULL == pTemp)
	{
		m_pHead = NULL;
		m_pTail = NULL;
	}
	else
	{
		m_pTail = pTemp;
		m_pTail->pNext = NULL;
	}
	--m_size;
	return;
}

template<typename T>
void CLinkList<T>::DeleteHead()
{
	if(NULL == this)
	{
		return;
	}

	if(NULL == m_pHead)
	{
		return;
	}
	Node<T>* pTemp = m_pHead->pNext;
	delete m_pHead;
	if(NULL == pTemp)
	{
		m_pHead = NULL;
		m_pTail = NULL;
	}
	else
	{
		m_pHead = pTemp;
		m_pHead->pPrevious = NULL;
	}
	--m_size;
	return;
}

template<typename T>
BOOL CLinkList<T>::DeleteNode(Node<T>* pNode)
{
	if(NULL == this)
	{
		return FALSE;
	}

	if(NULL == pNode)
	{
		return FALSE;
	}
	Node<T>* pTemp = GetHead();
	while (NULL != pTemp)
	{
		if(pTemp == pNode)
		{
			Node<T>* pPrevious = pTemp->pPrevious;
			Node<T>* pNext = pTemp->pNext;
			if(NULL == pPrevious)
			{
				DeleteHead();
			}
			else if(NULL == pNext)
			{
				DeleteTail();
			}
			else
			{
				pNext->pPrevious = pPrevious;
				pPrevious->pNext = pNext;
				MyDeletePtr(&pNode);
				--m_size;
			}
			pNode = NULL;
			return TRUE;
		}
		pTemp = pTemp->pNext;
	}
	return FALSE;
}

template<typename T>
BOOL CLinkList<T>::DeleteData(T* pData)
{
	if(NULL == this)
	{
		return FALSE;
	}

	if(NULL == pData)
	{
		return FALSE;
	}

	BOOL retValue = FALSE;
	Node<T>* pTemp = GetHead();
	while (NULL != pTemp)
	{
		if(pTemp->data == *pData)
		{
			Node<T>* pPrevious = pTemp->pPrevious;
			Node<T>* pNext = pTemp->pNext;
			if(NULL == pPrevious)
			{
				DeleteHead();
			}
			else if(NULL == pNext)
			{
				DeleteTail();
			}
			else
			{
				pNext->pPrevious = pPrevious;
				pPrevious->pNext = pNext;
				MyDeletePtr(&pTemp);
				--m_size;
			}
			
			pTemp = pNext;
			retValue = TRUE;
			continue;
		}
		pTemp = pTemp->pNext;
	}
	return retValue;
}

template<typename T>
BOOL CLinkList<T>::DeleteAt(int nodeId)
{
	if(NULL == this)
	{
		return FALSE;
	}

	if(nodeId < 0 || nodeId >= m_size)
	{
		return FALSE;
	}

	if(0 == nodeId)
	{
		DeleteHead();
		return TRUE;
	}
	else if(m_size - 1 == nodeId)
	{
		DeleteTail();
		return TRUE;
	}
	else
	{
		Node<T>* pNode = m_pHead;
		int index = 0;
		while(NULL != pNode && index++ < nodeId)
		{
			pNode = pNode->pNext;
		}

		Node<T>* pPrevious = pNode->pPrevious;
		Node<T>* pNext = pNode->pNext;
		pNext->pPrevious = pPrevious;
		pPrevious->pNext = pNext;
		MyDeletePtr(&pNode);
		--m_size;
		return TRUE;
	}

	return FALSE;
}

template<typename T>
BOOL CLinkList<T>::DataInLink(T* pData)
{
	if(NULL == this)
	{
		return FALSE;
	}

	if(NULL == pData)
	{
		return FALSE;
	}
	Node<T>* pNode = GetHead();
	while(NULL != pNode)
	{
		if(pNode->data == *pData)
		{
			return TRUE;
		}
		pNode = pNode->pNext;
	}
	return FALSE;
}

template<typename T>
BOOL CLinkList<T>::IsEmpty()
{
	if(NULL == this)
	{
		return TRUE;
	}

	return NULL == m_pHead;
}

template<typename T>
void CLinkList<T>::Clear()
{
	if(NULL == this)
	{
		return;
	}
	m_size = 0;
	if(NULL == m_pHead )
	{
		return;
	}
	Node<T>* pTemp = m_pHead;
	Node<T>* pTempNext = NULL;
	while(NULL != pTemp)
	{
		pTempNext = pTemp->pNext;
		delete pTemp;
		pTemp = pTempNext;
	}
	m_pHead = NULL;
	m_pTail = NULL;
}

template<typename T>
Node<T>* CLinkList<T>::GetHead() const
{
	if(NULL == this)
	{
		return NULL;
	}
	return m_pHead;
}

template<typename T>
Node<T>* CLinkList<T>::GetTail()
{
	if(NULL == this)
	{
		return NULL;
	}

	return m_pTail;
}

template<typename T>
int CLinkList<T>::GetSize()
{
	if(NULL == this)
	{
		return -1; 
	}

	return m_size;
}

template<typename T>
Node<T>* CLinkList<T>::GetAt(int index)
{
	if(NULL == this)
	{
		return NULL;
	}

	if(m_size <= 0 || index >= m_size || index < 0)
	{
		return NULL;
	}

	Node<T>* pNode = m_pHead;
	for(int i = 1;i <= index;++i)
	{
		pNode = pNode->pNext;
	}

	return pNode;
}

template<typename T>
BOOL CLinkList<T>::InsertPreNode(Node<T>* pNode,T& element)
{
	if(NULL == this)
	{
		return FALSE;
	}

	//添加pNode合法性验证,20190610,添加简单性验证，后续可逐渐优化
	if(NULL == pNode)
	{
		return FALSE;
	}

	Node<T>* pNewNode = new Node<T>;
	pNewNode->data = element;
	pNewNode->pNext = pNode;
	if(NULL == pNode->pPrevious)
	{
		pNode->pPrevious = pNewNode;
		pNewNode->pPrevious = NULL;
		m_pHead = pNewNode;
	}
	else
	{
		Node<T>* pTemp = pNode->pPrevious;
		pTemp->pNext = pNewNode;
		pNewNode->pPrevious = pTemp;
		pNode->pPrevious = pNewNode;
	}
	++m_size;

	if(m_bLimitMax && m_size > m_maxSize)
	{
		DeleteTail();
	}

	return TRUE;
}

template<typename T>
void CLinkList<T>::operator=(const CLinkList<T>& lnkData)
{
	if(NULL == this)
	{
		return;
	}

	Clear();
	Node<T>* pNode = lnkData.GetHead();
	while (NULL != pNode)
	{
		AddTail(pNode->data);
		pNode = pNode->pNext;
	}
	return;
}

template<typename T>
int CLinkList<T>::operator+(CLinkList<T>& lnkData)
{
	if(NULL == this)
	{
		return -1;
	}

	int iAddSize = lnkData.GetSize();
	if(iAddSize < 1)
	{
		return -2;
	}
	for(int i = 0;i < iAddSize;++i)
	{
		AddTail(lnkData.GetAt(i)->data);
	}

	return 0;
}

template<typename T>
T* CLinkList<T>::operator[] (int index)
{
	if(NULL == this)
	{
		return NULL;
	}

	Node<T>* pNode = GetAt(index);
	if(NULL == pNode)
	{
		return NULL;
	}
	else
	{
		return &(pNode->data);
	}
}

template<typename T>
void CLinkList<T>::SetLimitMaxParam(BOOL bLimitMax,int maxNum)
{
	m_bLimitMax = bLimitMax;
	m_maxSize = maxNum;
	return;
}
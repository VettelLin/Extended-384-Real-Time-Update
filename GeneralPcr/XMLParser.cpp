#include "StdAfx.h"
#include "XMLParser.h"

XMLParser::XMLParser()
{
}

XMLParser::~XMLParser(void)
{
	if(m_Doc != NULL)
	{
		m_Doc.Release();
		m_Doc = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////
/**
 *	取得满足指定结点路径的第一个结点
 *	@param	CString NodeName	--  指向指定结点的路径字符串，格式：
 *									Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
 *	@param	XML_Node& node		--  存放目标结点的变量
 *	
 *	@return	是否成功获取结点
 *	@author	thh		@date	modified 2006-11-29
 */ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetFirstNode(CString NodeName, XML_Node& node)
{
	MSXML2::IXMLDOMNodePtr pNode;
	if(!FindFirstNode(NodeName, pNode))
	{
		return FALSE;
	}
	return NodeToNode(pNode, node);
}

BOOL XMLParser::GetFirstNode(XML_Node node, MSXML2::IXMLDOMElementPtr& pElem)
{
	CString strNode = node.nodeName;
	CString strAttr;
	for(UINT i = 0; i < node.vAttrName.size(); i++)
	{
		strAttr += _T("@") + node.vAttrName.at(i) + _T(" = \"") + node.vAttrValue.at(i) + _T("\" and ");
	}
	if(strAttr.GetLength() > 0)
	{
		strAttr = strAttr.Left(strAttr.GetLength() - (int)strlen(" and "));
		strNode += + _T("[") + strAttr + _T("]");
	}
	
	MSXML2::IXMLDOMNodePtr pNode;
	if(!FindFirstNode(strNode, pNode))
	{
		return FALSE;
	}
	pElem = (MSXML2::IXMLDOMElementPtr)pNode;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
/**
*	取得满足指定结点路径的所有结点
*	@param	CString NodeName				--  指向指定结点的路径字符串，格式：
*												Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param	std::vector<XML_Node>& nodes	--  存放目标结点的变量数组
*	
*	@return	是否成功获取结点
*	@author	thh		@date	modified 2006-11-29
*/ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetNodes(CString NodeName, std::vector<XML_Node>& nodes)
{
	std::vector<MSXML2::IXMLDOMNodePtr> vNodes;
	if(!FindNodes(NodeName, vNodes) || vNodes.size() <= 0)
	{
		return FALSE;
	}
	for(UINT i = 0; i < vNodes.size(); i++)
	{
		XML_Node node;
		if(NodeToNode(vNodes.at(i), node))
		{
			nodes.push_back(node);
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	按指定的Index获取父结点下相应的子结点
*
*	@param CString ParentName	--	指向父结点，格式为：
*									Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param long lIndex			--  要获取的子结点的序号
*	@param XML_Node& ChildNode	--  得到的子结点
*
*	@return	子结点是否获取成功
*	@author	thh		@date	2006-11-29
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetChildNode(CString ParentName, long lIndex, XML_Node& ChildNode)
{
	MSXML2::IXMLDOMNodePtr pParent;
	if(!FindFirstNode(ParentName, pParent) || !pParent->hasChildNodes())
	{
		return FALSE;
	}
	MSXML2::IXMLDOMNodeListPtr pNodeList = pParent->childNodes;
	if(lIndex > pNodeList->Getlength())
	{
		return FALSE;
	}
	return  NodeToNode(pNodeList->Getitem(lIndex), ChildNode);
}

//////////////////////////////////////////////////////////////////////////
/**
*	获取第一个满足条件的父结点下满足路径的所有子结点
*
*	@param CString ParentName					--	指向父结点，格式为：
*													Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param std::vector<XML_Node>& ChildNodes	--  存放找到的子结点
*
*	@return	子结点是否获取成功，若未找到子结点，返回FALSE
*	@author	thh		@date	modified 2006-11-29
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetChildNodes(CString ParentName, std::vector<XML_Node>& ChildNodes)
{
	MSXML2::IXMLDOMNodePtr pParent;
	if(!FindFirstNode(ParentName, pParent) || !pParent->hasChildNodes())
	{
		return FALSE;
	}
	MSXML2::IXMLDOMNodeListPtr pNodeList = pParent->childNodes;
	BOOL bFlag = FALSE;
	for(long i = 0; i < pNodeList->Getlength(); i++)
	{
		XML_Node node;
		if(NodeToNode(pNodeList->Getitem(i), node))
		{
			ChildNodes.push_back(node);
			bFlag = TRUE;
		}
	}
	return bFlag;
}

//////////////////////////////////////////////////////////////////////////
/**
*	取得第一个满足条件的父结点的子结点个数
*	@param	CString ParentName	--  指向父结点的字符串，格式为：
*									Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param	long& nCount		--  要返回的子结点个数
*
*	@return	子结点个数，查找失败则返回-1
*	@author thh		@date	modified 2006-11-29
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetChildCount(CString ParentName, long& nCount)
{
	int count = 0;
	MSXML2::IXMLDOMNodePtr pParent;
	if(!FindFirstNode(ParentName, pParent) || !pParent->hasChildNodes())
	{
		return FALSE;
	}
	nCount = GetElementsCount(pParent->GetchildNodes());
	return TRUE;
}

BOOL XMLParser::GetNodeCount(CString strNode, long& nCount)
{
	if (m_Doc == NULL)
	{
		return FALSE;
	}
	if (strNode.GetLength() <= 0)	// Length of the string is zero or it only contains one node name.
	{
		nCount = 1;
		return TRUE;
	}
	MSXML2::IXMLDOMNodePtr node = (MSXML2::IXMLDOMNodePtr)m_Doc->GetdocumentElement();
	MSXML2::IXMLDOMNodeListPtr nodeList = node->selectNodes((_bstr_t)strNode);
	nCount = (long)GetElementsCount(nodeList);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	取得当前DOM的所有内容
*	@param	CString& strXML		--  当前DOM所有内容的字符串
*
*	@return	是否获取成功
*	@author thh		@date	modified 2006-12-01
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetXMLString(CString& strXML)
{
	if(m_Doc == NULL)
		return FALSE;
	LPWSTR strW;
	if(SUCCEEDED(m_Doc->get_xml(&strW)))
	{
		strXML = (CString)strW;
		return TRUE;
	}
	return FALSE;
}

BOOL XMLParser::GetXMLString(BSTR& strXML)
{
	if(m_Doc == NULL)
		return FALSE;

	if(SUCCEEDED(m_Doc->get_xml(&strXML)))
		return TRUE;
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////
/**
*	根据给定的IXMLDomNodePtr填充XML_Node结构
*	@param	MSXML2::IXMLDOMNodePtr pNode	--  源Node
*	@param	XML_Node& node					--  目标Node
*	
*	@return	是否转化成功
*	@author	thh		@date	modified 2006-11-29
*/ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::NodeToNode(MSXML2::IXMLDOMNodePtr pNode, XML_Node& node)
{
	node.Clear();

	CComVariant varVal(VT_EMPTY);
	BSTR strName;
	if( SUCCEEDED(pNode->get_nodeTypedValue(&varVal)) && SUCCEEDED(pNode->get_nodeName(&strName)) )
	{
		node.nodeName = (CString)strName;
		node.nodeValue = (LPCTSTR)(_bstr_t)varVal;
		MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = pNode->Getattributes();
		if(pNodeMap == NULL)
			return TRUE;
		for(long i = 0; i < pNodeMap->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pAttr;
			if( SUCCEEDED(pNodeMap->get_item(i, &pAttr)) )
			{
				node.vAttrName.push_back((LPCTSTR)pAttr->GetnodeName());
				node.vAttrValue.push_back((LPCTSTR)(_bstr_t)pAttr->GetnodeTypedValue());
			}
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
/**
*	根据结点字符串获得满足条件的第一个结点
*
*	@param	name	--  结点字符串，格式：Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param  pNode	--	存放找到的第一个结点
*
*	@return	是否寻找成功
*	@author	thh		@date	modified 2006-11-30
*/ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::FindFirstNode(CString name, MSXML2::IXMLDOMNodePtr& pNode)
{
	if (m_Doc == NULL)
	{
		return FALSE;
	}
	pNode = (MSXML2::IXMLDOMNodePtr)m_Doc->GetdocumentElement();
	if (pNode == NULL)
		return FALSE;
	
	if (name.GetLength() <= 0)	// Length of the string is zero or it only contains one node name.
		return TRUE;
	
	std::vector<MSXML2::IXMLDOMNodePtr> vNodes;
	if(!FindChildNodes(pNode, name, vNodes, TRUE))
		return FALSE;
	pNode = vNodes.at(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
/**
*	根据结点字符串获得对应结点集
*
*	@param	CString name								--  结点字符串：Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param	std::vector<MSXML2::IXMLDOMNodePtr>& vNodes	--	存放找到的节点集合
*
*	@return	是否获取成功，若未找到对应的结点，也返回FALSE
*	@author	thh		@date	modified 2006-11-30
*/ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::FindNodes(CString name, std::vector<MSXML2::IXMLDOMNodePtr>& vNodes)
{
	if (m_Doc == NULL)
	{
		return FALSE;
	}
	MSXML2::IXMLDOMNodePtr node = (MSXML2::IXMLDOMNodePtr)m_Doc->GetdocumentElement();
	if (node == NULL)
	{
		return FALSE;
	}
	if (name.GetLength() <= 0)	// Length of the string is zero or it only contains one node name.
	{
		vNodes.push_back(node);
		return TRUE;
	}
	return FindChildNodes(node, name, vNodes);
}

/////////////////////////////////////////////////////////////////////////
/**
*	在指定父结点下查找符合路径描述的子结点
*
*	@param	pParentNode		--  父结点指针
*	@param	strChildNode	--  子结点的路径描述，格式：Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param	vNodes			--	存放查找到的子结点指针
*
*	@return	是否查找成功，若未找到符合条件的子结点,返回FALSE
*	@author	thh		@date	modified 2006-11-29
*/ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::GetFirstChild(MSXML2::IXMLDOMNodePtr pParentNode, CString strChildNode, MSXML2::IXMLDOMElementPtr& pChild)
{
	if(!pParentNode->hasChildNodes())
		return FALSE;

	std::vector<MSXML2::IXMLDOMNodePtr> vNodes;
	if(FindChildNodes(pParentNode, strChildNode, vNodes, TRUE) && vNodes.size() >= 1)
	{	
		pChild = vNodes.at(0);
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
/**
*	在指定父结点下查找符合路径描述的子结点
*
*	@param	pParentNode		--  父结点指针
*	@param	strChildNode	--  子结点的路径描述，格式：Node1/Node2/NodeName[@attName1 = attValue1 and @attName2 = attValue2]/Node4
*	@param	vNodes			--	存放查找到的子结点指针
*
*	@return	是否查找成功，若未找到符合条件的子结点,返回FALSE
*	@author	thh		@date	modified 2006-11-29
*/ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::FindChildNodes(MSXML2::IXMLDOMNodePtr pParentNode, CString strChildNode, std::vector<MSXML2::IXMLDOMNodePtr>& vNodes, BOOL bOnlyFirst)
{
	if(pParentNode == NULL || strChildNode.GetLength() <= 0)
		return FALSE;

	BOOL bFlag = FALSE;
	MSXML2::IXMLDOMNodeListPtr nodeList = pParentNode->selectNodes((_bstr_t)strChildNode);
	for(long i = 0; i < nodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode;
		if(SUCCEEDED(nodeList->get_item(i, &pNode)))
		{
			vNodes.push_back(pNode);
			if(bOnlyFirst)
				return TRUE;
			bFlag = TRUE;
		}
	}
	return bFlag;
}


/////////////////////////////////////////////////////////////////////////
/**
*	在给定的结点集中统计指定类型结点的个数
*	@param	MSXML2::IXMLDOMNodeListPtr nodes	--	给定的结点集
*	@param	int nType							--	指定的结点类型，主要包含：
*													NODE_ELEMENT NODE_ATTRIBUTE NODE_TEXT NODE_COMMENT等
*
*	@return	满足类型的结点个数
*	@author	thh		@date	modified 2006-11-29
*/ 
/////////////////////////////////////////////////////////////////////////
int XMLParser::GetElementsCount(MSXML2::IXMLDOMNodeListPtr nodes, int nType)
{
	int count = 0;
	for (long i = 0; i < nodes->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode;
		if (SUCCEEDED(nodes->get_item(i, &pNode)) && pNode->GetnodeType() == nType)
		{
			count++;
		}
	}
	return count;
}

/////////////////////////////////////////////////////////////////////////
/**
 *	解析带属性的结点字符串，字符串格式：NodeName(AttName1=AttValue1,AttName2=AttValue2,.....]:NodeValue
 *							其中NodeName项是必须的，其它项可以没有
 *	@param	name	--  结点的字符串
 *	@param	node	--  存放解析后字符串的数据结构
 *
 *	@author	thh		@date	modified 2006-11-29
 */ 
/////////////////////////////////////////////////////////////////////////
void XMLParser::ParseSingleNode(CString name, XML_Node& node)
{
	if(name.GetLength() == 0)
		return;

	int nValueIndex = name.Find(':');
	if(nValueIndex != -1)
		node.nodeValue = name.Right(name.GetLength() - nValueIndex - 1);

	int nLIndex = name.Find('(');
	int nRIndex = name.Find(')');
	if(nLIndex != -1 && nRIndex != -1)
	{
		node.nodeName = name.Left(nLIndex);
		CString str;
		std::vector<CString> vStr;
		str = name.Mid(nLIndex+1, nRIndex-nLIndex-1);
		CGeneralOperator::ExtractSubString(str, _T(',') , vStr);
		for(UINT i = 0; i < vStr.size(); i++)
		{
			int nAttIndex = vStr.at(i).Find('=');
			if(nAttIndex != -1)
			{
				node.vAttrName.push_back((vStr.at(i).Left(nAttIndex)).Trim());
				node.vAttrValue.push_back((vStr.at(i).Right(vStr.at(i).GetLength() - nAttIndex - 1)).Trim());				
			}
		}
	}
	else
	{
		node.nodeName = ((nValueIndex == -1) ? name : name.Left(nValueIndex));
	}
}

/////////////////////////////////////////////////////////////////////////
/**
 *	验证当前系统是否可以应用xml相关操作
 *
 *	@return	当前系统是否可以应用xml相关操作
 *	@author	thh		@date	marked 2006-07-25
 */ 
/////////////////////////////////////////////////////////////////////////
BOOL XMLParser::XMLValidate()
{
	if( FAILED(CoInitialize(NULL)) || FAILED(m_Doc.CreateInstance("msxml2.domdocument")) )
		return FALSE;

	if(m_Doc != NULL)
	{
		m_Doc.Release();
		m_Doc = NULL;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	初始化 XML com对象
*
*	@return 初始化是否成功
*	@author	thh		@date	2006-07-06
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::InitialXMLCom()
{
	if(m_Doc == NULL)
	{
		if( FAILED(CoInitialize(NULL)) || FAILED(m_Doc.CreateInstance("msxml2.domdocument")))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	载入XML，通过XML格式的字符串
*
*	@param CString strXML	--	XML格式的字符串
*	@return 载入是否成功
*	@author	thh		@date	2006-07-06
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::LoadXML(CString strXML)
{
	if( !InitialXMLCom() || !m_Doc->loadXML(_bstr_t(strXML)) )
		return FALSE;
	return TRUE;
}

BOOL XMLParser::LoadXML(const BSTR& bsXML)
{
	if( !InitialXMLCom() || !m_Doc->loadXML(bsXML) )
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	载入XML，通过指定的XML文件路径
*
*	@param CString strXML	--	XML文件路径
*	@return 载入是否成功
*	@author	thh		@date	2006-07-06
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::Load(CString strXML)
{
	if( !InitialXMLCom() || !m_Doc->load(_variant_t(strXML)) )
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	在指定父结点下插入新结点
*
*	@param CString strParent	--	指向父结点的字符串
*	@param CString strNewNode	--	表示新的子结点的字符串，格式为：
*									NodeName(attName=attValue):NodeContent
*
*	@return	子结点是否成功插入指定位置
*	@author	thh		@date	modified 2006-11-30
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::InsertItem(CString strParent, XML_Node NewNode)//thh refactory
{	
	MSXML2::IXMLDOMNodePtr pNode;
	if(!FindFirstNode(strParent, pNode))
		return FALSE;

	MSXML2::IXMLDOMElementPtr pElement;
	if(!CreateElement(NewNode, pElement))
		return FALSE;

	pNode->appendChild(pElement);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	在指定父结点下插入新结点
*
*	@param CString strParent					--	指向父结点的字符串
*	@param MSXML2::IXMLDOMElementPtr pElement	--	子结点指针
*
*	@return	子结点是否成功插入指定位置
*	@author	thh		@date	2006-12-01
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::InsertItem(CString strParent, MSXML2::IXMLDOMElementPtr pElement)
{
	MSXML2::IXMLDOMNodePtr pNode;
	if(!FindFirstNode(strParent, pNode))
		return FALSE;
	
	if(pElement == NULL)
		return FALSE;

	pNode->appendChild(pElement);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	创建新结点
*
*	@param XML_Node node						--	新结点的数据结构
*	@param MSXML2::IXMLDOMElementPtr& pElement	--	指向新创建的结点
*
*	@return	结点是否创建成功
*	@author	thh		@date	2006-12-01
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::CreateElement(XML_Node node, MSXML2::IXMLDOMElementPtr& pElement)
{
	if(m_Doc == NULL || node.nodeName.GetLength() <= 0 || node.vAttrName.size() != node.vAttrValue.size())
		return FALSE;

	pElement = m_Doc->createElement((_bstr_t)node.nodeName);
	if(pElement == NULL)
		return FALSE;

	if(node.nodeValue.GetLength() > 0 && FAILED(pElement->put_text((_bstr_t)node.nodeValue)))
		return FALSE;

	for(UINT i = 0; i < node.vAttrName.size(); i++)
	{
		if(FAILED(pElement->setAttribute((_bstr_t)node.vAttrName.at(i), (_bstr_t)node.vAttrValue.at(i))))
			return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
*	创建新结点
*
*	@param CString strNode						--	新结点的字符串表达式，格式为：
*													NodeName(attName=attValue):NodeValue
*	@param MSXML2::IXMLDOMElementPtr& pElement	--	指向新创建的结点
*
*	@return	结点是否创建成功
*	@author	thh		@date	2006-12-01
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::CreateElement(CString strNode, MSXML2::IXMLDOMElementPtr& pElement)
{
	XML_Node node;
	ParseSingleNode(strNode, node);
	return CreateElement(node, pElement);
}

/////////////////////////////////////////////////////////////////////////
/**
*	删除指定结点
*
*	@param CString strNode	--	指向欲删除的结点，格式为：
*								Node1.Node2.NodeName(attName=attValue).Node4
*
*	@return	结点是否成功删除
*	@author	thh		@date	2006-07-06
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::DeleteItem(CString strNode)
{
	std::vector<MSXML2::IXMLDOMNodePtr> vNodes;
	if(!FindNodes(strNode, vNodes))
		return FALSE;

	BOOL bFlag =TRUE;
	for(UINT i = 0; i < vNodes.size(); i++)
	{
		MSXML2::IXMLDOMNodePtr pParent = NULL;
		if(SUCCEEDED(vNodes.at(i)->get_parentNode(&pParent)))
		{
			pParent->removeChild(vNodes.at(i));
		}
		else
			bFlag = FALSE;
	}
	return bFlag;
}

/////////////////////////////////////////////////////////////////////////
/**
*	保存当前内容到指定文件中
*
*	@param CString strFile	--	指定的文件路径
*
*	@return	是否保存成功
*	@author	thh		@date	2006-12-04
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::SaveXMLFile(CString strFile)
{
	if(m_Doc == NULL || strFile.GetLength() <= 0)
		return FALSE;

	int nPos = strFile.ReverseFind('\\');
	
	CFileFind ff;
	CString str = strFile.Left(nPos);
	if(ff.FindFile(strFile.Left(nPos)))
	{
		_variant_t varXml((LPCTSTR)strFile);
		return SUCCEEDED(m_Doc->save(varXml));		
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
/**
*	为当前XML增加StyleSheet
*
*	@param CString strType	--	StyleSheet类型：xml-stylesheet
*	@param CString strData	--	StyleSheet内容：type="text/xsl" href="DeafReport.xslt"
*
*	@return	是否添加成功
*	@author	thh		@date	2006-12-12
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::AddProcessingInstruction(CString strType, CString strData)
{
	MSXML2::IXMLDOMProcessingInstruction* pProcIns;
	BOOL bFlag = FALSE;
	if(SUCCEEDED(m_Doc->raw_createProcessingInstruction(_bstr_t(strType), _bstr_t(strData), &pProcIns)) && pProcIns)	
	{
		m_Doc->appendChild(pProcIns);
		bFlag = TRUE;
	}
	pProcIns->Release();
	pProcIns = NULL;
	return bFlag;
}

BOOL XMLParser::ModifyItemValue(CString strNode, CString strValue)
{
	MSXML2::IXMLDOMNodePtr pNode;
	if(!FindFirstNode(strNode, pNode))
		return FALSE;

	return SUCCEEDED(pNode->put_text((_bstr_t)strValue));
}

BOOL XMLParser::ModifyItemAttrValue(CString strNode, CString strAttrName, CString strAttrValue)
{
	MSXML2::IXMLDOMNodePtr pNode;
	if(!FindFirstNode(strNode, pNode))
		return FALSE;
	MSXML2::IXMLDOMElementPtr pElem = (MSXML2::IXMLDOMElementPtr)pNode;
	if(FAILED(pElem->setAttribute((_bstr_t)strAttrName, (_bstr_t)strAttrValue)))
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
/**
*	将当前XML通过指定的XSLT文件转换为HTML字符串输出
*
*	@param CString strXSLTFile	--	XSLT文件路径
*	@param BSTR& strOut			--	转换后的HTML字符串
*
*	@return	是否转换成功
*	@author	thh		@date	2007-03-02
*/
//////////////////////////////////////////////////////////////////////////
BOOL XMLParser::TransformToHTML(IN CString strXSLTFile, OUT _bstr_t& strOut)
{
	if(m_Doc == NULL)
		return FALSE;

	MSXML2::IXMLDOMDocumentPtr XSLTDoc;
	if(FAILED(XSLTDoc.CreateInstance("msxml2.domdocument")) || !XSLTDoc->load(_variant_t(strXSLTFile)))
		return FALSE;

	strOut = m_Doc->transformNode(XSLTDoc);

	return TRUE;
}
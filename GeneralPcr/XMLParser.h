#pragma once

#include <vector>
#import "msxml3.dll"
#include "GeneralOperator.h"

struct XML_Node
{
	CString nodeName;
	CString nodeValue;
	std::vector<CString> vAttrName;
	std::vector<CString> vAttrValue;
	
	XML_Node()
	{
		nodeName = "";
		nodeValue = "";
	}

	BOOL GetAttrValue(CString strName, CString& strValue)
	{
		for(UINT i = 0; i < vAttrName.size(); i++)
		{
			if(!vAttrName.at(i).CompareNoCase(strName))
			{
				strValue = vAttrValue.at(i);
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL CompareAttrName(const XML_Node& node)
	{
		if(vAttrName.size() != node.vAttrName.size())
			return FALSE;
		for(UINT i = 0; i < vAttrName.size(); i++)
		{
			if(vAttrName.at(i).CompareNoCase(node.vAttrName.at(i)))
				return FALSE;
		}
		return TRUE;
	}
	BOOL CompareAttrValue(const XML_Node& node)
	{
		if(vAttrValue.size() != node.vAttrValue.size())
			return FALSE;
		for(UINT i = 0; i < vAttrValue.size(); i++)
		{
			if(vAttrValue.at(i).CompareNoCase(node.vAttrValue.at(i)))
				return FALSE;
		}
		return TRUE;
	}
	BOOL CompareOneOfTheAttr(const XML_Node& node)
	{
		for(UINT i = 0; i < node.vAttrName.size(); i++)
		{
			CString strName;
			if(!GetAttrValue(node.vAttrName.at(i), strName)
				&& node.vAttrValue.at(i).CompareNoCase(strName))
				return FALSE;			
		}
		return TRUE;
	}
	BOOL operator == (const XML_Node& node)
	{
		if(nodeName.CompareNoCase(node.nodeName)
			|| nodeValue.CompareNoCase(node.nodeValue)
			|| vAttrName.size() != node.vAttrName.size()
			|| vAttrValue.size() != node.vAttrValue.size()
			|| node.vAttrName.size() != node.vAttrValue.size())
		{
			return FALSE;
		}
		for(UINT i = 0; i < vAttrName.size(); i++)
		{
			if(vAttrName.at(i).CompareNoCase(node.vAttrName.at(i))
				|| vAttrValue.at(i).CompareNoCase(node.vAttrValue.at(i)))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	void Clear()
	{
		nodeName = "";
		nodeValue = "";
		vAttrName.clear();
		vAttrValue.clear();
	}

};

class XMLParser
{
public:
	XMLParser();
	virtual ~XMLParser(void);
	
	BOOL GetFirstNode(CString NodeName, XML_Node& node);
	BOOL GetFirstNode(XML_Node node, MSXML2::IXMLDOMElementPtr& pNode);
	BOOL GetNodes(CString NodeName, std::vector<XML_Node>& nodes);
	BOOL GetNodeCount(CString strNode, long& nCount);
	BOOL GetChildNode(CString ParentName, long lIndex, XML_Node& ChildNode);
	BOOL GetChildNodes(CString ParentName, std::vector<XML_Node>& ChildNodes);
	BOOL GetChildCount(CString ParentName, long& lCount);
	BOOL GetXMLString(CString& strXML);
	BOOL GetXMLString(BSTR& strXML);
	BOOL GetFirstChild(MSXML2::IXMLDOMNodePtr pParentNode, CString strChildNode, MSXML2::IXMLDOMElementPtr& pChild);

	BOOL InsertItem(CString strParent, XML_Node NewNode);
	BOOL InsertItem(CString strParent, MSXML2::IXMLDOMElementPtr pElement);
	BOOL CreateElement(XML_Node node, MSXML2::IXMLDOMElementPtr& pElement);
	BOOL CreateElement(CString strNode, MSXML2::IXMLDOMElementPtr& pElement);
	BOOL DeleteItem(CString strNode);
	BOOL ModifyItemValue(CString strNode, CString strValue);
	BOOL ModifyItemAttrValue(CString strNode, CString strAttrName, CString strAttrValue);

	BOOL XMLValidate();
	// 以XML格式字符串方式载入xml
	BOOL LoadXML(CString strXML);
	BOOL LoadXML(const BSTR& bsXML);
	// 以文件路径方式载入xml
	BOOL Load(CString strXMLFile);
	// 将xml保存到给定文件路径中
	BOOL SaveXMLFile(CString strFile);
	// 为xml增加StyleSheet 或 DTD
	BOOL AddProcessingInstruction(CString strType, CString strData);
	//
	BOOL TransformToHTML(IN CString strXSLTFile, OUT _bstr_t& strOut);

protected:
	BOOL InitialXMLCom();//XML Com对象初始化

	BOOL FindFirstNode(CString name, MSXML2::IXMLDOMNodePtr& pNode);
	BOOL FindNodes(CString name, std::vector<MSXML2::IXMLDOMNodePtr>& vNodes);
	void ParseSingleNode(CString name, XML_Node& node);
	BOOL NodeToNode(MSXML2::IXMLDOMNodePtr pNode, XML_Node& node);
	BOOL FindChildNodes(MSXML2::IXMLDOMNodePtr pParentNode, CString strChildNode, std::vector<MSXML2::IXMLDOMNodePtr>& vNodes, BOOL bOnlyFirst = FALSE);
	int  GetElementsCount(MSXML2::IXMLDOMNodeListPtr nodes, int nType = MSXML2::NODE_ELEMENT);


protected:
	MSXML2::IXMLDOMDocumentPtr m_Doc; // XML Document
};

#pragma once

#import <msxml3.dll>
#include <string>

class CXMLHelper
{
public:
	CXMLHelper();
	~CXMLHelper();
	static IXMLDOMNodeList* GetXMLNodes(IXMLDOMNode* pNode, const char* pszExpression);
	static IXMLDOMNodeList* GetXMLChildNodes(IXMLDOMNode* pNode, const char* pszExpression);
	static IXMLDOMNode* GetXMLNode(IXMLDOMNode* pNode, const char* pszExpression);
	static void GetXMLText(IXMLDOMNode* pNode, const char* pszExpression, BSTR* pbstr);
	static std::string GetXMLString(IXMLDOMNode* pNode, const char* pszExpression, const char* pszDefault);
	static int GetXMLInt(IXMLDOMNode* pNode, const char* pszExpression, int nDefault);
	static double GetXMLR8(IXMLDOMNode* pNode, const char* pszExpression, double dDefault);
	static IXMLDOMDocument* Load(const char* pszFile);
};


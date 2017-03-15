#include "stdafx.h"
#include "XMLHelper.h"
#include <atlbase.h>

CXMLHelper::CXMLHelper()
{
}


CXMLHelper::~CXMLHelper()
{
}
IXMLDOMNodeList* CXMLHelper::GetXMLNodes(IXMLDOMNode* pNode, const char* pszExpression)
{
	CComBSTR bstrExpression(pszExpression);
	CComPtr<IXMLDOMNodeList> spID;
	pNode->selectNodes(bstrExpression, &spID);
	return spID.Detach();
}
IXMLDOMNodeList* CXMLHelper::GetXMLChildNodes(IXMLDOMNode* pNode, const char* pszExpression)
{
	CComBSTR bstrExpression(pszExpression);
	CComPtr<IXMLDOMNode> spNode = GetXMLNode(pNode, pszExpression);
	CComPtr<IXMLDOMNodeList> spID;
	spNode->get_childNodes(&spID);
	return spID.Detach();
}
IXMLDOMNode* CXMLHelper::GetXMLNode(IXMLDOMNode* pNode, const char* pszExpression)
{
	CComBSTR bstrExpression(pszExpression);
	CComPtr<IXMLDOMNode> spID;
	pNode->selectSingleNode(bstrExpression, &spID);
	return spID.Detach();
}
void CXMLHelper::GetXMLText(IXMLDOMNode* pNode, const char* pszExpression, BSTR* pbstr)
{
	*pbstr = NULL;
	CComBSTR bstrExpression(pszExpression);
	CComPtr<IXMLDOMNode> spID;
	pNode->selectSingleNode(bstrExpression, &spID);
	if (spID.p)
	{
		spID->get_text(pbstr);
	}
}
std::string CXMLHelper::GetXMLString(IXMLDOMNode* pNode, const char* pszExpression, const char* pszDefault)
{
	CComBSTR bstr;
	GetXMLText(pNode, pszExpression, &bstr);
	std::string str = "";
	if (bstr.Length() > 0)
	{
		CW2A c = bstr;
		str = c;
	}
	else
	{
		str = pszDefault;
	}
	return str;
}
int CXMLHelper::GetXMLInt(IXMLDOMNode* pNode, const char* pszExpression, int nDefault)
{
	CComBSTR bstr;
	GetXMLText(pNode, pszExpression, &bstr);
	if (bstr.Length() > 0)
	{
		return _wtoi(bstr);
	}
	return nDefault;
}
double CXMLHelper::GetXMLR8(IXMLDOMNode* pNode, const char* pszExpression, double dDefault)
{
	CComBSTR bstr;
	GetXMLText(pNode, pszExpression, &bstr);
	if (bstr.Length() > 0)
	{
		return _wtof(bstr);
	}
	return dDefault;
}
IXMLDOMDocument* CXMLHelper::Load(const char* pszFile)
{
	VARIANT_BOOL bSucceeded;
	CComBSTR bstrParams(pszFile);
	VARIANT varDef;
	varDef.vt = VT_BSTR;
	varDef.bstrVal = bstrParams;

	CComPtr<IXMLDOMDocument> spXMLDocument;
	spXMLDocument.CoCreateInstance(OLESTR("Msxml2.DOMDocument.6.0"));
	spXMLDocument->put_async(VARIANT_FALSE);
	spXMLDocument->load(varDef, &bSucceeded);
	if (!bSucceeded)
	{
		//
		// If the load failed report the error
		//
		CComPtr<IXMLDOMParseError> spError;
		spXMLDocument->get_parseError(&spError);
		if (spError)
		{
			CComBSTR bstrDesc;
			spError->get_reason(&bstrDesc);
			CW2A a(bstrDesc);
			std::string str(a);
			throw str.c_str();
		}
	}
	return spXMLDocument.Detach();
}

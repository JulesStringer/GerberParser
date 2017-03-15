#pragma once

#include <string>
#import <msxml3.dll>
#include <atlbase.h>

class CGerberParser;
class CDrillsParser;
class CGerberRender;
class CLayer;

typedef struct tagDEFAULTS
{
	double dDefInflate;
	double dDefDeflate;
	double dDefMinsize;
	double dCutlineInflate;
}DEFAULTS;
class CFolder
{
	int m_nID;
	std::string m_strPath;
	CComPtr<IXMLDOMNode> m_spPartsNode;
	void RenderGraphics(CGerberRender* pRender, IXMLDOMNode* spGraphics, CLayer* pLayer = NULL);
public:
	CFolder();
	CFolder(int nID, const char* pszPath, IXMLDOMNode* pPartsNode);
	virtual ~CFolder();
	void Process(CGerberRender* pRender, IXMLDOMNode* pDrawing, DEFAULTS* pDefaults);
};


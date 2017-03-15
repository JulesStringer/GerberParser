#pragma once

class CGerberRender;
class CLayer;
#import "msxml3.dll"

class CGraphic
{
public:
	CGraphic();
	virtual ~CGraphic();
	static CGraphic* Create(const wchar_t* pszName);
	virtual void Parse(IXMLDOMNode* pNode);
	virtual void Render(CGerberRender* pRender, CLayer* pLayer);
};


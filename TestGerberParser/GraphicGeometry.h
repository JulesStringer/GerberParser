#pragma once
#include "Graphic.h"

#include <string>
#include "..\GerberParser\WKBGeometry.h"

class CGraphicGeometry :
	public CGraphic
{
	std::string m_strLayer;
	CWKBGeometry* m_pGeometry;
public:
	CGraphicGeometry();
	virtual ~CGraphicGeometry();
	virtual void Parse(IXMLDOMNode* pNode);
	virtual void Render(CGerberRender* pRender, CLayer* pLayer);
};


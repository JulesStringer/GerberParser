#pragma once
#include "Graphic.h"

#include <string>

class CHole :
	public CGraphic
{
	double m_X;
	double m_Y;
	double m_dDiameter;
	std::string m_strLayer;
public:
	CHole();
	virtual ~CHole();
	virtual void Parse(IXMLDOMNode* pNode);
	virtual void Render(CGerberRender* pRender, CLayer* pLayer);
};


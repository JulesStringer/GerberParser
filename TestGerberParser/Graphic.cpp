#include "stdafx.h"
#include "Graphic.h"
#include "Hole.h"
#include "GraphicGeometry.h"

CGraphic::CGraphic()
{
}


CGraphic::~CGraphic()
{
}
CGraphic* CGraphic::Create(const wchar_t* pszName)
{
	if (wcscmp(pszName, L"hole") == 0)
	{
		return new CHole();
	}
	else if (wcscmp(pszName, L"geometry") == 0)
	{
		return new CGraphicGeometry();
	}
	return NULL;
}
void CGraphic::Render(CGerberRender* pRender, CLayer* pLayer)
{
	throw "CGraphic::Render must be implemented in derived class";
}
void CGraphic::Parse(IXMLDOMNode* pNode)
{
	throw "CGraphic::Parse Parse must be implemented in derived class";
}

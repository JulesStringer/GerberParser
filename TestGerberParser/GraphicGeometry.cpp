#include "stdafx.h"
#include "GraphicGeometry.h"
#include "..\GerberParser\GerberRender.h"
#include "..\GerberParser\GerberState.h"
#include "XMLHelper.h"

CGraphicGeometry::CGraphicGeometry()
	: m_pGeometry(NULL)
{
}
CGraphicGeometry::~CGraphicGeometry()
{
}
void CGraphicGeometry::Parse(IXMLDOMNode* pNode)
{
	m_strLayer = CXMLHelper::GetXMLString(pNode, "layer", "graphics");
	std::string strGeometry = CXMLHelper::GetXMLString(pNode, "geometry", "");
	m_pGeometry = new CWKBGeometry();
	m_pGeometry->Parse(strGeometry.c_str());
}
void CGraphicGeometry::Render(CGerberRender* pRender, CLayer* pLayer)
{
	if (pLayer == NULL)
	{
		pLayer = pRender->GetLayer(m_strLayer.c_str());
	}
	CGerberState State;
	State.SetLayer(pLayer);
	pRender->Geometry(m_pGeometry, &State);
}

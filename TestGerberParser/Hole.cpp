#include "stdafx.h"
#include "Hole.h"
#include "..\GerberParser\GerberState.h"

#include "..\GerberParser\GerberRender.h"
#include "XMLHelper.h"

CHole::CHole()
	: m_X(0)
    , m_Y(0)
	, m_dDiameter(0)
{
}
CHole::~CHole()
{
}
void CHole::Parse(IXMLDOMNode* pNode)
{
	m_X = CXMLHelper::GetXMLR8(pNode, "X", 0.0);
	m_Y = CXMLHelper::GetXMLR8(pNode, "Y", 0.0);
	m_dDiameter = CXMLHelper::GetXMLR8(pNode, "diameter", 0.0);
	m_strLayer = CXMLHelper::GetXMLString(pNode, "layer", "Mounting_Holes");
}
void CHole::Render(CGerberRender* pRender, CLayer* pLayer)
{
	if (pLayer == NULL)
	{
		pLayer = pRender->GetLayer(m_strLayer.c_str());
	}
	CGerberState State;
	State.SetLayer(pLayer);
	pRender->Circle(m_X, m_Y, m_dDiameter / 2, &State);
}

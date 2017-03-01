#include "GerberPrimitive.h"
#include "GerberPrimitiveCircle.h"
#include "GerberPrimitiveCenterLine.h"

CGerberPrimitive::CGerberPrimitive()
{
}
CGerberPrimitive::~CGerberPrimitive()
{
}
void CGerberPrimitive::AddModifier(CExpression* pExpression)
{
	m_vecModifiers.push_back(pExpression);
}
CGerberPrimitive* CGerberPrimitive::Create(int nPrimitive)
{
	switch (nPrimitive)
	{
	case 1:
		// circle
		return new CGerberPrimitiveCircle();
#if 0
	case 2:
		// vector line
		return new CGerberPrimitiveVectorLine();
#endif
	case 21:
		// CenterLine
		return new CGerberPrimitiveCenterLine();
#if 0
	case 4:
		// Outline
		return new CGerberPrimitiveOutline();
	case 5:
		// Polygon
		return new CGerberPrimitivePolygon();
	case 6:
		// Moire
		return new CGerberPrimitiveMoire();
	case 7:
		// Thermal
		return new CGerberPrimitiveThermal();
#endif
	default:
		throw "CGerberPrimitive::Create primitive not yet implemented";
	}
}
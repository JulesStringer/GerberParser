#include "GerberAperatureRectangle.h"
#include "WKBGeometry.h"
#include "GerberRender.h"
#include "Part.h"
#include "Rectangle.h"

CGerberAperatureRectangle::CGerberAperatureRectangle()
{
}
CGerberAperatureRectangle::~CGerberAperatureRectangle()
{
}
void CGerberAperatureRectangle::Render(CGerberRender* pRender, double X, double Y, CGerberState* pState, double dDeflate)
{
	double XSize = m_vecArgs[0] - dDeflate * 2;
	double YSize = m_vecArgs[1] - dDeflate * 2;
	double dHoleSize = 0;
	if (m_vecArgs.size() > 2)
	{
		dHoleSize = m_vecArgs[2];
	}
	if ( dHoleSize == 0)
	{
		CRectangle rc(X - XSize / 2, Y - YSize / 2, X + XSize / 2, Y + YSize / 2);
		pRender->Rectangle(&rc, pState);
	}
	else
	{
		throw "Rectangle with hole not yet supported";
	}
}
void CGerberAperatureRectangle::Inflate(CPart* pPart, double X, double Y)
{
	double XSize = m_vecArgs[0];
	double YSize = m_vecArgs[1];
	pPart->Inflate(X - XSize / 2, Y - YSize / 2);
	pPart->Inflate(X + XSize / 2, Y + YSize / 2);
}
void CGerberAperatureRectangle::Inflate(CRectangle* pRect, double X, double Y)
{
	double XSize = m_vecArgs[0];
	double YSize = m_vecArgs[1];
	pRect->Inflate(X - XSize / 2, Y - YSize / 2);
	pRect->Inflate(X + XSize / 2, Y + YSize / 2);
}

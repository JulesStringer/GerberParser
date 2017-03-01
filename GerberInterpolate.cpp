#include "GerberInterpolate.h"
#include "GerberRender.h"
#include "Part.h"
#include "Rectangle.h"

CGerberInterpolate::CGerberInterpolate()
	: m_X(0)
	, m_Y(0)
	, m_I(0)
	, m_J(0)
{
}
CGerberInterpolate::~CGerberInterpolate()
{
}
CGerberInterpolate::CGerberInterpolate(double X, double Y, double I, double J, CGerberState* pState)
	: CGerberCommand(pState)
	, m_X(X)
	, m_Y(Y)
	, m_I(I)
	, m_J(J)
{

}
void CGerberInterpolate::Render(CGerberRender* pRender, double dDeflate)
{
	switch (m_pState->GetInterpolation())
	{
	case INTERPOLATION_LINEAR:
		pRender->LineTo(m_X, m_Y, m_pState);
		break;
	case INTERPOLATION_CLOCKWISE:
	case INTERPOLATION_ANTICLOCK:
		throw "CGerberInterpolate::Render non-linear interpolation not yet implemented";
		break;
	}
}
void CGerberInterpolate::Inflate(CPart* pPart)
{
	pPart->Inflate(m_X, m_Y);
}
void CGerberInterpolate::Inflate(CRectangle* pRect)
{
	pRect->Inflate(m_X, m_Y);
}

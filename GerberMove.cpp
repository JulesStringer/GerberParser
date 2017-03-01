#include "GerberMove.h"
#include "GerberRender.h"
#include "Part.h"
#include "Rectangle.h"

CGerberMove::CGerberMove()
	: m_X(0)
	, m_Y(0)
{
}
CGerberMove::~CGerberMove()
{
}
CGerberMove::CGerberMove(double X, double Y, CGerberState* pState)
	: CGerberCommand(pState)
	, m_X(X)
	, m_Y(Y)
{
}
void CGerberMove::Render(CGerberRender* pRender, double dDeflate)
{
	pRender->MoveTo(m_X, m_Y, m_pState);
}
void CGerberMove::Inflate(CPart* pPart)
{
	pPart->Inflate(m_X, m_Y);
}
void CGerberMove::Inflate(CRectangle* pRect)
{
	pRect->Inflate(m_X, m_Y);
}

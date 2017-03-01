#include "GerberFlash.h"
#include "GerberAperature.h"
#include "GerberRender.h"

#include "GerberState.h"
#include "Rectangle.h"

CGerberFlash::CGerberFlash()
	: m_X(0)
	, m_Y(0)
	, m_pAperature(NULL)
{
}
CGerberFlash::~CGerberFlash()
{
}
CGerberFlash::CGerberFlash(double X, double Y, CGerberAperature* pAperature, CGerberState* pState)
	: CGerberCommand(pState)
	, m_X(X)
	, m_Y(Y)
	, m_pAperature(pAperature)
{

}
void CGerberFlash::Render(CGerberRender* pRender, double dDeflate)
{
	m_pAperature->Render(pRender, m_X, m_Y, m_pState, dDeflate);
	pRender->MoveTo(m_X, m_Y, m_pState);
}
void CGerberFlash::Inflate(CPart* pPart)
{
	m_pAperature->Inflate(pPart, m_X, m_Y);
}
void CGerberFlash::Inflate(CRectangle* pRect)
{
	m_pAperature->Inflate(pRect, m_X, m_Y);
}

#include "GerberAperatureMacro.h"



CGerberAperatureMacro::CGerberAperatureMacro()
	: m_pMacro(NULL)
{
}
CGerberAperatureMacro::CGerberAperatureMacro(CGerberMacro* pMacro)
	: m_pMacro(pMacro)
{
}
CGerberAperatureMacro::~CGerberAperatureMacro()
{
}
void CGerberAperatureMacro::Render(CGerberRender* pRender, double X, double Y, CGerberState* pState)
{
	throw "CGerberRenderMacro::Render not yet implemented";
}

// string needed for declaration of NULL
#include <string>
#include "DrillHole.h"

#include "DrillTool.h"
#include "GerberRender.h"
#include "GerberState.h"

CDrillHole::CDrillHole()
	: m_X(0)
	, m_Y(0)
	, m_pTool(NULL)
{
}
CDrillHole::CDrillHole(double X, double Y, CDrillTool* pTool)
	: m_X(X)
	, m_Y(Y)
	, m_pTool(pTool)
{
}
CDrillHole::~CDrillHole()
{
}
double CDrillHole::Diameter()
{
	if (m_pTool)
	{
		return m_pTool->Diameter();
	}
	return 0;
}
void CDrillHole::Render(CGerberRender* pRender, CGerberState* pState)
{
	double dRadius = 0.0;
	if (m_pTool)
	{
		dRadius = m_pTool->Diameter() / 2;
	}
	pRender->Circle(m_X, m_Y, dRadius, pState);
}

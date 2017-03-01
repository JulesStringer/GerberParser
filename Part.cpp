#include "Part.h"
#include "Rectangle.h"

#include "WKBGeometry.h"
#include "GerberRender.h"
#include "GerberAperature.h"
#include "GerberState.h"

CPart::CPart()
	: m_xmin(0)
	, m_ymin(0)
	, m_xmax(0)
	, m_ymax(0)
	, m_bFirst(true)
	, m_pState(NULL)
	, m_bInflated(false)
	, m_dInflateW(0.0)
	, m_dInflateH(0.0)
{
}
CPart::CPart(const char* pszName, CGerberState* pState)
	: m_xmin(0)
	, m_ymin(0)
	, m_xmax(0)
	, m_ymax(0)
	, m_bFirst(true)
	, m_pState(pState)
	, m_strName(pszName)
	, m_bInflated(false)
	, m_dInflateW(0.0)
	, m_dInflateH(0.0)
{
}
CPart::~CPart()
{
}
void CPart::Inflate(double X, double Y)
{
	if (m_bFirst)
	{
		m_xmin = m_xmax = X;
		m_ymin = m_ymax = Y;
		m_bFirst = false;
	}
	else
	{
		m_xmin = X < m_xmin ? X : m_xmin;
		m_ymin = Y < m_ymin ? Y : m_ymin;
		m_xmax = X > m_xmax ? X : m_xmax;
		m_ymax = Y > m_ymax ? Y : m_ymax;
	}
}
#if 0
void CPart::Inflate(CGerberCommand* pCommand)
{
	double X = pCommand->xmin();
	double Y = pCommand->ymin();
	Inflate(X, Y);
	X = p->xmax();
	Y = p->ymax();
	Inflate(X, Y);
}
#endif
const char* CPart::Name()
{
	return m_strName.c_str();
}
void CPart::Render(CGerberRender* pRender, CLayer* pLayer, double dInflate)
{
	if (!m_bFirst)
	{
		double dInflateW = dInflate;
		double dInflateH = dInflate;
		if (m_bInflated)
		{
			dInflateW = m_dInflateW;
			dInflateH = m_dInflateH;
		}
		// Add in inflation parameter
		m_pState->SetLayer(pLayer);
		CRectangle rc(m_xmin - dInflateW, m_ymin - dInflateH, m_xmax + dInflateW, m_ymax + dInflateH);
		pRender->Rectangle(&rc, m_pState);
	}
}
void CPart::SetInflate(double W, double H)
{
	m_dInflateW = W;
	m_dInflateH = H;
	m_bInflated = true;
}
#include "Rectangle.h"

#include "Layer.h"
#include "GerberRender.h"
#include "GerberState.h"

CRectangle::CRectangle()
	: m_minx(0)
	, m_miny(0)
    , m_maxx(0)
    , m_maxy(0)
    , m_bFirst(true)
{
}
CRectangle::CRectangle(double xmin, double ymin, double xmax, double ymax)
	: m_minx(xmin)
	, m_miny(ymin)
	, m_maxx(xmax)
	, m_maxy(ymax)
	, m_bFirst(false)
{}
CRectangle::~CRectangle()
{
}
void CRectangle::Inflate(double X, double Y)
{
	if (m_bFirst)
	{
		m_minx = m_maxx = X;
		m_miny = m_maxy = Y;
		m_bFirst = false;
	}
	else
	{
		m_minx = X < m_minx ? X : m_minx;
		m_miny = Y < m_miny ? Y : m_miny;
		m_maxx = X > m_maxx ? X : m_maxx;
		m_maxy = Y > m_maxy ? Y : m_maxy;
	}
}
double CRectangle::MinX() { return m_minx; }
double CRectangle::MinY() { return m_miny; }
double CRectangle::MaxX() { return m_maxx; }
double CRectangle::MaxY() { return m_maxy; }
void CRectangle::Render(CGerberRender* pRender, CLayer* pLayer, double dInflate)
{
	CGerberState State;
	State.SetLayer(pLayer);
	CRectangle rc(m_minx - dInflate, m_miny - dInflate, m_maxx + dInflate, m_maxy + dInflate);
	pRender->Rectangle(&rc, &State);
}

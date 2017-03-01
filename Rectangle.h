#pragma once
class CGerberRender;
class CLayer;

class CRectangle
{
	double m_minx;
	double m_miny;
	double m_maxx;
	double m_maxy;
	bool m_bFirst;
public:
	CRectangle();
	CRectangle(double xmin, double ymin, double xmax, double ymax);
	virtual ~CRectangle();
	void Inflate(double X, double Y);
	double MinX();
	double MinY();
	double MaxX();
	double MaxY();
	void Render(CGerberRender* pRender, CLayer* pLayer, double dInflate);
};


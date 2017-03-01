#pragma once

class CDrillTool;
class CGerberRender;
class CGerberState;


class CDrillHole
{
	double m_X;
	double m_Y;
	CDrillTool* m_pTool;
public:
	CDrillHole();
	CDrillHole(double X, double Y, CDrillTool* pTool);
	virtual ~CDrillHole();
	void Render(CGerberRender* pRender, CGerberState* pState);
	double Diameter();
};


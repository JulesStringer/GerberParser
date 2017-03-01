#pragma once
#include "GerberCommand.h"

class CGerberState;

class CGerberMove :
	public CGerberCommand
{
	double m_X;
	double m_Y;
public:
	CGerberMove();
	CGerberMove(double X, double Y, CGerberState* pState);
	virtual ~CGerberMove();
	virtual void Render(CGerberRender* pRender, double dDeflate);
	virtual void Inflate(CPart* pPart);
	virtual void Inflate(CRectangle* pRect);
};


#pragma once
#include "GerberCommand.h"
#include "GerberTypes.h"
#include "GerberState.h"
class CGerberInterpolate :
	public CGerberCommand
{
	double m_X;
	double m_Y;
	double m_I;
	double m_J;
public:
	CGerberInterpolate();
	CGerberInterpolate(double X, double Y, double I, double J, CGerberState* pState);
	virtual ~CGerberInterpolate();
	virtual void Render(CGerberRender* pRender, double dDeflate);
	virtual void Inflate(CPart* pPart);
	virtual void Inflate(CRectangle* pRect);
};


#pragma once
#include "GerberAperature.h"
class CGerberState;

class CGerberAperatureRectangle :
	public CGerberAperature
{
public:
	CGerberAperatureRectangle();
	virtual ~CGerberAperatureRectangle();
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState, double dDeflate);
	virtual void Inflate(CPart* pPart, double X, double Y);
	virtual void Inflate(CRectangle* pRect, double X, double Y);
};


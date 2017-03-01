#pragma once
#include "GerberAperature.h"

class CGerberState;

class CGerberAperatureObround :
	public CGerberAperature
{
public:
	CGerberAperatureObround();
	virtual ~CGerberAperatureObround();
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState);
};


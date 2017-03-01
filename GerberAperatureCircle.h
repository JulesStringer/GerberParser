#pragma once
#include "GerberAperature.h"

class CGerberState;

class CGerberAperatureCircle :
	public CGerberAperature
{
public:
	CGerberAperatureCircle();
	virtual ~CGerberAperatureCircle();
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState);
};


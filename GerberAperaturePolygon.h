#pragma once
#include "GerberAperature.h"

class CGerberState;

class CGerberAperaturePolygon :
	public CGerberAperature
{
public:
	CGerberAperaturePolygon();
	virtual ~CGerberAperaturePolygon();
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState);
};


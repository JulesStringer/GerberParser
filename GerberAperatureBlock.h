#pragma once
#include "GerberAperature.h"

class CGerberState;

class CGerberAperatureBlock :
	public CGerberAperature
{
public:
	CGerberAperatureBlock();
	virtual ~CGerberAperatureBlock();
	void Parse(CGerberParser* pParser);
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState);
};


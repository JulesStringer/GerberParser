#pragma once
#include "GerberAperature.h"

class CGerberMacro;
class CGerberState;

class CGerberAperatureMacro :
	public CGerberAperature
{
	CGerberMacro* m_pMacro;
public:
	CGerberAperatureMacro();
	CGerberAperatureMacro(CGerberMacro* pMacro);
	virtual ~CGerberAperatureMacro();
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState);
};


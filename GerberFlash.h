#pragma once
#include "GerberCommand.h"

class CGerberAperature;
class CGerberState;

class CGerberFlash :
	public CGerberCommand
{
	double m_X;
	double m_Y;
	CGerberAperature* m_pAperature;
public:
	CGerberFlash();
	CGerberFlash(double X, double Y, CGerberAperature* pAperature, CGerberState* pState);
	virtual ~CGerberFlash();
	virtual void Render(CGerberRender* pRender, double dDeflate);
	virtual void Inflate(CPart* pPart);
	virtual void Inflate(CRectangle* pRect);
};


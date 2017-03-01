#pragma once
#include "GerberCommand.h"

#include <vector>

class CGerberState;

class CGerberAperature :
	public CGerberCommand
{
protected:
	std::vector<double> m_vecArgs;
public:
	CGerberAperature();
	virtual ~CGerberAperature();
	static CGerberAperature* Create(const char* pszName, CGerberParser* pParser);
	virtual void Parse(CGerberParser* pParser);
	virtual void Render(CGerberRender* pRender, double X, double Y, CGerberState* pState, double dDeflate);
	virtual void Inflate(CPart* pPart, double X, double Y);
	virtual void Inflate(CRectangle* pRect, double X, double Y);
};


#pragma once

#include "InputStream.h"

class CGerberParser;
class CGerberState;
class CGerberRender;
class CPart;
class CRectangle;
class CLayer;

class CGerberCommand
{
protected:
	CGerberState* m_pState;
public:
	CGerberCommand();
	CGerberCommand(CGerberState* pState);
	virtual ~CGerberCommand();
	static CGerberCommand* Create(CGerberParser* pParser, CGerberState* pState);
	virtual void Render(CGerberRender* pRender, double dDeflate);
	virtual void Inflate(CPart* pPart);
	virtual void Inflate(CRectangle* pRect);
	bool MatchesLayer(CLayer* pLayer);
};


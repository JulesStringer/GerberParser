#pragma once
#include "GerberCommand.h"
class CGerberStepRepeat :
	public CGerberCommand
{
public:
	CGerberStepRepeat();
	virtual ~CGerberStepRepeat();
	void Parse(CGerberParser* pParser, CGerberState* pState);
};


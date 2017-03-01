#pragma once
#include "GerberCommand.h"
#include <vector>
#include "GerberTypes.h"
#include "GerberState.h"

class CGerberSegment :
	public CGerberCommand
{
	std::vector<CGerberCommand*> m_vecCommands;
public:
	CGerberSegment();
	virtual ~CGerberSegment();
	bool Parse(CGerberParser* pParser, CGerberState* pState);
};


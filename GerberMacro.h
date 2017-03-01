#pragma once
#include "GerberCommand.h"

class CVariable;
class CGerberPrimitive;

#include <map>
#include <vector>

class CGerberMacro :
	public CGerberCommand
{
	std::map<int, CVariable*> m_mapVariables;
	std::vector<CGerberPrimitive*> m_vecPrimitives;
public:
	CGerberMacro();
	virtual ~CGerberMacro();
	void Parse(CGerberParser* pParser);
	CVariable* GetVariable(int nVar);
};


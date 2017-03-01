#pragma once

#include "InputStream.h"
#include "Gerbermacro.h"
#include <vector>

class CExpression;

class CVariable
{
	CExpression* m_pExpression;
	unsigned int m_nIndex;
public:
	CVariable(int nIndex);
	~CVariable();
	void ParseExpression(CInputStream* pStream, CGerberMacro* pMacro);
	double GetValue(std::vector<double>& vecArgs);
};

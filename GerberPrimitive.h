#pragma once

class CExpression;
#include <vector>

class CGerberPrimitive
{
	std::vector<CExpression*> m_vecModifiers;
public:
	CGerberPrimitive();
	~CGerberPrimitive();
	void AddModifier(CExpression* pExpression);
	static CGerberPrimitive* Create(int mPrimitive);
};


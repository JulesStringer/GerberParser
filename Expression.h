#pragma once

class CGerberMacro;
#include <vector>

class CVariable;

class CExpression
{
	char m_Operator;
	double m_dValue;
	CVariable* m_pVariable;
	CExpression* m_pOp1;
	CExpression* m_pOp2;
	std::vector<CExpression*> m_vecOperators;
	std::vector<CExpression*> m_vecOperands;
	void PushPop(char Op);
public:
	CExpression();
	CExpression(CVariable* pVar);
	CExpression(double d);
	CExpression(char Operator);
	CExpression(char Operator, CExpression* pOp1, CExpression* pOp2);
	virtual ~CExpression();
	void Parse(const char* pszExpression, CGerberMacro* pMacro);
	double GetValue(std::vector<double>& vecArgs);
	static int Priority(char Operator);
	int Priority();
	void SetOp1(CExpression* pExpression);
	void SetOp2(CExpression* pExpression);
	void SetOp(char ch);
};


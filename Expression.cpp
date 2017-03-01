#include "Expression.h"
#include "GerberMacro.h"
#include "Variable.h"

CExpression::CExpression()
	: m_Operator('L')
	, m_dValue(0)
	, m_pVariable(NULL)
	, m_pOp1(NULL)
	, m_pOp2(NULL)
{
}
CExpression::CExpression(CVariable* pVar)
	: m_Operator('V')
	, m_dValue(0)
	, m_pVariable(pVar)
	, m_pOp1(NULL)
	, m_pOp2(NULL)
{}
CExpression::CExpression(double d)
	: m_Operator('L')
	, m_dValue(d)
	, m_pVariable(NULL)
	, m_pOp1(NULL)
	, m_pOp2(NULL)
{}
CExpression::CExpression(char Op)
	: m_Operator(Op)
	, m_dValue(0.0)
	, m_pVariable(NULL)
	, m_pOp1(NULL)
	, m_pOp2(NULL)
{}
CExpression::CExpression(char Operator, CExpression* pOp1, CExpression* pOp2)
	: m_Operator(Operator)
	, m_dValue(0)
	, m_pVariable(NULL)
	, m_pOp1(pOp1)
	, m_pOp2(pOp2)
{}
CExpression::~CExpression()
{
}
int CExpression::Priority(char Operator)
{
	switch (Operator)
	{
	case '+':
	case '-':
		return 1;
	case 'x':
	case '/':
		return 2;
	case '(':
	case ')':
		return 3;
	case 0:
		return 0;
	}
	return 0;
}
int CExpression::Priority()
{
	return Priority(m_Operator);
}
void CExpression::PushPop(char Op)
{
	while (m_vecOperators.size() > 0 && m_vecOperators.back()->Priority() >= Priority(Op))
	{
		CExpression* pExp = m_vecOperators.back();
		m_vecOperators.pop_back();
		CExpression* pOp2 = m_vecOperands.back();
		m_vecOperands.pop_back();
		CExpression* pOp1 = m_vecOperands.back();
		m_vecOperands.pop_back();
		pExp->SetOp1(pOp1);
		pExp->SetOp2(pOp2);
		m_vecOperands.push_back(pExp);
	}
	if (Op != ')' && Op != 0)
	{
		CExpression* pExpression = new CExpression(Op);
		m_vecOperators.push_back(pExpression);
	}
}
void CExpression::Parse(const char* pszExpression, CGerberMacro* pMacro)
{
	// arithmetic operators are:
	//  +
	//  -
	//  x
	//  /
	// Brackets are used to enforce priority
	// operands can be decimals or variables
	const char* psz = pszExpression;

//	std::vector<char> vecOperators;
//	std::vector<CExpression*> vecOperands;

	bool bSign = false;
	bool bUnary = true;
	while (*psz)
	{
		if (*psz == '$')
		{
			int nVar = 0;
			psz++;
			while (isdigit(*psz))
			{
				nVar = nVar * 10 + *psz - '0';
				psz++;
			}
			CVariable* pVar = pMacro->GetVariable(nVar);
			CExpression* pExp = new CExpression(pVar);
			if (bSign)
			{
				pExp->SetOp('U');
			}
			m_vecOperands.push_back(pExp);
			bUnary = false;
			bSign = false;
		}
		else if (*psz == '+')
		{
			PushPop('+');
			psz++;
			bUnary = true;
		}
		else if (*psz == '-')
		{
			// Need to distinguish unary and binary -
			// if preceding character is an operator not ) then it is unary.
			if (bUnary)
			{
				bSign = true;
			}
			else
			{
				PushPop('-');
			}
			psz++;
			bUnary = false;
		}
		else if (*psz == 'x')
		{
			PushPop('x');
			psz++;
			bUnary = false;
		}
		else if (*psz == '/')
		{
			PushPop('/');
			psz++;
			bUnary = false;
		}
		else if (*psz == '(')
		{
			// remove ?? priority operators and operands
			PushPop('(');
			psz++;
			bUnary = true;
		}
		else if (*psz == ')')
		{
			PushPop(')');
			psz++;
			bUnary = false;
		}
		else if (isdigit(*psz))
		{
			double d = 0;
			while (isdigit(*psz))
			{
				d = d * 10 + *psz - '0';
				psz++;
			}
			if (*psz == '.')
			{
				psz++;
				double dFactor = 0.1;
				while (isdigit(*psz))
				{
					d += (*psz - '0') * dFactor;
					dFactor *= 0.1;
					psz++;
				}
			}
			if (bSign)
			{
				d = -d;
			}
			CExpression* pExp = new CExpression(d);
			m_vecOperands.push_back(pExp);
			bSign = false;
			bUnary = false;
		}
		//psz++;
	}
	// operator stack should now be empty and there should be one operand
	// this operand now replaces expression.
	PushPop(0);
	if (m_vecOperands.size() != 1 || m_vecOperators.size())
	{
		throw "Invalid expression";
	}
	CExpression* pOperand = m_vecOperands[0];
	m_Operator = pOperand->m_Operator;
	m_dValue = pOperand->m_dValue;
	m_pOp1 = pOperand->m_pOp1;
	m_pOp2 = pOperand->m_pOp2;
	m_pVariable = pOperand->m_pVariable;
	m_vecOperands.clear();
	delete pOperand;
}
double CExpression::GetValue(std::vector<double>& vecArgs)
{
	switch (m_Operator)
	{
	case 'L':
		return m_dValue;
	case 'V':
		return m_pVariable->GetValue(vecArgs);
	case '+':
		return m_pOp1->GetValue(vecArgs) + m_pOp2->GetValue(vecArgs);
	case '-':
		return m_pOp1->GetValue(vecArgs) - m_pOp2->GetValue(vecArgs);
	case 'U':
		return -m_pVariable->GetValue(vecArgs);
	case 'x':
		return m_pOp1->GetValue(vecArgs) * m_pOp2->GetValue(vecArgs);
	case '/':
		return m_pOp1->GetValue(vecArgs) + m_pOp2->GetValue(vecArgs);
	}
	throw "Expression::GetValue invalid operator";
}
void CExpression::SetOp1(CExpression* pOp1)
{
	m_pOp1 = pOp1;
}
void CExpression::SetOp2(CExpression* pOp2)
{
	m_pOp2 = pOp2;
}
void CExpression::SetOp(char ch)
{
	m_Operator = ch;
}
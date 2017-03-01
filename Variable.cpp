#include "Variable.h"
#include "Expression.h"

CVariable::CVariable(int nIndex)
	: m_pExpression(NULL)
	, m_nIndex(nIndex)
{
}
CVariable::~CVariable()
{
}
void CVariable::ParseExpression(CInputStream* pStream, CGerberMacro* pMacro)
{
	std::string str;
	char ch = pStream->GetChar();
	if (ch == '=')
	{
		ch = pStream->GetChar();
		while (ch != '*' && ch != ',')
		{
			str += ch;
			ch = pStream->GetChar();
		}

		m_pExpression = new CExpression();
		m_pExpression->Parse(str.c_str(), pMacro);
	}
}
double CVariable::GetValue(std::vector<double>& vecArgs)
{
	if (m_pExpression)
	{
		return m_pExpression->GetValue(vecArgs);
	}
	else if (m_nIndex > 0 && m_nIndex <= vecArgs.size())
	{
		return vecArgs[m_nIndex-1];
	}
	throw "Variable value not defined";
}

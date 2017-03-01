#include "GerberMacro.h"
#include "GerberParser.h"
#include "InputStream.h"
#include "Variable.h"
#include "GerberPrimitive.h"
#include "Expression.h"

CGerberMacro::CGerberMacro()
{
}
CGerberMacro::~CGerberMacro()
{
}
void CGerberMacro::Parse(CGerberParser* pParser)
{
	CInputStream* pStream = pParser->Stream();
	while (!pStream->AtEndExtended())
	{
		char ch = pStream->PeekChar();
		if (ch == '$')
		{
			pStream->GetChar();
			unsigned int nVar = pStream->GetInt();
			// Variable definition
			CVariable* pVariable = new CVariable(nVar);
			pVariable->ParseExpression(pStream, this);
			m_mapVariables[nVar] = pVariable;
		}
		else if ( isdigit(ch))
		{
			unsigned int nPrimitive = pStream->GetInt();
			if (nPrimitive == 0)
			{
				pStream->SkipToEndBlock();
			}
			else
			{
				CGerberPrimitive* pPrimitive = CGerberPrimitive::Create(nPrimitive);
				// Get modifiers
				while (pStream->PeekChar() != '*')
				{
					ch = pStream->GetChar();
					if (ch == ',')
					{
						ch = pStream->GetChar();
					}
					std::string strModifier;
					while (ch != ',' && ch != '*' && !pStream->AtEOF())
					{
						strModifier += ch;
						ch = pStream->PeekChar();
						if (ch != '*')
						{
							ch = pStream->GetChar();
						}
					}
					CExpression* pExpression = new CExpression();
					pExpression->Parse(strModifier.c_str(), this);
					pPrimitive->AddModifier(pExpression);
				}
				m_vecPrimitives.push_back(pPrimitive);
			}
		}
		else
		{
			ch = pStream->GetChar();
		}
	}
	pStream->SkipToEndExtended();
}
CVariable* CGerberMacro::GetVariable(int nVar)
{
	std::map<int, CVariable*>::iterator it = m_mapVariables.find(nVar);
	if (it == m_mapVariables.end())
	{
		CVariable* pVar = new CVariable(nVar);
		m_mapVariables[nVar] = pVar;
		return pVar;
	}
	else
	{
		return it->second;
	}
	return NULL;
}
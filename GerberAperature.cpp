#include "GerberAperature.h"

#include "GerberParser.h"
#include "InputStream.h"
#include "GerberAperatureCircle.h"
#include "GerberAperatureMacro.h"
#include "GerberAperatureObround.h"
#include "GerberAperaturePolygon.h"
#include "GerberAperatureRectangle.h"

CGerberAperature::CGerberAperature()
{
}


CGerberAperature::~CGerberAperature()
{
}
CGerberAperature* CGerberAperature::Create(const char* pszName, CGerberParser* pParser)
{
	// arguments are being separated by X
	if (strcmp(pszName, "C") == 0 )
	{
		return new CGerberAperatureCircle();
	}
	else if (strcmp(pszName, "R") == 0 )
	{
		return new CGerberAperatureRectangle();
	}
	else if (strcmp(pszName, "O") == 0 )
	{
		return new CGerberAperatureObround();
	}
	else if (strcmp( pszName, "P") == 0 )
	{
		return new CGerberAperaturePolygon();
	}
	else
	{
		// Macro defined aperature
		CGerberMacro* pMacro = pParser->GetMacro(pszName);
		if (!pMacro)
		{
			throw "CGerberAperature::Create macro not found";
		}
		return new CGerberAperatureMacro(pMacro);
	}
}
void CGerberAperature::Parse(CGerberParser* pParser)
{
	// TODO form vector of arguments to pass to macro / standard primitive
	// form vector of evaluated primitives
	CInputStream* pStream = pParser->Stream();
	char ch = pStream->PeekChar();
	if (ch == ',')
	{
		ch = pStream->GetChar();
	}
	while (ch != '*' && !pStream->AtEOF())
	{
		double d = pStream->GetSize();
		ch = pStream->PeekChar();
		if (ch == 'X' )
		{
			ch = pStream->GetChar();
		}
		m_vecArgs.push_back(d);
	}
}
void CGerberAperature::Render(CGerberRender* pRender, double X, double Y, CGerberState* pState, double dDeflate)
{
	throw "CGerberRender::Render not implements in derived class, base implementation called";
}
void CGerberAperature::Inflate(CPart* pPart, double X, double Y)
{
	throw "CGerberAperature::Inflate not implemented in derived class, base implementation called";
}
void CGerberAperature::Inflate(CRectangle* pRect, double X, double Y)
{
	throw "CGerberAperature::Inflate not implemented in derived class, base implementation called";
}
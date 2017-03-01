#include "GerberSegment.h"
#include "GerberParser.h"
#include "InputStream.h"
#include "GerberEndSegment.h"

CGerberSegment::CGerberSegment()
{
}
CGerberSegment::~CGerberSegment()
{
}
bool CGerberSegment::Parse(CGerberParser* pParser, CGerberState* pState)
{
	CInputStream* pStream = pParser->Stream();
	while (!pStream->AtEOF())
	{
		CGerberCommand* pCommand = CGerberCommand::Create(pParser, pState);
		if (pCommand)
		{
			m_vecCommands.push_back(pCommand);
			// Is this G37 ?
			CGerberEndSegment* pEnd = dynamic_cast<CGerberEndSegment*>(pCommand);
			if (pEnd)
			{
				return true;
			}
		}
	}
	return false;
}